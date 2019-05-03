#include "bigmac.h"

#include <stdint.h>
#include <string.h>

#include <mbedtls/aes.h>
#include <mbedtls/cmac.h>
#include <mbedtls/sha1.h>
#include <mbedtls/sha256.h>

//=======================================
// ATTENTION: some of the crypto functions are hardcoded for specific card
//            this is done because some F00D master keys are not public
//=======================================

uint8_t key_8124C0[0x10] = {0x7F, 0x1F, 0xD0, 0x65, 0xDD, 0x2F, 0x40, 0xB3, 0xE2, 0x65, 0x79, 0xA6, 0x39, 0x0B, 0x61, 0x6D, };
uint8_t iv_8124D0[0x10] = {0x8B, 0x14, 0xC8, 0xA1, 0xE9, 0x6F, 0x30, 0xA7, 0xF1, 0x01, 0xA9, 0x6A, 0x30, 0x33, 0xC5, 0x5B, };
   
uint8_t keyslot_0x21[0x10] = {0};
uint8_t keyslot_0x24[0x10] = {0};

//=======================================

//this function is an emulation for single case for now, because master keys 0x345 and 0x348 are unknown
int bigmac_aes_256_ecb_decrypt_set_keyslot_from_keyslot(const uint8_t* src, int keyslot_dst, int keyslot_src)
{
   if(keyslot_dst != 0x21)
      return -1;
   
   if(keyslot_src != 0x345)
      return -1;
   
   // verify that we are processing expected F00D key
   
   uint8_t src_base[0x10] = {0x7F, 0x1F, 0xD0, 0x65, 0xDD, 0x2F, 0x40, 0xB3, 0xE2, 0x65, 0x79, 0xA6, 0x39, 0x0B, 0x61, 0x6D, };
      
   if(memcmp(src_base, src, 0x10) != 0)
      return -1;
   
   // initialize keyslot with hardcoded value
   
   uint8_t keyslot_0x21_base[0x10] = {0x72, 0x50, 0x6A, 0x4B, 0xA8, 0x36, 0xC8, 0x76, 0xC4, 0x48, 0x40, 0x70, 0x1F, 0x0E, 0xA1, 0x02};
   
   memcpy(keyslot_0x21, keyslot_0x21_base, 0x10);
   
   return 0;
}

//this function is an emulation for single case for now, because master keys 0x345 and 0x348 are unknown
int bigmac_cmac_aes_128_with_keyslot(uint8_t* dst, const uint8_t* src, int size, int keyslot)
{
   if(keyslot != 0x21)
      return -1;
   
   // master key of the card (from packet 6)
   uint8_t src_base[0x20] = { 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0xFE, 0x95, 0xF2, 
                              0xC2, 0x73, 0xE9, 0x45, 0x42, 0x08, 0x00, 0x5F, 0xE8, 0x56, 0x07, 0x99, 0xDD, 0xF9, 0x1F, 0x9F};
   
   // verify that we are processing expected master key
   if(memcmp(src_base, src, 0x20) != 0)
      return -1;
   
   // initialize dst with hardcoded value
   
   uint8_t dst_base[0x10] = {0x3D, 0xCA, 0x1D, 0xB5, 0xA9, 0x1F, 0x3E, 0x76, 0x58, 0x6D, 0x4F, 0xF8, 0x8C, 0xD2, 0x60, 0xE8};
   
   memcpy(dst, dst_base, 0x10);
   
   return 0;
}

//this function is an emulation for single case for now, because master keys 0x345 and 0x348 are unknown
int bigmac_aes_128_cbc_decrypt_set_keyslot_from_keyslot(const uint8_t* src, const uint8_t* iv, int keyslot_dst, int keyslot_src)
{
   if(keyslot_dst != 0x24)
      return -1;
   
   if(keyslot_src != 0x348)
      return -1;
   
   // verify that we are processing expected iv
   
   uint8_t iv_base[0x10] = {0x8B, 0x14, 0xC8, 0xA1, 0xE9, 0x6F, 0x30, 0xA7, 0xF1, 0x01, 0xA9, 0x6A, 0x30, 0x33, 0xC5, 0x5B, };
      
   if(memcmp(iv_base, iv, 0x10) != 0)
      return -1;
   
   //verify that we are processing expected source
   
   uint8_t src_base[0x10] = {0x3D, 0xCA, 0x1D, 0xB5, 0xA9, 0x1F, 0x3E, 0x76, 0x58, 0x6D, 0x4F, 0xF8, 0x8C, 0xD2, 0x60, 0xE8};
   
   if(memcmp(src_base, src, 0x10) != 0)
      return -1;
   
   // initialize keyslot with hardcoded value
   
   uint8_t keyslot_0x24_base[0x10] = {0x36, 0x90, 0x71, 0xd7, 0xd5, 0xa0, 0xf3, 0xa3, 0xbe, 0xe5, 0x71, 0xd4, 0x15, 0x75, 0x7f, 0x93};
   
   memcpy(keyslot_0x24, keyslot_0x24_base, 0x10);
   
   return 0;
}

//this function is an emulation for single case for now, because master keys 0x345 and 0x348 are unknown
int initialize_keyslot_0x21_0x24_with_cmac(const uint8_t* cmac_input, int key_id, uint8_t* cmac_output, int* mode)
{
   if(key_id != 1)
      return 0xE;
   
   int res0 = bigmac_aes_256_ecb_decrypt_set_keyslot_from_keyslot(key_8124C0, 0x21, 0x345); //initialize key slot 0x21 by decrypting key_8124C0 with key slot 0x345
   if(res0 != 0)
      return res0;

   int res1 = bigmac_cmac_aes_128_with_keyslot(cmac_output, cmac_input, 0x20, 0x21); // calculate cmac of the cmac_input with key slot 0x21
   if(res1 != 0)
      return res1;

   int res2 = bigmac_aes_128_cbc_decrypt_set_keyslot_from_keyslot(cmac_output, iv_8124D0, 0x24, 0x348); // decrypt cmac with iv_8124D0 and key slot 0x348 and set key slot 0x24
   if(res2 != 0)
      return res2;

   *mode = 2; //with keyslot 0x24
   
   return 0;
}

//=======================================

int bigmac_aes_128_cbc_decrypt_with_mode_select(uint8_t* src_dst, int size, uint8_t* key, int enc_mode)
{
   if(enc_mode == 2)
   {
      mbedtls_aes_context aes_ctx;
      mbedtls_aes_init(&aes_ctx);

      mbedtls_aes_setkey_dec(&aes_ctx, keyslot_0x24, 128);

      unsigned char dec_iv[0x10] = {0};

      mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_DECRYPT, size, dec_iv, src_dst, src_dst);

      mbedtls_aes_free(&aes_ctx);
      
      return 0;
   }
   else if(enc_mode == 1)
   {
      mbedtls_aes_context aes_ctx;
      mbedtls_aes_init(&aes_ctx);

      mbedtls_aes_setkey_dec(&aes_ctx, key, 128);

      unsigned char dec_iv[0x10] = {0};

      mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_DECRYPT, size, dec_iv, src_dst, src_dst);

      mbedtls_aes_free(&aes_ctx);
      
      return 0;      
   }
   else
   {
      return -1;
   }
}

int bigmac_aes_128_cbc_encrypt_with_mode_select(uint8_t* src_dst, int size, uint8_t* key, int enc_mode)
{
   if(enc_mode == 2)
   {
      mbedtls_aes_context aes_ctx;
      mbedtls_aes_init(&aes_ctx);
      
      mbedtls_aes_setkey_enc(&aes_ctx, keyslot_0x24, 128);

      unsigned char enc_iv[0x10] = {0};

      mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_ENCRYPT, size, enc_iv, src_dst, src_dst);

      mbedtls_aes_free(&aes_ctx);
      
      return 0;
   }
   else if(enc_mode == 1)
   {
      mbedtls_aes_context aes_ctx;
      mbedtls_aes_init(&aes_ctx);
      
      mbedtls_aes_setkey_enc(&aes_ctx, key, 128);

      unsigned char enc_iv[0x10] = {0};

      mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_ENCRYPT, size, enc_iv, src_dst, src_dst);

      mbedtls_aes_free(&aes_ctx);
      
      return 0;
   }
   else
   {
      return -1;
   }
}

int initialize_keyslot_0x21_0x24_with_cmac_and_dec_key(uint8_t* master_key, uint16_t key_id, uint8_t* src, uint8_t* dst)
{
   uint8_t drv_key[0x10];
   int mode;

   int res0 = initialize_keyslot_0x21_0x24_with_cmac(master_key, key_id, drv_key, &mode);
   if(res0 != 0)
      return res0;

   memcpy(dst, src, 0x10);
   
   int res1 = bigmac_aes_128_cbc_decrypt_with_mode_select(dst, 0x10, drv_key, mode);
                           
   return res1;
}

//this function is just an emulation - returns constant value
int bigmac_generate_random_number0(uint8_t* dst, uint32_t size)
{
   if(size >= 0xFF)
      return -1;
   
   for(uint32_t i = 0; i < size; i++)
   {
      dst[i] = i + 0x10;
   }
   
   return 0;
}

//this function is just an emulation - returns constant value
int bigmac_generate_random_number1(uint8_t* dst, uint32_t size)
{
   if(size >= 0xFF)
      return -1;
   
   for(uint32_t i = 0; i < size; i++)
   {
      dst[i] = i + 0x30;
   }
   
   return 0;
}

int bigmac_cmac_aes_128_with_key(const uint8_t* src, int size, uint8_t* key, uint8_t* dst)
{
   const mbedtls_cipher_info_t* cipher_info = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB);
   
   int res0 = mbedtls_cipher_cmac(cipher_info, key, 128, src, size, dst);
   if(res0 != 0)
      return -1;
   
   return 0;
}

int bigmac_sha256(const uint8_t* src, int size, uint8_t* dst)
{
   mbedtls_sha256(src, size, dst, 0);
   
   return 0;
}

int bigmac_sha1(const uint8_t* src, int size, uint8_t* dst)
{
   mbedtls_sha1(src, size, dst);
   
   return 0;
}
