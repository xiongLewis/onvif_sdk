#ifndef __ONVIF_ONVIF_COMM_H__
#define __ONVIF_ONVIF_COMM_H__

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

#define SOAP_ASSERT     assert

#define SOAP_CHECK_ERROR(result, soap, str) \
    do { \
        if (SOAP_OK != (result) || SOAP_OK != (soap)->error) { \
            soap_perror((soap), (str)); \
            if (SOAP_OK == (result)) { \
                (result) = (soap)->error; \
            } \
            goto EXIT; \
        } \
    } while (0)

void* ONVIF_soap_malloc(struct soap *soap, unsigned int n);
struct soap *ONVIF_soap_new(int timeout);
void ONVIF_soap_delete(struct soap *soap);

int ONVIF_SetAuthInfo(struct soap *soap, const char *username, const char *password);

#ifdef __cplusplus
}
#endif

#endif