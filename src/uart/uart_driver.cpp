/**
 * @file uart_driver.cpp
 * @author Marco van Eerden (mavaneerden@gmail.com)
 * @brief This file contains the uart_driver_t class that can be used to
 *        communicate with a device over a serial UART port.
 * @date 07-04-2022
 *
 */

#include <assert.h>
#include <string>
#include "include/uart_driver.hpp"

using namespace pi_zero_peripherals;

/**
 * @brief Construct a new uart_driver_t object.
 *
 * @param bus_number UART bus number.
 * @param baud_rate  Baud rate used for the device.
 */
uart_driver_t::uart_driver_t(uint8_t bus_number, uint32_t baud_rate) :
    device{
        .filename = ("/dev/ttyS" + std::to_string(bus_number)).data(),
        .rate     = baud_rate
    },
    initialised(false)
{}

/**
 * @brief Destroy the uart_driver_t object.
 * Closes the file descriptor and frees the tty pointer.
 */
uart_driver_t::~uart_driver_t()
{
    if (this->initialised)
    {
        uart_stop(&this->device);
    }
}

/**
 * @brief Initialises the UART driver.
 */
void uart_driver_t::initialise()
{
    assert(!this->initialised);

    int8_t status = uart_start(&this->device, false);

    if (status != 0)
    {
        // TODO: handle UART start failure.
    }
    else
    {
        this->initialised == 1u;
    }
}

/**
 * @brief Writes data to the UART serial bus.
 *
 * @param data Data to write.
 * @param size Size of the data in bytes.
 */
void uart_driver_t::write(uint8_t* data, uint8_t size)
{
    int8_t status = uart_writen(&this->device, data, size);

    if (status < 0)
    {
        // TODO: handle UART write failure.
    }
}

/**
 * @brief Reads data from the UART serial bus.
 *
 * @param buffer Buffer to store the read data into.
 * @param size   Size of the data to read.
 */
void uart_driver_t::read(uint8_t* buffer, uint8_t size)
{
    int8_t status = uart_readn(&this->device, buffer, size);

    if (status < 0)
    {
        // TODO: handle UART read failure.
    }
}
