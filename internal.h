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
    size_t (*read)(WPIO_Stream *stream, void *buffer, size_t length);
    size_t (*write)(WPIO_Stream *stream, const void *buffer, size_t length);
    int (*flush)(WPIO_Stream *stream);
    int (*seek)(WPIO_Stream *stream, off64_t offset, int whence);
    off64_t (*tell)(WPIO_Stream *stream);
    int (*eof)(WPIO_Stream *stream);
    int (*close)(WPIO_Stream *stream);
};

#endif // _INTERNAL_H_
