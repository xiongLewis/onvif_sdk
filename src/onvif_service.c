
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
**������ONVIF_GetCapabilities
**���ܣ���ȡ�豸������Ϣ
**������
        [in] DeviceXAddr - �豸�����ַ
        [out] capa       - �����豸������Ϣ��Ϣ
**���أ�
        0�����ɹ�����0����ʧ��
**��ע��
    1). ��������Ҫ�Ĳ���֮һ��ý������ַ
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
**������ONVIF_GetProfiles
**���ܣ���ȡ�豸������Ƶ����������Ϣ
**������
        [in] MediaXAddr - ý������ַ
        [out] profiles  - ���ص��豸����Ƶ����������Ϣ�б�������������ʹ��free�ͷŸû���
**���أ�
        �����豸��֧�ֵ�����������ͨ������/������������ʹprofiles�б����
**��ע��
        1). ע�⣺һ���������������������԰�����Ƶ����Ƶ���ݣ�Ҳ���Խ���������Ƶ���ݡ�
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

    if (rep.__sizeProfiles > 0) {                                               // ���仺��
        (*profiles) = (struct tagProfile *)malloc(rep.__sizeProfiles * sizeof(struct tagProfile));
        SOAP_ASSERT(NULL != (*profiles));
        memset((*profiles), 0x00, rep.__sizeProfiles * sizeof(struct tagProfile));
    }

    for(i = 0; i < rep.__sizeProfiles; i++) {                                   // ��ȡ���������ļ���Ϣ�����������ĵģ�
        struct tt__Profile *ttProfile = &rep.Profiles[i];
        struct tagProfile *plst = &(*profiles)[i];

        if (NULL != ttProfile->token) {                                         // �����ļ�Token
            strncpy(plst->token, ttProfile->token, sizeof(plst->token) - 1);
        }

        if (NULL != ttProfile->VideoEncoderConfiguration) {                     // ��Ƶ������������Ϣ
            if (NULL != ttProfile->VideoEncoderConfiguration->token) {          // ��Ƶ������Token
                strncpy(plst->venc.token, ttProfile->VideoEncoderConfiguration->token, sizeof(plst->venc.token) - 1);
            }
            if (NULL != ttProfile->VideoEncoderConfiguration->Resolution) {     // ��Ƶ�������ֱ���
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
**������ONVIF_GetStreamUri
**���ܣ���ȡ�豸������ַ(RTSP)
**������
        [in]  MediaXAddr    - ý������ַ
        [in]  ProfileToken  - the media profile token
        [out] uri           - ���صĵ�ַ
        [in]  sizeuri       - ��ַ�����С
**���أ�
        0�����ɹ�����0����ʧ��
**��ע��
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