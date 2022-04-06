/**
 * @file oled_example.cpp
 * @author Marco van Eerden (mavaneerden@gmail.com)
 * @brief Contains tests for i2c_bus_t and ssd1306_t using doctest.
 * @date 06-04-2022
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../../lib/doctest/doctest.h"

#include <fcntl.h>

#include "include/ssd1306.hpp"

using namespace pi_zero_peripherals;

/**
 * @brief Tests the i2c_bus_t class.
 */
TEST_CASE("Test i2c_bus_t")
{
    i2c_bus_t i2c_bus(1u);

    /* Test the constructor. */
    SUBCASE("Constructor")
    {
        CHECK(i2c_bus.bus_fd == -1);
        CHECK(i2c_bus.initialised == 0u);
    }

    /* Test bus initialisation. */
    SUBCASE("Initialisation")
    {
        i2c_bus.initialise();
        /* Check if file descriptor is set. */
        CHECK(i2c_bus.bus_fd != -1);
        /* Check if initialised flag is set. */
        CHECK(i2c_bus.initialised == 1u);
    }

    /* Test the destructor. */
    SUBCASE("Destructor")
    {
        i2c_bus.~i2c_bus_t();
        /* Check if file descriptor is invalid. */
        CHECK(fcntl(i2c_bus.bus_fd, F_GETFD) == -1);
    }
}

/**
 * @brief Tests the ssd1306_t class.
 */
TEST_CASE("Test ssd1306_t")
{
    ssd1306_t ssd1306(i2c);

    /* Test initialisation. */
    SUBCASE("Initialisation")
    {
        ssd1306.initialise();
        /* Check if display is on. */
        CHECK(ssd1306.get_display_status() == 1u);
    }

    /* Test disabling of display. */
    SUBCASE("Disable display")
    {
        ssd1306.enable_display(false);
        /* Check if display is off. */
        CHECK(ssd1306.get_display_status() == 0u);
    }

    // TODO: check if screen is clear.
    // TODO: write data and check if data is written.
}
