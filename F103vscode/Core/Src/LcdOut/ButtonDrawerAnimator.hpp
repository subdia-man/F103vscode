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
#define LCD_CONFIRM_BUTTON_STARTY 	32
#define LCD_CONFIRM_BUTTON_FINISHY 	42

#define LCD_CANCEL_BUTTON_STARTX 	69
#define LCD_CANCEL_BUTTON_FINISHX 	119
#define LCD_CANCEL_BUTTON_STARTY 	32
#define LCD_CANCEL_BUTTON_FINISHY 	42

#define LCD_FILE_DELETE_MODAL_STARTX 	4
#define LCD_FILE_DELETE_MODAL_FINISHX 	123
#define LCD_FILE_DELETE_MODAL_STARTY 	8
#define LCD_FILE_DELETE_MODAL_FINISHY 	48

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

struct modal {
	uint8_t borderStartx;
	uint8_t borderEndx;
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
	void DrawButton(lcdButton button);
	void AnimateButton(lcdButton button);
	void DrawReversedButton(lcdButton button);
	void ChooseButton(lcdButton button);

	void DrawModal(modal modal);

	lcdButtons GetLcdButtons() {
		return _lcdButtons;
	}

private:
	uint8_t pixelBufferBackup[SSD1306_BUFFER_SIZE];

	LcdGraphics* _lcdGraphics;
	void FillArea(uint8_t startx, uint8_t finishx, uint8_t starty, uint8_t finishy, SSD1306_COLOR color);
	void ClearArea(uint8_t startx, uint8_t finishx, uint8_t starty, uint8_t finishy, SSD1306_COLOR color);
	SSD1306_COLOR GetReversedColor(SSD1306_COLOR color);
	
};


#endif //_BUTTON_DRAWER_ANIMATOR_H