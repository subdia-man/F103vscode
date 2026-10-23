#ifndef _LITTLEFILESYSTEM_H
#define _LITTLEFILESYSTEM_H

#include "BlockDevice.hpp"
#include "lfs/lfs.h"

int read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
int write(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);
int erase(const struct lfs_config *c, lfs_block_t block);
int sync(const struct lfs_config *c);

class LittleFileSystem {

	lfs_t lfs;
	lfs_file_t file;
	lfs_config lfscfg;
	BlockDevice _blockDevice;

public:

	LittleFileSystem(BlockDevice blockDevice) : _blockDevice(blockDevice) {
		this->_blockDevice = blockDevice;
		lfscfg.read = read;
		lfscfg.prog = write;
    	lfscfg.erase = erase;
    	lfscfg.sync = sync;

		lfscfg.read_size = 64;
    	lfscfg.prog_size = 64;
    	lfscfg.block_size = 4096;
    	lfscfg.block_count = 256;
    	lfscfg.block_cycles = 500;
    	lfscfg.cache_size = 64;
    	lfscfg.lookahead_size = 64;
	}

	~LittleFileSystem() {}

	int Mount();

private:

	void InitHardware();
	
};

#endif