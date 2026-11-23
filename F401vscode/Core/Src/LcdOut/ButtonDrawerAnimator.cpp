
#include <cstring>
#include "ButtonDrawerAnimator.hpp"

#if (_BDA_USE_FREERTOS == 1)
#define BtnDriverAnimatorDelay(delay)   osDelay(delay)
#include "cmsis_os.h"
#else
#define BtnDriverAnimatorDelay(delay)   HAL_Delay(delay)
#endif

void ButtonDrawerAnimator::Init() {
	_lcdButtons.lcdViewButton.color = _lcdGraphics->GetCurrentColor();
	_lcdButtons.lcdDeleteButton.color = _lcdGraphics->GetCurrentColor();
	_lcdButtons.lcdCancelButton.color = _lcdGraphics->GetCurrentColor();
	_lcdButtons.lcdConfirmButton.color = _lcdGraphics->GetCurrentColor();
	return;
}

void ButtonDrawerAnimator::DrawButton(lcdButton button) {
	ClearArea(button.startx, button.finishx, button.starty, button.finishy, button.color);
	_lcdGraphics->DrawRectangle(button.startx, button.finishx, button.starty, button.finishy, button.color);
	_lcdGraphics->SetCursor(button.startx + ((button.finishx - button.startx - 
		(strlen(button.name) * _lcdGraphics->GetCurrentFontWidth())) / 2), 
		button.starty + ((button.finishy - button.starty - _lcdGraphics->GetCurrentFontHeight()) / 2));
  	_lcdGraphics->WriteString(_lcdGraphics->GetCurrentX(), _lcdGraphics->GetCurrentY(), button.name, button.color);
	return;
}

void ButtonDrawerAnimator::AnimateButton(lcdButton button) {
	DrawReversedButton(button);
	BtnDriverAnimatorDelay(500);
	DrawButton(button);
	return;
}

void ButtonDrawerAnimator::DrawReversedButton(lcdButton button) {
	FillArea(button.startx, button.finishx, button.starty, button.finishy, button.color);
	button.color = GetReversedColor(button.color);
	DrawButton(button);
	return;
}

void ButtonDrawerAnimator::ChooseButton(lcdButton button) {
	return DrawReversedButton(button);
}

void ButtonDrawerAnimator::HighLightRow(LCD_Line row, std::string text) {
	HighLightRowArea(row, text);
	_lcdGraphics->SetTextPosition(row, 2);
	_lcdGraphics->WriteString(_lcdGraphics->GetCurrentX(), _lcdGraphics->GetCurrentY(),
								text.data(), GetReversedColor(_lcdGraphics->GetCurrentColor()));
	return;
}

void ButtonDrawerAnimator::HighLightRowArea(LCD_Line row, std::string text) {
	_lcdGraphics->DrawRectangle(_lcdGraphics->GetCurrentFontWidth() * 2 - 1, 
								_lcdGraphics->GetCurrentFontWidth() * 2 + 
								_lcdGraphics->GetCurrentFontWidth() * text.length(), 
								_lcdGraphics->GetCurrentFontHeight() * row, 
								(_lcdGraphics->GetCurrentFontHeight() + 
								(row * _lcdGraphics->GetCurrentFontHeight())) - 1, 
								_lcdGraphics->GetCurrentColor());
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
	_lcdGraphics->WriteString(_lcdGraphics->GetCurrentX(), _lcdGraphics->GetCurrentY(), modal.text, modal.buttonLeft.color);
	DrawButton(modal.buttonLeft);
	DrawButton(modal.buttonRight);
	return;
}

uint16_t ButtonDrawerAnimator::GetReversedColor(uint16_t color) {
	if (color == _colors.GREEN) {
		return _colors.BLACK;
	} else {
		return _colors.GREEN;
	}
	return _colors.GREEN;
}

void ButtonDrawerAnimator::FillArea(uint16_t startx, uint16_t finishx, uint8_t starty, uint8_t finishy, 
									uint16_t color) {
	for (uint8_t i = starty; i < finishy; i++) {
		_lcdGraphics->DrawHorizontalLine(startx, finishx, i, color);
	}
	return;
}

void ButtonDrawerAnimator::ClearArea(uint16_t startx, uint16_t finishx, uint8_t starty, uint8_t finishy, uint16_t color) {
	color = GetReversedColor(color);
	for (uint8_t i = starty; i < finishy; i++) {
		_lcdGraphics->DrawHorizontalLine(startx, finishx, i, color);
	}
	return;
}