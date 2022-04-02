#include <assert.h>
#include <string>

#include "include/i2c_device.hpp"
#include "include/i2c_exception.hpp"

using namespace pi_zero_peripherals;

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

void i2c_device_t::i2c_read(uint8_t* buffer, uint8_t size, uint8_t internal_address)
{
    assert(this->bus.initialised == 1u);

    /* Set device struct flags. */
    this->device.flags = this->flags;
    this->device.bus = this->bus.bus_fd;

    /* Read from I2C device into buffer. */
    if(i2c_ioctl_read(&this->device, internal_address, buffer, size) == -1)
    {
        throw i2c_read_exception("unable to read from I2C device");
    }
}

void i2c_device_t::i2c_write(uint8_t* data, uint8_t size, uint8_t internal_address)
{
    assert(this->bus.initialised == 1u);

    /* Set device struct flags. */
    this->device.flags = this->flags;
    this->device.bus = this->bus.bus_fd;

    /* Write data from I2C device. */
    if(i2c_ioctl_write(&this->device, internal_address, data, size) == -1)
    {
        throw i2c_write_exception("unable to write to I2C device");
    }
}
