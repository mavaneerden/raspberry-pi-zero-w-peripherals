#include "../../src/gpio/include/gpio_pin.hpp"

#include <thread>
#include <chrono>

using namespace pi_zero_peripherals;

int main()
{
    gpio_config_t config;
    config.direction = GPIOD_LINE_DIRECTION_OUTPUT;
    config.output_value = GPIO_STATE_HIGH;

    GPIO21.initialise(config);

    while (1u)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        GPIO21.set_value(GPIO_STATE_LOW);
        std::this_thread::sleep_for(std::chrono::milliseconds(950));
        GPIO21.set_value(GPIO_STATE_HIGH);
    }

    return 0;
}