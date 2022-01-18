

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "onvif_debug.h"
#include "onvif_comm.h"
#include "onvif_discovery.h"
#include "onvif_service.h"

#ifdef WIN32
#include <windows.h>
#endif

#define USERNAME        "admin"                                                 // 认证信息（用户名、密码）
#define PASSWORD        "admin"
char * dh_ipc = "http://192.168.151.166/onvif/device_service";

void cb_discovery(char *DeviceXAddr)
{
    int stmno = 0;                                                              // 码流序号，0为主码流，1为辅码流
    int profile_cnt = 0;                                                        // 设备配置文件个数
    struct tagProfile *profiles = NULL;                                         // 设备配置文件列表
    struct tagCapabilities capa;                                                // 设备能力信息

    ONVIF_GetCapabilities(DeviceXAddr, &capa, USERNAME, PASSWORD);                                  // 获取设备能力信息（获取媒体服务地址）

    profile_cnt = ONVIF_GetProfiles(DeviceXAddr, &profiles, USERNAME, PASSWORD);                    // 获取媒体配置信息（主/辅码流配置信息）

    if (profile_cnt > stmno) {
        struct tagVideoEncoderConfiguration venc;
        char *vencToken = profiles[stmno].venc.token;

        ONVIF_GetVideoEncoderConfigurationOptions(capa.MediaXAddr, vencToken, USERNAME, PASSWORD);  // 获取该码流支持的视频编码器参数选项集
    
        ONVIF_GetVideoEncoderConfiguration(capa.MediaXAddr, vencToken, USERNAME, PASSWORD);         // 获取该码流当前的视频编码器参数
    
        venc = profiles[stmno].venc;
        venc.Height = 576;
        venc.Width  = 704;
        ONVIF_SetVideoEncoderConfiguration(capa.MediaXAddr, &venc, USERNAME, PASSWORD);             // 设置该码流当前的视频编码器参数
    
        ONVIF_GetVideoEncoderConfiguration(capa.MediaXAddr, vencToken, USERNAME, PASSWORD);         // 观察是否修改成功
    }

    if (NULL != profiles) {
        ONVIF_ReleaseProfiles(profiles);
        profiles = NULL;
    }
}

int main(int argc, char **argv)
{
    //ONVIF_DetectDevice(cb_discovery);

    cb_discovery(dh_ipc);

    return 0;
}