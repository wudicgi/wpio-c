#include "wpio.h"

#define AUX_FP(stream) (((FileStreamData*)((stream)->aux))->fp)

typedef struct _file_stream_data {
    FILE    *fp;
} FileStreamData;

static size_t file_read(WPIO_Stream *stream, void *buffer, size_t length) {
    return fread(buffer, sizeof(char), length, AUX_FP(stream));
}

static size_t file_write(WPIO_Stream *stream, const void *buffer, size_t length) {
    return fwrite(buffer, sizeof(char), length, AUX_FP(stream));
}

static int file_flush(WPIO_Stream *stream) {
    return fflush(AUX_FP(stream));
}

static int file_seek(WPIO_Stream *stream, off64_t offset, int whence) {
    return fseeko64(AUX_FP(stream), offset, whence);
}

static off64_t file_tell(WPIO_Stream *stream) {
    return ftello64(AUX_FP(stream));
}

static int file_eof(WPIO_Stream *stream) {
    return feof(AUX_FP(stream));
}

static int file_close(WPIO_Stream *stream) {
    return fclose(AUX_FP(stream));
}

static const WPIO_StreamOps file_ops = {
    file_read,
    file_write,
    file_flush,
    file_seek,
    file_tell,
    file_eof,
    file_close
};

WPIO_API WPIO_Stream *file_open(const char *filename, const WPIO_Mode mode) {
    FileStreamData *aux;

    if (filename == NULL) {
        return NULL;
    }

    aux = malloc(sizeof(*aux));

    aux->fp = fopen64(filename, (mode == WPIO_MODE_READ_WRITE) ? "r+b" : "rb");
    if (aux->fp == NULL) {
        return NULL;
    }

    return wpio_alloc(&file_ops, aux);
}
