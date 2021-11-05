#ifndef __ONVIF_PRINTLOG_H__
#define __ONVIF_PRINTLOG_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#define log_fmt             printf
#define log_func_in         log_fmt("\n================= + %s + >>>\n", __FUNCTION__)
#define log_func_out        log_fmt("================= - %s - <<<\n", __FUNCTION__)

enum LOG_TYPE {
    log_pos = 0,                        // 打印地址值
    log_int,                            // 打印整形
    log_str,                            // 打印字符串(char *)
    log_strp,                           // 打印字符串(char **)
    log_float,                          // 打印浮点数
    log_bool,                           // 打印布尔值
    log_int64,                          // 打印64位整形
};

void log_level_fmt(unsigned int level, const char *fmt, ...);
void log_level_val(unsigned int level, const char *prestr, enum LOG_TYPE type, const void *val);

#ifdef __cplusplus
}
#endif

#endif
