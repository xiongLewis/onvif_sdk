

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

#define USERNAME        "admin"                                                 // ��֤��Ϣ���û��������룩
#define PASSWORD        "admin"
char * dh_ipc = "http://192.168.151.166/onvif/device_service";

void cb_discovery(char *DeviceXAddr)
{
    int stmno = 0;                                                              // ������ţ�0Ϊ��������1Ϊ������
    int profile_cnt = 0;                                                        // �豸�����ļ�����
    struct tagProfile *profiles = NULL;                                         // �豸�����ļ��б�
    struct tagCapabilities capa;                                                // �豸������Ϣ

    ONVIF_GetCapabilities(DeviceXAddr, &capa, USERNAME, PASSWORD);                                  // ��ȡ�豸������Ϣ����ȡý������ַ��

    profile_cnt = ONVIF_GetProfiles(DeviceXAddr, &profiles, USERNAME, PASSWORD);                    // ��ȡý��������Ϣ����/������������Ϣ��

    if (profile_cnt > stmno) {
        struct tagVideoEncoderConfiguration venc;
        char *vencToken = profiles[stmno].venc.token;

        ONVIF_GetVideoEncoderConfigurationOptions(capa.MediaXAddr, vencToken, USERNAME, PASSWORD);  // ��ȡ������֧�ֵ���Ƶ����������ѡ�
    
        ONVIF_GetVideoEncoderConfiguration(capa.MediaXAddr, vencToken, USERNAME, PASSWORD);         // ��ȡ��������ǰ����Ƶ����������
    
        venc = profiles[stmno].venc;
        venc.Height = 576;
        venc.Width  = 704;
        ONVIF_SetVideoEncoderConfiguration(capa.MediaXAddr, &venc, USERNAME, PASSWORD);             // ���ø�������ǰ����Ƶ����������
    
        ONVIF_GetVideoEncoderConfiguration(capa.MediaXAddr, vencToken, USERNAME, PASSWORD);         // �۲��Ƿ��޸ĳɹ�
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