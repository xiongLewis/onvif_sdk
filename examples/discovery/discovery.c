
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


#define SOAP_TO         "urn:schemas-xmlsoap-org:ws:2005:04:discovery"
#define SOAP_ACTION     "http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe"

#define SOAP_MCAST_ADDR "soap.udp://239.255.255.250:3702"                       // onvif规定的组播地址

#define SOAP_ITEM       ""                                                      // 寻找的设备范围
#define SOAP_TYPES      "dn:NetworkVideoTransmitter"                            // 寻找的设备类型

#define SOAP_SOCK_TIMEOUT    (10)                                               // socket超时时间（单秒秒）

/************************************************************************
**函数：ONVIF_init_header
**功能：初始化soap描述消息头
**参数：
        [in] soap - soap环境变量
**返回：无
**备注：
    1). 在本函数内部通过ONVIF_soap_malloc分配的内存，将在ONVIF_soap_delete中被释放
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
**函数：ONVIF_init_ProbeType
**功能：初始化探测设备的范围和类型
**参数：
        [in]  soap  - soap环境变量
        [out] probe - 填充要探测的设备范围和类型
**返回：
        0表明探测到，非0表明未探测到
**备注：
    1). 在本函数内部通过ONVIF_soap_malloc分配的内存，将在ONVIF_soap_delete中被释放
************************************************************************/
void ONVIF_init_ProbeType(struct soap *soap, struct wsdd__ProbeType *probe)
{
    struct wsdd__ScopesType *scope = NULL;                                      // 用于描述查找哪类的Web服务

    SOAP_ASSERT(NULL != soap);
    SOAP_ASSERT(NULL != probe);

    scope = (struct wsdd__ScopesType *)ONVIF_soap_malloc(soap, sizeof(struct wsdd__ScopesType));
    soap_default_wsdd__ScopesType(soap, scope);                                 // 设置寻找设备的范围
    scope->__item = (char*)ONVIF_soap_malloc(soap, strlen(SOAP_ITEM) + 1);
    strcpy(scope->__item, SOAP_ITEM);

    memset(probe, 0x00, sizeof(struct wsdd__ProbeType));
    soap_default_wsdd__ProbeType(soap, probe);
    probe->Scopes = scope;
    probe->Types  = (char*)ONVIF_soap_malloc(soap, strlen(SOAP_TYPES) + 1);     // 设置寻找设备的类型
    strcpy(probe->Types, SOAP_TYPES);

    return;
}

void ONVIF_DetectDevice(void (*cb)(char *DeviceXAddr))
{
    int i;
    int result = 0;
    unsigned int count = 0;                                                     // 搜索到的设备个数
    struct soap *soap = NULL;                                                   // soap环境变量
    struct wsdd__ProbeType      req;                                            // 用于发送Probe消息
    struct __wsdd__ProbeMatches rep;                                            // 用于接收Probe应答
    struct wsdd__ProbeMatchType *probeMatch;

    SOAP_ASSERT(NULL != (soap = ONVIF_soap_new(SOAP_SOCK_TIMEOUT)));

    ONVIF_init_header(soap);                                                    // 设置消息头描述
    ONVIF_init_ProbeType(soap, &req);                                           // 设置寻找的设备的范围和类型
    result = soap_send___wsdd__Probe(soap, SOAP_MCAST_ADDR, NULL, &req);        // 向组播地址广播Probe消息
    while (SOAP_OK == result)                                                   // 开始循环接收设备发送过来的消息
    {
        memset(&rep, 0x00, sizeof(rep));
        result = soap_recv___wsdd__ProbeMatches(soap, &rep);
        if (SOAP_OK == result) {
            if (soap->error) {
                soap_perror(soap, "ProbeMatches");
            } else {                                                            // 成功接收到设备的应答消息
                dump__wsdd__ProbeMatches(&rep);

                if (NULL != rep.wsdd__ProbeMatches) {
                    count += rep.wsdd__ProbeMatches->__sizeProbeMatch;
                    for(i = 0; i < rep.wsdd__ProbeMatches->__sizeProbeMatch; i++) {
                        probeMatch = rep.wsdd__ProbeMatches->ProbeMatch + i;
                        if (NULL != cb) {
                            cb(probeMatch->XAddrs);                             // 使用设备服务地址执行函数回调
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