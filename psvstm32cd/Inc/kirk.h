#pragma once

#include <stdint.h>

#include "cmd56.h"

//========================================

#pragma pack(push, 1)

typedef struct kirk_1B_input
{
   cmd56_packet6_out packet6; //0x20
   cmd56_packet7_in packet7; //0x10
   cmd56_packet8_out_ex packet8; //0x23
} kirk_1B_input;

typedef struct kirk_1C_input
{
   cmd56_packet6_out packet6; //0x20
   cmd56_packet8_out packet8; //0x20
} kirk_1C_input;

typedef struct kirk_1C_output
{
   cmd56_packet9_in_ex packet9; //0x33
} kirk_1C_output;

typedef struct kirk_1D_input
{
   cmd56_packet6_out packet6; // 0x20
   cmd56_packet9_in packet9; // 0x30
   cmd56_packet13_in packet13; // 0x10
   cmd56_packet14_out_ex packet14;// 0x43
} kirk_1D_input;

typedef struct kirk_1E_input
{
   cmd56_packet6_out packet6; // 0x20
   cmd56_packet9_in packet9; // 0x30
   uint8_t tweak_seed;
} kirk_1E_input;

typedef struct kirk_1E_output
{
   cmd56_packet15_17_in_ex packet1x; //0x33
} kirk_1E_output;

typedef struct kirk_1F_input
{
   cmd56_packet6_out packet6; // 0x20
   cmd56_packet9_in packet9; // 0x30
   cmd56_packet15_in_aux packet15; // 0x20
   cmd56_packet16_out_ex packet16; // 0x43
} kirk_1F_input;

typedef struct kirk_1F_output
{
   cmd56_packet16_out_aux_part packet16_aux_part; 
}kirk_1F_output;

typedef struct kirk_20_input
{
   cmd56_packet6_out packet6; //0x20
   cmd56_packet9_in packet9; // 0x30
   cmd56_packet17_in_aux packet17; // 0x20
   cmd56_packet18_out_ex packet18; // 0x43
   cmd56_packet19_in packet19; // 0x10
   cmd56_packet20_out_ex packet20; // 0x53
} kirk_20_input;

typedef struct kirk_20_output
{
   cmd56_packet18_20_out_aux_part packet18_20_aux_part; // 0x34
}kirk_20_output;

#pragma pack(pop)

//========================================

HAL_StatusTypeDef KirkSendCommand_1B(uint16_t key_id, 
                                     cmd56_packet6_out* packet6_data,
                                     cmd56_packet7_in* packet7_data,
                                     cmd56_packet8_out_ex* packet8_data);

HAL_StatusTypeDef KirkSendCommand_1C(uint16_t key_id, 
                                     cmd56_packet6_out* packet6_data,
                                     cmd56_packet8_out* packet8_data,
                                     cmd56_packet9_in_ex* packet9_data);

HAL_StatusTypeDef KirkSendCommand_1D(uint16_t key_id,
                                     cmd56_packet6_out* packet6_data,
                                     cmd56_packet9_in* packet9_data, 
                                     cmd56_packet13_in* packet13_data,
                                     cmd56_packet14_out_ex* packet14_data);

HAL_StatusTypeDef KirkSendCommand_1E(uint16_t key_id, uint8_t tweak_seed,
                                     cmd56_packet6_out* packet6_data,
                                     cmd56_packet9_in* packet9_data,
                                     cmd56_packet15_17_in_ex* packet1x_data);
                                     
HAL_StatusTypeDef KirkSendCommand_1F(uint16_t key_id,
                                     cmd56_packet6_out* packet6_data,
                                     cmd56_packet9_in* packet9_data, 
                                     cmd56_packet15_in_aux* packet15_data,
                                     cmd56_packet16_out_ex* packet16_data,
                                     cmd56_packet16_out_aux_part* packet16_data_aux_part);

HAL_StatusTypeDef KirkSendCommand_20(uint16_t key_id,
                                     cmd56_packet6_out* packet6_data,
                                     cmd56_packet9_in* packet9_data, 
                                     cmd56_packet17_in_aux* packet17_data,
                                     cmd56_packet18_out_ex* packet18_data,
                                     cmd56_packet19_in* packet19_data,
                                     cmd56_packet20_out_ex* packet20_data,
                                     cmd56_packet18_20_out_aux_part* packet18_20_aux_part);
