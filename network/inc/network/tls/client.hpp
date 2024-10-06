#pragma once

#include <iostream>
#include <string>
#include <sys/socket.h>

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

namespace network::tls
{
class Client
{
public:
    Client(const std::string &hostname, const int port)
        : m_ctx(nullptr), m_ssl(nullptr), m_hostname(hostname), m_port(std::to_string(port))
    {
    }

    ~Client()
    {
        /*
         * Free the resources we allocated. We do not free the BIO object here
         * because ownership of it was immediately transferred to the SSL object
         * via SSL_set_bio(). The BIO will be freed when we free the SSL object.
         */
        SSL_free(m_ssl);
        SSL_CTX_free(m_ctx);
    }

    int try_connect();

    int write(const char *data, int len) const;
    int read(char *buf, int len) const;

private:
    static BIO *create_socket_bio(const char *hostname, const char *port, int family);
    static int handle_io_failure(SSL *ssl, int res);

    SSL_CTX *m_ctx;
    SSL *m_ssl;
    const std::string m_hostname;
    const std::string m_port;
};
} // namespace network::tls