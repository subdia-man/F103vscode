
#include <cstring>
#include "ButtonDrawerAnimator.hpp"

#if (_BDA_USE_FREERTOS == 1)
#define BtnDriverAnimatorDelay(delay)   osDelay(delay)
#include "cmsis_os.h"
#else
#define BtnDriverAnimatorDelay(delay)   HAL_Delay(delay)
#endif

void ButtonDrawerAnimator::Init() {
	_lcdButtons.lcdViewButton.color = 
	_lcdButtons.lcdDeleteButton.color = 
	_lcdButtons.lcdCancelButton.color = 
	_lcdButtons.lcdConfirmButton.color = _lcdGraphics->GetCurrentColor();
	 
	_lcdButtons.lcdViewButton.highlightedColor = 
	_lcdButtons.lcdDeleteButton.highlightedColor = 
	_lcdButtons.lcdCancelButton.highlightedColor = 
	_lcdButtons.lcdConfirmButton.highlightedColor = _lcdGraphics->GetHighlightColor();

	_fileDeleteModal.buttonLeft.color = _fileDeleteModal.buttonRight.color = _lcdGraphics->GetCurrentColor();
	_fileDeleteModal.buttonLeft.highlightedColor = 
	_fileDeleteModal.buttonRight.highlightedColor = _lcdGraphics->GetHighlightColor();
	return;
}

void ButtonDrawerAnimator::DrawButton(lcdButton button, uint16_t bgColor) {
	FillArea(button.startx, button.finishx, button.starty, button.finishy, bgColor);
	_lcdGraphics->DrawRectangle(button.startx, button.finishx, button.starty, button.finishy, button.color);
	_lcdGraphics->SetCursor(button.startx + ((button.finishx - button.startx - 
		(strlen(button.name) * _lcdGraphics->GetCurrentFontWidth())) / 2), 
		button.starty + ((button.finishy - button.starty - _lcdGraphics->GetCurrentFontHeight()) / 2));
  	_lcdGraphics->WriteString(button.name, button.color);
	return;
}

void ButtonDrawerAnimator::AnimateButton(lcdButton button) {
	DrawReversedButton(button);
	BtnDriverAnimatorDelay(500);
	DrawButton(button, _lcdGraphics->GetBackgroundColor());
	return;
}

void ButtonDrawerAnimator::DrawReversedButton(lcdButton button) {
	DrawButton(button, button.highlightedColor);
	return;
}

void ButtonDrawerAnimator::ChooseButton(lcdButton button) {
	return DrawReversedButton(button);
}

void ButtonDrawerAnimator::HighLightRow(LCD_Line row, std::string text) {
	HighLightRowArea(row, text);
	_lcdGraphics->SetTextPosition(row, 2);
	_lcdGraphics->WriteString(text.data(), _lcdGraphics->GetCurrentColor());
	return;
}

void ButtonDrawerAnimator::HighLightRowArea(LCD_Line row, std::string text) {
	uint16_t w = (_lcdGraphics->GetCurrentFontWidth() * 2 + _lcdGraphics->GetCurrentFontWidth() * text.length() - 
				_lcdGraphics->GetCurrentFontWidth() * 2 - 1);
	uint8_t h = ((_lcdGraphics->GetCurrentFontHeight() + (row * _lcdGraphics->GetCurrentFontHeight())) - 1) - 
					(_lcdGraphics->GetCurrentFontHeight() * row);
	_lcdGraphics->FillRectangle(_lcdGraphics->GetCurrentFontWidth() * 2 - 1, //startx
								_lcdGraphics->GetCurrentFontHeight() * row,		//starty
								w, h, GetHighlightColor());

	return;
}

void ButtonDrawerAnimator::DrawModal(modal modal) {
	_lcdGraphics->ClearScreen();
	_lcdGraphics->DrawRectangle(modal.borderStartx, modal.borderEndx, modal.borderStarty, 
								modal.borderFinishy, modal.buttonLeft.color);
	_lcdGraphics->SetCursor(modal.borderStartx + ((modal.borderEndx - modal.borderStartx - 
							(strlen(modal.text) * _lcdGraphics->GetCurrentFontWidth())) / 2), 
							modal.borderStarty + ((modal.borderFinishy - modal.borderStarty - 
							_lcdGraphics->GetCurrentFontHeight()) / 2) 
							- _lcdGraphics->GetCurrentFontHeight());
	_lcdGraphics->WriteString(modal.text, modal.buttonLeft.color);
	DrawButton(modal.buttonLeft, _lcdGraphics->GetBackgroundColor());
	DrawButton(modal.buttonRight, _lcdGraphics->GetBackgroundColor());
	return;
}

lcdButtons ButtonDrawerAnimator::GetLcdButtons() {
		return _lcdButtons;
}

lcdButton ButtonDrawerAnimator::GetButtonByName(char* name) {
	lcdButton button = {0, 0, 0, 0, 0, 0, ""};
	if (!strcmp(name, "View")) {
		return _lcdButtons.lcdViewButton;
	} 
	else if (!strcmp(name, "Delete")) {
		return _lcdButtons.lcdDeleteButton;
	}
	else if (!strcmp(name, "Cancel")) {
		return _lcdButtons.lcdCancelButton;
	}
	else if (!strcmp(name, "Confirm")) {
		return _lcdButtons.lcdConfirmButton;
	}
	return button;
}

uint16_t ButtonDrawerAnimator::GetReversedColor(uint16_t color) {
	if (color == _colors.GREEN) {
		return _colors.BLACK;
	} else {
		return _colors.GREEN;
	}
	return _colors.GREEN;
}

uint16_t ButtonDrawerAnimator::GetHighlightColor() {
	return _lcdGraphics->GetHighlightColor();
}

void ButtonDrawerAnimator::FillArea(uint16_t startx, uint16_t finishx, uint8_t starty, uint8_t finishy, 
									uint16_t color) {
	_lcdGraphics->FillRectangle(startx, starty, (finishx - startx), (finishy - starty), color);
	return;
}

void ButtonDrawerAnimator::ClearArea(uint16_t startx, uint16_t finishx, uint8_t starty, uint8_t finishy) {
	FillArea(startx, finishx, starty, finishy, _lcdGraphics->GetBackgroundColor());
	return;
}
