#include <rtthread.h>
#include <rtdevice.h>

#include <dfs_fs.h>

#define DBG_TAG "sd.hotplug"
#define DBG_LVL DBG_INFO
#include "rtdbg.h"

static void sd_hotplug_thread(void *param)
{
    while (1) {
        // 等待热插拔事件（超时时间设为永久等待） cd:card detect引脚
        int result = mmcsd_wait_cd_changed(RT_WAITING_FOREVER);
        if (result == MMCSD_HOST_PLUGED) {
            // 卡插入：尝试挂载文件系统（以elm-fatfs为例）
            if (dfs_mount("sd0", "/", "elm", 0, NULL) == 0) {
                LOG_I("SD card mounted to /");
            } else {
                LOG_E("Failed to mount SD card");
            }
        } else if (result == MMCSD_HOST_UNPLUGED) {
            // 卡移除：尝试卸载文件系统
            if (dfs_unmount("/") == 0) {
                LOG_I("SD card unmounted from /");
            } else {
                LOG_E("Failed to unmount SD card");
            }
        }
    }
}

// 初始化热插拔线程
int sd_hotplug_init(void)
{
    rt_thread_t tid = rt_thread_create("sd_hotplug",
                                       sd_hotplug_thread,
                                       RT_NULL,
                                       1024, // 栈大小
                                       25,   // 优先级（低于检测线程）
                                       10);  // 时间片
    if (tid != RT_NULL) {
        rt_thread_startup(tid);
        return 0;
    }
    return -1;
}
INIT_APP_EXPORT(sd_hotplug_init);
