/**     \file   SSD1306.h

        \brief  SSD1306 display controller driver declarations
 
        \license 
                MIT: The MIT License (https://opensource.org/licenses/MIT)
                .
                Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
                and associated documentation files (the "Software"), to deal in the Software without restriction, 
                including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
                and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
                subject to the following conditions:
                .
                The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
                .
                THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
                INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
                IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
                WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR 
                THE USE OR OTHER DEALINGS IN THE SOFTWARE.
                
        \copyright
                DIT: 2017; Drechsler Information Technologies; www.drechsler-it.de
 
        \authors
                jrgdre: Joerg Drechsler; DIT
 
        \versions
                1.0.0: 2017-06-21 jrgdre initial release

 */
#ifndef SSD1306_H_
#define SSD1306_H_

#include <asf.h>
#include "Com_Driver.h"         // Generic Communication Driver
#include "Framebuffer.h"        // Generic Framebuffer

/**
 * \brief  SSD1306 type of data enumeration                              
 *
 * Enumeration of the data type specifiers the SSD1306 understands.
 */
enum SSD1306_Datatype {
        SSD1306_COMMAND = 0x00, //< following byte is a command
        SSD1306_DATA    = 0x40  //< following byte are data
};



// =========================================================
//  fundamental command codes                            
// =========================================================

#define SSD1306_SET_CONTRAST                            0x81    //< 2 byte command: set display contrast level
                                                                //  2.byte: contrast level value
                                                                                                                                
#define SSD1306_SET_DISPLAY_RESUME_RAM                  0xA4    //< 1 byte command: on DISPLAY_ON the content of the GDDRAM is shown 
#define SSD1306_SET_DISPLAY_IGNORE_RAM                  0xA5    //< 1 byte command: on DISPLAY_ON all pixels are on, independent of the GDDRAM content
#define SSD1306_SET_DISPLAY_NORMAL                      0xA6    //< 1 byte command: normal GDDRAM content display
#define SSD1306_SET_DISPLAY_INVERS                      0xA7    //< 1 byte command: inverts GDDRAM content display

#define SSD1306_DISPLAY_OFF                             0xAE    //< 1 byte command: switch display off
#define SSD1306_DISPLAY_ON                              0xAF    //< 1 byte command: switch display on



// =========================================================
//  scrolling command codes                               
// =========================================================

#define SSD1306_SET_CONTINUOUS_HSCROLL_RIGHT            0x26    //< 7 byte command: setup continuous right horizontal scroll
#define SSD1306_SET_CONTINUOUS_HSCROLL_LEFT             0x27    //< 7 byte command: setup continuous left horizontal scroll
                                                                //  2.byte: dummy byte                   (set as 00h)
                                                                //  3.byte: start page adAdress          (low 3 bits)
                                                                //  4.byte: frame frequency              (low 3 bits)
                                                                //  5.byte: end page address (>= 2.byte) (low 3 bits)
                                                                //  6.byte: dummy byte (set as 00h)
                                                                //  7.byte: dummy byte (set as FFh)

#define SSD1306_SET_CONTINUOUS_HVSCROLL_RIGHT           0x29    //< 6 byte command: setup continuous vertical and right horizontal scroll
#define SSD1306_SET_CONTINUOUS_HVSCROLL_LEFT            0x2A    //< 6 byte command: setup continuous vertical and left horizontal scroll
                                                                //  2.byte: dummy byte                   (set as 00h)
                                                                //  3.byte: start page address           (low 3 bits)
                                                                //  4.byte: frame frequency              (low 3 bits)
                                                                //  5.byte: end page address (>= 2.byte) (low 3 bits)
                                                                //  6.byte: vertical scrolling offset    (low 5 bits)
                                
#define SSD1306_SET_VERTICAL_SCROLL_AREA                0xA3    //< 3 byte command: set the area (rows) to scroll vertically
                                                                //  2.byte: number of rows in top fixed area (RESET= 0d)
                                                                //  3.byte: number of rows in scroll area    (RESET=64d)
                                                                                                                
#define SSD1306_SCROLL_DEACTIVATE                       0x2E    //< 1 byte command: stop scrolling
#define SSD1306_SCROLL_ACTIVATE                         0x2F    //< 1 byte command: start scrolling



// =========================================================
//  addressing setting command codes                      
// =========================================================

#define SSD1306_SET_MEMORY_ADRESSING_MODE               0x20    //< 2 byte command: setup memory addressing mode
                                                                //  2.byte: SSD1306_Memory_Adressing_Mode
/**
 * \brief SSD1306 addressing mode enumeration
 *
 * Enumeration of the memory addressing modes that can be used with the SSD1306.
 */
enum SSD1306_Memory_Adressing_Mode {
        SSD1306_ADDR_MODE_HORIZONTAL = 0x00, //< first column pointer, at column end address page pointer automatically increased after each RAM read/write
        SSD1306_ADDR_MODE_VERTICAL   = 0x01, //< first page pointer, at page end address column pointer auto-increased after each RAM read/write
        SSD1306_ADDR_MODE_PAGE       = 0x02  //< column pointer increases after each RAM read/write; resets to column start when column end is reached; manual page control
};

// SSD1306_ADDR_MODE_HORIZONTAL, SSD1306_ADDR_MODE_VERTICAL only
#define SSD1306_SET_COLUMN_ADRESSES                     0x21    //< 3 byte command: setup column start and end addresses
                                                                //  2.byte: column start address, range 0..127d (RESET=  0d)
                                                                //  3.byte: column end   address, range 0..127d (RESET=127d)

#define SSD1306_SET_PAGE_ADRESSES                       0x22    //< 3 byte command: setup column start and end addresses
                                                                //  2.byte: page start address, range 0..7d (RESET=0d)
                                                                //  3.byte: page end   address, range 0..7d (RESET=7d)

// SSD1306_ADDR_MODE_PAGE only
#define SSD1306_SET_PAGE_ADRESSING_COLUMN_START_LOWER   0x00    //< 2 nibble command: upper nibble command code (0x0), lower nibble column
#define SSD1306_SET_PAGE_ADRESSING_COLUMN_START_HIGHER  0x10    //< 2 nibble command: upper nibble command code (0x0), lower nibble column
#define SSD1306_SET_PAGE_ADRESSING_START_PAGE           0xB0    //< 2 nibble command: upper nibble command code (0x0), lower nibble page



// =========================================================
//  hardware configuration command codes                  
// =========================================================

#define SSD1306_SET_CHARGE_PUMP_SOURCE                  0x8D    //< 2 byte command: set vertical shift by COM
                                                                //  2.byte: SSD1306_COM_Pins_Hardware_Configuration
/**
 * \brief SSD1306 charge pump mode enumeration
 *
 * Enumeration of the charge pump modes that can be used with the SSD1306.
 */
enum SSD1306_Charge_Pump_Source {
        SSD1306_CHARGE_PUMP_SOURCE_INTERNAL_VCC = 0x14, //< alternative COM pin configuration (RESET)
        SSD1306_CHARGE_PUMP_SOURCE_EXTERNAL_VCC = 0x10, //< sequential  COM pin configuration
};

#define SSD1306_SET_DISPLAY_START_LINE                  0x40    //< 1 byte command: 2 MSB command | 6 LSB: line number (6 LSB RESET=0)

#define SSD1306_SET_SEGMENT_NORMAL                      0xA0    //< 1 byte command: column address   0d is mapped to SEG0 (RESET)
#define SSD1306_SET_SEGMENT_REMAP                       0xA1    //< 1 byte command: column address 127d is mapped to SEG0

enum SSD1306_Segment_Mapping {
        SSD1306_SEGMENT_MAPPING_NORMAL = SSD1306_SET_SEGMENT_NORMAL,
        SSD1306_SEGMENT_MAPPING_REMAP  = SSD1306_SET_SEGMENT_REMAP        
};

#define SSD1306_SET_MULTIPLEX_RATIO                     0xA8    //< 2 byte command: set MUX ration to N+1 MUX
                                                                //  2.byte: display_height-1; 0-14 invalid

#define SSD1306_SET_COM_OUTPUT_DIRECTION_NORMAL         0xC0    //< 1 byte command: normal mode (RESET);              scan from COM0 to COM[MUX_ratio-1]
#define SSD1306_SET_COM_OUTPUT_DIRECTION_REMAP          0xC8    //< 1 byte command: remapped (vertical flipped) mode; scan from COM[MUX_ratio-1] to COM0

enum SSD1306_Page_Mapping {
        SSD1306_PAGE_MAPPING_NORMAL    = SSD1306_SET_COM_OUTPUT_DIRECTION_NORMAL,
        SSD1306_PAGE_MAPPING_REMAP     = SSD1306_SET_COM_OUTPUT_DIRECTION_REMAP        
};

#define SSD1306_SET_DISPLAY_OFFSET                      0xD3    //< 2 byte command: set vertical shift by COM 
                                                                //  2.byte: 0..63d (RESET=0d)

#define SSD1306_SET_COM_PINS_HARDWARE_CONFIGURATION     0xDA    //< 2 byte command: set vertical shift by COM
                                                                //  2.byte: SSD1306_COM_Pins_Hardware_Configuration
                                                                                                                                
/**
 * \brief SSD1306 COM pin mode enumeration
 *
 *        Enumeration of the COM pin modes that can be used with the SSD1306.
 */
enum SSD1306_COM_Pins_Hardware_Configuration {
        SSD1306_COM_PIN_CONFIGURATION_SEQUENTIAL  = 0x02, //< sequential  COM pin configuration
        SSD1306_COM_PIN_CONFIGURATION_ALTERNATIVE = 0x12, //< alternative COM pin configuration (RESET)
        SSD1306_COM_PIN_REMAP_DISABLE             = 0x02, //< disable COM left/right remap      (RESET)
        SSD1306_COM_PIN_REMAP_ENABLE              = 0x22  //< enable  COM left/right remap
};



// =========================================================
//  timing and driving scheme setting command codes       
// =========================================================

#define SSD1306_SET_DISPLAY_OSC_FREQ_AND_CLOCK_DIV      0xD5    //< 2 byte command: set Display Clock Divide Ratio and Oscillator Frequency
                                                                //  2.byte: 4 MSB: Oscillator Frequency, range 0x0..0xF (RESET=0x8)
                                                                //          4 LSB: divide ratio; ratio is 4 LSB value +1 (RESET=0x0; ratio=1)

#define SSD1306_SET_PRECHARGE_PERIODE                   0xD9    //< 2 byte command: set pre-charge period
                                                                //  2.byte: 4 MSB: Phase 2 period of up to 15 DCLK; clocks 0 is invalid entry (RESET=0x2 )
                                                                //          4 LSB: Phase 1 period of up to 15 DCLK; clocks 0 is invalid entry (RESET=0x2 )

#define SSD1306_SET_VCOMH_DESELECT_LEVEL                0xDB    //< 2 byte command: set pre-charge period
                                                                //  2.byte: 4 MSB: Phase 2 period of up to 15 DCLK; clocks 0 is invalid entry (RESET=2h )

/**
 * \brief SSD1306 VCOMH de-select level enumeration
 *
 * Enumeration of the VCOMH de-select levels that can be used with the SSD1306.
 */
enum SSD1306_Vcomh_Deselect_Level {
        SSD1306_VCOMH_0p65 = 0x00, // ~0.65 x Vcc
        SSD1306_VCOMH_0p77 = 0x20, // ~0.77 x Vcc (RESET)
        SSD1306_VCOMH_0p83 = 0x30  // ~0.83 x Vcc
};



// =========================================================
//  driver configuration                                               
// =========================================================

enum SSD1306_Geometry {
        SSD1306_GEOMETRY_128x32 = 32,
        SSD1306_GEOMETRY_128x64 = 64
};

/**
 * \brief Configuration data structure for a single SSD1306 controller IC 
 */
struct SSD1306 {
        struct Com_Driver                com_driver        ; //< a driver that knows how to talk to this SSD1306
        uint16_t                         address           ; //< bus address of this SSD1306
        enum SSD1306_Geometry            geometry          ; //< geometry of AMOLED screen attached to this SSD1306
        enum SSD1306_Charge_Pump_Source  charge_pump_source; //< charge pump power source configuration of this SSD1306
        bool                             flip_horizontal   ; //< flip the rows
        bool                             flip_vertical     ; //< flip the columns
};
        
        
        
// =========================================================
//  driver functions                                      
// =========================================================

/**
 * \brief Initialize the controller using the most basic configuration
 *        for Horizontal Addressing Mode.
 *
 * If you need a more advanced setup, you can always call send the
 * commands to the controller, before switching on the display.
 */
enum status_code ssd1306_init(
  struct SSD1306 *ssd1306                       //< pointer to the configuration data structure to initialize
);

/** 
 * \brief Write a raw command sequence to the controller.
 */
enum status_code ssd1306_command_sequence_write (
  struct SSD1306 *const ssd1306                 //< configuration data structure for the controller to write the sequence to
,        uint8_t *const command_sequence        //< command sequence to write
,       uint16_t  const sequence_length         //< length of command sequence
);

/** 
 * \brief Write a raw data sequence to the controller.
 */
enum status_code ssd1306_data_sequence_write (
  struct SSD1306 *const ssd1306                 //< configuration data structure for the controller to write the sequence to
,        uint8_t *const data_sequence           //< data sequence to write
,       uint16_t  const sequence_length         //< length of data sequence
);

/** 
 * \brief Write the command sequence to switch the display ON or OFF to the controller.
 */
enum status_code ssd1306_display_on (
  struct SSD1306 *const ssd1306                 //< configuration data structure for the controller this command is for
,           bool  const value                   //< display on state to set (true: ON; false: OFF)
);

/**
 * \brief Update the display connected to this SSD1306 with the content of the framebuffer.
 *
 * This function uses differential update, unless the number of pixels changed is so large, 
 * that a complete update is more efficient.
 */
enum status_code ssd1306_display_update (
  struct SSD1306     *const ssd1306             //< data structure of the SSD1306 controller to write the update to
, struct Framebuffer *const framebuffer         //< framebuffer holding the data to display on the OLED
);

/**
 * \brief Update the display connected to this SSD1306 with the content of the framebuffer.
 *
 * Always updates the complete display.
 */
enum status_code ssd1306_display_update_all (
  struct SSD1306     *const ssd1306             //< data structure of the SSD1306 controller to write the update to
, struct Framebuffer *const framebuffer         //< framebuffer holding the data to display on the OLED
);

/**
 * \brief Update the display connected to this SSD1306 with the content of the framebuffer.
 *
 * Updates only tiles that have changed since the last update.
 */
enum status_code ssd1306_display_update_differential (
  struct SSD1306     *const ssd1306             //< data structure of the SSD1306 controller to write the update to
, struct Framebuffer *const framebuffer         //< framebuffer holding the data to display on the OLED
);

/**
 * \brief Specify the column start and end address of the display data RAM. 
 */
enum status_code ssd1306_set_column_range (
  struct SSD1306 *const ssd1306                 //< data structure of the SSD1306 controller to write the command  to
,        uint8_t  const column_start            //< first column to write next data byte to
,        uint8_t  const column_end              //< last column to write a data byte to 
);

/**
 * \brief Specify the page start and end address of the display data RAM. 
 */
enum status_code ssd1306_set_page_range (
  struct SSD1306 *const ssd1306                 //< data structure of the SSD1306 controller to write the command  to
,        uint8_t  const page_start              //< first column to write next data byte to
,        uint8_t  const page_end                //< last column to write a data byte to
);

#endif /* _SSD1306_H_ */
