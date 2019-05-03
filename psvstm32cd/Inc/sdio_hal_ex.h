#pragma once

#include <stdint.h>

#include "sdio.h"

uint32_t SDMMC_CmdGenCmdWrite(SDIO_TypeDef *SDIOx);

HAL_StatusTypeDef HAL_MMC_GenCmdWrite(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t Timeout);

uint32_t SDMMC_CmdGenCmdRead(SDIO_TypeDef *SDIOx);

HAL_StatusTypeDef HAL_MMC_GenCmdRead(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t Timeout);
