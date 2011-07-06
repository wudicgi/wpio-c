#include "wpio.h"

WPIO_Stream *wpio_alloc(const WPIO_StreamOps *ops, void *aux) {
    WPIO_Stream *stream;

    stream = (WPIO_Stream*)malloc(sizeof(WPIO_Stream));
    if (stream == NULL) {
        return NULL;
    }

    stream->ops = ops;
    stream->aux = aux;

    stream->for_debug = 12345;

    return stream;
}

WPIO_API int wpio_is_readable(WPIO_Stream *stream) {
    return 1;
}

WPIO_API int wpio_is_writable(WPIO_Stream *stream) {
    return 1;
}

WPIO_API int wpio_is_seekable(WPIO_Stream *stream) {
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
WPIO_API size_t wpio_read(WPIO_Stream *stream, void *buffer, size_t length) {
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
WPIO_API size_t wpio_write(WPIO_Stream *stream, const void *buffer, size_t length) {
    return stream->ops->write(stream, buffer, length);
}

/**
 * 将缓冲内容输出到流
 *
 * @return bool 成功时返回 true, 失败时抛出异常
 */
WPIO_API int wpio_flush(WPIO_Stream *stream) {
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
WPIO_API int wpio_seek(WPIO_Stream *stream, off64_t offset, int whence) {
    return stream->ops->seek(stream, offset, whence);
}

/**
 * 获取指针在流中的位置
 *
 * @return int 返回指针在流中的位置，产生错误时抛出异常
 */
WPIO_API off64_t wpio_tell(WPIO_Stream *stream) {
    return stream->ops->tell(stream);
}

/**
 * 测试指针是否到了流结束的位置
 *
 * @return bool 如果指针到了流结束的位置则返回 true, 否则返回 false
 */
WPIO_API int wpio_eof(WPIO_Stream *stream) {
    return stream->ops->eof(stream);
}

/**
 * 关闭流
 *
 * @return bool 成功时返回 true, 失败时抛出异常
 */
WPIO_API int wpio_close(WPIO_Stream *stream) {
    return stream->ops->close(stream);
}
/*
void main(void) {
    printf("sizeof(size_t) = %d\r\n", sizeof(size_t));
    printf("sizeof(off64_t) = %d\r\n", sizeof(off64_t));

    system("pause");
}
*/
