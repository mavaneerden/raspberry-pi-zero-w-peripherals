#pragma once

#include <stdexcept>

namespace pi_zero_peripherals
{

class i2c_bus_exception: public std::runtime_error
{
public:
    i2c_bus_exception(const std::string& message);
};

class i2c_read_exception: public std::runtime_error
{
public:
    i2c_read_exception(const std::string& message);
};

class i2c_write_exception: public std::runtime_error
{
public:
    i2c_write_exception(const std::string& message);
};

} /* pi_zero_peripherals */