#include "internal.h"

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

WPIO_API int wpio_close(WPIO_Stream *stream) {
    return stream->ops->close(stream);
}

WPIO_API int wpio_flush(WPIO_Stream *stream) {
    return stream->ops->flush(stream);
}

WPIO_API int wpio_seek(WPIO_Stream *stream, off64_t offset, int whence) {
    return stream->ops->seek(stream, offset, whence);
}

WPIO_API off64_t wpio_tell(WPIO_Stream *stream) {
    return stream->ops->tell(stream);
}

WPIO_API size_t wpio_read(WPIO_Stream *stream, void *buffer, size_t length) {
    return stream->ops->read(stream, buffer, length);
}

WPIO_API size_t wpio_write(WPIO_Stream *stream, const void *buffer, size_t length) {
    return stream->ops->write(stream, buffer, length);
}

WPIO_API int wpio_eof(WPIO_Stream *stream) {
    return stream->ops->eof(stream);
}

/*
void main(void) {
    printf("sizeof(size_t) = %d\r\n", sizeof(size_t));
    printf("sizeof(off64_t) = %d\r\n", sizeof(off64_t));

    system("pause");
}
*/
