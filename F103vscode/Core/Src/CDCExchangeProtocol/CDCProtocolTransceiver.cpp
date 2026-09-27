
#include "CDCProtocolTransceiver.hpp"

int8_t CDCProtocolTransceiver::Process() {
	switch (_cdcStateMachine.GetState()) {
		case CDCFree:
		//send FAIL frame here
		return RES_FAIL; //communication state can't be free at this moment
		case CDCCommandAwaiting:
		//is this a command?
		if (_cdcProtocolFormer.IsItCommand(_txrxBuffer.at(CDC_PROTOCOL_CMD_PLACE))) {
			if (_cdcProtocolFormer.GetCommandFromFrame(_txrxBuffer) == 
				CDCCommands::SendConfig) {
				//send OK frame
				//send config transmittion command
				//pack data and send
				_cdcStateMachine.SetState(CDCFileTransmitting);
			}
			else if (_cdcProtocolFormer.GetCommandFromFrame(_txrxBuffer) == 
				CDCCommands::CatchConfig) {
				//extract data about config flow here
				//send OK frame
				_cdcStateMachine.SetState(CDCFileReceiving);
			}
		}
		else return RES_FAIL;
		//if yes - recognize command
		//switch state machine
		//if no - return RES_FAIL
		break;
		case CDCFileReceiving:
		//get packet
		//parse?
		//write to the file
		break;
		case CDCFileTransmitting:
		//form packet
		//transmit
		break;
		default:
		break;
	}
	return RES_OK;
}

int8_t CDCProtocolTransceiver::StoreMsgToIncoming(etl::array<uint8_t, COMMAND_FRAME_MAX_LENGTH> packet) {
	_rxTransceiverQueue.push(packet);
	return RES_OK;
}

int8_t CDCProtocolTransceiver::StoreMsgToOutgoing(etl::array<uint8_t, COMMAND_FRAME_MAX_LENGTH> packet) {
	_txTransceiverQueue.push(packet);
	return RES_OK;
}

int8_t CDCProtocolTransceiver::Receive(uint8_t* buf, size_t size) {
	if (CDC_Receive_FS(buf, (uint32_t*) size) != USBD_OK) {
		return RES_FAIL;
	}
	return RES_OK;
}

int8_t CDCProtocolTransceiver::Transmit(uint8_t* buf, size_t size) {
	if (CDC_Transmit_FS(buf, (uint16_t) size) != USBD_OK) {
		return RES_FAIL;
	}
	return RES_OK;
}