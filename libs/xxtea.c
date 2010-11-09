#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "xxtea.h"

static void xxtea_encrypt_blocks(uint32_t *v, uint32_t len, uint32_t *k) {
    uint32_t n = len - 1;
    uint32_t z = v[n], y = v[0], p, q = 6 + 52 / (n + 1), sum = 0, e;
    /*
    if (n < 1) {
        return;
    }
    */
    while (0 < q--) {
        sum += XXTEA_DELTA;
        e = sum >> 2 & 3;
        for (p = 0; p < n; p++) {
            y = v[p + 1];
            z = v[p] += XXTEA_MX;
        }
        y = v[0];
        z = v[n] += XXTEA_MX;
    }
}

static void xxtea_decrypt_blocks(uint32_t *v, uint32_t len, uint32_t *k) {
    uint32_t n = len - 1;
    uint32_t z = v[n], y = v[0], p, q = 6 + 52 / (n + 1), sum = q * XXTEA_DELTA, e;
    /*
    if (n < 1) {
        return;
    }
    */
    while (sum != 0) {
        e = sum >> 2 & 3;
        for (p = n; p > 0; p--) {
            z = v[p - 1];
            y = v[p] -= XXTEA_MX;
        }
        z = v[n];
        y = v[0] -= XXTEA_MX;
        sum -= XXTEA_DELTA;
    }
}

static uint32_t *xxtea_bytes_to_blocks(const unsigned char *src, uint32_t len, uint32_t *ret_len) {
    uint32_t i, n, *result;

    // XXTEA requires the block size is at least two words, i.e. 4 bytes. If the length of
    // data is not multiples of 4, we need to fill up the last block to 4 bytes.
    if ((len & 3) == 0) {
        n = len >> 2; // len / sizeof(uint32_t)
    } else {
        n = (len >> 2) + 1; // one more unit32 for the 1 to 3 bytes at the end
    }

    *ret_len = n;

    // Allocates memory spaces for the uint32 array
    result = (uint32_t *)malloc(n << 2); // n * sizeof(uint32_t)

    // Fills up the array with '\0'
    memset(result, 0, n << 2);

    // Converts bytes one by one
    for (i = 0; i < len; i++) {
        result[i >> 2] |= (uint32_t)src[i] << ((i & 3) << 3);
    }

    return result;
}

static void *xxtea_blocks_to_bytes(unsigned char *dest, const uint32_t *src, uint32_t len, uint32_t *ret_len) {
    uint32_t i, n;

    n = len << 2; // len * sizeof(uint32_t)
    *ret_len = n;

    // Converts uint32s one by one
    for (i = 0; i < n; i++) {
        dest[i] = (unsigned char)((src[i >> 2] >> ((i & 3) << 3)) & 0xff);
    }

    return dest;
}

unsigned char *xxtea_encrypt(unsigned char *out, unsigned char *in, uint32_t len, unsigned char *key, uint32_t *ret_len) {
    uint32_t *v, *k, v_len, k_len;

    v = xxtea_bytes_to_blocks(in, len, &v_len);
    k = xxtea_bytes_to_blocks(key, 16, &k_len);

    xxtea_encrypt_blocks(v, v_len, k);

    xxtea_blocks_to_bytes(out, v, v_len, ret_len);

    free(v);
    free(k);

    return out;
}

unsigned char *xxtea_decrypt(unsigned char *out, unsigned char *in, uint32_t len, unsigned char *key, uint32_t *ret_len) {
    uint32_t *v, *k, v_len, k_len;

    v = xxtea_bytes_to_blocks(in, len, &v_len);
    k = xxtea_bytes_to_blocks(key, 16, &k_len);

    xxtea_decrypt_blocks(v, v_len, k);

    xxtea_blocks_to_bytes(out, v, v_len, ret_len);

    free(v);
    free(k);

    return out;
}
