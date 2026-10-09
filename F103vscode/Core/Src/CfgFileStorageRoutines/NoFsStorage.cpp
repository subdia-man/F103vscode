
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
		assert(sizeof(_fileStorageInfo) == MAX_STORAGE_INFO_SIZE);
		memcpy((void*)&_fileStorageInfo, _rwxBuffer, sizeof(StorageStruct_t));
	}
	return RES_OK;
}

StorageStruct_t NoFsStorage::CreateNewStorageInfo() {
	StorageStruct_t info;
	info.header = STORAGE_INFO_HEADER;
	info.memoryRecordedVolume = 0;
	info.filesNumber = 0;
	memset(&info.fAddresses.at(0), 0, sizeof(uint32_t)*MAX_FILES_NUMBER);
	memset(&info.fUIDs.at(0), 0, sizeof(uint32_t)*MAX_FILES_NUMBER);
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

int8_t NoFsStorage::StartNewCfgRecord(uint16_t cfgRecordSize) {
	_recordSession = StartNewRecordSession(cfgRecordSize);
	return RES_OK;
}

int8_t NoFsStorage::WriteCfgRecordNextPart(uint8_t* data, size_t size) {
	if (!_recordSession.isOpened) {
		return RES_FAIL;
	}
	
	//if size bigger than remaining size of last page, finish writing it
	uint16_t pageRemainingSize = _flashDeviceInfo.PageSize - _recordSession.currentPageOffset;
	if(size > pageRemainingSize) {
		_flashDevice.W25QxxWritePage(data, _recordSession.currentPage, _recordSession.currentPageOffset, 0);
		size-= pageRemainingSize;
		data+= pageRemainingSize;
		_recordSession.currentAddress += pageRemainingSize;
		_recordSession.currentPage ++;
		_recordSession.currentPageOffset = 0;
		_recordSession.writtenBytes += pageRemainingSize;

		uint8_t iterations = (size / _flashDeviceInfo.PageSize);
		if(size % _flashDeviceInfo.PageSize != 0) {
			iterations++;
		}
		for (uint8_t i = 0; i < (iterations+1); i++) {

			_flashDevice.W25QxxWritePage(data, _recordSession.currentPage, 0, 0);

			uint8_t bytesToWrite = 0;
			if(size >= _flashDeviceInfo.PageSize) {
				bytesToWrite = _flashDeviceInfo.PageSize;
				_recordSession.currentPage ++;
				_recordSession.currentPageOffset = 0;
			} else {
				bytesToWrite = _flashDeviceInfo.PageSize - size;
				_recordSession.currentPageOffset = bytesToWrite;
			}
			size-= bytesToWrite;
			data+= bytesToWrite;
			_recordSession.currentAddress += bytesToWrite;
			_recordSession.writtenBytes += bytesToWrite;
		}
	} else if (size == pageRemainingSize) {
		_flashDevice.W25QxxWritePage(data, _recordSession.currentPage, _recordSession.currentPageOffset, 0);
		_recordSession.currentAddress += size;
		_recordSession.currentPage ++;
		_recordSession.currentPageOffset = 0;
		_recordSession.writtenBytes += size;
	} else {
		_flashDevice.W25QxxWritePage(data, _recordSession.currentPage, _recordSession.currentPageOffset, 0);
		_recordSession.currentAddress += size;
		_recordSession.currentPageOffset += size;
		_recordSession.writtenBytes += size;
	}
	uint8_t pagesAtSector = (_flashDeviceInfo.SectorSize / _flashDeviceInfo.PageSize);

	_recordSession.currentSector = (_recordSession.currentPage / pagesAtSector);
	if (_recordSession.currentPage % pagesAtSector) {
		_recordSession.currentSector++;
	}

	//if written = toBeWritten close session and update values in the structures
	if(_recordSession.writtenBytes == _recordSession.toBeWrittenBytes) {
		CloseRecordSession(_recordSession);
	}
	return RES_OK;
}

RecordSession_t NoFsStorage::StartNewRecordSession(uint16_t cfgRecordSize) {
	ResetRecordSession(_recordSession);
	//find nearest empty sector
	for(uint16_t i = RECORDS_START_SECTOR; i < (_flashDeviceInfo.SectorCount + 1); i++) {
		if (_flashDevice.W25QxxIsEmptySector(i, 0)) {
			_recordSession.currentSector = i;
			break;
		}
	}
	uint8_t pagesAtSector = (_flashDeviceInfo.SectorSize/_flashDeviceInfo.PageSize);
	//find nearest empty page
	for(uint16_t i = 0; i < (pagesAtSector + 1); i++) {
		if (_flashDevice.W25QxxIsEmptyPage((_recordSession.currentSector * pagesAtSector) + i, 0)) {
			_recordSession.currentPage = (_recordSession.currentSector * pagesAtSector) + i;
			break;
		}
	}
	//calculate start address disregarding the offset inside previous (not empty) page
	_recordSession.startAddress = _recordSession.currentAddress = 
											_flashDeviceInfo.PageSize * _recordSession.currentPage;
	_recordSession.toBeWrittenBytes = cfgRecordSize;
	_recordSession.isOpened = true; //open session
	return _recordSession;
}

void NoFsStorage::ResetRecordSession(RecordSession_t session) {
	memset(&session.startAddress, 0, sizeof(RecordSession_t));
	return;
}

void NoFsStorage::CloseRecordSession(RecordSession_t session) {
	//update storage info structure
	_fileStorageInfo.memoryRecordedVolume += session.writtenBytes;
	_fileStorageInfo.filesNumber ++;
	for(uint8_t i = 0; i < _fileStorageInfo.fAddresses.size(); i++) {
		if (!_fileStorageInfo.fAddresses.at(i)) {
			_fileStorageInfo.fAddresses.at(i) = session.startAddress;
			break;
		}
	}
	//save storage info
	WriteStorageInfo(_fileStorageInfo);
	//reset session
	ResetRecordSession(session);
	return;
}