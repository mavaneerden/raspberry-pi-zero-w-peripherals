#pragma once

#include <thread>
#include "socket_client.hpp"

namespace pi_zero_peripherals
{

class socket_server_t
{
public:
    socket_server_t();
    ~socket_server_t();
private:
    static constexpr uint8_t MAX_CLIENTS = 5u;

    std::thread listening_thread;
    socket_client_t clients[MAX_CLIENTS];

    void server_loop();
};

} /* pi_zero_peripherals */
