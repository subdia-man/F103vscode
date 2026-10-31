#ifndef LCD_EXPLORER_H
#define LCD_EXPLORER_H

#include <string>
#include "ssd1306.h"
#include "LcdGraphics.h"
#include "LcdOutStateMachine.hpp"

#define INIT_LOAD_LINE_STARTX 				14 
#define INIT_LOAD_LINE_10_PERCENTS_STARTX 	14 
#define INIT_LOAD_LINE_10_PERCENTS_ENDX 	27	//for start init
#define INIT_LOAD_LINE_30_PERCENTS_STARTX 	14
#define INIT_LOAD_LINE_30_PERCENTS_ENDX 	47	//for usb initted
#define INIT_LOAD_LINE_70_PERCENTS_STARTX 	14
#define INIT_LOAD_LINE_70_PERCENTS_ENDX 	87	//for noFS initted 
#define INIT_LOAD_LINE_100_PERCENTS_STARTX 	14
#define INIT_LOAD_LINE_100_PERCENTS_ENDX 	117	//for initialization end  
#define INIT_LOAD_LINE_10_PERCENTS 			INIT_LOAD_LINE_10_PERCENTS_ENDX
#define INIT_LOAD_LINE_30_PERCENTS 			INIT_LOAD_LINE_30_PERCENTS_ENDX
#define INIT_LOAD_LINE_70_PERCENTS 			INIT_LOAD_LINE_70_PERCENTS_ENDX
#define INIT_LOAD_LINE_100_PERCENTS 		INIT_LOAD_LINE_100_PERCENTS_ENDX
#define INIT_LOAD_LINE_Y 					4

#define FILE_VIEW_TABLE_HEADING_Y				8
#define FILE_VIEW_TABLE_FILE_NUMBER_COLUMN_X	6

#define FILE_VIEW_DATA_FIRST_LINE				Third
#define FILE_VIEW_DATA_FIRST_INDENT				2		
#define FILE_VIEW_FILE_NUMBER_COLUMN			0		

typedef struct {
	SSD1306_Line currentLine;
	uint8_t numberColumn;
	uint8_t fileNameColumn;
} FiesView_t;

class LcdExplorer {

	LcdGraphics _lcdGraphics;
	FiesView_t _filesView;
	LcdOutStateMachine _lcdStateMachine;

public:

	LcdExplorer() {}
	~LcdExplorer() {}

	void Init();
	void ProcessLcdState();

private:

	LcdOutState LoadScreenView(LcdOutState state);

	void DrawScreenViewSystemInit();
	void DrawScreenViewFiles();

	void DrawFileViewTable();
	void DrawFileInfo(uint8_t number, std::string name, char* model);

	void DrawLoading(uint8_t percents);
	void DrawLoadingLine(uint8_t startx, uint8_t endx, uint8_t y);
	void WriteNewTextLine(uint8_t indentx, SSD1306_Line line, char* text);
	std::string ToString(uint32_t value);
	char ToChar(uint8_t value);
	void BufferToChar(char* buf, uint8_t bytesToConvert);
};

#endif