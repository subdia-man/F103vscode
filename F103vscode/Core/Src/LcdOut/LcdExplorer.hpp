#ifndef LCD_EXPLORER_H
#define LCD_EXPLORER_H

#include <string>
#include "ssd1306.h"
#include "LcdGraphics.h"

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

typedef enum {
	SystemLoad = 1,
	FilesView

} ScreenView_t;

typedef struct {
	SSD1306_Line currentLine;
	uint8_t numberColumn;
	uint8_t fileNameColumn;
} FiesView_t;

class LcdExplorer {

	LcdGraphics _lcdGraphics;
	FiesView_t _filesView;

public:

	LcdExplorer() {}
	~LcdExplorer() {}

	void Init();
	void LoadScreenView(ScreenView_t screenView);

private:
	void DrawScreenViewSystemInit();
	void DrawScreenViewFiles();

	void DrawFileViewTable();
	void DrawFileInfo(uint8_t number, uint32_t uid);

	void DrawLoading(uint8_t percents);
	void DrawLoadingLine(uint8_t startx, uint8_t endx, uint8_t y);
	void WriteNewTextLine(uint8_t indentx, SSD1306_Line line, char* text);
	std::string ToString(uint32_t value);
	char ToChar(uint8_t value);
};

#endif