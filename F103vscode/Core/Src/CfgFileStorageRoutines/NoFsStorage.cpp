
#include <cstring>
#include "NoFsStorage.hpp"

extern W25Q128Driver _flashDevice;

int8_t NoFsStorage::Init() {
	int8_t res = _flashDevice.W25QxxInit();
	if (res != 1) {
		return RES_FAIL;
	}
	_flashDeviceInfo = GetDeviceInfo();
	if (InitStorageInfo() != RES_OK) {
		return RES_FAIL;
	}
	return RES_OK;
}

w25qxx_t NoFsStorage::GetDeviceInfo() {
	return _flashDevice.GetW25QxxInstance ();
}

int8_t NoFsStorage::InitStorageInfo() {
	//check zero page for some data
	if (_flashDevice.W25QxxIsEmptyPage(STORAGE_INFO_PAGE, 0)) {
		_fileStorageInfo = CreateNewStorageInfo();
		WriteStorageInfo(_fileStorageInfo);
		return RES_OK;
	} 
	//read zero page
	_flashDevice.W25QxxReadPage(_rwxBuffer, STORAGE_INFO_PAGE, 0, 0);
	//search header
	uint32_t checkHeader = (_rwxBuffer[0] << 8) | _rwxBuffer[1];
	if (checkHeader != STORAGE_INFO_HEADER) {
		//corrupted or not exist
		_fileStorageInfo = CreateNewStorageInfo();
		WriteStorageInfo(_fileStorageInfo);
	} else {
		//init storage info with read data
		memcpy((void*)&_fileStorageInfo, _rwxBuffer, sizeof(StorageStruct_t));
	}
	return RES_OK;
}

StorageStruct_t NoFsStorage::CreateNewStorageInfo() {
	StorageStruct_t info;
	info.header = STORAGE_INFO_HEADER;
	info.memoryRecordedVolume = 0;
	info.filesNumber = 0;
	memset(info.fAddresses, 0, sizeof(uint32_t)*MAX_FILES_NUMBER);
	memset(info.fUIDs, 0, sizeof(uint32_t)*MAX_FILES_NUMBER);
	info.cyclesCounter = 0;
	return info;
}

int8_t NoFsStorage::WriteStorageInfo(StorageStruct_t info) {
	//erase zero sector
	_flashDevice.W25QxxEraseSector(STORAGE_INFO_SECTOR);
	info.cyclesCounter++;
	_flashDevice.W25QxxWritePage((uint8_t*)&info, STORAGE_INFO_PAGE, 0, 0);
	return RES_OK;
}