/*
 * lcd_graphics.h
 *
 *  Created on: Sep 18, 2026
 *      Author: subdia
 */
#include <cstdint>
#include <cstddef>
#include "ST7789vLcdDriver.hpp"

#ifndef INC_LCD_GRAPHICS_H_
#define INC_LCD_GRAPHICS_H_

typedef enum {
	First = 0x00,
	Second = 0x01,
	Third = 0x02,
	Fourth = 0x03,
	Fifth = 0x04,
	Sixth = 0x05,
	Seventh = 0x06,
	Eighth = 0x07,
	Nineth = 0x08,
	Tenth = 0x09,
	Eleventh = 0x0A,
	Twelfth = 0x0B,
	Thirteenth = 0x0C,
	Fourteenth = 0x0D,
	Fifteenth = 0x0E,
} LCD_Line;

// Struct to store transformations
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    LCD_Line line;
    uint8_t indent;
    uint8_t Initialized;
    uint8_t DisplayOn;
} LCD_t;

class LcdGraphics {
	// Screen object
	LCD_t ST7789v;
	ST7789_Font_t _font;
	ST7789_Colors_t _colors;
	uint16_t _currentTextColor = _colors.GREEN;
	uint16_t _currentBGColor = _colors.BLACK;
	uint16_t _currentHighlightColor = _colors.MAGENTA;

	ST7789vLcdDriver _lcdDriver;

public:

	LcdGraphics() {}
	~LcdGraphics() {}

	void WriteChar(char ch, uint16_t color);
	void WriteString(char* str, uint16_t color);
	void DrawHorizontalLine(uint16_t startx, uint16_t endx, uint8_t y, uint16_t color);
	void DrawVerticalLine(uint16_t x, uint16_t starty, uint8_t endy, uint16_t color);
	void DrawLine(uint16_t startx, uint16_t endx, uint8_t starty, uint8_t endy, uint16_t color);
	void DrawRectangle(uint16_t startx, uint16_t endx, uint8_t starty, uint8_t endy, uint16_t color);
	void FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
	void ClearScreen();
	void SetTextPosition(LCD_Line line, uint8_t chars);
	void SetCursor(uint8_t x, uint8_t y);
	
	void ClearString(LCD_Line line, uint8_t indent);

	// getters and setters
	uint16_t GetCurrentColor();
	void SetCurrentColor(uint16_t color);
	uint16_t GetBackgroundColor();
	void SetBackgroundColor(uint16_t color);
	uint16_t GetHighlightColor();
	void SetHighlightColor (uint16_t color);
	uint8_t GetCurrentFontWidth();
	uint8_t GetCurrentFontHeight();
	uint16_t GetCurrentX();
	uint16_t GetCurrentY();

private:
	void SetLine (LCD_Line line);
	void SetIndent(uint8_t chars);
};
#endif /* INC_LCD_GRAPHICS_H_ */
