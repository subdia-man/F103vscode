
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
		(strlen(button.name) * _lcdGraphics->GetCurrentFont().width)) / 2), 
		button.starty + ((button.finishy - button.starty - _lcdGraphics->GetCurrentFont().height) / 2));
  	_lcdGraphics->WriteString(button.name, _lcdGraphics->GetCurrentFont(), button.color);
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

void ButtonDrawerAnimator::DrawModal(modal modal) {
	_lcdGraphics->ClearScreen();
	_lcdGraphics->DrawRectangle(modal.borderStartx, modal.borderEndx, modal.borderStarty, 
								modal.borderFinishy, modal.buttonLeft.color);
	_lcdGraphics->SetCursor(modal.borderStartx + ((modal.borderEndx - modal.borderStartx - 
							(strlen(modal.text) * _lcdGraphics->GetCurrentFont().width)) / 2), 
							modal.borderStarty + ((modal.borderFinishy - modal.borderStarty - 
							_lcdGraphics->GetCurrentFont().height) / 2) 
							- _lcdGraphics->GetCurrentFont().height);
	_lcdGraphics->WriteString(modal.text, _lcdGraphics->GetCurrentFont(), modal.buttonLeft.color);
	DrawButton(modal.buttonLeft);
	DrawButton(modal.buttonRight);
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

void ButtonDrawerAnimator::ClearArea(uint8_t startx, uint8_t finishx, uint8_t starty, uint8_t finishy, SSD1306_COLOR color) {
	color = GetReversedColor(color);
	for (uint8_t i = starty; i < finishy; i++) {
		_lcdGraphics->DrawHorizontalLine(startx, finishx, i, color);
	}
	return;
}