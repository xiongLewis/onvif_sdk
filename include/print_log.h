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
    log_pos = 0,                        // ��ӡ��ֵַ
    log_int,                            // ��ӡ����
    log_str,                            // ��ӡ�ַ���(char *)
    log_strp,                           // ��ӡ�ַ���(char **)
    log_float,                          // ��ӡ������
    log_bool,                           // ��ӡ����ֵ
    log_int64,                          // ��ӡ64λ����
};

void log_level_fmt(unsigned int level, const char *fmt, ...);
void log_level_val(unsigned int level, const char *prestr, enum LOG_TYPE type, const void *val);
void log_val(const char *prestr, enum LOG_TYPE type, const void *val);

#ifdef __cplusplus
}
#endif

#endif
