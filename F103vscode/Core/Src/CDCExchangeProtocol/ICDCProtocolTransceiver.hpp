#ifndef ICDCPROTOCOLTRANSCEIVER_H
#define ICDCPROTOCOLTRANSCEIVER_H
#include "cstdint"

class ICDCProtocolTransceiver {

public:
	ICDCProtocolTransceiver();
	~ICDCProtocolTransceiver();
	virtual int8_t Process() = 0;
};

#endif