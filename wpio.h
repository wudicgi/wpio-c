#ifndef __WPIO_H__
#define __WPIO_H__

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

#ifndef WPIO_API
# define WPIO_API __declspec(dllexport)
#endif

#define WPIO_Stream_BUFFER_SIZE 4096

typedef enum _WPIO_Mode WPIO_Mode;

typedef struct _WPIO_StreamOps WPIO_StreamOps;
typedef struct _WPIO_Stream WPIO_Stream;

#include "file.h"
#include "wpes.h"

enum _WPIO_Mode {
    WPIO_MODE_READ_ONLY = 0,
    WPIO_MODE_READ_WRITE = 1
};

struct _WPIO_StreamOps {
    size_t (*read)(WPIO_Stream *stream, void *buffer, size_t length);
    size_t (*write)(WPIO_Stream *stream, const void *buffer, size_t length);
    int (*flush)(WPIO_Stream *stream);
    int (*seek)(WPIO_Stream *stream, off64_t offset, int whence);
    off64_t (*tell)(WPIO_Stream *stream);
    int (*eof)(WPIO_Stream *stream);
    int (*close)(WPIO_Stream *stream);
};

struct _WPIO_Stream {
    const WPIO_StreamOps    *ops;
    WPIO_Mode               mode;
    void                    *aux;
    int                     for_debug;
};

WPIO_Stream *wpio_alloc(const WPIO_StreamOps *ops, void *aux);

WPIO_API int wpio_is_readable(WPIO_Stream *stream);
WPIO_API int wpio_is_writable(WPIO_Stream *stream);
WPIO_API int wpio_is_seekable(WPIO_Stream *stream);

WPIO_API size_t wpio_read(WPIO_Stream *stream, void *buffer, size_t length);
WPIO_API size_t wpio_write(WPIO_Stream *stream, const void *buffer, size_t length);
WPIO_API int wpio_flush(WPIO_Stream *stream);
WPIO_API int wpio_seek(WPIO_Stream *stream, off64_t offset, int whence);
WPIO_API off64_t wpio_tell(WPIO_Stream *stream);
WPIO_API int wpio_eof(WPIO_Stream *stream);
WPIO_API int wpio_close(WPIO_Stream *stream);

#endif // __WPIO_H__
