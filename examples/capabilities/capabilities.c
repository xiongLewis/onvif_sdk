

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
    struct tagCapabilities capa;
    
    printf("DeviceXAddr = %s\n", DeviceXAddr);
    ONVIF_GetCapabilities(DeviceXAddr, &capa, USERNAME, PASSWORD);
}

int main(int argc, char **argv)
{
    //ONVIF_DetectDevice(cb_discovery);

    cb_discovery(dh_ipc);

    return 0;
}