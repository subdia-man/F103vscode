#ifndef NO_FS_STORAGE_H
#define NO_FS_STORAGE_H

#include "W25Q128Driver.hpp"

//relatied to the storage info
#define STORAGE_INFO_HEADER			0xAAAA
#define MAX_STORAGE_INFO_SIZE 		256
#define STORAGE_INFO_SECTOR	 		0
#define STORAGE_INFO_PAGE			0
#define MAX_FILES_NUMBER 			20
#define MAX_SECTOR_CYCLES_NUMBER 	100000
#define RESERVED_WORDS				21

//related to other
#define MAX_BUFFER_RWX_SIZE			1024

typedef struct {
	uint16_t header;
	uint32_t memoryRecordedVolume; //bytes
	uint16_t filesNumber;
	uint32_t fAddresses[MAX_FILES_NUMBER];
	uint32_t fUIDs[MAX_FILES_NUMBER];
	uint32_t cyclesCounter;
	uint32_t reserved[RESERVED_WORDS];
} StorageStruct_t;

class NoFsStorage {

	uint8_t _rwxBuffer[MAX_BUFFER_RWX_SIZE];
	w25qxx_t _flashDeviceInfo;
	StorageStruct_t _fileStorageInfo;

public:
	NoFsStorage() {}
	~NoFsStorage() {}

	int8_t Init();

private:
	w25qxx_t GetDeviceInfo();
	int8_t InitStorageInfo();
	StorageStruct_t CreateNewStorageInfo();
	int8_t WriteStorageInfo(StorageStruct_t info);
};

#endif