#include "CDCProtocolFormer.hpp"



bool CDCProtocolFormer::IsItCommand(uint8_t cmd) {
	if (cmd == (CatchConfig | SendConfig)) {
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

