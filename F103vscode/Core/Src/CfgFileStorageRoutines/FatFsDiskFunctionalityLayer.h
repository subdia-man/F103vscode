#ifndef FATFS_DISK_FUNCTIONALITY_DRIVER_H
#define FATFS_DISK_FUNCTIONALITY_DRIVER_H

#include "diskio.h"

extern DSTATUS usr_status(BYTE pdrv);
extern DSTATUS usr_initialize (BYTE pdrv);
extern DRESULT usr_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
extern DRESULT usr_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);

DSTATUS BD_initialize (BYTE pdrv) { /* Physical drive nmuber to identify the drive */
	return usr_initialize(pdrv);
}
 
DSTATUS BD_status (BYTE pdrv) { /* Physical drive nmuber to identify the drive */
	return RES_OK;  
}
 
DRESULT BD_read (
    BYTE pdrv,      /* Physical drive nmuber to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    DWORD sector,   /* Sector address in LBA */
    UINT count      /* Number of sectors to read */
) {
	return usr_read(pdrv, buff, sector, count);  
}
 
#if _USE_WRITE == 1
DRESULT BD_write (
    BYTE pdrv,          /* Physical drive nmuber to identify the drive */
    const BYTE *buff,   /* Data to be written */
    DWORD sector,       /* Sector address in LBA */
    UINT count          /* Number of sectors to write */
) {
	return usr_write(pdrv, buff, sector, count); 
}
#endif /* _USE_WRITE == 1 */
 
#if _USE_IOCTL == 1
DRESULT BD_ioctl (
    BYTE pdrv,      /* Physical drive nmuber (0..) */
    BYTE cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
) {
	return RES_OK;  
}
#endif /* _USE_IOCTL == 1 */

#endif //FATFS_DISK_FUNCTIONALITY_DRIVER_H