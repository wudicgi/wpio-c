#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>
#include "wpio.h"
#include "file.h"

#define FILE_IO_STREAM(stream) (((FILE_STREAM_DATA*)stream->aux)->io)
#define FILE_INPUT_STREAM(stream) (((FILE_STREAM_DATA*)stream->aux)->input)
#define FILE_OUTPUT_STREAM(stream) (((FILE_STREAM_DATA*)stream->aux)->output)

static gssize file_read(WpioStream *stream, void *buffer, gsize length) {
    gssize bytes_read;

    bytes_read = g_input_stream_read(FILE_INPUT_STREAM(stream), buffer, length, NULL, NULL);

    return bytes_read;
}

static gssize file_write(WpioStream *stream, const void *buffer, gsize length) {
    gssize bytes_written;

    bytes_written = g_output_stream_write(FILE_OUTPUT_STREAM(stream), buffer, length, NULL, NULL);

    return bytes_written;
}

static gboolean file_flush(WpioStream *stream) {
    return g_output_stream_flush(FILE_OUTPUT_STREAM(stream), NULL, NULL);
}

static gboolean file_seek(WpioStream *stream, goffset offset, GSeekType whence) {
    return g_seekable_seek((GSeekable*)FILE_INPUT_STREAM(stream), offset, whence, NULL, NULL);
}

static goffset file_tell(WpioStream *stream) {
    return g_seekable_tell((GSeekable*)FILE_INPUT_STREAM(stream));
}
/*
static int file_eof(WpioStream *stream) {
}
*/
static gboolean file_close(WpioStream *stream) {
    if (FILE_IO_STREAM(stream) != NULL) {
        g_io_stream_close(FILE_IO_STREAM(stream), NULL, NULL);
    } else {
        g_input_stream_close(FILE_INPUT_STREAM(stream), NULL, NULL);
    }

    return TRUE;
}

static const WpioStream_OPS file_ops = {
    file_read,
    file_write,
    file_flush,
    file_seek,
    file_tell,
//    file_eof,
    file_close
};

WpioStream *file_open(const char *filename, const WpioMode mode) {
    FILE_STREAM_DATA *aux;
    GFile *file;
    GError *error = NULL;

    if (filename == NULL) {
        return NULL;
    }

    aux = malloc(sizeof(*aux));

    file = g_file_new_for_path(filename);
    if (mode == WPIO_MODE_READ_WRITE) {
        aux->io = (GIOStream*)g_file_open_readwrite(file, NULL, &error);
        if (error != NULL) {
            return NULL;
        }
        aux->input = g_io_stream_get_input_stream(aux->io);
        aux->output = g_io_stream_get_output_stream(aux->io);
    } else {
        aux->io = NULL;
        aux->input = (GInputStream*)g_file_read(file, NULL, &error);
        if (error != NULL) {
            return NULL;
        }
        aux->output = NULL;
    }

    return wpio_alloc(&file_ops, aux);
}
