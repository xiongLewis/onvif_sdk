

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

    char cmd[256];
    char uri[ONVIF_ADDRESS_SIZE] = {0};                                         // ������֤��Ϣ��URI��ַ
    char uri_auth[ONVIF_ADDRESS_SIZE + 50] = {0};                               // ������֤��Ϣ��URI��ַ

    ONVIF_GetCapabilities(DeviceXAddr, &capa, USERNAME, PASSWORD);                                  // ��ȡ�豸������Ϣ����ȡý������ַ��

    profile_cnt = ONVIF_GetProfiles(DeviceXAddr, &profiles, USERNAME, PASSWORD);                    // ��ȡý��������Ϣ����/������������Ϣ��

    if (profile_cnt > stmno) {
        ONVIF_GetSnapshotUri(capa.MediaXAddr, profiles[stmno].token, uri, sizeof(uri), USERNAME, PASSWORD); // ��ȡͼ��ץ��URI

        ONVIF_MakeUriWithAuth(uri, USERNAME, PASSWORD, uri_auth, sizeof(uri_auth)); // ���ɴ���֤��Ϣ��URI���е�IPCҪ����֤��
        sprintf(cmd, "wget -O out.jpeg '%s'", uri_auth);                        // ʹ��wget����ͼƬ
        system(cmd);
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