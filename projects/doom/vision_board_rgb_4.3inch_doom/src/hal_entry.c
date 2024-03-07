/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2023-12-17     Rbb666        first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <dirent.h>

#include "hal_data.h"
#include "doomgeneric.h"
#include "doom.h"

#define DRV_DEBUG
#define LOG_TAG             "main"
#include <drv_log.h>

#define LED_PIN    BSP_IO_PORT_01_PIN_02 /* Onboard LED pins */

static int Doom_run_start(void);

void hal_entry(void)
{
    rt_kprintf("======================================================\n");
    rt_kprintf("This is rgb-4.3inch doom demo.\n");
    rt_kprintf("======================================================\n");
    /* Start-up doom */
    Doom_run_start();
}

static void doom_thread_entry(void *param)
{
    DG_Init();
    D_DoomMain();
}

static int find_doom_file(void)
{
    DIR *dir;
    struct dirent *entry;
    int fileFound = 0;

    /* Open /doom folder */
    dir = opendir("/doom");

    if (dir == NULL)
    {
        LOG_E("Can't open /doom folder");
        return -RT_ERROR;
    }

    /* Iterate through the files in the /doom folder */
    while ((entry = readdir(dir)) != NULL)
    {
        /* Determine whether the file name ends with .wad */
        char *filename = entry->d_name;
        size_t len = strlen(filename);

        if (len >= 4 && strcmp(filename + len - 4, ".wad") == 0)
        {
            LOG_I("Find the file ending with .wad in the /doom folder:：%s", filename);
            fileFound = 1;
            break;
        }
    }

    /* Close dir */
    closedir(dir);

    if (!fileFound)
    {
        LOG_E("No files ending with .wad were found in the /doom folder!!!");
        return -RT_ERROR;
    }

    return RT_EOK;
}

static int Doom_run_start(void)
{
    rt_device_t device;
    device = rt_device_find("sd");
    while (device == NULL)
    {
        device = rt_device_find("sd");
        rt_pin_write(LED_PIN, PIN_HIGH);
        rt_thread_mdelay(200);
        rt_pin_write(LED_PIN, PIN_LOW);
        rt_thread_mdelay(200);
    }

    if (find_doom_file() != RT_EOK)
        return -RT_ERROR;

    rt_thread_t tid;
    tid = rt_thread_create("doom", doom_thread_entry, RT_NULL, 4096, 22, 20);

    RT_ASSERT(tid != RT_NULL);
    rt_thread_startup(tid);

    return 0;
}
