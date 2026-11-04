
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
	_lcdGraphics->DrawRectangle(button.startx, button.finishx, button.starty, button.finishy, button.color);
	_lcdGraphics->SetCursor(button.startx + ((button.finishx - button.startx - 
		(strlen(button.name) * _lcdGraphics->GetCurrentFont().width)) / 2), 
		button.starty + ((button.finishy - button.starty - _lcdGraphics->GetCurrentFont().height) / 2));
  	_lcdGraphics->WriteString(button.name, _lcdGraphics->GetCurrentFont(), button.color);
	return;
}

void ButtonDrawerAnimator::AnimateButton(lcdButton button) {
	FillArea(button.startx, button.finishx, button.starty, button.finishy, button.color);
	BtnDriverAnimatorDelay(30);
	button.color = GetReversedColor(button.color);
	DrawButton(button);
	BtnDriverAnimatorDelay(500);
	FillArea(button.startx, button.finishx, button.starty, button.finishy, button.color);
	BtnDriverAnimatorDelay(30);
	button.color = GetReversedColor(button.color);
	DrawButton(button);
	return;
}

SSD1306_COLOR ButtonDrawerAnimator::GetReversedColor(SSD1306_COLOR color) {
	if (color == White) {
		return Black;
	} else {
		return White;
	}
	return White;
}

void ButtonDrawerAnimator::FillArea(uint8_t startx, uint8_t finishx, uint8_t starty, uint8_t finishy, 
									SSD1306_COLOR color) {
	for (uint8_t i = starty; i < finishy; i++) {
		_lcdGraphics->DrawHorizontalLine(startx, finishx, i, color);
	}
	return;
}