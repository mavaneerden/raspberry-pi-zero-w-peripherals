/**
 * @file i2c_device.cpp
 * @author Marco van Eerden (mavaneerden@gmail.com)
 * @brief Contains various exceptions for I2C operations.
 * @date 06-04-2022
 */

#include "include/i2c_exception.hpp"

using namespace pi_zero_peripherals;

/**
 * @brief Construct a new i2c_bus_exception object. Used when a character device file for an I2C bus cannot be opened.
 *
 * @param message Error message.
 */
i2c_bus_exception::i2c_bus_exception(const std::string& message) :
    runtime_error(message)
{}

/**
 * @brief Construct a new i2c_read_exception object. Used when unable to read via I2C.
 *
 * @param message Error message.
 */
i2c_read_exception::i2c_read_exception(const std::string& message) :
    runtime_error(message)
{}

/**
 * @brief Construct a new i2c_write_exception object. Used when unable to write via I2C.
 *
 * @param message Error message.
 */
i2c_write_exception::i2c_write_exception(const std::string& message) :
    runtime_error(message)
{}
