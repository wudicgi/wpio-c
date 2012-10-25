#ifndef __WPIO_H__
#define __WPIO_H__

#include <stdio.h>

#ifndef WPIO_API
# define WPIO_API __declspec(dllexport)
#endif

#define WPIO_Stream_BUFFER_SIZE 4096

typedef enum _WPIO_Mode WPIO_Mode;

typedef struct _WPIO_Stream WPIO_Stream;

enum _WPIO_Mode {
    WPIO_MODE_READ_ONLY = 0,
    WPIO_MODE_READ_WRITE = 1
};

struct _WPIO_Stream {
    const WPIO_StreamOps    *ops;
    WPIO_Mode               mode;
    void                    *aux;
    int                     for_debug;
};

WPIO_Stream *wpio_alloc(const WPIO_StreamOps *ops, void *aux);

/**
 * 测试流是否支持读取
 *
 * @return bool 若支持读取则返回 true, 否则返回 false
 */
WPIO_API int wpio_is_readable(WPIO_Stream *stream);

/**
 * 测试流是否支持写入
 *
 * @return bool 若支持写入则返回 true, 否则返回 false
 */
WPIO_API int wpio_is_writable(WPIO_Stream *stream);

/**
 * 测试流是否支持定位
 *
 * @return bool 若支持定位则返回 true, 否则返回 false
 */
WPIO_API int wpio_is_seekable(WPIO_Stream *stream);

/**
 * 关闭流
 *
 * @return bool 成功时返回 true, 失败时抛出异常
 */
WPIO_API int wpio_close(WPIO_Stream *stream);

/**
 * 将缓冲内容输出到流
 *
 * @return bool 成功时返回 true, 失败时抛出异常
 */
WPIO_API int wpio_flush(WPIO_Stream *stream);

/**
 * 在流中定位
 *
 * @param int offset   偏移量
 * @param int whence   定位起始点
 *
 * @return bool 成功时返回 true, 失败时抛出异常
 */
WPIO_API int wpio_seek(WPIO_Stream *stream, off64_t offset, int whence);

/**
 * 获取指针在流中的位置
 *
 * @return int 返回指针在流中的位置，产生错误时抛出异常
 */
WPIO_API off64_t wpio_tell(WPIO_Stream *stream);

/**
 * 读取流
 *
 * 如果可读取的数据没有请求的那么多，则返回的字符串长度小于所请求的字节数。
 * 如果到达流的结尾，返回值为空字符串。
 *
 * @param int length   最多读取的字节数
 *
 * @return string 返回实际所读取的数据，产生错误时抛出异常
 */
WPIO_API size_t wpio_read(WPIO_Stream *stream, void *buffer, size_t length);

/**
 * 写入流
 *
 * 如果可写入流的数据没有请求的那么多，则返回的实际写入的字节数小于所请求的字节数。
 *
 * @param string data  要写入的数据
 *
 * @return int 返回实际写入的字节数，产生错误时抛出异常
 */
WPIO_API size_t wpio_write(WPIO_Stream *stream, const void *buffer, size_t length);

/**
 * 测试指针是否到了流结束的位置
 *
 * @return bool 如果指针到了流结束的位置则返回 true, 否则返回 false
 */
WPIO_API int wpio_eof(WPIO_Stream *stream);

#endif // __WPIO_H__
