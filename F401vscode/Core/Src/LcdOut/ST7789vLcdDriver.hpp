#ifndef ST7789V_LCD_DRIVER_H
#define ST7789V_LCD_DRIVER_H

#include <cstdint>
#include <algorithm>
#include <cmath>
#include "main.h"
#include "font8x16.hpp"

#define _ST7789_LCD_USE_FREERTOS     0

#define ST7789_RES_PIN GPIO_PIN_0
#define ST7789_DC_PIN GPIO_PIN_1
#define ST7789_CS_PIN GPIO_PIN_3
#define ST7789_PORT GPIOB

// Display size
constexpr uint16_t ST7789_WIDTH = 320;
constexpr uint16_t ST7789_HEIGHT = 240;

// Colors RGB565
struct ST7789_Colors_t {
    uint16_t BLACK   = 0x0000;
    uint16_t WHITE   = 0xFFFF;
    uint16_t RED     = 0xF800;
    uint16_t GREEN   = 0x07E0;
    uint16_t BLUE    = 0x001F;
    uint16_t YELLOW  = 0xFFE0;
    uint16_t CYAN    = 0x07FF;
    uint16_t MAGENTA = 0xF81F;
};

// Commands ST7789
namespace ST7789_Commands {
    constexpr uint8_t SWRESET = 0x01;
    constexpr uint8_t SLPOUT  = 0x11;
    constexpr uint8_t COLMOD  = 0x3A;
    constexpr uint8_t MADCTL  = 0x36;
    constexpr uint8_t CASET   = 0x2A;
    constexpr uint8_t RASET   = 0x2B;
    constexpr uint8_t RAMWR   = 0x2C;
    constexpr uint8_t DISPON  = 0x29;
    constexpr uint8_t INVOFF  = 0x20;  // Отключить инверсию цветов
    constexpr uint8_t INVON   = 0x21;  // Включить инверсию цветов
}

// Timeouts
namespace ST7789_Config {
    constexpr uint32_t SPI_TIMEOUT = 5000;  // Увеличен таймаут для передачи фреймбуфера
    constexpr uint32_t RESET_DELAY = 100;
    constexpr uint32_t INIT_DELAY = 120;
}

// Font settings
struct ST7789_Font_t {
    uint8_t CHAR_WIDTH = 8;
    uint8_t CHAR_HEIGHT = 16;
};

class ST7789vLcdDriver {

public:

	void Init();
	ST7789vLcdDriver() {}
	~ST7789vLcdDriver() {}
	
	void FillScreen(uint16_t color);
	void FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
	void SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

	void DrawPixel(uint16_t x, uint16_t y, uint16_t color);
	void DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
	void DrawChar(uint16_t x, uint16_t y, char ch, uint16_t color, uint16_t bg_color);
	void DrawString(uint16_t x, uint16_t y, const char* str, uint16_t color, uint16_t bg_color);
	void DrawStringUTF8(uint16_t x, uint16_t y, const char* utf8_str, uint16_t color, uint16_t bg_color);

private:
	ST7789_Font_t _font;
	ST7789_Colors_t _colors;
	void LcdReset();
	void WriteCommand(uint8_t cmd);
	void WriteData(uint8_t data);
	void WriteData8(uint8_t data);
	void WriteData16(uint16_t data);
};


#endif //ST7789V_LCD_DRIVER_H