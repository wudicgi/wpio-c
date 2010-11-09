#ifndef __WPIO_H__
#define __WPIO_H__

#include "glib.h"

#define WpioStream_BUFFER_SIZE 4096

typedef struct _WpioStream WpioStream;

typedef struct _WpioStream_ops {
    gssize (*read)(WpioStream *stream, void *buffer, gsize length);
    gssize (*write)(WpioStream *stream, const void *buffer, gsize length);
    gboolean (*flush)(WpioStream *stream);
    gboolean (*seek)(WpioStream *stream, goffset offset, GSeekType whence);
    goffset (*tell)(WpioStream *stream);
//    int (*eof)(WpioStream *stream);
    int (*close)(WpioStream *stream);
} WpioStream_OPS;

typedef enum _WpioMode {
    WPIO_MODE_READ_ONLY,
    WPIO_MODE_READ_WRITE
} WpioMode;

struct _WpioStream {
    const WpioStream_OPS    *ops;
    WpioMode                mode;
    void                    *aux;
}; // WpioStream

WpioStream *wpio_alloc(const WpioStream_OPS *ops, void *aux);

int wpio_is_readable(WpioStream *stream);
int wpio_is_writable(WpioStream *stream);
int wpio_is_seekable(WpioStream *stream);

size_t wpio_read(WpioStream *stream, void *buffer, size_t length);
size_t wpio_write(WpioStream *stream, void *buffer, size_t length);
int wpio_flush(WpioStream *stream);
int wpio_seek(WpioStream *stream, long offset, int whence);
long wpio_tell(WpioStream *stream);
int wpio_eof(WpioStream *stream);
int wpio_close(WpioStream *stream);

#endif // __WPIO_H__
