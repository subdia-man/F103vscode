#include "CDCProtocolFormer.hpp"



bool CDCProtocolFormer::IsItCommand(uint8_t cmd) {
	if ((cmd == CDCCommands::SendConfig) | (cmd == CDCCommands::CatchConfig)) {
		return true;
	}
	return false;
}

CDCCommands CDCProtocolFormer::GetCommandFromFrame(etl::array<uint8_t, COMMAND_FRAME_MAX_LENGTH> frame) {
	if (frame.at(CDC_PROTOCOL_CMD_PLACE) == CDCCommands::SendConfig) {
		return CDCCommands::SendConfig;
	}
	else if (frame.at(CDC_PROTOCOL_CMD_PLACE) == CDCCommands::CatchConfig) {
		return CDCCommands::CatchConfig;
	}
	else return CDCCommands::Undefined;
}

void CDCProtocolFormer::FormOkFrame(uint8_t* buf) {
	memset(buf, 0, sizeof(uint8_t)*COMMAND_FRAME_MAX_LENGTH);
	buf[CDC_PROTOCOL_HEADER_PLACE] = CDC_PROTOCOL_HEADER;
	buf[CDC_PROTOCOL_RES_PLACE] = CDC_PROTOCOL_OK_RESULT;
	return;
}

void CDCProtocolFormer::FormFailFrame(uint8_t* buf) {
	memset(buf, 0, sizeof(uint8_t)*COMMAND_FRAME_MAX_LENGTH);
	buf[CDC_PROTOCOL_HEADER_PLACE] = CDC_PROTOCOL_HEADER;
	buf[CDC_PROTOCOL_RES_PLACE] = CDC_PROTOCOL_FAIL_RESULT;
	return;
}