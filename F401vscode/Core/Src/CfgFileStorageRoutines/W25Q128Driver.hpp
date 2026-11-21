#ifndef _W25QXX_H
#define _W25QXX_H

/*
  Author:     Nima Askari
  WebSite:    http://www.github.com/NimaLTD
*/

#include "main.h"
#include "Common.h"

extern SPI_HandleTypeDef hspi2;

#define W25QXX_SPI_PTR           &hspi2


#define _W25QXX_USE_FREERTOS     0

#define INIT_DEBUG               1

#define W25_WRITE_DISABLE     0x04
#define W25_WRITE_ENABLE      0x06
#define W25_CHIP_ERASE        0xC7
#define W25_SECTOR_ERASE      0x20
#define W25_BLOCK_ERASE       0xD8
#define W25_READ              0x03
#define W25_FAST_READ         0x0B
#define W25_PAGE_PROGRAMM     0x02
#define W25_GET_JEDEC_ID      0x9F
#define W25_READ_STATUS_1     0x05
#define W25_READ_STATUS_2     0x35
#define W25_READ_STATUS_3     0x15
#define W25_WRITE_STATUS_1    0x01
#define W25_WRITE_STATUS_2    0x31
#define W25_WRITE_STATUS_3    0x11
#define W25_READ_UNIQUE_ID    0x4B
#define W25_RELEASE_POWERDOWN 0xAB
#define W25_WRITE_VOLATILE    0x50
#define W25_ENABLE_RESET      0x66
#define W25_RESET             0x99
#define W25_EXIT_QSPI_MODE    0xFF


typedef enum {
  W25Q10 = 1,
  W25Q20,
  W25Q40,
  W25Q80,
  W25Q16,
  W25Q32,
  W25Q64,
  W25Q128,
  W25Q256,
  W25Q512,
} W25QXX_ID_t;

typedef struct {
  W25QXX_ID_t ID;
  uint8_t   UniqID[8];
  uint16_t  PageSize;
  uint32_t  PageCount;
  uint32_t  SectorSize;
  uint32_t  SectorCount;
  uint32_t  BlockSize;
  uint32_t  BlockCount;
  uint32_t  CapacityInKiloByte;
  uint8_t   StatusRegister1;
  uint8_t   StatusRegister2;
  uint8_t   StatusRegister3;  
  uint8_t   Lock;
} w25qxx_t;

typedef enum {
  StatusRegister1 = 0x05,
  StatusRegister2 = 0x35,
  StatusRegister3 = 0x15
} StatusReg_t;

class W25Q128Driver {

  w25qxx_t w25qxx;

public:

  W25Q128Driver() {}
  ~W25Q128Driver() {}

  uint8_t W25QxxInit(void);
  uint8_t W25QxxReadStatus(StatusReg_t reg);

  void W25QxxEraseChip(void);
  void W25QxxEraseSector(uint32_t SectorAddr);
  void W25QxxEraseBlock(uint32_t BlockAddr);

  uint32_t W25QxxPageToSector(uint32_t PageAddress);
  uint32_t W25QxxPageToBlock(uint32_t PageAddress);
  uint32_t W25QxxSectorToBlock(uint32_t SectorAddress);
  uint32_t W25QxxSectorToPage(uint32_t SectorAddress);
  uint32_t W25QxxBlockToPage(uint32_t BlockAddress);

  uint8_t W25QxxIsEmptyPage(uint32_t Page_Address, uint32_t OffsetInByte);
  uint8_t W25QxxIsEmptySector(uint32_t Sector_Address, uint32_t OffsetInByte);
  uint8_t W25QxxIsEmptyBlock(uint32_t Block_Address, uint32_t OffsetInByte);

  void W25QxxWriteByte(uint8_t byte, uint32_t addr);
  void W25QxxWritePage(uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte, 
                      uint32_t NumByteToWrite_up_to_PageSize);
  void W25QxxWriteSector(uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte, 
                      uint32_t NumByteToWrite_up_to_SectorSize);
  void W25QxxWriteBlock(uint8_t* pBuffer, uint32_t Block_Address, uint32_t OffsetInByte, 
                      uint32_t NumByteToWrite_up_to_BlockSize);

  void W25QxxReadByte(uint8_t *pBuffer, uint32_t Bytes_Address);
  void W25QxxReadBytes(uint8_t *pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
  void W25QxxReadPage(uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte, 
                      uint32_t NumByteToRead_up_to_PageSize);
  void W25QxxReadSector(uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte, 
                      uint32_t NumByteToRead_up_to_SectorSize);
  void W25QxxReadBlock(uint8_t *pBuffer, uint32_t Block_Address, uint32_t OffsetInByte, 
                      uint32_t NumByteToRead_up_to_BlockSize);
  w25qxx_t GetW25QxxInstance ();

private:
  uint8_t W25QxxSpi(uint8_t Data);
  uint32_t W25QxxReadID(void);
  void W25QxxWriteEnable(void);
  void W25QxxWriteDisable(void);
  void W25QxxWaitForWriteEnd(void);
};

#endif
