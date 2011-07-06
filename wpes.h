#ifndef __WPES_H__
#define __WPES_H__

WPIO_Stream *wpes_open_stream(const WPIO_Stream *base_stream, const WPIO_Mode mode);
WPIO_API WPIO_Stream *wpes_open(const char *filename, const WPIO_Mode mode);

#endif // __WPES_H__
