#include "sdio_hal_ex.h"

#include "stm32f4xx_ll_sdmmc.h"

uint32_t SDMMC_CmdGenCmdWrite(SDIO_TypeDef *SDIOx)
{
  SDIO_CmdInitTypeDef  sdmmc_cmdinit;
  uint32_t errorstate = SDMMC_ERROR_NONE;
  
  sdmmc_cmdinit.Argument         = 0;
  sdmmc_cmdinit.CmdIndex         = SDMMC_CMD_GEN_CMD;
  sdmmc_cmdinit.Response         = SDIO_RESPONSE_SHORT;
  sdmmc_cmdinit.WaitForInterrupt = SDIO_WAIT_NO;
  sdmmc_cmdinit.CPSM             = SDIO_CPSM_ENABLE;
  SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);
  
  // Check for error conditions
  errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_GEN_CMD, SDIO_CMDTIMEOUT);

  return errorstate;
}

HAL_StatusTypeDef HAL_MMC_GenCmdWrite(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t Timeout)
{
   SDIO_DataInitTypeDef config;
   uint32_t errorstate = HAL_MMC_ERROR_NONE;
   uint32_t tickstart = HAL_GetTick();
   uint32_t count = 0U;
   uint32_t *tempbuff = (uint32_t *)pData;

   if(NULL == pData)
   {
      hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;
      return HAL_ERROR;
   }

   if(hmmc->State != HAL_MMC_STATE_READY)
   {
      hmmc->ErrorCode |= HAL_MMC_ERROR_BUSY;
      return HAL_ERROR;
   }
  
   hmmc->ErrorCode = HAL_DMA_ERROR_NONE;

   hmmc->State = HAL_MMC_STATE_BUSY;

   // Initialize data control register
   hmmc->Instance->DCTRL = 0U;

   hmmc->Context = MMC_CONTEXT_WRITE_SINGLE_BLOCK;

   // Send CMD56 write command
   errorstate = SDMMC_CmdGenCmdWrite(hmmc->Instance);

   if(errorstate != HAL_MMC_ERROR_NONE)
   {
      // Clear all the static flags
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);  
      hmmc->ErrorCode |= errorstate;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
   }
 
   // Configure the MMC DPSM (Data Path State Machine)
   config.DataTimeOut   = SDMMC_DATATIMEOUT;
   config.DataLength    = BLOCKSIZE;
   config.DataBlockSize = SDIO_DATABLOCK_SIZE_512B;
   config.TransferDir   = SDIO_TRANSFER_DIR_TO_CARD;
   config.TransferMode  = SDIO_TRANSFER_MODE_BLOCK;
   config.DPSM          = SDIO_DPSM_ENABLE;
   SDIO_ConfigData(hmmc->Instance, &config);
 
   // Write block(s) in polling mode
#ifdef SDIO_STA_STBITERR
   while(!__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_TXUNDERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DATAEND | SDIO_FLAG_STBITERR))
#else // SDIO_STA_STBITERR not defined
   while(!__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_TXUNDERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DATAEND))
#endif // SDIO_STA_STBITERR
   {
      if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_TXFIFOHE))
      {
         // Write data to SDIO Tx FIFO
         for(count = 0U; count < 8U; count++)
         {
            SDIO_WriteFIFO(hmmc->Instance, (tempbuff + count));
         }
         tempbuff += 8U;
      }
      
      if((Timeout == 0U)||((HAL_GetTick() - tickstart) >=  Timeout))
      {
         // Clear all the static flags
         __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);  
         hmmc->ErrorCode |= errorstate;
         hmmc->State = HAL_MMC_STATE_READY;
         return HAL_TIMEOUT;
      }
   }
    
   // Get error state
   if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_DTIMEOUT))
   {
      // Clear all the static flags
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);
      hmmc->ErrorCode |= HAL_MMC_ERROR_DATA_TIMEOUT;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
   }
   else if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_DCRCFAIL))
   {
      // Clear all the static flags
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);
      hmmc->ErrorCode |= HAL_MMC_ERROR_DATA_CRC_FAIL;      
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
   }
   else if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_TXUNDERR))
   {
      // Clear all the static flags
      __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);
      hmmc->ErrorCode |= HAL_MMC_ERROR_TX_UNDERRUN;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
   }
 
   // Clear all the static flags
   __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);
 
   hmmc->State = HAL_MMC_STATE_READY;
 
   return HAL_OK;
}

uint32_t SDMMC_CmdGenCmdRead(SDIO_TypeDef *SDIOx)
{
  SDIO_CmdInitTypeDef  sdmmc_cmdinit;
  uint32_t errorstate = SDMMC_ERROR_NONE;
  
  // Set Block Size for Card
  sdmmc_cmdinit.Argument         = 1;
  sdmmc_cmdinit.CmdIndex         = SDMMC_CMD_GEN_CMD;
  sdmmc_cmdinit.Response         = SDIO_RESPONSE_SHORT;
  sdmmc_cmdinit.WaitForInterrupt = SDIO_WAIT_NO;
  sdmmc_cmdinit.CPSM             = SDIO_CPSM_ENABLE;
  SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);
  
  // Check for error conditions
  errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_GEN_CMD, SDIO_CMDTIMEOUT);

  return errorstate;
}

HAL_StatusTypeDef HAL_MMC_GenCmdRead(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t Timeout)
{
   SDIO_DataInitTypeDef config;
   uint32_t errorstate = HAL_MMC_ERROR_NONE;
   uint32_t tickstart = HAL_GetTick();
   uint32_t count = 0U, *tempbuff = (uint32_t *)pData;
  
   if(NULL == pData)
   {
      hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;
      return HAL_ERROR;
   }
 
   if(hmmc->State != HAL_MMC_STATE_READY)
   {
      hmmc->ErrorCode |= HAL_MMC_ERROR_BUSY;
      return HAL_ERROR;
   }
    
   hmmc->ErrorCode = HAL_DMA_ERROR_NONE;

   hmmc->State = HAL_MMC_STATE_BUSY;
    
   // Initialize data control register
   hmmc->Instance->DCTRL = 0U;
    
   // Configure the MMC DPSM (Data Path State Machine)
   config.DataTimeOut   = SDMMC_DATATIMEOUT;
   config.DataLength    = BLOCKSIZE;
   config.DataBlockSize = SDIO_DATABLOCK_SIZE_512B;
   config.TransferDir   = SDIO_TRANSFER_DIR_TO_SDIO;
   config.TransferMode  = SDIO_TRANSFER_MODE_BLOCK;
   config.DPSM          = SDIO_DPSM_ENABLE;
   SDIO_ConfigData(hmmc->Instance, &config);

   hmmc->Context = MMC_CONTEXT_READ_SINGLE_BLOCK;
   
   // Read Single Block command
   errorstate = SDMMC_CmdGenCmdRead(hmmc->Instance);
    
   if(errorstate != HAL_MMC_ERROR_NONE)
   {
      // Clear all the static flags
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
      hmmc->ErrorCode |= errorstate;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
   }
      
    // Poll on SDMMC flags
#ifdef SDIO_STA_STBITERR
   while(!__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DATAEND | SDIO_STA_STBITERR))
#else // SDIO_STA_STBITERR not defined
   while(!__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DATAEND))
#endif // SDIO_STA_STBITERR
   {
      if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_RXFIFOHF))
      {
         // Read data from SDMMC Rx FIFO
         for(count = 0U; count < 8U; count++)
         {
          *(tempbuff + count) = SDIO_ReadFIFO(hmmc->Instance);
         }
         tempbuff += 8U;
      }

      if((Timeout == 0U)||((HAL_GetTick()-tickstart) >=  Timeout))
      {
         // Clear all the static flags
         __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
         hmmc->ErrorCode |= HAL_MMC_ERROR_TIMEOUT;
         hmmc->State= HAL_MMC_STATE_READY;
         return HAL_TIMEOUT;
      }
   }
    
   // Get error state
   if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_DTIMEOUT))
   {
      // Clear all the static flags
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
      hmmc->ErrorCode |= HAL_MMC_ERROR_DATA_TIMEOUT;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
   }
   else if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_DCRCFAIL))
   {
      // Clear all the static flags
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
      hmmc->ErrorCode |= HAL_MMC_ERROR_DATA_CRC_FAIL;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
   }
   else if(__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_RXOVERR))
   {
      // Clear all the static flags
      __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);
      hmmc->ErrorCode |= HAL_MMC_ERROR_RX_OVERRUN;
      hmmc->State = HAL_MMC_STATE_READY;
      return HAL_ERROR;
   }
    
   // Empty FIFO if there is still any data
   while ((__HAL_MMC_GET_FLAG(hmmc, SDIO_FLAG_RXDAVL)))
   {
      *tempbuff = SDIO_ReadFIFO(hmmc->Instance);
      tempbuff++;

      if((Timeout == 0U)||((HAL_GetTick()-tickstart) >=  Timeout))
      {
         // Clear all the static flags
         __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);        
         hmmc->ErrorCode |= HAL_MMC_ERROR_TIMEOUT;
         hmmc->State= HAL_MMC_STATE_READY;
         return HAL_ERROR;
      }
   }
    
   // Clear all the static flags
   __HAL_MMC_CLEAR_FLAG(hmmc, SDMMC_STATIC_FLAGS);

   hmmc->State = HAL_MMC_STATE_READY;

   return HAL_OK;
}
