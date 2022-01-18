#ifndef __ONVIF_ONVIF_DISCOVERY_H__
#define __ONVIF_ONVIF_DISCOVERY_H__

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

void ONVIF_DetectDevice(void (*cb)(char *DeviceXAddr));

#ifdef __cplusplus
}
#endif

#endif