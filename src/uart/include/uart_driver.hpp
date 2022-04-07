#pragma once

#include <stdint.h>
#include "../../../lib/uart/uart.h"

namespace pi_zero_peripherals
{

class uart_driver_t
{
public:
    uart_driver_t(uint8_t bus_number, uint32_t baud_rate);
    ~uart_driver_t();

    void initialise();
    void write(uint8_t* data, uint8_t size);
    void read(uint8_t* buffer, uint8_t size);
private:
    UartDevice device;
    bool initialised;
};

} /* pi_zero_peripherals */
