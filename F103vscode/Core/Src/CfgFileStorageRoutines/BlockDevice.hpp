#ifndef _BLOCKDEVICE_H
#define _BLOCKDEVICE_H

#include "lfs/lfs.h"
#include "lfs/lfs_util.h"
#include "W25Q128Driver.hpp"

class BlockDevice {

	W25Q128Driver _flashDevice;

public:

	BlockDevice(W25Q128Driver flashDevice) : _flashDevice(flashDevice) {
		this->_flashDevice = flashDevice;
	}
	~BlockDevice() {}
	
	uint8_t Init();
	int Read(lfs_off_t off, uint8_t *buffer, lfs_size_t size);
	int Write(lfs_off_t off, uint8_t *buffer, lfs_size_t size);
	int Erase(lfs_block_t block);
	int Sync(const struct lfs_config *c);
	void FullErase();
	
};

#endif