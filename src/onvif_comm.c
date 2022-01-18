#include "onvif_comm.h"
#include "wsseapi.h"

void* ONVIF_soap_malloc(struct soap *soap, unsigned int n)
{
    void *p = NULL;

    if (n > 0) {
        p = soap_malloc(soap, n);
        SOAP_ASSERT(NULL != p);
        memset(p, 0x00 ,n);
    }
    return p;
}

struct soap *ONVIF_soap_new(int timeout)
{
    struct soap *soap = NULL;                                                   // soap环境变量

    SOAP_ASSERT(NULL != (soap = soap_new()));

    soap_set_namespaces(soap, namespaces);                                      // 设置soap的namespaces
    soap->recv_timeout    = timeout;                                            // 设置超时（超过指定时间没有数据就退出）
    soap->send_timeout    = timeout;
    soap->connect_timeout = timeout;

#if defined(__linux__) || defined(__linux)                                      // 参考https://www.genivia.com/dev.html#client-c的修改：
    soap->socket_flags = MSG_NOSIGNAL;                                          // To prevent connection reset errors
#endif

    soap_set_mode(soap, SOAP_C_UTFSTRING);                                      // 设置为UTF-8编码，否则叠加中文OSD会乱码

    return soap;
}

void ONVIF_soap_delete(struct soap *soap)
{
    soap_destroy(soap);                                                         // remove deserialized class instances (C++ only)
    soap_end(soap);                                                             // Clean up deserialized data (except class instances) and temporary data
    soap_done(soap);                                                            // Reset, close communications, and remove callbacks
    soap_free(soap);                                                            // Reset and deallocate the context created with soap_new or soap_copy
}

/************************************************************************
**函数：ONVIF_SetAuthInfo
**功能：设置认证信息
**参数：
        [in] soap     - soap环境变量
        [in] username - 用户名
        [in] password - 密码
**返回：
        0表明成功，非0表明失败
**备注：
************************************************************************/
int ONVIF_SetAuthInfo(struct soap *soap, const char *username, const char *password)
{
    int result = 0;

    SOAP_ASSERT(NULL != username);
    SOAP_ASSERT(NULL != password);

    result = soap_wsse_add_UsernameTokenDigest(soap, NULL, username, password);
    SOAP_CHECK_ERROR(result, soap, "add_UsernameTokenDigest");

EXIT:

    return result;
}

/************************************************************************
**函数：ONVIF_MakeUriWithAuth
**功能：构造带有认证信息的URI地址
**参数：
        [in]  src_uri       - 未带认证信息的URI地址
        [in]  username      - 用户名
        [in]  password      - 密码
        [out] dest_uri      - 返回的带认证信息的URI地址
        [in]  size_dest_uri - dest_uri缓存大小
**返回：
        0成功，非0失败
**备注：
    1). 例子：
    无认证信息的uri：rtsp://100.100.100.140:554/av0_0
    带认证信息的uri：rtsp://username:password@100.100.100.140:554/av0_0
************************************************************************/
int ONVIF_MakeUriWithAuth(char *src_uri, char *username, char *password, char *dest_uri, unsigned int size_dest_uri)
{
    int result = 0;
    unsigned int needBufSize = 0;

    SOAP_ASSERT(NULL != src_uri);
    SOAP_ASSERT(NULL != username);
    SOAP_ASSERT(NULL != password);
    SOAP_ASSERT(NULL != dest_uri);
    memset(dest_uri, 0x00, size_dest_uri);

    needBufSize = strlen(src_uri) + strlen(username) + strlen(password) + 3;    // 检查缓存是否足够，包括‘:’和‘@’和字符串结束符
    if (size_dest_uri < needBufSize) {
        SOAP_DBGERR("dest uri buf size is not enough.\n");
        result = -1;
        goto EXIT;
    }

    if (0 == strlen(username) && 0 == strlen(password)) {                       // 生成新的uri地址
        strcpy(dest_uri, src_uri);
    } else {
        char *p = strstr(src_uri, "//");
        if (NULL == p) {
            SOAP_DBGERR("can't found '//', src uri is: %s.\n", src_uri);
            result = -1;
            goto EXIT;
        }
        p += 2;

        memcpy(dest_uri, src_uri, p - src_uri);
        sprintf(dest_uri + strlen(dest_uri), "%s:%s@", username, password);
        strcat(dest_uri, p);
    }

EXIT:

    return result;
}