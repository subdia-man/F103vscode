
#include "NoFsStorage.hpp"

extern W25Q128Driver _flashDevice;

int8_t NoFsStorage::Init() {
	int8_t res = _flashDevice.W25QxxInit();
	if (res == 1) {
		_flashDeviceInfo = GetDeviceInfo();
		return RES_OK;
	}
	else return RES_FAIL;
}

int8_t NoFsStorage::ReadStorageInfo() {
	_flashDevice.W25QxxReadSector(_rwxBuffer, STORAGE_INFO_SECTOR, 0, 0);
	return RES_OK;
}

w25qxx_t NoFsStorage::GetDeviceInfo() {
	return _flashDevice.GetW25QxxInstance ();
}

void NoFsStorage::WriteByte(uint8_t byte, uint32_t addr) {
	_flashDevice.W25QxxWriteByte(byte, addr);
}

void NoFsStorage::Write(uint8_t* pBuffer, size_t size, uint32_t address) {
	_flashDevice.W25QxxWritePage(pBuffer, address, 0, size);
	return;
}

void NoFsStorage::Read(uint8_t* pBuffer, size_t size, uint32_t address) {
	_flashDevice.W25QxxReadBytes(pBuffer, address, size);
	return;
}

void NoFsStorage::EraseSector(uint32_t SectorAddr) {
	_flashDevice.W25QxxEraseSector(SectorAddr);
}

void NoFsStorage::WritePage (uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte, 
                      uint32_t NumByteToWrite_up_to_PageSize) {
	return _flashDevice.W25QxxWritePage(pBuffer, Page_Address, OffsetInByte, NumByteToWrite_up_to_PageSize);
}