/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Armink (armink.ztl@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <rtthread.h>
#ifdef RT_USING_DFS
#include <unistd.h>
#include <fcntl.h>
#include <dfs.h>
#endif
#include "py/mphal.h"
#include "mpy_main.h"

#ifdef RT_USING_DFS
int mp_sys_resource_bak(struct dfs_fdtable **table_bak)
{
    struct dfs_fdtable *fd_table;
    struct dfs_fdtable *fd_table_bak;
    struct dfs_file **fds;

    fd_table = dfs_fdtable_get();
    if (!fd_table) 
    {
        return RT_FALSE;
    }

    fd_table_bak = (struct dfs_fdtable *)rt_malloc(sizeof(struct dfs_fdtable));
    if (!fd_table_bak)
    {
        goto _exit_tab;
    }

    fds = (struct dfs_file **)rt_malloc((int)fd_table->maxfd * sizeof(struct dfs_file *));
    if (!fds)
    {
        goto _exit_fds;
    }
    else
    {
        rt_memcpy(fds, fd_table->fds, (int)fd_table->maxfd * sizeof(struct dfs_file *));
        fd_table_bak->maxfd = (int)fd_table->maxfd;
        fd_table_bak->fds = fds;
    }

    *table_bak = fd_table_bak;

    return RT_TRUE;

_exit_fds:
    rt_free(fd_table_bak);
    
_exit_tab:
    return RT_FALSE;
}

void mp_sys_resource_gc(struct dfs_fdtable *fd_table_bak)
{
    struct dfs_fdtable *fd_table;
    
    if (!fd_table_bak) return;

    fd_table = dfs_fdtable_get();

    for(int i = 0; i < (int)fd_table->maxfd; i++)
    {
        if (fd_table->fds[i] != RT_NULL)
        {
            if ((i < fd_table_bak->maxfd && fd_table_bak->fds[i] == RT_NULL) || (i >= fd_table_bak->maxfd))
            {
                close(i);
            }
        }
    }

    rt_free(fd_table_bak->fds);
    rt_free(fd_table_bak);
}
#endif

#if !MICROPY_PY_MODUOS_FILE
mp_import_stat_t mp_import_stat(const char *path) {
    return MP_IMPORT_STAT_NO_EXIST;
}
#endif

NORETURN void nlr_jump_fail(void *val) {
    mp_printf(MICROPY_ERROR_PRINTER, "nlr_jump_fail\n");
    while (1);
}

#ifndef NDEBUG
NORETURN void MP_WEAK __assert_func(const char *file, int line, const char *func, const char *expr) {
    mp_printf(MICROPY_ERROR_PRINTER, "Assertion '%s' failed, at file %s:%d\n", expr, file, line);
    while (1);
}
#endif

int DEBUG_printf(const char *format, ...)
{
    static char log_buf[512];
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    /* must use vprintf to print */
    rt_vsprintf(log_buf, format, args);
    mp_printf(&mp_plat_print, "%s", log_buf);
    va_end(args);

    return 0;
}

#ifndef MICROPYTHON_USING_UOS
mp_lexer_t *mp_lexer_new_from_file(const char *filename) {
    mp_raise_OSError(MP_ENOENT);
}
#endif
