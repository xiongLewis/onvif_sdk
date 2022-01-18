#ifndef __ONVIF_ONVIF_SERVICE_H__
#define __ONVIF_ONVIF_SERVICE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include "soapH.h"
#include "stdsoap2.h"
#include "soapStub.h"

#ifdef __cplusplus
extern "C" {
#endif
/* �豸������Ϣ */
struct tagCapabilities {
    char MediaXAddr[ONVIF_ADDRESS_SIZE];                                        // ý������ַ
    char EventXAddr[ONVIF_ADDRESS_SIZE];                                        // �¼������ַ
                                                                                // ������������ַ�Ͳ��г���
};

/* ��Ƶ������������Ϣ */
struct tagVideoEncoderConfiguration
{
    char token[ONVIF_TOKEN_SIZE];                                               // Ψһ��ʶ����Ƶ�������������ַ���
    int Width;                                                                  // �ֱ���
    int Height;
};

/* �豸������Ϣ */
struct tagProfile {
    char token[ONVIF_TOKEN_SIZE];                                               // Ψһ��ʶ�豸�����ļ��������ַ���

    struct tagVideoEncoderConfiguration venc;                                   // ��Ƶ������������Ϣ
};



int ONVIF_GetCapabilities(const char *DeviceXAddr, struct tagCapabilities *capa, const char *username, const char *password);
int ONVIF_GetProfiles(const char *MediaXAddr, struct tagProfile **profiles, const char *username, const char *password);
int ONVIF_ReleaseProfiles(struct tagProfile *profiles);
int ONVIF_GetStreamUri(const char *MediaXAddr, char *ProfileToken, char *uri, unsigned int sizeuri, const char *username, const char *password);
int ONVIF_GetSnapshotUri(const char *MediaXAddr, char *ProfileToken, char *uri, unsigned int sizeuri, const char *username, const char *password);
int ONVIF_GetVideoEncoderConfigurationOptions(const char *MediaXAddr, char *ConfigurationToken, const char *username, const char *password);
int ONVIF_GetVideoEncoderConfiguration(const char *MediaXAddr, char *ConfigurationToken, const char *username, const char *password);
int ONVIF_SetVideoEncoderConfiguration(const char *MediaXAddr, struct tagVideoEncoderConfiguration *venc, const char *username, const char *password);

#ifdef __cplusplus
}
#endif

#endif