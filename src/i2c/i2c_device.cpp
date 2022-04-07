/**
 * @file i2c_device.cpp
 * @author Marco van Eerden (mavaneerden@gmail.com)
 * @brief Contains the i2c_device_t class that represents an I2C slave device.
 *        We can read/write from/to the device using the functions in libi2c.
 * @date 06-04-2022
 */

#include <assert.h>
#include <string>

#include "include/i2c_device.hpp"
#include "include/i2c_exception.hpp"

using namespace pi_zero_peripherals;

/**
 * @brief Construct a new i2c_device_t with a 7-bit address.
 *
 * @param bus I2C bus that the device is on.
 * @param address Slave address of the device.
 * @param internal_address_bytes Number of bytes used to address internal device registers (default: 0u).
 * @param flags I2C flags (default: 0u).
 */
i2c_device_t::i2c_device_t(i2c_bus_t& bus, uint8_t address, uint8_t internal_address_bytes, uint16_t flags) :
    bus(bus),
    flags(flags),
    device({
        .bus         = bus.bus_fd,
        .addr        = address,
        .tenbit      = 0u,
        .delay       = 0u,
        .flags       = flags,
        .page_bytes  = 8u,
        .iaddr_bytes = internal_address_bytes
    })
{}

/**
 * @brief Read from the I2C device using libi2c.
 *
 * @param buffer Buffer to store the read data into.
 * @param size Size of the data to read.
 * @param internal_address Address of the internal register to read from (default: 0u).
 */
void i2c_device_t::i2c_read(uint8_t* buffer, uint8_t size, uint32_t internal_address)
{
    assert(this->bus.initialised == 1u);

    /* Set device struct flags. */
    this->device.flags = this->flags;
    /* Set device struct bus. */
    this->device.bus = this->bus.bus_fd;

    /* Read from I2C device into buffer. */
    if(i2c_ioctl_read(&this->device, internal_address, buffer, size) == -1)
    {
        throw i2c_read_exception("unable to read from I2C device");
    }
}

/**
 * @brief Write to the I2C device using libi2c.
 *
 * @param data Data to write to the device.
 * @param size Size of the data to write.
 * @param internal_address Address of the internal register to write to (default: 0u).
 */
void i2c_device_t::i2c_write(uint8_t* data, uint8_t size, uint32_t internal_address)
{
    assert(this->bus.initialised == 1u);

    /* Set device struct flags. */
    this->device.flags = this->flags;
    /* Set device struct bus. */
    this->device.bus = this->bus.bus_fd;

    /* Write data from I2C device. */
    if(i2c_ioctl_write(&this->device, internal_address, data, size) == -1)
    {
        throw i2c_write_exception("unable to write to I2C device");
    }
}
