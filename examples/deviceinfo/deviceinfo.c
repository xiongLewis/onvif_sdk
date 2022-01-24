
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

#define USERNAME        "admin"                                                 // ��֤��Ϣ���û��������룩
#define PASSWORD        "admin"

/************************************************************************
**������ONVIF_GetDeviceInformation
**���ܣ���ȡ�豸������Ϣ
**������
        [in] DeviceXAddr - �豸�����ַ
**���أ�
        0�����ɹ�����0����ʧ��
**��ע��
************************************************************************/
int ONVIF_GetDeviceInformation(const char *DeviceXAddr)
{
    int result = 0;
    struct soap *soap = NULL;
    struct _tds__GetDeviceInformation           req;
    struct _tds__GetDeviceInformationResponse   rep;

    SOAP_ASSERT(NULL != DeviceXAddr);
    SOAP_ASSERT(NULL != (soap = ONVIF_soap_new(SOAP_SOCK_TIMEOUT)));

    ONVIF_SetAuthInfo(soap, USERNAME, PASSWORD);

    memset(&req, 0x00, sizeof(req));
    memset(&rep, 0x00, sizeof(rep));

    soap_set_sent_logfile(soap, "device_info_send.xml");
    soap_set_recv_logfile(soap, "device_info_recv.xml");

    result = soap_call___tds__GetDeviceInformation(soap, DeviceXAddr, NULL, &req, &rep);
    SOAP_CHECK_ERROR(result, soap, "GetDeviceInformation");

    dump_tds__GetDeviceInformationResponse(&rep);

EXIT:

    if (NULL != soap) {
        ONVIF_soap_delete(soap);
    }

    return result;
}

int main(int argc, char **argv)
{
    ONVIF_GetDeviceInformation("http://192.168.151.166/onvif/device_service");

    return 0;
}