
#include "W25Q128Driver.hpp"

#if (INIT_DEBUG == 1)
#include "string.h"
#include "stdio.h"
char buf[64] = {0,};
#endif

#define W25QXX_DUMMY_BYTE        0xA5

w25qxx_t w25qxx;

#if (_W25QXX_USE_FREERTOS == 1)
#define W25QxxDelay(delay)   osDelay(delay)
#include "cmsis_os.h"
#else
#define W25QxxDelay(delay)   HAL_Delay(delay)
#endif

uint8_t W25Q128Driver::W25QxxSpi(uint8_t  Data) {
  uint8_t ret;

  HAL_SPI_TransmitReceive(W25QXX_SPI_PTR, &Data, &ret, 1, 100); // spi2

  return ret;
}

uint32_t W25Q128Driver::W25QxxReadID(void) {
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  W25QxxSpi(W25_GET_JEDEC_ID);

  Temp0 = W25QxxSpi(W25QXX_DUMMY_BYTE);
  Temp1 = W25QxxSpi(W25QXX_DUMMY_BYTE);
  Temp2 = W25QxxSpi(W25QXX_DUMMY_BYTE);

  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}

void W25Q128Driver::W25QxxWriteEnable(void) {
  W25QxxSpi(W25_WRITE_ENABLE);
  W25QxxDelay(1);
}

void W25Q128Driver::W25QxxWriteDisable(void) {
  W25QxxSpi(W25_WRITE_DISABLE);
  W25QxxDelay(1);
}

void W25Q128Driver::W25QxxWaitForWriteEnd(void) {
  W25QxxDelay(1);
  W25QxxSpi(W25_READ_STATUS_1);
  do {
    w25qxx.StatusRegister1 = W25QxxSpi(W25QXX_DUMMY_BYTE);
    W25QxxDelay(1);
  } while((w25qxx.StatusRegister1 & 0x01) == 0x01);
}

uint8_t W25Q128Driver::W25QxxInit(void) {
  w25qxx.Lock = 1;
  /*while(HAL_GetTick() < 100) {
    W25QxxDelay(1);
    W25QxxDelay(100);
  }*/

  uint32_t  id = W25QxxReadID();

  switch(id & 0x0000FFFF) {
    case 0x401A:  //  w25q512
      w25qxx.ID = W25Q512;
      w25qxx.BlockCount = 1024;
    break;

    case 0x4019:  //  w25q256
      w25qxx.ID = W25Q256;
      w25qxx.BlockCount = 512;
    break;

    case 0x4018:  //  w25q128
      w25qxx.ID = W25Q128;
      w25qxx.BlockCount = 256;
    break;

    case 0x4017:  //  w25q64
      w25qxx.ID = W25Q64;
      w25qxx.BlockCount = 128;
    break;

    case 0x4016:  //  w25q32
      w25qxx.ID = W25Q32;
      w25qxx.BlockCount = 64;
    break;

    case 0x4015:  //  w25q16
      w25qxx.ID = W25Q16;
      w25qxx.BlockCount = 32;
    break;

    case 0x4014:  //  w25q80
      w25qxx.ID = W25Q80;
      w25qxx.BlockCount = 16;
    break;

    case 0x4013:  //  w25q40
      w25qxx.ID = W25Q40;
      w25qxx.BlockCount = 8;
    break;

    case 0x4012:  //  w25q20
      w25qxx.ID = W25Q20;
      w25qxx.BlockCount = 4;
    break;

    case 0x4011:  //  w25q10
      w25qxx.ID = W25Q10;
      w25qxx.BlockCount = 2;
    break;

    case 0x3017:  //  w25x64
      //w25qxx.ID = W25Q64;
      w25qxx.BlockCount = 128;
    break;

    case 0x3016:  //  w25x32
      //w25qxx.ID = W25Q32;
      w25qxx.BlockCount = 64;
    break;

    case 0x3015:  //  w25q16
      //w25qxx.ID = W25Q16;
      w25qxx.BlockCount = 32;
    break;

    case 0x3014:  //  w25x80
      //w25qxx.ID = W25Q80;
      w25qxx.BlockCount = 16;
      
    break;

    case 0x3013:  //  w25x40
      //w25qxx.ID = W25Q40;
      w25qxx.BlockCount = 8;
    break;

    case 0x3012:  //  w25x20
      //w25qxx.ID = W25Q20;
      w25qxx.BlockCount = 4;
    break;

    case 0x3011:  //  w25x10
      //w25qxx.ID = W25Q10;
      w25qxx.BlockCount = 2;
    break;

    default:
      
      w25qxx.Lock = 0;
      return 0;
  }

  w25qxx.PageSize = 256;
  w25qxx.SectorSize = 0x1000;
  w25qxx.SectorCount = w25qxx.BlockCount * 16;
  w25qxx.PageCount = (w25qxx.SectorCount * w25qxx.SectorSize) / w25qxx.PageSize;
  w25qxx.BlockSize = w25qxx.SectorSize * 16;
  w25qxx.CapacityInKiloByte = (w25qxx.SectorCount * w25qxx.SectorSize) / 1024;

  w25qxx.Lock = 0;
  return 1;
} 

void W25Q128Driver::W25QxxEraseChip(void) {
  while(w25qxx.Lock == 1) {
    W25QxxDelay(1);
  }

  w25qxx.Lock = 1;

  W25QxxWriteEnable();
  W25QxxSpi(W25_CHIP_ERASE);
  W25QxxWaitForWriteEnd();
  W25QxxDelay(10);

  w25qxx.Lock = 0;
}

void W25Q128Driver::W25QxxEraseSector(uint32_t SectorAddr) {
  while(w25qxx.Lock == 1) {
    W25QxxDelay(1);
  }

  w25qxx.Lock = 1;

  W25QxxWaitForWriteEnd();
  SectorAddr = SectorAddr * w25qxx.SectorSize;

  W25QxxWriteEnable();

  W25QxxSpi(W25_SECTOR_ERASE);

  if(w25qxx.ID >= W25Q256){
    W25QxxSpi((SectorAddr & 0xFF000000) >> 24);
  }

  W25QxxSpi((SectorAddr & 0xFF0000) >> 16);
  W25QxxSpi((SectorAddr & 0xFF00) >> 8);
  W25QxxSpi(SectorAddr & 0xFF);

  W25QxxWaitForWriteEnd();

  W25QxxDelay(1);
  w25qxx.Lock = 0;
}

void W25Q128Driver::W25QxxEraseBlock(uint32_t BlockAddr) {
  while(w25qxx.Lock == 1) {
    W25QxxDelay(1);
  }

  w25qxx.Lock = 1;

  W25QxxWaitForWriteEnd();

  BlockAddr = BlockAddr * w25qxx.SectorSize * 16;

  W25QxxWriteEnable();

  W25QxxSpi(W25_BLOCK_ERASE);

  if(w25qxx.ID>=W25Q256){
    W25QxxSpi((BlockAddr & 0xFF000000) >> 24);
  }

  W25QxxSpi((BlockAddr & 0xFF0000) >> 16);
  W25QxxSpi((BlockAddr & 0xFF00) >> 8);
  W25QxxSpi(BlockAddr & 0xFF);

  W25QxxWaitForWriteEnd();

  W25QxxDelay(1);
  w25qxx.Lock = 0;
}

uint32_t W25Q128Driver::W25QxxPageToSector(uint32_t PageAddress) {
  return((PageAddress * w25qxx.PageSize) / w25qxx.SectorSize);
}

uint32_t W25Q128Driver::W25QxxPageToBlock(uint32_t PageAddress) {
  return((PageAddress * w25qxx.PageSize) / w25qxx.BlockSize);
}

uint32_t W25Q128Driver::W25QxxSectorToBlock(uint32_t SectorAddress) {
  return((SectorAddress * w25qxx.SectorSize) / w25qxx.BlockSize);
}

uint32_t W25Q128Driver::W25QxxSectorToPage(uint32_t SectorAddress) {
  return(SectorAddress * w25qxx.SectorSize) / w25qxx.PageSize;
}

uint32_t W25Q128Driver::W25QxxBlockToPage(uint32_t BlockAddress) {
  return (BlockAddress * w25qxx.BlockSize) / w25qxx.PageSize;
}

uint8_t W25Q128Driver::W25QxxIsEmptyPage(uint32_t Page_Address, uint32_t OffsetInByte) {
  while(w25qxx.Lock == 1) {
    W25QxxDelay(1);
  }

  w25qxx.Lock = 1;

  uint8_t pBuffer[256] = {0,};
  uint32_t WorkAddress = 0;
  uint16_t size = 0;

  size = w25qxx.PageSize - OffsetInByte;
  WorkAddress = (OffsetInByte + Page_Address * w25qxx.PageSize);

  W25QxxSpi(W25_FAST_READ);

  if(w25qxx.ID >= W25Q256) {
    W25QxxSpi((WorkAddress & 0xFF000000) >> 24);
  }

  W25QxxSpi((WorkAddress & 0xFF0000) >> 16);
  W25QxxSpi((WorkAddress & 0xFF00) >> 8);
  W25QxxSpi(WorkAddress & 0xFF);

  W25QxxSpi(0);

  HAL_SPI_Receive(W25QXX_SPI_PTR, pBuffer, size, 100);

  for(uint16_t i = 0; i < size; i++) {
    if(pBuffer[i] != 0xFF) {
      w25qxx.Lock = 0;
      return 0;
    }
  }

  w25qxx.Lock = 0;
  return 1;
}

uint8_t W25Q128Driver::W25QxxIsEmptySector(uint32_t Sector_Address, uint32_t OffsetInByte) {
  while(w25qxx.Lock == 1) {
    W25QxxDelay(1);
  }

  w25qxx.Lock = 1;

  uint8_t pBuffer[256] = {0,};
  uint32_t WorkAddress = 0;
  uint16_t s_buf = 256;
  uint16_t size = 0;

  size = w25qxx.SectorSize - OffsetInByte;
  WorkAddress = (OffsetInByte + Sector_Address * w25qxx.SectorSize);

  uint16_t cycle = size / 256;
  uint16_t cycle2 = size % 256;
  uint16_t count_cycle = 0;

  if(size <= 256) {
    count_cycle = 1;
  }
  else if(cycle2 == 0) {
    count_cycle = cycle;
  }
  else {
    count_cycle = cycle + 1;
  }

  for(uint16_t i = 0; i < count_cycle; i++) {
   
    W25QxxSpi(W25_FAST_READ);

    if(w25qxx.ID>=W25Q256) {
      W25QxxSpi((WorkAddress & 0xFF000000) >> 24);
    }

    W25QxxSpi((WorkAddress & 0xFF0000) >> 16);
    W25QxxSpi((WorkAddress & 0xFF00) >> 8);
    W25QxxSpi(WorkAddress & 0xFF);

    W25QxxSpi(0);

    if(size < 256) s_buf = size;

    HAL_SPI_Receive(W25QXX_SPI_PTR, pBuffer, s_buf, 100);

    for(uint16_t i = 0; i < s_buf; i++) {
      if(pBuffer[i] != 0xFF) {
        w25qxx.Lock = 0;
        return 0;
      }
    }

    size = size - 256;
    WorkAddress = WorkAddress + 256;
  }

  w25qxx.Lock = 0;
  return 1;
}

uint8_t W25Q128Driver::W25QxxIsEmptyBlock(uint32_t Block_Address, uint32_t OffsetInByte) {
  while(w25qxx.Lock == 1) {
    W25QxxDelay(1);
  }

  w25qxx.Lock = 1;

  uint8_t pBuffer[256] = {0,};
  uint32_t WorkAddress = 0;
  uint16_t s_buf = 256;
  uint32_t size = 0;

  size = w25qxx.BlockSize - OffsetInByte;
  WorkAddress = (OffsetInByte + Block_Address * w25qxx.BlockSize);

  uint16_t cycle = size / 256;
  uint16_t cycle2 = size % 256;
  uint16_t count_cycle = 0;

  if(size <= 256) {
    count_cycle = 1;
  }
  else if(cycle2 == 0) {
    count_cycle = cycle;
  }
  else {
    count_cycle = cycle + 1;
  }


  for(uint16_t i = 0; i < count_cycle; i++) {
    W25QxxSpi(W25_FAST_READ);

    if(w25qxx.ID>=W25Q256) {
      W25QxxSpi((WorkAddress & 0xFF000000) >> 24);
    }

    W25QxxSpi((WorkAddress & 0xFF0000) >> 16);
    W25QxxSpi((WorkAddress & 0xFF00) >> 8);
    W25QxxSpi(WorkAddress & 0xFF);

    W25QxxSpi(0);

    if(size < 256) s_buf = size;

    HAL_SPI_Receive(W25QXX_SPI_PTR, pBuffer, s_buf, 100);

    for(uint16_t i = 0; i < s_buf; i++) {
      if(pBuffer[i] != 0xFF) {
        w25qxx.Lock = 0;
        return 0;
      }
    }

    size = size - 256;
    WorkAddress = WorkAddress + 256;
  }

  w25qxx.Lock = 0;
  return 1;
}

void W25Q128Driver::W25QxxWriteByte(uint8_t byte, uint32_t addr) {
  while(w25qxx.Lock == 1) {
    W25QxxDelay(1);
  }

  w25qxx.Lock = 1;

  W25QxxWaitForWriteEnd();
  W25QxxWriteEnable();

  W25QxxSpi(W25_PAGE_PROGRAMM);

  if(w25qxx.ID >= W25Q256) {
    W25QxxSpi((addr & 0xFF000000) >> 24);
  }

  W25QxxSpi((addr & 0xFF0000) >> 16);
  W25QxxSpi((addr & 0xFF00) >> 8);
  W25QxxSpi(addr & 0xFF);

  W25QxxSpi(byte);

  W25QxxWaitForWriteEnd();

  w25qxx.Lock = 0;
}

void W25Q128Driver::W25QxxWritePage(uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte, 
                                    uint32_t NumByteToWrite_up_to_PageSize) {
  while(w25qxx.Lock == 1) {
    W25QxxDelay(1);
  }

  w25qxx.Lock = 1;

  if(((NumByteToWrite_up_to_PageSize + OffsetInByte) > w25qxx.PageSize) || (NumByteToWrite_up_to_PageSize == 0))
    NumByteToWrite_up_to_PageSize = w25qxx.PageSize - OffsetInByte;

  if((OffsetInByte + NumByteToWrite_up_to_PageSize) > w25qxx.PageSize)
    NumByteToWrite_up_to_PageSize = w25qxx.PageSize - OffsetInByte;


  W25QxxWaitForWriteEnd();

  W25QxxWriteEnable();

  W25QxxSpi(W25_PAGE_PROGRAMM);

  Page_Address = (Page_Address * w25qxx.PageSize) + OffsetInByte;

  if(w25qxx.ID >= W25Q256) {
    W25QxxSpi((Page_Address & 0xFF000000) >> 24);
  }

  W25QxxSpi((Page_Address & 0xFF0000) >> 16);
  W25QxxSpi((Page_Address & 0xFF00) >> 8);
  W25QxxSpi(Page_Address & 0xFF);

  HAL_SPI_Transmit(W25QXX_SPI_PTR, pBuffer, NumByteToWrite_up_to_PageSize, 100);

  W25QxxWaitForWriteEnd();

  W25QxxDelay(1);
  w25qxx.Lock = 0;
}

void W25Q128Driver::W25QxxWriteSector(uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte, 
                                      uint32_t NumByteToWrite_up_to_SectorSize) {
  if((NumByteToWrite_up_to_SectorSize > w25qxx.SectorSize) || (NumByteToWrite_up_to_SectorSize == 0))
    NumByteToWrite_up_to_SectorSize = w25qxx.SectorSize;

  uint32_t StartPage;
  int32_t BytesToWrite;
  uint32_t LocalOffset;

  if((OffsetInByte + NumByteToWrite_up_to_SectorSize) > w25qxx.SectorSize)
    BytesToWrite = w25qxx.SectorSize - OffsetInByte;
  else
    BytesToWrite = NumByteToWrite_up_to_SectorSize; 

  StartPage = W25QxxSectorToPage(Sector_Address) + (OffsetInByte / w25qxx.PageSize);
  LocalOffset = OffsetInByte % w25qxx.PageSize;

  do {   
    W25QxxWritePage(pBuffer, StartPage, LocalOffset, BytesToWrite);
    StartPage++;

    BytesToWrite -= w25qxx.PageSize - LocalOffset;
    pBuffer += w25qxx.PageSize - LocalOffset;
    LocalOffset = 0;
  } while(BytesToWrite > 0);
}

void W25Q128Driver::W25QxxWriteBlock(uint8_t* pBuffer, uint32_t Block_Address, uint32_t OffsetInByte, 
                                      uint32_t NumByteToWrite_up_to_BlockSize) {
  if((NumByteToWrite_up_to_BlockSize>w25qxx.BlockSize)||(NumByteToWrite_up_to_BlockSize == 0))
    NumByteToWrite_up_to_BlockSize=w25qxx.BlockSize;

  uint32_t  StartPage;
  int32_t   BytesToWrite;
  uint32_t  LocalOffset;

  if((OffsetInByte+NumByteToWrite_up_to_BlockSize) > w25qxx.BlockSize)
    BytesToWrite = w25qxx.BlockSize - OffsetInByte;
  else
    BytesToWrite = NumByteToWrite_up_to_BlockSize;  

  StartPage = W25QxxBlockToPage(Block_Address)+(OffsetInByte/w25qxx.PageSize);

  LocalOffset = OffsetInByte%w25qxx.PageSize; 

  do {   
    W25QxxWritePage(pBuffer,StartPage,LocalOffset,BytesToWrite);
    StartPage++;
    BytesToWrite -= w25qxx.PageSize - LocalOffset;
    pBuffer += w25qxx.PageSize - LocalOffset;
    LocalOffset = 0;
  } while(BytesToWrite > 0);
}

void W25Q128Driver::W25QxxReadByte(uint8_t *pBuffer, uint32_t Bytes_Address) {
  while(w25qxx.Lock==1){
    W25QxxDelay(1);
  }

  w25qxx.Lock=1;

  W25QxxSpi(W25_FAST_READ);

  if(w25qxx.ID >= W25Q256) {
    W25QxxSpi((Bytes_Address & 0xFF000000) >> 24);
  }

  W25QxxSpi((Bytes_Address & 0xFF0000) >> 16);
  W25QxxSpi((Bytes_Address& 0xFF00) >> 8);
  W25QxxSpi(Bytes_Address & 0xFF);
  W25QxxSpi(0);

  *pBuffer = W25QxxSpi(W25QXX_DUMMY_BYTE);

  w25qxx.Lock = 0;
}

void W25Q128Driver::W25QxxReadBytes(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead) {
  while(w25qxx.Lock == 1) {
    W25QxxDelay(1);
  }

  w25qxx.Lock = 1;

  W25QxxSpi(W25_FAST_READ);

  if(w25qxx.ID >= W25Q256) {
    W25QxxSpi((ReadAddr & 0xFF000000) >> 24);
    }
    W25QxxSpi((ReadAddr & 0xFF0000) >> 16);
    W25QxxSpi((ReadAddr& 0xFF00) >> 8);
    W25QxxSpi(ReadAddr & 0xFF);
    W25QxxSpi(0);
  

  HAL_SPI_Receive(W25QXX_SPI_PTR, pBuffer, NumByteToRead, 2000);

  W25QxxDelay(1);
  w25qxx.Lock = 0;
}

void W25Q128Driver::W25QxxReadPage(uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte, 
                                    uint32_t NumByteToRead_up_to_PageSize) {
  while(w25qxx.Lock==1) {
    W25QxxDelay(1);
  }

  w25qxx.Lock = 1;

  if((NumByteToRead_up_to_PageSize>w25qxx.PageSize) || (NumByteToRead_up_to_PageSize==0))
    NumByteToRead_up_to_PageSize=w25qxx.PageSize;

  if((OffsetInByte+NumByteToRead_up_to_PageSize) > w25qxx.PageSize)
    NumByteToRead_up_to_PageSize = w25qxx.PageSize - OffsetInByte;

  Page_Address = Page_Address * w25qxx.PageSize + OffsetInByte;

  W25QxxSpi(W25_FAST_READ);

  if(w25qxx.ID >= W25Q256) {
    W25QxxSpi((Page_Address & 0xFF000000) >> 24);
  }

  W25QxxSpi((Page_Address & 0xFF0000) >> 16);
  W25QxxSpi((Page_Address& 0xFF00) >> 8);
  W25QxxSpi(Page_Address & 0xFF);

  W25QxxSpi(0);

  HAL_SPI_Receive(W25QXX_SPI_PTR, pBuffer, NumByteToRead_up_to_PageSize, 100);

  //W25QxxDelay(1);
  w25qxx.Lock=0;
}

void W25Q128Driver::W25QxxReadSector(uint8_t *pBuffer,uint32_t Sector_Address,uint32_t OffsetInByte, 
                                      uint32_t NumByteToRead_up_to_SectorSize) { 
  if((NumByteToRead_up_to_SectorSize>w25qxx.SectorSize) || (NumByteToRead_up_to_SectorSize==0))
    NumByteToRead_up_to_SectorSize=w25qxx.SectorSize;

  uint32_t StartPage;
  int32_t BytesToRead;
  uint32_t LocalOffset;

  if((OffsetInByte + NumByteToRead_up_to_SectorSize) > w25qxx.SectorSize)
    BytesToRead = w25qxx.SectorSize - OffsetInByte;
  else
    BytesToRead = NumByteToRead_up_to_SectorSize; 

  StartPage = W25QxxSectorToPage(Sector_Address) + (OffsetInByte / w25qxx.PageSize);

  LocalOffset = OffsetInByte % w25qxx.PageSize;

  do {   
    W25QxxReadPage(pBuffer, StartPage, LocalOffset, BytesToRead);
    StartPage++;
    BytesToRead -= w25qxx.PageSize-LocalOffset;
    pBuffer += w25qxx.PageSize - LocalOffset;
    LocalOffset = 0;
  } while(BytesToRead > 0);
}

void W25Q128Driver::W25QxxReadBlock(uint8_t *pBuffer, uint32_t Block_Address, uint32_t OffsetInByte, 
                                    uint32_t NumByteToRead_up_to_BlockSize) {
  if((NumByteToRead_up_to_BlockSize > w25qxx.BlockSize) || (NumByteToRead_up_to_BlockSize == 0))
    NumByteToRead_up_to_BlockSize = w25qxx.BlockSize;

  uint32_t StartPage;
  int32_t BytesToRead;
  uint32_t LocalOffset;

  if((OffsetInByte+NumByteToRead_up_to_BlockSize) > w25qxx.BlockSize)
    BytesToRead = w25qxx.BlockSize-OffsetInByte;
  else
    BytesToRead = NumByteToRead_up_to_BlockSize;

  StartPage = W25QxxBlockToPage(Block_Address) + (OffsetInByte / w25qxx.PageSize);

  LocalOffset = OffsetInByte%w25qxx.PageSize; 

  do {   
    W25QxxReadPage(pBuffer,StartPage,LocalOffset,BytesToRead);
    StartPage++;
    BytesToRead-=w25qxx.PageSize-LocalOffset;
    pBuffer += w25qxx.PageSize - LocalOffset;
    LocalOffset=0;
  } while(BytesToRead > 0);
}

w25qxx_t W25Q128Driver::GetW25QxxInstance () {
  return w25qxx;
}