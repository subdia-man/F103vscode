#include "LcdExplorer.hpp"
#include "CDCProtocolTransceiver.hpp"
#include "NoFsStorage.hpp"

#if (_W25QXX_USE_FREERTOS == 1)
#define LcdExplorerDelay(delay)   osDelay(delay)
#include "cmsis_os.h"
#else
#define LcdExplorerDelay(delay)   HAL_Delay(delay)
#endif

extern CDCProtocolTransceiver _cdcProtocolTransceiver;
extern NoFsStorage _noFsStorage;

void LcdExplorer::Init() {
	SSD1306_Init();
	return;
}

void LcdExplorer::LoadScreenView(ScreenView_t screenView) {
	switch (screenView) {
		case SystemLoad:
			DrawScreenViewSystemInit();
		break;
		case FilesView:
			DrawScreenViewFiles();
		break;
	}
}

void LcdExplorer::DrawScreenViewSystemInit() {
	_lcdGraphics.ClearScreen();
  	WriteNewTextLine(2, Third, "ECU OBD II emulator");
  	LcdExplorerDelay(3000);

  	_lcdGraphics.ClearScreen();
  	WriteNewTextLine(4, Third, "SYSTEM INIT...");
  	DrawLoading(INIT_LOAD_LINE_10_PERCENTS);
  	LcdExplorerDelay(1000);
	
  	WriteNewTextLine(2, Sixth, "Await for USB init");
  	while (_cdcProtocolTransceiver.GetStateOfTheStateMachine() != CDCCommandAwaiting) {}
  	DrawLoading(INIT_LOAD_LINE_30_PERCENTS);
  	LcdExplorerDelay(1000);

  	WriteNewTextLine(6, Sixth, "USB initted");
  	LcdExplorerDelay(1500);

  	WriteNewTextLine(1, Sixth, "Awaiting for storage");
  	LcdExplorerDelay(2000);

  	while(!_noFsStorage.IsInit()) {}
  	DrawLoading(INIT_LOAD_LINE_70_PERCENTS);

  	WriteNewTextLine(2, Sixth, "Int. memory initted");
  	DrawLoading(INIT_LOAD_LINE_100_PERCENTS);
  	LcdExplorerDelay(2000);

	_lcdGraphics.ClearScreen();
  	WriteNewTextLine(6, Third, "EMU READY");
	LcdExplorerDelay(2000);
	_lcdGraphics.ClearScreen();
	return;
}

void LcdExplorer::DrawLoading(uint8_t percents) {
	DrawLoadingLine(INIT_LOAD_LINE_STARTX, percents, INIT_LOAD_LINE_Y);
	return;
}

void LcdExplorer::DrawLoadingLine(uint8_t startx, uint8_t endx, uint8_t starty) {
	_lcdGraphics.DrawHorizontalLine(startx, endx, starty);
  	_lcdGraphics.DrawHorizontalLine(startx, endx, starty+1);
  	_lcdGraphics.DrawHorizontalLine(startx, endx, starty+2);
}

void LcdExplorer::WriteNewTextLine(uint8_t indentx, SSD1306_Line line, char* text) {
	_lcdGraphics.ClearString(line);
  	_lcdGraphics.SetTextPosition(line, indentx);
  	_lcdGraphics.WriteString(text, _lcdGraphics.GetCurrentFont(), _lcdGraphics.GetCurrentColor());
	return;
}

void LcdExplorer::DrawScreenViewFiles() {
	return;
}