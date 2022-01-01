/**
 * @file gpio_pin.hpp
 * @author Marco van Eerden (mavaneerden@gmail.com)
 * @date 2021-12-31
 *
 * @copyright Copyright (c) 2021 Marco van Eerden
 */

#pragma once

#include <gpiod.hpp>
#include <stdint.h>

namespace pi_zero_peripherals
{

/* GPIO pin states. */
enum : uint8_t
{
    GPIO_STATE_LOW  = 0u,
    GPIO_STATE_HIGH = 1u
};

/* GPIO configuration. */
struct gpio_config_t
{
    uint8_t direction    = GPIOD_LINE_DIRECTION_INPUT;
    uint8_t bias         = GPIOD_LINE_BIAS_AS_IS;
    uint8_t output_value = 0u;
};

/**
 * @brief Represents a single GPIO pin.
 */
class gpio_pin_t
{
public:
    gpio_pin_t(uint8_t pin_number);
    gpio_pin_t(uint8_t pin_number, gpio_config_t& config);
    ~gpio_pin_t();

    void initialise(std::string consumer_string = "default");
    void initialise(const gpio_config_t& config, std::string consumer_string = "default");

    void set_direction(uint8_t direction);
    void set_bias(uint8_t bias);
    void set_value(uint8_t value);
    uint8_t get_value();
private:
    const uint8_t pin_number;
    const std::string consumer;
    uint8_t initialised;
    gpiod::line gpio_line;
    std::bitset<32> flags;
    uint8_t event;
};

extern gpio_pin_t GPIO0;
extern gpio_pin_t GPIO1;
extern gpio_pin_t GPIO2;
extern gpio_pin_t GPIO3;
extern gpio_pin_t GPIO4;
extern gpio_pin_t GPIO5;
extern gpio_pin_t GPIO6;
extern gpio_pin_t GPIO7;
extern gpio_pin_t GPIO8;
extern gpio_pin_t GPIO9;
extern gpio_pin_t GPIO10;
extern gpio_pin_t GPIO11;
extern gpio_pin_t GPIO12;
extern gpio_pin_t GPIO13;
extern gpio_pin_t GPIO14;
extern gpio_pin_t GPIO15;
extern gpio_pin_t GPIO16;
extern gpio_pin_t GPIO17;
extern gpio_pin_t GPIO18;
extern gpio_pin_t GPIO19;
extern gpio_pin_t GPIO20;
extern gpio_pin_t GPIO21;
extern gpio_pin_t GPIO22;
extern gpio_pin_t GPIO23;
extern gpio_pin_t GPIO24;
extern gpio_pin_t GPIO25;
extern gpio_pin_t GPIO26;
extern gpio_pin_t GPIO27;
extern gpio_pin_t GPIO28;
extern gpio_pin_t GPIO29;
extern gpio_pin_t GPIO30;
extern gpio_pin_t GPIO31;
extern gpio_pin_t GPIO32;
extern gpio_pin_t GPIO33;
extern gpio_pin_t GPIO34;
extern gpio_pin_t GPIO35;
extern gpio_pin_t GPIO36;
extern gpio_pin_t GPIO37;
extern gpio_pin_t GPIO38;
extern gpio_pin_t GPIO39;
extern gpio_pin_t GPIO40;
extern gpio_pin_t GPIO41;
extern gpio_pin_t GPIO42;
extern gpio_pin_t GPIO43;
extern gpio_pin_t GPIO44;
extern gpio_pin_t GPIO45;
extern gpio_pin_t GPIO46;
extern gpio_pin_t GPIO47;
extern gpio_pin_t GPIO48;
extern gpio_pin_t GPIO49;
extern gpio_pin_t GPIO50;
extern gpio_pin_t GPIO51;
extern gpio_pin_t GPIO52;
extern gpio_pin_t GPIO53;

} /* pi_zero_peripherals */
