#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "libs/crc32.h"
#include "libs/xxtea.h"
#include "wpio.h"
#include "wpes.h"

static long wpes_get_block_offset(long offset) {
    return ((offset / WPES_DATA_SIZE) * WPES_BLOCK_SIZE);
}

static int wpes_read_buffer(WpioStream *stream, long offset_block) {
    WPES_STREAM_DATA *self;
    WPES_HEAD head;

    uint32_t ret_len;

    self = (WPES_STREAM_DATA*)stream->aux;

    wpio_seek(self->base_stream, offset_block, SEEK_SET);

    wpio_read(self->base_stream, &head, WPES_HEAD_SIZE);

    if (head.signature != WPES_BLOCK_SIGNATURE) {
        return -1;
    }

    if (!(head.flags & WPES_BLOCK_FLAG_DATA)) {
        return -1;
    }

    wpio_read(self->base_stream, self->buffer, WPES_DATA_SIZE);

    if (head.signature & WPES_BLOCK_FLAG_XXTEA) {
        xxtea_decrypt(self->buffer, self->buffer, WPES_DATA_SIZE, (unsigned char*)"tttggg", &ret_len);
    }

    // assert('$temp[\'crc32\'] == crc32($this->_buffer)');

    self->buffer_length = head.length;
    self->buffer_offset = offset_block;

    return 0;
}

static int wpes_write_buffer(WpioStream *stream) {
    WPES_STREAM_DATA *self;
    WPES_HEAD head;

    uint32_t ret_len;

    self = (WPES_STREAM_DATA*)stream->aux;

    wpio_seek(self->base_stream, self->buffer_offset, SEEK_SET);

    if (self->buffer_length < WPES_DATA_SIZE) { // to be noticed, different
        memset((self->buffer + self->buffer_length), 0,
               (WPES_DATA_SIZE - self->buffer_length));
    }

    head.signature = WPES_BLOCK_SIGNATURE;
    head.flags = WPES_BLOCK_FLAG_DATA | WPES_BLOCK_FLAG_XXTEA;
    head.length = self->buffer_length;
    head.datasize = WPES_DATA_SIZE;
    head.crc32 = crc32(self->buffer, self->buffer_length);
    memset(&head.reserved, 0, 16);

    xxtea_encrypt(self->buffer, self->buffer, WPES_DATA_SIZE, (unsigned char*)"tttggg", &ret_len);

    wpio_write(self->base_stream, &head, sizeof(WPES_HEAD));
    wpio_write(self->base_stream, self->buffer, WPES_DATA_SIZE);

    return 0;
}

static gssize wpes_read(WpioStream *stream, void *buffer, gsize length) {
    WPES_STREAM_DATA *self;
    int len_ahead, len_behind, len_read;
    int didread = 0;

    self = (WPES_STREAM_DATA*)stream->aux;

    while (didread < length) {
        if (self->offset >= self->length) {
            break;
        }
        if (self->buffer_offset != wpes_get_block_offset(self->offset)) {
            wpes_read_buffer(stream, wpes_get_block_offset(self->offset));
        }
        len_ahead = self->offset % WPES_DATA_SIZE;
        len_behind = self->buffer_length - len_ahead; // notice, diff to write()
        len_read = MIN(len_behind, length - didread);
        /*
        if (len_ahead == 0 && len_read == WPES_DATA_SIZE) { // notice, diff to write()
            memcpy(buffer, self->buffer, WPES_DATA_SIZE);
        } else {
            memcpy(buffer, self->buffer + len_ahead, len_read);
        }
        */
        memcpy(buffer, self->buffer + len_ahead, len_read);
        didread += len_read;
        self->offset += len_read;
    }

//    assert('strlen($data) == $didread');

    return 0;
}

static gssize wpes_write(WpioStream *stream, const void *buffer, gsize length) {
    WPES_STREAM_DATA *self;
    int len_ahead, len_behind, len_write;
    int didwrite = 0;

    self = (WPES_STREAM_DATA*)stream->aux;

    if (length == 0) {
        // to be noticed
        return 0;
    }

    // 如果数据还没写完
    while (didwrite < length) {
        // 计算在块中，当前位置前数据的长度
        len_ahead = self->offset % WPES_DATA_SIZE;
        // 计算在块中，当前位置后数据的长度
        len_behind = WPES_DATA_SIZE - len_ahead;
        // 计算在块中，可写数据的长度
        len_write = MIN(len_behind, length - didwrite);
        // 如果当前在文件末尾新的块中，或者当前可写一整个块
        /* to summarized */
        if ((self->offset == self->length && len_ahead == 0) ||
            (len_ahead == 0 && len_write == len_behind)) {
            memcpy(self->buffer, (buffer + didwrite), len_write);
            self->buffer_length = len_write;
            self->buffer_offset = wpes_get_block_offset(self->offset);
        } else {
            if (self->buffer_offset != wpes_get_block_offset(self->offset)) {
                wpes_read_buffer(stream, wpes_get_block_offset(self->offset));
            }
            memcpy((self->buffer + len_ahead), (buffer + didwrite), len_write);
            if ((len_ahead + len_write) > self->buffer_length) {
                self->buffer_length = len_ahead + len_write;
            }
        }
        /* to summarized */

        wpes_write_buffer(stream);
        didwrite += len_write;
        self->offset += len_write;
        if (self->offset > self->length) {
            self->length = self->offset;
            /*
            $this->_buffer = '';
            $this->_buffer_length = 0;
            $this->_buffer_offset = $this->_offset;
            */
        }
    }

//    assert('$didwrite == $length');

    return 0;
}

static gboolean wpes_flush(WpioStream *stream) {
    return TRUE;
}

static gboolean wpes_seek(WpioStream *stream, goffset offset, GSeekType whence) {
    WPES_STREAM_DATA *self = (WPES_STREAM_DATA*)stream->aux;

    if (whence == G_SEEK_SET) {
        self->offset = offset;
    } else if (whence == G_SEEK_END) {
        self->offset = self->length + offset;
    } else if (whence == G_SEEK_CUR) {
        self->offset += offset;
    }

    return TRUE;
}

static goffset wpes_tell(WpioStream *stream) {
    WPES_STREAM_DATA *self = (WPES_STREAM_DATA*)stream->aux;

    return self->offset;
}

/*
static int wpes_eof(WpioStream *stream) {
    WPES_STREAM_DATA *self = (WPES_STREAM_DATA*)stream->aux;

    return (self->offset == self->length);
}
*/

static gboolean wpes_close(WpioStream *stream) {
    WPES_STREAM_DATA *self = (WPES_STREAM_DATA*)stream->aux;

    return wpio_close(self->base_stream);
}

static const WpioStream_OPS wpes_ops = {
    wpes_read,
    wpes_write,
    wpes_flush,
    wpes_seek,
    wpes_tell,
//    wpes_eof,
    wpes_close
};

WpioStream *wpes_open(const WpioStream *base_stream, const WpioMode mode) {
    WPES_STREAM_DATA *aux;

    if (base_stream == NULL) {
        return NULL;
    }

    aux = malloc(sizeof(*aux));

    aux->base_stream = (WpioStream*)base_stream;
    aux->offset = 0;
    aux->length = 0;
    aux->buffer_length = 0;
    aux->buffer_offset = -65536; // to be noticed
    memset(aux->buffer, 0, WPES_DATA_SIZE);

    return wpio_alloc(&wpes_ops, aux);
}
