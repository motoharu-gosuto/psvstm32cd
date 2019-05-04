#include "usbd_storage_if.h"

#include <string.h>

#include "mbr_types.h"
#include "sdio.h"

#define MOUNT_GRO_AS_EXFAT

#define STORAGE_LUN_NBR                  1

// USB Mass storage Standard Inquiry Data.
const int8_t STORAGE_Inquirydata_FS[] = {/* 36 */
  
  /* LUN 0 */
  0x00,
  0x80,
  0x02,
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,	
  0x00,
  'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', /* Manufacturer : 8 bytes */
  'P', 'r', 'o', 'd', 'u', 'c', 't', ' ', /* Product      : 16 Bytes */
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  '0', '.', '0' ,'1'                      /* Version      : 4 Bytes */
};

extern USBD_HandleTypeDef hUsbDeviceFS;

static int8_t STORAGE_Init_FS(uint8_t lun);
static int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size);
static int8_t STORAGE_IsReady_FS(uint8_t lun);
static int8_t STORAGE_IsWriteProtected_FS(uint8_t lun);
static int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_GetMaxLun_FS(void);

USBD_StorageTypeDef USBD_Storage_Interface_fops_FS =
{
  STORAGE_Init_FS,
  STORAGE_GetCapacity_FS,
  STORAGE_IsReady_FS,
  STORAGE_IsWriteProtected_FS,
  STORAGE_Read_FS,
  STORAGE_Write_FS,
  STORAGE_GetMaxLun_FS,
  (int8_t *)STORAGE_Inquirydata_FS
};

MBR game_card_mbr;
int gro_partition_index = -1;

// Initializes over USB FS IP
int8_t STORAGE_Init_FS(uint8_t lun)
{
   return (USBD_OK);
}

int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
   *block_num  = 0;
   *block_size = 0;
   
   // read sony mbr
   if(HAL_MMC_ReadBlocks(&hmmc, (uint8_t*)&game_card_mbr, 0, 1, 100) != HAL_OK)
      return USBD_FAIL;
   
   // check header
   if(memcmp(SCEHeader, game_card_mbr.header, sizeof(game_card_mbr.header)) != 0)
      return USBD_FAIL;
   
   // find gro0 partition
   for(int i = 0; i < NPartitions; i++)
   {
      PartitionEntry* pe = &game_card_mbr.partitions[i];
      if(pe->partitionCode == gro0)
      {
         gro_partition_index = i;
         break;
      }
   }
   
   #ifdef MOUNT_GRO_AS_EXFAT
   *block_num  = game_card_mbr.partitions[gro_partition_index].partitionSize;
   *block_size = SD_DEFAULT_SECTOR_SIZE;
   #else
   *block_num = game_card_mbr.sizeInBlocks;
   *block_size = SD_DEFAULT_SECTOR_SIZE;
   #endif

   return (USBD_OK);
}

int8_t STORAGE_IsReady_FS(uint8_t lun)
{
   return (USBD_OK);
}

int8_t STORAGE_IsWriteProtected_FS(uint8_t lun)
{
   return (USBD_FAIL); // Read Only
}

int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
   #ifdef MOUNT_GRO_AS_EXFAT
   if(HAL_MMC_ReadBlocks(&hmmc, buf, game_card_mbr.partitions[gro_partition_index].partitionOffset + blk_addr, blk_len, 20) != HAL_OK)
      return USBD_FAIL;
   #else
   if(HAL_MMC_ReadBlocks(&hmmc, buf, blk_addr, blk_len, 20) != HAL_OK)
      return USBD_FAIL;
   #endif
   
   return (USBD_OK);
}

int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
   return (USBD_FAIL); // Read Only
}

int8_t STORAGE_GetMaxLun_FS(void)
{
   return (STORAGE_LUN_NBR - 1);
}
