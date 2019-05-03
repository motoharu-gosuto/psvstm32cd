#include <stdint.h>

//this function is an emulation for single case for now, because master keys 0x345 and 0x348 are unknown
int bigmac_aes_256_ecb_decrypt_set_keyslot_from_keyslot(const uint8_t* src, int keyslot_dst, int keyslot_src);

//this function is an emulation for single case for now, because master keys 0x345 and 0x348 are unknown
int bigmac_cmac_aes_128_with_keyslot(uint8_t* dst, const uint8_t* src, int size, int keyslot);   

//this function is an emulation for single case for now, because master keys 0x345 and 0x348 are unknown
int bigmac_aes_128_cbc_decrypt_set_keyslot_from_keyslot(const uint8_t* src, const uint8_t* iv, int keyslot_dst, int keyslot_src);

//this function is an emulation for single case for now, because master keys 0x345 and 0x348 are unknown
int initialize_keyslot_0x21_0x24_with_cmac(const uint8_t* cmac_input, int key_id, uint8_t* cmac_output, int* mode);

//=======================================

int bigmac_aes_128_cbc_decrypt_with_mode_select(uint8_t* src_dst, int size, uint8_t* key, int enc_mode);
   
int bigmac_aes_128_cbc_encrypt_with_mode_select(uint8_t* src_dst, int size, uint8_t* key, int enc_mode);

int initialize_keyslot_0x21_0x24_with_cmac_and_dec_key(uint8_t* master_key, uint16_t key_id, uint8_t* src, uint8_t* dst);

int bigmac_generate_random_number0(uint8_t* dst, uint32_t size);

int bigmac_generate_random_number1(uint8_t* dst, uint32_t size);

int bigmac_cmac_aes_128_with_key(const uint8_t* src, int size, uint8_t* key, uint8_t* dst);

int bigmac_sha256(const uint8_t* src, int size, uint8_t* dst);

int bigmac_sha1(const uint8_t* src, int size, uint8_t* dst);
