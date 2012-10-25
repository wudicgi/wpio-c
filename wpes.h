#ifndef __WPES_H__
#define __WPES_H__

/**
 * 打开 WPES 文件
 *
 * @return pointer 成功时返回指向 WPIO_Stream 结构的指针, 失败时返回 NULL
 */
WPIO_API WPIO_Stream *wpes_open(const char *filename, const WPIO_Mode mode);

#endif // __WPES_H__
