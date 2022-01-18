

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
    int stmno = 1;                                                              // 码流序号，0为主码流，1为辅码流
    int profile_cnt = 0;                                                        // 设备配置文件个数
    struct tagProfile *profiles = NULL;                                         // 设备配置文件列表
    struct tagCapabilities capa;                                                // 设备能力信息

    char uri[ONVIF_ADDRESS_SIZE] = {0};                                         // 不带认证信息的URI地址
    char uri_auth[ONVIF_ADDRESS_SIZE + 50] = {0};                               // 带有认证信息的URI地址

    ONVIF_GetCapabilities(DeviceXAddr, &capa, USERNAME, PASSWORD);                                  // 获取设备能力信息（获取媒体服务地址）

    profile_cnt = ONVIF_GetProfiles(DeviceXAddr, &profiles, USERNAME, PASSWORD);                    // 获取媒体配置信息（主/辅码流配置信息）

    if (profile_cnt > stmno) {
        ONVIF_GetStreamUri(capa.MediaXAddr, profiles[stmno].token, uri, sizeof(uri), USERNAME, PASSWORD); // 获取RTSP地址

        ONVIF_MakeUriWithAuth(uri, USERNAME, PASSWORD, uri_auth, sizeof(uri_auth)); // 生成带认证信息的URI（有的IPC要求认证）
        printf("%s\n\n", uri_auth);
        //open_rtsp(uri_auth);                                                    // 读取主码流的音视频数据
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