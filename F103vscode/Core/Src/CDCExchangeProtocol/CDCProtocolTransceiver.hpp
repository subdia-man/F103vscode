#ifndef CDCPROTOCOLTRANSCEIVER_H
#define CDCPROTOCOLTRANSCEIVER_H

#include "ICDCProtocolTransceiver.hpp"
#include "Instances.hpp"
#include "Common.h"
#include "etl/queue.h"

extern "C" {
	#include "usbd_cdc_if.h"	
}

class CDCStateMachine;

class CDCProtocolTransceiver : public ICDCProtocolTransceiver {

	CDCStateMachine _cdcStateMachine;
	CDCProtocolFormer _cdcProtocolFormer;
	
	uint16_t _cfgFileSize = 0; //bytes
	uint8_t _cfgFilePacketSize = 0; //bytes
	uint16_t _cfgFilePacketsNum = 0;
	uint16_t _lastPacketNum = 0;
	etl::array<uint8_t, COMMAND_FRAME_MAX_LENGTH> _txrxBuffer;
	etl::queue<etl::array<uint8_t, COMMAND_FRAME_MAX_LENGTH>, 64> _rxTransceiverQueue;
	etl::queue<etl::array<uint8_t, COMMAND_FRAME_MAX_LENGTH>, 64> _txTransceiverQueue;

public:
	
	CDCProtocolTransceiver() {}
	~CDCProtocolTransceiver() {}
	int8_t Process();
	int8_t CfgFileReceiveProcessingStep();
	int8_t Receive();
	int8_t Transmit();
	int8_t StoreMsgToIncoming(etl::array<uint8_t, COMMAND_FRAME_MAX_LENGTH> packet);
	int8_t StoreMsgToOutgoing(etl::array<uint8_t, COMMAND_FRAME_MAX_LENGTH> packet);
	void ExtractPacketDataFromCmd(uint8_t* buf);

private:
	int8_t Receive(uint8_t* buf, uint32_t size);
	int8_t Transmit(uint8_t* buf, size_t size);
};

#endif