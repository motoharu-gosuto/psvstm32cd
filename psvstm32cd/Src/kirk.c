#include "kirk.h"

#include <stdint.h>
#include <string.h>

#include "bigmac.h"

//========================================

int service_handler_0x1000B_command_1B(uint16_t key_id, kirk_1B_input* input_data)
{
   uint8_t drv_key[0x10];
   int mode;

   int res0 = initialize_keyslot_0x21_0x24_with_cmac(input_data->packet6.master_key, key_id, drv_key, &mode);
   if(res0 != 0)
      return res0;

   cmd56_packet8_out dec_input;
   memcpy(&dec_input, &input_data->packet8.p8, sizeof(cmd56_packet8_out));
   
   int res1 = bigmac_aes_128_cbc_decrypt_with_mode_select((uint8_t*)&dec_input, sizeof(cmd56_packet8_out), drv_key, mode);
   
   int res2 = memcmp(input_data->packet7.challenge0 + 1, dec_input.challenge0 + 1, 0xF);
   if(res2 != 0)
      return 5;

   return 0;
}

HAL_StatusTypeDef KirkSendCommand_1B(uint16_t key_id, 
                                     cmd56_packet6_out* packet6_data,
                                     cmd56_packet7_in* packet7_data,
                                     cmd56_packet8_out_ex* packet8_data)
{
   kirk_1B_input input;
   memcpy(&input.packet6, packet6_data, sizeof(cmd56_packet6_out));
   memcpy(&input.packet7, packet7_data, sizeof(cmd56_packet7_in));
   memcpy(&input.packet8, packet8_data, sizeof(cmd56_packet8_out_ex));

   int f00d_resp = service_handler_0x1000B_command_1B(key_id, &input);
   if(f00d_resp != 0)
      return HAL_ERROR;
   
   return HAL_OK;
}

int service_handler_0x1000B_command_1C(uint16_t key_id, kirk_1C_input* input_data, kirk_1C_output* output_data)
{
   uint8_t drv_key[0x10];
   int mode;
   
   int res0 = initialize_keyslot_0x21_0x24_with_cmac(input_data->packet6.master_key, key_id, drv_key, &mode);
   if(res0 != 0)
      return res0;

   cmd56_packet8_out dec_input;
   memcpy(&dec_input, &input_data->packet8, sizeof(cmd56_packet8_out));
   
   int res1 = bigmac_aes_128_cbc_decrypt_with_mode_select((uint8_t*)&dec_input, sizeof(cmd56_packet8_out), drv_key, mode);
   
   cmd56_packet9_in enc_output;
                        
   memcpy(enc_output.card_challenge0, dec_input.card_challenge0, 0x10);
   memcpy(enc_output.challenge0, dec_input.challenge0, 0x10);

   enc_output.card_challenge0[0] |= 0x80;
   enc_output.challenge0[0] |= 0x80;

   uint8_t secondary_key0[0x20];
   
   int res2 = bigmac_generate_random_number0(secondary_key0, 0x20);
   if(res2 != 0)
      return 5;

   memcpy(enc_output.secondary_key0, secondary_key0, 0x10);
   
   int res3 = bigmac_aes_128_cbc_encrypt_with_mode_select((uint8_t*)&enc_output, sizeof(cmd56_packet9_in), drv_key, mode);
   
   output_data->packet9.code = 0xA3;
   output_data->packet9.unk = 0;
   output_data->packet9.additionalDataSize1 = 0x33;
   
   memcpy(&output_data->packet9.p9, &enc_output, sizeof(cmd56_packet9_in));

   return 0;
}

HAL_StatusTypeDef KirkSendCommand_1C(uint16_t key_id, 
                                     cmd56_packet6_out* packet6_data,
                                     cmd56_packet8_out* packet8_data,
                                     cmd56_packet9_in_ex* packet9_data)
{
   kirk_1C_input input;
   memcpy(&input.packet6, packet6_data, sizeof(cmd56_packet6_out));
   memcpy(&input.packet8, packet8_data, sizeof(cmd56_packet8_out));

   kirk_1C_output output;
   memset(&output, 0, sizeof(kirk_1C_output));

   int f00d_resp = service_handler_0x1000B_command_1C(key_id, &input, &output);
   if(f00d_resp != 0)
      return HAL_ERROR;
   
   memcpy(packet9_data, &output.packet9, sizeof(cmd56_packet9_in_ex));
   
   return HAL_OK;
}

int service_handler_0x1000B_command_1D(uint16_t key_id, kirk_1D_input* input_data)
{  
   uint8_t drv_key[0x10];

   int res0 = initialize_keyslot_0x21_0x24_with_cmac_and_dec_key(input_data->packet6.master_key, key_id, input_data->packet9.secondary_key0, drv_key);
   if(res0 != 0)
      return res0;

   cmd56_packet14_out dec_input;
   memcpy(&dec_input, &input_data->packet14.p14, sizeof(cmd56_packet14_out));

   int res1 = bigmac_aes_128_cbc_decrypt_with_mode_select((uint8_t*)&dec_input, sizeof(cmd56_packet14_out), drv_key, 1); //dec with key mode

   int res2 = memcmp(input_data->packet13.challenge1 + 1, dec_input.challenge1 + 1, 0xF);
   if(res2 != 0)
      return 5;

   int res3 = memcmp(input_data->packet6.master_key, dec_input.master_key, 0x20);
   if(res3 != 0)
      return 5;
   
   return 0;
}

HAL_StatusTypeDef KirkSendCommand_1D(uint16_t key_id,
                                     cmd56_packet6_out* packet6_data,
                                     cmd56_packet9_in* packet9_data, 
                                     cmd56_packet13_in* packet13_data,
                                     cmd56_packet14_out_ex* packet14_data)
{
   kirk_1D_input input;
   memcpy(&input.packet6, packet6_data, sizeof(cmd56_packet6_out));
   memcpy(&input.packet9, packet9_data, sizeof(cmd56_packet9_in));
   memcpy(&input.packet13, packet13_data, sizeof(cmd56_packet13_in));
   memcpy(&input.packet14, packet14_data, sizeof(cmd56_packet14_out_ex));

   int f00d_resp = service_handler_0x1000B_command_1D(key_id, &input);
   if(f00d_resp != 0)
      return HAL_ERROR;
   
   return HAL_OK;
}

int service_handler_0x1000B_command_1E(uint16_t key_id, kirk_1E_input* input_data, kirk_1E_output* output_data)
{
   uint8_t drv_key[0x10];
   
   int res0 = initialize_keyslot_0x21_0x24_with_cmac_and_dec_key(input_data->packet6.master_key, key_id, input_data->packet9.secondary_key0, drv_key);
   if(res0 != 0)
      return res0;
   
   uint8_t secondary_key1[0x20];

   int res1 = bigmac_generate_random_number1(secondary_key1, 0x20);
   if(res1 != 0)
      return 5;
   
   cmd56_packet15_17_in_aux enc_output;
   
   memcpy(enc_output.secondary_key1, secondary_key1, 0x10);
   enc_output.secondary_key1[0] |= 0x80;
   
   memset(enc_output.tweak_padding, 0, 0x10);
   enc_output.tweak_padding[0xF] = input_data->tweak_seed;
   
   int res2 = bigmac_aes_128_cbc_encrypt_with_mode_select((uint8_t*)&enc_output, sizeof(cmd56_packet15_17_in_aux), drv_key, 1); // enc with key mode
   
   cmd56_packet15_17_in_cmac cmac_input;
   cmac_input.code = 0xB1;
   cmac_input.unk = 0;
   cmac_input.additionalDataSize1 = 0x33;
   
   memset(cmac_input.padding, 0, 0xD);
   memcpy(&cmac_input.p1x, &enc_output, sizeof(cmd56_packet15_17_in_aux));
   
   uint8_t cmac_output[0x10];
   
   int res3 = bigmac_cmac_aes_128_with_key((uint8_t*)&cmac_input, 0x30, drv_key, cmac_output);
   
   output_data->packet1x.code = 0xB1;
   output_data->packet1x.unk = 0;
   output_data->packet1x.additionalDataSize1 = 0x33;
   
   memcpy(&output_data->packet1x.p1x.p1x, &enc_output, sizeof(cmd56_packet15_17_in_aux));
   memcpy(output_data->packet1x.p1x.p1x_cmac, cmac_output, 0x10);
   
   return 0;
}

HAL_StatusTypeDef KirkSendCommand_1E(uint16_t key_id, uint8_t tweak_seed,
                                     cmd56_packet6_out* packet6_data,
                                     cmd56_packet9_in* packet9_data,
                                     cmd56_packet15_17_in_ex* packet1x_data)
{
   kirk_1E_input input;
   memcpy(&input.packet6, packet6_data, sizeof(cmd56_packet6_out));
   memcpy(&input.packet9, packet9_data, sizeof(cmd56_packet9_in));
   input.tweak_seed = tweak_seed;

   kirk_1E_output output;
   memset(&output, 0, sizeof(kirk_1E_output));

   int f00d_resp = service_handler_0x1000B_command_1E(key_id, &input, &output);
   if(f00d_resp != 0)
      return HAL_ERROR;
   
   memcpy(packet1x_data, &output.packet1x, sizeof(cmd56_packet15_17_in_ex));
   
   return HAL_OK;
}

int service_handler_0x1000B_command_1F(uint16_t key_id, kirk_1F_input* input_data, kirk_1F_output* output_data)
{
   uint8_t drv_key[0x10];

   int res0 = initialize_keyslot_0x21_0x24_with_cmac_and_dec_key(input_data->packet6.master_key, key_id, input_data->packet9.secondary_key0, drv_key);
   if(res0 != 0)
      return res0;

   cmd56_packet16_out_cmac cmac_input;
   cmac_input.size = input_data->packet16.size;
   cmac_input.errorCode = input_data->packet16.errorCode;
   
   memset(cmac_input.padding, 0, 0xD);
   memcpy(&cmac_input.p16, &input_data->packet16.p16.p16, sizeof(cmd56_packet16_out_aux));

   uint8_t cmac_output[0x10];

   int res1 = bigmac_cmac_aes_128_with_key((uint8_t*)&cmac_input, sizeof(cmd56_packet16_out_cmac), drv_key, cmac_output);

   int res2 = memcmp(input_data->packet16.p16.p16_cmac, cmac_output, 0x10);
   if(res2 != 0)
      return 5;

   cmd56_packet15_in_aux dec_input1;
   memcpy(&dec_input1, &input_data->packet15, sizeof(cmd56_packet15_in_aux));

   int res3 = bigmac_aes_128_cbc_decrypt_with_mode_select((uint8_t*)&dec_input1, sizeof(cmd56_packet15_in_aux), drv_key, 1);  //dec with key mode

   cmd56_packet16_out_aux dec_input2;
   memcpy(&dec_input2, &input_data->packet16.p16.p16, sizeof(cmd56_packet16_out_aux));

   int res4 = bigmac_aes_128_cbc_decrypt_with_mode_select((uint8_t*)&dec_input2, sizeof(cmd56_packet16_out_aux), drv_key, 1);  //dec with key mode

   int res5 = memcmp(dec_input1.secondary_key1 + 1, dec_input2.secondary_key1_p15 + 1, 0xF);
   if(res5 != 0)
      return 5;

   if(dec_input1.tweak_padding[0xF] == 3)
      return 0x12;

   memcpy(output_data, &dec_input2.p16p, sizeof(cmd56_packet16_out_aux_part));
   
   return 0;
}

HAL_StatusTypeDef KirkSendCommand_1F(uint16_t key_id,
                                     cmd56_packet6_out* packet6_data,
                                     cmd56_packet9_in* packet9_data, 
                                     cmd56_packet15_in_aux* packet15_data,
                                     cmd56_packet16_out_ex* packet16_data,
                                     cmd56_packet16_out_aux_part* packet16_data_aux_part)
{
   kirk_1F_input input;
   memcpy(&input.packet6, packet6_data, sizeof(cmd56_packet6_out));
   memcpy(&input.packet9, packet9_data, sizeof(cmd56_packet9_in));
   memcpy(&input.packet15, packet15_data, sizeof(cmd56_packet15_in_aux));
   memcpy(&input.packet16, packet16_data, sizeof(cmd56_packet16_out_ex));
   
   kirk_1F_output output;
   memset(&output, 0, sizeof(kirk_1F_output));

   int f00d_resp = service_handler_0x1000B_command_1F(key_id, &input, &output);
   if(f00d_resp != 0)
      return HAL_ERROR;
   
   memcpy(packet16_data_aux_part, &output.packet16_aux_part, sizeof(cmd56_packet16_out_aux_part));
   
   return HAL_OK;
}

int service_handler_0x1000B_command_20(uint16_t key_id, kirk_20_input* input_data, kirk_20_output* output_data)
{
   uint8_t drv_key[0x10];
   
   int res0 = initialize_keyslot_0x21_0x24_with_cmac_and_dec_key(input_data->packet6.master_key, key_id, input_data->packet9.secondary_key0, drv_key);
   if(res0 != 0)
      return res0;
   
   cmd56_packet18_out_cmac cmac_input1;
   cmac_input1.size = input_data->packet18.size;
   cmac_input1.errorCode = input_data->packet18.errorCode;
   
   memset(cmac_input1.padding, 0, 0xD);
   memcpy(&cmac_input1.p18, &input_data->packet18.p18.p18, sizeof(cmd56_packet18_out_aux));
   
   uint8_t cmac_output1[0x10];
   
   int res1 = bigmac_cmac_aes_128_with_key((uint8_t*)&cmac_input1, sizeof(cmd56_packet18_out_cmac), drv_key, cmac_output1);
   
   int res2 = memcmp(input_data->packet18.p18.p18_cmac, cmac_output1, 0x10);
   if(res2 != 0)
      return 5;

   cmd56_packet17_in_aux dec_input1;
   memcpy(&dec_input1, &input_data->packet17, sizeof(cmd56_packet17_in_aux));

   int res3 = bigmac_aes_128_cbc_decrypt_with_mode_select((uint8_t*)&dec_input1, sizeof(cmd56_packet17_in_aux), drv_key, 1);
   
   cmd56_packet18_out_aux dec_input2;
   memcpy(&dec_input2, &input_data->packet18.p18.p18, sizeof(cmd56_packet18_out_aux));
   
   int res4 = bigmac_aes_128_cbc_decrypt_with_mode_select((uint8_t*)&dec_input2, sizeof(cmd56_packet18_out_aux), drv_key, 1);
   
   int res5 = memcmp(dec_input1.secondary_key1 + 1, dec_input2.secondary_key1_p17 + 1, 0xF);
   if(res5 != 0)
      return 5;
   
   if(dec_input1.tweak_padding[0xF] != 3)
      return 0x12;
   
   cmd56_packet20_out_cmac cmac_input2;
   cmac_input2.size = input_data->packet20.size;
   cmac_input2.errorCode = input_data->packet20.errorCode;
  
   memset(cmac_input2.padding, 0, 0xD);
   memcpy(&cmac_input2.p20, &input_data->packet20.p20.p20, sizeof(cmd56_packet20_out_aux));

   uint8_t cmac_output2[0x10];

   int res6 = bigmac_cmac_aes_128_with_key((uint8_t*)&cmac_input2, sizeof(cmd56_packet20_out_cmac), drv_key, cmac_output2);

   int res7 = memcmp(input_data->packet20.p20.p20_cmac, cmac_output2, 0x10);
   if(res7 != 0)
      return 5;

   cmd56_packet20_out_aux dec_input3;
   memcpy(&dec_input3, &input_data->packet20.p20.p20, sizeof(cmd56_packet20_out_aux)); 

   int res8 = bigmac_aes_128_cbc_decrypt_with_mode_select((uint8_t*)&dec_input3, 0x40, drv_key, 1);

   int res9 = memcmp(input_data->packet19.challenge2 + 1, dec_input3.challenge2 + 1, 0xF);
   if(res9 != 0)
      return 5;

   klicensee_key_material sha_input;
   memcpy(sha_input.klicensee_key_material_p20, dec_input3.klicensee_key_material_p20, 0x20);
   memcpy(sha_input.klicensee_key_material_p18, dec_input2.p18p.klicensee_key_material_p18, 0x20);
   
   int res10 = bigmac_sha256((uint8_t*)&sha_input, sizeof(klicensee_key_material), output_data->packet18_20_aux_part.klicensee_keys);
   
   int res11 = bigmac_sha1(dec_input3.klicensee_key_material_p20, 0x20, output_data->packet18_20_aux_part.rif_digest);
   
   return 0;
}

HAL_StatusTypeDef KirkSendCommand_20(uint16_t key_id,
                                     cmd56_packet6_out* packet6_data,
                                     cmd56_packet9_in* packet9_data, 
                                     cmd56_packet17_in_aux* packet17_data,
                                     cmd56_packet18_out_ex* packet18_data,
                                     cmd56_packet19_in* packet19_data,
                                     cmd56_packet20_out_ex* packet20_data,
                                     cmd56_packet18_20_out_aux_part* packet18_20_aux_part)
{
   kirk_20_input input;
   memcpy(&input.packet6, packet6_data, sizeof(cmd56_packet6_out));
   memcpy(&input.packet9, packet9_data, sizeof(cmd56_packet9_in));
   memcpy(&input.packet17, packet17_data, sizeof(cmd56_packet17_in_aux));
   memcpy(&input.packet18, packet18_data, sizeof(cmd56_packet18_out_ex));
   memcpy(&input.packet19, packet19_data, sizeof(cmd56_packet19_in));
   memcpy(&input.packet20, packet20_data, sizeof(cmd56_packet20_out_ex));
   
   kirk_20_output output;
   memset(&output, 0, sizeof(kirk_20_output));
   
   int f00d_resp = service_handler_0x1000B_command_20(key_id, &input, &output);
   if(f00d_resp != 0)
      return HAL_ERROR;
   
   memcpy(packet18_20_aux_part, &output.packet18_20_aux_part, sizeof(cmd56_packet18_20_out_aux_part));
   
   return HAL_OK;
}
