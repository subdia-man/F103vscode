#ifndef LCD_EXPLORER_H
#define LCD_EXPLORER_H

#include <string>
#include "etl/map.h"
#include "ButtonDrawerAnimator.hpp"
#include "ssd1306.h"
#include "ST7789vLcdDriver.hpp"
#include "LcdGraphics.h"
#include "LcdOutStateMachine.hpp"
#include "NoFsStorage.hpp"

#define INIT_LOAD_LINE_STARTX 				10 
#define INIT_LOAD_LINE_10_PERCENTS_ENDX 	40	//for start init
#define INIT_LOAD_LINE_30_PERCENTS_ENDX 	100	//for usb initted
#define INIT_LOAD_LINE_70_PERCENTS_ENDX 	220	//for noFS initted 
#define INIT_LOAD_LINE_100_PERCENTS_ENDX 	310	//for initialization end  
#define INIT_LOAD_LINE_10_PERCENTS 			INIT_LOAD_LINE_10_PERCENTS_ENDX
#define INIT_LOAD_LINE_30_PERCENTS 			INIT_LOAD_LINE_30_PERCENTS_ENDX
#define INIT_LOAD_LINE_70_PERCENTS 			INIT_LOAD_LINE_70_PERCENTS_ENDX
#define INIT_LOAD_LINE_100_PERCENTS 		INIT_LOAD_LINE_100_PERCENTS_ENDX
#define INIT_LOAD_LINE_Y 					60
#define INIT_LOAD_LINE_WIDTH				10

#define FILE_VIEW_TABLE_HEADING_Y				16
#define FILE_VIEW_TABLE_FILE_NUMBER_COLUMN_X	8

#define FILE_VIEW_DATA_FIRST_LINE				Third
#define FILE_VIEW_DATA_FIRST_INDENT				2		
#define FILE_VIEW_FILE_NUMBER_COLUMN			0		

typedef struct {
	LCD_Line currentLine;
	uint8_t numberColumn;
	uint8_t fileNameColumn;
} FiesView_t;

extern LcdGraphics _lcdGraphics;
extern ButtonDrawerAnimator _ButtonDrawerAnimator;

class LcdExplorer {
	FiesView_t _filesView;
	LcdOutState _storedLcdMachineState = LcdOutState::Deinitted;
	LcdOutStateMachine* _lcdStateMachine;
	etl::map<LCD_Line, uint32_t, MAX_FILES_NUMBER> _screenRowObjects;

	ST7789vLcdDriver _lcdDriver;

public:

	LcdExplorer(LcdOutStateMachine& lcdOutStateMachine) : _lcdStateMachine(&lcdOutStateMachine) {}
	~LcdExplorer() {}

	void Init();
	void ProcessLcdState();
	void WriteNewTextLine(uint8_t indentx, LCD_Line line, char* text);

private:

	void LoadScreenView(LcdOutState state);

	void DrawScreenViewSystemInit();
	void DrawScreenViewFiles();
	void DrawFileDeleteModalView();

	void DrawFileViewTable();
	void DrawFileInfo(uint8_t number, std::string nameModel);

	std::string GetCarNameAndModel(uint8_t uidIndex);

	void DrawLoading(uint16_t percents);
	void DrawLoadingLine(uint16_t startx, uint16_t endx, uint8_t y);
	std::string ToString(uint32_t value);
	char ToChar(uint8_t value);
	void BufferToChar(char* buf, uint8_t bytesToConvert);
};

#endif
