#include "onvif_comm.h"


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