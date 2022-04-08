#include "socket_client.hpp"

using namespace pi_zero_peripherals;

socket_client_t::socket_client_t(std::string client_address, uint16_t client_port, int32_t client_socket) :
    client_address(client_address),
    client_port(client_port),
    client_socket(client_socket),
    thread(&socket_client_t::client_loop, this)
{}

void socket_client_t::client_loop()
{
    while (1u)
    {
        printf("SDOIFJDOSI\n");
    }
}

void socket_client_t::client_read_non_blocking(uint8_t* buffer, uint8_t size)
{
    this->read_threads.push_back(std::thread(&socket_client_t::client_loop, this));
}

void socket_client_t::client_read_blocking(uint8_t* buffer, uint8_t size)
{
    read(this->client_socket, buffer, size);
}
