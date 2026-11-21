
#include "etl/array.h"
#include "ICDCProtocolFormer.hpp"

#define COMMAND_FRAME_MAX_LENGTH 10
#define DATA_FRAME_MAX_LENGTH 68
#define CDC_PROTOCOL_HEADER 0xAA
#define CDC_PROTOCOL_HEADER_PLACE 0
#define CDC_PROTOCOL_CMD_PLACE 1
#define CDC_PROTOCOL_RES_PLACE 1
#define CDC_PROTOCOL_CFG_FILE_VAL_PLACE0 2
#define CDC_PROTOCOL_CFG_FILE_VAL_PLACE1 3
#define CDC_PROTOCOL_CFG_FILE_PACKET_SIZE 4
#define CDC_PROTOCOL_CFG_FILE_PACKETS_NUM0 5
#define CDC_PROTOCOL_CFG_FILE_PACKETS_NUM1 6
#define CDC_PROTOCOL_CFG_FILE_PACKET_NUM0 1
#define CDC_PROTOCOL_CFG_FILE_PACKET_NUM1 2
#define CDC_PROTOCOL_PACKET_NUM_PLACE0 1
#define CDC_PROTOCOL_PACKET_NUM_PLACE1 2
#define CDC_PROTOCOL_OK_RESULT 0x00
#define CDC_PROTOCOL_FAIL_RESULT 0xFF

enum CDCCommands {
	CatchConfig = 0xBB,
	SendConfig = 0xCC,
	Undefined
};

class CDCProtocolFormer : public ICDCProtocolFormer {


public:

	CDCProtocolFormer() {}
	~CDCProtocolFormer() {}

	bool IsItCommand(uint8_t cmd);
	CDCCommands GetCommandFromFrame(etl::array<uint8_t, COMMAND_FRAME_MAX_LENGTH> frame);

	void FormOkFrame(uint8_t* buf);
	void FormFailFrame(uint8_t* buf);
};