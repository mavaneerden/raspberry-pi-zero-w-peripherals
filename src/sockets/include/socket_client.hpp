#pragma once

#include <string>
#include <thread>
#include <mutex>
#include <unistd.h>

namespace pi_zero_peripherals
{

class socket_client_t
{
public:
    socket_client_t(std::string client_address, uint16_t client_port, int32_t client_socket);
    ~socket_client_t();

    void client_read_blocking(uint8_t* buffer, uint8_t size);
    void client_read_non_blocking(uint8_t* buffer, uint8_t size);
    void client_write(uint8_t* data, uint8_t size);
private:
    static constexpr uint8_t MAX_MESSAGE_LENGTH = 255u;

    std::string client_address;
    uint16_t client_port;
    int32_t client_socket;

    std::vector<std::thread> read_threads;

    void client_loop();
};

} /* pi_zero_peripherals */
