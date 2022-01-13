

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "onvif_debug.h"
#include "onvif_comm.h"
#include "onvif_discovery.h"

#ifdef WIN32
#include <windows.h>
#endif

#define USERNAME        "admin"                                                 // ��֤��Ϣ���û��������룩
#define PASSWORD        "admin"

/************************************************************************
**������ONVIF_GetHostTimeZone
**���ܣ���ȡ������ʱ����Ϣ
**������
        [out] TZ    - ���ص�ʱ����Ϣ
        [in] sizeTZ - TZ�����С
**���أ���
**��ע��
************************************************************************/
static void ONVIF_GetHostTimeZone(char *TZ, int sizeTZ)
{
    char timezone[20] = {0};

#ifdef WIN32

    TIME_ZONE_INFORMATION TZinfo;
    GetTimeZoneInformation(&TZinfo);
    sprintf(timezone, "GMT%c%02d:%02d",  (TZinfo.Bias <= 0) ? '+' : '-', labs(TZinfo.Bias) / 60, labs(TZinfo.Bias) % 60);

#else

    FILE *fp = NULL;
    char time_fmt[32] = {0};

    fp = popen("date +%z", "r");
    fread(time_fmt, sizeof(time_fmt), 1, fp);
    pclose(fp);

    if( ((time_fmt[0] == '+') || (time_fmt[0] == '-')) &&
        isdigit(time_fmt[1]) && isdigit(time_fmt[2]) && isdigit(time_fmt[3]) && isdigit(time_fmt[4]) ) {
            sprintf(timezone, "GMT%c%c%c:%c%c", time_fmt[0], time_fmt[1], time_fmt[2], time_fmt[3], time_fmt[4]);
    } else {
        strcpy(timezone, "GMT+08:00");
    }

#endif

    if (sizeTZ > strlen(timezone)) {
        strcpy(TZ, timezone);
    }

    return;
}

/************************************************************************
**������ONVIF_GetSystemDateAndTime
**���ܣ���ȡ�豸��ϵͳʱ��
**������
        [in] DeviceXAddr - �豸�����ַ
**���أ�
        0�����ɹ�����0����ʧ��
**��ע��
    1). ����IPC����ͷ��OSD��ӡ��ʱ������LocalDateTime
************************************************************************/
int ONVIF_GetSystemDateAndTime(const char *DeviceXAddr)
{
    int result = 0;
    struct soap *soap = NULL;
    struct _tds__GetSystemDateAndTime         req;
    struct _tds__GetSystemDateAndTimeResponse rep;

    SOAP_ASSERT(NULL != DeviceXAddr);

    SOAP_ASSERT(NULL != (soap = ONVIF_soap_new(SOAP_SOCK_TIMEOUT)));

    ONVIF_SetAuthInfo(soap, USERNAME, PASSWORD);

    memset(&req, 0x00, sizeof(req));
    memset(&rep, 0x00, sizeof(rep));
    result = soap_call___tds__GetSystemDateAndTime(soap, DeviceXAddr, NULL, &req, &rep);
    SOAP_CHECK_ERROR(result, soap, "GetSystemDateAndTime");

    dump_tds__GetSystemDateAndTime(&rep);

EXIT:

    if (NULL != soap) {
        ONVIF_soap_delete(soap);
    }

    return result;
}

/************************************************************************
**������ONVIF_SetSystemDateAndTime
**���ܣ����ݿͻ���������ǰʱ�䣬УʱIPC��ϵͳʱ��
**������
        [in] DeviceXAddr - �豸�����ַ
**���أ�
        0�����ɹ�����0����ʧ��
**��ע��
    1). ����IPC����ͷ��OSD��ӡ��ʱ�����䱾��ʱ�䣨����ʱ���ʱ��ϢϢ��أ���
        ����ʱ��ʱһ��Ҫע��ʱ������ȷ�ԡ�
************************************************************************/
int ONVIF_SetSystemDateAndTime(const char *DeviceXAddr)
{
    int result = 0;
    struct soap *soap = NULL;
    struct _tds__SetSystemDateAndTime           req;
    struct _tds__SetSystemDateAndTimeResponse   rep;

    char TZ[20];                                                                // ���ڻ�ȡ�ͻ���������ʱ����Ϣ����"GMT+08:00"��
    time_t t;                                                                   // ���ڻ�ȡ�ͻ���������UTCʱ��
    struct tm tm;

    SOAP_ASSERT(NULL != DeviceXAddr);
    SOAP_ASSERT(NULL != (soap = ONVIF_soap_new(SOAP_SOCK_TIMEOUT)));

    ONVIF_GetHostTimeZone(TZ, DIM(TZ));                                         // ��ȡ�ͻ���������ʱ����Ϣ

    t = time(NULL);                                                             // ��ȡ�ͻ���������UTCʱ��
#ifdef WIN32
    gmtime_s(&tm, &t);
#else
    gmtime_r(&t, &tm);
#endif

    memset(&req, 0x00, sizeof(req));
    memset(&rep, 0x00, sizeof(rep));
    req.DateTimeType      = tt__SetDateTimeType__Manual;
    req.DaylightSavings   = xsd__boolean__false_;
    req.TimeZone          = (struct tt__TimeZone *)ONVIF_soap_malloc(soap, sizeof(struct tt__TimeZone));
    req.UTCDateTime       = (struct tt__DateTime *)ONVIF_soap_malloc(soap, sizeof(struct tt__DateTime));
    req.UTCDateTime->Date = (struct tt__Date *)ONVIF_soap_malloc(soap, sizeof(struct tt__Date));
    req.UTCDateTime->Time = (struct tt__Time *)ONVIF_soap_malloc(soap, sizeof(struct tt__Time));

    req.TimeZone->TZ              = TZ;                                         // ���ñ���ʱ����IPC��OSD��ʾ��ʱ����Ǳ���ʱ�䣩
    req.UTCDateTime->Date->Year   = tm.tm_year + 1900;                          // ����UTCʱ�䣨ע�ⲻ�Ǳ���ʱ�䣩
    req.UTCDateTime->Date->Month  = tm.tm_mon + 1;
    req.UTCDateTime->Date->Day    = tm.tm_mday;
    req.UTCDateTime->Time->Hour   = tm.tm_hour;
    req.UTCDateTime->Time->Minute = tm.tm_min;
    req.UTCDateTime->Time->Second = tm.tm_sec;

	printf("TZ=%s, dy=%d, dm=%d, dd=%d, th=%d, tm=%d, ts=%d\n",
		req.TimeZone->TZ     ,        
		req.UTCDateTime->Date->Year , 
		req.UTCDateTime->Date->Month ,
		req.UTCDateTime->Date->Day   ,
		req.UTCDateTime->Time->Hour  ,
		req.UTCDateTime->Time->Minute,
		req.UTCDateTime->Time->Second
	);

    ONVIF_SetAuthInfo(soap, USERNAME, PASSWORD);
    result = soap_call___tds__SetSystemDateAndTime(soap, DeviceXAddr, NULL, &req, &rep);
    SOAP_CHECK_ERROR(result, soap, "SetSystemDateAndTime");

EXIT:

    if (NULL != soap) {
        ONVIF_soap_delete(soap);
    }

    return result;
}

void cb_discovery(char *DeviceXAddr)
{
    printf("DeviceXAddr = %s\n", DeviceXAddr);
    ONVIF_GetSystemDateAndTime(DeviceXAddr);
    ONVIF_SetSystemDateAndTime(DeviceXAddr);
    ONVIF_GetSystemDateAndTime(DeviceXAddr);
}

char * dh_ipc = "http://192.168.151.166/onvif/device_service";

int main(int argc, char **argv)
{
    //ONVIF_DetectDevice(cb_discovery);

    cb_discovery(dh_ipc);

    return 0;
}