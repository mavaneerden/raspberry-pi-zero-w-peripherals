/**
 * @file gpio_pin.cpp
 * @author Marco van Eerden (mavaneerden@gmail.com)
 * @brief Contains the implementation for the gpio_pin class.
 * Also defines all 54 GPIO pins.
 * @date 2021-12-31
 *
 * @copyright Copyright (c) 2021 Marco van Eerden
 */

#include <assert.h>

#include "include/gpio_pin.hpp"

namespace pi_zero_peripherals
{
/* Number of GPIO pins supported. */
static constexpr uint8_t NUM_GPIO_PINS = 54u;
/* Define default config. */
static constexpr gpio_config_t DEFAULT_CONFIG;

/* Define chip for gpiochip0. */
static gpiod::chip gpiochip0("0", gpiod::chip::OPEN_BY_NUMBER);

/* Define GPIO pins. */
gpio_pin_t GPIO0(0u);
gpio_pin_t GPIO1(1u);
gpio_pin_t GPIO2(2u);
gpio_pin_t GPIO3(3u);
gpio_pin_t GPIO4(4u);
gpio_pin_t GPIO5(5u);
gpio_pin_t GPIO6(6u);
gpio_pin_t GPIO7(7u);
gpio_pin_t GPIO8(8u);
gpio_pin_t GPIO9(9u);
gpio_pin_t GPIO10(10u);
gpio_pin_t GPIO11(11u);
gpio_pin_t GPIO12(12u);
gpio_pin_t GPIO13(13u);
gpio_pin_t GPIO14(14u);
gpio_pin_t GPIO15(15u);
gpio_pin_t GPIO16(16u);
gpio_pin_t GPIO17(17u);
gpio_pin_t GPIO18(18u);
gpio_pin_t GPIO19(19u);
gpio_pin_t GPIO20(20u);
gpio_pin_t GPIO21(21u);
gpio_pin_t GPIO22(22u);
gpio_pin_t GPIO23(23u);
gpio_pin_t GPIO24(24u);
gpio_pin_t GPIO25(25u);
gpio_pin_t GPIO26(26u);
gpio_pin_t GPIO27(27u);
gpio_pin_t GPIO28(28u);
gpio_pin_t GPIO29(29u);
gpio_pin_t GPIO30(30u);
gpio_pin_t GPIO31(31u);
gpio_pin_t GPIO32(32u);
gpio_pin_t GPIO33(33u);
gpio_pin_t GPIO34(34u);
gpio_pin_t GPIO35(35u);
gpio_pin_t GPIO36(36u);
gpio_pin_t GPIO37(37u);
gpio_pin_t GPIO38(38u);
gpio_pin_t GPIO39(39u);
gpio_pin_t GPIO40(40u);
gpio_pin_t GPIO41(41u);
gpio_pin_t GPIO42(42u);
gpio_pin_t GPIO43(43u);
gpio_pin_t GPIO44(44u);
gpio_pin_t GPIO45(45u);
gpio_pin_t GPIO46(46u);
gpio_pin_t GPIO47(47u);
gpio_pin_t GPIO48(48u);
gpio_pin_t GPIO49(49u);
gpio_pin_t GPIO50(50u);
gpio_pin_t GPIO51(51u);
gpio_pin_t GPIO52(52u);
gpio_pin_t GPIO53(53u);

/**
 * @brief Construct a new gpio_pin_t object.
 *
 * @param pin_number Number of the GPIO pin.
 */
gpio_pin_t::gpio_pin_t(uint8_t pin_number) :
    pin_number(pin_number),
    initialised(0u),
    gpio_line(gpiochip0.get_line(pin_number)),
    flags(0u),
    event(0u)
{}

/**
 * @brief Destroy a gpio_pin_t object.
 * Release the GPIO line if it is used by this program.
 */
gpio_pin_t::~gpio_pin_t()
{
    if (this->gpio_line.is_used())
    {
        this->gpio_line.release();
    }
}

/**
 * @brief Initialise the GPIO pin. Can only be called once after the module is configured.
 * The pin values must be valid.
 * Requests the GPIO pin for this program using the stored configuration.
 *
 * @param consumer_string Name of the consumer (default: "default").
 */
void gpio_pin_t::initialise(std::string consumer_string)
{
    initialise(DEFAULT_CONFIG, consumer_string);
}

/**
 * @brief Initialise the GPIO pin. Can only be called once after the module is configured.
 * The pin values must be valid.
 * Requests the GPIO pin for this program using the stored configuration.
 *
 * @param config Configuration for the GPIO pin.
 * @param consumer_string Name of the consumer (default: "default").
 */
void gpio_pin_t::initialise(const gpio_config_t& config, std::string consumer_string)
{
    /* Pin must not be initialised yet. */
    assert(this->initialised == 0u);
    /* Pin number must fall within the GPIO range. */
    assert(this->pin_number < NUM_GPIO_PINS);

    /* Request GPIO pin for this program. */
    this->gpio_line.request({
        consumer_string,
        config.direction == GPIOD_LINE_DIRECTION_OUTPUT ? gpiod::line_request::DIRECTION_OUTPUT : gpiod::line_request::DIRECTION_OUTPUT,
        config.bias
    }, config.output_value);

    /* Set initialised to 1. */
    this->initialised = 1u;

    /* Set flags. */
    this->set_bias(config.bias);
}

/**
 * @brief Set the direction of the pin. Pin must be initialised and line must be used by this program.
 *
 * @param direction Direction to set. Must be GPIOD_LINE_DIRECTION_INPUT or GPIOD_LINE_DIRECTION_OUTPUT.
 */
void gpio_pin_t::set_direction(uint8_t direction)
{
    /* Pin must be initialised. */
    assert(this->initialised == 1u);
    /* GPIO line must be used by this program. */
    assert(this->gpio_line.is_used());
    /* Direction must be valid. */
    assert(direction == GPIOD_LINE_DIRECTION_INPUT || direction == GPIOD_LINE_DIRECTION_OUTPUT);

    /* Set the GPIO direction. */
    direction == GPIOD_LINE_DIRECTION_INPUT ? this->gpio_line.set_direction_input() : this->gpio_line.set_direction_output();
}

/**
 * @brief Set bias of the pin. Pin must be initialised and line must be used by this program.
 *
 * @param bias Bias to set. Must be GPIOD_LINE_BIAS_AS_IS, GPIOD_LINE_BIAS_DISABLE, GPIOD_LINE_BIAS_PULL_UP or GPIOD_LINE_BIAS_PULL_DOWN.
 */
void gpio_pin_t::set_bias(uint8_t bias)
{
    /* Pin must be initialised. */
    assert(this->initialised == 1u);
    /* Bias must be valid. */
    assert(bias >= GPIOD_LINE_BIAS_AS_IS && bias <= GPIOD_LINE_BIAS_PULL_DOWN);

    /* Set new flags according to the bias. */
    switch (bias)
    {
    case GPIOD_LINE_BIAS_DISABLE:
        this->flags |= gpiod::line_request::FLAG_BIAS_DISABLE;
        break;
    case GPIOD_LINE_BIAS_PULL_UP:
        this->flags |= gpiod::line_request::FLAG_BIAS_PULL_UP;
        break;
    case GPIOD_LINE_BIAS_PULL_DOWN:
        this->flags |= gpiod::line_request::FLAG_BIAS_PULL_DOWN;
        break;
    default:
        break;
    }

    /* Set new flags. */
    this->gpio_line.set_flags(this->flags);
}

/**
 * @brief Set value of the pin. Pin must be initialised and line must be used by this program.
 *
 * @param value Value to set. Must be GPIO_STATE_HIGH (1) or GPIO_STATE_LOW (0).
 */
void gpio_pin_t::set_value(uint8_t value)
{
    /* Pin must be initialised. */
    assert(this->initialised == 1u);
    /* GPIO line must be used by this program. */
    assert(this->gpio_line.is_used());
    /* Value must be high or low. */
    assert(value == GPIO_STATE_HIGH || value == GPIO_STATE_LOW);

    /* Set new value. */
    this->gpio_line.set_value(value);
}

/**
 * @brief Get the value of the pin. Pin must be initialised and line must be used by this program.
 *
 * @return Value of the pin.
 */
uint8_t gpio_pin_t::get_value()
{
    /* Pin must be initialised. */
    assert(this->initialised == 1u);
    /* GPIO line must be used by this program. */
    assert(this->gpio_line.is_used());
    /* Function must be input. */
    assert(this->gpio_line.direction() == GPIOD_LINE_DIRECTION_INPUT);

    /* Return value. */
    return this->gpio_line.get_value();
}

} /* pi_zero_peripherals */
