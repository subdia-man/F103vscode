#include "ButtonControl.hpp"

#ifdef _BUTTON_CONTROL_USE_FREERTOS
#define ButtonControlDelay(delay)   osDelay(delay)
#include "cmsis_os.h"
#else
#define ButtonControlDelay(delay)   HAL_Delay(delay)
#endif

Button ButtonControl::GetKeyPressed() {
	Button button = Button::NoOne;
	switch(Scan()) {
		case 10:
		button = Button::Zero;
		break;
		case 1:
		button = Button::One;
		break;
		case 2:
		button = Button::Two;
		break;
		case 3:
		button = Button::Three;
		break;
		case 4:
		button = Button::Four;
		break;
		case 5:
		button = Button::Five;
		break;
		case 6:
		button = Button::Six;
		break;
		case 7:
		button = Button::Seven;
		break;
		case 8:
		button = Button::Eight;
		break;
		case 9:
		button = Button::Nine;
		break;
		case 71:
		button = Button::AsteriskButton;
		break;
		case 91:
		button = Button::SharpButton;
		break;
		case 11:
		button = Button::Abutton;
		break;
		case 21:
		button = Button::Bbutton;
		break;
		case 31:
		button = Button::Cbutton;
		break;
		case 41:
		button = Button::Dbutton;
		break;
		case 0xFF:
		button = Button::NoOne;
		break;
		default:
		button = Button::NoOne;
		break;
	}
	_lastPressed = button;
	return _lastPressed;
}

uint8_t ButtonControl::Scan() {
   	return ScanColumnOne() | ScanColumnTwo() | ScanColumnThree() | ScanColumnFour();
}

uint8_t ButtonControl::ScanColumnOne() {
	uint8_t key = 0;
	HAL_GPIO_WritePin(Column_Port, C1_Pin, PIN_RESET);
    	if (HAL_GPIO_ReadPin(Row_Port, R1_Pin) == GPIO_PIN_RESET) {
    		key = 1;
    	}
    	else if (HAL_GPIO_ReadPin(Row_Port, R2_Pin) == GPIO_PIN_RESET) {
    		key = 4;
    	}
    	else if (HAL_GPIO_ReadPin(Row_Port, R3_Pin) == GPIO_PIN_RESET) {
    		key = 7;
    	}
    	else if (HAL_GPIO_ReadPin(Row_Port, R4_Pin) == GPIO_PIN_RESET) {
    		key = 71;
    	}
    HAL_GPIO_WritePin(Column_Port, C1_Pin, PIN_SET);
    ButtonControlDelay(KEYPRESS_DELAY);
    return key;
}

uint8_t ButtonControl::ScanColumnTwo() {
	uint8_t key = 0;
	HAL_GPIO_WritePin(Column_Port, C2_Pin, PIN_RESET);
    	if (HAL_GPIO_ReadPin(Row_Port, R1_Pin) == GPIO_PIN_RESET) {
    		key = 2;
    	}
    	else if (HAL_GPIO_ReadPin(Row_Port, R2_Pin) == GPIO_PIN_RESET) {
    		key = 5;
    	}
    	else if (HAL_GPIO_ReadPin(Row_Port, R3_Pin) == GPIO_PIN_RESET) {
    		key = 8;
    	}
    	else if (HAL_GPIO_ReadPin(Row_Port, R4_Pin) == GPIO_PIN_RESET) {
    		key = 10;
    	}
    HAL_GPIO_WritePin(Column_Port, C2_Pin, PIN_SET);
    ButtonControlDelay(KEYPRESS_DELAY);
    return key;
}

uint8_t ButtonControl::ScanColumnThree() {
	uint8_t key = 0;
	HAL_GPIO_WritePin(Column_Port, C3_Pin, PIN_RESET);
    	if (HAL_GPIO_ReadPin(Row_Port, R1_Pin) == GPIO_PIN_RESET) {
    		key = 3;
    	}
    	else if (HAL_GPIO_ReadPin(Row_Port, R2_Pin) == GPIO_PIN_RESET) {
    		key = 6;
    	}
    	else if (HAL_GPIO_ReadPin(Row_Port, R3_Pin) == GPIO_PIN_RESET) {
    		key = 9;
    	}
    	else if (HAL_GPIO_ReadPin(Row_Port, R4_Pin) == GPIO_PIN_RESET) {
    		key = 91;
    	}
    HAL_GPIO_WritePin(Column_Port, C3_Pin, PIN_SET);
    ButtonControlDelay(KEYPRESS_DELAY);
    return key;
}

uint8_t ButtonControl::ScanColumnFour() {
	uint8_t key = 0;
	HAL_GPIO_WritePin(Column_Port, C4_Pin, PIN_RESET);
    	if (HAL_GPIO_ReadPin(Row_Port, R1_Pin) == GPIO_PIN_RESET) {
    		key = 11;
    	}
    	else if (HAL_GPIO_ReadPin(Row_Port, R2_Pin) == GPIO_PIN_RESET) {
    		key = 21;
    	}
    	else if (HAL_GPIO_ReadPin(Row_Port, R3_Pin) == GPIO_PIN_RESET) {
    		key = 31;
    	}
    	else if (HAL_GPIO_ReadPin(Row_Port, R4_Pin) == GPIO_PIN_RESET) {
    		key = 41;
    	}
    HAL_GPIO_WritePin(Column_Port, C4_Pin, PIN_SET);
    ButtonControlDelay(KEYPRESS_DELAY);
    return key;
}