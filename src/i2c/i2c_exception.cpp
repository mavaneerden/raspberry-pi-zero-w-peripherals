#include "include/i2c_exception.hpp"

using namespace pi_zero_peripherals;

i2c_bus_exception::i2c_bus_exception(const std::string& message) :
    runtime_error(message)
{}

i2c_read_exception::i2c_read_exception(const std::string& message) :
    runtime_error(message)
{}

i2c_write_exception::i2c_write_exception(const std::string& message) :
    runtime_error(message)
{}
