/**
 * @file i2c_bus.cpp
 * @author Marco van Eerden (mavaneerden@gmail.com)
 * @brief Driver for the SSD1306 128x64 monochrome OLED display
 * @date 06-04-2022
 *
 * In this example, the display uses a 128x32 pixel matrix.
 * The device uses GDDRAM to store the state of the screen, with one bit
 * for each pixel (1 = on, 0 = off). These bits are divided into 8 pages,
 * and each page has an 8x128 matrix of pixels.
 *
 * The device uses a column pointer and a page pointer. When writing a byte to a page, the byte will be written to the first column
 * of the page by default. The column pointer is then increased. This is the page addressing mode.
 * For other addressing modes, see pages 34-35 of the SSD1306 manual.
 *
 * After sending the slave address (done by libi2c), we need to send
 * a control byte <Co, D/C, 0, 0, 0, 0, 0, 0>.
 * - If Co == 0, all following bytes are data bytes.
 * - If Co == 1, following bytes may contain control bytes.
 * - If D/C == 0, all following data bytes are a command.
 * - If D/C == 1, all following data bytes are written to the GDDRAM.
 *
 * Using the Co bit, we could potentially write several commands at the same time.
 * Some commands require writing multiple bytes. The commands can be found on pages 28-32
 * of the SSD1306 manual. Detailed descriptions of each command are on pages 34-46.
 *
 * The device also employs a frame sync (FR) signal that is pulled high when the
 * device finishes writing the values in the GDDRAM to the screen.
 * We should only write values to the GDDRAM when the FR signal is detected.
 * However, the board that the display is on doesn't provide an interface to this signal so we can't detect it.
 *
 * TODO: create function for specifying frames per second using the formula on page 22 of the data sheet.
 */

#include <assert.h>

#include "include/ssd1306.hpp"

using namespace pi_zero_peripherals;

/**
 * @brief Construct a new ssd1306_t object.
 *
 * @param bus I2C bus that the device is on.
 * @param address_lsb LSB of the slave address. Can be 0 or 1 depending on the SA0 pin.
 */
ssd1306_t::ssd1306_t(i2c_bus_t& bus, uint8_t address_lsb) :
    i2c_device_t(bus, ADDRESS_BASE | address_lsb)
{}

/**
 * @brief Initialises the SSD1306 OLED module. Uses the software initialisation example
 * on page 64 of the data sheet:
 * 1.  Set MUX ratio to default (64).
 * 2.  Set display offset to default (0).
 * 3.  Set display start line to default (0).
 * 4.  Set segment re-map to default (0).
 * 5.  Set COM output scan direction to default (normal).
 * 6.  Set COM pins hardware configuration to sequential.
 * 7.  Set contrast control to default (127).
 * 8.  Set display to default (normal).
 * 9.  Set oscillator frequency to default (8).
 * 10. Enable charge pump regulator.
 * 11. Enable display.
 */
void ssd1306_t::initialise()
{
    assert(!this->initialised);

    /* Initialise the I2C bus. */
    this->bus.initialise();

    /* Disable display for initialisation. */
    this->enable_display(false);

    /* Initialise device using software initialisation sequence. */
    this->set_multiplex_ratio();
    this->set_display_offset();
    this->set_display_start_line();
    this->set_segmet_re_map();
    this->set_com_output_scan_direction();
    this->set_com_pins_hardware_configuration(COM_PINS_HARDWARE_SEQUENTIAL);
    this->set_contrast();
    this->set_inverse_display();
    /* Charge pump enabled because V_bat is 3.3V. */
    this->enable_charge_pump(true);
    /* Enable the usage of GDRAM to allow custom images on the OLED display. */
    this->use_ram_contents();
    /* Clear the screen. */
    this->clear_screen();
    /* Enable the display. */
    this->enable_display(true);

    /* Set the initialised flag. */
    this->initialised = 1u;
}

/**
 * @brief Displays data on the OLED display.
 *
 * @param display_data A 2D array of data to display on the screen.
 *
 * TODO: allow writing of variable sized data at dynamic screen position.
 */
void ssd1306_t::display(uint8_t display_data[SCREEN_HEIGHT][SCREEN_WIDTH])
{
    /* Set addressing mode to horizontal: auto-wrap of page and column addresses. */
    this->set_memory_addressing_mode(HORIZONTAL_ADDRESSING_MODE);
    /* Set page addresses to default, increased automatically. */
    this->set_page_addresses();
    /* Set column addresses to default, increased automatically. */
    this->set_column_addresses();

    /* Loop over pages. */
    for (size_t page = 0; page < NUMBER_OF_PAGES; page++)
    {
        /* Loop over columns. */
        for (size_t column = 0; column < SCREEN_WIDTH; column++)
        {
            uint8_t data = 0u;

            /* Extract column data from display data. */
            for (size_t pixel = 0; pixel < 8u; pixel++)
            {
                data |= display_data[page * 8 + pixel][column] << pixel;
            }

            /* Write data to SSD1306. */
            this->write_data(data);
        }
    }
}

/**
 * @brief Clear the OLED screen by setting all data to 0.
 */
void ssd1306_t::clear_screen()
{
    uint8_t data[SCREEN_HEIGHT][SCREEN_WIDTH] = { 0u };

    this->display(data);
}

/**
 * @brief Set the contrast of the display.
 *
 * @param contrast A value between 0-255 that indicates the screen contrast.
 */
void ssd1306_t::set_contrast(uint8_t contrast)
{
    this->write_command(COMMAND_SET_CONTRAST_CONTROL);
    this->write_command(contrast);
}

/**
 * @brief Use the GDDRAM contents or not.
 *
 * @param state If true, uses GDDRAM contents. If false, doesn't use the GDDRAM contents.
 */
void ssd1306_t::use_ram_contents(bool state)
{
    this->write_command(COMMAND_ENTIRE_DISPLAY | !state);
}

/**
 * @brief Set the display to inverted or normal mode.
 *
 * @param state If true, inverts the GDDRAM contents before displaying. If false, uses normal display mode.
 */
void ssd1306_t::set_inverse_display(bool state)
{
    this->write_command(COMMAND_SET_NORMAL_INVERTED_DISPLAY | state);
}

/**
 * @brief Enable or disable the display.
 *
 * @param state If true, enables the display. If false, disables the display.
 */
void ssd1306_t::enable_display(bool state)
{
    this->write_command(COMMAND_SET_DISPLAY | state);
}

/**
 * @brief Activate or deactivate scrolling.
 *
 * @param state If true, activates scrolling. If false, deactivates scrolling.
 */
void ssd1306_t::activate_scroll(bool state)
{
    this->write_command(state ? COMMAND_ACTIVATE_SCROLL : COMMAND_DEACTIVATE_SCROLL);
}

/**
 * @brief Set parameters for scrolling the screen horizontally.
 *
 * @param mode Mode indicating left or right scrolling.
 * @param start_page First page that is part of the scrolling.
 * @param interval Scrolling interval.
 * @param end_page Last page that is part of the scrolling.
 */
void ssd1306_t::set_continuous_horizontal_scroll(continuous_horizontal_scroll_mode mode, uint8_t start_page, continuous_horizontal_scroll_interval interval, uint8_t end_page)
{
    assert(start_page < NUMBER_OF_PAGES);
    assert(end_page < NUMBER_OF_PAGES);
    assert(start_page <= end_page);

    this->write_command(COMMAND_CONTINUOUS_HORIZONTAL_SCROLL_SETUP | mode);
    this->write_command(0x00u);
    this->write_command(start_page);
    this->write_command(interval);
    this->write_command(end_page);
    this->write_command(0x00u);
    this->write_command(0xFFu);
}

/**
 * @brief Set parameters for scrolling the screen horizontally and vertically.
 *
 * @param mode Mode indicating left or right scrolling.
 * @param start_page First page that is part of the scrolling.
 * @param interval Scrolling interval.
 * @param end_page Last page that is part of the scrolling.
 * @param offset Scrolling offset.
 */
void ssd1306_t::set_continuous_vertical_horizontal_scroll(continuous_vertical_horizontal_scroll_mode mode, uint8_t start_page, continuous_horizontal_scroll_interval interval, uint8_t end_page, uint8_t offset)
{
    assert(start_page < NUMBER_OF_PAGES);
    assert(end_page < NUMBER_OF_PAGES);
    assert(start_page <= end_page);

    this->write_command(COMMAND_CONTINUOUS_VERTICAL_AND_HORIZONTAL_SCROLL_SETUP | mode);
    this->write_command(0x00u);
    this->write_command(start_page);
    this->write_command(interval);
    this->write_command(end_page);
    this->write_command(offset);
}

/**
 * @brief Set the amount of fixed and scrolling rows for vertical scrolling.
 *
 * @param fixed_rows Amount of fixed rows.
 * @param scroll_rows Amound of scrolling rows.
 */
void ssd1306_t::set_vertical_scroll_area(uint8_t fixed_rows, uint8_t scroll_rows)
{
    // TODO: assert MUX ratio, Display Start Line
    this->write_command(COMMAND_SET_VERTICAL_SCROLL_AREA);
    this->write_command(fixed_rows);
    this->write_command(scroll_rows);
}

/**
 * @brief Set the starting address for the column in page addressing mode.
 *
 * @param address Starting address.
 */
void ssd1306_t::set_column_start_address(uint8_t address)
{
    assert(address < SCREEN_WIDTH);

    this->write_command(COMMAND_SET_LOWER_COLUMN_START_ADDRESS | (address & 0x0F));
    this->write_command(COMMAND_SET_HIGHER_COLUMN_START_ADDRESS | (address >> 4u));
}

/**
 * @brief Set the memory addressing mode used for reading/writing data from/to the display.
 *
 * @param mode
 */
void ssd1306_t::set_memory_addressing_mode(addressing_mode mode)
{
    this->write_command(COMMAND_SET_MEMORY_ADDRESSING_MODE);
    this->write_command(mode);
}

/**
 * @brief Set the start and end column addresses. Used in horizontal and vertical addressing mode.
 *
 * @param start_address Column start address.
 * @param end_address Column end address.
 */
void ssd1306_t::set_column_addresses(uint8_t start_address, uint8_t end_address)
{
    assert(start_address < SCREEN_WIDTH);
    assert(end_address < SCREEN_WIDTH);
    assert(start_address <= end_address);

    this->write_command(COMMAND_SET_COLUMN_ADDRESS);
    this->write_command(start_address);
    this->write_command(end_address);
}

/**
 * @brief Set the start and end page addresses. Used in horizontal and vertical addressing mode.
 *
 * @param start_address Page start address.
 * @param end_address Page end address.
 */
void ssd1306_t::set_page_addresses(uint8_t start_address, uint8_t end_address)
{
    assert(start_address < NUMBER_OF_PAGES);
    assert(end_address < NUMBER_OF_PAGES);
    assert(start_address <= end_address);

    this->write_command(COMMAND_SET_PAGE_ADDRESS);
    this->write_command(start_address);
    this->write_command(end_address);
}

/**
 * @brief Set the page start address. Used in page addressing mode.
 *
 * @param address Page start address.
 */
void ssd1306_t::set_page_start_address(uint8_t address)
{
    assert(address < NUMBER_OF_PAGES);

    this->write_command(COMMAND_SET_PAGE_START_ADDRESS | address);
}

/**
 * @brief Set the first line to display the GDDRAM contents from.
 *
 * @param line Line to start displaying at.
 */
void ssd1306_t::set_display_start_line(uint8_t line)
{
    assert(line < SCREEN_HEIGHT);

    this->write_command(COMMAND_SET_DISPLAY_START_LINE | line);
}

/**
 * @brief Set the re-mapping mode of the segments (columns) of the display.
 *
 * @param mode When true, the segments are reversed. When false, the segments use their normal mapping.
 */
void ssd1306_t::set_segmet_re_map(re_map_mode mode)
{
    this->write_command(COMMAND_SET_SEGMENT_RE_MAP | mode);
}

/**
 * @brief Set the multiplex ratio of the COM pins.
 *
 * @param ratio Multiplex ratio to set. Must be between 16-64
 */
void ssd1306_t::set_multiplex_ratio(uint8_t ratio)
{
    assert(16u <= ratio && ratio <= 64u);

    this->write_command(COMMAND_SET_MULTIPLEX_RATIO);
    this->write_command(ratio - 1u);
}

/**
 * @brief Set the direction of the scanning of the commons (rows) from GDDRAM.
 *
 * @param mode Mode to use. Can be normal (from top to bottom) or re-mapped (from bottom to top).
 */
void ssd1306_t::set_com_output_scan_direction(com_output_scan_direction mode)
{
    this->write_command(COMMAND_SET_COM_OUTPUT_SCAN_DIRECTION | mode);
}

/**
 * @brief Shift the commons (rows) downwards by a set amount. The rows wrap around.
 *
 * @param offset Offset to set.
 */
void ssd1306_t::set_display_offset(uint8_t offset)
{
    assert(offset < SCREEN_HEIGHT);

    this->write_command(COMMAND_SET_DISPLAY_OFFSET);
    this->write_command(offset);
}

/**
 * @brief Set the common (COM) pin configuration and enable/disable re-mapping of rows.
 *
 * @param configuration Sequential or alternative configuration.
 * @param enable_remap When true, re-mapping of rows is enabled. When false, rows are not re-mapped.
 */
void ssd1306_t::set_com_pins_hardware_configuration(com_pins_hardware_configuration configuration, bool enable_remap)
{
    this->write_command(COMMAND_SET_COM_PINS_HARDWARE_CONFIGURATION);
    this->write_command(0x02u | (configuration << 5u) | (enable_remap << 4u));
}

/**
 * @brief Set the clock values that are used to display data.
 *
 * @param clock_divider Clock divider for the oscillator clock. Must be between 1 and 16.
 * @param oscillator_frequency Frequency used for the oscillator between 333 and 407 kHz.
 */
void ssd1306_t::set_display_clock(uint8_t clock_divider, uint8_t oscillator_frequency)
{
    assert(1u <= clock_divider && clock_divider <= 0b1111u + 1u);
    assert(oscillator_frequency <= 0b1111u);

    this->write_command(COMMAND_SET_DISPLAY_CLOCK_DIVIDE_RATIO_OSCILLATOR_FREQUENCY);
    this->write_command((oscillator_frequency << 4u) | (clock_divider - 1u));
}

/**
 * @brief Set periods for (dis)charging OLED pixels in clock cycles.
 *
 * @param phase_1_period Phase 1 discharging period.
 * @param phase_2_period Phase 2 charging period.
 */
void ssd1306_t::set_pre_charge_period(uint8_t phase_1_period, uint8_t phase_2_period)
{
    assert(1u <= phase_1_period && phase_1_period <= 0b1111u);
    assert(1u <= phase_2_period && phase_2_period <= 0b1111u);

    this->write_command(COMMAND_SET_PRE_CHARGE_PERIOD);
    this->write_command((phase_2_period << 4u) | phase_1_period);
}

/**
 * @brief Set the voltage level at which the COM signal is deselected.
 *
 * @param level Voltage level.
 */
void ssd1306_t::set_v_comh_deselect_level(v_comh_deselect_level level)
{
    this->write_command(COMMAND_SET_V_COMH_DESELECT_LEVEL);
    this->write_command(level << 4u);
}

/**
 * @brief No Operation command.
 */
void ssd1306_t::nop()
{
    this->write_command(COMMAND_NOP);
}

/**
 * @brief Enables charge pump regulator. Should be enabled when Vbat is connected to an external source.
 *
 * @param state If true, charge pump regulator is enabled. If false, it is disabled.
 */
void ssd1306_t::enable_charge_pump(bool state)
{
    this->write_command(COMMAND_CHARGE_PUMP_SETTING);
    this->write_command(0b010000 | (state << 2u));
}

/**
 * @brief Writes a command to the display.
 *
 * @param command Command to write.
 */
void ssd1306_t::write_command(uint8_t command)
{
    uint8_t buffer[2] = { COMMAND_BYTE, command };

    this->i2c_write(buffer, 2);
}

/**
 * @brief Reads the display status from the display.
 *
 * @return uint8_t Display status. 1u is on, 0u is off.
 */
uint8_t ssd1306_t::get_display_status()
{
    uint8_t result;
    uint8_t command[1] = { COMMAND_BYTE };

    this->i2c_write(command, 1u);
    this->i2c_read(&result, 1u);

    return result >> 6u;
}

/**
 * @brief Writes a data byte to the display.
 *
 * @param data Data byte to write.
 */
void ssd1306_t::write_data(uint8_t data)
{
    uint8_t buffer[2] = { DATA_BYTE, data };

    this->i2c_write(buffer, 2u);
}

/**
 * @brief Reads a data byte from the display.
 *
 * @return uint8_t Data that was read.
 */
uint8_t ssd1306_t::read_data()
{
    uint8_t data[1] = { DATA_BYTE };

    this->i2c_write(data, 1u);
    /* Dummy read. */
    this->i2c_read(data, 1u);
    this->i2c_read(data, 1u);

    return *data;
}
