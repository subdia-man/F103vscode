
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
	memset (&_filesView, 0, sizeof(_filesView));
	return;
}

void LcdExplorer::ProcessLcdState() {
	LcdOutState lcdMachineState = LoadScreenView(_lcdStateMachine.GetState());
	if (lcdMachineState != _lcdStateMachine.GetState()) {
		_lcdStateMachine.SetState(lcdMachineState);
	}
	return;
}

LcdOutState LcdExplorer::LoadScreenView(LcdOutState state) {
	switch (state) {
		case LcdOutState::SystemLoad:
			DrawScreenViewSystemInit();
			return FilesView;
		break;
		case LcdOutState::FilesView:
			DrawScreenViewFiles();
			return PassOut;
		break;
		case LcdOutState::MenuView:
			return PassOut;
		break;
		case LcdOutState::PassOut:
			return PassOut;
		break;
		default:
			return PassOut;
		break;
	}
	return PassOut;
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
  	while (_cdcProtocolTransceiver.GetStateOfTheStateMachine() != CDCCommandAwaiting) {
  		//implement some timer here
  	}
  	DrawLoading(INIT_LOAD_LINE_30_PERCENTS);
  	LcdExplorerDelay(1000);

  	WriteNewTextLine(6, Sixth, "USB initted");
  	LcdExplorerDelay(1500);

  	WriteNewTextLine(1, Sixth, "Awaiting for storage");
  	LcdExplorerDelay(2000);

  	while(!_noFsStorage.IsInit()) {
  		// and here
  	}
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

void LcdExplorer::DrawScreenViewFiles() {
	DrawFileViewTable();
	_filesView.currentLine = FILE_VIEW_DATA_FIRST_LINE;
	_filesView.numberColumn = FILE_VIEW_FILE_NUMBER_COLUMN;
	_filesView.fileNameColumn = FILE_VIEW_DATA_FIRST_INDENT;
	etl::array<std::string, MAX_FILES_NUMBER> carNames =  _noFsStorage.GetCarNames();
	etl::array<uint32_t, MAX_FILES_NUMBER> fUids = _noFsStorage.GetFileUids();
	char buf[CAR_MODEL_NAME_SIZE];
	memset(buf, 0, CAR_MODEL_NAME_SIZE);
	for (uint8_t i = 0; i < _noFsStorage.GetNumberOfUids(); i++) {
		_noFsStorage.GetCarModelName(fUids.at(i), buf);
		BufferToChar(buf, CAR_MODEL_NAME_SIZE);
		DrawFileInfo(i + 1, carNames.at(i), buf);
	}
return;
}

void LcdExplorer::DrawFileInfo(uint8_t number, std::string name, char* model) {
	//insert encryption number
	_lcdGraphics.SetTextPosition(_filesView.currentLine, _filesView.numberColumn);
	_lcdGraphics.WriteChar(ToChar(number), _lcdGraphics.GetCurrentFont(), _lcdGraphics.GetCurrentColor());

	//insert car manufacturer and model
	_lcdGraphics.SetTextPosition(_filesView.currentLine, _filesView.fileNameColumn);
  	_lcdGraphics.WriteString(name.data(), _lcdGraphics.GetCurrentFont(), _lcdGraphics.GetCurrentColor());
  	_lcdGraphics.WriteChar(' ', _lcdGraphics.GetCurrentFont(), _lcdGraphics.GetCurrentColor());
  	_lcdGraphics.WriteString(model, _lcdGraphics.GetCurrentFont(), _lcdGraphics.GetCurrentColor());
  	_filesView.currentLine = (SSD1306_Line)((uint8_t)_filesView.currentLine + 1);
	return;
}

void LcdExplorer::DrawFileViewTable() {
	_lcdGraphics.DrawHorizontalLine(0, SSD1306_X_SIZE-1, FILE_VIEW_TABLE_HEADING_Y);
	_lcdGraphics.DrawVerticalLine(FILE_VIEW_TABLE_FILE_NUMBER_COLUMN_X, 0, SSD1306_Y_SIZE-1);
	_lcdGraphics.SetTextPosition(First, 0);
	_lcdGraphics.WriteChar('#', _lcdGraphics.GetCurrentFont(), _lcdGraphics.GetCurrentColor());
	_lcdGraphics.SetTextPosition(First, 2);
	_lcdGraphics.WriteString("Cfg file", _lcdGraphics.GetCurrentFont(), _lcdGraphics.GetCurrentColor());
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

std::string LcdExplorer::ToString(uint32_t value) {
    return std::to_string(value);
}

char LcdExplorer::ToChar(uint8_t value) {
	return (char)value + 0x30;
}

void LcdExplorer::BufferToChar(char* buf, uint8_t bytesToConvert) {
	for (uint8_t i = 0; i < bytesToConvert; i++) {
		buf[i] = ToChar(buf[i]);
	}
	return;
}