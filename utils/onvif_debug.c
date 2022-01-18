#include "onvif_debug.h"
#include "print_log.h"

static const char *dump_enum2str_OSDType(enum tt__OSDType t)
{
    switch(t) {
    case tt__OSDType__Text:     return "Text";
    case tt__OSDType__Image:    return "Image";
    case tt__OSDType__Extended: return "Extended";
    }
    return "unknown";
}

static const char *dump_enum2str_VideoEncoding(enum tt__VideoEncoding e)
{
    switch(e) {
    case tt__VideoEncoding__JPEG:  return "JPEG";
    case tt__VideoEncoding__MPEG4: return "MPEG4";
    case tt__VideoEncoding__H264:  return "H264";
    }
    return "unknown";
}

static const char *dump_enum2str_AudioEncoding(enum tt__AudioEncoding e)
{
    switch (e) {
    case tt__AudioEncoding__G711: return "G711";
    case tt__AudioEncoding__G726: return "G726";
    case tt__AudioEncoding__AAC:  return "AAC";
    }
    return "unknown";
}

static const char *dump_enum2str_H264Profile(enum tt__H264Profile e)
{
    switch (e) {
    case tt__H264Profile__Baseline: return "Baseline";
    case tt__H264Profile__Main:     return "Main";
    case tt__H264Profile__Extended: return "Extended";
    case tt__H264Profile__High:     return "High";
    }
    return "unknown";
}

static void log_level_struct_tt__IntRange(unsigned int level, struct tt__IntRange *range)
{
    if (NULL != range) {
        log_level_val(level, "Min: ", log_int,  &range->Min);
        log_level_val(level, "Max: ", log_int,  &range->Max);
    }
}

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

void dump_tds__GetDeviceInformationResponse(struct _tds__GetDeviceInformationResponse *rep)
{
    assert(NULL != rep);
    log_func_in;
    log_val("Manufacturer:       ", log_str, rep->Manufacturer);
    log_val("Model:              ", log_str, rep->Model);
    log_val("Serial Number:      ", log_str, rep->SerialNumber);
    log_val("Hardware Id:        ", log_str, rep->HardwareId);
    log_val("Firmware Version:   ", log_str, rep->FirmwareVersion);
    log_func_out;
}

void dump_tds__GetSystemDateAndTime(struct _tds__GetSystemDateAndTimeResponse *rep)
{
    struct tt__SystemDateTime *systm;

    assert(NULL != rep);
    assert(NULL != rep->SystemDateAndTime);
    systm = rep->SystemDateAndTime;

    log_func_in;
    log_val("DateTimeType:      ", log_int, &systm->DateTimeType);
    log_val("DaylightSavings:   ", log_int, &systm->DaylightSavings);
    if (NULL == systm->TimeZone) {
        log_val("TimeZone:          ", log_pos, systm->TimeZone);
    } else {
        log_val("TimeZone:          ", log_str, systm->TimeZone->TZ);
    }
    if (NULL == systm->UTCDateTime) {
        log_val("UTCDateTime:       ", log_pos, systm->UTCDateTime);
    } else {
        assert(NULL != systm->UTCDateTime->Date);
        assert(NULL != systm->UTCDateTime->Time);
        log_fmt("UTCDateTime:       %04d-%02d-%02d %02d:%02d:%02d \n",
            systm->UTCDateTime->Date->Year,
            systm->UTCDateTime->Date->Month,
            systm->UTCDateTime->Date->Day,
            systm->UTCDateTime->Time->Hour,
            systm->UTCDateTime->Time->Minute,
            systm->UTCDateTime->Time->Second);
    }
    if (NULL == systm->LocalDateTime) {
        log_val("LocalDateTime:     ", log_pos, systm->LocalDateTime);
    } else {
        assert(NULL != systm->LocalDateTime->Date);
        assert(NULL != systm->LocalDateTime->Time);
        log_fmt("LocalDateTime:     %04d-%02d-%02d %02d:%02d:%02d \n",
            systm->LocalDateTime->Date->Year,
            systm->LocalDateTime->Date->Month,
            systm->LocalDateTime->Date->Day,
            systm->LocalDateTime->Time->Hour,
            systm->LocalDateTime->Time->Minute,
            systm->LocalDateTime->Time->Second);
    }
    log_func_out;
}


void dump_tds__GetCapabilitiesResponse(struct _tds__GetCapabilitiesResponse *rep)
{
    assert(NULL != rep);
    log_func_in;
    log_level_val(0, "Capabilities: ",      log_pos, rep->Capabilities);
    if (NULL != rep->Capabilities) {
        log_level_val(1, "Analytics: ",     log_pos, rep->Capabilities->Analytics);
        if (NULL != rep->Capabilities->Analytics) {
            log_level_val(2, "XAddr: ",     log_str, rep->Capabilities->Analytics->XAddr);
        }

        log_level_val(1, "Device: ",        log_pos, rep->Capabilities->Device);
        if (NULL != rep->Capabilities->Device) {
            log_level_val(2, "XAddr: ",     log_str, rep->Capabilities->Device->XAddr);
        }

        log_level_val(1, "Events: ",        log_pos, rep->Capabilities->Events);
        if (NULL != rep->Capabilities->Events) {
            log_level_val(2, "XAddr: ",     log_str, rep->Capabilities->Events->XAddr);
        }

        log_level_val(1, "Imaging: ",       log_pos, rep->Capabilities->Imaging);
        if (NULL != rep->Capabilities->Imaging) {
            log_level_val(2, "XAddr: ",     log_str, rep->Capabilities->Imaging->XAddr);
        }

        log_level_val(1, "Media: ",         log_pos, rep->Capabilities->Media);
        if (NULL != rep->Capabilities->Media) {
            log_level_val(2, "XAddr: ",     log_str, rep->Capabilities->Media->XAddr);
        }

        log_level_val(1, "PTZ: ",           log_pos, rep->Capabilities->PTZ);
        if (NULL != rep->Capabilities->PTZ) {
            log_level_val(2, "XAddr: ",     log_str, rep->Capabilities->PTZ->XAddr);
        }

        log_level_val(1, "Extension: ",     log_pos, rep->Capabilities->Extension);
        if (NULL != rep->Capabilities->Extension) {
            log_level_val(2, "DeviceIO: ",  log_pos, rep->Capabilities->Extension->DeviceIO);
            if (NULL != rep->Capabilities->Extension->DeviceIO) {
                log_level_val(3, "XAddr: ", log_str, rep->Capabilities->Extension->DeviceIO->XAddr);
            }

            log_level_val(2, "Display: ",  log_pos, rep->Capabilities->Extension->Display);
            if (NULL != rep->Capabilities->Extension->Display) {
                log_level_val(3, "XAddr: ", log_str, rep->Capabilities->Extension->Display->XAddr);
            }

            log_level_val(2, "Recording: ",  log_pos, rep->Capabilities->Extension->Recording);
            if (NULL != rep->Capabilities->Extension->Recording) {
                log_level_val(3, "XAddr: ", log_str, rep->Capabilities->Extension->Recording->XAddr);
            }
            
            log_level_val(2, "Search: ",  log_pos, rep->Capabilities->Extension->Search);
            if (NULL != rep->Capabilities->Extension->Search) {
                log_level_val(3, "XAddr: ", log_str, rep->Capabilities->Extension->Search->XAddr);
            }

            log_level_val(2, "Replay: ",  log_pos, rep->Capabilities->Extension->Replay);
            if (NULL != rep->Capabilities->Extension->Replay) {
                log_level_val(3, "XAddr: ", log_str, rep->Capabilities->Extension->Replay->XAddr);
            }

            log_level_val(2, "Receiver: ",  log_pos, rep->Capabilities->Extension->Receiver);
            if (NULL != rep->Capabilities->Extension->Receiver) {
                log_level_val(3, "XAddr: ", log_str, rep->Capabilities->Extension->Receiver->XAddr);
            }

            log_level_val(2, "AnalyticsDevice: ",  log_pos, rep->Capabilities->Extension->AnalyticsDevice);
            if (NULL != rep->Capabilities->Extension->AnalyticsDevice) {
                log_level_val(3, "XAddr: ", log_str, rep->Capabilities->Extension->AnalyticsDevice->XAddr);
            }
        }
    }
    log_func_out;
}

void dump_trt__GetProfilesResponse(struct _trt__GetProfilesResponse *rep)
{
    int i;

    assert(NULL != rep);

    log_func_in;

    log_level_val(0, "sizeProfiles: ", log_int, &rep->__sizeProfiles);
    log_level_val(0, "Profiles: ",     log_pos,  rep->Profiles);
    for(i=0; i<rep->__sizeProfiles; i++) {
        struct tt__Profile *profiles = &rep->Profiles[i];
        log_level_fmt(1, "%d\n", i);
        log_level_val(2, "Name: ",                     log_str,  profiles->Name);
        log_level_val(2, "token: ",                    log_str,  profiles->token);
        log_level_val(2, "fixed: ",                    log_bool, profiles->fixed);
        log_level_val(2, "VideoSourceConfiguration: ", log_pos,  profiles->VideoSourceConfiguration);
        if (NULL != profiles->VideoSourceConfiguration) {
            struct tt__VideoSourceConfiguration *vsc = profiles->VideoSourceConfiguration;

            log_level_val(3, "Name: ",        log_str,  vsc->Name);
            log_level_val(3, "UseCount: ",    log_int, &vsc->UseCount);
            log_level_val(3, "token: ",       log_str,  vsc->token);
            log_level_val(3, "SourceToken: ", log_str,  vsc->SourceToken);
            log_level_val(3, "Bounds: ",      log_pos,  vsc->Bounds);
            if (NULL != vsc->Bounds) {
                log_level_val(4, "x: ",       log_int, &vsc->Bounds->x);
                log_level_val(4, "y: ",       log_int, &vsc->Bounds->y);
                log_level_val(4, "width: ",   log_int, &vsc->Bounds->width);
                log_level_val(4, "height: ",  log_int, &vsc->Bounds->height);
            }
        }
        log_level_val(2, "VideoEncoderConfiguration: ", log_pos, profiles->VideoEncoderConfiguration);
        if (NULL != profiles->VideoEncoderConfiguration) {
            struct tt__VideoEncoderConfiguration *vec = profiles->VideoEncoderConfiguration;
            log_level_val(3, "Name: ",       log_str,    vec->Name);
            log_level_val(3, "UseCount: ",   log_int,   &vec->UseCount);
            log_level_val(3, "token: ",      log_str,    vec->token);
            log_level_val(3, "Encoding: ",   log_str,    dump_enum2str_VideoEncoding(vec->Encoding));
            log_level_val(3, "Quality: ",    log_float, &vec->Quality);
            log_level_val(3, "Resolution: ", log_pos,    vec->Resolution);
            if (NULL != vec->Resolution) {
                log_level_val(4, "Width: ",  log_int,   &vec->Resolution->Width);
                log_level_val(4, "Height: ", log_int,   &vec->Resolution->Height);
            }
            log_level_val(3, "RateControl: ",log_pos,    vec->RateControl);
            if (NULL !=  vec->RateControl) {
                log_level_val(4, "FrameRateLimit: ",   log_int, &vec->RateControl->FrameRateLimit);
                log_level_val(4, "EncodingInterval: ", log_int, &vec->RateControl->EncodingInterval);
                log_level_val(4, "BitrateLimit: ",     log_int, &vec->RateControl->BitrateLimit);
            }
        }
        log_level_val(2, "AudioSourceConfiguration: ", log_pos, profiles->AudioSourceConfiguration);
        if (NULL != profiles->AudioSourceConfiguration) {
            struct tt__AudioSourceConfiguration *asc = profiles->AudioSourceConfiguration;
            log_level_val(3, "Name: ",        log_str,  asc->Name);
            log_level_val(3, "UseCount: ",    log_int, &asc->UseCount);
            log_level_val(3, "token: ",       log_str,  asc->token);
            log_level_val(3, "SourceToken: ", log_str,  asc->SourceToken);
        }
        log_level_val(2, "AudioEncoderConfiguration: ", log_pos, profiles->AudioEncoderConfiguration);
        if (NULL != profiles->AudioEncoderConfiguration) {
            struct tt__AudioEncoderConfiguration *aec = profiles->AudioEncoderConfiguration;
            log_level_val(3, "Name: ",       log_str,  aec->Name);
            log_level_val(3, "UseCount: ",   log_int, &aec->UseCount);
            log_level_val(3, "token: ",      log_str,  aec->token);
            log_level_val(3, "Bitrate: ",    log_int, &aec->Bitrate);
            log_level_val(3, "SampleRate: ", log_int, &aec->SampleRate);
            log_level_val(3, "Encoding: ",   log_str,  dump_enum2str_AudioEncoding(aec->Encoding));
        }

    }
    log_func_out;
}

void dump_trt__GetStreamUriResponse(struct _trt__GetStreamUriResponse *rep)
{
    assert(NULL != rep);
    log_func_in;
    log_level_val(0, "MediaUri: ", log_pos, rep->MediaUri);
    if (NULL != rep->MediaUri) {
        log_level_val(1, "Uri:                 ", log_str,    rep->MediaUri->Uri);
        log_level_val(1, "InvalidAfterConnect: ", log_bool,  &rep->MediaUri->InvalidAfterConnect);
        log_level_val(1, "InvalidAfterReboot:  ", log_bool,  &rep->MediaUri->InvalidAfterReboot);
        log_level_val(1, "Timeout:             ", log_int64, &rep->MediaUri->Timeout);
    }
    log_func_out;
}

void dump_trt__GetSnapshotUriResponse(struct _trt__GetSnapshotUriResponse *rep)
{
    assert(NULL != rep);

    log_func_in;

    log_level_val(0, "MediaUri: ",                log_pos,    rep->MediaUri);
    if (NULL != rep->MediaUri) {
        log_level_val(1, "Uri: ",                 log_str,    rep->MediaUri->Uri);
        log_level_val(1, "InvalidAfterConnect: ", log_bool,  &rep->MediaUri->InvalidAfterConnect);
        log_level_val(1, "InvalidAfterReboot: ",  log_bool,  &rep->MediaUri->InvalidAfterReboot);
        log_level_val(1, "Timeout: ",             log_int64, &rep->MediaUri->Timeout);
    }

    log_func_out;
}

void dump_trt__GetVideoEncoderConfigurationOptionsResponse(struct _trt__GetVideoEncoderConfigurationOptionsResponse *rep)
{
    int i;
    assert(NULL != rep);

    log_func_in;

    log_level_val(0, "Options: ",                           log_pos,    rep->Options);
    if (NULL != rep->Options) {
        log_level_val(1, "QualityRange: ",                  log_pos,    rep->Options->QualityRange);
        log_level_struct_tt__IntRange(2,                                rep->Options->QualityRange);
        log_level_val(1, "JPEG: ",                          log_pos,    rep->Options->JPEG);
        log_level_val(1, "MPEG4: ",                         log_pos,    rep->Options->MPEG4);
        log_level_val(1, "H264: ",                          log_pos,    rep->Options->H264);
        if (NULL != rep->Options->H264) {
            struct tt__H264Options *pH264 = rep->Options->H264;
            log_level_val(2, "sizeResolutionsAvailable: ",  log_int,   &pH264->__sizeResolutionsAvailable);
            log_level_val(2, "ResolutionsAvailable: ",      log_pos,    pH264->ResolutionsAvailable);
            for (i = 0; i < pH264->__sizeResolutionsAvailable; i++) {
                log_level_fmt(3, "%d * %d\n",                           pH264->ResolutionsAvailable[i].Width,
                                                                        pH264->ResolutionsAvailable[i].Height);
            }
            log_level_val(2, "GovLengthRange: ",            log_pos,    pH264->GovLengthRange);
            log_level_struct_tt__IntRange(3,                            pH264->GovLengthRange);
            log_level_val(2, "FrameRateRange: ",            log_pos,    pH264->FrameRateRange);
            log_level_struct_tt__IntRange(3,                            pH264->FrameRateRange);
            log_level_val(2, "EncodingIntervalRange: ",     log_pos,    pH264->EncodingIntervalRange);
            log_level_struct_tt__IntRange(3,                            pH264->EncodingIntervalRange);
            log_level_val(2, "sizeH264ProfilesSupported: ", log_int,   &pH264->__sizeH264ProfilesSupported);
            log_level_val(2, "H264ProfilesSupported: ",     log_pos,   &pH264->H264ProfilesSupported);
            for (i = 0; i < pH264->__sizeH264ProfilesSupported; i++) {
                log_level_fmt(3, "%s\n",                                dump_enum2str_H264Profile(pH264->H264ProfilesSupported[i]));
            }
        }
        log_level_val(1, "Extension: ",                     log_pos,    rep->Options->Extension);
    }

    log_func_out;
}

static void log_level_struct_tt__VideoEncoderConfiguration(unsigned int level, struct tt__VideoEncoderConfiguration *vecfg)
{
    if (NULL == vecfg) {
        return;
    }

    log_level_val(level, "Name: ",                          log_str,    vecfg->Name);
    log_level_val(level, "UseCount: ",                      log_int,   &vecfg->UseCount);
    log_level_val(level, "token: ",                         log_str,    vecfg->token);
    log_level_val(level, "Encoding: ",                      log_str,    dump_enum2str_VideoEncoding(vecfg->Encoding));
    log_level_val(level, "Resolution: ",                    log_pos,    vecfg->Resolution);
    if (NULL != vecfg->Resolution) {
        log_level_val(level + 1, "Width: ",                 log_int,   &vecfg->Resolution->Width);
        log_level_val(level + 1, "Height: ",                log_int,   &vecfg->Resolution->Height);
    }
    log_level_val(level, "Quality: ",                       log_float, &vecfg->Quality);
    log_level_val(level, "RateControl: ",                   log_pos,    vecfg->RateControl);
    if (NULL !=  vecfg->RateControl) {
        log_level_val(level + 1, "FrameRateLimit: ",        log_int,   &vecfg->RateControl->FrameRateLimit);
        log_level_val(level + 1, "EncodingInterval: ",      log_int,   &vecfg->RateControl->EncodingInterval);
        log_level_val(level + 1, "BitrateLimit: ",          log_int,   &vecfg->RateControl->BitrateLimit);
    }
    log_level_val(level, "MPEG4: ",                         log_pos,    vecfg->MPEG4);
    log_level_val(level, "H264: ",                          log_pos,    vecfg->H264);
    log_level_val(level, "Multicast: ",                     log_pos,    vecfg->Multicast);
    log_level_val(level, "SessionTimeout: ",                log_int64, &vecfg->SessionTimeout);
}

void dump_trt__GetVideoEncoderConfigurationResponse(struct _trt__GetVideoEncoderConfigurationResponse *rep)
{
    assert(NULL != rep);

    log_func_in;
    log_level_struct_tt__VideoEncoderConfiguration(1, rep->Configuration);
    log_func_out;
}