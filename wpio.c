#include <stdio.h>
#include <stdlib.h>
#include "wpio.h"

WpioStream *wpio_alloc(const WpioStream_OPS *ops, void *aux) {
    WpioStream *stream;

    stream = (WpioStream*)malloc(sizeof(WpioStream));
    if (stream == NULL) {
        return NULL;
    }

    stream->ops = ops;
    stream->aux = aux;

    return stream;
}

int wpio_is_readable(WpioStream *stream) {
    return 1;
}

int wpio_is_writable(WpioStream *stream) {
    return 1;
}

int wpio_is_seekable(WpioStream *stream) {
    return 1;
}

/**
 * 读取流
 *
 * 如果可读取的数据没有请求的那么多，则返回的字符串长度小于所请求的字节数。
 * 如果到达流的结尾，返回值为空字符串。
 *
 * @param int $length   最多读取的字节数
 *
 * @return string 返回实际所读取的数据，产生错误时抛出异常
 */
size_t wpio_read(WpioStream *stream, void *buffer, size_t length) {
    return stream->ops->read(stream, buffer, length);
}

/**
 * 写入流
 *
 * 如果可写入流的数据没有请求的那么多，则返回的实际写入的字节数小于所请求的字节数。
 *
 * @param string $data  要写入的数据
 *
 * @return int 返回实际写入的字节数，产生错误时抛出异常
 */
size_t wpio_write(WpioStream *stream, void *buffer, size_t length) {
    return stream->ops->write(stream, buffer, length);
}

/**
 * 将缓冲内容输出到流
 *
 * @return bool 成功时返回 true, 失败时抛出异常
 */
int wpio_flush(WpioStream *stream) {
    return stream->ops->flush(stream);
}

/**
 * 在流中定位
 *
 * @param int $offset   偏移量
 * @param int $whence   定位起始点
 *
 * @return bool 成功时返回 true, 失败时抛出异常
 */
int wpio_seek(WpioStream *stream, long offset, int whence) {
    return stream->ops->seek(stream, offset, whence);
}

/**
 * 获取指针在流中的位置
 *
 * @return int 返回指针在流中的位置，产生错误时抛出异常
 */
long wpio_tell(WpioStream *stream) {
    return stream->ops->tell(stream);
}

/**
 * 测试指针是否到了流结束的位置
 *
 * @return bool 如果指针到了流结束的位置则返回 true, 否则返回 false
 */
/*
int wpio_eof(WpioStream *stream) {
    return stream->ops->eof(stream);
}
*/

/**
 * 关闭流
 *
 * @return bool 成功时返回 true, 失败时抛出异常
 */
int wpio_close(WpioStream *stream) {
    return stream->ops->close(stream);
}
