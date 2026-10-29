/**
 * Private configuration file for the SSD1306 library.
 * This example is configured for STM32F0, I2C and including all fonts.
 */

#ifndef __SSD1306_CONF_H__
#define __SSD1306_CONF_H__

// Choose a microcontroller family
#define STM32F0

// Choose a bus
#define SSD1306_USE_I2C

// I2C Configuration
#define SSD1306_I2C_PORT        hi2c2   /*À CHANGER SELON NOTRE USAGE*/
#define SSD1306_I2C_ADDR    	(0x3C << 1)

// SSD1306 OLED height in pixels
#define SSD1306_HEIGHT          64

// SSD1306 width in pixels
#define SSD1306_WIDTH           128

#define SSD1306_BUFFER_SIZE   SSD1306_HEIGHT * SSD1306_WIDTH / 8

#define SSD1306_X_OFFSET_LOWER 0
#define SSD1306_X_OFFSET_UPPER 0

// Include only needed fonts
#define SSD1306_INCLUDE_FONT_6x8
#define SSD1306_INCLUDE_FONT_7x10
#define SSD1306_INCLUDE_FONT_11x18
#define SSD1306_INCLUDE_FONT_16x26

#define SSD1306_INCLUDE_FONT_16x24

#define SSD1306_INCLUDE_FONT_16x15

#endif /* __SSD1306_CONF_H__ */
