/**
 * @file i2c_bus.cpp
 * @author Marco van Eerden (mavaneerden@gmail.com)
 * @brief Contains the i2c_bus_t class that represents an I2C bus in user space.
 *        The bus number is used to open the correct character device file.
 * @date 06-04-2022
 */

#include <assert.h>
#include <iostream>
#include <string>

#include "../../lib/libi2c/i2c.h"
#include "include/i2c_bus.hpp"
#include "include/i2c_exception.hpp"

using namespace pi_zero_peripherals;

/* Instantiate I2C buses. */
i2c_bus_t i2c(1u);
i2c_bus_t i2c_eeprom(2u);

/**
 * @brief Construct a new i2c_bus_t object.
 *
 * @param bus_number Number that uniquely identifies the I2C bus.
 */
i2c_bus_t::i2c_bus_t(uint8_t bus_number) :
    initialised(0u),
    bus_number(bus_number),
    bus_fd(-1)
{}

/**
 * @brief Destroy the i2c_bus_t object.
 * Closes the I2C character device file.
 */
i2c_bus_t::~i2c_bus_t()
{
    if (initialised == 1u)
    {
        i2c_close(this->bus_fd);
    }
}

/**
 * @brief Initialises the I2C bus by opening the file and storing the file descriptor.
 */
void i2c_bus_t::initialise()
{
    assert(initialised == 0u);

    std::string file_name = "/dev/i2c-" + std::to_string(bus_number);

    this->bus_fd = i2c_open(file_name.data());

    if(this->bus_fd == -1)
    {
        throw i2c_bus_exception("Could not open i2c bus " + std::to_string(bus_number));
    }

    this->initialised = 1u;
}
