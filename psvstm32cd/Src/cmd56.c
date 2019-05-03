#include "cmd56.h"

#include <string.h>

#include "sdio_hal_ex.h"

#include "kirk.h"

static uint8_t block0[0x20] = 
{
      0xDD, 0x10, 0x25, 0x44, 0x15, 0x23, 0xFD, 0xC0, 
      0xF9, 0xE9, 0x15, 0x26, 0xDC, 0x2A, 0xE0, 0x84,
      0xA9, 0x03, 0xA2, 0x97, 0xD4, 0xBB, 0xF8, 0x52,
      0xD3, 0xD4, 0x94, 0x2C, 0x89, 0x03, 0xCC, 0x77,
};

HAL_StatusTypeDef SendPacket1(void)
{
   cmd56_packet1 p1_raw;
   memcpy(p1_raw.data.block0, block0, 0x20);
   p1_raw.data.responseCode = 0x31;
   p1_raw.data.additionalDataSize0 = 0x03;
   p1_raw.data.expectedRespSize = 0x13;
   p1_raw.data.code = 0xC4;
   p1_raw.data.unk = 0x00;
   p1_raw.data.additionalDataSize1 = 0x03;
   
   uint8_t p1_final[0x200];
   memset(p1_final, 0, 0x200);
   memcpy(p1_final, &p1_raw, sizeof(cmd56_packet1));

   if(HAL_MMC_GenCmdWrite(&hmmc, p1_final, 100) != HAL_OK)
      return HAL_ERROR;

   return HAL_OK;
}

HAL_StatusTypeDef ReceivePacket2(void)
{
   uint8_t r2_raw[0x200];
   
   if(HAL_MMC_GenCmdRead(&hmmc, r2_raw, 100) != HAL_OK)
      return HAL_ERROR;
   
   HAL_MMC_GenCmdRead(&hmmc, r2_raw, 100);
   
   cmd56_packet2 r2_final;
   memcpy(&r2_final, r2_raw, sizeof(cmd56_packet2));

   if(r2_final.data.responseCode != 0x31)
      return HAL_ERROR;

   if(r2_final.data.unk != 0x00)
      return HAL_ERROR;

   if(r2_final.data.errorCode != 0x00)
      return HAL_ERROR;

   uint16_t dataSize = (uint16_t)(r2_final.data.size << 8) | (uint16_t)(r2_final.data.size >> 8); //not in original implementation
   if(dataSize != 0x13) //not in original implementation
      return HAL_ERROR;

   return HAL_OK;
}

HAL_StatusTypeDef SendPacket3(void)
{
   cmd56_packet3 p3_raw;
   memcpy(p3_raw.data.block0, block0, 0x20);
   p3_raw.data.responseCode = 0x23;
   p3_raw.data.additionalDataSize0 = 0x03;
   p3_raw.data.expectedRespSize = 0x05;
   p3_raw.data.code = 0xC2;
   p3_raw.data.unk = 0x00;
   p3_raw.data.additionalDataSize1 = 0x03;

   uint8_t p3_final[0x200];
   memset(p3_final, 0, 0x200);
   memcpy(p3_final, &p3_raw, sizeof(cmd56_packet3));
   
   if(HAL_MMC_GenCmdWrite(&hmmc, p3_final, 100) != HAL_OK)
      return HAL_ERROR;

   return HAL_OK;
}

HAL_StatusTypeDef ReceivePacket4(void)
{
   uint8_t r4_raw[0x200];
   
   if(HAL_MMC_GenCmdRead(&hmmc, r4_raw, 100) != HAL_OK)
      return HAL_ERROR;

   cmd56_packet4 r4_final;
   memcpy(&r4_final, r4_raw, sizeof(cmd56_packet4));
   
   if(r4_final.data.responseCode != 0x23)
      return HAL_ERROR;

   if(r4_final.data.unk != 0x00)
      return HAL_ERROR;

   if(r4_final.data.errorCode != 0x00)
      return HAL_ERROR;

   uint16_t dataSize = (uint16_t)(r4_final.data.size << 8) | (uint16_t)(r4_final.data.size >> 8); //not in original implementation
   if(dataSize != 0x5) //not in original implementation
      return HAL_ERROR;

   //check initialization state
   if(r4_final.state0 == 0) //must be 0xFF
      return HAL_ERROR;

   if(r4_final.state1 != 0) //must be 0x00
      return HAL_ERROR;

   return HAL_OK;
}

HAL_StatusTypeDef SendPacket5(void)
{
   cmd56_packet5 p5_raw;
   memcpy(p5_raw.data.block0, block0, 0x20);
   p5_raw.data.responseCode = 0x02;
   p5_raw.data.additionalDataSize0 = 0x03;
   p5_raw.data.expectedRespSize = 0x2B;
   p5_raw.data.code = 0xA1;
   p5_raw.data.unk = 0x00;
   p5_raw.data.additionalDataSize1 = 0x03;

   uint8_t p5_final[0x200];
   memset(p5_final, 0, 0x200);
   memcpy(p5_final, &p5_raw, sizeof(cmd56_packet5));
   
   if(HAL_MMC_GenCmdWrite(&hmmc, p5_final, 100) != HAL_OK)
      return HAL_ERROR;

   return HAL_OK;
}

HAL_StatusTypeDef ReceivePacket6(cmd56_packet6_out* packet6_data, uint16_t* key_id)
{
   uint8_t r6_raw[0x200];
   
   if(HAL_MMC_GenCmdRead(&hmmc, r6_raw, 100) != HAL_OK)
      return HAL_ERROR;

   cmd56_packet6 r6_final;
   memcpy(&r6_final, r6_raw, sizeof(cmd56_packet6));

   if(r6_final.data.responseCode != 0x02)
      return HAL_ERROR;

   if(r6_final.data.unk != 0x00)
      return HAL_ERROR;

   if(r6_final.data.errorCode != 0x00)
      return HAL_ERROR;

   uint16_t dataSize = (uint16_t)(r6_final.data.size << 8) | (uint16_t)(r6_final.data.size >> 8); //not in original implementation
   if(dataSize != 0x2B) //not in original implementation
      return HAL_ERROR;

   uint16_t key_id_temp = (uint16_t)(r6_final.key_id << 8) | (uint16_t)(r6_final.key_id >> 8);
   //uint16_t param1_temp = (uint16_t)(r6_final.param1 << 8) | (uint16_t)(r6_final.param1 >> 8); //not in original implementation
   //uint16_t param2_temp = (uint16_t)(r6_final.param2 << 8) | (uint16_t)(r6_final.param2 >> 8); //not in original implementation

   if((key_id_temp & (0x7FFF)) != 1)
      return HAL_ERROR;

   if(r6_final.state0 == 0x00) // must be 0xE0 - not in original implementation
      return HAL_ERROR;

   if(r6_final.state1 != 0) //not in original implementation
      return HAL_ERROR;

   memcpy(packet6_data, &r6_final.p6, sizeof(cmd56_packet6_out));
   
   *key_id = key_id_temp;
   
   return HAL_OK;
}

HAL_StatusTypeDef SendPacket7(uint16_t key_id, cmd56_packet7_in* packet7_data)
{
   cmd56_packet7 p7_raw;
   memcpy(p7_raw.data.block0, block0, 0x20);
   p7_raw.data.responseCode = 0x03;
   p7_raw.data.additionalDataSize0 = 0x15;
   p7_raw.data.expectedRespSize = 0x23;
   p7_raw.data.code = 0xA2;
   p7_raw.data.unk = 0x00;
   p7_raw.data.additionalDataSize1 = 0x15;

   p7_raw.key_id = key_id;
   p7_raw.key_id = (uint16_t)(p7_raw.key_id << 8) | (uint16_t)(p7_raw.key_id >> 8);
   memcpy(&p7_raw.p7, packet7_data, sizeof(cmd56_packet7_in));
   
   uint8_t p7_final[0x200];
   memset(p7_final, 0, 0x200);
   memcpy(p7_final, &p7_raw, sizeof(cmd56_packet7));

   if(HAL_MMC_GenCmdWrite(&hmmc, p7_final, 100) != HAL_OK)
      return HAL_ERROR;

   return HAL_OK;
}

HAL_StatusTypeDef ReceivePacket8(cmd56_packet8_out_ex* packet8_data)
{
   uint8_t r8_raw[0x200];
   
   if(HAL_MMC_GenCmdRead(&hmmc, r8_raw, 100) != HAL_OK)
      return HAL_ERROR;

   cmd56_packet8 r8_final;
   memcpy(&r8_final, r8_raw, sizeof(cmd56_packet8));
      
   if(r8_final.data.responseCode != 0x03)
      return HAL_ERROR;

   if(r8_final.data.unk != 0x00)
      return HAL_ERROR;

   if(r8_final.data.errorCode != 0x00)
      return HAL_ERROR;

   uint16_t dataSize = (uint16_t)(r8_final.data.size << 8) | (uint16_t)(r8_final.data.size >> 8); //not in original implementation
   if(dataSize != 0x23) //not in original implementation
      return HAL_ERROR;

   packet8_data->size = r8_final.data.size;
   packet8_data->errorCode = r8_final.data.errorCode;
   memcpy(&packet8_data->p8, &r8_final.p8, sizeof(cmd56_packet8_out));

   return HAL_OK;
}

HAL_StatusTypeDef SendPacket9(cmd56_packet9_in_ex* packet9_data)
{
   cmd56_packet9 p9_raw;
   memcpy(p9_raw.data.block0, block0, 0x20);
   p9_raw.data.responseCode = 0x05;
   p9_raw.data.additionalDataSize0 = 0x33;
   p9_raw.data.expectedRespSize = 0x03;
   p9_raw.data.code = packet9_data->code;
   p9_raw.data.unk = packet9_data->unk;
   p9_raw.data.additionalDataSize1 = packet9_data->additionalDataSize1;

   memcpy(&p9_raw.p9, &packet9_data->p9, sizeof(cmd56_packet9_in));

   uint8_t p9_final[0x200];
   memset(p9_final, 0, 0x200);
   memcpy(p9_final, &p9_raw, sizeof(cmd56_packet9));

   if(HAL_MMC_GenCmdWrite(&hmmc, p9_final, 100) != HAL_OK)
      return HAL_ERROR;

   return HAL_OK;
}

HAL_StatusTypeDef ReceivePacket10(void)
{
   uint8_t r10_raw[0x200];
   
   if(HAL_MMC_GenCmdRead(&hmmc, r10_raw, 100) != HAL_OK)
      return HAL_ERROR;

   cmd56_packet10 r10_final;
   memcpy(&r10_final, r10_raw, sizeof(cmd56_packet10));
    
   if(r10_final.data.responseCode != 0x05)
      return HAL_ERROR;
   
   if(r10_final.data.unk != 0x00)
      return HAL_ERROR;

   if(r10_final.data.errorCode != 0x00)
      return HAL_ERROR;
   
   uint16_t dataSize = (uint16_t)(r10_final.data.size << 8) | (uint16_t)(r10_final.data.size >> 8); //not in original implementation
   if(dataSize != 0x3) //not in original implementation
      return HAL_ERROR;

   return HAL_OK;
}

HAL_StatusTypeDef SendPacket11()
{
   cmd56_packet11 p11_raw;
   memcpy(p11_raw.data.block0, block0, 0x20);
   p11_raw.data.responseCode = 0x23;
   p11_raw.data.additionalDataSize0 = 0x03;
   p11_raw.data.expectedRespSize = 0x05;
   p11_raw.data.code = 0xC2;
   p11_raw.data.unk = 0x00;
   p11_raw.data.additionalDataSize1 = 0x03;

   uint8_t p11_final[0x200];
   memset(p11_final, 0, 0x200);
   memcpy(p11_final, &p11_raw, sizeof(cmd56_packet11));

   if(HAL_MMC_GenCmdWrite(&hmmc, p11_final, 100) != HAL_OK)
      return HAL_ERROR;

   return HAL_OK;
}

HAL_StatusTypeDef ReceivePacket12()
{
   uint8_t r12_raw[0x200];
   
   if(HAL_MMC_GenCmdRead(&hmmc, r12_raw, 100) != HAL_OK)
      return HAL_ERROR;

   cmd56_packet12 r12_final;
   memcpy(&r12_final, r12_raw, sizeof(cmd56_packet12));
   
   if(r12_final.data.responseCode != 0x23)
      return HAL_ERROR;

   if(r12_final.data.unk != 0x00)
      return HAL_ERROR;

   if(r12_final.data.errorCode != 0x00)
      return HAL_ERROR;

   uint16_t dataSize = (uint16_t)(r12_final.data.size << 8) | (uint16_t)(r12_final.data.size >> 8); //not in original implementation
   if(dataSize != 0x5) //not in original implementation
      return HAL_ERROR;

   //check initialization state
   if(r12_final.state0 != 0) //must be 0x00
      return HAL_ERROR;

   if(r12_final.state1 != 0) //must be 0x00
      return HAL_ERROR;

   return HAL_OK;
}

HAL_StatusTypeDef SendPacket13(cmd56_packet13_in* packet13_data)
{
   cmd56_packet13 p13_raw;
   memcpy(p13_raw.data.block0, block0, 0x20);
   p13_raw.data.responseCode = 0x07;
   p13_raw.data.additionalDataSize0 = 0x13;
   p13_raw.data.expectedRespSize = 0x43;
   p13_raw.data.code = 0xA4;
   p13_raw.data.unk = 0x00;
   p13_raw.data.additionalDataSize1 = 0x13;

   memcpy(&p13_raw.p13, packet13_data, sizeof(cmd56_packet13_in));
   
   uint8_t p13_final[0x200];
   memset(p13_final, 0, 0x200);
   memcpy(p13_final, &p13_raw, sizeof(cmd56_packet13));

   if(HAL_MMC_GenCmdWrite(&hmmc, p13_final, 100) != HAL_OK)
      return HAL_ERROR;

   return HAL_OK;
}

HAL_StatusTypeDef ReceivePacket14(cmd56_packet14_out_ex* packet14_data)
{
   uint8_t r14_raw[0x200];
   
   if(HAL_MMC_GenCmdRead(&hmmc, r14_raw, 100) != HAL_OK)
      return HAL_ERROR;

   cmd56_packet14 r14_final;
   memcpy(&r14_final, r14_raw, sizeof(cmd56_packet14));
      
   if(r14_final.data.responseCode != 0x07)
      return HAL_ERROR;

   if(r14_final.data.unk != 0x00)
      return HAL_ERROR;

   if(r14_final.data.errorCode != 0x00)
      return HAL_ERROR;

   uint16_t dataSize = (uint16_t)(r14_final.data.size << 8) | (uint16_t)(r14_final.data.size >> 8); //not in original implementation
   if(dataSize != 0x43) //not in original implementation
      return HAL_ERROR;

   packet14_data->size = r14_final.data.size;
   packet14_data->errorCode = r14_final.data.errorCode;
   memcpy(&packet14_data->p14, &r14_final.p14, sizeof(cmd56_packet14_out));

   return HAL_OK;
};

HAL_StatusTypeDef SendPacket15(cmd56_packet15_in_ex* packet15_data)
{
   cmd56_packet15 p15_raw;
   memcpy(p15_raw.data.block0, block0, 0x20);
   p15_raw.data.responseCode = 0x11;
   p15_raw.data.additionalDataSize0 = 0x33;
   p15_raw.data.expectedRespSize = 0x43;
   p15_raw.data.code = packet15_data->code;
   p15_raw.data.unk = packet15_data->unk;
   p15_raw.data.additionalDataSize1 = packet15_data->additionalDataSize1;

   memcpy(&p15_raw.p15, &packet15_data->p1x, sizeof(cmd56_packet15_in));

   uint8_t p15_final[0x200];
   memset(p15_final, 0, 0x200);
   memcpy(p15_final, &p15_raw, sizeof(cmd56_packet15));
   
   if(HAL_MMC_GenCmdWrite(&hmmc, p15_final, 100) != HAL_OK)
      return HAL_ERROR;

   return HAL_OK;
}

HAL_StatusTypeDef ReceivePacket16(cmd56_packet16_out_ex* packet16_data)
{
   uint8_t r16_raw[0x200];
   
   if(HAL_MMC_GenCmdRead(&hmmc, r16_raw, 100) != HAL_OK)
      return HAL_ERROR;

   cmd56_packet16 r16_final;
   memcpy(&r16_final, r16_raw, sizeof(cmd56_packet16));
      
   if(r16_final.data.responseCode != 0x11)
      return HAL_ERROR;

   if(r16_final.data.unk != 0x00)
      return HAL_ERROR;

   if(r16_final.data.errorCode != 0x00)
      return HAL_ERROR;

   uint16_t dataSize = (uint16_t)(r16_final.data.size << 8) | (uint16_t)(r16_final.data.size >> 8); //not in original implementation
   if(dataSize != 0x43) //not in original implementation
      return HAL_ERROR;

   packet16_data->size = r16_final.data.size;
   packet16_data->errorCode = r16_final.data.errorCode;
   memcpy(&packet16_data->p16, &r16_final.p16, sizeof(cmd56_packet16_out));

   return HAL_OK;
};

HAL_StatusTypeDef SendPacket17(cmd56_packet17_in_ex* packet17_data)
{
   cmd56_packet17 p17_raw;
   memcpy(p17_raw.data.block0, block0, 0x20);
   p17_raw.data.responseCode = 0x11;
   p17_raw.data.additionalDataSize0 = 0x33;
   p17_raw.data.expectedRespSize = 0x43;
   p17_raw.data.code = packet17_data->code;
   p17_raw.data.unk = packet17_data->unk;
   p17_raw.data.additionalDataSize1 = packet17_data->additionalDataSize1;

   memcpy(&p17_raw.p17, &packet17_data->p1x, sizeof(cmd56_packet17_in));
   
   uint8_t p17_final[0x200];
   memset(p17_final, 0, 0x200);
   memcpy(p17_final, &p17_raw, sizeof(cmd56_packet17));

   if(HAL_MMC_GenCmdWrite(&hmmc, p17_final, 100) != HAL_OK)
      return HAL_ERROR;

   return HAL_OK;
}

HAL_StatusTypeDef ReceivePacket18(cmd56_packet18_out_ex* packet18_data)
{
   uint8_t r18_raw[0x200];

   if(HAL_MMC_GenCmdRead(&hmmc, r18_raw, 100) != HAL_OK)
      return HAL_ERROR;

   cmd56_packet18 r18_final;
   memcpy(&r18_final, r18_raw, sizeof(cmd56_packet18));
      
   if(r18_final.data.responseCode != 0x11)
      return HAL_ERROR;

   if(r18_final.data.unk != 0x00)
      return HAL_ERROR;

   if(r18_final.data.errorCode != 0x00)
      return HAL_ERROR;

   uint16_t dataSize = (uint16_t)(r18_final.data.size << 8) | (uint16_t)(r18_final.data.size >> 8); //not in original implementation
   if(dataSize != 0x43) //not in original implementation
      return HAL_ERROR;

   packet18_data->size = r18_final.data.size;
   packet18_data->errorCode = r18_final.data.errorCode;
   memcpy(&packet18_data->p18, &r18_final.p18, sizeof(cmd56_packet18_out));

   return HAL_OK;
};

HAL_StatusTypeDef SendPacket19(cmd56_packet19_in* packet19_data)
{
   cmd56_packet19 p19_raw;
   memcpy(p19_raw.data.block0, block0, 0x20);
   p19_raw.data.responseCode = 0x19;
   p19_raw.data.additionalDataSize0 = 0x13;
   p19_raw.data.expectedRespSize = 0x53;
   p19_raw.data.code = 0xC1;
   p19_raw.data.unk = 0x00;
   p19_raw.data.additionalDataSize1 = 0x13;

   memcpy(&p19_raw.p19, packet19_data, sizeof(cmd56_packet19_in));
   
   uint8_t p19_final[0x200];
   memset(p19_final, 0, 0x200);
   memcpy(p19_final, &p19_raw, sizeof(cmd56_packet19));

   if(HAL_MMC_GenCmdWrite(&hmmc, p19_final, 100) != HAL_OK)
      return HAL_ERROR;

   return HAL_OK;
}

HAL_StatusTypeDef ReceivePacket20(cmd56_packet20_out_ex* packet20_data)
{
   uint8_t r20_raw[0x200];

   if(HAL_MMC_GenCmdRead(&hmmc, r20_raw, 100) != HAL_OK)
      return HAL_ERROR;

   cmd56_packet20 r20_final;
   memcpy(&r20_final, r20_raw, sizeof(cmd56_packet20));
      
   if(r20_final.data.responseCode != 0x19)
      return HAL_ERROR;

   if(r20_final.data.unk != 0x00)
      return HAL_ERROR;

   if(r20_final.data.errorCode != 0x00)
      return HAL_ERROR;

   uint16_t dataSize = (uint16_t)(r20_final.data.size << 8) | (uint16_t)(r20_final.data.size >> 8); //not in original implementation
   if(dataSize != 0x53) //not in original implementation
      return HAL_ERROR;

   packet20_data->size = r20_final.data.size;
   packet20_data->errorCode = r20_final.data.errorCode;
   memcpy(&packet20_data->p20, &r20_final.p20, sizeof(cmd56_packet20_out));

   return HAL_OK;
};

// seems delay beween commands may affect how well initialization works (maybe card measures timing)
// we use delay because in send commands card drives DAT0 low for some period of time after data it sent
// not sure what is the reason

//minimal delay
//#define PACKET_DELAY 40
#define PACKET_DELAY 100

//challenge0 should be random but for now it is static for easier debugging

HAL_StatusTypeDef GenerateChallenge0(cmd56_packet7_in* packet7_data)
{
   packet7_data->challenge0[0] = 0x00;
   packet7_data->challenge0[1] = 0x01;
   packet7_data->challenge0[2] = 0x02;
   packet7_data->challenge0[3] = 0x03;
   packet7_data->challenge0[4] = 0x04;
   packet7_data->challenge0[5] = 0x05;
   packet7_data->challenge0[6] = 0x06;
   packet7_data->challenge0[7] = 0x07;
   packet7_data->challenge0[8] = 0x08;
   packet7_data->challenge0[9] = 0x09;
   packet7_data->challenge0[10] = 0x0A;
   packet7_data->challenge0[11] = 0x0B;
   packet7_data->challenge0[12] = 0x0C;
   packet7_data->challenge0[13] = 0x0D;
   packet7_data->challenge0[14] = 0x0E;
   packet7_data->challenge0[15] = 0x0F;
   
   return HAL_OK;
}

//challenge1 should be random but for now it is static for easier debugging

HAL_StatusTypeDef GenerateChallenge1(cmd56_packet13_in* packet13_data)
{
   packet13_data->challenge1[0] = 0x20 + 0x00;
   packet13_data->challenge1[1] = 0x20 + 0x01;
   packet13_data->challenge1[2] = 0x20 + 0x02;
   packet13_data->challenge1[3] = 0x20 + 0x03;
   packet13_data->challenge1[4] = 0x20 + 0x04;
   packet13_data->challenge1[5] = 0x20 + 0x05;
   packet13_data->challenge1[6] = 0x20 + 0x06;
   packet13_data->challenge1[7] = 0x20 + 0x07;
   packet13_data->challenge1[8] = 0x20 + 0x08;
   packet13_data->challenge1[9] = 0x20 + 0x09;
   packet13_data->challenge1[10] = 0x20 + 0x0A;
   packet13_data->challenge1[11] = 0x20 + 0x0B;
   packet13_data->challenge1[12] = 0x20 + 0x0C;
   packet13_data->challenge1[13] = 0x20 + 0x0D;
   packet13_data->challenge1[14] = 0x20 + 0x0E;
   packet13_data->challenge1[15] = 0x20 + 0x0F;
   
   return HAL_OK;
}

//challenge2 should be random but for now it is static for easier debugging

HAL_StatusTypeDef GenerateChallenge2(cmd56_packet19_in* packet19_data)
{
   packet19_data->challenge2[0] = 0x40 + 0x00;
   packet19_data->challenge2[1] = 0x40 + 0x01;
   packet19_data->challenge2[2] = 0x40 + 0x02;
   packet19_data->challenge2[3] = 0x40 + 0x03;
   packet19_data->challenge2[4] = 0x40 + 0x04;
   packet19_data->challenge2[5] = 0x40 + 0x05;
   packet19_data->challenge2[6] = 0x40 + 0x06;
   packet19_data->challenge2[7] = 0x40 + 0x07;
   packet19_data->challenge2[8] = 0x40 + 0x08;
   packet19_data->challenge2[9] = 0x40 + 0x09;
   packet19_data->challenge2[10] = 0x40 + 0x0A;
   packet19_data->challenge2[11] = 0x40 + 0x0B;
   packet19_data->challenge2[12] = 0x40 + 0x0C;
   packet19_data->challenge2[13] = 0x40 + 0x0D;
   packet19_data->challenge2[14] = 0x40 + 0x0E;
   packet19_data->challenge2[15] = 0x40 + 0x0F;
   
   return HAL_OK;
}

HAL_StatusTypeDef MX_SDIO_MMC_CMD56_Init(void)
{
   if(SendPacket1() != HAL_OK)
      return HAL_ERROR;
   
   HAL_Delay(PACKET_DELAY); 
   
   if(ReceivePacket2() != HAL_OK)
      return HAL_ERROR;
   
   HAL_Delay(PACKET_DELAY); 
   
   if(SendPacket3() != HAL_OK)
      return HAL_ERROR;
   
   HAL_Delay(PACKET_DELAY);
   
   if(ReceivePacket4() != HAL_OK)
      return HAL_ERROR;
   
   HAL_Delay(PACKET_DELAY);
   
   if(SendPacket5() != HAL_OK)
      return HAL_ERROR;
   
   HAL_Delay(PACKET_DELAY);
   
   cmd56_packet6_out packet6_data;
   memset(&packet6_data, 0, sizeof(cmd56_packet6_out));

   uint16_t key_id = 0;
   
   if(ReceivePacket6(&packet6_data, &key_id) != HAL_OK)
      return HAL_ERROR;
   
   HAL_Delay(PACKET_DELAY);
   
   cmd56_packet7_in packet7_data;
   memset(&packet7_data, 0, sizeof(cmd56_packet7_in));
   
   if(GenerateChallenge0(&packet7_data) != HAL_OK)
      return HAL_ERROR;
   
   packet7_data.challenge0[0] = (packet7_data.challenge0[0] | (~0x7F));
   
   // packet 7 should contain challenge0 for the card that can be encrypted (by card) with key_id and master_key.
   if(SendPacket7(key_id, &packet7_data) != HAL_OK)
      return HAL_ERROR;
   
   HAL_Delay(PACKET_DELAY);
   
   cmd56_packet8_out_ex packet8_data;
   memset(&packet8_data, 0, sizeof(cmd56_packet8_out_ex));
   
   // packet 8 should contain encrypted message which can be decrypted (by vita) with key_id and master_key. 
   // part of message should be card_challenge0. 
   // another part of the message should be equal to challenge0.
   // this way we know that card knows how to properly encrypt.
   if(ReceivePacket8(&packet8_data) != HAL_OK)
      return HAL_ERROR;
   
   // kirk service 1C will decrypt packet 8 with key_id and master_key
   // then it will verify challenge0
   if(KirkSendCommand_1B(key_id, &packet6_data, &packet7_data, &packet8_data) != HAL_OK)
      return HAL_ERROR;
   
   HAL_Delay(PACKET_DELAY);
   
   cmd56_packet9_in_ex packet9_data;
   memset(&packet9_data, 0, sizeof(cmd56_packet9_in_ex));
   
   // kirk service 1C will generate packet 9
   // it will decrypt packet 8 to retrieve challenge0 and card_challenge0
   // then challenge0 and card_challenge0 will be tweaked
   // then it will generate secondary_key0
   // then packet 9 will be encrypted with key_id and master_key
   if(KirkSendCommand_1C(key_id, &packet6_data, &packet8_data.p8, &packet9_data) != HAL_OK)
      return HAL_ERROR;
   
   // packet 9 should contain encrypted message which can be decrypted (by card) with key_id and master_key. 
   // message should have secondary_key0, tweaked challenge0 and tweaked card_challenge0.
   // this way card will know that we know how to properly encrypt the data and we know the layout of the data because we tweak and reorder certain fields.
   if(SendPacket9(&packet9_data) != HAL_OK)
      return HAL_ERROR;
   
   HAL_Delay(PACKET_DELAY);
   
   if(ReceivePacket10() != HAL_OK)
      return HAL_ERROR;
   
   HAL_Delay(PACKET_DELAY);
   
   if(SendPacket11() != HAL_OK)
      return HAL_ERROR;
   
   HAL_Delay(PACKET_DELAY);
   
   if(ReceivePacket12() != HAL_OK)
      return HAL_ERROR;
  
   HAL_Delay(PACKET_DELAY);
   
   cmd56_packet13_in packet13_data;
   memset(&packet13_data, 0, sizeof(cmd56_packet13_in));
   
   if(GenerateChallenge1(&packet13_data) != HAL_OK)
      return HAL_ERROR;
   
   packet13_data.challenge1[0] = (packet13_data.challenge1[0] | (~0x7F));
   
   // packet 13 should contain challenge1 for the card that can be encrypted (by card) with key_id and master_key.
   if(SendPacket13(&packet13_data) != HAL_OK)
      return HAL_ERROR;
   
   HAL_Delay(PACKET_DELAY);
   
   // packet 14 should contain challenge1 and master_key that are encrypted (by card) with key_id and master_key
   cmd56_packet14_out_ex packet14_data;
   memset(&packet14_data, 0, sizeof(cmd56_packet14_out_ex));
   
   if(ReceivePacket14(&packet14_data) != HAL_OK)
      return HAL_ERROR;
   
   // kirk service 1D will decrypt secondary_key0 from packet 9 with key_id and master_key
   // then it will decrypt packet 14
   // then it will verify challenge1 in packet 13 against decrypted packet 14
   // then it will verify master_key in packet 6 against decrypted packet 14
   if(KirkSendCommand_1D(key_id, &packet6_data, &packet9_data.p9, &packet13_data, &packet14_data) != HAL_OK)
      return HAL_ERROR;
   
   HAL_Delay(PACKET_DELAY);
   
   cmd56_packet15_in_ex packet15_data;
   memset(&packet15_data, 0, sizeof(cmd56_packet15_in_ex));
   
   // kirk service 0x1E will generate packet 15
   // it will decrypt secondary_key0 from packet 9 with key_id and master_key
   // then it will generate secondary_key1
   // then it will create a buffer with tweaked secondary_key1 and tweak_padding
   // then it will encrypt the buffer using secondary_key0
   // then it will create cmd56 input like buffer with encrypted buffer
   // then it will calculate cmac of cmd56 like buffer using secondary_key0
   // encrypted buffer and cmac will be the resulting data of packet 15
   if(KirkSendCommand_1E(key_id, 0x02, &packet6_data, &packet9_data.p9, &packet15_data) != HAL_OK)
      return HAL_ERROR;

   if(SendPacket15(&packet15_data) != HAL_OK)
      return HAL_ERROR;
   
   HAL_Delay(PACKET_DELAY);
   
   cmd56_packet16_out_ex packet16_data;
   memset(&packet16_data, 0, sizeof(cmd56_packet16_out_ex));

   // packet 16 will contain ? 
   if(ReceivePacket16(&packet16_data) != HAL_OK)
      return HAL_ERROR;
   
   cmd56_packet16_out_aux_part packet16_data_aux_part;
   memset(&packet16_data_aux_part, 0, sizeof(cmd56_packet16_out_aux_part));
   
   // kirk service 0x1E will decrypt secondary_key0 from packet 9 with key_id and master_key
   // then it will combine data from packet 16 into a cmd56 like buffer
   // then it will calculate cmac of that cmd56 like buffer using secondary_key0
   // then it will verify calculated cmac against cmac from packet 16
   // then it will decrypt secondary_key1 and tweak_padding from packet 15
   // then it will decrypt secondary_key1 and unknown data from packet 16
   // then it will verify decrypted secondary_key1 from packet 15 against decrypted secondary_key1 from packet 16
   // then it will verify that decryped tweak_padding from packet 15 is properly tweaked
   // then it will return unknown decrypted data from packet 16
   if(KirkSendCommand_1F(key_id, &packet6_data, &packet9_data.p9, &packet15_data.p1x.p1x, &packet16_data, &packet16_data_aux_part) != HAL_OK)
      return HAL_ERROR;
   
   HAL_Delay(PACKET_DELAY);
   
   cmd56_packet17_in_ex packet17_data;
   memset(&packet17_data, 0, sizeof(cmd56_packet17_in_ex));

   // kirk service 0x1E will generate packet 17
   // it will decrypt secondary_key0 from packet 9 with key_id and master_key
   // then it will generate secondary_key1
   // then it will create a buffer with tweaked secondary_key1 and tweak_padding
   // then it will encrypt the buffer using secondary_key0
   // then it will create cmd56 input like buffer with encrypted buffer
   // then it will calculate cmac of cmd56 like buffer using secondary_key0
   // encrypted buffer and cmac will be the resulting data of packet 17
   if(KirkSendCommand_1E(key_id, 0x03, &packet6_data, &packet9_data.p9, &packet17_data) != HAL_OK)
      return HAL_ERROR;
   
   if(SendPacket17(&packet17_data) != HAL_OK)
      return HAL_ERROR;
   
   HAL_Delay(PACKET_DELAY);
   
   cmd56_packet18_out_ex packet18_data;
   memset(&packet18_data, 0, sizeof(cmd56_packet18_out_ex));

   if(ReceivePacket18(&packet18_data) != HAL_OK)
      return HAL_ERROR;
   
   HAL_Delay(PACKET_DELAY);
   
   cmd56_packet19_in packet19_data;
   memset(&packet19_data, 0, sizeof(cmd56_packet19_in));
   
   if(GenerateChallenge2(&packet19_data) != HAL_OK)
      return HAL_ERROR;
   
   packet19_data.challenge2[0] = (packet19_data.challenge2[0] | (~0x7F));
   
   if(SendPacket19(&packet19_data) != HAL_OK)
      return HAL_ERROR;

   HAL_Delay(PACKET_DELAY);
   
   cmd56_packet20_out_ex packet20_data;
   memset(&packet20_data, 0, sizeof(cmd56_packet20_out_ex));
   
   if(ReceivePacket20(&packet20_data) != HAL_OK)
      return HAL_ERROR;
   
   cmd56_packet18_20_out_aux_part packet18_20_aux_part;
   memset(&packet18_20_aux_part, 0, sizeof(cmd56_packet18_20_out_aux_part));
   
   // need to explain what this command does
   if(KirkSendCommand_20(key_id, &packet6_data, &packet9_data.p9, &packet17_data.p1x.p1x, &packet18_data, &packet19_data, &packet20_data, &packet18_20_aux_part) != HAL_OK)
      return HAL_ERROR;
   
   HAL_Delay(PACKET_DELAY);

   return HAL_OK;
}

