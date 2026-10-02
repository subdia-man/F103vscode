
#include "BlockDevice.hpp"

uint8_t BlockDevice::Init() {
	return _flashDevice.W25QxxInit();
}

int BlockDevice::Read(lfs_off_t off, uint8_t *buffer, lfs_size_t size) {
	_flashDevice.W25QxxReadBytes(buffer, off, size);
	return 0;
}

int BlockDevice::Write(lfs_off_t off, uint8_t *buffer, lfs_size_t size) {
	for (uint32_t i = off; i < off+size; i++) {
		_flashDevice.W25QxxWriteByte(*buffer, i);
		buffer++;
	}
	return 0;
}

int BlockDevice::Erase(lfs_block_t block) {
	_flashDevice.W25QxxEraseBlock(block);
	return 0;
}

int BlockDevice::Sync(const struct lfs_config *c) {
	return 0;
}

void BlockDevice::FullErase() {
	return _flashDevice.W25QxxEraseChip();
}