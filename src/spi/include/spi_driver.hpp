#pragma once

#include <stdint.h>

#include "../../../lib/azorg_spi/spi.h"

namespace pi_zero_peripherals
{

class spi_driver_t
{
public:
    spi_driver_t(
        uint8_t bus_number,
        uint8_t chip_select,
        uint8_t bits_per_word = 8u,
        uint8_t polarity = 0u,
        uint8_t phase = 0u,
        bool lsb_first = 0u,
        bool enable_3_wire = 0u,
        bool enable_loopback = 0u,
        bool enable_chip_select = 1u,
        bool chip_select_high = 0u
    );
    ~spi_driver_t();

    void initialise(uint32_t clock_frequency);

    void read(uint8_t* buffer, uint8_t size);
    void write(uint8_t* data, uint8_t size);
protected:
    spi_t spi;
    const uint8_t bus_number;
    const uint8_t chip_select;
    const uint8_t bits_per_word;
    const uint8_t mode;
    const uint8_t flags;
    bool initialised;
};

} /* pi_zero_peripherals */