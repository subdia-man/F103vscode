
#include "LcdKeyController.hpp"


void LcdKeyController::Step() {
	Button button = _buttonControl->Filter();
	switch (_lcdOutStateMachine -> GetState()) {
		case LcdOutState::FilesView:
			if(button == Button::Enter) {
				_lcdOutStateMachine -> SetState(LcdOutState::FileDeleteModalView);
			}
		break;
		case LcdOutState::FileDeleteModalView:
			if(button == Button::Enter) {
				//delete file
				//return to the FilesView state
			}
			else if (button == Button::Escape) {
				//return to the FilesView state
			}
		break;
	}
	
	return;
}