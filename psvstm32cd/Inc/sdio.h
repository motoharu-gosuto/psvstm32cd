#ifndef __sdio_H

   #define __sdio_H
   
   #ifdef __cplusplus
    extern "C" {
   #endif

   #include "main.h"

   extern MMC_HandleTypeDef hmmc;

   HAL_StatusTypeDef MX_SDIO_MMC_Init(void);

   #ifdef __cplusplus
   }
   #endif
   
#endif
