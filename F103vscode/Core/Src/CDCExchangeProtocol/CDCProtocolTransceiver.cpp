
#include "CDCProtocolTransceiver.hpp"

extern "C" {
	extern uint8_t cdc_global_rx_buffer[68];
	extern uint32_t cdc_global_rx_length;
	extern void cdc_is_something_received_flag_reset();
	extern bool is_cdc_something_received_flag_set();
}


int8_t CDCProtocolTransceiver::Process() {
	switch (_cdcStateMachine.GetState()) {
		case CDCFree:
		//send FAIL frame here
		return RES_FAIL; //communication state can't be free at this moment
		case CDCCommandAwaiting:
		//check incoming queue for something received
		if (!_rxTransceiverQueue.empty()) {
			//is this the command?
			if (_cdcProtocolFormer.IsItCommand(_rxTransceiverQueue.front().at(CDC_PROTOCOL_CMD_PLACE))) {
				if (_cdcProtocolFormer.GetCommandFromFrame(_rxTransceiverQueue.front()) == 
					CDCCommands::SendConfig) {
					_cdcProtocolFormer.FormOkFrame(_txrxBuffer.begin());
					_txTransceiverQueue.push(_txrxBuffer);
					//send config transmittion command
					//pack data and send
					_cdcStateMachine.SetState(CDCFileTransmitting);
				}
				else if (_cdcProtocolFormer.GetCommandFromFrame(_rxTransceiverQueue.front()) == 
					CDCCommands::CatchConfig) {
					ExtractPacketDataFromCmd(_rxTransceiverQueue.front().begin());
					_cdcProtocolFormer.FormOkFrame(_txrxBuffer.begin());
					_txTransceiverQueue.push(_txrxBuffer);
					_cdcStateMachine.SetState(CDCFileReceiving);
				}
			}
			_rxTransceiverQueue.pop(); //if received data is the cmd, handle it; if not - just ignore and delete from queue
		}
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

void CDCProtocolTransceiver::ExtractPacketDataFromCmd(uint8_t* buf) {
	_cfgFileSize = buf[2];
	_cfgFileSize = (_cfgFileSize << 8) | buf[3];
	_cfgFilePacketSize = buf[4];
	_cfgFilePacketsNum = buf[5];
	_cfgFilePacketsNum = (_cfgFilePacketsNum << 8) | buf[6];
	return;
}

int8_t CDCProtocolTransceiver::StoreMsgToIncoming(etl::array<uint8_t, COMMAND_FRAME_MAX_LENGTH> packet) {
	_rxTransceiverQueue.push(packet);
	return RES_OK;
}

int8_t CDCProtocolTransceiver::StoreMsgToOutgoing(etl::array<uint8_t, COMMAND_FRAME_MAX_LENGTH> packet) {
	_txTransceiverQueue.push(packet);
	return RES_OK;
}

int8_t CDCProtocolTransceiver::Receive() {
	Receive(_txrxBuffer.begin(), 64);
	if (_txrxBuffer.at(0) != 0) {
		StoreMsgToIncoming(_txrxBuffer);
		return RES_OK;
	}
	return RES_FAIL;
}

int8_t CDCProtocolTransceiver::Transmit() {
	if (!_txTransceiverQueue.empty()) {
		etl::array<uint8_t, COMMAND_FRAME_MAX_LENGTH> arr = _txTransceiverQueue.front();
		Transmit(arr.begin(), COMMAND_FRAME_MAX_LENGTH);
		_txTransceiverQueue.pop();
	}
	return RES_OK;
}

int8_t CDCProtocolTransceiver::Receive(uint8_t* buf, uint32_t size) {
	if (is_cdc_something_received_flag_set()) {
		memcpy(buf, cdc_global_rx_buffer, cdc_global_rx_length);
		cdc_is_something_received_flag_reset();
	}
	return RES_OK;
}

int8_t CDCProtocolTransceiver::Transmit(uint8_t* buf, size_t size) {
	if (CDC_Transmit_FS(buf, (uint16_t) size) != USBD_OK) {
		return RES_FAIL;
	}
	return RES_OK;
}
