#ifndef NO_FS_STORAGE_H
#define NO_FS_STORAGE_H

#include "W25Q128Driver.hpp"

#define MAX_SECTOR_SIZE 2048
#define MAX_FILES_NUMBER 300
#define STORAGE_INFO_SECTOR 0

typedef struct {
	uint16_t header;
	uint16_t filesNumber;
	uint32_t fAddresses[MAX_FILES_NUMBER];
	uint32_t fUIDs[MAX_FILES_NUMBER];
} StorageStruct_t;

class NoFsStorage {

	uint8_t _rwxBuffer[MAX_SECTOR_SIZE];
	w25qxx_t _flashDeviceInfo;
	StorageStruct_t _fileStorageInfo;

public:
	NoFsStorage() {}
	~NoFsStorage() {}

	int8_t Init();
	int8_t ReadStorageInfo();

	void Write(uint8_t* pBuffer, size_t size, uint32_t address);
	void WriteByte(uint8_t byte, uint32_t addr);
	void Read(uint8_t* pBuffer, size_t size, uint32_t address);
	void EraseSector(uint32_t SectorAddr);
	void WritePage (uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte, 
                      uint32_t NumByteToWrite_up_to_PageSize);

private:
	w25qxx_t GetDeviceInfo();
};

#endif