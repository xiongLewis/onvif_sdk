
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stdsoap2.h"
#include "soapStub.h"
//#include "duration.h"
#include "soapH.h"
#include "wsaapi.h"
#include "print_log.h"
#include "onvif_debug.h"
#include "onvif_comm.h"

#define SOAP_ASSERT     assert


#define SOAP_TO         "urn:schemas-xmlsoap-org:ws:2005:04:discovery"
#define SOAP_ACTION     "http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe"

#define SOAP_MCAST_ADDR "soap.udp://239.255.255.250:3702"                       // onvif�涨���鲥��ַ

#define SOAP_ITEM       ""                                                      // Ѱ�ҵ��豸��Χ
#define SOAP_TYPES      "dn:NetworkVideoTransmitter"                            // Ѱ�ҵ��豸����

#define SOAP_SOCK_TIMEOUT    (10)                                               // socket��ʱʱ�䣨�����룩

/************************************************************************
**������ONVIF_init_header
**���ܣ���ʼ��soap������Ϣͷ
**������
        [in] soap - soap��������
**���أ���
**��ע��
    1). �ڱ������ڲ�ͨ��ONVIF_soap_malloc������ڴ棬����ONVIF_soap_delete�б��ͷ�
************************************************************************/
void ONVIF_init_header(struct soap *soap)
{
    struct SOAP_ENV__Header *header = NULL;

    SOAP_ASSERT(NULL != soap);

    header = (struct SOAP_ENV__Header *)ONVIF_soap_malloc(soap, sizeof(struct SOAP_ENV__Header));
    soap_default_SOAP_ENV__Header(soap, header);
    header->wsa__MessageID = (char*)soap_wsa_rand_uuid(soap);
    header->wsa__To        = (char*)ONVIF_soap_malloc(soap, strlen(SOAP_TO) + 1);
    header->wsa__Action    = (char*)ONVIF_soap_malloc(soap, strlen(SOAP_ACTION) + 1);
    strcpy(header->wsa__To, SOAP_TO);
    strcpy(header->wsa__Action, SOAP_ACTION);
    soap->header = header;

    return;
}

/************************************************************************
**������ONVIF_init_ProbeType
**���ܣ���ʼ��̽���豸�ķ�Χ������
**������
        [in]  soap  - soap��������
        [out] probe - ���Ҫ̽����豸��Χ������
**���أ�
        0����̽�⵽����0����δ̽�⵽
**��ע��
    1). �ڱ������ڲ�ͨ��ONVIF_soap_malloc������ڴ棬����ONVIF_soap_delete�б��ͷ�
************************************************************************/
void ONVIF_init_ProbeType(struct soap *soap, struct wsdd__ProbeType *probe)
{
    struct wsdd__ScopesType *scope = NULL;                                      // �����������������Web����

    SOAP_ASSERT(NULL != soap);
    SOAP_ASSERT(NULL != probe);

    scope = (struct wsdd__ScopesType *)ONVIF_soap_malloc(soap, sizeof(struct wsdd__ScopesType));
    soap_default_wsdd__ScopesType(soap, scope);                                 // ����Ѱ���豸�ķ�Χ
    scope->__item = (char*)ONVIF_soap_malloc(soap, strlen(SOAP_ITEM) + 1);
    strcpy(scope->__item, SOAP_ITEM);

    memset(probe, 0x00, sizeof(struct wsdd__ProbeType));
    soap_default_wsdd__ProbeType(soap, probe);
    probe->Scopes = scope;
    probe->Types  = (char*)ONVIF_soap_malloc(soap, strlen(SOAP_TYPES) + 1);     // ����Ѱ���豸������
    strcpy(probe->Types, SOAP_TYPES);

    return;
}

void ONVIF_DetectDevice(void (*cb)(char *DeviceXAddr))
{
    int i;
    int result = 0;
    unsigned int count = 0;                                                     // ���������豸����
    struct soap *soap = NULL;                                                   // soap��������
    struct wsdd__ProbeType      req;                                            // ���ڷ���Probe��Ϣ
    struct __wsdd__ProbeMatches rep;                                            // ���ڽ���ProbeӦ��
    struct wsdd__ProbeMatchType *probeMatch;

    SOAP_ASSERT(NULL != (soap = ONVIF_soap_new(SOAP_SOCK_TIMEOUT)));

    ONVIF_init_header(soap);                                                    // ������Ϣͷ����
    ONVIF_init_ProbeType(soap, &req);                                           // ����Ѱ�ҵ��豸�ķ�Χ������
    result = soap_send___wsdd__Probe(soap, SOAP_MCAST_ADDR, NULL, &req);        // ���鲥��ַ�㲥Probe��Ϣ
    while (SOAP_OK == result)                                                   // ��ʼѭ�������豸���͹�������Ϣ
    {
        memset(&rep, 0x00, sizeof(rep));
        result = soap_recv___wsdd__ProbeMatches(soap, &rep);
        if (SOAP_OK == result) {
            if (soap->error) {
                soap_perror(soap, "ProbeMatches");
            } else {                                                            // �ɹ����յ��豸��Ӧ����Ϣ
                dump__wsdd__ProbeMatches(&rep);

                if (NULL != rep.wsdd__ProbeMatches) {
                    count += rep.wsdd__ProbeMatches->__sizeProbeMatch;
                    for(i = 0; i < rep.wsdd__ProbeMatches->__sizeProbeMatch; i++) {
                        probeMatch = rep.wsdd__ProbeMatches->ProbeMatch + i;
                        if (NULL != cb) {
                            cb(probeMatch->XAddrs);                             // ʹ���豸�����ִַ�к����ص�
                        }
                    }
                }
            }
        } else if (soap->error) {
            break;
        }
    }

    SOAP_DBGLOG("\ndetect end! It has detected %d devices!\n", count);

    if (NULL != soap) {
        ONVIF_soap_delete(soap);
    }

    return ;
}

int main(int argc, char **argv)
{
    ONVIF_DetectDevice(NULL);

    return 0;
}