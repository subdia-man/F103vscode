#ifndef LCD_EXPLORER_H
#define LCD_EXPLORER_H

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

typedef enum {
	SystemLoad = 1,
	FilesView

} ScreenView_t;

class LcdExplorer {

	LcdGraphics _lcdGraphics;

public:

	LcdExplorer() {}
	~LcdExplorer() {}

	void Init();
	void LoadScreenView(ScreenView_t screenView);

private:
	void DrawScreenViewSystemInit();
	void DrawScreenViewFiles();

	void DrawLoading(uint8_t percents);
	void DrawLoadingLine(uint8_t startx, uint8_t endx, uint8_t y);
	void WriteNewTextLine(uint8_t indentx, SSD1306_Line line, char* text);
};

#endif