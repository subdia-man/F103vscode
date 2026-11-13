#ifndef _LCD_KEY_CONTROLLER_H
#define _LCD_KEY_CONTROLLER_H

#include "ButtonControl.hpp"
#include "LcdExplorer.hpp"

class LcdKeyController {

	LcdOutStateMachine* _lcdOutStateMachine;
	ButtonControl* _buttonControl;

public:
	LcdKeyController(LcdOutStateMachine& lcdOutStateMachine, ButtonControl& buttonControl) : 
					_lcdOutStateMachine(&lcdOutStateMachine), _buttonControl(&buttonControl) {}
	~LcdKeyController() {}
	
	void Step();

};

#endif //_LCD_KEY_CONTROLLER_H