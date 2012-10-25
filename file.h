#ifndef __FILE_H__
#define __FILE_H__

/**
 * 打开文件
 *
 * @return pointer 成功时返回指向 WPIO_Stream 结构的指针, 失败时返回 NULL
 */
WPIO_API WPIO_Stream *file_open(const char *filename, const WPIO_Mode mode);

#endif // __FILE_H__
