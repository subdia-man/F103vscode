#ifndef _LCD_KEY_CONTROLLER_H
#define _LCD_KEY_CONTROLLER_H

#include "ButtonControl.hpp"
#include "ButtonDrawerAnimator.hpp"
#include "LcdExplorer.hpp"

class LcdKeyController {

	LcdOutStateMachine* _lcdOutStateMachine;
	ButtonControl* _buttonControl;
	ButtonDrawerAnimator* _buttonDrawerAnimator;

public:
	LcdKeyController(LcdOutStateMachine& lcdOutStateMachine, ButtonControl& buttonControl, ButtonDrawerAnimator& buttonDrawerAnimator) : 
					_lcdOutStateMachine(&lcdOutStateMachine), _buttonControl(&buttonControl), _buttonDrawerAnimator(&buttonDrawerAnimator) {}
	~LcdKeyController() {}
	
	void Step();

};

#endif //_LCD_KEY_CONTROLLER_H