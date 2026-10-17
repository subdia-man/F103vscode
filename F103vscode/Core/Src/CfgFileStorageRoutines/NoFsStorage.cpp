
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
	_isInit = true;
	return RES_OK;
}

w25qxx_t NoFsStorage::GetDeviceInfo() {
	return _flashDevice.GetW25QxxInstance ();
}

int8_t NoFsStorage::InitStorageInfo() {
	//check zero page for some data
	if (_flashDevice.W25QxxIsEmptyPage(STORAGE_INFO_PAGE, 0)) {
		_fileStorageInfo = CreateNewStorageInfo();
		WriteStorageInfo(&_fileStorageInfo);
		return RES_OK;
	} 
	//read zero page
	_flashDevice.W25QxxReadPage(_rwxBuffer, STORAGE_INFO_PAGE, 0, 0);
	//search header
	uint32_t checkHeader = (_rwxBuffer[0] << 8) | _rwxBuffer[1];
	if (checkHeader != STORAGE_INFO_HEADER) {
		//corrupted or not exist
		_fileStorageInfo = CreateNewStorageInfo();
		WriteStorageInfo(&_fileStorageInfo);
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
	memset(&info.fSizes.at(0), 0, sizeof(uint16_t)*MAX_FILES_NUMBER);
	info.cyclesCounter = 0;
	return info;
}

int8_t NoFsStorage::WriteStorageInfo(StorageStruct_t* info) {
	//erase zero sector
	_flashDevice.W25QxxEraseSector(STORAGE_INFO_SECTOR);
	info->cyclesCounter++;
	_flashDevice.W25QxxWritePage((uint8_t*)info, STORAGE_INFO_PAGE, 0, 0);
	return RES_OK;
}

int8_t NoFsStorage::StartNewCfgRecord(uint32_t uid, uint16_t cfgRecordSize) {
	_recordSession = StartNewRecordSession(uid, cfgRecordSize);
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
		CloseRecordSession(&_recordSession);
	}
	return RES_OK;
}

RecordSession_t NoFsStorage::StartNewRecordSession(uint32_t uid, uint16_t cfgRecordSize) {
	RecordSession_t session;
	ResetRecordSession(&session);

	uint16_t neededSectors = (cfgRecordSize / _flashDeviceInfo.SectorSize);
	if(cfgRecordSize % _flashDeviceInfo.SectorSize) {
		neededSectors++;
	}
	//determine new start sector
	for(uint16_t i = RECORDS_START_SECTOR; i < _flashDeviceInfo.SectorCount; i++) {
		if(_flashDevice.W25QxxIsEmptySector(i, 0) & isEmptySectorsEnoughForRecord((i * _flashDeviceInfo.SectorSize), 
																					neededSectors)) {
			session.currentSector = i;
			break;
		}
	}

	uint8_t pagesAtSector = (_flashDeviceInfo.SectorSize/_flashDeviceInfo.PageSize);
	session.currentPage = (session.currentSector * pagesAtSector);
	//calculate start address disregarding the offset inside previous (not empty) page
	session.startAddress = session.currentAddress = 
											_flashDeviceInfo.PageSize * session.currentPage;
	session.toBeWrittenBytes = cfgRecordSize;
	session.isOpened = true; //open session

	for (uint8_t i = 0; i < _fileStorageInfo.fUIDs.size(); i++) {
		if(_fileStorageInfo.fUIDs.at(i) == 0) {
			session.currentRecordIndex = i;
			_fileStorageInfo.fUIDs.at(i) = uid;
			break;
		}
	}

	return session;
}

bool NoFsStorage::isEmptySectorsEnoughForRecord(uint32_t startAddress, uint16_t neededSectors) {
	for (uint16_t i = (startAddress / _flashDeviceInfo.SectorSize); 
		i < (startAddress / _flashDeviceInfo.SectorSize) + neededSectors; i++) {
		if (!_flashDevice.W25QxxIsEmptySector(i, 0)) {
			return false;
		}
	}
	return true;
}

void NoFsStorage::ResetRecordSession(RecordSession_t* session) {
	memset(session, 0, sizeof(RecordSession_t));
	return;
}

void NoFsStorage::CloseRecordSession(RecordSession_t* session) {
	//update storage info structure
	_fileStorageInfo.memoryRecordedVolume += session->writtenBytes;
	_fileStorageInfo.filesNumber ++;
	for(uint8_t i = 0; i < _fileStorageInfo.fAddresses.size(); i++) {
		if (!_fileStorageInfo.fAddresses.at(i)) {
			_fileStorageInfo.fAddresses.at(i) = session->startAddress;
			_fileStorageInfo.fSizes.at(i) = session->writtenBytes;
			break;
		}
	}
	//save storage info
	WriteStorageInfo(&_fileStorageInfo);
	//reset session
	ResetRecordSession(session);
	return;
}

uint16_t NoFsStorage::StartNewCfgRead(uint32_t uid) {
	_readSession = StartNewReadSession(uid);
	return _readSession.toBeReadBytes; //return size of file to be read
}

int8_t NoFsStorage::ReadCfgRecordNextPart(uint8_t* data, size_t size) {
	if(!_readSession.isOpened) {
		return RES_FAIL;
	}
	//check if the rest of file bigger than size
	if(_readSession.toBeReadBytes - _readSession.readBytes < (uint16_t)size) {
		size = (_readSession.toBeReadBytes - _readSession.readBytes);
	}
	_flashDevice.W25QxxReadBytes(data, _readSession.currentAddress, size);
	_readSession.readBytes += size;
	_readSession.currentAddress += size;
	if(_readSession.readBytes == _readSession.toBeReadBytes) {
		CloseReadSession(&_readSession);
	}
	return RES_OK;
}

ReadSession_t NoFsStorage::StartNewReadSession(uint32_t uid) {
	ReadSession_t session;
	ResetReadSession(&session);
	for (uint8_t i = 0; i < _fileStorageInfo.fUIDs.size(); i++) {
		if(_fileStorageInfo.fUIDs.at(i) == uid) {
			session.currentRecordIndex = i;
			session.startAddress = session.currentAddress = _fileStorageInfo.fAddresses.at(i);
			session.toBeReadBytes = _fileStorageInfo.fSizes.at(i);
			break;
		}
	}
	session.isOpened = true;
	return session;
}

void NoFsStorage::ResetReadSession(ReadSession_t* session) {
	memset(session, 0, sizeof(ReadSession_t));
	return;
}

void NoFsStorage::CloseReadSession(ReadSession_t* session) {
	session->isOpened = false;
	return;
}

int8_t NoFsStorage::DeleteRecord(uint32_t uid) {
	//start new delete session and define file's data: start address, file size, start and end sectors
	_deleteSession = StartNewDeleteSession(uid);
	if(_deleteSession.startAddress == 0 || _deleteSession.startSector == 0 || _deleteSession.endSector == 0 ||
		_deleteSession.recordSize == 0) {
		return RES_FAIL;
	}
	//then erase all sectors related to the file being deleted
	for (uint16_t i = _deleteSession.startSector; i < _deleteSession.endSector; i++) {
		_flashDevice.W25QxxEraseSector(i);
	}
	//zero all fields at the storage info related to this file
	_fileStorageInfo.fAddresses.at(_deleteSession.currentRecordIndex) = 0;
	_fileStorageInfo.fSizes.at(_deleteSession.currentRecordIndex) = 0;
	_fileStorageInfo.fUIDs.at(_deleteSession.currentRecordIndex) = 0;
	if (_fileStorageInfo.filesNumber > 0) {
		_fileStorageInfo.filesNumber--;
	}
	if (_fileStorageInfo.memoryRecordedVolume > 0) {
		_fileStorageInfo.memoryRecordedVolume -= _deleteSession.recordSize;
	}
	ResetDeleteSession(&_deleteSession);
	//store renewed storage info
	WriteStorageInfo(&_fileStorageInfo);
	return RES_OK;
}

DeleteRecordSession_t NoFsStorage::StartNewDeleteSession(uint32_t uid) {
	DeleteRecordSession_t session;
	ResetDeleteSession(&session);
	for (uint8_t i = 0; i < _fileStorageInfo.fUIDs.size(); i++) {
		if(uid == _fileStorageInfo.fUIDs.at(i)) {
			session.currentRecordIndex = i;
			break;
		}
	}
	session.startAddress = _fileStorageInfo.fAddresses.at(session.currentRecordIndex);
	session.recordSize = _fileStorageInfo.fSizes.at(session.currentRecordIndex);
	session.startSector = (session.startAddress / _flashDeviceInfo.SectorSize);
	session.endSector = ((session.startAddress + session.recordSize) / _flashDeviceInfo.SectorSize);
	if ((session.startAddress + session.recordSize) % _flashDeviceInfo.SectorSize) {
		session.endSector++;
	}
	session.isOpened = true;
	return session;
}

void NoFsStorage::ResetDeleteSession(DeleteRecordSession_t* session) {
	memset(session, 0, sizeof(DeleteRecordSession_t));
	return;
}