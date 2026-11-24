
#include "LcdKeyController.hpp"


void LcdKeyController::Step() {
	Button button = _buttonControl->Filter();
	LcdOutState state = _lcdOutStateMachine -> GetState();
	switch (state) {
		case LcdOutState::Deinitted:
			_lcdOutStateMachine -> SetState(LcdOutState::SystemLoad);
		break;
		case LcdOutState::SystemLoad:
			_lcdOutStateMachine -> SetState(LcdOutState::FilesView);
		break;
		case LcdOutState::FilesView:
			if(button == Button::Escape) { //delete
				_lcdOutStateMachine -> SetState(LcdOutState::FileDeleteModalView);
			} 
			else if (button == Button::Enter) { //file view
				//view file
			}
		break;
		case LcdOutState::FileDeleteModalView:
			if(button == Button::Enter) {
				//delete file
				//return to the FilesView state
			}
			else if (button == Button::Escape) {
				//return to the FilesView state
				_lcdOutStateMachine -> SetState(LcdOutState::FilesView);
			}
		break;
		case LcdOutState::MenuView:
		break;
		case LcdOutState::PassOut:
		break;
	}
	
	return;
}
