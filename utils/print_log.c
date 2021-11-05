#include "soapH.h"
#include "stdsoap2.h"
#include "soapStub.h"
#include "print_log.h"

#define INDENTATION         3           // 打印时每层缩进的空格数

static void log_val(const char *prestr, enum LOG_TYPE type, const void *val)
{
#ifdef WIN32
#define POSFORMAT   "(0x%p)"
#else
#define POSFORMAT   "(%p)"
#endif

    if (NULL == val) {
        log_fmt("%s(null)\n", prestr);
        return ;
    }

    switch (type)
    {
    case log_pos:
        log_fmt("%s" POSFORMAT "\n", prestr, val);
        break;
    case log_int:
        log_fmt("%s%d\n", prestr, *((int*)val));
        break;
    case log_str:
        log_fmt("%s%s\n", prestr, (char*)val);
        break;
    case log_strp:
        log_fmt("%s%s\n", prestr, *((char**)val));
        break;
    case log_float:
        log_fmt("%s%f\n", prestr, *((float*)val));
        break;
    case log_bool:
        log_fmt("%s%s\n", prestr, (*((enum xsd__boolean*)val)) ? "true" : "false" );
        break;
    case log_int64:
        log_fmt("%s%lld\n", prestr, *((long long*)val));
        break;
    default:
        assert(0);
        break;
    }

    return ;
}

static void log_indentation(unsigned int level)
{
    if (level > 0) {
        log_fmt("%*s%s", level * INDENTATION, "", "├─ ");
    }
    return ;
}

/************************************************************************
**函数：log_level_fmt
**功能：打印具有层级结构的日志信息
**参数：
        [in] level  - 层级
        [in] fmt    - 格式化字符串
**返回：
**备注：
************************************************************************/
void log_level_fmt(unsigned int level, const char *fmt, ...)
{
    va_list ap;

    log_indentation(level);

    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);

    return ;
}

/************************************************************************
**函数：log_level
**功能：打印具有层级结构的日志信息
**参数：
        [in] level  - 层级
        [in] prestr - 附加的前缀日志信息字符串
        [in] type   - 要打印的参数类型
        [in] val    - 要打印的参数地址
**返回：
**备注：
************************************************************************/
void log_level_val(unsigned int level, const char *prestr, enum LOG_TYPE type, const void *val)
{
    log_indentation(level);
    log_val(prestr, type, val);
    return ;
}

