/*
 * LcdGraphics.c
 *
 *  Created on: Sep 18, 2026
 *      Author: subdia
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LcdGraphics.h"

void LcdGraphics::WriteChar(char ch, uint16_t color) {
    return _lcdDriver.DrawChar(ST7789v.CurrentX, ST7789v.CurrentY, ch, color, _currentBGColor);
}

void LcdGraphics::WriteString(char* str, uint16_t color) {
    return _lcdDriver.DrawString(ST7789v.CurrentX, ST7789v.CurrentY, str, color, _currentBGColor);
}

void LcdGraphics::DrawLine(uint16_t startx, uint16_t endx, 
			  uint8_t starty, uint8_t endy, uint16_t color) {
	_lcdDriver.DrawLine(startx, starty, endx, endy, color);
    return;
}

void LcdGraphics::DrawHorizontalLine(uint16_t startx, uint16_t endx, uint8_t y, uint16_t color) {
    DrawLine(startx, endx, y, y, color);
    return;
} 

void LcdGraphics::DrawVerticalLine(uint16_t x, uint16_t starty, uint8_t endy, uint16_t color) {
    DrawLine(x, x, starty, endy, color);
    return;
}

void LcdGraphics::DrawRectangle(uint16_t startx, uint16_t endx, uint8_t starty, uint8_t endy, uint16_t color) {
    _lcdDriver.DrawLine(startx, starty, endx, starty, color);
    _lcdDriver.DrawLine(startx, endy, endx, endy, color);
    _lcdDriver.DrawLine(startx, starty, startx, endy, color);
    _lcdDriver.DrawLine(endx, starty, endx, endy, color);
    return;
}

void LcdGraphics::FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    _lcdDriver.FillRect(x, y, w, h, color);
}

void LcdGraphics::ClearScreen() {
  	return _lcdDriver.FillScreen(_currentBGColor);
}

/* Service */

void LcdGraphics::SetCursor(uint8_t x, uint8_t y) {
    ST7789v.CurrentX = x;
    ST7789v.CurrentY = y;
    _lcdDriver.SetWindow(ST7789v.CurrentX, ST7789v.CurrentY, ST7789v.CurrentX, ST7789v.CurrentY);
    return;
}

void LcdGraphics::SetLine (LCD_Line line) {
	uint8_t maxLines = ST7789_HEIGHT / _font.CHAR_HEIGHT;
	if (line > maxLines) {
		return;
	} else {
		ST7789v.CurrentY = _font.CHAR_HEIGHT * line;
	}
	return;
}

void LcdGraphics::SetIndent(uint8_t chars) {
	uint8_t maxChars = ST7789_WIDTH / _font.CHAR_WIDTH;
	if (maxChars < chars + 1) {
		return;
	} else {
		ST7789v.CurrentX = _font.CHAR_WIDTH * chars;
	}
	return;
}

void LcdGraphics::SetTextPosition(LCD_Line line, uint8_t chars) {
	SetLine(line);
	SetIndent(chars);
	return;
}

void LcdGraphics::ClearString(LCD_Line line, uint8_t indent) {
        SetTextPosition(line, indent);
        _lcdDriver.FillRect(ST7789v.CurrentX, ST7789v.CurrentY, (ST7789_WIDTH - ST7789v.CurrentX), 
                            _font.CHAR_HEIGHT, _currentBGColor);
        SetTextPosition(line, indent);
        return;
}

uint16_t LcdGraphics::GetCurrentColor() {
    return _currentTextColor;
}

void LcdGraphics::SetCurrentColor(uint16_t color) {
    _currentTextColor = color;
    return;
}

uint16_t LcdGraphics::GetBackgroundColor() {
    return _currentBGColor;
}

void LcdGraphics::SetBackgroundColor(uint16_t color) {
    _currentBGColor = color;
    return;
}

uint16_t LcdGraphics::GetHighlightColor() {
    return _currentHighlightColor;
}

void LcdGraphics::SetHighlightColor (uint16_t color) {
    _currentHighlightColor = color;
    return;
}

uint8_t LcdGraphics::GetCurrentFontWidth() {
    return _font.CHAR_WIDTH;
}

uint8_t LcdGraphics::GetCurrentFontHeight() {
    return _font.CHAR_HEIGHT;
}

uint16_t LcdGraphics::GetCurrentX() {
    return ST7789v.CurrentX;
}

uint16_t LcdGraphics::GetCurrentY() {
    return ST7789v.CurrentY;
}
