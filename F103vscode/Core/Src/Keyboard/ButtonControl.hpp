#ifndef _BUTTON_CONTROL_H
#define _BUTTON_CONTROL_H

#include <cstdint>
#include "stm32f1xx_hal.h"

#ifndef _BUTTON_CONTROL_USE_FREERTOS
#define _BUTTON_CONTROL_USE_FREERTOS
#endif //_BUTTON_CONTROL_USE_FREERTOS

#define C1_Pin 		GPIO_PIN_1  	// PA1 - output - pin2 - C1 Column1 (Keys 1, 4, 7, *)
#define C2_Pin 		GPIO_PIN_2  	// PA2 - output - pin3 - C2 Column2 (Keys 2, 5, 8, 0)
#define C3_Pin 		GPIO_PIN_3  	// PA3 - output - pin4 - C3 Column3 (Keys 3, 6, 9, #)
#define C4_Pin 		GPIO_PIN_4  	// PA4 - output - pin5 - C4 Column4 (Keys A, B, C, D)
#define Column_Port GPIOA

#define R1_Pin 		GPIO_PIN_4  	// PB4 - input - pin6 - R1 Row1 (Keys 1, 2, 3, A)
#define R2_Pin 		GPIO_PIN_5  	// PB5 - input - pin7 - R2 Row2 (Keys 4, 5, 6, B)
#define R3_Pin 		GPIO_PIN_6  	// PB6 - input - pin8 - R3 Row3 (Keys 7, 8, 9, C)
#define R4_Pin 		GPIO_PIN_7  	// PB7 - input - pin9 - R4 Row4 (Keys *, 0, #, D)
#define Row_Port 	GPIOB

#define PIN_SET 	GPIO_PIN_SET 
#define PIN_RESET 	GPIO_PIN_RESET
#define KEYPRESS_DELAY	20

enum Button {
	NoOne,
	One,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	Zero,
	Abutton,
	Bbutton,
	Cbutton,
	Dbutton,
	AsteriskButton, //*
	SharpButton //#
};

class ButtonControl {

	Button _lastPressed = NoOne;

public:

	ButtonControl() {}
	~ButtonControl() {}

	Button GetKeyPressed();
	uint8_t Scan();

private:

	uint8_t ScanColumnOne();
	uint8_t ScanColumnTwo();
	uint8_t ScanColumnThree();
	uint8_t ScanColumnFour();
	
};

#endif //_BUTTON_CONTROL_H