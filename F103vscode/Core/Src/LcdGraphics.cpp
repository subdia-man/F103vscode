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
#include "ssd1306.h"
#include "LcdGraphics.h"

void LcdGraphics::SetPixel(uint8_t x, uint8_t y, SSD1306_COLOR color) {
	 if(x >= SSD1306_X_SIZE || y >= SSD1306_Y_SIZE) {
        return;
    }
    // Draw in the right color
    if(color == White) {
        pixelBuffer[x + (y / 8) * SSD1306_X_SIZE] |= 1 << (y % 8);
    } else { 
        pixelBuffer[x + (y / 8) * SSD1306_X_SIZE] &= ~(1 << (y % 8));
    }
  	return;
}

char LcdGraphics::WriteChar(char ch, SSD1306_Font_t Font, SSD1306_COLOR color) {
    uint32_t i, b, j;
    
    // Check if character is valid
    if (ch < 32 || ch > 126)
        return 0;
    
    // Char width is not equal to font width for proportional font
    const uint8_t char_width = Font.char_width ? Font.char_width[ch-32] : Font.width;
    // Check remaining space on current line
    if (SSD1306_X_SIZE < (SSD1306.CurrentX + char_width) ||
        SSD1306_Y_SIZE < (SSD1306.CurrentY + Font.height))
    {
        // Not enough space on current line
        return 0;
    }
    
    // Use the font to write
    for(i = 0; i < Font.height; i++) {
        b = Font.data[(ch - 32) * Font.height + i];
        for(j = 0; j < char_width; j++) {
            if((b << j) & 0x8000)  {
                SetPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR) color);
            } else {
                SetPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR) !color);
            }
        }
    }
    
    // The current space is now taken
    SSD1306.CurrentX += char_width;
    UpdateScreen();
    // Return written char for validation
    return ch;
}

/* Write full string to screenbuffer */
char LcdGraphics::WriteString(char* str, SSD1306_Font_t Font, SSD1306_COLOR color) {
    while (*str) {
        if (WriteChar(*str, Font, color) != *str) {
            // Char could not be written
            return *str;
        }
        str++;
    }
    return *str;
}

void LcdGraphics::DrawHorizontalLine(uint8_t startx, uint8_t endx, uint8_t y) {
	for (uint8_t i = startx; i< endx; i++) {
		SetPixel(i, y, White);
	}
	UpdateScreen();
	return;
}

void LcdGraphics::DrawVerticalLine(uint8_t x, uint8_t starty, uint8_t endy) {
	for (uint8_t i = starty; i< endy; i++) {
		SetPixel(x, i, White);
	}
	UpdateScreen();
	return;
}

void LcdGraphics::DrawLine(uint8_t startx, uint8_t endx, 
			  uint8_t starty, uint8_t endy) {

	float kornerKff = (abs(starty - endy)/abs(startx - endx));
	for (uint8_t i = startx; i < endx; i++) {
		uint8_t yk = kornerKff*(i - endx) + endy;
		SetPixel(i, yk, White);
	}
	UpdateScreen();
	return;
}

void LcdGraphics::UpdateScreen(void) {
    for(uint8_t i = 0; i < SSD1306_Y_SIZE/8; i++) {
        SSD1306_SendCommand(0xB0 + i); // Set the current RAM page address.
        SSD1306_SendCommand(0x00 + SSD1306_X_OFFSET_LOWER);
        SSD1306_SendCommand(0x10 + SSD1306_X_OFFSET_UPPER);
        SSD1306_SendData(&pixelBuffer[SSD1306_X_SIZE*i], SSD1306_X_SIZE);
    }
}

void LcdGraphics::ClearScreen() {
  	for (uint16_t i = 0; i < SSD1306_BUFFER_SIZE; i++) {
    	pixelBuffer[i] = 0x00;
  	}
  	UpdateScreen();
  	return;
}

/* Service */

void LcdGraphics::SetCursor(uint8_t x, uint8_t y) {
    SSD1306.CurrentX = x;
    SSD1306.CurrentY = y;
}

void LcdGraphics::SetLine (SSD1306_Line line) {
	uint8_t maxLines = SSD1306_Y_SIZE / GetCurrentFont().height;
	if (line > maxLines) {
		return;
	} else {
		SSD1306.CurrentY = GetCurrentFont().height * line;
	}
	return;
}

void LcdGraphics::SetIndent(uint8_t chars) {
	uint8_t maxChars = SSD1306_X_SIZE / GetCurrentFont().width;
	if (maxChars < chars + 1) {
		return;
	} else {
		SSD1306.CurrentX = GetCurrentFont().width * chars;
	}
	return;
}

void LcdGraphics::SetTextPosition(SSD1306_Line line, uint8_t chars) {
	SetLine(line);
	SetIndent(chars);
	return;
}

void LcdGraphics::SetCurrentFont(SSD1306_Font_t font) {
	_currentFont = font;
	return;
}

SSD1306_Font_t LcdGraphics::GetCurrentFont() {
	return _currentFont;
}