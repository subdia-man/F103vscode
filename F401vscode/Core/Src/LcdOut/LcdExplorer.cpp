
#include "LcdExplorer.hpp"
#include "CDCProtocolTransceiver.hpp"

#if (_W25QXX_USE_FREERTOS == 1)
#define LcdExplorerDelay(delay)   osDelay(delay)
#include "cmsis_os.h"
#else
#define LcdExplorerDelay(delay)   HAL_Delay(delay)
#endif

extern CDCProtocolTransceiver _cdcProtocolTransceiver;
extern NoFsStorage _noFsStorage;

void LcdExplorer::Init() {
	_lcdDriver.Init();
	memset (&_filesView, 0, sizeof(_filesView));
	_ButtonDrawerAnimator.Init();
	return;
}

void LcdExplorer::ProcessLcdState() {
	LcdOutState lcdMachineState = _lcdStateMachine->GetState(); 
	if (lcdMachineState != _storedLcdMachineState) {
		LoadScreenView(lcdMachineState);
		_storedLcdMachineState = lcdMachineState;
	}
	return;
}

void LcdExplorer::LoadScreenView(LcdOutState state) {
	_lcdGraphics.ClearScreen();
	switch (state) {
		case LcdOutState::Deinitted:
		break;
		case LcdOutState::SystemLoad:
			DrawScreenViewSystemInit();
		break;
		case LcdOutState::FilesView:
			DrawScreenViewFiles();
		break;
		case LcdOutState::FileDeleteModalView:
			DrawFileDeleteModalView();
		break;
		case LcdOutState::MenuView:
		break;
		case LcdOutState::PassOut:
		break;
		default:
			return;
		break;
	}
	return;
}

void LcdExplorer::DrawFileDeleteModalView() {
	_ButtonDrawerAnimator.DrawModal(_ButtonDrawerAnimator._fileDeleteModal);
	return;
}

void LcdExplorer::DrawScreenViewSystemInit() {
  	WriteNewTextLine(8, Third, "ECU OBD II emulator");
  	LcdExplorerDelay(3000);

  	_lcdGraphics.ClearScreen();
  	WriteNewTextLine(12, Third, "SYSTEM INIT...");
  	DrawLoading(INIT_LOAD_LINE_10_PERCENTS);
  	LcdExplorerDelay(1000);
	
  	WriteNewTextLine(8, Sixth, "Await for USB init");
  	while (_cdcProtocolTransceiver.GetStateOfTheStateMachine() != CDCCommandAwaiting) {
  		//implement some timer here
  	}
  	DrawLoading(INIT_LOAD_LINE_30_PERCENTS);
  	LcdExplorerDelay(1000);

  	WriteNewTextLine(12, Sixth, "USB initted");
  	LcdExplorerDelay(1500);

  	WriteNewTextLine(8, Sixth, "Awaiting for storage");
  	LcdExplorerDelay(2000);

  	while(!_noFsStorage.IsInit()) {
  		// and here
  	}
  	DrawLoading(INIT_LOAD_LINE_70_PERCENTS);

  	WriteNewTextLine(8, Sixth, "Int. memory initted");
  	DrawLoading(INIT_LOAD_LINE_100_PERCENTS);
  	LcdExplorerDelay(2000);

	_lcdGraphics.ClearScreen();
  	WriteNewTextLine(14, Sixth, "EMU READY");
	LcdExplorerDelay(2000);
	_lcdGraphics.ClearScreen();
	return;
}

void LcdExplorer::DrawScreenViewFiles() {
	DrawFileViewTable();
	_filesView.currentLine = FILE_VIEW_DATA_FIRST_LINE;
	_filesView.numberColumn = FILE_VIEW_FILE_NUMBER_COLUMN;
	_filesView.fileNameColumn = FILE_VIEW_DATA_FIRST_INDENT;
	for (uint8_t i = 0; i < _noFsStorage.GetNumberOfUids(); i++) {
		etl::pair<LCD_Line, uint32_t> _screenRowObjects {_filesView.currentLine, _noFsStorage.GetFileUids().at(i)};
		DrawFileInfo(i + 1, GetCarNameAndModel(i));
	}
	
	_ButtonDrawerAnimator.HighLightRow(FILE_VIEW_DATA_FIRST_LINE, 
							GetCarNameAndModel(_noFsStorage.
							GetIndexOfUid(_screenRowObjects.
							find(FILE_VIEW_DATA_FIRST_LINE)->second)));
	_ButtonDrawerAnimator.DrawButton(_ButtonDrawerAnimator.GetLcdButtons().lcdViewButton, 
									_lcdGraphics.GetBackgroundColor());
	_ButtonDrawerAnimator.DrawButton(_ButtonDrawerAnimator.GetLcdButtons().lcdDeleteButton, 
									_lcdGraphics.GetBackgroundColor());
return;
}

void LcdExplorer::DrawFileInfo(uint8_t number, std::string nameModel) {
	//insert encryption number
	_lcdGraphics.SetTextPosition(_filesView.currentLine, _filesView.numberColumn);
	_lcdGraphics.WriteChar(ToChar(number), _lcdGraphics.GetCurrentColor());

	//insert car manufacturer and model
	_lcdGraphics.SetTextPosition(_filesView.currentLine, _filesView.fileNameColumn);
  	_lcdGraphics.WriteString(nameModel.data(), _lcdGraphics.GetCurrentColor());
  	_filesView.currentLine = (LCD_Line)((uint8_t)_filesView.currentLine + 1);
	return;
}

std::string LcdExplorer::GetCarNameAndModel(uint8_t uidIndex) {
	std::string carName =  _noFsStorage.GetCarNames().at(uidIndex);
	char buf[CAR_MODEL_NAME_SIZE];
	memset(buf, 0, CAR_MODEL_NAME_SIZE);
	_noFsStorage.GetCarModelName(_noFsStorage.GetFileUids().at(uidIndex), buf);
	BufferToChar(buf, CAR_MODEL_NAME_SIZE);
	std::string fullString = carName
							.append(" ", 1)
							.append(buf, CAR_MODEL_NAME_SIZE);
	return fullString;
}

void LcdExplorer::DrawFileViewTable() {
	_lcdGraphics.DrawHorizontalLine(0, ST7789_WIDTH-1, FILE_VIEW_TABLE_HEADING_Y, _lcdGraphics.GetCurrentColor());
	_lcdGraphics.DrawVerticalLine(FILE_VIEW_TABLE_FILE_NUMBER_COLUMN_X, 0, ST7789_HEIGHT-16, _lcdGraphics.GetCurrentColor());
	_lcdGraphics.SetTextPosition(First, 0);
	_lcdGraphics.WriteChar('#', _lcdGraphics.GetCurrentColor());
	_lcdGraphics.SetTextPosition(First, 2);
	_lcdGraphics.WriteString("Cfg file", _lcdGraphics.GetCurrentColor());
	return;
}

void LcdExplorer::DrawLoading(uint16_t percents) {
	DrawLoadingLine(INIT_LOAD_LINE_STARTX, percents, INIT_LOAD_LINE_Y);
	return;
}

void LcdExplorer::DrawLoadingLine(uint16_t startx, uint16_t endx, uint8_t starty) {
	_lcdGraphics.FillRectangle(startx, starty, (endx - startx), INIT_LOAD_LINE_WIDTH, _lcdGraphics.GetCurrentColor());
}

void LcdExplorer::WriteNewTextLine(uint8_t indentx, LCD_Line line, char* text) {
	_lcdGraphics.ClearString(line, 0);
  	_lcdGraphics.SetTextPosition(line, indentx);
  	_lcdGraphics.WriteString(text, _lcdGraphics.GetCurrentColor());
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
