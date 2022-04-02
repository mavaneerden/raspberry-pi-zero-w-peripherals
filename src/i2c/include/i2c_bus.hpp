#pragma once

#include <stdint.h>

namespace pi_zero_peripherals
{

class i2c_bus_t
{
public:
    i2c_bus_t(uint8_t bus_number);
    ~i2c_bus_t();

    void initialise();

    uint8_t initialised;
    int bus_fd;
private:
    uint8_t bus_number;
};

} /* pi_zero_peripherals */