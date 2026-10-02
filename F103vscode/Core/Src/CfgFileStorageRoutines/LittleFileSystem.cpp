
#include "LittleFileSystem.hpp"

#ifndef __cplusplus
#define __cplusplus
#endif

W25Q128Driver _flashDevice;
BlockDevice _blockDevice(_flashDevice);

int read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size) {
  long offset = (block * c->block_size) + off;
  int result = _blockDevice.Read(offset, (uint8_t*)buffer, size);
  //return (result == size) ? 0 : -1;
  return 0;
}

int write(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) {
  long offset = (block * c->block_size) + off;
  int result = _blockDevice.Write(offset, (uint8_t*)buffer, size);
  //return (result == size) ? 0 : -1;
  return 0;
}

int erase(const struct lfs_config *c, lfs_block_t block) {
  long offset = block * c->block_size;
  int result = _blockDevice.Erase(block);
  //return (result >= 0) ? 0 : -1; // erase returns size or -1
  return 0;
}

int sync(const struct lfs_config *c) {
  return _blockDevice.Sync(c);
}

void LittleFileSystem::InitHardware() {
	_blockDevice.Init();
}

int LittleFileSystem::Mount() {
	InitHardware();
	volatile int err = lfs_mount(&lfs, &lfscfg);
  	if (err) {
    	err = lfs_format(&lfs, &lfscfg);
    	err = lfs_mount(&lfs, &lfscfg);
    	if (err) {
    		_blockDevice.FullErase();
      	return err;
    	}
  	}
  	return 0;
}