/**
 * Copyright (c) 2025 APU Team Author. [NUAA]. All Rights Reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-08-01     Tian         First version
 */

#include "autopilot.h"
#include <dfs_posix.h>
#include <dirent.h>        // 目录操作
#include <stdarg.h>        // 可变参数

#define MAX_PATH_LEN DFS_PATH_MAX  // 用 RT-Thread 定义的路径最大长度
#define BUFFER_SIZE 256

/**
 * 递归删除目录及其内容
 * @param path 目录路径（需为可修改的缓冲区）
 */
static void __deldir(char* path)
{
    DIR* dir = opendir(path);
    if (dir == RT_NULL)
        return;

    struct dirent* dr;
    while ((dr = readdir(dir)) != RT_NULL)
    {
        // 跳过 . 和 .. 目录项
        if (rt_strcmp(dr->d_name, ".") == 0 || rt_strcmp(dr->d_name, "..") == 0)
            continue;

        // 拼接完整路径（避免溢出）
        char sub_path[MAX_PATH_LEN];
        rt_snprintf(sub_path, MAX_PATH_LEN, "%s/%s", path, dr->d_name);

        if (dr->d_type == DT_DIR)  // 子目录：递归删除
        {
            __deldir(sub_path);
        }
        else  // 文件：直接删除
        {
            unlink(sub_path);
        }
    }

    closedir(dir);
    rmdir(path);  // 目录为空后，用 rmdir 删除
}

/**
 * 向文件描述符输出格式化字符串
 * @param fd 文件描述符
 * @param fmt 格式化字符串
 * @return 写入的字节数，失败返回 -1
 */
int fm_fprintf(int fd, const char* fmt, ...)
{
    va_list args;
    char buffer[BUFFER_SIZE];
    int length;

    va_start(args, fmt);
    // 用 vsnprintf 限制长度，避免溢出
    length = vsnprintf(buffer, BUFFER_SIZE, fmt, args);
    va_end(args);

    if (length <= 0)
        return -1;

    // 写入文件描述符（RT-Thread 的 write 接口兼容 POSIX）
    return write(fd, buffer, length);
}

/**
 * 删除目录（对外接口）
 * @param path 目标目录路径
 * @return 0 成功，-1 失败
 */
int fm_deldir(const char* path)
{
    struct stat buf;
    // 检查路径是否存在
    if (stat(path, &buf) != 0)
        return 0;  // 路径不存在，视为成功

    // 复制路径到可修改的缓冲区（避免修改原字符串）
    char temp_path[MAX_PATH_LEN];
    rt_strncpy(temp_path, path, MAX_PATH_LEN - 1);
    temp_path[MAX_PATH_LEN - 1] = '\0';  // 确保终止符

    __deldir(temp_path);
    return 0;
}