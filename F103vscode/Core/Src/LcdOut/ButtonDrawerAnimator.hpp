#ifndef _BUTTON_DRAWER_ANIMATOR_H
#define _BUTTON_DRAWER_ANIMATOR_H

#include <string>
#include "LcdGraphics.h"

#define _BDA_USE_FREERTOS     1

#define LCD_VIEW_BUTTON_STARTX 		8
#define LCD_VIEW_BUTTON_FINISHX 	58
#define LCD_VIEW_BUTTON_STARTY 		53
#define LCD_VIEW_BUTTON_FINISHY 	63

#define LCD_DELETE_BUTTON_STARTX 	69
#define LCD_DELETE_BUTTON_FINISHX 	119
#define LCD_DELETE_BUTTON_STARTY 	53
#define LCD_DELETE_BUTTON_FINISHY 	63

#define LCD_CONFIRM_BUTTON_STARTX 	8
#define LCD_CONFIRM_BUTTON_FINISHX 	58
#define LCD_CONFIRM_BUTTON_STARTY 	26
#define LCD_CONFIRM_BUTTON_FINISHY 	36

#define LCD_CANCEL_BUTTON_STARTX 	69
#define LCD_CANCEL_BUTTON_FINISHX 	119
#define LCD_CANCEL_BUTTON_STARTY 	26
#define LCD_CANCEL_BUTTON_FINISHY 	36

struct lcdButton {
	uint8_t startx;
	uint8_t finishx;
	uint8_t starty;
	uint8_t finishy;
	SSD1306_COLOR color;
	char* name;
};

struct lcdButtons {
	lcdButton lcdViewButton = {
		LCD_VIEW_BUTTON_STARTX,
		LCD_VIEW_BUTTON_FINISHX,
		LCD_VIEW_BUTTON_STARTY,
		LCD_VIEW_BUTTON_FINISHY,
		White, //by default
		"View"
	};

	lcdButton lcdDeleteButton = {
		LCD_DELETE_BUTTON_STARTX,
		LCD_DELETE_BUTTON_FINISHX,
		LCD_DELETE_BUTTON_STARTY,
		LCD_DELETE_BUTTON_FINISHY,
		White, //by default
		"Delete"
	};

	lcdButton lcdCancelButton = {
		LCD_CANCEL_BUTTON_STARTX,
		LCD_CANCEL_BUTTON_FINISHX,
		LCD_CANCEL_BUTTON_STARTY,
		LCD_CANCEL_BUTTON_FINISHY,
		White, //by default
		"Cancel"
	};

	lcdButton lcdConfirmButton = {
		LCD_CONFIRM_BUTTON_STARTX,
		LCD_CONFIRM_BUTTON_FINISHX,
		LCD_CONFIRM_BUTTON_STARTY,
		LCD_CONFIRM_BUTTON_FINISHY,
		White, //by default
		"Confirm"
	};
	
};

class ButtonDrawerAnimator {

	lcdButtons _lcdButtons;

public:

	ButtonDrawerAnimator(LcdGraphics& lcdGraphics) : _lcdGraphics(&lcdGraphics) {}
	~ButtonDrawerAnimator() {}

	void Init();
	void DrawButton(lcdButton button);
	void AnimateButton(lcdButton button);

	void FillArea(uint8_t startx, uint8_t finishx, uint8_t starty, uint8_t finishy, SSD1306_COLOR color);
	SSD1306_COLOR GetReversedColor(SSD1306_COLOR color);

	lcdButtons GetLcdButtons() {
		return _lcdButtons;
	}

private:

	LcdGraphics* _lcdGraphics;
	
};


#endif //_BUTTON_DRAWER_ANIMATOR_H