
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
#include "onvif_discovery.h"
#include "onvif_service.h"

/************************************************************************
**函数：ONVIF_GetCapabilities
**功能：获取设备能力信息
**参数：
        [in] DeviceXAddr - 设备服务地址
        [out] capa       - 返回设备能力信息信息
**返回：
        0表明成功，非0表明失败
**备注：
    1). 其中最主要的参数之一是媒体服务地址
************************************************************************/
int ONVIF_GetCapabilities(const char *DeviceXAddr, struct tagCapabilities *capa, const char *username, const char *password)
{
    int result = 0;
    struct soap *soap = NULL;
    struct _tds__GetCapabilities            req;
    struct _tds__GetCapabilitiesResponse    rep;

    SOAP_ASSERT(NULL != DeviceXAddr);
    SOAP_ASSERT(NULL != capa);
    SOAP_ASSERT(NULL != (soap = ONVIF_soap_new(SOAP_SOCK_TIMEOUT)));

    ONVIF_SetAuthInfo(soap, username, password);

    memset(&req, 0x00, sizeof(req));
    memset(&rep, 0x00, sizeof(rep));
    result = soap_call___tds__GetCapabilities(soap, DeviceXAddr, NULL, &req, &rep);
    SOAP_CHECK_ERROR(result, soap, "GetCapabilities");

    dump_tds__GetCapabilitiesResponse(&rep);

    memset(capa, 0x00, sizeof(struct tagCapabilities));
    if (NULL != rep.Capabilities) {
        if (NULL != rep.Capabilities->Media) {
            if (NULL != rep.Capabilities->Media->XAddr) {
                strncpy(capa->MediaXAddr, rep.Capabilities->Media->XAddr, sizeof(capa->MediaXAddr) - 1);
            }
        }
        if (NULL != rep.Capabilities->Events) {
            if (NULL != rep.Capabilities->Events->XAddr) {
                strncpy(capa->EventXAddr, rep.Capabilities->Events->XAddr, sizeof(capa->EventXAddr) - 1);
            }
        }
    }

EXIT:

    if (NULL != soap) {
        ONVIF_soap_delete(soap);
    }

    return result;
}

/************************************************************************
**函数：ONVIF_GetProfiles
**功能：获取设备的音视频码流配置信息
**参数：
        [in] MediaXAddr - 媒体服务地址
        [out] profiles  - 返回的设备音视频码流配置信息列表，调用者有责任使用free释放该缓存
**返回：
        返回设备可支持的码流数量（通常是主/辅码流），即使profiles列表个数
**备注：
        1). 注意：一个码流（如主码流）可以包含视频和音频数据，也可以仅仅包含视频数据。
************************************************************************/
int ONVIF_GetProfiles(const char *MediaXAddr, struct tagProfile **profiles, const char *username, const char *password)
{
    int i = 0;
    int result = 0;
    struct soap *soap = NULL;
    struct _trt__GetProfiles            req;
    struct _trt__GetProfilesResponse    rep;

    SOAP_ASSERT(NULL != MediaXAddr);
    SOAP_ASSERT(NULL != (soap = ONVIF_soap_new(SOAP_SOCK_TIMEOUT)));

    ONVIF_SetAuthInfo(soap, username, password);

    memset(&req, 0x00, sizeof(req));
    memset(&rep, 0x00, sizeof(rep));
    result = soap_call___trt__GetProfiles(soap, MediaXAddr, NULL, &req, &rep);
    SOAP_CHECK_ERROR(result, soap, "GetProfiles");

    dump_trt__GetProfilesResponse(&rep);

    if (rep.__sizeProfiles > 0) {                                               // 分配缓存
        (*profiles) = (struct tagProfile *)malloc(rep.__sizeProfiles * sizeof(struct tagProfile));
        SOAP_ASSERT(NULL != (*profiles));
        memset((*profiles), 0x00, rep.__sizeProfiles * sizeof(struct tagProfile));
    }

    for(i = 0; i < rep.__sizeProfiles; i++) {                                   // 提取所有配置文件信息（我们所关心的）
        struct tt__Profile *ttProfile = &rep.Profiles[i];
        struct tagProfile *plst = &(*profiles)[i];

        if (NULL != ttProfile->token) {                                         // 配置文件Token
            strncpy(plst->token, ttProfile->token, sizeof(plst->token) - 1);
        }

        if (NULL != ttProfile->VideoEncoderConfiguration) {                     // 视频编码器配置信息
            if (NULL != ttProfile->VideoEncoderConfiguration->token) {          // 视频编码器Token
                strncpy(plst->venc.token, ttProfile->VideoEncoderConfiguration->token, sizeof(plst->venc.token) - 1);
            }
            if (NULL != ttProfile->VideoEncoderConfiguration->Resolution) {     // 视频编码器分辨率
                plst->venc.Width  = ttProfile->VideoEncoderConfiguration->Resolution->Width;
                plst->venc.Height = ttProfile->VideoEncoderConfiguration->Resolution->Height;
            }
        }
    }

EXIT:

    if (NULL != soap) {
        ONVIF_soap_delete(soap);
    }

    return rep.__sizeProfiles;
}

int ONVIF_ReleaseProfiles(struct tagProfile *profiles)
{
    if (NULL != profiles) {
        free(profiles);
        profiles = NULL;
    }

    return 0;
}

/************************************************************************
**函数：ONVIF_GetStreamUri
**功能：获取设备码流地址(RTSP)
**参数：
        [in]  MediaXAddr    - 媒体服务地址
        [in]  ProfileToken  - the media profile token
        [out] uri           - 返回的地址
        [in]  sizeuri       - 地址缓存大小
**返回：
        0表明成功，非0表明失败
**备注：
************************************************************************/
int ONVIF_GetStreamUri(const char *MediaXAddr, char *ProfileToken, char *uri, unsigned int sizeuri, const char *username, const char *password)
{
    int result = 0;
    struct soap *soap = NULL;
    struct tt__StreamSetup              ttStreamSetup;
    struct tt__Transport                ttTransport;
    struct _trt__GetStreamUri           req;
    struct _trt__GetStreamUriResponse   rep;

    SOAP_ASSERT(NULL != MediaXAddr);
    SOAP_ASSERT(NULL != uri);
    memset(uri, 0x00, sizeuri);

    SOAP_ASSERT(NULL != (soap = ONVIF_soap_new(SOAP_SOCK_TIMEOUT)));

    memset(&req, 0x00, sizeof(req));
    memset(&rep, 0x00, sizeof(rep));
    memset(&ttStreamSetup, 0x00, sizeof(ttStreamSetup));
    memset(&ttTransport, 0x00, sizeof(ttTransport));
    ttStreamSetup.Stream                = tt__StreamType__RTP_Unicast;
    ttStreamSetup.Transport             = &ttTransport;
    ttStreamSetup.Transport->Protocol   = tt__TransportProtocol__RTSP;
    ttStreamSetup.Transport->Tunnel     = NULL;
    req.StreamSetup                     = &ttStreamSetup;
    req.ProfileToken                    = ProfileToken;

    ONVIF_SetAuthInfo(soap, username, password);
    result = soap_call___trt__GetStreamUri(soap, MediaXAddr, NULL, &req, &rep);
    SOAP_CHECK_ERROR(result, soap, "GetServices");

    dump_trt__GetStreamUriResponse(&rep);

    result = -1;
    if (NULL != rep.MediaUri) {
        if (NULL != rep.MediaUri->Uri) {
            if (sizeuri > strlen(rep.MediaUri->Uri)) {
                strcpy(uri, rep.MediaUri->Uri);
                result = 0;
            } else {
                SOAP_DBGERR("Not enough cache!\n");
            }
        }
    }

EXIT:

    if (NULL != soap) {
        ONVIF_soap_delete(soap);
    }

    return result;
}