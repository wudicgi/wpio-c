#ifndef __XXTEA_H__
#define __XXTEA_H__

#define XXTEA_MX ((((z >> 5) ^ (y << 2)) + ((y >> 3) ^ (z << 4))) \
                  ^ ((sum ^ y) + (k[(p & 3) ^ e] ^ z)))

#define XXTEA_DELTA 0x9E3779B9

unsigned char *xxtea_encrypt(unsigned char *out, unsigned char *in, uint32_t len, unsigned char *key, uint32_t *ret_len);
unsigned char *xxtea_decrypt(unsigned char *out, unsigned char *in, uint32_t len, unsigned char *key, uint32_t *ret_len);

#endif // __XXTEA_H__
