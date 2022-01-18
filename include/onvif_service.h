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
/* 设备能力信息 */
struct tagCapabilities {
    char MediaXAddr[ONVIF_ADDRESS_SIZE];                                        // 媒体服务地址
    char EventXAddr[ONVIF_ADDRESS_SIZE];                                        // 事件服务地址
                                                                                // 其他服务器地址就不列出了
};

/* 视频编码器配置信息 */
struct tagVideoEncoderConfiguration
{
    char token[ONVIF_TOKEN_SIZE];                                               // 唯一标识该视频编码器的令牌字符串
    int Width;                                                                  // 分辨率
    int Height;
};

/* 设备配置信息 */
struct tagProfile {
    char token[ONVIF_TOKEN_SIZE];                                               // 唯一标识设备配置文件的令牌字符串

    struct tagVideoEncoderConfiguration venc;                                   // 视频编码器配置信息
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