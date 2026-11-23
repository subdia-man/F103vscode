
#include "ST7789vLcdDriver.hpp"

#if (_ST7789_LCD_USE_FREERTOS == 1)
#define LcdDelay(delay)   osDelay(delay)
#include "cmsis_os.h"
#else
#define LcdDelay(delay)   HAL_Delay(delay)
#endif

extern SPI_HandleTypeDef hspi1;

void ST7789vLcdDriver::FillScreen(uint16_t color) {
	 FillRect(0, 0, ST7789_WIDTH, ST7789_HEIGHT, color);
	return;
}

void ST7789vLcdDriver::FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if (x + w > ST7789_WIDTH || y + h > ST7789_HEIGHT) return;
    
    SetWindow(x, y, x + w - 1, y + h - 1);
    
    uint32_t pixel_count = static_cast<uint32_t>(w) * h;
    uint8_t color_buffer[2] = {static_cast<uint8_t>((color >> 8) & 0xFF), 
                               static_cast<uint8_t>(color & 0xFF)};
    
    HAL_GPIO_WritePin(ST7789_PORT, ST7789_DC_PIN, GPIO_PIN_SET);   // DC = 1
    HAL_GPIO_WritePin(ST7789_PORT, ST7789_CS_PIN, GPIO_PIN_RESET); // CS = 0
    
    for (uint32_t i = 0; i < pixel_count; i++) {
        HAL_SPI_Transmit(&hspi1, color_buffer, 2, ST7789_Config::SPI_TIMEOUT);
    }
    
    HAL_GPIO_WritePin(ST7789_PORT, ST7789_CS_PIN, GPIO_PIN_SET);   // CS = 1
    return;
}

void ST7789vLcdDriver::SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    // Установка колонок
    WriteCommand(ST7789_Commands::CASET);
    WriteData16(x0);
    WriteData16(x1);
    
    // Установка строк
    WriteCommand(ST7789_Commands::RASET);
    WriteData16(y0);
    WriteData16(y1);
    
    // Начало записи в память
    WriteCommand(ST7789_Commands::RAMWR);
    return;
}

void ST7789vLcdDriver::DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= ST7789_WIDTH || y >= ST7789_HEIGHT) return;
    SetWindow(x, y, x, y);
    WriteData16(color);
    return;
}

void ST7789vLcdDriver::DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    int16_t dx = abs(static_cast<int16_t>(x1) - static_cast<int16_t>(x0));
    int16_t dy = abs(static_cast<int16_t>(y1) - static_cast<int16_t>(y0));
    int16_t sx = (x0 < x1) ? 1 : -1;
    int16_t sy = (y0 < y1) ? 1 : -1;
    int16_t err = dx - dy;
    
    while (true) {
        DrawPixel(x0, y0, color);
        
        if (x0 == x1 && y0 == y1) break;
        
        int16_t e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
    return;
}

void ST7789vLcdDriver::DrawChar(uint16_t x, uint16_t y, char ch, uint16_t color, uint16_t bg_color) {
    if (x + _font.CHAR_WIDTH > ST7789_WIDTH || 
        y + _font.CHAR_HEIGHT > ST7789_HEIGHT) return;
    
    const uint8_t* font_data = Font8x16_GetChar(static_cast<uint8_t>(ch));
    
    for (uint8_t row = 0; row < _font.CHAR_HEIGHT; row++) {
        uint8_t line = font_data[row];
        for (uint8_t col = 0; col < _font.CHAR_WIDTH; col++) {
            if (line & (0x80 >> col)) {
                DrawPixel(x + col, y + row, color);
            } else if (bg_color != _colors.BLACK) {
                DrawPixel(x + col, y + row, bg_color);
            }
        }
    }
    return;
}

void ST7789vLcdDriver::DrawString(uint16_t x, uint16_t y, const char* str, uint16_t color, uint16_t bg_color) {
    uint16_t current_x = x;
    const char* ptr = str;
    
    while (*ptr) {
        if (current_x + _font.CHAR_WIDTH > ST7789_WIDTH) break;
        
        uint8_t bytes_consumed;
        const uint8_t* font_data = Font8x16_GetCharUTF8(ptr, &bytes_consumed);
        
        // Рисуем символ
        for (uint8_t row = 0; row < _font.CHAR_HEIGHT; row++) {
            uint8_t line = font_data[row];
            for (uint8_t col = 0; col < _font.CHAR_WIDTH; col++) {
                if (line & (0x80 >> col)) {
                    DrawPixel(current_x + col, y + row, color);
                } else if (bg_color != _colors.BLACK) {
                    DrawPixel(current_x + col, y + row, bg_color);
                }
            }
        }
        
        current_x += _font.CHAR_WIDTH;
        ptr += bytes_consumed;
    }
    return;
}

// cyrillic
void ST7789vLcdDriver::DrawStringUTF8(uint16_t x, uint16_t y, const char* utf8_str, uint16_t color, uint16_t bg_color) {
    DrawString(x, y, utf8_str, color, bg_color);
    return;
}

void ST7789vLcdDriver::Init() {

    WriteCommand(0xB2);
    WriteData8(0x0B);
    WriteData8(0x0B);
    WriteData8(0x00);
    WriteData8(0x33);
    WriteData8(0x35);

    WriteCommand(0xB7);
    WriteData8(0x11);

    WriteCommand(0xBB);
    WriteData8(0x35);

    WriteCommand(0xC0);
    WriteData8(0x2C);

    WriteCommand(0xC2);
    WriteData8(0x01);

    WriteCommand(0xC3);
    WriteData8(0x0D);

    WriteCommand(0xC4);
    WriteData8(0x20);

    WriteCommand(0xC6);
    WriteData8(0x13);

    WriteCommand(0xD0);
    WriteData8(0xA4);
    WriteData8(0xA1);

    WriteCommand(0xD6);
    WriteData8(0xA1);

    WriteCommand(0xE0);
    WriteData8(0xF0);
    WriteData8(0x06);
    WriteData8(0x0B);
    WriteData8(0x0A);
    WriteData8(0x09);
    WriteData8(0x26);
    WriteData8(0x29);
    WriteData8(0x33);
    WriteData8(0x41);
    WriteData8(0x18);
    WriteData8(0x16);
    WriteData8(0x15);
    WriteData8(0x29);
    WriteData8(0x2D);

    WriteCommand(0xE1);
    WriteData8(0xF0);
    WriteData8(0x04);
    WriteData8(0x08);
    WriteData8(0x08);
    WriteData8(0x07);
    WriteData8(0x03);
    WriteData8(0x28);
    WriteData8(0x32);
    WriteData8(0x40);
    WriteData8(0x3B);
    WriteData8(0x19);
    WriteData8(0x18);
    WriteData8(0x2A);
    WriteData8(0x2E);

    WriteCommand(0xE4);
    WriteData8(0x25);
    WriteData8(0x00);
    WriteData8(0x00);

	LcdReset();
	LcdDelay(120);

	// sleep out
    WriteCommand(ST7789_Commands::SLPOUT);
    LcdDelay(ST7789_Config::INIT_DELAY);
    
    // color mode (RGB565)
    WriteCommand(ST7789_Commands::COLMOD);
    WriteData8(0x55);
    
    // orientation
    WriteCommand(ST7789_Commands::MADCTL);
    WriteData8(0x70);
    
    // display on
    WriteCommand(ST7789_Commands::DISPON);
    LcdDelay(100);
    
    // clear screen
    FillScreen(_colors.BLACK);
	return;
}

void ST7789vLcdDriver::LcdReset() {
	HAL_GPIO_WritePin(ST7789_PORT, ST7789_RES_PIN, GPIO_PIN_SET);
	LcdDelay(100);
	HAL_GPIO_WritePin(ST7789_PORT, ST7789_RES_PIN, GPIO_PIN_RESET);
	LcdDelay(100);
	HAL_GPIO_WritePin(ST7789_PORT, ST7789_RES_PIN, GPIO_PIN_SET);
	LcdDelay(100);
}

void ST7789vLcdDriver::WriteCommand(uint8_t cmd) {
	HAL_GPIO_WritePin(ST7789_PORT, ST7789_CS_PIN, GPIO_PIN_RESET); // CS = 0
	HAL_GPIO_WritePin(ST7789_PORT, ST7789_DC_PIN, GPIO_PIN_RESET); // DC = 0 для команды
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(ST7789_PORT, ST7789_CS_PIN, GPIO_PIN_SET);   // CS = 1
	return;
}

void ST7789vLcdDriver::WriteData(uint8_t data) {
	return;
}

void ST7789vLcdDriver::WriteData8(uint8_t data) {
	HAL_GPIO_WritePin(ST7789_PORT, ST7789_CS_PIN, GPIO_PIN_RESET); // CS = 0
	HAL_GPIO_WritePin(ST7789_PORT, ST7789_DC_PIN, GPIO_PIN_SET);   // DC = 1 для данных
    HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(ST7789_PORT, ST7789_CS_PIN, GPIO_PIN_SET);   // CS = 1
	return;
}

void ST7789vLcdDriver::WriteData16(uint16_t data) {
    uint8_t buffer[2] = {static_cast<uint8_t>((data >> 8) & 0xFF), 
                         static_cast<uint8_t>(data & 0xFF)};
    HAL_GPIO_WritePin(ST7789_PORT, ST7789_CS_PIN, GPIO_PIN_RESET); // CS = 0
    HAL_GPIO_WritePin(ST7789_PORT, ST7789_DC_PIN, GPIO_PIN_SET);   // DC = 1 для данных
    HAL_SPI_Transmit(&hspi1, buffer, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(ST7789_PORT, ST7789_CS_PIN, GPIO_PIN_SET);   // CS = 1
}
