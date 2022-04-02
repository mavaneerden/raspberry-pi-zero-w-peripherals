#pragma once

#include "../../../src/i2c/include/i2c_device.hpp"

namespace pi_zero_peripherals
{

class ssd1306_t : public i2c_device_t
{
private:
    static constexpr uint8_t SCREEN_WIDTH = 128u;
    static constexpr uint8_t SCREEN_HEIGHT = 32u;
    static constexpr uint8_t NUMBER_OF_PAGES = SCREEN_HEIGHT / 8u;
    static constexpr uint8_t ADDRESS_BASE = 0b0111100u;
public:
    ssd1306_t(i2c_bus_t& bus, uint8_t address_lsb = 0u);
    void initialise();
    void display(uint8_t display_data[SCREEN_HEIGHT][SCREEN_WIDTH]);
    void clear_screen();

    enum continuous_horizontal_scroll_mode : uint8_t {
        HORIZONTAL_SCROLL_RIGHT = 0u,
        HORIZONTAL_SCROLL_LEFT  = 1u
    };

    enum continuous_horizontal_scroll_interval : uint8_t {
        HORIZONTAL_SCROLL_INTERVAL_2_FRAMES   = 0b111u,
        HORIZONTAL_SCROLL_INTERVAL_3_FRAMES   = 0b100u,
        HORIZONTAL_SCROLL_INTERVAL_4_FRAMES   = 0b101u,
        HORIZONTAL_SCROLL_INTERVAL_25_FRAMES  = 0b110u,
        HORIZONTAL_SCROLL_INTERVAL_5_FRAMES   = 0b000u,
        HORIZONTAL_SCROLL_INTERVAL_64_FRAMES  = 0b001u,
        HORIZONTAL_SCROLL_INTERVAL_128_FRAMES = 0b010u,
        HORIZONTAL_SCROLL_INTERVAL_256_FRAMES = 0b011u,
    };

    enum continuous_vertical_horizontal_scroll_mode : uint8_t {
        VERTICAL_HORIZONTAL_SCROLL_RIGHT = 0b01u,
        VERTICAL_HORIZONTAL_SCROLL_LEFT  = 0b10u
    };

    enum addressing_mode : uint8_t {
        HORIZONTAL_ADDRESSING_MODE = 0b00u,
        VERTICAL_ADDRESSING_MODE   = 0b01u,
        PAGE_ADDRESSING_MODE       = 0b10u,
    };

    enum re_map_mode : uint8_t {
        RE_MAP_MODE_0   = 0u,
        RE_MAP_MODE_127 = 1u
    };

    enum com_output_scan_direction : uint8_t {
        COM_OUTPUT_SCAN_NORMAL   = 0x00u,
        COM_OUTPUT_SCAN_REMAPPED = 0x08u
    };

    enum com_pins_hardware_configuration : uint8_t {
        COM_PINS_HARDWARE_SEQUENTIAL  = 0u,
        COM_PINS_HARDWARE_ALTERNATIVE = 1u
    };

    enum v_comh_deselect_level : uint8_t {
        V_COMH_DESELECT_0_65 = 0x00u,
        V_COMH_DESELECT_0_77 = 0x20u,
        V_COMH_DESELECT_0_83 = 0x30u
    };

    void set_contrast(uint8_t contrast = 0x7Fu);
    void use_ram_contents(bool state = true);
    void set_inverse_display(bool state = false);
    void enable_display(bool state = false);
    void activate_scroll(bool state);
    void set_continuous_horizontal_scroll(continuous_horizontal_scroll_mode mode, uint8_t start_page, continuous_horizontal_scroll_interval interval, uint8_t end_page);
    void set_continuous_vertical_horizontal_scroll(continuous_vertical_horizontal_scroll_mode mode, uint8_t start_page, continuous_horizontal_scroll_interval interval, uint8_t end_page, uint8_t offset);
    void set_vertical_scroll_area(uint8_t fixed_rows = 0u, uint8_t scroll_rows = 64u);
    void set_column_start_address(uint8_t address = 0u);
    void set_memory_addressing_mode(addressing_mode mode = PAGE_ADDRESSING_MODE);
    void set_column_addresses(uint8_t start_address = 0u, uint8_t end_address = 127u);
    void set_page_addresses(uint8_t start_address = 0u, uint8_t end_address = NUMBER_OF_PAGES - 1u);
    void set_page_start_address(uint8_t address);
    void set_display_start_line(uint8_t line = 0u);
    void set_segmet_re_map(re_map_mode mode = RE_MAP_MODE_0);
    void set_multiplex_ratio(uint8_t ratio = 64u);
    void set_com_output_scan_direction(com_output_scan_direction mode = COM_OUTPUT_SCAN_NORMAL);
    void set_display_offset(uint8_t offset = 0u);
    void set_com_pins_hardware_configuration(com_pins_hardware_configuration configuration = COM_PINS_HARDWARE_ALTERNATIVE, bool enable_remap = false);
    void set_display_clock(uint8_t clock_divider = 1u, uint8_t oscillator_frequency = 0b1000u);
    void set_pre_charge_period(uint8_t phase_1_period = 0x02u, uint8_t phase_2_period = 0x02u);
    void set_v_comh_deselect_level(v_comh_deselect_level level = V_COMH_DESELECT_0_77);
    void nop();
    void enable_charge_pump(bool state = false);
private:
    addressing_mode mode;
    uint8_t initialised;

    enum dc_byte : uint8_t
    {
        COMMAND_BYTE = 0x00u,
        DATA_BYTE    = 0x40u
    };

    enum command : uint8_t
    {
        COMMAND_SET_CONTRAST_CONTROL = 0x81u,
        COMMAND_ENTIRE_DISPLAY = 0xA4u,
        COMMAND_SET_NORMAL_INVERTED_DISPLAY = 0xA6u,
        COMMAND_SET_DISPLAY = 0xAEu,
        COMMAND_CONTINUOUS_HORIZONTAL_SCROLL_SETUP = 0x26u,
        COMMAND_CONTINUOUS_VERTICAL_AND_HORIZONTAL_SCROLL_SETUP = 0x28u,
        COMMAND_DEACTIVATE_SCROLL = 0x2Eu,
        COMMAND_ACTIVATE_SCROLL = 0x2Fu,
        COMMAND_SET_VERTICAL_SCROLL_AREA = 0xA3u,
        COMMAND_SET_LOWER_COLUMN_START_ADDRESS = 0x00u,
        COMMAND_SET_HIGHER_COLUMN_START_ADDRESS = 0x10u,
        COMMAND_SET_MEMORY_ADDRESSING_MODE = 0x20u,
        COMMAND_SET_COLUMN_ADDRESS = 0x21u,
        COMMAND_SET_PAGE_ADDRESS = 0x22u,
        COMMAND_SET_PAGE_START_ADDRESS = 0xB0u,
        COMMAND_SET_DISPLAY_START_LINE = 0x40u,
        COMMAND_SET_SEGMENT_RE_MAP = 0xA0u,
        COMMAND_SET_MULTIPLEX_RATIO = 0xA8u,
        COMMAND_SET_COM_OUTPUT_SCAN_DIRECTION = 0xC0u,
        COMMAND_SET_DISPLAY_OFFSET = 0xD3u,
        COMMAND_SET_COM_PINS_HARDWARE_CONFIGURATION = 0xDAu,
        COMMAND_SET_DISPLAY_CLOCK_DIVIDE_RATIO_OSCILLATOR_FREQUENCY = 0xD5,
        COMMAND_SET_PRE_CHARGE_PERIOD = 0xD9u,
        COMMAND_SET_V_COMH_DESELECT_LEVEL = 0xDBu,
        COMMAND_NOP = 0xE3u,
        COMMAND_CHARGE_PUMP_SETTING = 0x8Du
    };

    void write_command(uint8_t command);
    void write_data(uint8_t data);
};

} /* pi_zero_peripherals */
