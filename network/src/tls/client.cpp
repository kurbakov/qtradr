#include "network/tls/client.hpp"

namespace network::tls
{
int Client::try_connect()
{
    int ret;

    m_ctx = SSL_CTX_new(TLS_client_method());
    SSL_CTX_set_verify(m_ctx, SSL_VERIFY_PEER, nullptr);

    if (!SSL_CTX_set_default_verify_paths(m_ctx))
    {
        std::cerr << "Failed to set the default trusted certificate store" << std::endl;
        return -1;
    }

    if (!SSL_CTX_set_min_proto_version(m_ctx, TLS1_2_VERSION))
    {
        std::cerr << "Failed to set the minimum TLS protocol version" << std::endl;
        return -1;
    }

    m_ssl = SSL_new(m_ctx);
    if (m_ssl == nullptr)
    {
        std::cerr << "Failed to create the SSL object" << std::endl;
        return -1;
    }

    /*
     * Create the underlying transport socket/BIO and associate it with the
     * connection.
     */
    BIO *bio = create_socket_bio(m_hostname.c_str(), m_port.c_str(), AF_INET);
    if (bio == nullptr)
    {
        bio = create_socket_bio(m_hostname.c_str(), m_port.c_str(), AF_INET6);
        if (bio == nullptr)
        {
            std::cerr << "Failed to crete the BIO" << std::endl;
            return -1;
        }
    }
    SSL_set_bio(m_ssl, bio, bio);

    /*
     * Tell the server during the handshake which hostname we are attempting
     * to connect to in case the server supports multiple hosts.
     */
    if (!SSL_set_tlsext_host_name(m_ssl, m_hostname.c_str()))
    {
        std::cerr << "Failed to set the SNI hostname" << std::endl;
        return -1;
    }

    /*
     * Ensure we check during certificate verification that the server has
     * supplied a certificate for the hostname that we were expecting.
     * Virtually all clients should do this unless you really know what you
     * are doing.
     */
    if (!SSL_set1_host(m_ssl, m_hostname.c_str()))
    {
        std::cerr << "Failed to set the certificate verification hostname" << std::endl;
        return -1;
    }

    /* Do the handshake with the server */
    while ((ret = SSL_connect(m_ssl)) != 1)
    {
        if (handle_io_failure(m_ssl, ret) == 1)
            continue; /* Retry */
        printf("Failed to connect to server\n");
        return -1;
    }

    return 0;
}

int Client::write(const char *data, const int len) const
{
    int sent = 0;
    while (sent < len)
    {
        size_t written;
        if (1 == SSL_write_ex(m_ssl, data, len, &written))
        {
            sent += written;
        }
        else
        {
            return -1;
        }
    }

    return sent;
}

int Client::read(char *buf, const int len) const { return SSL_read(m_ssl, buf, len); }

BIO *Client::create_socket_bio(const char *hostname, const char *port, int family)
{
    int sock = -1;

    BIO_ADDRINFO *res = nullptr;
    if (BIO_lookup_ex(hostname, port, BIO_LOOKUP_CLIENT, family, SOCK_STREAM, 0, &res) == 0)
    {
        std::cerr << "Failed to find the IP addr" << std::endl;
        return nullptr;
    }

    /*
     * Loop through all the possible addresses for the server and find one
     * we can connect to.
     */
    for (const BIO_ADDRINFO *ai = res; ai != nullptr; ai = BIO_ADDRINFO_next(ai))
    {
        /*
         * Create a TCP socket. We could equally use non-OpenSSL calls such
         * as "socket" here for this and the subsequent connect and close
         * functions. But for portability reasons and also so that we get
         * errors on the OpenSSL stack in the event of a failure we use
         * OpenSSL's versions of these functions.
         */
        sock = BIO_socket(BIO_ADDRINFO_family(ai), SOCK_STREAM, 0, 0);

        if (sock == -1)
            continue;

        /* Connect the socket to the server's address */
        if (!BIO_connect(sock, BIO_ADDRINFO_address(ai), BIO_SOCK_NODELAY))
        {
            BIO_closesocket(sock);
            sock = -1;
            continue;
        }

        /* Set to nonblocking mode */
        if (!BIO_socket_nbio(sock, 1))
        {
            sock = -1;
            continue;
        }

        /* We have a connected socket so break out of the loop */
        break;
    }

    /* Free the address information resources we allocated earlier */
    BIO_ADDRINFO_free(res);

    /* If sock is -1 then we've been unable to connect to the server */
    if (sock == -1)
        return nullptr;

    /* Create a BIO to wrap the socket */
    BIO *bio = BIO_new(BIO_s_socket());
    if (bio == nullptr)
    {
        BIO_closesocket(sock);
        return nullptr;
    }

    /*
     * Associate the newly created BIO with the underlying socket. By
     * passing BIO_CLOSE here the socket will be automatically closed when
     * the BIO is freed. Alternatively you can use BIO_NOCLOSE, in which
     * case you must close the socket explicitly when it is no longer
     * needed.
     */
    BIO_set_fd(bio, sock, BIO_CLOSE);

    return bio;
}

int Client::handle_io_failure(SSL *ssl, int res)
{
    switch (SSL_get_error(ssl, res))
    {
    case SSL_ERROR_WANT_READ:
        /* Temporary failure. Wait until we can read and try again */
        // wait_for_activity(ssl, 0);
        return 1;

    case SSL_ERROR_WANT_WRITE:
        /* Temporary failure. Wait until we can write and try again */
        // wait_for_activity(ssl, 1);
        return 1;

    case SSL_ERROR_ZERO_RETURN:
        /* EOF */
        return 0;

    case SSL_ERROR_SYSCALL:
        return -1;

    case SSL_ERROR_SSL:
        /*
         * If the failure is due to a verification error we can get more
         * information about it from SSL_get_verify_result().
         */
        if (SSL_get_verify_result(ssl) != X509_V_OK)
        {
            std::cerr << "Verify error: " << X509_verify_cert_error_string(SSL_get_verify_result(ssl)) << std::endl;
        }

        return -1;

    default:
        return -1;
    }
}

} // namespace network::tls