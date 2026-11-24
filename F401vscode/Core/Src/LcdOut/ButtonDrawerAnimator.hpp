#ifndef _BUTTON_DRAWER_ANIMATOR_H
#define _BUTTON_DRAWER_ANIMATOR_H

#include <string>
#include "LcdGraphics.h"

#define _BDA_USE_FREERTOS     1

#define LCD_VIEW_BUTTON_STARTX 		20
#define LCD_VIEW_BUTTON_FINISHX 	139
#define LCD_VIEW_BUTTON_STARTY 		200
#define LCD_VIEW_BUTTON_FINISHY 	229

#define LCD_DELETE_BUTTON_STARTX 	180
#define LCD_DELETE_BUTTON_FINISHX 	299
#define LCD_DELETE_BUTTON_STARTY 	200
#define LCD_DELETE_BUTTON_FINISHY 	229

#define LCD_CONFIRM_BUTTON_STARTX 	30
#define LCD_CONFIRM_BUTTON_FINISHX 	149
#define LCD_CONFIRM_BUTTON_STARTY 	140
#define LCD_CONFIRM_BUTTON_FINISHY 	169

#define LCD_CANCEL_BUTTON_STARTX 	170
#define LCD_CANCEL_BUTTON_FINISHX 	289
#define LCD_CANCEL_BUTTON_STARTY 	140
#define LCD_CANCEL_BUTTON_FINISHY 	169

#define LCD_FILE_DELETE_MODAL_STARTX 	20
#define LCD_FILE_DELETE_MODAL_FINISHX 	299
#define LCD_FILE_DELETE_MODAL_STARTY 	20
#define LCD_FILE_DELETE_MODAL_FINISHY 	179

struct lcdButton {
	uint16_t startx;
	uint16_t finishx;
	uint8_t starty;
	uint8_t finishy;
	uint16_t color;
	uint16_t highlightedColor;
	char* name;
};

struct lcdButtons {
	lcdButton lcdViewButton = {
		LCD_VIEW_BUTTON_STARTX,
		LCD_VIEW_BUTTON_FINISHX,
		LCD_VIEW_BUTTON_STARTY,
		LCD_VIEW_BUTTON_FINISHY,
		0, //by default
		0,
		"View"
	};

	lcdButton lcdDeleteButton = {
		LCD_DELETE_BUTTON_STARTX,
		LCD_DELETE_BUTTON_FINISHX,
		LCD_DELETE_BUTTON_STARTY,
		LCD_DELETE_BUTTON_FINISHY,
		0, //by default
		0,
		"Delete"
	};

	lcdButton lcdCancelButton = {
		LCD_CANCEL_BUTTON_STARTX,
		LCD_CANCEL_BUTTON_FINISHX,
		LCD_CANCEL_BUTTON_STARTY,
		LCD_CANCEL_BUTTON_FINISHY,
		0, //by default
		0,
		"Cancel"
	};

	lcdButton lcdConfirmButton = {
		LCD_CONFIRM_BUTTON_STARTX,
		LCD_CONFIRM_BUTTON_FINISHX,
		LCD_CONFIRM_BUTTON_STARTY,
		LCD_CONFIRM_BUTTON_FINISHY,
		0, //by default
		0,
		"Confirm"
	};
	
};

struct modal {
	uint16_t borderStartx;
	uint16_t borderEndx;
	uint8_t borderStarty;
	uint8_t borderFinishy;
	lcdButton buttonLeft;
	lcdButton buttonRight;
	char* 	text;
};

class ButtonDrawerAnimator {

	lcdButtons _lcdButtons;
	

public:
	modal _fileDeleteModal = {
		LCD_FILE_DELETE_MODAL_STARTX,
		LCD_FILE_DELETE_MODAL_FINISHX,
		LCD_FILE_DELETE_MODAL_STARTY,
		LCD_FILE_DELETE_MODAL_FINISHY,
		_lcdButtons.lcdConfirmButton,
		_lcdButtons.lcdCancelButton,
		"Are you sure?"
	};

	ButtonDrawerAnimator(LcdGraphics& lcdGraphics) : _lcdGraphics(&lcdGraphics) {}
	~ButtonDrawerAnimator() {}

	void Init();
	void DrawButton(lcdButton button, uint16_t bgColor);
	void AnimateButton(lcdButton button);
	void DrawReversedButton(lcdButton button);
	void ChooseButton(lcdButton button);
	void HighLightRow(LCD_Line row, std::string text);
	void HighLightRowArea(LCD_Line row, std::string text);

	void DrawModal(modal modal);

	lcdButtons GetLcdButtons() {
		return _lcdButtons;
	}

private:

	LcdGraphics* _lcdGraphics;
	ST7789_Colors_t _colors;

	void FillArea(uint16_t startx, uint16_t finishx, uint8_t starty, uint8_t finishy, uint16_t color);
	void ClearArea(uint16_t startx, uint16_t finishx, uint8_t starty, uint8_t finishy);
	uint16_t GetReversedColor(uint16_t color);
	uint16_t GetHighlightColor();
	
};


#endif //_BUTTON_DRAWER_ANIMATOR_H
