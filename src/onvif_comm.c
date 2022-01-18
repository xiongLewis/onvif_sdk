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
    struct soap *soap = NULL;                                                   // soap��������

    SOAP_ASSERT(NULL != (soap = soap_new()));

    soap_set_namespaces(soap, namespaces);                                      // ����soap��namespaces
    soap->recv_timeout    = timeout;                                            // ���ó�ʱ������ָ��ʱ��û�����ݾ��˳���
    soap->send_timeout    = timeout;
    soap->connect_timeout = timeout;

#if defined(__linux__) || defined(__linux)                                      // �ο�https://www.genivia.com/dev.html#client-c���޸ģ�
    soap->socket_flags = MSG_NOSIGNAL;                                          // To prevent connection reset errors
#endif

    soap_set_mode(soap, SOAP_C_UTFSTRING);                                      // ����ΪUTF-8���룬�����������OSD������

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
**������ONVIF_SetAuthInfo
**���ܣ�������֤��Ϣ
**������
        [in] soap     - soap��������
        [in] username - �û���
        [in] password - ����
**���أ�
        0�����ɹ�����0����ʧ��
**��ע��
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
**������ONVIF_MakeUriWithAuth
**���ܣ����������֤��Ϣ��URI��ַ
**������
        [in]  src_uri       - δ����֤��Ϣ��URI��ַ
        [in]  username      - �û���
        [in]  password      - ����
        [out] dest_uri      - ���صĴ���֤��Ϣ��URI��ַ
        [in]  size_dest_uri - dest_uri�����С
**���أ�
        0�ɹ�����0ʧ��
**��ע��
    1). ���ӣ�
    ����֤��Ϣ��uri��rtsp://100.100.100.140:554/av0_0
    ����֤��Ϣ��uri��rtsp://username:password@100.100.100.140:554/av0_0
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

    needBufSize = strlen(src_uri) + strlen(username) + strlen(password) + 3;    // ��黺���Ƿ��㹻��������:���͡�@�����ַ���������
    if (size_dest_uri < needBufSize) {
        SOAP_DBGERR("dest uri buf size is not enough.\n");
        result = -1;
        goto EXIT;
    }

    if (0 == strlen(username) && 0 == strlen(password)) {                       // �����µ�uri��ַ
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