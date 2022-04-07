#include <string>

#include "include/spi_driver.hpp"

using namespace pi_zero_peripherals;

spi_driver_t::spi_driver_t(
    uint8_t bus_number,
    uint8_t chip_select,
    uint8_t bits_per_word,
    uint8_t polarity,
    uint8_t phase,
    bool lsb_first,
    bool enable_3_wire,
    bool enable_loopback,
    bool enable_chip_select,
    bool chip_select_high
) : bus_number(bus_number),
    chip_select(chip_select),
    bits_per_word(bits_per_word),
    mode((polarity << 1u) | phase),
    flags(((lsb_first << 4u) & SPI_LSB_FIRST)
         | ((enable_3_wire << 5u) & SPI_3WIRE)
         | ((enable_loopback << 6u) & SPI_LOOP)
         | ((!enable_chip_select << 7u) & SPI_NO_CS)
         | ((chip_select_high << 3u) & SPI_CS_HIGH)),
    initialised(false)
{}

spi_driver_t::~spi_driver_t()
{
    if (this->initialised)
    {
        spi_free(&spi);
    }
}

void spi_driver_t::initialise(uint32_t clock_frequency)
{
    std::string filename = "/dev/spidev" + std::to_string(this->bus_number) + "." + std::to_string(this->chip_select);

    int8_t status = spi_init(&spi, filename.data(), this->mode | this->flags, this->bits_per_word, clock_frequency);

    if (status != SPI_ERR_NONE)
    {
        // TODO: handle initialisation error.
    }

    this->initialised = true;
}

void spi_driver_t::read(uint8_t* buffer, uint8_t size)
{
    int8_t status = spi_read(&spi, buffer, size);

    if (status == SPI_ERR_READ)
    {
        // TODO: handle read error.
    }
}

void spi_driver_t::write(uint8_t* data, uint8_t size)
{
    int8_t status = spi_write(&spi, data, size);

    if (status == SPI_ERR_WRITE)
    {
        // TODO: handle write error.
    }
}
