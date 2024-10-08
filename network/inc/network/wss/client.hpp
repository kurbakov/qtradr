#pragma once

#include "network/tls/client.hpp"

namespace network::wss
{
/**
 * WebSocker Secure Client
 *
 * Standard:
 * https://www.rfc-editor.org/rfc/rfc6455
 */
class WssClient
{
public:
private:
    ::network::tls::Client m_socket;
};
} // namespace network::wss