#include <assert.h>
#include <iostream>
#include <string>

#include "../../lib/libi2c/i2c.h"
#include "include/i2c_bus.hpp"
#include "include/i2c_exception.hpp"

using namespace pi_zero_peripherals;

i2c_bus_t::i2c_bus_t(uint8_t bus_number) :
    initialised(0u),
    bus_number(bus_number),
    bus_fd(-1)
{}

i2c_bus_t::~i2c_bus_t()
{
    if (initialised == 1u)
    {
        i2c_close(this->bus_fd);
    }
}

void i2c_bus_t::initialise()
{
    assert(initialised == 0u);
    assert(bus_number == 1u || bus_number == 2u);

    std::string file_name = "/dev/i2c-" + std::to_string(bus_number);

    this->bus_fd = i2c_open(file_name.data());

    printf("%d", this->bus_fd);

    if(this->bus_fd == -1)
    {
        throw i2c_bus_exception("Could not open i2c bus " + std::to_string(bus_number));
    }

    this->initialised = 1u;
}
