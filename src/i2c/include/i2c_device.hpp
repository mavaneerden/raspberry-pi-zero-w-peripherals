#pragma once

#include "i2c_bus.hpp"
#include "../../../lib/libi2c/i2c.h"

namespace pi_zero_peripherals
{

class i2c_device_t
{
public:
    i2c_device_t(i2c_bus_t& bus, uint8_t address, uint8_t internal_address_bytes = 0u, uint16_t flags = 0u);

    void i2c_read(uint8_t* buffer, uint8_t size, uint32_t internal_address = 0u);
    void i2c_write(uint8_t* data, uint8_t size, uint32_t internal_address = 0u);

    uint16_t flags;
protected:
    i2c_bus_t& bus;
    I2CDevice device;
};

} /* pi_zero_peripherals */