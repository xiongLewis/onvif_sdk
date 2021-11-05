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

void* ONVIF_soap_malloc(struct soap *soap, unsigned int n);
struct soap *ONVIF_soap_new(int timeout);
void ONVIF_soap_delete(struct soap *soap);

#ifdef __cplusplus
}
#endif

#endif