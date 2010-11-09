#ifndef __FILE_H__
#define __FILE_H__

#include <glib.h>

/*
#ifndef _FILE_OFFSET_BITS
# define _FILE_OFFSET_BITS 64
#endif

#ifndef _LARGEFILE_SOURCE
# define _LARGEFILE_SOURCE
#endif

#ifndef _LARGEFILE64_SOURCE
# define _LARGEFILE64_SOURCE
#endif

#define __USE_FILE_OFFSET64
#define __USE_LARGEFILE64
*/

#define FILE_BUFFER_SIZE 8192

typedef struct _file_stream_data {
    GIOStream       *io;
    GInputStream    *input;
    GOutputStream   *output;
} FILE_STREAM_DATA;

WpioStream *file_open(const char *filename, const WpioMode mode);

#endif // __FILE_H__
