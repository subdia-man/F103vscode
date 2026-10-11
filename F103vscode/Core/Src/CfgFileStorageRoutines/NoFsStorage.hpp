#ifndef NO_FS_STORAGE_H
#define NO_FS_STORAGE_H

#include <cassert>
#include "W25Q128Driver.hpp"
#include "etl/vector.h"

//relatied to the storage info
#define STORAGE_INFO_HEADER			0xAAAA
#define MAX_STORAGE_INFO_SIZE 		256
#define STORAGE_INFO_SECTOR	 		0
#define STORAGE_INFO_PAGE			0
#define MAX_FILES_NUMBER 			20
#define MAX_SECTOR_CYCLES_NUMBER 	100000
#define RESERVED_WORDS				11

//related to other
#define MAX_BUFFER_RWX_SIZE			1024
#define RECORDS_START_SECTOR		1

typedef struct __attribute__((packed)) {
	uint16_t header;
	uint32_t memoryRecordedVolume; //bytes
	uint16_t filesNumber;
	etl::array<uint32_t, MAX_FILES_NUMBER> fAddresses;
	etl::array<uint32_t, MAX_FILES_NUMBER> fUIDs;
	etl::array<uint16_t, MAX_FILES_NUMBER> fSizes;
	uint32_t cyclesCounter;
	uint32_t reserved[RESERVED_WORDS];
} StorageStruct_t;

typedef struct {
	uint8_t currentRecordIndex;
	uint32_t startAddress;
	uint32_t currentAddress;
	uint8_t currentPageOffset;
	uint16_t currentPage;
	uint16_t currentSector;
	uint16_t writtenBytes;
	uint16_t toBeWrittenBytes;
	bool isOpened;
} RecordSession_t;

typedef struct {
	uint8_t currentRecordIndex;
	uint32_t startAddress;
	uint32_t currentAddress;
	uint16_t readBytes;
	uint16_t toBeReadBytes;
	bool isOpened;
} ReadSession_t;

typedef struct {
	uint8_t currentRecordIndex;
	uint32_t startAddress;
	uint32_t startSector;
	uint32_t endSector;
	uint16_t recordSize;
	bool isOpened;
} DeleteRecordSession_t;

class NoFsStorage {

	uint8_t _rwxBuffer[MAX_BUFFER_RWX_SIZE];
	w25qxx_t _flashDeviceInfo;
	StorageStruct_t _fileStorageInfo;
	RecordSession_t _recordSession;
	ReadSession_t _readSession;
	DeleteRecordSession_t _deleteSession;

public:
	NoFsStorage() {}
	~NoFsStorage() {}

	int8_t Init();

	int8_t StartNewCfgRecord(uint16_t cfgRecordSize);
	int8_t WriteCfgRecordNextPart(uint8_t* data, size_t size);

	uint16_t StartNewCfgRead(uint32_t uid = 0);
	int8_t ReadCfgRecordNextPart(uint8_t* data, size_t size);

	int8_t DeleteRecord(uint32_t uid = 0);

private:
	w25qxx_t GetDeviceInfo();
	int8_t InitStorageInfo();
	StorageStruct_t CreateNewStorageInfo();
	int8_t WriteStorageInfo(StorageStruct_t* info);

	RecordSession_t StartNewRecordSession(uint16_t cfgRecordSize);
	void ResetRecordSession(RecordSession_t* session);
	void CloseRecordSession(RecordSession_t* session);

	ReadSession_t StartNewReadSession(uint32_t uid);
	void ResetReadSession(ReadSession_t* session);
	void CloseReadSession(ReadSession_t* session);

	DeleteRecordSession_t StartNewDeleteSession(uint32_t uid);
	void ResetDeleteSession(DeleteRecordSession_t* session);

};

#endif