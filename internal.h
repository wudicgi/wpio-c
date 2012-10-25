#ifndef _INTERNAL_H_
#define _INTERNAL_H_

#ifndef _FILE_OFFSET_BITS
# define _FILE_OFFSET_BITS 64
#endif

#ifndef _LARGEFILE_SOURCE
# define _LARGEFILE_SOURCE 1
#endif

#ifndef _LARGEFILE64_SOURCE
# define _LARGEFILE64_SOURCE 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct _WPIO_StreamOps WPIO_StreamOps;

#include "wpio.h"

#include "file.h"
#include "wpes.h"

struct _WPIO_StreamOps {
    int     (*close)(WPIO_Stream *stream);                                      // 关闭流
    int     (*flush)(WPIO_Stream *stream);                                      // 将缓冲内容输出到流
    int     (*seek)(WPIO_Stream *stream, off64_t offset, int whence);           // 在流中定位
    off64_t (*tell)(WPIO_Stream *stream);                                       // 获取指针在流中的位置
    int     (*eof)(WPIO_Stream *stream);                                        // 测试指针是否到了流结束的位置
    size_t  (*read)(WPIO_Stream *stream, void *buffer, size_t length);          // 读取流
    size_t  (*write)(WPIO_Stream *stream, const void *buffer, size_t length);   // 写入流
};

#endif // _INTERNAL_H_
