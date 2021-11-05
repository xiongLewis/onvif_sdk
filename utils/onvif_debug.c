#include "onvif_debug.h"
#include "print_log.h"

void soap_perror(struct soap *soap, const char *str)
{
    if (NULL == str) {
        SOAP_DBGERR("[soap] error: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap));
    } else {
        SOAP_DBGERR("[soap] %s error: %d, %s, %s\n", str, soap->error, *soap_faultcode(soap), *soap_faultstring(soap));
    }
    return;
}

static void log_level_struct_wsa__EndpointReferenceType(unsigned int level, struct wsa__EndpointReferenceType *a)
{
    int i;

    if (NULL == a) {
        return;
    }

    log_level_val(level, "Address: ",               log_str,    a->Address);
    log_level_val(level, "ReferenceProperties: ",   log_pos,    a->ReferenceProperties);
    if (NULL != a->ReferenceProperties) {
        log_level_val(level + 1, "__size: ",        log_int,   &a->ReferenceProperties->__size);
        log_level_val(level + 1, "__any: ",         log_pos,    a->ReferenceProperties->__any);
        for (i = 0; i < a->ReferenceProperties->__size; i++) {
            log_level_val(level + 2, "",            log_str,    a->ReferenceProperties->__any[i]);
        }
    }
    log_level_val(level, "ReferenceParameters: ",   log_pos,    a->ReferenceParameters);
    if (NULL != a->ReferenceParameters) {
        log_level_val(level + 1, "__size: ",        log_int,   &a->ReferenceParameters->__size);
        log_level_val(level + 1, "__any: ",         log_pos,    a->ReferenceParameters->__any);
        for (i = 0; i < a->ReferenceParameters->__size; i++) {
            log_level_val(level + 2, "",            log_str,    a->ReferenceParameters->__any[i]);
        }
    }
    log_level_val(level, "PortType: ",              log_strp,   a->PortType);
    log_level_val(level, "ServiceName: ",           log_pos,    a->ServiceName);
    if (NULL != a->ServiceName) {
        log_level_val(level + 1, "ServiceName: ",   log_str,    a->ServiceName->__item);
        log_level_val(level + 1, "PortName: ",      log_str,    a->ServiceName->PortName);
        log_level_val(level + 1, "__anyAttribute: ",log_str,    a->ServiceName->__anyAttribute);
    }
    log_level_val(level, "__size: ",                log_int,   &a->__size);
    log_level_val(level, "__any: ",                 log_pos,    a->__any);
    for (i = 0; i < a->__size; i++) {
        log_level_val(level + 1, "",                log_str,    a->__any[i]);
    }
    log_level_val(level, "__anyAttribute: ",        log_str,    a->__anyAttribute);
}

void dump__wsdd__ProbeMatches(struct __wsdd__ProbeMatches *rep)
{
    int i;
    struct wsdd__ProbeMatchType* probeMatch;

    assert(NULL != rep);
    log_func_in;
    log_level_val(0, "wsdd__ProbeMatches: ",                log_pos,    rep->wsdd__ProbeMatches);
    if (NULL != rep->wsdd__ProbeMatches) {
        log_level_val(1, "__sizeProbeMatch: ",              log_int,   &rep->wsdd__ProbeMatches->__sizeProbeMatch);
        log_level_val(1, "ProbeMatch: ",                    log_pos,   &rep->wsdd__ProbeMatches->ProbeMatch);
        for(i = 0; i < rep->wsdd__ProbeMatches->__sizeProbeMatch; i++) {
            probeMatch = rep->wsdd__ProbeMatches->ProbeMatch + i;
            log_level_fmt(2, "%d\n", i);
            log_level_val(3, "wsa__EndpointReference: ",    log_pos,   &probeMatch->wsa__EndpointReference);
            log_level_struct_wsa__EndpointReferenceType(4,             &probeMatch->wsa__EndpointReference);
            log_level_val(3, "Types: ",                     log_str,    probeMatch->Types);
            log_level_val(3, "Scopes: ",                    log_pos,    probeMatch->Scopes);
            if (NULL != probeMatch->Scopes) {
                log_level_val(4, "__item: ",                log_str,    probeMatch->Scopes->__item);
                log_level_val(4, "MatchBy: ",               log_str,    probeMatch->Scopes->MatchBy);
            }
            log_level_val(3, "XAddrs: ",                    log_str,    probeMatch->XAddrs);
            log_level_val(3, "MetadataVersion: ",           log_int,   &probeMatch->MetadataVersion);
        }
    }
    log_func_out;
}