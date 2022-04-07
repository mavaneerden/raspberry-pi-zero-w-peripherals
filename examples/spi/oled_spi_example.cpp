#include "include/ssd1306_spi.hpp"

using namespace pi_zero_peripherals;

int main(int argc, char const *argv[])
{
    ssd1306_spi_t ssd1306(0u, true);

    ssd1306.initialise();

    ssd1306.set_inverse_display(true);

    return 0;
}
