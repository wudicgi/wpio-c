#ifndef __WPES_H__
#define __WPES_H__

#include <stdint.h>

#define WPES_DATA_SIZE 4096
#define WPES_HEAD_SIZE 32
#define WPES_BLOCK_SIZE (WPES_DATA_SIZE + WPES_HEAD_SIZE)

#ifndef MIN
# define MIN(a,b) ((a)<(b)?(a):(b))
#endif

// 块标识常量
enum enum_wpes_block {
    WPES_BLOCK_SIGNATURE = 0x53455057 // WPES
};

// 块标记常量
enum enum_wpes_block_flag {
    WPES_BLOCK_FLAG_NONE = 0x00000000,
    WPES_BLOCK_FLAG_DATA = 0x00000001,
    WPES_BLOCK_FLAG_XXTEA = 0x00000100,
    WPES_BLOCK_FLAG_AES128 = 0x00000200,
    WPES_BLOCK_FLAG_AES256 = 0x00000400
};

#include <pshpack1.h>

typedef struct wpes_head_struct {
    uint32_t    signature;
    uint32_t    flags;
    uint16_t    length;
    uint16_t    datasize;
    uint32_t    crc32;
    uint8_t     reserved[16];
} WPES_HEAD;

#include <poppack.h>

typedef struct _wpes_stream_data {
    WpioStream *base_stream;
    long        offset; // 当前文件指针读写的位置
    size_t      length; // 当前文件的长度
    unsigned char   buffer[WPES_DATA_SIZE]; // 当前块的有效数据
    int         buffer_length; // 当前块的有效数据长度
    long        buffer_offset; // 当前块的位置
} WPES_STREAM_DATA;

WpioStream *wpes_open(const WpioStream *base_stream, const WpioMode mode);

#endif // __WPES_H__
