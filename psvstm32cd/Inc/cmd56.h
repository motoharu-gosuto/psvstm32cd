#pragma once

#include <stdint.h>

#include "sdio.h"

#pragma pack(push, 1)

//========================================

typedef struct cmd56_packet_request_base
{
   //basic data
   uint8_t block0[0x20];
   uint32_t responseCode;
   uint32_t additionalDataSize0;
   uint32_t expectedRespSize;

   //additional data
   uint8_t code;
   uint8_t unk;
   uint8_t additionalDataSize1;
} cmd56_packet_request_base;

typedef struct cmd56_packet_response_base
{
   uint32_t responseCode;
   uint32_t unk;
   uint16_t size; //big endian
   uint8_t errorCode;
} cmd56_packet_response_base;

//========================================

typedef struct cmd56_packet1
{
   cmd56_packet_request_base data;
} cmd56_packet1;

//========================================

typedef struct cmd56_packet2
{
   cmd56_packet_response_base data;
   uint8_t additionalData[0x10];
} cmd56_packet2;

//========================================

typedef struct cmd56_packet3
{
   cmd56_packet_request_base data;
} cmd56_packet3;

//========================================

typedef struct cmd56_packet4
{
   cmd56_packet_response_base data;
   
   uint8_t state0; //initialization status - shows if already initialized? - changes after sending packet 11 which has same structure
   uint8_t state1; //initialization status - shows if already initialized? - changes after sending packet 11 which has same structure
} cmd56_packet4;

//========================================

typedef struct cmd56_packet5
{
   cmd56_packet_request_base data;
} cmd56_packet5;

//========================================

typedef struct cmd56_packet6_out
{
   uint8_t master_key[0x20];
} cmd56_packet6_out;

typedef struct cmd56_packet6
{
   cmd56_packet_response_base data;
   
   uint8_t state0;
   uint8_t state1;
   
   uint16_t key_id;
   uint16_t param1;
   uint16_t param2;

   cmd56_packet6_out p6;
} cmd56_packet6;

//========================================

typedef struct cmd56_packet7_in
{
   uint8_t challenge0[0x10];
}cmd56_packet7_in;

typedef struct cmd56_packet7
{
   cmd56_packet_request_base data;
   uint16_t key_id;
   cmd56_packet7_in p7;
} cmd56_packet7;

//========================================

typedef struct cmd56_packet8_out
{
   uint8_t card_challenge0[0x10];
   uint8_t challenge0[0x10];
} cmd56_packet8_out;

typedef struct cmd56_packet8_out_ex
{
   uint16_t size; //big endian
   uint8_t errorCode;
   cmd56_packet8_out p8;
} cmd56_packet8_out_ex;

typedef struct cmd56_packet8
{
   cmd56_packet_response_base data;
   cmd56_packet8_out p8;
} cmd56_packet8;

//========================================

typedef struct cmd56_packet9_in
{
   uint8_t secondary_key0[0x10];
   uint8_t challenge0[0x10];
   uint8_t card_challenge0[0x10];
} cmd56_packet9_in;

typedef struct cmd56_packet9_in_ex
{
   uint8_t code;
   uint8_t unk;
   uint8_t additionalDataSize1;
   cmd56_packet9_in p9;
} cmd56_packet9_in_ex;

typedef struct cmd56_packet9
{
   cmd56_packet_request_base data;
   cmd56_packet9_in p9;
} cmd56_packet9;

//========================================

typedef struct cmd56_packet10
{
   cmd56_packet_response_base data;
} cmd56_packet10;

//========================================

typedef struct cmd56_packet11
{
   cmd56_packet_request_base data;
} cmd56_packet11;

//========================================

typedef struct cmd56_packet12
{
   cmd56_packet_response_base data;
   
   uint8_t state0; //initialization status - shows if already initialized? - changes after sending packet 11 which has same structure
   uint8_t state1; //initialization status - shows if already initialized? - changes after sending packet 11 which has same structure
} cmd56_packet12;

//========================================

typedef struct cmd56_packet13_in
{
   uint8_t challenge1[0x10];
} cmd56_packet13_in;

typedef struct cmd56_packet13
{
   cmd56_packet_request_base data;
   cmd56_packet13_in p13;
} cmd56_packet13;

//========================================

typedef struct cmd56_packet14_out
{
   uint8_t unknown0[0x08];
   uint8_t challenge1[0x10];
   uint8_t master_key[0x20];
   uint8_t unknown1[0x08];
} cmd56_packet14_out;

typedef struct cmd56_packet14_out_ex
{
   uint16_t size; //big endian
   uint8_t errorCode;
   cmd56_packet14_out p14;
} cmd56_packet14_out_ex;

typedef struct cmd56_packet14
{
   cmd56_packet_response_base data;
   cmd56_packet14_out p14;
} cmd56_packet14;

//========================================

typedef struct cmd56_packet15_17_in_aux
{
   uint8_t secondary_key1[0x10];
   uint8_t tweak_padding[0x10];
}cmd56_packet15_17_in_aux;

typedef struct cmd56_packet15_17_in
{
   cmd56_packet15_17_in_aux p1x;
   uint8_t p1x_cmac[0x10];
} cmd56_packet15_17_in;

typedef cmd56_packet15_17_in cmd56_packet15_in;

typedef cmd56_packet15_17_in cmd56_packet17_in;

typedef struct cmd56_packet15_17_in_cmac
{
   uint8_t code;
   uint8_t unk;
   uint8_t additionalDataSize1;
   uint8_t padding[0xD];
   cmd56_packet15_17_in_aux p1x;
}cmd56_packet15_17_in_cmac;

typedef struct cmd56_packet15_17_in_ex
{
   uint8_t code;
   uint8_t unk;
   uint8_t additionalDataSize1;
   cmd56_packet15_17_in p1x;
} cmd56_packet15_17_in_ex;

typedef cmd56_packet15_17_in_ex cmd56_packet15_in_ex;

typedef cmd56_packet15_17_in_ex cmd56_packet17_in_ex;

typedef cmd56_packet15_17_in_aux cmd56_packet15_in_aux;

typedef cmd56_packet15_17_in_aux cmd56_packet17_in_aux;

typedef struct cmd56_packet15
{
   cmd56_packet_request_base data;
   cmd56_packet15_in p15;
} cmd56_packet15;

//========================================

typedef struct cmd56_packet16_out_aux_part
{
   uint8_t unknown[0x20]; // most likely klicensee_key_material_p16
}cmd56_packet16_out_aux_part;

typedef struct cmd56_packet16_out_aux
{
   uint8_t secondary_key1_p15[0x10];
   cmd56_packet16_out_aux_part p16p;
}cmd56_packet16_out_aux;

typedef struct cmd56_packet16_out
{
   cmd56_packet16_out_aux p16;
   uint8_t p16_cmac[0x10];
} cmd56_packet16_out;

typedef struct cmd56_packet16_out_cmac
{
   uint16_t size; //big endian
   uint8_t errorCode;
   uint8_t padding[0xD];
   cmd56_packet16_out_aux p16;
}cmd56_packet16_out_cmac;

typedef struct cmd56_packet16_out_ex
{
   uint16_t size; //big endian
   uint8_t errorCode;
   cmd56_packet16_out p16;
} cmd56_packet16_out_ex;

typedef struct cmd56_packet16
{
   cmd56_packet_response_base data;
   cmd56_packet16_out p16;
} cmd56_packet16;

//========================================

typedef struct cmd56_packet17
{
   cmd56_packet_request_base data;
   cmd56_packet17_in p17;
} cmd56_packet17;

//========================================

typedef struct cmd56_packet18_out_aux_part
{
   uint8_t klicensee_key_material_p18[0x20];
}cmd56_packet18_out_aux_part;

typedef struct cmd56_packet18_out_aux
{
   uint8_t secondary_key1_p17[0x10];
   cmd56_packet18_out_aux_part p18p;
} cmd56_packet18_out_aux;

typedef struct cmd56_packet18_out
{
   cmd56_packet18_out_aux p18;
   uint8_t p18_cmac[0x10];
} cmd56_packet18_out;

typedef struct cmd56_packet18_out_cmac
{
   uint16_t size; //big endian
   uint8_t errorCode;
   uint8_t padding[0xD];
   cmd56_packet18_out_aux p18;
} cmd56_packet18_out_cmac;

typedef struct cmd56_packet18_out_ex
{
   uint16_t size; //big endian
   uint8_t errorCode;
   cmd56_packet18_out p18;
} cmd56_packet18_out_ex;

typedef struct cmd56_packet18
{
   cmd56_packet_response_base data;
   cmd56_packet18_out p18;
} cmd56_packet18;

//========================================

typedef struct cmd56_packet19_in
{
   uint8_t challenge2[0x10];
} cmd56_packet19_in;

typedef struct cmd56_packet19
{
   cmd56_packet_request_base data;
   cmd56_packet19_in p19;
} cmd56_packet19;

//========================================

typedef struct cmd56_packet18_20_out_aux_part
{
   uint8_t klicensee_keys[0x20];
   uint8_t rif_digest[0x14];
}cmd56_packet18_20_out_aux_part;

typedef struct cmd56_packet20_out_aux
{
   uint8_t unknown0[0x08];
   uint8_t challenge2[0x10];
   uint8_t klicensee_key_material_p20[0x20];
   uint8_t unknown1[0x08];
} cmd56_packet20_out_aux;

typedef struct cmd56_packet20_out
{
   cmd56_packet20_out_aux p20;
   uint8_t p20_cmac[0x10];
} cmd56_packet20_out;

typedef struct cmd56_packet20_out_cmac
{
   uint16_t size; //big endian
   uint8_t errorCode;
   uint8_t padding[0xD];
   cmd56_packet20_out_aux p20;
} cmd56_packet20_out_cmac;

typedef struct cmd56_packet20_out_ex
{
   uint16_t size; //big endian
   uint8_t errorCode;
   cmd56_packet20_out p20;
} cmd56_packet20_out_ex;

typedef struct cmd56_packet20
{
   cmd56_packet_response_base data;
   cmd56_packet20_out p20;
} cmd56_packet20;

//========================================

typedef struct klicensee_key_material
{
   uint8_t klicensee_key_material_p20[0x20];
   uint8_t klicensee_key_material_p18[0x20];
}klicensee_key_material;

#pragma pack(pop)

HAL_StatusTypeDef MX_SDIO_MMC_CMD56_Init(void);
