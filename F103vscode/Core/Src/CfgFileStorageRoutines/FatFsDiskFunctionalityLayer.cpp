#include "W25Q128Driver.hpp"
#include "diskio.h"

extern W25Q128Driver _flashDevice;

extern "C" {

DSTATUS usr_status(BYTE pdrv) {
	return RES_OK;
}

DSTATUS usr_initialize (BYTE pdrv) {
	_flashDevice.W25QxxInit();
	return RES_OK;
}

DRESULT usr_read (
    BYTE pdrv,      /* Physical drive nmuber to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    DWORD sector,   /* Sector address in LBA */
    UINT count      /* Number of sectors to read */
) {
	_flashDevice.W25QxxReadSector(buff, sector, 0, 0);
	return RES_OK;  
}

DRESULT usr_write (
    BYTE pdrv,          /* Physical drive nmuber to identify the drive */
    const BYTE *buff,   /* Data to be written */
    DWORD sector,       /* Sector address in LBA */
    UINT count          /* Number of sectors to write */
) {
	_flashDevice.W25QxxWriteSector((uint8_t*)buff, sector, 0, 0);
	return RES_OK; 
}



}

