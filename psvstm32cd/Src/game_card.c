#include "game_card.h"

#include <string.h>

#include "sdio.h"
#include "mbr_types.h"
#include "cmd56.h"

HAL_StatusTypeDef StartGameCardInit(MBR* mbr)
{
   // clear mbr record
   memset((uint8_t*)mbr, 0, sizeof(MBR));
   
   // first - perform basic MMC initialization
   if(MX_SDIO_MMC_Init() != HAL_OK)
      return HAL_ERROR;
   
   // second - perform CMD56 initialization
   if(MX_SDIO_MMC_CMD56_Init() != HAL_OK)
      return HAL_ERROR;

   // perform read of first sector
   // it seems that timeout has to be large enough (2000) to be able to send first cmd17
   if(HAL_MMC_ReadBlocks(&hmmc, (uint8_t*)mbr, 0, 1, 2000) != HAL_OK)
      return HAL_ERROR;
   
   return HAL_OK;
}
