/*
*  Copyright (C) 2014 MediaTek Inc.
*
*  Modification based on code covered by the below mentioned copyright
*  and/or permission notice(s).
*/


/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MTK_RIL_H
#define MTK_RIL_H 1

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <telephony/ril_cdma_sms.h>
#include <telephony/ril_nv_items.h>
#include <telephony/ril_msim.h>
#ifndef FEATURE_UNIT_TEST
#include <sys/time.h>
#endif /* !FEATURE_UNIT_TEST */

#include <utils/Log.h>

#ifdef __cplusplus
extern "C" {
#endif

#if 1
#define LOGV RLOGV
#define LOGD RLOGD
#define LOGE RLOGE
#define LOGI RLOGI
#define LOGW RLOGW
#endif

#define MAX_GEMINI_SIM_NUM 4 /* Currently max SIM number support up to 4 */

#define MAX_IPV4_ADDRESS_LENGTH 17
//xxx.xxx.xxx.xxx
#define MAX_IPV6_ADDRESS_LENGTH 65
//xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx
//xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx

//VoLTE
#define MAX_PCSCF_NUMBER 6

typedef struct {
    //0: QCI is selected by network
    //[1-4]: value range for guaranteed bit rate Traffic Flows
    //[5-9]: value range for non-guarenteed bit rate Traffic Flows
    //[128-254]: value range for Operator-specific QCIs
    int qci; //class of EPS QoS
    int dlGbr; //downlink guaranteed bit rate
    int ulGbr; //uplink guaranteed bit rate
    int dlMbr; //downlink maximum bit rate
    int ulMbr; //uplink maximum bit rate
} Qos;

typedef struct {
    int id;
    int precedence;
    int direction;
    int networkPfIdentifier;
    int bitmap;
    char address[MAX_IPV6_ADDRESS_LENGTH];
    char mask[MAX_IPV6_ADDRESS_LENGTH];
    int protocolNextHeader;
    int localPortLow;
    int localPortHigh;
    int remotePortLow;
    int remotePortHigh;
    int spi;
    int tos;
    int tosMask;
    int flowLabel;
} PktFilter;

typedef struct {
    int authTokenNumber;
    int authTokenList[16];
    int flowIdNumber;
    int flowIdList[4][4];
} AuthToken;

typedef struct {
    int linkedPfNumber;
    int linkedPfList [16];
    int authtokenFlowIdNumber;
    AuthToken authtokenFlowIdList[4];
} TftParameter;

typedef struct {
    int operation;
    int pfNumber;
    PktFilter pfList [16]; //for response, just send necessary length. check responseSetupDedicateDataCall in ril.cpp
    TftParameter tftParameter;
} Tft;

typedef struct {
    int             ddcId;
    int             interfaceId;
    int             primaryCid;
    int             cid;        /* Context ID, uniquely identifies this call */
    int             active;     /* 0=inactive, 1=active/physical link down, 2=active/physical link up */
    int             signalingFlag;
    int             bearerId;
    int             failCause;
    int             hasQos;
    Qos             qos;
    int             hasTft;
    Tft             tft;
    int             hasPcscf;
    char            pcscf[MAX_PCSCF_NUMBER * (MAX_IPV6_ADDRESS_LENGTH + 1)];
} RIL_Dedicate_Data_Call_Struct;

typedef struct {
    int             isValid;
    Qos             qos;
    int             emergency_ind;
    int             pcscf_discovery_flag;
    int             signalingFlag;
    int             isHandover;
    int             assigned_rate;  //assign by MAL
} RIL_Default_Bearer_VA_Config_Struct;

typedef struct {
    int emc_attached_support;
    int emc_call_barred;
    char* ci;               // string type; four byte E-UTRAN cell ID in hexadecimal format
    char* tac;              // string type; two byte tracking area code in hexadecimal format (e.g. "00C3" equals 195 in decimal)
} RIL_EpcNetworkFeatureInfo;

/// M: For 3G VT only @{
typedef enum {
    MSG_ID_WRAP_3GVT_RIL_CONFIG_INIT_IND = 0,
    MSG_ID_WRAP_3GVT_RIL_CONFIG_UPDATE_IND = 1,
    MSG_ID_WRAP_3GVT_RIL_CONFIG_DEINIT_IND = 2,
    MSG_ID_WRAP_3GVT_RIL_ON_USER_INPUT = 3,
} RIL_VT_MsgType;

typedef struct {
    unsigned char   callId;        // identify this call
    char   dtmfKey;
} RIL_VT_MsgParams;
/// @}

#if defined(ANDROID_SIM_COUNT_2)
#define SIM_COUNT 2
#elif defined(ANDROID_SIM_COUNT_3)
#define SIM_COUNT 3
#elif defined(ANDROID_SIM_COUNT_4)
#define SIM_COUNT 4
#else
#define SIM_COUNT 1
#endif
#define MAX_SIM_COUNT 4

#define PROPERTY_3G_SIM "persist.radio.simswitch"

#define RIL_VERSION 10     /* Current version */
#define RIL_VERSION_MIN 6 /* Minimum RIL_VERSION supported */

#define CDMA_ALPHA_INFO_BUFFER_LENGTH 64
#define CDMA_NUMBER_INFO_BUFFER_LENGTH 81

#define MAX_RILDS 3
#define MAX_SOCKET_NAME_LENGTH 20
#define MAX_CLIENT_ID_LENGTH 2
#define MAX_DEBUG_SOCKET_NAME_LENGTH 20
#define MAX_QEMU_PIPE_NAME_LENGTH  11
#define MAX_UUID_LENGTH 64

/* <name> in CNAP URC is an up to 80 characters long string containing the calling name  */
#define MAX_CNAP_LENGTH 81
/* <ccidx> in CLCC response ranges from 1 to N.
 * N, the maximum number of simultaneous call control processes is implementation specific.
 * For GSM, N is 7
 */
#define MAX_GSMCALL_CONNECTIONS 7   // only 7 connections allowed in GSM

typedef void * RIL_Token;

typedef enum {
    RIL_SOCKET_1,
    RIL_SOCKET_2,
    RIL_SOCKET_3,
    RIL_SOCKET_4,
    RIL_SOCKET_NUM,
    IMS_RIL_SOCKET_1,
    RIL_SOCKET_NUM_TOTAL
} RIL_SOCKET_ID;

typedef enum {
    MD_STATE_NORMAL                = 0,
    MD_STATE_ASSERT                = 1
} RIL_MDState;
typedef enum
{
    RIL_CLIENT_ID_RILJ   =  0, //reserve the value 0
    RIL_CLIENT_ID_MAL    = 1001,
    RIL_CLIENT_ID_MAL_AT = 1002,
}RIL_CLIENT_ID;

//RIL_Client_Header must define same as RIL_CLIENT
typedef struct RIL_Client_Header {
    int clientID;
}RIL_Client_Header;

#ifdef MTK_RIL
typedef struct RIL_Token_Header {
    RIL_Client_Header * client; //MUST in the beginning
} RIL_Token_Header;
#endif

typedef enum {
    RIL_E_SUCCESS = 0,
    RIL_E_RADIO_NOT_AVAILABLE = 1,     /* If radio did not start or is resetting */
    RIL_E_GENERIC_FAILURE = 2,
    RIL_E_PASSWORD_INCORRECT = 3,      /* for PIN/PIN2 methods only! */
    RIL_E_SIM_PIN2 = 4,                /* Operation requires SIM PIN2 to be entered */
    RIL_E_SIM_PUK2 = 5,                /* Operation requires SIM PIN2 to be entered */
    RIL_E_REQUEST_NOT_SUPPORTED = 6,
    RIL_E_CANCELLED = 7,
    RIL_E_OP_NOT_ALLOWED_DURING_VOICE_CALL = 8, /* data ops are not allowed during voice
                                                   call on a Class C GPRS device */
    RIL_E_OP_NOT_ALLOWED_BEFORE_REG_TO_NW = 9,  /* data ops are not allowed before device
                                                   registers in network */
    RIL_E_SMS_SEND_FAIL_RETRY = 10,             /* fail to send sms and need retry */
    RIL_E_SIM_ABSENT = 11,                      /* fail to set the location where CDMA subscription
                                                   shall be retrieved because of SIM or RUIM
                                                   card absent */
    RIL_E_SUBSCRIPTION_NOT_AVAILABLE = 12,      /* fail to find CDMA subscription from specified
                                                   location */
    RIL_E_MODE_NOT_SUPPORTED = 13,              /* HW does not support preferred network type */
    RIL_E_FDN_CHECK_FAILURE = 14,               /* command failed because recipient is not on FDN list */
    RIL_E_ILLEGAL_SIM_OR_ME = 15,               /* network selection failed due to
                                                   illegal SIM or ME */
    RIL_E_MISSING_RESOURCE = 16,                /* no logical channel available */
    RIL_E_NO_SUCH_ELEMENT = 17,                 /* application not found on SIM */
    RIL_E_DIAL_MODIFIED_TO_USSD = 18,           /* DIAL request modified to USSD */
    RIL_E_DIAL_MODIFIED_TO_SS = 19,             /* DIAL request modified to SS */
    RIL_E_DIAL_MODIFIED_TO_DIAL = 20,           /* DIAL request modified to DIAL with different
                                                   data */
    RIL_E_USSD_MODIFIED_TO_DIAL = 21,           /* USSD request modified to DIAL */
    RIL_E_USSD_MODIFIED_TO_SS = 22,             /* USSD request modified to SS */
    RIL_E_USSD_MODIFIED_TO_USSD = 23,           /* USSD request modified to different USSD
                                                   request */
    RIL_E_SS_MODIFIED_TO_DIAL = 24,             /* SS request modified to DIAL */
    RIL_E_SS_MODIFIED_TO_USSD = 25,             /* SS request modified to USSD */
    RIL_E_SUBSCRIPTION_NOT_SUPPORTED = 26,      /* Subscription not supported by RIL */
    RIL_E_SS_MODIFIED_TO_SS = 27,               /* SS request modified to different SS request */
    RIL_E_LCE_NOT_SUPPORTED = 36,               /* LCE service not supported(36 in RILConstants.java) */

    RIL_E_DIAL_STRING_TOO_LONG = 1001,
    RIL_E_TEXT_STRING_TOO_LONG = 1002,
    RIL_E_SIM_MEM_FULL = 1003,
    RIL_E_CALL_BARRED = 1004,                      /* command failed because call barred */

    RIL_E_EXTERNAL_APP_CAUSE_BEGIN = 2000,
    RIL_E_BT_SAP_UNDEFINED = 2001,
    RIL_E_BT_SAP_NOT_ACCESSIBLE = 2002,
    RIL_E_BT_SAP_CARD_REMOVED = 2003,
    /// @}

    //Remote SIM ME lock related APIs [Start]
    RIL_E_VERIFICATION_FAILED = 3000,
    RIL_E_REBOOT_REQUEST = 3001,
    //Remote SIM ME lock related APIs [End]

    // Call control start
    RIL_E_HOLD_FAILED_CAUSED_BY_TERMINATED = 6000,
    // Call control end
} RIL_Errno;

typedef enum {
    RIL_CALL_ACTIVE = 0,
    RIL_CALL_HOLDING = 1,
    RIL_CALL_DIALING = 2,    /* MO call only */
    RIL_CALL_ALERTING = 3,   /* MO call only */
    RIL_CALL_INCOMING = 4,   /* MT call only */
    RIL_CALL_WAITING = 5     /* MT call only */
} RIL_CallState;

typedef enum {
    RADIO_STATE_OFF = 0,                   /* Radio explictly powered off (eg CFUN=0) */
    RADIO_STATE_UNAVAILABLE = 1,           /* Radio unavailable (eg, resetting or not booted) */
    /* States 2-9 below are deprecated. Just leaving them here for backward compatibility. */
    RADIO_STATE_SIM_NOT_READY = 2,         /* Radio is on, but the SIM interface is not ready */
    RADIO_STATE_SIM_LOCKED_OR_ABSENT = 3,  /* SIM PIN locked, PUK required, network
                                              personalization locked, or SIM absent */
    RADIO_STATE_SIM_READY = 4,             /* Radio is on and SIM interface is available */
    RADIO_STATE_RUIM_NOT_READY = 5,        /* Radio is on, but the RUIM interface is not ready */
    RADIO_STATE_RUIM_READY = 6,            /* Radio is on and the RUIM interface is available */
    RADIO_STATE_RUIM_LOCKED_OR_ABSENT = 7, /* RUIM PIN locked, PUK required, network
                                              personalization locked, or RUIM absent */
    RADIO_STATE_NV_NOT_READY = 8,          /* Radio is on, but the NV interface is not available */
    RADIO_STATE_NV_READY = 9,              /* Radio is on and the NV interface is available */
    RADIO_STATE_ON = 10                    /* Radio is on */
} RIL_RadioState;

typedef enum {
    RADIO_TEMPSTATE_AVAILABLE = 0,     /* Radio available */
    RADIO_TEMPSTATE_UNAVAILABLE = 1,           /* Radio unavailable temporarily */
} RIL_RadioTempState;

typedef enum {
    RADIO_TECH_UNKNOWN = 0,
    RADIO_TECH_GPRS = 1,
    RADIO_TECH_EDGE = 2,
    RADIO_TECH_UMTS = 3,
    RADIO_TECH_IS95A = 4,
    RADIO_TECH_IS95B = 5,
    RADIO_TECH_1xRTT =  6,
    RADIO_TECH_EVDO_0 = 7,
    RADIO_TECH_EVDO_A = 8,
    RADIO_TECH_HSDPA = 9,
    RADIO_TECH_HSUPA = 10,
    RADIO_TECH_HSPA = 11,
    RADIO_TECH_EVDO_B = 12,
    RADIO_TECH_EHRPD = 13,
    RADIO_TECH_LTE = 14,
    RADIO_TECH_HSPAP = 15, // HSPA+
    RADIO_TECH_GSM = 16, // Only supports voice
    RADIO_TECH_TD_SCDMA = 17,
    RADIO_TECH_IWLAN = 18
} RIL_RadioTechnology;

typedef enum {
    RAF_UNKNOWN =  (1 <<  RADIO_TECH_UNKNOWN),
    RAF_GPRS = (1 << RADIO_TECH_GPRS),
    RAF_EDGE = (1 << RADIO_TECH_EDGE),
    RAF_UMTS = (1 << RADIO_TECH_UMTS),
    RAF_IS95A = (1 << RADIO_TECH_IS95A),
    RAF_IS95B = (1 << RADIO_TECH_IS95B),
    RAF_1xRTT = (1 << RADIO_TECH_1xRTT),
    RAF_EVDO_0 = (1 << RADIO_TECH_EVDO_0),
    RAF_EVDO_A = (1 << RADIO_TECH_EVDO_A),
    RAF_HSDPA = (1 << RADIO_TECH_HSDPA),
    RAF_HSUPA = (1 << RADIO_TECH_HSUPA),
    RAF_HSPA = (1 << RADIO_TECH_HSPA),
    RAF_EVDO_B = (1 << RADIO_TECH_EVDO_B),
    RAF_EHRPD = (1 << RADIO_TECH_EHRPD),
    RAF_LTE = (1 << RADIO_TECH_LTE),
    RAF_HSPAP = (1 << RADIO_TECH_HSPAP),
    RAF_GSM = (1 << RADIO_TECH_GSM),
    RAF_TD_SCDMA = (1 << RADIO_TECH_TD_SCDMA),
} RIL_RadioAccessFamily;

typedef enum {
    RC_PHASE_CONFIGURED = 0,  // LM is configured is initial value and value after FINISH completes
    RC_PHASE_START      = 1,  // START is sent before Apply and indicates that an APPLY will be
                              // forthcoming with these same parameters
    RC_PHASE_APPLY      = 2,  // APPLY is sent after all LM's receive START and returned
                              // RIL_RadioCapability.status = 0, if any START's fail no
                              // APPLY will be sent
    RC_PHASE_UNSOL_RSP  = 3,  // UNSOL_RSP is sent with RIL_UNSOL_RADIO_CAPABILITY
    RC_PHASE_FINISH     = 4   // FINISH is sent after all commands have completed. If an error
                              // occurs in any previous command the RIL_RadioAccessesFamily and
                              // logicalModemUuid fields will be the prior configuration thus
                              // restoring the configuration to the previous value. An error
                              // returned by this command will generally be ignored or may
                              // cause that logical modem to be removed from service.
} RadioCapabilityPhase;

typedef enum {
    RC_STATUS_NONE       = 0, // This parameter has no meaning with RC_PHASE_START,
                              // RC_PHASE_APPLY
    RC_STATUS_SUCCESS    = 1, // Tell modem the action transaction of set radio
                              // capability was success with RC_PHASE_FINISH
    RC_STATUS_FAIL       = 2, // Tell modem the action transaction of set radio
                              // capability is fail with RC_PHASE_FINISH.
} RadioCapabilityStatus;

#define RIL_RADIO_CAPABILITY_VERSION 1
typedef struct {
    int version;            // Version of structure, RIL_RADIO_CAPABILITY_VERSION
    int session;            // Unique session value defined by framework returned in all "responses/unsol"
    int phase;              // CONFIGURED, START, APPLY, FINISH
    int rat;                // RIL_RadioAccessFamily for the radio
    char logicalModemUuid[MAX_UUID_LENGTH]; // A UUID typically "com.xxxx.lmX where X is the logical modem.
    int status;             // Return status and an input parameter for RC_PHASE_FINISH
} RIL_RadioCapability;

typedef enum {
    ApplyRC_NONEED,
    ApplyRC_SUCCESS,
    APPLYRC_FAIL
} ApplyRadioCapabilityResult;

// Do we want to split Data from Voice and the use
// RIL_RadioTechnology for get/setPreferredVoice/Data ?
typedef enum {
    PREF_NET_TYPE_GSM_WCDMA                = 0, /* GSM/WCDMA (WCDMA preferred) */
    PREF_NET_TYPE_GSM_ONLY                 = 1, /* GSM only */
    PREF_NET_TYPE_WCDMA                    = 2, /* WCDMA  */
    PREF_NET_TYPE_GSM_WCDMA_AUTO           = 3, /* GSM/WCDMA (auto mode, according to PRL) */
    PREF_NET_TYPE_CDMA_EVDO_AUTO           = 4, /* CDMA and EvDo (auto mode, according to PRL) */
    PREF_NET_TYPE_CDMA_ONLY                = 5, /* CDMA only */
    PREF_NET_TYPE_EVDO_ONLY                = 6, /* EvDo only */
    PREF_NET_TYPE_GSM_WCDMA_CDMA_EVDO_AUTO = 7, /* GSM/WCDMA, CDMA, and EvDo (auto mode, according to PRL) */
    PREF_NET_TYPE_LTE_CDMA_EVDO            = 8, /* LTE, CDMA and EvDo */
    PREF_NET_TYPE_LTE_GSM_WCDMA            = 9, /* LTE, GSM/WCDMA */
    PREF_NET_TYPE_LTE_CMDA_EVDO_GSM_WCDMA  = 10, /* LTE, CDMA, EvDo, GSM/WCDMA */
    PREF_NET_TYPE_LTE_ONLY                 = 11, /* LTE only */
    PREF_NET_TYPE_LTE_WCDMA                = 12  /* LTE/WCDMA */
} RIL_PreferredNetworkType;

/* Source for cdma subscription */
typedef enum {
   CDMA_SUBSCRIPTION_SOURCE_RUIM_SIM = 0,
   CDMA_SUBSCRIPTION_SOURCE_NV = 1
} RIL_CdmaSubscriptionSource;

/* User-to-User signaling Info activation types derived from 3GPP 23.087 v8.0 */
typedef enum {
    RIL_UUS_TYPE1_IMPLICIT = 0,
    RIL_UUS_TYPE1_REQUIRED = 1,
    RIL_UUS_TYPE1_NOT_REQUIRED = 2,
    RIL_UUS_TYPE2_REQUIRED = 3,
    RIL_UUS_TYPE2_NOT_REQUIRED = 4,
    RIL_UUS_TYPE3_REQUIRED = 5,
    RIL_UUS_TYPE3_NOT_REQUIRED = 6
} RIL_UUS_Type;

/* User-to-User Signaling Information data coding schemes. Possible values for
 * Octet 3 (Protocol Discriminator field) in the UUIE. The values have been
 * specified in section 10.5.4.25 of 3GPP TS 24.008 */
typedef enum {
    RIL_UUS_DCS_USP = 0,          /* User specified protocol */
    RIL_UUS_DCS_OSIHLP = 1,       /* OSI higher layer protocol */
    RIL_UUS_DCS_X244 = 2,         /* X.244 */
    RIL_UUS_DCS_RMCF = 3,         /* Reserved for system mangement
                                     convergence function */
    RIL_UUS_DCS_IA5c = 4          /* IA5 characters */
} RIL_UUS_DCS;

/* User-to-User Signaling Information defined in 3GPP 23.087 v8.0
 * This data is passed in RIL_ExtensionRecord and rec contains this
 * structure when type is RIL_UUS_INFO_EXT_REC */
typedef struct {
  RIL_UUS_Type    uusType;    /* UUS Type */
  RIL_UUS_DCS     uusDcs;     /* UUS Data Coding Scheme */
  int             uusLength;  /* Length of UUS Data */
  char *          uusData;    /* UUS Data */
} RIL_UUS_Info;

/* CDMA Signal Information Record as defined in C.S0005 section 3.7.5.5 */
typedef struct {
  char isPresent;    /* non-zero if signal information record is present */
  char signalType;   /* as defined 3.7.5.5-1 */
  char alertPitch;   /* as defined 3.7.5.5-2 */
  char signal;       /* as defined 3.7.5.5-3, 3.7.5.5-4 or 3.7.5.5-5 */
} RIL_CDMA_SignalInfoRecord;

typedef struct {
    RIL_CallState   state;
    int             index;      /* Connection Index for use with, eg, AT+CHLD */
    int             toa;        /* type of address, eg 145 = intl */
    char            isMpty;     /* nonzero if is mpty call */
    char            isMT;       /* nonzero if call is mobile terminated */
    char            als;        /* ALS line indicator if available
                                   (0 = line 1) */
    char            isVoice;    /* nonzero if this is is a voice call */
    char            isVoicePrivacy;     /* nonzero if CDMA voice privacy mode is active */
    char *          number;     /* Remote party number */
    int             numberPresentation; /* 0=Allowed, 1=Restricted, 2=Not Specified/Unknown 3=Payphone */
    char *          name;       /* Remote party name */
    int             namePresentation; /* 0=Allowed, 1=Restricted, 2=Not Specified/Unknown 3=Payphone */
    RIL_UUS_Info *  uusInfo;    /* NULL or Pointer to User-User Signaling Information */
} RIL_Call;

/* Deprecated, use RIL_Data_Call_Response_v6 */
typedef struct {
    int             cid;        /* Context ID, uniquely identifies this call */
    int             active;     /* 0=inactive, 1=active/physical link down, 2=active/physical link up */
    char *          type;       /* One of the PDP_type values in TS 27.007 section 10.1.1.
                                   For example, "IP", "IPV6", "IPV4V6", or "PPP". */
    char *          apn;        /* ignored */
    char *          address;    /* An address, e.g., "192.0.1.3" or "2001:db8::1". */
} RIL_Data_Call_Response_v4;

/*
 * Returned by RIL_REQUEST_SETUP_DATA_CALL, RIL_REQUEST_DATA_CALL_LIST
 * and RIL_UNSOL_DATA_CALL_LIST_CHANGED, on error status != 0.
 */
typedef struct {
    int             status;     /* A RIL_DataCallFailCause, 0 which is PDP_FAIL_NONE if no error */
    int             suggestedRetryTime; /* If status != 0, this fields indicates the suggested retry
                                           back-off timer value RIL wants to override the one
                                           pre-configured in FW.
                                           The unit is miliseconds.
                                           The value < 0 means no value is suggested.
                                           The value 0 means retry should be done ASAP.
                                           The value of INT_MAX(0x7fffffff) means no retry. */
    int             cid;        /* Context ID, uniquely identifies this call */
    int             active;     /* 0=inactive, 1=active/physical link down, 2=active/physical link up */
    int             mtuSize;    /* 0 is default and query fail*/
    char *          type;       /* One of the PDP_type values in TS 27.007 section 10.1.1.
                                   For example, "IP", "IPV6", "IPV4V6", or "PPP". If status is
                                   PDP_FAIL_ONLY_SINGLE_BEARER_ALLOWED this is the type supported
                                   such as "IP" or "IPV6" */
    char *          ifname;     /* The network interface name */
    char *          addresses;  /* A space-delimited list of addresses with optional "/" prefix length,
                                   e.g., "192.0.1.3" or "192.0.1.11/16 2001:db8::1/64".
                                   May not be empty, typically 1 IPv4 or 1 IPv6 or
                                   one of each. If the prefix length is absent the addresses
                                   are assumed to be point to point with IPv4 having a prefix
                                   length of 32 and IPv6 128. */
    char *          dnses;      /* A space-delimited list of DNS server addresses,
                                   e.g., "192.0.1.3" or "192.0.1.11 2001:db8::1".
                                   May be empty. */
    char *          gateways;   /* A space-delimited list of default gateway addresses,
                                   e.g., "192.0.1.3" or "192.0.1.11 2001:db8::1".
                                   May be empty in which case the addresses represent point
                                   to point connections. */
    //VoLTE
    char*                           pcscf;
    int                             concatenateNum;
    RIL_Dedicate_Data_Call_Struct*  concatenate;
    RIL_Dedicate_Data_Call_Struct   defaultBearer;
} RIL_Data_Call_Response_v6;

typedef struct {
    int             status;     /* A RIL_DataCallFailCause, 0 which is PDP_FAIL_NONE if no error */
    int             suggestedRetryTime; /* If status != 0, this fields indicates the suggested retry
                                           back-off timer value RIL wants to override the one
                                           pre-configured in FW.
                                           The unit is miliseconds.
                                           The value < 0 means no value is suggested.
                                           The value 0 means retry should be done ASAP.
                                           The value of INT_MAX(0x7fffffff) means no retry. */
    int             cid;        /* Context ID, uniquely identifies this call */
    int             active;     /* 0=inactive, 1=active/physical link down, 2=active/physical link up */
    char *          type;       /* One of the PDP_type values in TS 27.007 section 10.1.1.
                                   For example, "IP", "IPV6", "IPV4V6", or "PPP". If status is
                                   PDP_FAIL_ONLY_SINGLE_BEARER_ALLOWED this is the type supported
                                   such as "IP" or "IPV6" */
    char *          ifname;     /* The network interface name */
    char *          addresses;  /* A space-delimited list of addresses with optional "/" prefix length,
                                   e.g., "192.0.1.3" or "192.0.1.11/16 2001:db8::1/64".
                                   May not be empty, typically 1 IPv4 or 1 IPv6 or
                                   one of each. If the prefix length is absent the addresses
                                   are assumed to be point to point with IPv4 having a prefix
                                   length of 32 and IPv6 128. */
    char *          dnses;      /* A space-delimited list of DNS server addresses,
                                   e.g., "192.0.1.3" or "192.0.1.11 2001:db8::1".
                                   May be empty. */
    char *          gateways;   /* A space-delimited list of default gateway addresses,
                                   e.g., "192.0.1.3" or "192.0.1.11 2001:db8::1".
                                   May be empty in which case the addresses represent point
                                   to point connections. */
    char *          pcscf;    /* the Proxy Call State Control Function address
                                 via PCO(Protocol Configuration Option) for IMS client. */

    //VoLTE
    int                             concatenateNum;
    RIL_Dedicate_Data_Call_Struct*  concatenate;
    RIL_Dedicate_Data_Call_Struct   defaultBearer;
} RIL_Data_Call_Response_v9; // FIXME: Change to v10

typedef struct {
    int             status;     /* A RIL_DataCallFailCause, 0 which is PDP_FAIL_NONE if no error */
    int             suggestedRetryTime; /* If status != 0, this fields indicates the suggested retry
                                           back-off timer value RIL wants to override the one
                                           pre-configured in FW.
                                           The unit is miliseconds.
                                           The value < 0 means no value is suggested.
                                           The value 0 means retry should be done ASAP.
                                           The value of INT_MAX(0x7fffffff) means no retry. */
    int             cid;        /* Context ID, uniquely identifies this call */
    int             active;     /* 0=inactive, 1=active/physical link down, 2=active/physical link up */
    char *          type;       /* One of the PDP_type values in TS 27.007 section 10.1.1.
                                   For example, "IP", "IPV6", "IPV4V6", or "PPP". If status is
                                   PDP_FAIL_ONLY_SINGLE_BEARER_ALLOWED this is the type supported
                                   such as "IP" or "IPV6" */
    char *          ifname;     /* The network interface name */
    char *          addresses;  /* A space-delimited list of addresses with optional "/" prefix length,
                                   e.g., "192.0.1.3" or "192.0.1.11/16 2001:db8::1/64".
                                   May not be empty, typically 1 IPv4 or 1 IPv6 or
                                   one of each. If the prefix length is absent the addresses
                                   are assumed to be point to point with IPv4 having a prefix
                                   length of 32 and IPv6 128. */
    char *          dnses;      /* A space-delimited list of DNS server addresses,
                                   e.g., "192.0.1.3" or "192.0.1.11 2001:db8::1".
                                   May be empty. */
    char *          gateways;   /* A space-delimited list of default gateway addresses,
                                   e.g., "192.0.1.3" or "192.0.1.11 2001:db8::1".
                                   May be empty in which case the addresses represent point
                                   to point connections. */
    char *          pcscf;    /* the Proxy Call State Control Function address
                                 via PCO(Protocol Configuration Option) for IMS client. */
    int             mtu;        /* MTU received from network
                                   Value <= 0 means network has either not sent a value or
                                   sent an invalid value */
    // VoLTE
    int concatenateNum;
    RIL_Dedicate_Data_Call_Struct*  concatenate;
    RIL_Dedicate_Data_Call_Struct   defaultBearer;
    int eran_type;          /* 0: UNSPEC, 1: MOBILE_3GPP, 2: WIFI, 3: MOBILE_3GPP2
                                     4: TYPE_MAX, 0xFE: RDS_RAN_NONE, 0xFF: RDS_RAN_DENY */
} RIL_Data_Call_Response_v11;

typedef enum {
    RADIO_TECH_3GPP = 1, /* 3GPP Technologies - GSM, WCDMA */
    RADIO_TECH_3GPP2 = 2 /* 3GPP2 Technologies - CDMA */
} RIL_RadioTechnologyFamily;

typedef struct {
    RIL_RadioTechnologyFamily tech;
    unsigned char             retry;       /* 0 == not retry, nonzero == retry */
    int                       messageRef;  /* Valid field if retry is set to nonzero.
                                              Contains messageRef from RIL_SMS_Response
                                              corresponding to failed MO SMS.
                                            */

    union {
        /* Valid field if tech is RADIO_TECH_3GPP2. See RIL_REQUEST_CDMA_SEND_SMS */
        RIL_CDMA_SMS_Message* cdmaMessage;

        /* Valid field if tech is RADIO_TECH_3GPP. See RIL_REQUEST_SEND_SMS */
        char**                gsmMessage;
    } message;
} RIL_IMS_SMS_Message;

typedef struct {
    int messageRef;   /* TP-Message-Reference for GSM,
                         and BearerData MessageId for CDMA
                         (See 3GPP2 C.S0015-B, v2.0, table 4.5-1). */
    char *ackPDU;     /* or NULL if n/a */
    int errorCode;    /* See 3GPP 27.005, 3.2.5 for GSM/UMTS,
                         3GPP2 N.S0005 (IS-41C) Table 171 for CDMA,
                         -1 if unknown or not applicable*/
} RIL_SMS_Response;

/** Used by RIL_REQUEST_WRITE_SMS_TO_SIM */
typedef struct {
    int status;     /* Status of message.  See TS 27.005 3.1, "<stat>": */
                    /*      0 = "REC UNREAD"    */
                    /*      1 = "REC READ"      */
                    /*      2 = "STO UNSENT"    */
                    /*      3 = "STO SENT"      */
    char * pdu;     /* PDU of message to write, as an ASCII hex string less the SMSC address,
                       the TP-layer length is "strlen(pdu)/2". */
    char * smsc;    /* SMSC address in GSM BCD format prefixed by a length byte
                       (as expected by TS 27.005) or NULL for default SMSC */
} RIL_SMS_WriteArgs;

typedef struct
{
	int	used;
	int total;
} RIL_SMS_Memory_Status;


/** Used by RIL_REQUEST_DIAL */
typedef struct {
    char * address;
    int clir;
            /* (same as 'n' paremeter in TS 27.007 7.7 "+CLIR"
             * clir == 0 on "use subscription default value"
             * clir == 1 on "CLIR invocation" (restrict CLI presentation)
             * clir == 2 on "CLIR suppression" (allow CLI presentation)
             */
    RIL_UUS_Info *  uusInfo;    /* NULL or Pointer to User-User Signaling Information */
} RIL_Dial;

typedef struct {
    int command;    /* one of the commands listed for TS 27.007 +CRSM*/
    int fileid;     /* EF id */
    char *path;     /* "pathid" from TS 27.007 +CRSM command.
                       Path is in hex asciii format eg "7f205f70"
                       Path must always be provided.
                     */
    int p1;
    int p2;
    int p3;
    char *data;     /* May be NULL*/
    char *pin2;     /* May be NULL*/
} RIL_SIM_IO_v5;

typedef struct {
    int command;    /* one of the commands listed for TS 27.007 +CRSM*/
    int fileid;     /* EF id */
    char *path;     /* "pathid" from TS 27.007 +CRSM command.
                       Path is in hex asciii format eg "7f205f70"
                       Path must always be provided.
                     */
    int p1;
    int p2;
    int p3;
    char *data;     /* May be NULL*/
    char *pin2;     /* May be NULL*/
    char *aidPtr;   /* AID value, See ETSI 102.221 8.1 and 101.220 4, NULL if no value. */
} RIL_SIM_IO_v6;

typedef struct {
    int command;    /* one of the commands listed for TS 27.007 +CRSM*/
    int fileid;     /* EF id */
    char *path;     /* "pathid" from TS 27.007 +CRSM command.
                       Path is in hex asciii format eg "7f205f70"
                       Path must always be provided.
                     */
    int p1;
    int p2;
    int p3;
    char *data;     /* May be NULL*/
    char *pin2;     /* May be NULL*/
    char *aidPtr;   /* AID value, See ETSI 102.221 8.1 and 101.220 4, NULL if no value. */
    int sessionId;    /* Channel, which is used to get files from multi-application */
} RIL_SIM_IO_EX_v6;

/* Used by RIL_REQUEST_SIM_TRANSMIT_APDU_CHANNEL and
 * RIL_REQUEST_SIM_TRANSMIT_APDU_BASIC. */
typedef struct {
    int sessionid;  /* "sessionid" from TS 27.007 +CGLA command. Should be
                       ignored for +CSIM command. */

    /* Following fields are used to derive the APDU ("command" and "length"
       values in TS 27.007 +CSIM and +CGLA commands). */
    int cla;
    int instruction;
    int p1;
    int p2;
    int p3;         /* A negative P3 implies a 4 byte APDU. */
    char *data;     /* May be NULL. In hex string format. */
} RIL_SIM_APDU;

typedef struct {
    int sw1;
    int sw2;
    char *simResponse;  /* In hex string format ([a-fA-F0-9]*), except for SIM_AUTHENTICATION
                           response for which it is in Base64 format, see 3GPP TS 31.102 7.1.2 */
} RIL_SIM_IO_Response;

/* See also com.android.internal.telephony.gsm.CallForwardInfo */

typedef struct {
    int             status;     /*
                                 * For RIL_REQUEST_QUERY_CALL_FORWARD_STATUS
                                 * status 1 = active, 0 = not active
                                 *
                                 * For RIL_REQUEST_SET_CALL_FORWARD:
                                 * status is:
                                 * 0 = disable
                                 * 1 = enable
                                 * 2 = interrogate
                                 * 3 = registeration
                                 * 4 = erasure
                                 */

    int             reason;      /* from TS 27.007 7.11 "reason" */
    int             serviceClass;/* From 27.007 +CCFC/+CLCK "class"
                                    See table for Android mapping from
                                    MMI service code
                                    0 means user doesn't input class */
    int             toa;         /* "type" from TS 27.007 7.11 */
    char *          number;      /* "number" from TS 27.007 7.11. May be NULL */
    int             timeSeconds; /* for CF no reply only */
}RIL_CallForwardInfo;

typedef struct {
   char * cid;         /* Combination of LAC and Cell Id in 32 bits in GSM.
                        * Upper 16 bits is LAC and lower 16 bits
                        * is CID (as described in TS 27.005)
                        * Primary Scrambling Code (as described in TS 25.331)
                        *         in 9 bits in UMTS
                        * Valid values are hexadecimal 0x0000 - 0xffffffff.
                        */
   int    rssi;        /* Received RSSI in GSM,
                        * Level index of CPICH Received Signal Code Power in UMTS
                        */
} RIL_NeighboringCell;

typedef struct {
  char lce_status;                 /* LCE service status:
                                    * -1 = not supported;
                                    * 0 = stopped;
                                    * 1 = active.
                                    */
  unsigned int actual_interval_ms; /* actual LCE reporting interval,
                                    * meaningful only if LCEStatus = 1.
                                    */
} RIL_LceStatusInfo;

typedef struct {
  unsigned int last_hop_capacity_kbps; /* last-hop cellular capacity: kilobits/second. */
  unsigned char confidence_level;      /* capacity estimate confidence: 0-100 */
  unsigned char lce_suspended;         /* LCE report going to be suspended? (e.g., radio
                                        * moves to inactive state or network type change)
                                        * 1 = suspended;
                                        * 0 = not suspended.
                                        */
} RIL_LceDataInfo;

/* See RIL_REQUEST_LAST_CALL_FAIL_CAUSE */
typedef enum {
    CALL_FAIL_UNOBTAINABLE_NUMBER = 1,
    CALL_FAIL_NORMAL = 16,
    CALL_FAIL_BUSY = 17,
    CALL_FAIL_CONGESTION = 34,
    CALL_FAIL_ACM_LIMIT_EXCEEDED = 68,
    CALL_FAIL_CALL_BARRED = 240,
    CALL_FAIL_FDN_BLOCKED = 241,
    CALL_FAIL_IMSI_UNKNOWN_IN_VLR = 242,
    CALL_FAIL_IMEI_NOT_ACCEPTED = 243,
    CALL_FAIL_DIAL_MODIFIED_TO_USSD = 244, /* STK Call Control */
    CALL_FAIL_DIAL_MODIFIED_TO_SS = 245,
    CALL_FAIL_DIAL_MODIFIED_TO_DIAL = 246,
    CALL_FAIL_CDMA_LOCKED_UNTIL_POWER_CYCLE = 1000,
    CALL_FAIL_CDMA_DROP = 1001,
    CALL_FAIL_CDMA_INTERCEPT = 1002,
    CALL_FAIL_CDMA_REORDER = 1003,
    CALL_FAIL_CDMA_SO_REJECT = 1004,
    CALL_FAIL_CDMA_RETRY_ORDER = 1005,
    CALL_FAIL_CDMA_ACCESS_FAILURE = 1006,
    CALL_FAIL_CDMA_PREEMPTED = 1007,
    CALL_FAIL_CDMA_NOT_EMERGENCY = 1008, /* For non-emergency number dialed
                                            during emergency callback mode */
    CALL_FAIL_CDMA_ACCESS_BLOCKED = 1009, /* CDMA network access probes blocked */
    CALL_FAIL_ERROR_UNSPECIFIED = 0xffff
} RIL_LastCallFailCause;

typedef struct {
  RIL_LastCallFailCause cause_code;
  char *                vendor_cause;
} RIL_LastCallFailCauseInfo;

/* See RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE */
typedef enum {
    PDP_FAIL_NONE = 0, /* No error, connection ok */

    /* an integer cause code defined in TS 24.008
       section 6.1.3.1.3 or TS 24.301 Release 8+ Annex B.
       If the implementation does not have access to the exact cause codes,
       then it should return one of the following values,
       as the UI layer needs to distinguish these
       cases for error notification and potential retries. */
    PDP_FAIL_OPERATOR_BARRED = 0x08,               /* no retry */
    PDP_FAIL_INSUFFICIENT_RESOURCES = 0x1A,
    PDP_FAIL_MISSING_UKNOWN_APN = 0x1B,            /* no retry */
    PDP_FAIL_UNKNOWN_PDP_ADDRESS_TYPE = 0x1C,      /* no retry */
    PDP_FAIL_USER_AUTHENTICATION = 0x1D,           /* no retry */
    PDP_FAIL_ACTIVATION_REJECT_GGSN = 0x1E,        /* no retry */
    PDP_FAIL_ACTIVATION_REJECT_UNSPECIFIED = 0x1F,
    PDP_FAIL_SERVICE_OPTION_NOT_SUPPORTED = 0x20,  /* no retry */
    PDP_FAIL_SERVICE_OPTION_NOT_SUBSCRIBED = 0x21, /* no retry */
    PDP_FAIL_SERVICE_OPTION_OUT_OF_ORDER = 0x22,
    PDP_FAIL_NSAPI_IN_USE = 0x23,                  /* no retry */
    PDP_FAIL_REGULAR_DEACTIVATION = 0x24,          /* possibly restart radio,
                                                      based on framework config */
    PDP_FAIL_ONLY_IPV4_ALLOWED = 0x32,             /* no retry */
    PDP_FAIL_ONLY_IPV6_ALLOWED = 0x33,             /* no retry */
    PDP_FAIL_ONLY_SINGLE_BEARER_ALLOWED = 0x34,
    PDP_FAIL_PROTOCOL_ERRORS   = 0x6F,             /* no retry */

    /* Not mentioned in the specification */
    PDP_FAIL_VOICE_REGISTRATION_FAIL = -1,
    PDP_FAIL_DATA_REGISTRATION_FAIL = -2,

   /* reasons for data call drop - network/modem disconnect */
    PDP_FAIL_SIGNAL_LOST = -3,
    PDP_FAIL_PREF_RADIO_TECH_CHANGED = -4,/* preferred technology has changed, should retry
                                             with parameters appropriate for new technology */
    PDP_FAIL_RADIO_POWER_OFF = -5,        /* data call was disconnected because radio was resetting,
                                             powered off - no retry */
    PDP_FAIL_TETHERED_CALL_ACTIVE = -6,   /* data call was disconnected by modem because tethered
                                             mode was up on same APN/data profile - no retry until
                                             tethered call is off */

    PDP_FAIL_ERROR_UNSPECIFIED = 0xffff,  /* retry silently */
} RIL_DataCallFailCause;

/* See RIL_REQUEST_SETUP_DATA_CALL */
typedef enum {
    RIL_DATA_PROFILE_DEFAULT    = 0,
    RIL_DATA_PROFILE_TETHERED   = 1,
    RIL_DATA_PROFILE_IMS        = 2,
    RIL_DATA_PROFILE_FOTA       = 3,
    RIL_DATA_PROFILE_CBS        = 4,
    RIL_DATA_PROFILE_OEM_BASE   = 1000,    /* Start of OEM-specific profiles */

    RIL_DATA_PROFILE_VENDOR_MMS    = RIL_DATA_PROFILE_OEM_BASE + 1,
    RIL_DATA_PROFILE_INVALID    = 0xFFFFFFFF
} RIL_DataProfile;

/* Used by RIL_UNSOL_SUPP_SVC_NOTIFICATION */
typedef struct {
    int     notificationType;   /*
                                 * 0 = MO intermediate result code
                                 * 1 = MT unsolicited result code
                                 */
    int     code;               /* See 27.007 7.17
                                   "code1" for MO
                                   "code2" for MT. */
    int     index;              /* CUG index. See 27.007 7.17. */
    int     type;               /* "type" from 27.007 7.17 (MT only). */
    char *  number;             /* "number" from 27.007 7.17
                                   (MT only, may be NULL). */
} RIL_SuppSvcNotification;

/* Used by RIL_UNSOL_CRSS_NOTIFICATION */

typedef struct {
    int    code;            /*
                             * 0: +CCWA
                             * 1: +CDIP
                             * 2: +CLIP
                             * 3: +COLP
                             */
    int    type;            /* type of address octet in integer format (refer GSM 04.08 [8] subclause 10.5.4.7) */
    char * number;          /* string type phone number of format specified by <type> */
    char * alphaid;         /*
                             * optional string type alphanumeric representation of <number>
                             * corresponding to the entry found in phonebook;
                             */
    int    cli_validity;    /* CLI validity value,
                               0: PRESENTATION_ALLOWED,
                               1: PRESENTATION_RESTRICTED,
                               2: PRESENTATION_UNKNOWN
                            */
} RIL_CrssNotification;





#define RIL_CARD_MAX_APPS     8

typedef enum {
    RIL_CARDSTATE_ABSENT   = 0,
    RIL_CARDSTATE_PRESENT  = 1,
    RIL_CARDSTATE_ERROR    = 2
} RIL_CardState;

typedef enum {
    RIL_PERSOSUBSTATE_UNKNOWN                   = 0, /* initial state */
    RIL_PERSOSUBSTATE_IN_PROGRESS               = 1, /* in between each lock transition */
    RIL_PERSOSUBSTATE_READY                     = 2, /* when either SIM or RUIM Perso is finished
                                                        since each app can only have 1 active perso
                                                        involved */
    RIL_PERSOSUBSTATE_SIM_NETWORK               = 3,
    RIL_PERSOSUBSTATE_SIM_NETWORK_SUBSET        = 4,
    RIL_PERSOSUBSTATE_SIM_CORPORATE             = 5,
    RIL_PERSOSUBSTATE_SIM_SERVICE_PROVIDER      = 6,
    RIL_PERSOSUBSTATE_SIM_SIM                   = 7,
    RIL_PERSOSUBSTATE_SIM_NETWORK_PUK           = 8, /* The corresponding perso lock is blocked */
    RIL_PERSOSUBSTATE_SIM_NETWORK_SUBSET_PUK    = 9,
    RIL_PERSOSUBSTATE_SIM_CORPORATE_PUK         = 10,
    RIL_PERSOSUBSTATE_SIM_SERVICE_PROVIDER_PUK  = 11,
    RIL_PERSOSUBSTATE_SIM_SIM_PUK               = 12,
    RIL_PERSOSUBSTATE_RUIM_NETWORK1             = 13,
    RIL_PERSOSUBSTATE_RUIM_NETWORK2             = 14,
    RIL_PERSOSUBSTATE_RUIM_HRPD                 = 15,
    RIL_PERSOSUBSTATE_RUIM_CORPORATE            = 16,
    RIL_PERSOSUBSTATE_RUIM_SERVICE_PROVIDER     = 17,
    RIL_PERSOSUBSTATE_RUIM_RUIM                 = 18,
    RIL_PERSOSUBSTATE_RUIM_NETWORK1_PUK         = 19, /* The corresponding perso lock is blocked */
    RIL_PERSOSUBSTATE_RUIM_NETWORK2_PUK         = 20,
    RIL_PERSOSUBSTATE_RUIM_HRPD_PUK             = 21,
    RIL_PERSOSUBSTATE_RUIM_CORPORATE_PUK        = 22,
    RIL_PERSOSUBSTATE_RUIM_SERVICE_PROVIDER_PUK = 23,
    RIL_PERSOSUBSTATE_RUIM_RUIM_PUK             = 24
} RIL_PersoSubstate;

typedef enum {
    RIL_APPSTATE_UNKNOWN               = 0,
    RIL_APPSTATE_DETECTED              = 1,
    RIL_APPSTATE_PIN                   = 2, /* If PIN1 or UPin is required */
    RIL_APPSTATE_PUK                   = 3, /* If PUK1 or Puk for UPin is required */
    RIL_APPSTATE_SUBSCRIPTION_PERSO    = 4, /* perso_substate should be look at
                                               when app_state is assigned to this value */
    RIL_APPSTATE_READY                 = 5
} RIL_AppState;

typedef enum {
    RIL_PINSTATE_UNKNOWN              = 0,
    RIL_PINSTATE_ENABLED_NOT_VERIFIED = 1,
    RIL_PINSTATE_ENABLED_VERIFIED     = 2,
    RIL_PINSTATE_DISABLED             = 3,
    RIL_PINSTATE_ENABLED_BLOCKED      = 4,
    RIL_PINSTATE_ENABLED_PERM_BLOCKED = 5
} RIL_PinState;

typedef enum {
  RIL_APPTYPE_UNKNOWN = 0,
  RIL_APPTYPE_SIM     = 1,
  RIL_APPTYPE_USIM    = 2,
  RIL_APPTYPE_RUIM    = 3,
  RIL_APPTYPE_CSIM    = 4,
  RIL_APPTYPE_ISIM    = 5
} RIL_AppType;

typedef struct
{
  RIL_AppType      app_type;
  RIL_AppState     app_state;
  RIL_PersoSubstate perso_substate; /* applicable only if app_state ==
                                       RIL_APPSTATE_SUBSCRIPTION_PERSO */
  char             *aid_ptr;        /* null terminated string, e.g., from 0xA0, 0x00 -> 0x41,
                                       0x30, 0x30, 0x30 */
  char             *app_label_ptr;  /* null terminated string */
  int              pin1_replaced;   /* applicable to USIM, CSIM & ISIM */
  RIL_PinState     pin1;
  RIL_PinState     pin2;
} RIL_AppStatus;

/* Deprecated, use RIL_CardStatus_v6 */
typedef struct
{
  RIL_CardState card_state;
  RIL_PinState  universal_pin_state;             /* applicable to USIM and CSIM: RIL_PINSTATE_xxx */
  int           gsm_umts_subscription_app_index; /* value < RIL_CARD_MAX_APPS, -1 if none */
  int           cdma_subscription_app_index;     /* value < RIL_CARD_MAX_APPS, -1 if none */
  int           num_applications;                /* value <= RIL_CARD_MAX_APPS */
  RIL_AppStatus applications[RIL_CARD_MAX_APPS];
} RIL_CardStatus_v5;

typedef struct
{
  RIL_CardState card_state;
  RIL_PinState  universal_pin_state;             /* applicable to USIM and CSIM: RIL_PINSTATE_xxx */
  int           gsm_umts_subscription_app_index; /* value < RIL_CARD_MAX_APPS, -1 if none */
  int           cdma_subscription_app_index;     /* value < RIL_CARD_MAX_APPS, -1 if none */
  int           ims_subscription_app_index;      /* value < RIL_CARD_MAX_APPS, -1 if none */
  int           num_applications;                /* value <= RIL_CARD_MAX_APPS */
  RIL_AppStatus applications[RIL_CARD_MAX_APPS];
} RIL_CardStatus_v6;

/** The result of a SIM refresh, returned in data[0] of RIL_UNSOL_SIM_REFRESH
 *      or as part of RIL_SimRefreshResponse_v7
 */
typedef enum {
    /* A file on SIM has been updated.  data[1] contains the EFID. */
    SIM_FILE_UPDATE = 0,
    /* SIM initialized.  All files should be re-read. */
    SIM_INIT = 1,

    /* SIM reset.  SIM power required, SIM may be locked and all files should be re-read. */
    SIM_RESET = 2,
    /*ISIM application init*/
    APP_INIT = 3,
    /* SIM Initialization and Full File Change Notification */
    SIM_INIT_FULL_FILE_CHANGE  = 4,
    /* SIM Initialization and File Change Notification */
    SIM_INIT_FILE_CHANGE = 5,
    /* NAA Session reset*/
    SESSION_RESET = 6
} RIL_SimRefreshResult;

typedef struct {
    RIL_SimRefreshResult result;
    char *               ef_id; /* is the EFID of the updated file if the result is */
                                /* SIM_FILE_UPDATE or 0 for any other result. */
    char *               aid;   /* is AID(application ID) of the card application */
                                /* See ETSI 102.221 8.1 and 101.220 4 */
                                /*     For SIM_FILE_UPDATE result it can be set to AID of */
                                /*         application in which updated EF resides or it can be */
                                /*         NULL if EF is outside of an application. */
                                /*     For SIM_INIT result this field is set to AID of */
                                /*         application that caused REFRESH */
                                /*     For SIM_RESET result it is NULL. */
} RIL_SimRefreshResponse_v7;

/* Deprecated, use RIL_CDMA_CallWaiting_v6 */
typedef struct {
    char *          number;             /* Remote party number */
    int             numberPresentation; /* 0=Allowed, 1=Restricted, 2=Not Specified/Unknown */
    char *          name;               /* Remote party name */
    RIL_CDMA_SignalInfoRecord signalInfoRecord;
} RIL_CDMA_CallWaiting_v5;

typedef struct {
    char *          number;             /* Remote party number */
    int             numberPresentation; /* 0=Allowed, 1=Restricted, 2=Not Specified/Unknown */
    char *          name;               /* Remote party name */
    RIL_CDMA_SignalInfoRecord signalInfoRecord;
    /* Number type/Number plan required to support International Call Waiting */
    int             number_type;        /* 0=Unknown, 1=International, 2=National,
                                           3=Network specific, 4=subscriber */
    int             number_plan;        /* 0=Unknown, 1=ISDN, 3=Data, 4=Telex, 8=Nat'l, 9=Private */
} RIL_CDMA_CallWaiting_v6;

/**
 * Which types of Cell Broadcast Message (CBM) are to be received by the ME
 *
 * uFromServiceID - uToServiceID defines a range of CBM message identifiers
 * whose value is 0x0000 - 0xFFFF as defined in TS 23.041 9.4.1.2.2 for GMS
 * and 9.4.4.2.2 for UMTS. All other values can be treated as empty
 * CBM message ID.
 *
 * uFromCodeScheme - uToCodeScheme defines a range of CBM data coding schemes
 * whose value is 0x00 - 0xFF as defined in TS 23.041 9.4.1.2.3 for GMS
 * and 9.4.4.2.3 for UMTS.
 * All other values can be treated as empty CBM data coding scheme.
 *
 * selected 0 means message types specified in <fromServiceId, toServiceId>
 * and <fromCodeScheme, toCodeScheme>are not accepted, while 1 means accepted.
 *
 * Used by RIL_REQUEST_GSM_GET_BROADCAST_CONFIG and
 * RIL_REQUEST_GSM_SET_BROADCAST_CONFIG.
 */
typedef struct {
    int fromServiceId;
    int toServiceId;
    int fromCodeScheme;
    int toCodeScheme;
    unsigned char selected;
} RIL_GSM_BroadcastSmsConfigInfo;

/* No restriction at all including voice/SMS/USSD/SS/AV64 and packet data. */
#define RIL_RESTRICTED_STATE_NONE           0x00
/* Block emergency call due to restriction. But allow all normal voice/SMS/USSD/SS/AV64. */
#define RIL_RESTRICTED_STATE_CS_EMERGENCY   0x01
/* Block all normal voice/SMS/USSD/SS/AV64 due to restriction. Only Emergency call allowed. */
#define RIL_RESTRICTED_STATE_CS_NORMAL      0x02
/* Block all voice/SMS/USSD/SS/AV64 including emergency call due to restriction.*/
#define RIL_RESTRICTED_STATE_CS_ALL         0x04
/* Block packet data access due to restriction. */
#define RIL_RESTRICTED_STATE_PS_ALL         0x10

/* The status for an OTASP/OTAPA session */
typedef enum {
    CDMA_OTA_PROVISION_STATUS_SPL_UNLOCKED,
    CDMA_OTA_PROVISION_STATUS_SPC_RETRIES_EXCEEDED,
    CDMA_OTA_PROVISION_STATUS_A_KEY_EXCHANGED,
    CDMA_OTA_PROVISION_STATUS_SSD_UPDATED,
    CDMA_OTA_PROVISION_STATUS_NAM_DOWNLOADED,
    CDMA_OTA_PROVISION_STATUS_MDN_DOWNLOADED,
    CDMA_OTA_PROVISION_STATUS_IMSI_DOWNLOADED,
    CDMA_OTA_PROVISION_STATUS_PRL_DOWNLOADED,
    CDMA_OTA_PROVISION_STATUS_COMMITTED,
    CDMA_OTA_PROVISION_STATUS_OTAPA_STARTED,
    CDMA_OTA_PROVISION_STATUS_OTAPA_STOPPED,
    CDMA_OTA_PROVISION_STATUS_OTAPA_ABORTED
} RIL_CDMA_OTA_ProvisionStatus;

typedef struct {
    int signalStrength;  /* Valid values are (0-31, 99) as defined in TS 27.007 8.5 */
    int bitErrorRate;    /* bit error rate (0-7, 99) as defined in TS 27.007 8.5 */
} RIL_GW_SignalStrength;

typedef struct {
    int signalStrength;  /* Valid values are (0-31, 99) as defined in TS 27.007 8.5 */
    int bitErrorRate;    /* bit error rate (0-7, 99) as defined in TS 27.007 8.5 */
} RIL_SignalStrengthWcdma;

typedef struct {
    int dbm;  /* Valid values are positive integers.  This value is the actual RSSI value
               * multiplied by -1.  Example: If the actual RSSI is -75, then this response
               * value will be 75.
               */
    int ecio; /* Valid values are positive integers.  This value is the actual Ec/Io multiplied
               * by -10.  Example: If the actual Ec/Io is -12.5 dB, then this response value
               * will be 125.
               */
} RIL_CDMA_SignalStrength;


typedef struct {
    int dbm;  /* Valid values are positive integers.  This value is the actual RSSI value
               * multiplied by -1.  Example: If the actual RSSI is -75, then this response
               * value will be 75.
               */
    int ecio; /* Valid values are positive integers.  This value is the actual Ec/Io multiplied
               * by -10.  Example: If the actual Ec/Io is -12.5 dB, then this response value
               * will be 125.
               */
    int signalNoiseRatio; /* Valid values are 0-8.  8 is the highest signal to noise ratio. */
} RIL_EVDO_SignalStrength;

typedef struct {
    int signalStrength;  /* Valid values are (0-31, 99) as defined in TS 27.007 8.5 */
    int rsrp;            /* The current Reference Signal Receive Power in dBm multipled by -1.
                          * Range: 44 to 140 dBm
                          * INT_MAX: 0x7FFFFFFF denotes invalid value.
                          * Reference: 3GPP TS 36.133 9.1.4 */
    int rsrq;            /* The current Reference Signal Receive Quality in dB multiplied by -1.
                          * Range: 20 to 3 dB.
                          * INT_MAX: 0x7FFFFFFF denotes invalid value.
                          * Reference: 3GPP TS 36.133 9.1.7 */
    int rssnr;           /* The current reference signal signal-to-noise ratio in 0.1 dB units.
                          * Range: -200 to +300 (-200 = -20.0 dB, +300 = 30dB).
                          * INT_MAX : 0x7FFFFFFF denotes invalid value.
                          * Reference: 3GPP TS 36.101 8.1.1 */
    int cqi;             /* The current Channel Quality Indicator.
                          * Range: 0 to 15.
                          * INT_MAX : 0x7FFFFFFF denotes invalid value.
                          * Reference: 3GPP TS 36.101 9.2, 9.3, A.4 */
} RIL_LTE_SignalStrength;

typedef struct {
    int signalStrength;  /* Valid values are (0-31, 99) as defined in TS 27.007 8.5 */
    int rsrp;            /* The current Reference Signal Receive Power in dBm multipled by -1.
                          * Range: 44 to 140 dBm
                          * INT_MAX: 0x7FFFFFFF denotes invalid value.
                          * Reference: 3GPP TS 36.133 9.1.4 */
    int rsrq;            /* The current Reference Signal Receive Quality in dB multiplied by -1.
                          * Range: 20 to 3 dB.
                          * INT_MAX: 0x7FFFFFFF denotes invalid value.
                          * Reference: 3GPP TS 36.133 9.1.7 */
    int rssnr;           /* The current reference signal signal-to-noise ratio in 0.1 dB units.
                          * Range: -200 to +300 (-200 = -20.0 dB, +300 = 30dB).
                          * INT_MAX : 0x7FFFFFFF denotes invalid value.
                          * Reference: 3GPP TS 36.101 8.1.1 */
    int cqi;             /* The current Channel Quality Indicator.
                          * Range: 0 to 15.
                          * INT_MAX : 0x7FFFFFFF denotes invalid value.
                          * Reference: 3GPP TS 36.101 9.2, 9.3, A.4 */
    int timingAdvance;   /* timing advance in micro seconds for a one way trip from cell to device.
                          * Approximate distance can be calculated using 300m/us * timingAdvance.
                          * Range: 0 to 0x7FFFFFFE
                          * INT_MAX : 0x7FFFFFFF denotes invalid value.
                          * Reference: 3GPP 36.321 section 6.1.3.5
                          * also: http://www.cellular-planningoptimization.com/2010/02/timing-advance-with-calculation.html */
} RIL_LTE_SignalStrength_v8;

typedef struct {
    int rscp;    /* The Received Signal Code Power in dBm multipled by -1.
                  * Range : 25 to 120
                  * INT_MAX: 0x7FFFFFFF denotes invalid value.
                  * Reference: 3GPP TS 25.123, section 9.1.1.1 */
} RIL_TD_SCDMA_SignalStrength;

typedef struct {
    int isGsm;           /* is GSM or not */
    int rssiQdbm;        /* This valus is GSM 3G rssi value */
    int rscpQdbm;        /* This valus is GSM 3G rscp value */
    int Ecn0Qdbm;        /* This valus is GSM 3G ecn0 value */
} RIL_WCDMA_SignalStrength;

/* Deprecated, use RIL_SignalStrength_v6 */
typedef struct {
    RIL_GW_SignalStrength   GW_SignalStrength;
    RIL_CDMA_SignalStrength CDMA_SignalStrength;
    RIL_EVDO_SignalStrength EVDO_SignalStrength;
} RIL_SignalStrength_v5;

typedef struct {
    RIL_GW_SignalStrength    GW_SignalStrength;
    RIL_CDMA_SignalStrength  CDMA_SignalStrength;
    RIL_EVDO_SignalStrength  EVDO_SignalStrength;
    RIL_LTE_SignalStrength   LTE_SignalStrength;
    RIL_WCDMA_SignalStrength WCDMA_SignalStrength;
} RIL_SignalStrength_v6;

typedef struct {
    RIL_GW_SignalStrength       GW_SignalStrength;
    RIL_CDMA_SignalStrength     CDMA_SignalStrength;
    RIL_EVDO_SignalStrength     EVDO_SignalStrength;
    RIL_LTE_SignalStrength_v8   LTE_SignalStrength;
} RIL_SignalStrength_v8;

typedef struct {
    RIL_GW_SignalStrength       GW_SignalStrength;
    RIL_CDMA_SignalStrength     CDMA_SignalStrength;
    RIL_EVDO_SignalStrength     EVDO_SignalStrength;
    RIL_LTE_SignalStrength_v8   LTE_SignalStrength;
    RIL_TD_SCDMA_SignalStrength TD_SCDMA_SignalStrength;
} RIL_SignalStrength_v10;

/** RIL_CellIdentityGsm */
typedef struct {
    int mcc;    /* 3-digit Mobile Country Code, 0..999, INT_MAX if unknown */
    int mnc;    /* 2 or 3-digit Mobile Network Code, 0..999, INT_MAX if unknown */
    int lac;    /* 16-bit Location Area Code, 0..65535, INT_MAX if unknown  */
    int cid;    /* 16-bit GSM Cell Identity described in TS 27.007, 0..65535, INT_MAX if unknown  */
} RIL_CellIdentityGsm;

/** RIL_CellIdentityWcdma */
typedef struct {
    int mcc;    /* 3-digit Mobile Country Code, 0..999, INT_MAX if unknown  */
    int mnc;    /* 2 or 3-digit Mobile Network Code, 0..999, INT_MAX if unknown  */
    int lac;    /* 16-bit Location Area Code, 0..65535, INT_MAX if unknown  */
    int cid;    /* 28-bit UMTS Cell Identity described in TS 25.331, 0..268435455, INT_MAX if unknown  */
    int psc;    /* 9-bit UMTS Primary Scrambling Code described in TS 25.331, 0..511, INT_MAX if unknown */
} RIL_CellIdentityWcdma;

/** RIL_CellIdentityCdma */
typedef struct {
    int networkId;      /* Network Id 0..65535, INT_MAX if unknown */
    int systemId;       /* CDMA System Id 0..32767, INT_MAX if unknown  */
    int basestationId;  /* Base Station Id 0..65535, INT_MAX if unknown  */
    int longitude;      /* Longitude is a decimal number as specified in 3GPP2 C.S0005-A v6.0.
                         * It is represented in units of 0.25 seconds and ranges from -2592000
                         * to 2592000, both values inclusive (corresponding to a range of -180
                         * to +180 degrees). INT_MAX if unknown */

    int latitude;       /* Latitude is a decimal number as specified in 3GPP2 C.S0005-A v6.0.
                         * It is represented in units of 0.25 seconds and ranges from -1296000
                         * to 1296000, both values inclusive (corresponding to a range of -90
                         * to +90 degrees). INT_MAX if unknown */
} RIL_CellIdentityCdma;

/** RIL_CellIdentityLte */
typedef struct {
    int mcc;    /* 3-digit Mobile Country Code, 0..999, INT_MAX if unknown  */
    int mnc;    /* 2 or 3-digit Mobile Network Code, 0..999, INT_MAX if unknown  */
    int ci;     /* 28-bit Cell Identity described in TS ???, INT_MAX if unknown */
    int pci;    /* physical cell id 0..503, INT_MAX if unknown  */
    int tac;    /* 16-bit tracking area code, INT_MAX if unknown  */
} RIL_CellIdentityLte;

/** RIL_CellIdentityTdscdma */
typedef struct {
    int mcc;    /* 3-digit Mobile Country Code, 0..999, INT_MAX if unknown  */
    int mnc;    /* 2 or 3-digit Mobile Network Code, 0..999, INT_MAX if unknown  */
    int lac;    /* 16-bit Location Area Code, 0..65535, INT_MAX if unknown  */
    int cid;    /* 28-bit UMTS Cell Identity described in TS 25.331, 0..268435455, INT_MAX if unknown  */
    int cpid;    /* 8-bit Cell Parameters ID described in TS 25.331, 0..127, INT_MAX if unknown */
} RIL_CellIdentityTdscdma;

/** RIL_CellInfoGsm */
typedef struct {
  RIL_CellIdentityGsm   cellIdentityGsm;
  RIL_GW_SignalStrength signalStrengthGsm;
} RIL_CellInfoGsm;

/** RIL_CellInfoWcdma */
typedef struct {
  RIL_CellIdentityWcdma cellIdentityWcdma;
  RIL_SignalStrengthWcdma signalStrengthWcdma;
} RIL_CellInfoWcdma;

/** RIL_CellInfoCdma */
typedef struct {
  RIL_CellIdentityCdma      cellIdentityCdma;
  RIL_CDMA_SignalStrength   signalStrengthCdma;
  RIL_EVDO_SignalStrength   signalStrengthEvdo;
} RIL_CellInfoCdma;

/** RIL_CellInfoLte */
typedef struct {
  RIL_CellIdentityLte        cellIdentityLte;
  RIL_LTE_SignalStrength_v8  signalStrengthLte;
} RIL_CellInfoLte;

/** RIL_CellInfoTdscdma */
typedef struct {
  RIL_CellIdentityTdscdma cellIdentityTdscdma;
  RIL_TD_SCDMA_SignalStrength signalStrengthTdscdma;
} RIL_CellInfoTdscdma;

// Must be the same as CellInfo.TYPE_XXX
typedef enum {
  RIL_CELL_INFO_TYPE_GSM    = 1,
  RIL_CELL_INFO_TYPE_CDMA   = 2,
  RIL_CELL_INFO_TYPE_LTE    = 3,
  RIL_CELL_INFO_TYPE_WCDMA  = 4,
  RIL_CELL_INFO_TYPE_TD_SCDMA  = 5
} RIL_CellInfoType;

// Must be the same as CellInfo.TIMESTAMP_TYPE_XXX
typedef enum {
    RIL_TIMESTAMP_TYPE_UNKNOWN = 0,
    RIL_TIMESTAMP_TYPE_ANTENNA = 1,
    RIL_TIMESTAMP_TYPE_MODEM = 2,
    RIL_TIMESTAMP_TYPE_OEM_RIL = 3,
    RIL_TIMESTAMP_TYPE_JAVA_RIL = 4,
} RIL_TimeStampType;

typedef struct {
  RIL_CellInfoType  cellInfoType;   /* cell type for selecting from union CellInfo */
  int               registered;     /* !0 if this cell is registered 0 if not registered */
  RIL_TimeStampType timeStampType;  /* type of time stamp represented by timeStamp */
  uint64_t          timeStamp;      /* Time in nanos as returned by ril_nano_time */
  union {
    RIL_CellInfoGsm     gsm;
    RIL_CellInfoCdma    cdma;
    RIL_CellInfoLte     lte;
    RIL_CellInfoWcdma   wcdma;
    RIL_CellInfoTdscdma tdscdma;
  } CellInfo;
} RIL_CellInfo;

/* Names of the CDMA info records (C.S0005 section 3.7.5) */
typedef enum {
  RIL_CDMA_DISPLAY_INFO_REC,
  RIL_CDMA_CALLED_PARTY_NUMBER_INFO_REC,
  RIL_CDMA_CALLING_PARTY_NUMBER_INFO_REC,
  RIL_CDMA_CONNECTED_NUMBER_INFO_REC,
  RIL_CDMA_SIGNAL_INFO_REC,
  RIL_CDMA_REDIRECTING_NUMBER_INFO_REC,
  RIL_CDMA_LINE_CONTROL_INFO_REC,
  RIL_CDMA_EXTENDED_DISPLAY_INFO_REC,
  RIL_CDMA_T53_CLIR_INFO_REC,
  RIL_CDMA_T53_RELEASE_INFO_REC,
  RIL_CDMA_T53_AUDIO_CONTROL_INFO_REC
} RIL_CDMA_InfoRecName;

/* Display Info Rec as defined in C.S0005 section 3.7.5.1
   Extended Display Info Rec as defined in C.S0005 section 3.7.5.16
   Note: the Extended Display info rec contains multiple records of the
   form: display_tag, display_len, and display_len occurrences of the
   chari field if the display_tag is not 10000000 or 10000001.
   To save space, the records are stored consecutively in a byte buffer.
   The display_tag, display_len and chari fields are all 1 byte.
*/

typedef struct {
  char alpha_len;
  char alpha_buf[CDMA_ALPHA_INFO_BUFFER_LENGTH];
} RIL_CDMA_DisplayInfoRecord;

/* Called Party Number Info Rec as defined in C.S0005 section 3.7.5.2
   Calling Party Number Info Rec as defined in C.S0005 section 3.7.5.3
   Connected Number Info Rec as defined in C.S0005 section 3.7.5.4
*/

typedef struct {
  char len;
  char buf[CDMA_NUMBER_INFO_BUFFER_LENGTH];
  char number_type;
  char number_plan;
  char pi;
  char si;
} RIL_CDMA_NumberInfoRecord;

/* Redirecting Number Information Record as defined in C.S0005 section 3.7.5.11 */
typedef enum {
  RIL_REDIRECTING_REASON_UNKNOWN = 0,
  RIL_REDIRECTING_REASON_CALL_FORWARDING_BUSY = 1,
  RIL_REDIRECTING_REASON_CALL_FORWARDING_NO_REPLY = 2,
  RIL_REDIRECTING_REASON_CALLED_DTE_OUT_OF_ORDER = 9,
  RIL_REDIRECTING_REASON_CALL_FORWARDING_BY_THE_CALLED_DTE = 10,
  RIL_REDIRECTING_REASON_CALL_FORWARDING_UNCONDITIONAL = 15,
  RIL_REDIRECTING_REASON_RESERVED
} RIL_CDMA_RedirectingReason;

typedef struct {
  RIL_CDMA_NumberInfoRecord redirectingNumber;
  /* redirectingReason is set to RIL_REDIRECTING_REASON_UNKNOWN if not included */
  RIL_CDMA_RedirectingReason redirectingReason;
} RIL_CDMA_RedirectingNumberInfoRecord;

/* Line Control Information Record as defined in C.S0005 section 3.7.5.15 */
typedef struct {
  char lineCtrlPolarityIncluded;
  char lineCtrlToggle;
  char lineCtrlReverse;
  char lineCtrlPowerDenial;
} RIL_CDMA_LineControlInfoRecord;

/* T53 CLIR Information Record */
typedef struct {
  char cause;
} RIL_CDMA_T53_CLIRInfoRecord;

/* T53 Audio Control Information Record */
typedef struct {
  char upLink;
  char downLink;
} RIL_CDMA_T53_AudioControlInfoRecord;

typedef struct {

  RIL_CDMA_InfoRecName name;

  union {
    /* Display and Extended Display Info Rec */
    RIL_CDMA_DisplayInfoRecord           display;

    /* Called Party Number, Calling Party Number, Connected Number Info Rec */
    RIL_CDMA_NumberInfoRecord            number;

    /* Signal Info Rec */
    RIL_CDMA_SignalInfoRecord            signal;

    /* Redirecting Number Info Rec */
    RIL_CDMA_RedirectingNumberInfoRecord redir;

    /* Line Control Info Rec */
    RIL_CDMA_LineControlInfoRecord       lineCtrl;

    /* T53 CLIR Info Rec */
    RIL_CDMA_T53_CLIRInfoRecord          clir;

    /* T53 Audio Control Info Rec */
    RIL_CDMA_T53_AudioControlInfoRecord  audioCtrl;
  } rec;
} RIL_CDMA_InformationRecord;

#define RIL_CDMA_MAX_NUMBER_OF_INFO_RECS 10

#define RIL_HARDWARE_CONFIG_UUID_LENGTH 64

typedef struct {
  char numberOfInfoRecs;
  RIL_CDMA_InformationRecord infoRec[RIL_CDMA_MAX_NUMBER_OF_INFO_RECS];
} RIL_CDMA_InformationRecords;

/* See RIL_REQUEST_NV_READ_ITEM */
typedef struct {
  RIL_NV_Item itemID;
} RIL_NV_ReadItem;

/* See RIL_REQUEST_NV_WRITE_ITEM */
typedef struct {
  RIL_NV_Item   itemID;
  char *        value;
} RIL_NV_WriteItem;

typedef enum {
    HANDOVER_STARTED = 0,
    HANDOVER_COMPLETED = 1,
    HANDOVER_FAILED = 2,
    HANDOVER_CANCELED = 3
} RIL_SrvccState;

/* hardware configuration reported to RILJ. */
typedef enum {
   RIL_HARDWARE_CONFIG_MODEM = 0,
   RIL_HARDWARE_CONFIG_SIM = 1,
} RIL_HardwareConfig_Type;

typedef enum {
   RIL_HARDWARE_CONFIG_STATE_ENABLED = 0,
   RIL_HARDWARE_CONFIG_STATE_STANDBY = 1,
   RIL_HARDWARE_CONFIG_STATE_DISABLED = 2,
} RIL_HardwareConfig_State;

typedef struct {
   int rilModel;
   uint32_t rat; /* bitset - ref. RIL_RadioTechnology. */
   int maxVoice;
   int maxData;
   int maxStandby;
} RIL_HardwareConfig_Modem;

typedef struct {
   char modemUuid[RIL_HARDWARE_CONFIG_UUID_LENGTH];
} RIL_HardwareConfig_Sim;

typedef struct {
  RIL_HardwareConfig_Type type;
  char uuid[RIL_HARDWARE_CONFIG_UUID_LENGTH];
  RIL_HardwareConfig_State state;
  union {
     RIL_HardwareConfig_Modem modem;
     RIL_HardwareConfig_Sim sim;
  } cfg;
} RIL_HardwareConfig;

typedef enum {
  SS_CFU,
  SS_CF_BUSY,
  SS_CF_NO_REPLY,
  SS_CF_NOT_REACHABLE,
  SS_CF_ALL,
  SS_CF_ALL_CONDITIONAL,
  SS_CLIP,
  SS_CLIR,
  SS_COLP,
  SS_COLR,
  SS_WAIT,
  SS_BAOC,
  SS_BAOIC,
  SS_BAOIC_EXC_HOME,
  SS_BAIC,
  SS_BAIC_ROAMING,
  SS_ALL_BARRING,
  SS_OUTGOING_BARRING,
  SS_INCOMING_BARRING
} RIL_SsServiceType;

typedef enum {
  SS_ACTIVATION,
  SS_DEACTIVATION,
  SS_INTERROGATION,
  SS_REGISTRATION,
  SS_ERASURE
} RIL_SsRequestType;

typedef enum {
  SS_ALL_TELE_AND_BEARER_SERVICES,
  SS_ALL_TELESEVICES,
  SS_TELEPHONY,
  SS_ALL_DATA_TELESERVICES,
  SS_SMS_SERVICES,
  SS_ALL_TELESERVICES_EXCEPT_SMS
} RIL_SsTeleserviceType;

#define SS_INFO_MAX 4
#define NUM_SERVICE_CLASSES 7

typedef struct {
  int numValidIndexes; /* This gives the number of valid values in cfInfo.
                       For example if voice is forwarded to one number and data
                       is forwarded to a different one then numValidIndexes will be
                       2 indicating total number of valid values in cfInfo.
                       Similarly if all the services are forwarded to the same
                       number then the value of numValidIndexes will be 1. */

  RIL_CallForwardInfo cfInfo[NUM_SERVICE_CLASSES]; /* This is the response data
                                                      for SS request to query call
                                                      forward status. see
                                                      RIL_REQUEST_QUERY_CALL_FORWARD_STATUS */
} RIL_CfData;

typedef struct {
  RIL_SsServiceType serviceType;
  RIL_SsRequestType requestType;
  RIL_SsTeleserviceType teleserviceType;
  int serviceClass;
  RIL_Errno result;

  union {
    int ssInfo[SS_INFO_MAX]; /* This is the response data for most of the SS GET/SET
                                RIL requests. E.g. RIL_REQUSET_GET_CLIR returns
                                two ints, so first two values of ssInfo[] will be
                                used for response if serviceType is SS_CLIR and
                                requestType is SS_INTERROGATION */

    RIL_CfData cfData;
  };
} RIL_StkCcUnsolSsResponse;

/**
 * Data connection power state
 */
typedef enum {
    RIL_DC_POWER_STATE_LOW      = 1,        // Low power state
    RIL_DC_POWER_STATE_MEDIUM   = 2,        // Medium power state
    RIL_DC_POWER_STATE_HIGH     = 3,        // High power state
    RIL_DC_POWER_STATE_UNKNOWN  = INT32_MAX // Unknown state
} RIL_DcPowerStates;

/**
 * Data connection real time info
 */
typedef struct {
    uint64_t                    time;       // Time in nanos as returned by ril_nano_time
    RIL_DcPowerStates           powerState; // Current power state
} RIL_DcRtInfo;

/**
 * Data profile to modem
 */
typedef struct {
    /* id of the data profile */
    int profileId;
    /* the APN to connect to */
    char* apn;
    /** one of the PDP_type values in TS 27.007 section 10.1.1.
     * For example, "IP", "IPV6", "IPV4V6", or "PPP".
     */
    char* protocol;
    /** authentication protocol used for this PDP context
     * (None: 0, PAP: 1, CHAP: 2, PAP&CHAP: 3)
     */
    int authType;
    /* the username for APN, or NULL */
    char* user;
    /* the password for APN, or NULL */
    char* password;
    /* the profile type, TYPE_COMMON-0, TYPE_3GPP-1, TYPE_3GPP2-2 */
    int type;
    /* the period in seconds to limit the maximum connections */
    int maxConnsTime;
    /* the maximum connections during maxConnsTime */
    int maxConns;
    /** the required wait time in seconds after a successful UE initiated
     * disconnect of a given PDN connection before the device can send
     * a new PDN connection request for that given PDN
     */
    int waitTime;
    /* true to enable the profile, 0 to disable, 1 to enable */
    int enabled;
} RIL_DataProfileInfo;

/* Tx Power Levels */
#define RIL_NUM_TX_POWER_LEVELS     5

typedef struct {

  /* period (in ms) when modem is power collapsed */
  uint32_t sleep_mode_time_ms;

  /* period (in ms) when modem is awake and in idle mode*/
  uint32_t idle_mode_time_ms;

  /* period (in ms) for which Tx is active */
  uint32_t tx_mode_time_ms[RIL_NUM_TX_POWER_LEVELS];

  /* period (in ms) for which Rx is active */
  uint32_t rx_mode_time_ms;
} RIL_ActivityStatsInfo;

typedef struct {
	int format;
	int vp;
	int pid;
	int dcs;
} RIL_SmsParams;

typedef struct {
    int mode;
    char *channelConfigInfo;
    char *languageConfigInfo;
    int isAllLanguageOn;
} RIL_CBConfigInfo;

typedef struct {
	int warningType;
	int messageId;
	int serialNumber;
	char *plmnId;
	char *securityInfo;
} RIL_CBEtwsNotification;

//[New R8 modem FD]
typedef struct {
    int args_num; //record total number of arguments of this mode
    int mode; //allowed mode:0,1,2,3
    int parameter1; //only mode 2 and 3 has parameter1 this field
    int parameter2;	//only mode2 has parameter2 this field
}RIL_FDModeStructure;

/// M: [mtk03923][20111123][ALPS00093395]ICS Migration - Telephony. @{
#ifdef MTK_RIL
typedef enum {
    RIL_PHB_ADN = 0,
    RIL_PHB_FDN = 1,
    RIL_PHB_MSISDN = 2,
    RIL_PHB_ECC = 3
} RIL_PhbStorageType;

typedef struct {
    int type;                       /* type of the entry, refer to RIL_PhbStorageType */
    int index;                     /* the stored index of the entry */
    char *number;              /* the phone number */
    int ton;                        /* type of the number */
    char * alphaId;             /* the alpha ID, using Hexdecimal coding method */
} RIL_PhbEntryStrucutre;
/// @}
typedef struct {
    char * storage;
    int used;
    int total;
} RIL_PHB_MEM_STORAGE_RESPONSE;

typedef struct {
    int index; // values in the range of location numbers of phonebook memory
    char * number; // phone number of format <type>
    int type; // type of address octet in integer format; default 145 when number include "+", otherwise 129
    char * text; // associated with the number
    int hidden; //0
    char * group;//indicating a group the entry may belong to
    char * adnumber; // an additional number of format<adtype>
    int adtype;//
    char * secondtext; // a second text field associated with the number
    char * email;// email field
} RIL_PHB_ENTRY;

//General SIM Authentication
typedef struct {
    int sessionId;
    int mode;
    char * param1;
    char * param2;
    int tag;
}RIL_SimAuthStructure;

//MTK-START [mtk80776] WiFi Calling
typedef struct {
    int sessionId;  /* for +CRLA	*/
    int command;    /* one of the commands listed for TS 27.007 +CRSM/CRLA*/
    int fileId;     /* EF id */
    char *path;     /* "pathid" from TS 27.007 +CRSM command.
                       Path is in hex asciii format eg "7f205f70"
                       Path must always be provided.
                     */
    int p1;
    int p2;
    int p3;
    char *data;     /* May be NULL*/
    char *pin2;     /* May be NULL*/
    char *aidPtr;   /* AID value, See ETSI 102.221 8.1 and 101.220 4, NULL if no value. */
} RIL_UICC_IO_v6;

typedef struct {
    int session_id;
    char *context1;
    char *context2;
} RIL_UICC_Authentication;

typedef struct {
    char *res;
    char *ck;
    char *ik;
    char *kc;
    char *auts;
} RIL_UICC_AkaAuthenticateResponse;

typedef struct {
    char *res;
    char *auts;
} RIL_UICC_GbaAuthenticateBootstrapResponse;

typedef struct {
    int session_id;
    RIL_SimRefreshResult app_status;
    int *file_id;
} RIL_UICC_ApplicationRefresh;
//MTK-END [mtk80776] WiFi Calling

// External SIM [Start]
typedef struct VsimMessage {
    uint32_t transaction_id;
    uint32_t message_id;
    uint32_t slotId;
    uint32_t length;
} __attribute__((packed)) VsimMessage;

typedef struct RIL_VsimEvent {
    uint32_t transaction_id;
    uint32_t eventId;
    uint32_t sim_type; //0: local sim, 1: remote sim
} RIL_VsimEvent;

typedef struct RIL_VsimOperationEvent {
    uint32_t transaction_id;
    uint32_t eventId; //0: ATR, 1: APDU
    int32_t result; // 0: successful, < 0: error cause
    uint32_t data_length;
    char *data;  // It is ATR in case of reset response, APDU incase of APDU request
} RIL_VsimOperationEvent;

enum MessageId {
    MSG_ID_INITIALIZATION = 1,
    MSG_ID_GET_PLATFORM_CAPABILITY = 2,
    MSG_ID_EVENT_REQUEST = 3,
    MSG_ID_UICC_RESET_RESPONSE = 4,
    MSG_ID_UICC_APDU_RESPONSE = 5,
    MSG_ID_UICC_POWER_DOWN_RESPONSE = 6,
    MSG_ID_GET_SERVICE_STATE_REQUEST = 7,

    MSG_ID_INITIALIZATION_RESPONSE = 1001,
    MSG_ID_GET_PLATFORM_CAPABILITY_RESPONSE = 1002,
    MSG_ID_EVENT_RESPONSE = 1003,
    MSG_ID_UICC_RESET_REQUEST = 1004,
    MSG_ID_UICC_APDU_REQUEST = 1005,
    MSG_ID_UICC_POWER_DOWN_REQUEST = 1006,
    MSG_ID_GET_SERVICE_STATE_RESPONSE = 1007,
};

enum EventTypeId {
    REQUEST_TYPE_ENABLE_EXTERNAL_SIM = 1,
    REQUEST_TYPE_DISABLE_EXTERNAL_SIM = 2,
    REQUEST_TYPE_PLUG_OUT = 3,
    REQUEST_TYPE_PLUG_IN = 4,
    REQUEST_TYPE_ATR_EVENT = 1,
    REQUEST_TYPE_APDU_EVENT = 2,
    REQUEST_TYPE_CARD_POWER_DOWN = 3,
};
// External SIM [End]

/// M: [C2K] IRAT feature. @{
typedef struct {
	int sourceRat;	// Source RAT
	int targetRat;	// Target RAT
	int action;		// IRAT action
	int type;		// IRAT type, from source RAT to target RAT
}RIL_Pdn_IratInfo;
/// @}

#endif

typedef enum {
    GSMPHONE,
    IMSPHONE
} DIAL_SOURCE;

/**
 * RIL_REQUEST_GET_SIM_STATUS
 *
 * Requests status of the SIM interface and the SIM card
 *
 * "data" is NULL
 *
 * "response" is const RIL_CardStatus_v6 *
 *
 * Valid errors:
 *  Must never fail
 */
#define RIL_REQUEST_GET_SIM_STATUS 1

/**
 * RIL_REQUEST_ENTER_SIM_PIN
 *
 * Supplies SIM PIN. Only called if RIL_CardStatus has RIL_APPSTATE_PIN state
 *
 * "data" is const char **
 * ((const char **)data)[0] is PIN value
 * ((const char **)data)[1] is AID value, See ETSI 102.221 8.1 and 101.220 4, NULL if no value.
 *
 * "response" is int *
 * ((int *)response)[0] is the number of retries remaining, or -1 if unknown
 *
 * Valid errors:
 *
 * SUCCESS
 * RADIO_NOT_AVAILABLE (radio resetting)
 * GENERIC_FAILURE
 * PASSWORD_INCORRECT
 */

#define RIL_REQUEST_ENTER_SIM_PIN 2


/**
 * RIL_REQUEST_ENTER_SIM_PUK
 *
 * Supplies SIM PUK and new PIN.
 *
 * "data" is const char **
 * ((const char **)data)[0] is PUK value
 * ((const char **)data)[1] is new PIN value
 * ((const char **)data)[2] is AID value, See ETSI 102.221 8.1 and 101.220 4, NULL if no value.
 *
 * "response" is int *
 * ((int *)response)[0] is the number of retries remaining, or -1 if unknown
 *
 * Valid errors:
 *
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *  PASSWORD_INCORRECT
 *     (PUK is invalid)
 */

#define RIL_REQUEST_ENTER_SIM_PUK 3

/**
 * RIL_REQUEST_ENTER_SIM_PIN2
 *
 * Supplies SIM PIN2. Only called following operation where SIM_PIN2 was
 * returned as a a failure from a previous operation.
 *
 * "data" is const char **
 * ((const char **)data)[0] is PIN2 value
 * ((const char **)data)[1] is AID value, See ETSI 102.221 8.1 and 101.220 4, NULL if no value.
 *
 * "response" is int *
 * ((int *)response)[0] is the number of retries remaining, or -1 if unknown
 *
 * Valid errors:
 *
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *  PASSWORD_INCORRECT
 */

#define RIL_REQUEST_ENTER_SIM_PIN2 4

/**
 * RIL_REQUEST_ENTER_SIM_PUK2
 *
 * Supplies SIM PUK2 and new PIN2.
 *
 * "data" is const char **
 * ((const char **)data)[0] is PUK2 value
 * ((const char **)data)[1] is new PIN2 value
 * ((const char **)data)[2] is AID value, See ETSI 102.221 8.1 and 101.220 4, NULL if no value.
 *
 * "response" is int *
 * ((int *)response)[0] is the number of retries remaining, or -1 if unknown
 *
 * Valid errors:
 *
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *  PASSWORD_INCORRECT
 *     (PUK2 is invalid)
 */

#define RIL_REQUEST_ENTER_SIM_PUK2 5

/**
 * RIL_REQUEST_CHANGE_SIM_PIN
 *
 * Supplies old SIM PIN and new PIN.
 *
 * "data" is const char **
 * ((const char **)data)[0] is old PIN value
 * ((const char **)data)[1] is new PIN value
 * ((const char **)data)[2] is AID value, See ETSI 102.221 8.1 and 101.220 4, NULL if no value.
 *
 * "response" is int *
 * ((int *)response)[0] is the number of retries remaining, or -1 if unknown
 *
 * Valid errors:
 *
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *  PASSWORD_INCORRECT
 *     (old PIN is invalid)
 *
 */

#define RIL_REQUEST_CHANGE_SIM_PIN 6


/**
 * RIL_REQUEST_CHANGE_SIM_PIN2
 *
 * Supplies old SIM PIN2 and new PIN2.
 *
 * "data" is const char **
 * ((const char **)data)[0] is old PIN2 value
 * ((const char **)data)[1] is new PIN2 value
 * ((const char **)data)[2] is AID value, See ETSI 102.221 8.1 and 101.220 4, NULL if no value.
 *
 * "response" is int *
 * ((int *)response)[0] is the number of retries remaining, or -1 if unknown
 *
 * Valid errors:
 *
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *  PASSWORD_INCORRECT
 *     (old PIN2 is invalid)
 *
 */

#define RIL_REQUEST_CHANGE_SIM_PIN2 7

/**
 * RIL_REQUEST_ENTER_NETWORK_DEPERSONALIZATION
 *
 * Requests that network personlization be deactivated
 *
 * "data" is const char **
 * ((const char **)(data))[0]] is network depersonlization code
 *
 * "response" is int *
 * ((int *)response)[0] is the number of retries remaining, or -1 if unknown
 *
 * Valid errors:
 *
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *  PASSWORD_INCORRECT
 *     (code is invalid)
 */

#define RIL_REQUEST_ENTER_NETWORK_DEPERSONALIZATION 8

/**
 * RIL_REQUEST_GET_CURRENT_CALLS
 *
 * Requests current call list
 *
 * "data" is NULL
 *
 * "response" must be a "const RIL_Call **"
 *
 * Valid errors:
 *
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *      (request will be made again in a few hundred msec)
 */

#define RIL_REQUEST_GET_CURRENT_CALLS 9


/**
 * RIL_REQUEST_DIAL
 *
 * Initiate voice call
 *
 * "data" is const RIL_Dial *
 * "response" is NULL
 *
 * This method is never used for supplementary service codes
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  DIAL_MODIFIED_TO_USSD
 *  DIAL_MODIFIED_TO_SS
 *  DIAL_MODIFIED_TO_DIAL
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_DIAL 10

/**
 * RIL_REQUEST_GET_IMSI
 *
 * Get the SIM IMSI
 *
 * Only valid when radio state is "RADIO_STATE_ON"
 *
 * "data" is const char **
 * ((const char **)data)[0] is AID value, See ETSI 102.221 8.1 and 101.220 4, NULL if no value.
 * "response" is a const char * containing the IMSI
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_GET_IMSI 11

/**
 * RIL_REQUEST_HANGUP
 *
 * Hang up a specific line (like AT+CHLD=1x)
 *
 * After this HANGUP request returns, RIL should show the connection is NOT
 * active anymore in next RIL_REQUEST_GET_CURRENT_CALLS query.
 *
 * "data" is an int *
 * (int *)data)[0] contains Connection index (value of 'x' in CHLD above)
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_HANGUP 12

/**
 * RIL_REQUEST_HANGUP_WAITING_OR_BACKGROUND
 *
 * Hang up waiting or held (like AT+CHLD=0)
 *
 * After this HANGUP request returns, RIL should show the connection is NOT
 * active anymore in next RIL_REQUEST_GET_CURRENT_CALLS query.
 *
 * "data" is NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_HANGUP_WAITING_OR_BACKGROUND 13

/**
 * RIL_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND
 *
 * Hang up waiting or held (like AT+CHLD=1)
 *
 * After this HANGUP request returns, RIL should show the connection is NOT
 * active anymore in next RIL_REQUEST_GET_CURRENT_CALLS query.
 *
 * "data" is NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND 14

/**
 * RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE
 *
 * Switch waiting or holding call and active call (like AT+CHLD=2)
 *
 * State transitions should be is follows:
 *
 * If call 1 is waiting and call 2 is active, then if this re
 *
 *   BEFORE                               AFTER
 * Call 1   Call 2                 Call 1       Call 2
 * ACTIVE   HOLDING                HOLDING     ACTIVE
 * ACTIVE   WAITING                HOLDING     ACTIVE
 * HOLDING  WAITING                HOLDING     ACTIVE
 * ACTIVE   IDLE                   HOLDING     IDLE
 * IDLE     IDLE                   IDLE        IDLE
 *
 * "data" is NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE 15
#define RIL_REQUEST_SWITCH_HOLDING_AND_ACTIVE 15

/**
 * RIL_REQUEST_CONFERENCE
 *
 * Conference holding and active (like AT+CHLD=3)

 * "data" is NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_CONFERENCE 16

/**
 * RIL_REQUEST_UDUB
 *
 * Send UDUB (user determined used busy) to ringing or
 * waiting call answer)(RIL_BasicRequest r);
 *
 * "data" is NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_UDUB 17

/**
 * RIL_REQUEST_LAST_CALL_FAIL_CAUSE
 *
 * Requests the failure cause code for the most recently terminated call
 *
 * "data" is NULL
 * "response" is a "int *"
 * ((int *)response)[0] is RIL_LastCallFailCause.  GSM failure reasons are
 * mapped to cause codes defined in TS 24.008 Annex H where possible. CDMA
 * failure reasons are derived from the possible call failure scenarios
 * described in the "CDMA IS-2000 Release A (C.S0005-A v6.0)" standard.
 *
 * The implementation should return CALL_FAIL_ERROR_UNSPECIFIED for blocked
 * MO calls by restricted state (See RIL_UNSOL_RESTRICTED_STATE_CHANGED)
 *
 * If the implementation does not have access to the exact cause codes,
 * then it should return one of the values listed in RIL_LastCallFailCause,
 * as the UI layer needs to distinguish these cases for tone generation or
 * error notification.
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 * See also: RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE
 */
#define RIL_REQUEST_LAST_CALL_FAIL_CAUSE 18

/**
 * RIL_REQUEST_SIGNAL_STRENGTH
 *
 * Requests current signal strength and associated information
 *
 * Must succeed if radio is on.
 *
 * "data" is NULL
 *
 * "response" is a const RIL_SignalStrength *
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 */
#define RIL_REQUEST_SIGNAL_STRENGTH 19

/**
 * RIL_REQUEST_VOICE_REGISTRATION_STATE
 *
 * Request current registration state
 *
 * "data" is NULL
 * "response" is a "char **"
 * ((const char **)response)[0] is registration state 0-6,
 *              0 - Not registered, MT is not currently searching
 *                  a new operator to register
 *              1 - Registered, home network
 *              2 - Not registered, but MT is currently searching
 *                  a new operator to register
 *              3 - Registration denied
 *              4 - Unknown
 *              5 - Registered, roaming
 *             10 - Same as 0, but indicates that emergency calls
 *                  are enabled.
 *             12 - Same as 2, but indicates that emergency calls
 *                  are enabled.
 *             13 - Same as 3, but indicates that emergency calls
 *                  are enabled.
 *             14 - Same as 4, but indicates that emergency calls
 *                  are enabled.
 *
 * ((const char **)response)[1] is LAC if registered on a GSM/WCDMA system or
 *                              NULL if not.Valid LAC are 0x0000 - 0xffff
 * ((const char **)response)[2] is CID if registered on a * GSM/WCDMA or
 *                              NULL if not.
 *                                 Valid CID are 0x00000000 - 0xffffffff
 *                                    In GSM, CID is Cell ID (see TS 27.007)
 *                                            in 16 bits
 *                                    In UMTS, CID is UMTS Cell Identity
 *                                             (see TS 25.331) in 28 bits
 * ((const char **)response)[3] indicates the available voice radio technology,
 *                              valid values as defined by RIL_RadioTechnology.
 * ((const char **)response)[4] is Base Station ID if registered on a CDMA
 *                              system or NULL if not.  Base Station ID in
 *                              decimal format
 * ((const char **)response)[5] is Base Station latitude if registered on a
 *                              CDMA system or NULL if not. Base Station
 *                              latitude is a decimal number as specified in
 *                              3GPP2 C.S0005-A v6.0. It is represented in
 *                              units of 0.25 seconds and ranges from -1296000
 *                              to 1296000, both values inclusive (corresponding
 *                              to a range of -90 to +90 degrees).
 * ((const char **)response)[6] is Base Station longitude if registered on a
 *                              CDMA system or NULL if not. Base Station
 *                              longitude is a decimal number as specified in
 *                              3GPP2 C.S0005-A v6.0. It is represented in
 *                              units of 0.25 seconds and ranges from -2592000
 *                              to 2592000, both values inclusive (corresponding
 *                              to a range of -180 to +180 degrees).
 * ((const char **)response)[7] is concurrent services support indicator if
 *                              registered on a CDMA system 0-1.
 *                                   0 - Concurrent services not supported,
 *                                   1 - Concurrent services supported
 * ((const char **)response)[8] is System ID if registered on a CDMA system or
 *                              NULL if not. Valid System ID are 0 - 32767
 * ((const char **)response)[9] is Network ID if registered on a CDMA system or
 *                              NULL if not. Valid System ID are 0 - 65535
 * ((const char **)response)[10] is the TSB-58 Roaming Indicator if registered
 *                               on a CDMA or EVDO system or NULL if not. Valid values
 *                               are 0-255.
 * ((const char **)response)[11] indicates whether the current system is in the
 *                               PRL if registered on a CDMA or EVDO system or NULL if
 *                               not. 0=not in the PRL, 1=in the PRL
 * ((const char **)response)[12] is the default Roaming Indicator from the PRL,
 *                               if registered on a CDMA or EVDO system or NULL if not.
 *                               Valid values are 0-255.
 * ((const char **)response)[13] if registration state is 3 (Registration
 *                               denied) this is an enumerated reason why
 *                               registration was denied.  See 3GPP TS 24.008,
 *                               10.5.3.6 and Annex G.
 *                                 0 - General
 *                                 1 - Authentication Failure
 *                                 2 - IMSI unknown in HLR
 *                                 3 - Illegal MS
 *                                 4 - Illegal ME
 *                                 5 - PLMN not allowed
 *                                 6 - Location area not allowed
 *                                 7 - Roaming not allowed
 *                                 8 - No Suitable Cells in this Location Area
 *                                 9 - Network failure
 *                                10 - Persistent location update reject
 *                                11 - PLMN not allowed
 *                                12 - Location area not allowed
 *                                13 - Roaming not allowed in this Location Area
 *                                15 - No Suitable Cells in this Location Area
 *                                17 - Network Failure
 *                                20 - MAC Failure
 *                                21 - Sync Failure
 *                                22 - Congestion
 *                                23 - GSM Authentication unacceptable
 *                                25 - Not Authorized for this CSG
 *                                32 - Service option not supported
 *                                33 - Requested service option not subscribed
 *                                34 - Service option temporarily out of order
 *                                38 - Call cannot be identified
 *                                48-63 - Retry upon entry into a new cell
 *                                95 - Semantically incorrect message
 *                                96 - Invalid mandatory information
 *                                97 - Message type non-existent or not implemented
 *                                98 - Message not compatible with protocol state
 *                                99 - Information element non-existent or not implemented
 *                               100 - Conditional IE error
 *                               101 - Message not compatible with protocol state
 *                               111 - Protocol error, unspecified
 * ((const char **)response)[14] is the Primary Scrambling Code of the current
 *                               cell as described in TS 25.331, in hexadecimal
 *                               format, or NULL if unknown or not registered
 *                               to a UMTS network.
 *
 * Please note that registration state 4 ("unknown") is treated
 * as "out of service" in the Android telephony system
 *
 * Registration state 3 can be returned if Location Update Reject
 * (with cause 17 - Network Failure) is received repeatedly from the network,
 * to facilitate "managed roaming"
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_VOICE_REGISTRATION_STATE 20

/**
 * RIL_REQUEST_DATA_REGISTRATION_STATE
 *
 * Request current DATA registration state
 *
 * "data" is NULL
 * "response" is a "char **"
 * ((const char **)response)[0] is registration state 0-5 from TS 27.007 10.1.20 AT+CGREG
 * ((const char **)response)[1] is LAC if registered or NULL if not
 * ((const char **)response)[2] is CID if registered or NULL if not
 * ((const char **)response)[3] indicates the available data radio technology,
 *                              valid values as defined by RIL_RadioTechnology.
 * ((const char **)response)[4] if registration state is 3 (Registration
 *                               denied) this is an enumerated reason why
 *                               registration was denied.  See 3GPP TS 24.008,
 *                               Annex G.6 "Additonal cause codes for GMM".
 *      7 == GPRS services not allowed
 *      8 == GPRS services and non-GPRS services not allowed
 *      9 == MS identity cannot be derived by the network
 *      10 == Implicitly detached
 *      14 == GPRS services not allowed in this PLMN
 *      16 == MSC temporarily not reachable
 *      40 == No PDP context activated
 * ((const char **)response)[5] The maximum number of simultaneous Data Calls that can be
 *                              established using RIL_REQUEST_SETUP_DATA_CALL.
 *
 * The values at offsets 6..10 are optional LTE location information in decimal.
 * If a value is unknown that value may be NULL. If all values are NULL,
 * none need to be present.
 *  ((const char **)response)[6] is TAC, a 16-bit Tracking Area Code.
 *  ((const char **)response)[7] is CID, a 0-503 Physical Cell Identifier.
 *  ((const char **)response)[8] is ECI, a 28-bit E-UTRAN Cell Identifier.
 *  ((const char **)response)[9] is CSGID, a 27-bit Closed Subscriber Group Identity.
 *  ((const char **)response)[10] is TADV, a 6-bit timing advance value.
 *
 * LAC and CID are in hexadecimal format.
 * valid LAC are 0x0000 - 0xffff
 * valid CID are 0x00000000 - 0x0fffffff
 *
 * Please note that registration state 4 ("unknown") is treated
 * as "out of service" in the Android telephony system
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_DATA_REGISTRATION_STATE 21

/**
 * RIL_REQUEST_OPERATOR
 *
 * Request current operator ONS or EONS
 *
 * "data" is NULL
 * "response" is a "const char **"
 * ((const char **)response)[0] is long alpha ONS or EONS
 *                                  or NULL if unregistered
 *
 * ((const char **)response)[1] is short alpha ONS or EONS
 *                                  or NULL if unregistered
 * ((const char **)response)[2] is 5 or 6 digit numeric code (MCC + MNC)
 *                                  or NULL if unregistered
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_OPERATOR 22

/**
 * RIL_REQUEST_RADIO_POWER
 *
 * Toggle radio on and off (for "airplane" mode)
 * If the radio is is turned off/on the radio modem subsystem
 * is expected return to an initialized state. For instance,
 * any voice and data calls will be terminated and all associated
 * lists emptied.
 *
 * "data" is int *
 * ((int *)data)[0] is > 0 for "Radio On"
 * ((int *)data)[0] is == 0 for "Radio Off"
 *
 * "response" is NULL
 *
 * Turn radio on if "on" > 0
 * Turn radio off if "on" == 0
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_RADIO_POWER 23

/**
 * RIL_REQUEST_DTMF
 *
 * Send a DTMF tone
 *
 * If the implementation is currently playing a tone requested via
 * RIL_REQUEST_DTMF_START, that tone should be cancelled and the new tone
 * should be played instead
 *
 * "data" is a char * containing a single character with one of 12 values: 0-9,*,#
 * "response" is NULL
 *
 * FIXME should this block/mute microphone?
 * How does this interact with local DTMF feedback?
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 * See also: RIL_REQUEST_DTMF_STOP, RIL_REQUEST_DTMF_START
 *
 */
#define RIL_REQUEST_DTMF 24

/**
 * RIL_REQUEST_SEND_SMS
 *
 * Send an SMS message
 *
 * "data" is const char **
 * ((const char **)data)[0] is SMSC address in GSM BCD format prefixed
 *      by a length byte (as expected by TS 27.005) or NULL for default SMSC
 * ((const char **)data)[1] is SMS in PDU format as an ASCII hex string
 *      less the SMSC address
 *      TP-Layer-Length is be "strlen(((const char **)data)[1])/2"
 *
 * "response" is a const RIL_SMS_Response *
 *
 * Based on the return error, caller decides to resend if sending sms
 * fails. SMS_SEND_FAIL_RETRY means retry (i.e. error cause is 332)
 * and GENERIC_FAILURE means no retry (i.e. error cause is 500)
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  SMS_SEND_FAIL_RETRY
 *  FDN_CHECK_FAILURE
 *  GENERIC_FAILURE
 *
 * FIXME how do we specify TP-Message-Reference if we need to resend?
 */
#define RIL_REQUEST_SEND_SMS 25


/**
 * RIL_REQUEST_SEND_SMS_EXPECT_MORE
 *
 * Send an SMS message. Identical to RIL_REQUEST_SEND_SMS,
 * except that more messages are expected to be sent soon. If possible,
 * keep SMS relay protocol link open (eg TS 27.005 AT+CMMS command)
 *
 * "data" is const char **
 * ((const char **)data)[0] is SMSC address in GSM BCD format prefixed
 *      by a length byte (as expected by TS 27.005) or NULL for default SMSC
 * ((const char **)data)[1] is SMS in PDU format as an ASCII hex string
 *      less the SMSC address
 *      TP-Layer-Length is be "strlen(((const char **)data)[1])/2"
 *
 * "response" is a const RIL_SMS_Response *
 *
 * Based on the return error, caller decides to resend if sending sms
 * fails. SMS_SEND_FAIL_RETRY means retry (i.e. error cause is 332)
 * and GENERIC_FAILURE means no retry (i.e. error cause is 500)
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  SMS_SEND_FAIL_RETRY
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_SEND_SMS_EXPECT_MORE 26


/**
 * RIL_REQUEST_SETUP_DATA_CALL
 *
 * Setup a packet data connection. If RIL_Data_Call_Response_v6.status
 * return success it is added to the list of data calls and a
 * RIL_UNSOL_DATA_CALL_LIST_CHANGED is sent. The call remains in the
 * list until RIL_REQUEST_DEACTIVATE_DATA_CALL is issued or the
 * radio is powered off/on. This list is returned by RIL_REQUEST_DATA_CALL_LIST
 * and RIL_UNSOL_DATA_CALL_LIST_CHANGED.
 *
 * The RIL is expected to:
 *  - Create one data call context.
 *  - Create and configure a dedicated interface for the context
 *  - The interface must be point to point.
 *  - The interface is configured with one or more addresses and
 *    is capable of sending and receiving packets. The prefix length
 *    of the addresses must be /32 for IPv4 and /128 for IPv6.
 *  - Must NOT change the linux routing table.
 *  - Support up to RIL_REQUEST_DATA_REGISTRATION_STATE response[5]
 *    number of simultaneous data call contexts.
 *
 * "data" is a const char **
 * ((const char **)data)[0] Radio technology to use: 0-CDMA, 1-GSM/UMTS, 2...
 *                          for values above 2 this is RIL_RadioTechnology + 2.
 * ((const char **)data)[1] is a RIL_DataProfile (support is optional)
 * ((const char **)data)[2] is the APN to connect to if radio technology is GSM/UMTS. This APN will
 *                          override the one in the profile. NULL indicates no APN overrride.
 * ((const char **)data)[3] is the username for APN, or NULL
 * ((const char **)data)[4] is the password for APN, or NULL
 * ((const char **)data)[5] is the PAP / CHAP auth type. Values:
 *                          0 => PAP and CHAP is never performed.
 *                          1 => PAP may be performed; CHAP is never performed.
 *                          2 => CHAP may be performed; PAP is never performed.
 *                          3 => PAP / CHAP may be performed - baseband dependent.
 * ((const char **)data)[6] is the connection type to request must be one of the
 *                          PDP_type values in TS 27.007 section 10.1.1.
 *                          For example, "IP", "IPV6", "IPV4V6", or "PPP".
 * ((const char **)data)[7] Optional connection property parameters, format to be defined.
 *
 * "response" is a RIL_Data_Call_Response_v11
 *
 * FIXME may need way to configure QoS settings
 *
 * Valid errors:
 *  SUCCESS should be returned on both success and failure of setup with
 *  the RIL_Data_Call_Response_v6.status containing the actual status.
 *  For all other errors the RIL_Data_Call_Resonse_v6 is ignored.
 *
 *  Other errors could include:
 *    RADIO_NOT_AVAILABLE, GENERIC_FAILURE, OP_NOT_ALLOWED_BEFORE_REG_TO_NW,
 *    OP_NOT_ALLOWED_DURING_VOICE_CALL and REQUEST_NOT_SUPPORTED.
 *
 * See also: RIL_REQUEST_DEACTIVATE_DATA_CALL
 */
#define RIL_REQUEST_SETUP_DATA_CALL 27


/**
 * RIL_REQUEST_SIM_IO
 *
 * Request SIM I/O operation.
 * This is similar to the TS 27.007 "restricted SIM" operation
 * where it assumes all of the EF selection will be done by the
 * callee.
 *
 * "data" is a const RIL_SIM_IO_v6 *
 * Please note that RIL_SIM_IO has a "PIN2" field which may be NULL,
 * or may specify a PIN2 for operations that require a PIN2 (eg
 * updating FDN records)
 *
 * "response" is a const RIL_SIM_IO_Response *
 *
 * Arguments and responses that are unused for certain
 * values of "command" should be ignored or set to NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *  SIM_PIN2
 *  SIM_PUK2
 */
#define RIL_REQUEST_SIM_IO 28

/**
 * RIL_REQUEST_SEND_USSD
 *
 * Send a USSD message
 *
 * If a USSD session already exists, the message should be sent in the
 * context of that session. Otherwise, a new session should be created.
 *
 * The network reply should be reported via RIL_UNSOL_ON_USSD
 *
 * Only one USSD session may exist at a time, and the session is assumed
 * to exist until:
 *   a) The android system invokes RIL_REQUEST_CANCEL_USSD
 *   b) The implementation sends a RIL_UNSOL_ON_USSD with a type code
 *      of "0" (USSD-Notify/no further action) or "2" (session terminated)
 *
 * "data" is a const char * containing the USSD request in UTF-8 format
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  FDN_CHECK_FAILURE
 *  USSD_MODIFIED_TO_DIAL
 *  USSD_MODIFIED_TO_SS
 *  USSD_MODIFIED_TO_USSD
 *  GENERIC_FAILURE
 *
 * See also: RIL_REQUEST_CANCEL_USSD, RIL_UNSOL_ON_USSD
 */

#define RIL_REQUEST_SEND_USSD 29

/**
 * RIL_REQUEST_CANCEL_USSD
 *
 * Cancel the current USSD session if one exists
 *
 * "data" is null
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_CANCEL_USSD 30

/**
 * RIL_REQUEST_GET_CLIR
 *
 * Gets current CLIR status
 * "data" is NULL
 * "response" is int *
 * ((int *)data)[0] is "n" parameter from TS 27.007 7.7
 * ((int *)data)[1] is "m" parameter from TS 27.007 7.7
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  SS_MODIFIED_TO_DIAL
 *  SS_MODIFIED_TO_USSD
 *  SS_MODIFIED_TO_SS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_CLIR 31

/**
 * RIL_REQUEST_SET_CLIR
 *
 * "data" is int *
 * ((int *)data)[0] is "n" parameter from TS 27.007 7.7
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  SS_MODIFIED_TO_DIAL
 *  SS_MODIFIED_TO_USSD
 *  SS_MODIFIED_TO_SS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_CLIR 32

/**
 * RIL_REQUEST_QUERY_CALL_FORWARD_STATUS
 *
 * "data" is const RIL_CallForwardInfo *
 *
 * "response" is const RIL_CallForwardInfo **
 * "response" points to an array of RIL_CallForwardInfo *'s, one for
 * each distinct registered phone number.
 *
 * For example, if data is forwarded to +18005551212 and voice is forwarded
 * to +18005559999, then two separate RIL_CallForwardInfo's should be returned
 *
 * If, however, both data and voice are forwarded to +18005551212, then
 * a single RIL_CallForwardInfo can be returned with the service class
 * set to "data + voice = 3")
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  SS_MODIFIED_TO_DIAL
 *  SS_MODIFIED_TO_USSD
 *  SS_MODIFIED_TO_SS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_QUERY_CALL_FORWARD_STATUS 33


/**
 * RIL_REQUEST_SET_CALL_FORWARD
 *
 * Configure call forward rule
 *
 * "data" is const RIL_CallForwardInfo *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  SS_MODIFIED_TO_DIAL
 *  SS_MODIFIED_TO_USSD
 *  SS_MODIFIED_TO_SS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_CALL_FORWARD 34


/**
 * RIL_REQUEST_QUERY_CALL_WAITING
 *
 * Query current call waiting state
 *
 * "data" is const int *
 * ((const int *)data)[0] is the TS 27.007 service class to query.
 * "response" is a const int *
 * ((const int *)response)[0] is 0 for "disabled" and 1 for "enabled"
 *
 * If ((const int *)response)[0] is = 1, then ((const int *)response)[1]
 * must follow, with the TS 27.007 service class bit vector of services
 * for which call waiting is enabled.
 *
 * For example, if ((const int *)response)[0]  is 1 and
 * ((const int *)response)[1] is 3, then call waiting is enabled for data
 * and voice and disabled for everything else
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  SS_MODIFIED_TO_DIAL
 *  SS_MODIFIED_TO_USSD
 *  SS_MODIFIED_TO_SS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_QUERY_CALL_WAITING 35


/**
 * RIL_REQUEST_SET_CALL_WAITING
 *
 * Configure current call waiting state
 *
 * "data" is const int *
 * ((const int *)data)[0] is 0 for "disabled" and 1 for "enabled"
 * ((const int *)data)[1] is the TS 27.007 service class bit vector of
 *                           services to modify
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  SS_MODIFIED_TO_DIAL
 *  SS_MODIFIED_TO_USSD
 *  SS_MODIFIED_TO_SS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_CALL_WAITING 36

/**
 * RIL_REQUEST_SMS_ACKNOWLEDGE
 *
 * Acknowledge successful or failed receipt of SMS previously indicated
 * via RIL_UNSOL_RESPONSE_NEW_SMS
 *
 * "data" is int *
 * ((int *)data)[0] is 1 on successful receipt
 *                  (basically, AT+CNMA=1 from TS 27.005
 *                  is 0 on failed receipt
 *                  (basically, AT+CNMA=2 from TS 27.005)
 * ((int *)data)[1] if data[0] is 0, this contains the failure cause as defined
 *                  in TS 23.040, 9.2.3.22. Currently only 0xD3 (memory
 *                  capacity exceeded) and 0xFF (unspecified error) are
 *                  reported.
 *
 * "response" is NULL
 *
 * FIXME would like request that specified RP-ACK/RP-ERROR PDU
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SMS_ACKNOWLEDGE  37

/**
 * RIL_REQUEST_GET_IMEI - DEPRECATED
 *
 * Get the device IMEI, including check digit
 *
 * The request is DEPRECATED, use RIL_REQUEST_DEVICE_IDENTITY
 * Valid when RadioState is not RADIO_STATE_UNAVAILABLE
 *
 * "data" is NULL
 * "response" is a const char * containing the IMEI
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_GET_IMEI 38

/**
 * RIL_REQUEST_GET_IMEISV - DEPRECATED
 *
 * Get the device IMEISV, which should be two decimal digits
 *
 * The request is DEPRECATED, use RIL_REQUEST_DEVICE_IDENTITY
 * Valid when RadioState is not RADIO_STATE_UNAVAILABLE
 *
 * "data" is NULL
 * "response" is a const char * containing the IMEISV
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_GET_IMEISV 39


/**
 * RIL_REQUEST_ANSWER
 *
 * Answer incoming call
 *
 * Will not be called for WAITING calls.
 * RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE will be used in this case
 * instead
 *
 * "data" is NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_ANSWER 40

/**
 * RIL_REQUEST_DEACTIVATE_DATA_CALL
 *
 * Deactivate packet data connection and remove from the
 * data call list if SUCCESS is returned. Any other return
 * values should also try to remove the call from the list,
 * but that may not be possible. In any event a
 * RIL_REQUEST_RADIO_POWER off/on must clear the list. An
 * RIL_UNSOL_DATA_CALL_LIST_CHANGED is not expected to be
 * issued because of an RIL_REQUEST_DEACTIVATE_DATA_CALL.
 *
 * "data" is const char **
 * ((char**)data)[0] indicating CID
 * ((char**)data)[1] indicating Disconnect Reason
 *                   0 => No specific reason specified
 *                   1 => Radio shutdown requested
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 * See also: RIL_REQUEST_SETUP_DATA_CALL
 */
#define RIL_REQUEST_DEACTIVATE_DATA_CALL 41

/**
 * RIL_REQUEST_QUERY_FACILITY_LOCK
 *
 * Query the status of a facility lock state
 *
 * "data" is const char **
 * ((const char **)data)[0] is the facility string code from TS 27.007 7.4
 *                      (eg "AO" for BAOC, "SC" for SIM lock)
 * ((const char **)data)[1] is the password, or "" if not required
 * ((const char **)data)[2] is the TS 27.007 service class bit vector of
 *                           services to query
 * ((const char **)data)[3] is AID value, See ETSI 102.221 8.1 and 101.220 4, NULL if no value.
 *                            This is only applicable in the case of Fixed Dialing Numbers
 *                            (FDN) requests.
 *
 * "response" is an int *
 * ((const int *)response) 0 is the TS 27.007 service class bit vector of
 *                           services for which the specified barring facility
 *                           is active. "0" means "disabled for all"
 *
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  SS_MODIFIED_TO_DIAL
 *  SS_MODIFIED_TO_USSD
 *  SS_MODIFIED_TO_SS
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_QUERY_FACILITY_LOCK 42

/**
 * RIL_REQUEST_SET_FACILITY_LOCK
 *
 * Enable/disable one facility lock
 *
 * "data" is const char **
 *
 * ((const char **)data)[0] = facility string code from TS 27.007 7.4
 * (eg "AO" for BAOC)
 * ((const char **)data)[1] = "0" for "unlock" and "1" for "lock"
 * ((const char **)data)[2] = password
 * ((const char **)data)[3] = string representation of decimal TS 27.007
 *                            service class bit vector. Eg, the string
 *                            "1" means "set this facility for voice services"
 * ((const char **)data)[4] = AID value, See ETSI 102.221 8.1 and 101.220 4, NULL if no value.
 *                            This is only applicable in the case of Fixed Dialing Numbers
 *                            (FDN) requests.
 *
 * "response" is int *
 * ((int *)response)[0] is the number of retries remaining, or -1 if unknown
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  SS_MODIFIED_TO_DIAL
 *  SS_MODIFIED_TO_USSD
 *  SS_MODIFIED_TO_SS
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_SET_FACILITY_LOCK 43

/**
 * RIL_REQUEST_CHANGE_BARRING_PASSWORD
 *
 * Change call barring facility password
 *
 * "data" is const char **
 *
 * ((const char **)data)[0] = facility string code from TS 27.007 7.4
 * (eg "AO" for BAOC)
 * ((const char **)data)[1] = old password
 * ((const char **)data)[2] = new password
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  SS_MODIFIED_TO_DIAL
 *  SS_MODIFIED_TO_USSD
 *  SS_MODIFIED_TO_SS
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CHANGE_BARRING_PASSWORD 44

/**
 * RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE
 *
 * Query current network selectin mode
 *
 * "data" is NULL
 *
 * "response" is int *
 * ((const int *)response)[0] is
 *     0 for automatic selection
 *     1 for manual selection
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE 45

/**
 * RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC
 *
 * Specify that the network should be selected automatically
 *
 * "data" is NULL
 * "response" is NULL
 *
 * This request must not respond until the new operator is selected
 * and registered
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  ILLEGAL_SIM_OR_ME
 *  GENERIC_FAILURE
 *
 * Note: Returns ILLEGAL_SIM_OR_ME when the failure is permanent and
 *       no retries needed, such as illegal SIM or ME.
 *       Returns GENERIC_FAILURE for all other causes that might be
 *       fixed by retries.
 *
 */
#define RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC 46

/**
 * RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL
 *
 * Manually select a specified network.
 *
 * "data" is const char * specifying MCCMNC of network to select (eg "310170")
 * "response" is NULL
 *
 * This request must not respond until the new operator is selected
 * and registered
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  ILLEGAL_SIM_OR_ME
 *  GENERIC_FAILURE
 *
 * Note: Returns ILLEGAL_SIM_OR_ME when the failure is permanent and
 *       no retries needed, such as illegal SIM or ME.
 *       Returns GENERIC_FAILURE for all other causes that might be
 *       fixed by retries.
 *
 */
#define RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL 47

/**
 * RIL_REQUEST_QUERY_AVAILABLE_NETWORKS
 *
 * Scans for available networks
 *
 * "data" is NULL
 * "response" is const char ** that should be an array of n*4 strings, where
 *    n is the number of available networks
 * For each available network:
 *
 * ((const char **)response)[n+0] is long alpha ONS or EONS
 * ((const char **)response)[n+1] is short alpha ONS or EONS
 * ((const char **)response)[n+2] is 5 or 6 digit numeric code (MCC + MNC)
 * ((const char **)response)[n+3] is a string value of the status:
 *           "unknown"
 *           "available"
 *           "current"
 *           "forbidden"
 *
 * This request must not respond until the new operator is selected
 * and registered
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_QUERY_AVAILABLE_NETWORKS 48

/**
 * RIL_REQUEST_DTMF_START
 *
 * Start playing a DTMF tone. Continue playing DTMF tone until
 * RIL_REQUEST_DTMF_STOP is received
 *
 * If a RIL_REQUEST_DTMF_START is received while a tone is currently playing,
 * it should cancel the previous tone and play the new one.
 *
 * "data" is a char *
 * ((char *)data)[0] is a single character with one of 12 values: 0-9,*,#
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 * See also: RIL_REQUEST_DTMF, RIL_REQUEST_DTMF_STOP
 */
#define RIL_REQUEST_DTMF_START 49

/**
 * RIL_REQUEST_DTMF_STOP
 *
 * Stop playing a currently playing DTMF tone.
 *
 * "data" is NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 * See also: RIL_REQUEST_DTMF, RIL_REQUEST_DTMF_START
 */
#define RIL_REQUEST_DTMF_STOP 50

/**
 * RIL_REQUEST_BASEBAND_VERSION
 *
 * Return string value indicating baseband version, eg
 * response from AT+CGMR
 *
 * "data" is NULL
 * "response" is const char * containing version string for log reporting
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_BASEBAND_VERSION 51

/**
 * RIL_REQUEST_SEPARATE_CONNECTION
 *
 * Separate a party from a multiparty call placing the multiparty call
 * (less the specified party) on hold and leaving the specified party
 * as the only other member of the current (active) call
 *
 * Like AT+CHLD=2x
 *
 * See TS 22.084 1.3.8.2 (iii)
 * TS 22.030 6.5.5 "Entering "2X followed by send"
 * TS 27.007 "AT+CHLD=2x"
 *
 * "data" is an int *
 * (int *)data)[0] contains Connection index (value of 'x' in CHLD above) "response" is NULL
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SEPARATE_CONNECTION 52


/**
 * RIL_REQUEST_SET_MUTE
 *
 * Turn on or off uplink (microphone) mute.
 *
 * Will only be sent while voice call is active.
 * Will always be reset to "disable mute" when a new voice call is initiated
 *
 * "data" is an int *
 * (int *)data)[0] is 1 for "enable mute" and 0 for "disable mute"
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_SET_MUTE 53

/**
 * RIL_REQUEST_GET_MUTE
 *
 * Queries the current state of the uplink mute setting
 *
 * "data" is NULL
 * "response" is an int *
 * (int *)response)[0] is 1 for "mute enabled" and 0 for "mute disabled"
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  SS_MODIFIED_TO_DIAL
 *  SS_MODIFIED_TO_USSD
 *  SS_MODIFIED_TO_SS
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_GET_MUTE 54

/**
 * RIL_REQUEST_QUERY_CLIP
 *
 * Queries the status of the CLIP supplementary service
 *
 * (for MMI code "*#30#")
 *
 * "data" is NULL
 * "response" is an int *
 * (int *)response)[0] is 1 for "CLIP provisioned"
 *                           and 0 for "CLIP not provisioned"
 *                           and 2 for "unknown, e.g. no network etc"
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */

#define RIL_REQUEST_QUERY_CLIP 55

/**
 * RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE - Deprecated use the status
 * field in RIL_Data_Call_Response_v6.
 *
 * Requests the failure cause code for the most recently failed PDP
 * context or CDMA data connection active
 * replaces RIL_REQUEST_LAST_PDP_FAIL_CAUSE
 *
 * "data" is NULL
 *
 * "response" is a "int *"
 * ((int *)response)[0] is an integer cause code defined in TS 24.008
 *   section 6.1.3.1.3 or close approximation
 *
 * If the implementation does not have access to the exact cause codes,
 * then it should return one of the values listed in
 * RIL_DataCallFailCause, as the UI layer needs to distinguish these
 * cases for error notification
 * and potential retries.
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 * See also: RIL_REQUEST_LAST_CALL_FAIL_CAUSE
 *
 * Deprecated use the status field in RIL_Data_Call_Response_v6.
 */

#define RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE 56

/**
 * RIL_REQUEST_DATA_CALL_LIST
 *
 * Returns the data call list. An entry is added when a
 * RIL_REQUEST_SETUP_DATA_CALL is issued and removed on a
 * RIL_REQUEST_DEACTIVATE_DATA_CALL. The list is emptied
 * when RIL_REQUEST_RADIO_POWER off/on is issued.
 *
 * "data" is NULL
 * "response" is an array of RIL_Data_Call_Response_v6
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *
 * See also: RIL_UNSOL_DATA_CALL_LIST_CHANGED
 */

#define RIL_REQUEST_DATA_CALL_LIST 57

/**
 * RIL_REQUEST_RESET_RADIO - DEPRECATED
 *
 * Request a radio reset. The RIL implementation may postpone
 * the reset until after this request is responded to if the baseband
 * is presently busy.
 *
 * The request is DEPRECATED, use RIL_REQUEST_RADIO_POWER
 *
 * "data" is NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *  REQUEST_NOT_SUPPORTED
 */

#define RIL_REQUEST_RESET_RADIO 58

/**
 * RIL_REQUEST_OEM_HOOK_RAW
 *
 * This request reserved for OEM-specific uses. It passes raw byte arrays
 * back and forth.
 *
 * It can be invoked on the Java side from
 * com.android.internal.telephony.Phone.invokeOemRilRequestRaw()
 *
 * "data" is a char * of bytes copied from the byte[] data argument in java
 * "response" is a char * of bytes that will returned via the
 * caller's "response" Message here:
 * (byte[])(((AsyncResult)response.obj).result)
 *
 * An error response here will result in
 * (((AsyncResult)response.obj).result) == null and
 * (((AsyncResult)response.obj).exception) being an instance of
 * com.android.internal.telephony.gsm.CommandException
 *
 * Valid errors:
 *  All
 */

#define RIL_REQUEST_OEM_HOOK_RAW 59

/**
 * RIL_REQUEST_OEM_HOOK_STRINGS
 *
 * This request reserved for OEM-specific uses. It passes strings
 * back and forth.
 *
 * It can be invoked on the Java side from
 * com.android.internal.telephony.Phone.invokeOemRilRequestStrings()
 *
 * "data" is a const char **, representing an array of null-terminated UTF-8
 * strings copied from the "String[] strings" argument to
 * invokeOemRilRequestStrings()
 *
 * "response" is a const char **, representing an array of null-terminated UTF-8
 * stings that will be returned via the caller's response message here:
 *
 * (String[])(((AsyncResult)response.obj).result)
 *
 * An error response here will result in
 * (((AsyncResult)response.obj).result) == null and
 * (((AsyncResult)response.obj).exception) being an instance of
 * com.android.internal.telephony.gsm.CommandException
 *
 * Valid errors:
 *  All
 */

#define RIL_REQUEST_OEM_HOOK_STRINGS 60

/**
 * RIL_REQUEST_SCREEN_STATE
 *
 * Indicates the current state of the screen.  When the screen is off, the
 * RIL should notify the baseband to suppress certain notifications (eg,
 * signal strength and changes in LAC/CID or BID/SID/NID/latitude/longitude)
 * in an effort to conserve power.  These notifications should resume when the
 * screen is on.
 *
 * "data" is int *
 * ((int *)data)[0] is == 1 for "Screen On"
 * ((int *)data)[0] is == 0 for "Screen Off"
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SCREEN_STATE 61


/**
 * RIL_REQUEST_SET_SUPP_SVC_NOTIFICATION
 *
 * Enables/disables supplementary service related notifications
 * from the network.
 *
 * Notifications are reported via RIL_UNSOL_SUPP_SVC_NOTIFICATION.
 *
 * "data" is int *
 * ((int *)data)[0] is == 1 for notifications enabled
 * ((int *)data)[0] is == 0 for notifications disabled
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 * See also: RIL_UNSOL_SUPP_SVC_NOTIFICATION.
 */
#define RIL_REQUEST_SET_SUPP_SVC_NOTIFICATION 62

/**
 * RIL_REQUEST_WRITE_SMS_TO_SIM
 *
 * Stores a SMS message to SIM memory.
 *
 * "data" is RIL_SMS_WriteArgs *
 *
 * "response" is int *
 * ((const int *)response)[0] is the record index where the message is stored.
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_WRITE_SMS_TO_SIM 63

/**
 * RIL_REQUEST_DELETE_SMS_ON_SIM
 *
 * Deletes a SMS message from SIM memory.
 *
 * "data" is int  *
 * ((int *)data)[0] is the record index of the message to delete.
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_DELETE_SMS_ON_SIM 64

/**
 * RIL_REQUEST_SET_BAND_MODE
 *
 * Assign a specified band for RF configuration.
 *
 * "data" is int *
 * ((int *)data)[0] is == 0 for "unspecified" (selected by baseband automatically)
 * ((int *)data)[0] is == 1 for "EURO band" (GSM-900 / DCS-1800 / WCDMA-IMT-2000)
 * ((int *)data)[0] is == 2 for "US band" (GSM-850 / PCS-1900 / WCDMA-850 / WCDMA-PCS-1900)
 * ((int *)data)[0] is == 3 for "JPN band" (WCDMA-800 / WCDMA-IMT-2000)
 * ((int *)data)[0] is == 4 for "AUS band" (GSM-900 / DCS-1800 / WCDMA-850 / WCDMA-IMT-2000)
 * ((int *)data)[0] is == 5 for "AUS band 2" (GSM-900 / DCS-1800 / WCDMA-850)
 * ((int *)data)[0] is == 6 for "Cellular (800-MHz Band)"
 * ((int *)data)[0] is == 7 for "PCS (1900-MHz Band)"
 * ((int *)data)[0] is == 8 for "Band Class 3 (JTACS Band)"
 * ((int *)data)[0] is == 9 for "Band Class 4 (Korean PCS Band)"
 * ((int *)data)[0] is == 10 for "Band Class 5 (450-MHz Band)"
 * ((int *)data)[0] is == 11 for "Band Class 6 (2-GMHz IMT2000 Band)"
 * ((int *)data)[0] is == 12 for "Band Class 7 (Upper 700-MHz Band)"
 * ((int *)data)[0] is == 13 for "Band Class 8 (1800-MHz Band)"
 * ((int *)data)[0] is == 14 for "Band Class 9 (900-MHz Band)"
 * ((int *)data)[0] is == 15 for "Band Class 10 (Secondary 800-MHz Band)"
 * ((int *)data)[0] is == 16 for "Band Class 11 (400-MHz European PAMR Band)"
 * ((int *)data)[0] is == 17 for "Band Class 15 (AWS Band)"
 * ((int *)data)[0] is == 18 for "Band Class 16 (US 2.5-GHz Band)"
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_BAND_MODE 65

/**
 * RIL_REQUEST_QUERY_AVAILABLE_BAND_MODE
 *
 * Query the list of band mode supported by RF.
 *
 * "data" is NULL
 *
 * "response" is int *
 * "response" points to an array of int's, the int[0] is the size of array, reset is one for
 * each available band mode.
 *
 *  0 for "unspecified" (selected by baseband automatically)
 *  1 for "EURO band" (GSM-900 / DCS-1800 / WCDMA-IMT-2000)
 *  2 for "US band" (GSM-850 / PCS-1900 / WCDMA-850 / WCDMA-PCS-1900)
 *  3 for "JPN band" (WCDMA-800 / WCDMA-IMT-2000)
 *  4 for "AUS band" (GSM-900 / DCS-1800 / WCDMA-850 / WCDMA-IMT-2000)
 *  5 for "AUS band 2" (GSM-900 / DCS-1800 / WCDMA-850)
 *  6 for "Cellular (800-MHz Band)"
 *  7 for "PCS (1900-MHz Band)"
 *  8 for "Band Class 3 (JTACS Band)"
 *  9 for "Band Class 4 (Korean PCS Band)"
 *  10 for "Band Class 5 (450-MHz Band)"
 *  11 for "Band Class 6 (2-GMHz IMT2000 Band)"
 *  12 for "Band Class 7 (Upper 700-MHz Band)"
 *  13 for "Band Class 8 (1800-MHz Band)"
 *  14 for "Band Class 9 (900-MHz Band)"
 *  15 for "Band Class 10 (Secondary 800-MHz Band)"
 *  16 for "Band Class 11 (400-MHz European PAMR Band)"
 *  17 for "Band Class 15 (AWS Band)"
 *  18 for "Band Class 16 (US 2.5-GHz Band)"
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 * See also: RIL_REQUEST_SET_BAND_MODE
 */
#define RIL_REQUEST_QUERY_AVAILABLE_BAND_MODE 66

/**
 * RIL_REQUEST_STK_GET_PROFILE
 *
 * Requests the profile of SIM tool kit.
 * The profile indicates the SAT/USAT features supported by ME.
 * The SAT/USAT features refer to 3GPP TS 11.14 and 3GPP TS 31.111
 *
 * "data" is NULL
 *
 * "response" is a const char * containing SAT/USAT profile
 * in hexadecimal format string starting with first byte of terminal profile
 *
 * Valid errors:
 *  RIL_E_SUCCESS
 *  RIL_E_RADIO_NOT_AVAILABLE (radio resetting)
 *  RIL_E_GENERIC_FAILURE
 */
#define RIL_REQUEST_STK_GET_PROFILE 67

/**
 * RIL_REQUEST_STK_SET_PROFILE
 *
 * Download the STK terminal profile as part of SIM initialization
 * procedure
 *
 * "data" is a const char * containing SAT/USAT profile
 * in hexadecimal format string starting with first byte of terminal profile
 *
 * "response" is NULL
 *
 * Valid errors:
 *  RIL_E_SUCCESS
 *  RIL_E_RADIO_NOT_AVAILABLE (radio resetting)
 *  RIL_E_GENERIC_FAILURE
 */
#define RIL_REQUEST_STK_SET_PROFILE 68

/**
 * RIL_REQUEST_STK_SEND_ENVELOPE_COMMAND
 *
 * Requests to send a SAT/USAT envelope command to SIM.
 * The SAT/USAT envelope command refers to 3GPP TS 11.14 and 3GPP TS 31.111
 *
 * "data" is a const char * containing SAT/USAT command
 * in hexadecimal format string starting with command tag
 *
 * "response" is a const char * containing SAT/USAT response
 * in hexadecimal format string starting with first byte of response
 * (May be NULL)
 *
 * Valid errors:
 *  RIL_E_SUCCESS
 *  RIL_E_RADIO_NOT_AVAILABLE (radio resetting)
 *  RIL_E_GENERIC_FAILURE
 */
#define RIL_REQUEST_STK_SEND_ENVELOPE_COMMAND 69

/**
 * RIL_REQUEST_STK_SEND_TERMINAL_RESPONSE
 *
 * Requests to send a terminal response to SIM for a received
 * proactive command
 *
 * "data" is a const char * containing SAT/USAT response
 * in hexadecimal format string starting with first byte of response data
 *
 * "response" is NULL
 *
 * Valid errors:
 *  RIL_E_SUCCESS
 *  RIL_E_RADIO_NOT_AVAILABLE (radio resetting)
 *  RIL_E_GENERIC_FAILURE
 */
#define RIL_REQUEST_STK_SEND_TERMINAL_RESPONSE 70

/**
 * RIL_REQUEST_STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM
 *
 * When STK application gets RIL_UNSOL_STK_CALL_SETUP, the call actually has
 * been initialized by ME already. (We could see the call has been in the 'call
 * list') So, STK application needs to accept/reject the call according as user
 * operations.
 *
 * "data" is int *
 * ((int *)data)[0] is > 0 for "accept" the call setup
 * ((int *)data)[0] is == 0 for "reject" the call setup
 *
 * "response" is NULL
 *
 * Valid errors:
 *  RIL_E_SUCCESS
 *  RIL_E_RADIO_NOT_AVAILABLE (radio resetting)
 *  RIL_E_GENERIC_FAILURE
 */
#define RIL_REQUEST_STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM 71

/**
 * RIL_REQUEST_EXPLICIT_CALL_TRANSFER
 *
 * Connects the two calls and disconnects the subscriber from both calls.
 *
 * "data" is NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_EXPLICIT_CALL_TRANSFER 72

/**
 * RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE
 *
 * Requests to set the preferred network type for searching and registering
 * (CS/PS domain, RAT, and operation mode)
 *
 * "data" is int * which is RIL_PreferredNetworkType
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *  MODE_NOT_SUPPORTED
 */
#define RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE 73

/**
 * RIL_REQUEST_GET_PREFERRED_NETWORK_TYPE
 *
 * Query the preferred network type (CS/PS domain, RAT, and operation mode)
 * for searching and registering
 *
 * "data" is NULL
 *
 * "response" is int *
 * ((int *)reponse)[0] is == RIL_PreferredNetworkType
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 * See also: RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE
 */
#define RIL_REQUEST_GET_PREFERRED_NETWORK_TYPE 74

/**
 * RIL_REQUEST_NEIGHBORING_CELL_IDS
 *
 * Request neighboring cell id in GSM network
 *
 * "data" is NULL
 * "response" must be a " const RIL_NeighboringCell** "
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_NEIGHBORING_CELL_IDS 75

/**
 * RIL_REQUEST_SET_LOCATION_UPDATES
 *
 * Enables/disables network state change notifications due to changes in
 * LAC and/or CID (for GSM) or BID/SID/NID/latitude/longitude (for CDMA).
 * Basically +CREG=2 vs. +CREG=1 (TS 27.007).
 *
 * Note:  The RIL implementation should default to "updates enabled"
 * when the screen is on and "updates disabled" when the screen is off.
 *
 * "data" is int *
 * ((int *)data)[0] is == 1 for updates enabled (+CREG=2)
 * ((int *)data)[0] is == 0 for updates disabled (+CREG=1)
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 * See also: RIL_REQUEST_SCREEN_STATE, RIL_UNSOL_RESPONSE_NETWORK_STATE_CHANGED
 */
#define RIL_REQUEST_SET_LOCATION_UPDATES 76

/**
 * RIL_REQUEST_CDMA_SET_SUBSCRIPTION_SOURCE
 *
 * Request to set the location where the CDMA subscription shall
 * be retrieved
 *
 * "data" is int *
 * ((int *)data)[0] is == RIL_CdmaSubscriptionSource
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *  SIM_ABSENT
 *  SUBSCRIPTION_NOT_AVAILABLE
 *
 * See also: RIL_REQUEST_CDMA_GET_SUBSCRIPTION_SOURCE
 */
#define RIL_REQUEST_CDMA_SET_SUBSCRIPTION_SOURCE 77

/**
 * RIL_REQUEST_CDMA_SET_ROAMING_PREFERENCE
 *
 * Request to set the roaming preferences in CDMA
 *
 * "data" is int *
 * ((int *)data)[0] is == 0 for Home Networks only, as defined in PRL
 * ((int *)data)[0] is == 1 for Roaming on Affiliated networks, as defined in PRL
 * ((int *)data)[0] is == 2 for Roaming on Any Network, as defined in the PRL
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_CDMA_SET_ROAMING_PREFERENCE 78

/**
 * RIL_REQUEST_CDMA_QUERY_ROAMING_PREFERENCE
 *
 * Request the actual setting of the roaming preferences in CDMA in the modem
 *
 * "data" is NULL
 *
 * "response" is int *
 * ((int *)response)[0] is == 0 for Home Networks only, as defined in PRL
 * ((int *)response)[0] is == 1 for Roaming on Affiliated networks, as defined in PRL
 * ((int *)response)[0] is == 2 for Roaming on Any Network, as defined in the PRL
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_CDMA_QUERY_ROAMING_PREFERENCE 79

/**
 * RIL_REQUEST_SET_TTY_MODE
 *
 * Request to set the TTY mode
 *
 * "data" is int *
 * ((int *)data)[0] is == 0 for TTY off
 * ((int *)data)[0] is == 1 for TTY Full
 * ((int *)data)[0] is == 2 for TTY HCO (hearing carryover)
 * ((int *)data)[0] is == 3 for TTY VCO (voice carryover)
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_TTY_MODE 80

/**
 * RIL_REQUEST_QUERY_TTY_MODE
 *
 * Request the setting of TTY mode
 *
 * "data" is NULL
 *
 * "response" is int *
 * ((int *)response)[0] is == 0 for TTY off
 * ((int *)response)[0] is == 1 for TTY Full
 * ((int *)response)[0] is == 2 for TTY HCO (hearing carryover)
 * ((int *)response)[0] is == 3 for TTY VCO (voice carryover)
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_QUERY_TTY_MODE 81

/**
 * RIL_REQUEST_CDMA_SET_PREFERRED_VOICE_PRIVACY_MODE
 *
 * Request to set the preferred voice privacy mode used in voice
 * scrambling
 *
 * "data" is int *
 * ((int *)data)[0] is == 0 for Standard Privacy Mode (Public Long Code Mask)
 * ((int *)data)[0] is == 1 for Enhanced Privacy Mode (Private Long Code Mask)
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_CDMA_SET_PREFERRED_VOICE_PRIVACY_MODE 82

/**
 * RIL_REQUEST_CDMA_QUERY_PREFERRED_VOICE_PRIVACY_MODE
 *
 * Request the setting of preferred voice privacy mode
 *
 * "data" is NULL
 *
 * "response" is int *
 * ((int *)response)[0] is == 0 for Standard Privacy Mode (Public Long Code Mask)
 * ((int *)response)[0] is == 1 for Enhanced Privacy Mode (Private Long Code Mask)
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_CDMA_QUERY_PREFERRED_VOICE_PRIVACY_MODE 83

/**
 * RIL_REQUEST_CDMA_FLASH
 *
 * Send FLASH
 *
 * "data" is const char *
 * ((const char *)data)[0] is a FLASH string
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CDMA_FLASH 84

/**
 * RIL_REQUEST_CDMA_BURST_DTMF
 *
 * Send DTMF string
 *
 * "data" is const char **
 * ((const char **)data)[0] is a DTMF string
 * ((const char **)data)[1] is the DTMF ON length in milliseconds, or 0 to use
 *                          default
 * ((const char **)data)[2] is the DTMF OFF length in milliseconds, or 0 to use
 *                          default
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CDMA_BURST_DTMF 85

/**
 * RIL_REQUEST_CDMA_VALIDATE_AND_WRITE_AKEY
 *
 * Takes a 26 digit string (20 digit AKEY + 6 digit checksum).
 * If the checksum is valid the 20 digit AKEY is written to NV,
 * replacing the existing AKEY no matter what it was before.
 *
 * "data" is const char *
 * ((const char *)data)[0] is a 26 digit string (ASCII digits '0'-'9')
 *                         where the last 6 digits are a checksum of the
 *                         first 20, as specified in TR45.AHAG
 *                         "Common Cryptographic Algorithms, Revision D.1
 *                         Section 2.2"
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CDMA_VALIDATE_AND_WRITE_AKEY 86

/**
 * RIL_REQUEST_CDMA_SEND_SMS
 *
 * Send a CDMA SMS message
 *
 * "data" is const RIL_CDMA_SMS_Message *
 *
 * "response" is a const RIL_SMS_Response *
 *
 * Based on the return error, caller decides to resend if sending sms
 * fails. The CDMA error class is derived as follows,
 * SUCCESS is error class 0 (no error)
 * SMS_SEND_FAIL_RETRY is error class 2 (temporary failure)
 * and GENERIC_FAILURE is error class 3 (permanent and no retry)
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  SMS_SEND_FAIL_RETRY
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CDMA_SEND_SMS 87

/**
 * RIL_REQUEST_CDMA_SMS_ACKNOWLEDGE
 *
 * Acknowledge the success or failure in the receipt of SMS
 * previously indicated via RIL_UNSOL_RESPONSE_CDMA_NEW_SMS
 *
 * "data" is const RIL_CDMA_SMS_Ack *
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CDMA_SMS_ACKNOWLEDGE 88

/**
 * RIL_REQUEST_GSM_GET_BROADCAST_SMS_CONFIG
 *
 * Request the setting of GSM/WCDMA Cell Broadcast SMS config.
 *
 * "data" is NULL
 *
 * "response" is a const RIL_GSM_BroadcastSmsConfigInfo **
 * "responselen" is count * sizeof (RIL_GSM_BroadcastSmsConfigInfo *)
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_GSM_GET_BROADCAST_SMS_CONFIG 89

/**
 * RIL_REQUEST_GSM_SET_BROADCAST_SMS_CONFIG
 *
 * Set GSM/WCDMA Cell Broadcast SMS config
 *
 * "data" is a const RIL_GSM_BroadcastSmsConfigInfo **
 * "datalen" is count * sizeof(RIL_GSM_BroadcastSmsConfigInfo *)
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_GSM_SET_BROADCAST_SMS_CONFIG 90

/**
 * RIL_REQUEST_GSM_SMS_BROADCAST_ACTIVATION
 *
* Enable or disable the reception of GSM/WCDMA Cell Broadcast SMS
 *
 * "data" is const int *
 * (const int *)data[0] indicates to activate or turn off the
 * reception of GSM/WCDMA Cell Broadcast SMS, 0-1,
 *                       0 - Activate, 1 - Turn off
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_GSM_SMS_BROADCAST_ACTIVATION 91

/**
 * RIL_REQUEST_CDMA_GET_BROADCAST_SMS_CONFIG
 *
 * Request the setting of CDMA Broadcast SMS config
 *
 * "data" is NULL
 *
 * "response" is a const RIL_CDMA_BroadcastSmsConfigInfo **
 * "responselen" is count * sizeof (RIL_CDMA_BroadcastSmsConfigInfo *)
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CDMA_GET_BROADCAST_SMS_CONFIG 92

/**
 * RIL_REQUEST_CDMA_SET_BROADCAST_SMS_CONFIG
 *
 * Set CDMA Broadcast SMS config
 *
 * "data" is an const RIL_CDMA_BroadcastSmsConfigInfo **
 * "datalen" is count * sizeof(const RIL_CDMA_BroadcastSmsConfigInfo *)
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CDMA_SET_BROADCAST_SMS_CONFIG 93

/**
 * RIL_REQUEST_CDMA_SMS_BROADCAST_ACTIVATION
 *
 * Enable or disable the reception of CDMA Broadcast SMS
 *
 * "data" is const int *
 * (const int *)data[0] indicates to activate or turn off the
 * reception of CDMA Broadcast SMS, 0-1,
 *                       0 - Activate, 1 - Turn off
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CDMA_SMS_BROADCAST_ACTIVATION 94

/**
 * RIL_REQUEST_CDMA_SUBSCRIPTION
 *
 * Request the device MDN / H_SID / H_NID.
 *
 * The request is only allowed when CDMA subscription is available.  When CDMA
 * subscription is changed, application layer should re-issue the request to
 * update the subscription information.
 *
 * If a NULL value is returned for any of the device id, it means that error
 * accessing the device.
 *
 * "response" is const char **
 * ((const char **)response)[0] is MDN if CDMA subscription is available
 * ((const char **)response)[1] is a comma separated list of H_SID (Home SID) if
 *                              CDMA subscription is available, in decimal format
 * ((const char **)response)[2] is a comma separated list of H_NID (Home NID) if
 *                              CDMA subscription is available, in decimal format
 * ((const char **)response)[3] is MIN (10 digits, MIN2+MIN1) if CDMA subscription is available
 * ((const char **)response)[4] is PRL version if CDMA subscription is available
 *
 * Valid errors:
 *  SUCCESS
 *  RIL_E_SUBSCRIPTION_NOT_AVAILABLE
 */

#define RIL_REQUEST_CDMA_SUBSCRIPTION 95

/**
 * RIL_REQUEST_CDMA_WRITE_SMS_TO_RUIM
 *
 * Stores a CDMA SMS message to RUIM memory.
 *
 * "data" is RIL_CDMA_SMS_WriteArgs *
 *
 * "response" is int *
 * ((const int *)response)[0] is the record index where the message is stored.
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CDMA_WRITE_SMS_TO_RUIM 96

/**
 * RIL_REQUEST_CDMA_DELETE_SMS_ON_RUIM
 *
 * Deletes a CDMA SMS message from RUIM memory.
 *
 * "data" is int  *
 * ((int *)data)[0] is the record index of the message to delete.
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_CDMA_DELETE_SMS_ON_RUIM 97

/**
 * RIL_REQUEST_DEVICE_IDENTITY
 *
 * Request the device ESN / MEID / IMEI / IMEISV.
 *
 * The request is always allowed and contains GSM and CDMA device identity;
 * it substitutes the deprecated requests RIL_REQUEST_GET_IMEI and
 * RIL_REQUEST_GET_IMEISV.
 *
 * If a NULL value is returned for any of the device id, it means that error
 * accessing the device.
 *
 * When CDMA subscription is changed the ESN/MEID may change.  The application
 * layer should re-issue the request to update the device identity in this case.
 *
 * "response" is const char **
 * ((const char **)response)[0] is IMEI if GSM subscription is available
 * ((const char **)response)[1] is IMEISV if GSM subscription is available
 * ((const char **)response)[2] is ESN if CDMA subscription is available
 * ((const char **)response)[3] is MEID if CDMA subscription is available
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_DEVICE_IDENTITY 98

/**
 * RIL_REQUEST_EXIT_EMERGENCY_CALLBACK_MODE
 *
 * Request the radio's system selection module to exit emergency
 * callback mode.  RIL will not respond with SUCCESS until the modem has
 * completely exited from Emergency Callback Mode.
 *
 * "data" is NULL
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_EXIT_EMERGENCY_CALLBACK_MODE 99

/**
 * RIL_REQUEST_GET_SMSC_ADDRESS
 *
 * Queries the default Short Message Service Center address on the device.
 *
 * "data" is NULL
 *
 * "response" is const char * containing the SMSC address.
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_GET_SMSC_ADDRESS 100

/**
 * RIL_REQUEST_SET_SMSC_ADDRESS
 *
 * Sets the default Short Message Service Center address on the device.
 *
 * "data" is const char * containing the SMSC address.
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_SET_SMSC_ADDRESS 101

/**
 * RIL_REQUEST_REPORT_SMS_MEMORY_STATUS
 *
 * Indicates whether there is storage available for new SMS messages.
 *
 * "data" is int *
 * ((int *)data)[0] is 1 if memory is available for storing new messages
 *                  is 0 if memory capacity is exceeded
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_REPORT_SMS_MEMORY_STATUS 102

/**
 * RIL_REQUEST_REPORT_STK_SERVICE_IS_RUNNING
 *
 * Indicates that the StkSerivce is running and is
 * ready to receive RIL_UNSOL_STK_XXXXX commands.
 *
 * "data" is NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_REPORT_STK_SERVICE_IS_RUNNING 103

/**
 * RIL_REQUEST_CDMA_GET_SUBSCRIPTION_SOURCE
 *
 * Request to query the location where the CDMA subscription shall
 * be retrieved
 *
 * "data" is NULL
 *
 * "response" is int *
 * ((int *)data)[0] is == RIL_CdmaSubscriptionSource
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *  SUBSCRIPTION_NOT_AVAILABLE
 *
 * See also: RIL_REQUEST_CDMA_SET_SUBSCRIPTION_SOURCE
 */
#define RIL_REQUEST_CDMA_GET_SUBSCRIPTION_SOURCE 104

/**
 * RIL_REQUEST_ISIM_AUTHENTICATION
 *
 * Request the ISIM application on the UICC to perform AKA
 * challenge/response algorithm for IMS authentication
 *
 * "data" is a const char * containing the challenge string in Base64 format
 * "response" is a const char * containing the response in Base64 format
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_ISIM_AUTHENTICATION 105

/**
 * RIL_REQUEST_ACKNOWLEDGE_INCOMING_GSM_SMS_WITH_PDU
 *
 * Acknowledge successful or failed receipt of SMS previously indicated
 * via RIL_UNSOL_RESPONSE_NEW_SMS, including acknowledgement TPDU to send
 * as the RP-User-Data element of the RP-ACK or RP-ERROR PDU.
 *
 * "data" is const char **
 * ((const char **)data)[0] is "1" on successful receipt (send RP-ACK)
 *                          is "0" on failed receipt (send RP-ERROR)
 * ((const char **)data)[1] is the acknowledgement TPDU in hexadecimal format
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_ACKNOWLEDGE_INCOMING_GSM_SMS_WITH_PDU 106

/**
 * RIL_REQUEST_STK_SEND_ENVELOPE_WITH_STATUS
 *
 * Requests to send a SAT/USAT envelope command to SIM.
 * The SAT/USAT envelope command refers to 3GPP TS 11.14 and 3GPP TS 31.111.
 *
 * This request has one difference from RIL_REQUEST_STK_SEND_ENVELOPE_COMMAND:
 * the SW1 and SW2 status bytes from the UICC response are returned along with
 * the response data, using the same structure as RIL_REQUEST_SIM_IO.
 *
 * The RIL implementation shall perform the normal processing of a '91XX'
 * response in SW1/SW2 to retrieve the pending proactive command and send it
 * as an unsolicited response, as RIL_REQUEST_STK_SEND_ENVELOPE_COMMAND does.
 *
 * "data" is a const char * containing the SAT/USAT command
 * in hexadecimal format starting with command tag
 *
 * "response" is a const RIL_SIM_IO_Response *
 *
 * Valid errors:
 *  RIL_E_SUCCESS
 *  RIL_E_RADIO_NOT_AVAILABLE (radio resetting)
 *  RIL_E_GENERIC_FAILURE
 */
#define RIL_REQUEST_STK_SEND_ENVELOPE_WITH_STATUS 107

/**
 * RIL_REQUEST_VOICE_RADIO_TECH
 *
 * Query the radio technology type (3GPP/3GPP2) used for voice. Query is valid only
 * when radio state is RADIO_STATE_ON
 *
 * "data" is NULL
 * "response" is int *
 * ((int *) response)[0] is of type const RIL_RadioTechnology
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_VOICE_RADIO_TECH 108

/**
 * RIL_REQUEST_GET_CELL_INFO_LIST
 *
 * Request all of the current cell information known to the radio. The radio
 * must a list of all current cells, including the neighboring cells. If for a particular
 * cell information isn't known then the appropriate unknown value will be returned.
 * This does not cause or change the rate of RIL_UNSOL_CELL_INFO_LIST.
 *
 * "data" is NULL
 *
 * "response" is an array of  RIL_CellInfo.
 */
#define RIL_REQUEST_GET_CELL_INFO_LIST 109

/**
 * RIL_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE
 *
 * Sets the minimum time between when RIL_UNSOL_CELL_INFO_LIST should be invoked.
 * A value of 0, means invoke RIL_UNSOL_CELL_INFO_LIST when any of the reported
 * information changes. Setting the value to INT_MAX(0x7fffffff) means never issue
 * a RIL_UNSOL_CELL_INFO_LIST.
 *
 * "data" is int *
 * ((int *)data)[0] is minimum time in milliseconds
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE 110

/**
 * RIL_REQUEST_SET_INITIAL_ATTACH_APN
 *
 * Set an apn to initial attach network
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *  SUBSCRIPTION_NOT_AVAILABLE
 */
#define RIL_REQUEST_SET_INITIAL_ATTACH_APN 111

/**
 * RIL_REQUEST_IMS_REGISTRATION_STATE
 *
 * Request current IMS registration state
 *
 * "data" is NULL
 *
 * "response" is int *
 * ((int *)response)[0] is registration state:
 *              0 - Not registered
 *              1 - Registered
 *
 * If ((int*)response)[0] is = 1, then ((int *) response)[1]
 * must follow with IMS SMS format:
 *
 * ((int *) response)[1] is of type RIL_RadioTechnologyFamily
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_IMS_REGISTRATION_STATE 112

/**
 * RIL_REQUEST_IMS_SEND_SMS
 *
 * Send a SMS message over IMS
 *
 * "data" is const RIL_IMS_SMS_Message *
 *
 * "response" is a const RIL_SMS_Response *
 *
 * Based on the return error, caller decides to resend if sending sms
 * fails. SMS_SEND_FAIL_RETRY means retry, and other errors means no retry.
 * In case of retry, data is encoded based on Voice Technology available.
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  SMS_SEND_FAIL_RETRY
 *  FDN_CHECK_FAILURE
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_IMS_SEND_SMS 113

/**
 * RIL_REQUEST_SIM_TRANSMIT_APDU_BASIC
 *
 * Request APDU exchange on the basic channel. This command reflects TS 27.007
 * "generic SIM access" operation (+CSIM). The modem must ensure proper function
 * of GSM/CDMA, and filter commands appropriately. It should filter
 * channel management and SELECT by DF name commands.
 *
 * "data" is a const RIL_SIM_APDU *
 * "sessionid" field should be ignored.
 *
 * "response" is a const RIL_SIM_IO_Response *
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SIM_TRANSMIT_APDU_BASIC 114

/**
 * RIL_REQUEST_SIM_OPEN_CHANNEL
 *
 * Open a new logical channel and select the given application. This command
 * reflects TS 27.007 "open logical channel" operation (+CCHO).
 *
 * "data" is const char * and set to AID value, See ETSI 102.221 and 101.220.
 *
 * "response" is int *
 * ((int *)data)[0] contains the session id of the logical channel.
 * ((int *)data)[1] onwards may optionally contain the select response for the
 *     open channel command with one byte per integer.
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *  MISSING_RESOURCE
 *  NO_SUCH_ELEMENT
 */
#define RIL_REQUEST_SIM_OPEN_CHANNEL 115

/**
 * RIL_REQUEST_SIM_CLOSE_CHANNEL
 *
 * Close a previously opened logical channel. This command reflects TS 27.007
 * "close logical channel" operation (+CCHC).
 *
 * "data" is int *
 * ((int *)data)[0] is the session id of logical the channel to close.
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SIM_CLOSE_CHANNEL 116

/**
 * RIL_REQUEST_SIM_TRANSMIT_APDU_CHANNEL
 *
 * Exchange APDUs with a UICC over a previously opened logical channel. This
 * command reflects TS 27.007 "generic logical channel access" operation
 * (+CGLA). The modem should filter channel management and SELECT by DF name
 * commands.
 *
 * "data" is a const RIL_SIM_APDU*
 *
 * "response" is a const RIL_SIM_IO_Response *
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SIM_TRANSMIT_APDU_CHANNEL 117

/**
 * RIL_REQUEST_NV_READ_ITEM
 *
 * Read one of the radio NV items defined in RadioNVItems.java / ril_nv_items.h.
 * This is used for device configuration by some CDMA operators.
 *
 * "data" is a const RIL_NV_ReadItem *
 *
 * "response" is const char * containing the contents of the NV item
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_NV_READ_ITEM 118

/**
 * RIL_REQUEST_NV_WRITE_ITEM
 *
 * Write one of the radio NV items defined in RadioNVItems.java / ril_nv_items.h.
 * This is used for device configuration by some CDMA operators.
 *
 * "data" is a const RIL_NV_WriteItem *
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_NV_WRITE_ITEM 119

/**
 * RIL_REQUEST_NV_WRITE_CDMA_PRL
 *
 * Update the CDMA Preferred Roaming List (PRL) in the radio NV storage.
 * This is used for device configuration by some CDMA operators.
 *
 * "data" is a const char * containing the PRL as a byte array
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_NV_WRITE_CDMA_PRL 120

/**
 * RIL_REQUEST_NV_RESET_CONFIG
 *
 * Reset the radio NV configuration to the factory state.
 * This is used for device configuration by some CDMA operators.
 *
 * "data" is int *
 * ((int *)data)[0] is 1 to reload all NV items
 * ((int *)data)[0] is 2 for erase NV reset (SCRTN)
 * ((int *)data)[0] is 3 for factory reset (RTN)
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_NV_RESET_CONFIG 121

 /** RIL_REQUEST_SET_UICC_SUBSCRIPTION
 * FIXME This API needs to have more documentation.
 *
 * Selection/de-selection of a subscription from a SIM card
 * "data" is const  RIL_SelectUiccSub*

 *
 * "response" is NULL
 *
 *  Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *  SUBSCRIPTION_NOT_SUPPORTED
 *
 */
#define RIL_REQUEST_SET_UICC_SUBSCRIPTION  122

/**
 *  RIL_REQUEST_ALLOW_DATA
 *
 *  Tells the modem whether data calls are allowed or not
 *
 * "data" is int *
 * FIXME slotId and aid will be added.
 * ((int *)data)[0] is == 0 to allow data calls
 * ((int *)data)[0] is == 1 to disallow data calls
 *
 * "response" is NULL
 *
 *  Valid errors:
 *
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *
 */
#define RIL_REQUEST_ALLOW_DATA  123

/**
 * RIL_REQUEST_GET_HARDWARE_CONFIG
 *
 * Request all of the current hardware (modem and sim) associated
 * with the RIL.
 *
 * "data" is NULL
 *
 * "response" is an array of  RIL_HardwareConfig.
 */
#define RIL_REQUEST_GET_HARDWARE_CONFIG 124

/**
 * RIL_REQUEST_SIM_AUTHENTICATION
 *
 * Returns the response of SIM Authentication through RIL to a
 * challenge request.
 *
 * "data" Base64 encoded string containing challenge:
 *      int   authContext;          P2 value of authentication command, see P2 parameter in
 *                                  3GPP TS 31.102 7.1.2
 *      char *authData;             the challenge string in Base64 format, see 3GPP
 *                                  TS 31.102 7.1.2
 *      char *aid;                  AID value, See ETSI 102.221 8.1 and 101.220 4,
 *                                  NULL if no value
 *
 * "response" Base64 encoded strings containing response:
 *      int   sw1;                  Status bytes per 3GPP TS 31.102 section 7.3
 *      int   sw2;
 *      char *simResponse;          Response in Base64 format, see 3GPP TS 31.102 7.1.2
 */
#define RIL_REQUEST_SIM_AUTHENTICATION 125

/**
 * RIL_REQUEST_GET_DC_RT_INFO
 *
 * The request is DEPRECATED, use RIL_REQUEST_GET_ACTIVITY_INFO
 * Requests the Data Connection Real Time Info
 *
 * "data" is NULL
 *
 * "response" is the most recent RIL_DcRtInfo
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *
 * See also: RIL_UNSOL_DC_RT_INFO_CHANGED
 */
#define RIL_REQUEST_GET_DC_RT_INFO 126

/**
 * RIL_REQUEST_SET_DC_RT_INFO_RATE
 *
 * The request is DEPRECATED
 * This is the minimum number of milliseconds between successive
 * RIL_UNSOL_DC_RT_INFO_CHANGED messages and defines the highest rate
 * at which RIL_UNSOL_DC_RT_INFO_CHANGED's will be sent. A value of
 * 0 means send as fast as possible.
 *
 * "data" The number of milliseconds as an int
 *
 * "response" is null
 *
 * Valid errors:
 *  SUCCESS must not fail
 */
#define RIL_REQUEST_SET_DC_RT_INFO_RATE 127

/**
 * RIL_REQUEST_SET_DATA_PROFILE
 *
 * Set data profile in modem
 * Modem should erase existed profiles from framework, and apply new profiles
 * "data" is an const RIL_DataProfileInfo **
 * "datalen" is count * sizeof(const RIL_DataProfileInfo *)
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 *  SUBSCRIPTION_NOT_AVAILABLE
 */
#define RIL_REQUEST_SET_DATA_PROFILE 128

/**
 * RIL_REQUEST_SHUTDOWN
 *
 * Device is shutting down. All further commands are ignored
 * and RADIO_NOT_AVAILABLE must be returned.
 *
 * "data" is null
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SHUTDOWN 129

/**
 * RIL_REQUEST_GET_RADIO_CAPABILITY
 *
 * Used to get phone radio capablility.
 *
 * "data" is int *
 * ((int *)data)[0] is the phone radio access family defined in
 * RadioAccessFamily. It's a bit mask value to represent the support type.
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_RADIO_CAPABILITY 130

/**
 * RIL_REQUEST_SET_RADIO_CAPABILITY
 *
 * Used to set the phones radio capability. Be VERY careful
 * using this request as it may cause some vendor modems to reset. Because
 * of the possible modem reset any RIL commands after this one may not be
 * processed.
 *
 * "data" is the RIL_RadioCapability structure
 *
 * data[0] is the Version of structure, RIL_RADIO_CAPABILITY_VERSION
 * data[1] is the Unique session value defined by framework returned in all "responses/unsol"
 * data[2] is the RadioCapabilityPhase that the value list are CONFIGURED(0)/START(1)/APPLY(2)/FINISH(3)
 * data[3] is the RIL_RadioAccessFamily for the radio
 * data[4] is the Logical modem this radio is be connected to
 * data[5] is the return status and an input parameter for RC_PHASE_FINISH
 *
 * "response" is the RIL_RadioCapability structure, used to feedback return status
 *
 * Valid errors:
 *  SUCCESS means a RIL_UNSOL_RADIO_CAPABILITY will be sent within 30 seconds.
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_RADIO_CAPABILITY 131

/**
 * RIL_REQUEST_START_LCE
 *
 * Start Link Capacity Estimate (LCE) service if supported by the radio.
 *
 * "data" is const int *
 * ((const int*)data)[0] specifies the desired reporting interval (ms).
 * ((const int*)data)[1] specifies the LCE service mode. 1: PULL; 0: PUSH.
 *
 * "response" is the RIL_LceStatusInfo.
 *
 * Valid errors:
 * SUCCESS
 * RADIO_NOT_AVAILABLE
 * LCE_NOT_SUPPORTED
 */
#define RIL_REQUEST_START_LCE 132

/**
 * RIL_REQUEST_STOP_LCE
 *
 * Stop Link Capacity Estimate (LCE) service, the STOP operation should be
 * idempotent for the radio modem.
 *
 * "response" is the RIL_LceStatusInfo.
 *
 * Valid errors:
 * SUCCESS
 * RADIO_NOT_AVAILABLE
 * LCE_NOT_SUPPORTED
 */
#define RIL_REQUEST_STOP_LCE 133

/**
 * RIL_REQUEST_PULL_LCEDATA
 *
 * Pull LCE service for capacity information.
 *
 * "response" is the RIL_LceDataInfo.
 *
 * Valid errors:
 * SUCCESS
 * RADIO_NOT_AVAILABLE
 * LCE_NOT_SUPPORTED
 */
#define RIL_REQUEST_PULL_LCEDATA 134

/**
 * RIL_REQUEST_GET_ACTIVITY_INFO
 *
 * Get modem activity statisitics info.
 *
 * There can be multiple RIL_REQUEST_GET_ACTIVITY_INFO calls to modem.
 * Once the response for the request is sent modem will clear
 * current statistics information.
 *
 * "data" is null
 * "response" is const RIL_ActivityStatsInfo *
 *
 * Valid errors:
 *
 * SUCCESS
 * RADIO_NOT_AVAILABLE (radio resetting)
 * GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_ACTIVITY_INFO 135

/***********************************************************************/


#define RIL_UNSOL_RESPONSE_BASE 1000

/**
 * RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED
 *
 * Indicate when value of RIL_RadioState has changed.
 *
 * Callee will invoke RIL_RadioStateRequest method on main thread
 *
 * "data" is NULL
 */

#define RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED 1000


/**
 * RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED
 *
 * Indicate when call state has changed
 *
 * Callee will invoke RIL_REQUEST_GET_CURRENT_CALLS on main thread
 *
 * "data" is NULL
 *
 * Response should be invoked on, for example,
 * "RING", "BUSY", "NO CARRIER", and also call state
 * transitions (DIALING->ALERTING ALERTING->ACTIVE)
 *
 * Redundent or extraneous invocations are tolerated
 */
#define RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED 1001


/**
 * RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED
 *
 * Called when the voice network state changed
 *
 * Callee will invoke the following requests on main thread:
 *
 * RIL_REQUEST_VOICE_REGISTRATION_STATE
 * RIL_REQUEST_OPERATOR
 *
 * "data" is NULL
 *
 * FIXME should this happen when SIM records are loaded? (eg, for
 * EONS)
 */
#define RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED 1002

/**
 * RIL_UNSOL_RESPONSE_NEW_SMS
 *
 * Called when new SMS is received.
 *
 * "data" is const char *
 * This is a pointer to a string containing the PDU of an SMS-DELIVER
 * as an ascii string of hex digits. The PDU starts with the SMSC address
 * per TS 27.005 (+CMT:)
 *
 * Callee will subsequently confirm the receipt of thei SMS with a
 * RIL_REQUEST_SMS_ACKNOWLEDGE
 *
 * No new RIL_UNSOL_RESPONSE_NEW_SMS
 * or RIL_UNSOL_RESPONSE_NEW_SMS_STATUS_REPORT messages should be sent until a
 * RIL_REQUEST_SMS_ACKNOWLEDGE has been received
 */

#define RIL_UNSOL_RESPONSE_NEW_SMS 1003

/**
 * RIL_UNSOL_RESPONSE_NEW_SMS_STATUS_REPORT
 *
 * Called when new SMS Status Report is received.
 *
 * "data" is const char *
 * This is a pointer to a string containing the PDU of an SMS-STATUS-REPORT
 * as an ascii string of hex digits. The PDU starts with the SMSC address
 * per TS 27.005 (+CDS:).
 *
 * Callee will subsequently confirm the receipt of the SMS with a
 * RIL_REQUEST_SMS_ACKNOWLEDGE
 *
 * No new RIL_UNSOL_RESPONSE_NEW_SMS
 * or RIL_UNSOL_RESPONSE_NEW_SMS_STATUS_REPORT messages should be sent until a
 * RIL_REQUEST_SMS_ACKNOWLEDGE has been received
 */

#define RIL_UNSOL_RESPONSE_NEW_SMS_STATUS_REPORT 1004

/**
 * RIL_UNSOL_RESPONSE_NEW_SMS_ON_SIM
 *
 * Called when new SMS has been stored on SIM card
 *
 * "data" is const int *
 * ((const int *)data)[0] contains the slot index on the SIM that contains
 * the new message
 */

#define RIL_UNSOL_RESPONSE_NEW_SMS_ON_SIM 1005

/**
 * RIL_UNSOL_ON_USSD
 *
 * Called when a new USSD message is received.
 *
 * "data" is const char **
 * ((const char **)data)[0] points to a type code, which is
 *  one of these string values:
 *      "0"   USSD-Notify -- text in ((const char **)data)[1]
 *      "1"   USSD-Request -- text in ((const char **)data)[1]
 *      "2"   Session terminated by network
 *      "3"   other local client (eg, SIM Toolkit) has responded
 *      "4"   Operation not supported
 *      "5"   Network timeout
 *
 * The USSD session is assumed to persist if the type code is "1", otherwise
 * the current session (if any) is assumed to have terminated.
 *
 * ((const char **)data)[1] points to a message string if applicable, which
 * should always be in UTF-8.
 */
#define RIL_UNSOL_ON_USSD 1006
/* Previously #define RIL_UNSOL_ON_USSD_NOTIFY 1006   */

/**
 * RIL_UNSOL_ON_USSD_REQUEST
 *
 * Obsolete. Send via RIL_UNSOL_ON_USSD
 */
#define RIL_UNSOL_ON_USSD_REQUEST 1007

/**
 * RIL_UNSOL_NITZ_TIME_RECEIVED
 *
 * Called when radio has received a NITZ time message
 *
 * "data" is const char * pointing to NITZ time string
 * in the form "yy/mm/dd,hh:mm:ss(+/-)tz,dt"
 */
#define RIL_UNSOL_NITZ_TIME_RECEIVED  1008

/**
 * RIL_UNSOL_SIGNAL_STRENGTH
 *
 * Radio may report signal strength rather han have it polled.
 *
 * "data" is a const RIL_SignalStrength *
 */
#define RIL_UNSOL_SIGNAL_STRENGTH  1009


/**
 * RIL_UNSOL_DATA_CALL_LIST_CHANGED
 *
 * "data" is an array of RIL_Data_Call_Response_v6 identical to that
 * returned by RIL_REQUEST_DATA_CALL_LIST. It is the complete list
 * of current data contexts including new contexts that have been
 * activated. A data call is only removed from this list when the
 * framework sends a RIL_REQUEST_DEACTIVATE_DATA_CALL or the radio
 * is powered off/on.
 *
 * See also: RIL_REQUEST_DATA_CALL_LIST
 */

#define RIL_UNSOL_DATA_CALL_LIST_CHANGED 1010

/**
 * RIL_UNSOL_SUPP_SVC_NOTIFICATION
 *
 * Reports supplementary service related notification from the network.
 *
 * "data" is a const RIL_SuppSvcNotification *
 *
 */

#define RIL_UNSOL_SUPP_SVC_NOTIFICATION 1011

/**
 * RIL_UNSOL_STK_SESSION_END
 *
 * Indicate when STK session is terminated by SIM.
 *
 * "data" is NULL
 */
#define RIL_UNSOL_STK_SESSION_END 1012

/**
 * RIL_UNSOL_STK_PROACTIVE_COMMAND
 *
 * Indicate when SIM issue a STK proactive command to applications
 *
 * "data" is a const char * containing SAT/USAT proactive command
 * in hexadecimal format string starting with command tag
 *
 */
#define RIL_UNSOL_STK_PROACTIVE_COMMAND 1013

/**
 * RIL_UNSOL_STK_EVENT_NOTIFY
 *
 * Indicate when SIM notifies applcations some event happens.
 * Generally, application does not need to have any feedback to
 * SIM but shall be able to indicate appropriate messages to users.
 *
 * "data" is a const char * containing SAT/USAT commands or responses
 * sent by ME to SIM or commands handled by ME, in hexadecimal format string
 * starting with first byte of response data or command tag
 *
 */
#define RIL_UNSOL_STK_EVENT_NOTIFY 1014

/**
 * RIL_UNSOL_STK_CALL_SETUP
 *
 * Indicate when SIM wants application to setup a voice call.
 *
 * "data" is const int *
 * ((const int *)data)[0] contains timeout value (in milliseconds)
 */
#define RIL_UNSOL_STK_CALL_SETUP 1015

/**
 * RIL_UNSOL_SIM_SMS_STORAGE_FULL
 *
 * Indicates that SMS storage on the SIM is full.  Sent when the network
 * attempts to deliver a new SMS message.  Messages cannot be saved on the
 * SIM until space is freed.  In particular, incoming Class 2 messages
 * cannot be stored.
 *
 * "data" is null
 *
 */
#define RIL_UNSOL_SIM_SMS_STORAGE_FULL 1016

/**
 * RIL_UNSOL_SIM_REFRESH
 *
 * Indicates that file(s) on the SIM have been updated, or the SIM
 * has been reinitialized.
 *
 * In the case where RIL is version 6 or older:
 * "data" is an int *
 * ((int *)data)[0] is a RIL_SimRefreshResult.
 * ((int *)data)[1] is the EFID of the updated file if the result is
 * SIM_FILE_UPDATE or NULL for any other result.
 *
 * In the case where RIL is version 7:
 * "data" is a RIL_SimRefreshResponse_v7 *
 *
 * Note: If the SIM state changes as a result of the SIM refresh (eg,
 * SIM_READY -> SIM_LOCKED_OR_ABSENT), RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED
 * should be sent.
 */
#define RIL_UNSOL_SIM_REFRESH 1017

/**
 * RIL_UNSOL_CALL_RING
 *
 * Ring indication for an incoming call (eg, RING or CRING event).
 * There must be at least one RIL_UNSOL_CALL_RING at the beginning
 * of a call and sending multiple is optional. If the system property
 * ro.telephony.call_ring.multiple is false then the upper layers
 * will generate the multiple events internally. Otherwise the vendor
 * ril must generate multiple RIL_UNSOL_CALL_RING if
 * ro.telephony.call_ring.multiple is true or if it is absent.
 *
 * The rate of these events is controlled by ro.telephony.call_ring.delay
 * and has a default value of 3000 (3 seconds) if absent.
 *
 * "data" is null for GSM
 * "data" is const RIL_CDMA_SignalInfoRecord * if CDMA
 */
#define RIL_UNSOL_CALL_RING 1018

/**
 * RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED
 *
 * Indicates that SIM state changes.
 *
 * Callee will invoke RIL_REQUEST_GET_SIM_STATUS on main thread

 * "data" is null
 */
#define RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED 1019

/**
 * RIL_UNSOL_RESPONSE_CDMA_NEW_SMS
 *
 * Called when new CDMA SMS is received
 *
 * "data" is const RIL_CDMA_SMS_Message *
 *
 * Callee will subsequently confirm the receipt of the SMS with
 * a RIL_REQUEST_CDMA_SMS_ACKNOWLEDGE
 *
 * No new RIL_UNSOL_RESPONSE_CDMA_NEW_SMS should be sent until
 * RIL_REQUEST_CDMA_SMS_ACKNOWLEDGE has been received
 *
 */
#define RIL_UNSOL_RESPONSE_CDMA_NEW_SMS 1020

/**
 * RIL_UNSOL_RESPONSE_NEW_BROADCAST_SMS
 *
 * Called when new Broadcast SMS is received
 *
 * "data" can be one of the following:
 * If received from GSM network, "data" is const char of 88 bytes
 * which indicates each page of a CBS Message sent to the MS by the
 * BTS as coded in 3GPP 23.041 Section 9.4.1.2.
 * If received from UMTS network, "data" is const char of 90 up to 1252
 * bytes which contain between 1 and 15 CBS Message pages sent as one
 * packet to the MS by the BTS as coded in 3GPP 23.041 Section 9.4.2.2.
 *
 */
#define RIL_UNSOL_RESPONSE_NEW_BROADCAST_SMS 1021

/**
 * RIL_UNSOL_CDMA_RUIM_SMS_STORAGE_FULL
 *
 * Indicates that SMS storage on the RUIM is full.  Messages
 * cannot be saved on the RUIM until space is freed.
 *
 * "data" is null
 *
 */
#define RIL_UNSOL_CDMA_RUIM_SMS_STORAGE_FULL 1022

/**
 * RIL_UNSOL_RESTRICTED_STATE_CHANGED
 *
 * Indicates a restricted state change (eg, for Domain Specific Access Control).
 *
 * Radio need send this msg after radio off/on cycle no matter it is changed or not.
 *
 * "data" is an int *
 * ((int *)data)[0] contains a bitmask of RIL_RESTRICTED_STATE_* values.
 */
#define RIL_UNSOL_RESTRICTED_STATE_CHANGED 1023

/**
 * RIL_UNSOL_ENTER_EMERGENCY_CALLBACK_MODE
 *
 * Indicates that the radio system selection module has
 * autonomously entered emergency callback mode.
 *
 * "data" is null
 *
 */
#define RIL_UNSOL_ENTER_EMERGENCY_CALLBACK_MODE 1024

/**
 * RIL_UNSOL_CDMA_CALL_WAITING
 *
 * Called when CDMA radio receives a call waiting indication.
 *
 * "data" is const RIL_CDMA_CallWaiting *
 *
 */
#define RIL_UNSOL_CDMA_CALL_WAITING 1025

/**
 * RIL_UNSOL_CDMA_OTA_PROVISION_STATUS
 *
 * Called when CDMA radio receives an update of the progress of an
 * OTASP/OTAPA call.
 *
 * "data" is const int *
 *  For CDMA this is an integer OTASP/OTAPA status listed in
 *  RIL_CDMA_OTA_ProvisionStatus.
 *
 */
#define RIL_UNSOL_CDMA_OTA_PROVISION_STATUS 1026

/**
 * RIL_UNSOL_CDMA_INFO_REC
 *
 * Called when CDMA radio receives one or more info recs.
 *
 * "data" is const RIL_CDMA_InformationRecords *
 *
 */
#define RIL_UNSOL_CDMA_INFO_REC 1027

/**
 * RIL_UNSOL_OEM_HOOK_RAW
 *
 * This is for OEM specific use.
 *
 * "data" is a byte[]
 */
#define RIL_UNSOL_OEM_HOOK_RAW 1028

/**
 * RIL_UNSOL_RINGBACK_TONE
 *
 * Indicates that nework doesn't have in-band information,  need to
 * play out-band tone.
 *
 * "data" is an int *
 * ((int *)data)[0] == 0 for stop play ringback tone.
 * ((int *)data)[0] == 1 for start play ringback tone.
 */
#define RIL_UNSOL_RINGBACK_TONE 1029

/**
 * RIL_UNSOL_RESEND_INCALL_MUTE
 *
 * Indicates that framework/application need reset the uplink mute state.
 *
 * There may be situations where the mute state becomes out of sync
 * between the application and device in some GSM infrastructures.
 *
 * "data" is null
 */
#define RIL_UNSOL_RESEND_INCALL_MUTE 1030

/**
 * RIL_UNSOL_CDMA_SUBSCRIPTION_SOURCE_CHANGED
 *
 * Called when CDMA subscription source changed.
 *
 * "data" is int *
 * ((int *)data)[0] is == RIL_CdmaSubscriptionSource
 */
#define RIL_UNSOL_CDMA_SUBSCRIPTION_SOURCE_CHANGED 1031

/**
 * RIL_UNSOL_CDMA_PRL_CHANGED
 *
 * Called when PRL (preferred roaming list) changes.
 *
 * "data" is int *
 * ((int *)data)[0] is PRL_VERSION as would be returned by RIL_REQUEST_CDMA_SUBSCRIPTION
 */
#define RIL_UNSOL_CDMA_PRL_CHANGED 1032

/**
 * RIL_UNSOL_EXIT_EMERGENCY_CALLBACK_MODE
 *
 * Called when Emergency Callback Mode Ends
 *
 * Indicates that the radio system selection module has
 * proactively exited emergency callback mode.
 *
 * "data" is NULL
 *
 */
#define RIL_UNSOL_EXIT_EMERGENCY_CALLBACK_MODE 1033

/**
 * RIL_UNSOL_RIL_CONNECTED
 *
 * Called the ril connects and returns the version
 *
 * "data" is int *
 * ((int *)data)[0] is RIL_VERSION
 */
#define RIL_UNSOL_RIL_CONNECTED 1034

/**
 * RIL_UNSOL_VOICE_RADIO_TECH_CHANGED
 *
 * Indicates that voice technology has changed. Contains new radio technology
 * as a data in the message.
 *
 * "data" is int *
 * ((int *)data)[0] is of type const RIL_RadioTechnology
 *
 */
#define RIL_UNSOL_VOICE_RADIO_TECH_CHANGED 1035

/**
 * RIL_UNSOL_CELL_INFO_LIST
 *
 * Same information as returned by RIL_REQUEST_GET_CELL_INFO_LIST, but returned
 * at the rate no greater than specified by RIL_REQUEST_SET_UNSOL_CELL_INFO_RATE.
 *
 * "data" is NULL
 *
 * "response" is an array of RIL_CellInfo.
 */
#define RIL_UNSOL_CELL_INFO_LIST 1036

/**
 * RIL_UNSOL_RESPONSE_IMS_NETWORK_STATE_CHANGED
 *
 * Called when IMS registration state has changed
 *
 * To get IMS registration state and IMS SMS format, callee needs to invoke the
 * following request on main thread:
 *
 * RIL_REQUEST_IMS_REGISTRATION_STATE
 *
 * "data" is NULL
 *
 */
#define RIL_UNSOL_RESPONSE_IMS_NETWORK_STATE_CHANGED 1037

/**
 * RIL_UNSOL_UICC_SUBSCRIPTION_STATUS_CHANGED
 *
 * Indicated when there is a change in subscription status.
 * This event will be sent in the following scenarios
 *  - subscription readiness at modem, which was selected by telephony layer
 *  - when subscription is deactivated by modem due to UICC card removal
 *  - When network invalidates the subscription i.e. attach reject due to authentication reject
 *
 * "data" is const int *
 * ((const int *)data)[0] == 0 for Subscription Deactivated
 * ((const int *)data)[0] == 1 for Subscription Activated
 *
 */
#define RIL_UNSOL_UICC_SUBSCRIPTION_STATUS_CHANGED 1038

/**
 * RIL_UNSOL_SRVCC_STATE_NOTIFY
 *
 * Called when Single Radio Voice Call Continuity(SRVCC)
 * progress state has changed
 *
 * "data" is int *
 * ((int *)data)[0] is of type const RIL_SrvccState
 *
 */

#define RIL_UNSOL_SRVCC_STATE_NOTIFY 1039

/**
 * RIL_UNSOL_HARDWARE_CONFIG_CHANGED
 *
 * Called when the hardware configuration associated with the RILd changes
 *
 * "data" is an array of RIL_HardwareConfig
 *
 */
#define RIL_UNSOL_HARDWARE_CONFIG_CHANGED 1040

/**
 * RIL_UNSOL_DC_RT_INFO_CHANGED
 *
 * The message is DEPRECATED, use RIL_REQUEST_GET_ACTIVITY_INFO
 * Sent when the DC_RT_STATE changes but the time
 * between these messages must not be less than the
 * value set by RIL_REQUEST_SET_DC_RT_RATE.
 *
 * "data" is the most recent RIL_DcRtInfo
 *
 */
#define RIL_UNSOL_DC_RT_INFO_CHANGED 1041

/**
 * RIL_UNSOL_RADIO_CAPABILITY
 *
 * Sent when RIL_REQUEST_SET_RADIO_CAPABILITY completes.
 * Returns the phone radio capability exactly as
 * RIL_REQUEST_GET_RADIO_CAPABILITY and should be the
 * same set as sent by RIL_REQUEST_SET_RADIO_CAPABILITY.
 *
 * "data" is the RIL_RadioCapability structure
 *
 * data[0] is the Version of structure, RIL_RADIO_CAPABILITY_VERSION
 * data[1] is the Unique session value defined by framework returned in all "responses/unsol"
 * data[2] is the RadioCapabilityPhase that the value list are CONFIGURED(0)/START(1)/APPLY(2)/FINISH(3)
 * data[3] is the RIL_RadioAccessFamily for the radio
 * data[4] is the Logical modem this radio is be connected to
 * data[5] is the return status and an input parameter for RC_PHASE_FINISH
 *
 */
#define RIL_UNSOL_RADIO_CAPABILITY 1042

/*
 * RIL_UNSOL_ON_SS
 *
 * Called when SS response is received when DIAL/USSD/SS is changed to SS by
 * call control.
 *
 * "data" is const RIL_StkCcUnsolSsResponse *
 *
 */
#define RIL_UNSOL_ON_SS 1043

/**
 * RIL_UNSOL_STK_CC_ALPHA_NOTIFY
 *
 * Called when there is an ALPHA from UICC during Call Control.
 *
 * "data" is const char * containing ALPHA string from UICC in UTF-8 format.
 *
 */
#define RIL_UNSOL_STK_CC_ALPHA_NOTIFY 1044

/**
 * RIL_UNSOL_LCEDATA_RECV
 *
 * Called when there is an incoming Link Capacity Estimate (LCE) info report.
 *
 * "data" is the RIL_LceDataInfo structure.
 *
 */
#define RIL_UNSOL_LCEDATA_RECV 1045

/***********************************************************************/

#define RIL_REQUEST_VENDOR_BASE 2000

/**
 * RIL_REQUEST_GET_COLP
 *
 * Gets current COLP status
 * "data" is NULL
 * "response" is int *
 * ((int *)data)[0] is "n" parameter from TS 27.007 7.8
 * ((int *)data)[1] is "m" parameter from TS 27.007 7.8
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_COLP (RIL_REQUEST_VENDOR_BASE + 0)


/**
 * RIL_REQUEST_SET_COLP
 *
 * "data" is int *
 * ((int *)data)[0] is "n" parameter from TS 27.007 7.8
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_COLP (RIL_REQUEST_VENDOR_BASE + 1)

/**
 * RIL_REQUEST_GET_COLR
 *
 * Gets current COLR status
 * "data" is NULL
 * "response" is int *
 * ((int *)data)[0] is "n" parameter for provision status (0: Not Provisioned 1: Provisioned 2: Unknown)
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_COLR (RIL_REQUEST_VENDOR_BASE + 2)

/**
 * RIL_REQUEST_GET_CCM
 *
 * get current call meter
 *
 * "data" is NULL
 * "response" is a const char * containing the CCM
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_CCM (RIL_REQUEST_VENDOR_BASE + 3)

/**
 * RIL_REQUEST_GET_ACM
 *
 * get accumulated call meter
 *
 * "data" is NULL
 * "response" is a const char * containing the ACM
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_ACM (RIL_REQUEST_VENDOR_BASE + 4)

/**
 * RIL_REQUEST_GET_ACMMAX
 *
 * get the maximum of accumulated call meter
 *
 * "data" is NULL
 * "response" is a const char * containing the ACMMAX
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_ACMMAX (RIL_REQUEST_VENDOR_BASE + 5)

/**
 * RIL_REQUEST_GET_PPU_AND_CURRENCY
 *
 * get price per unit and currency
 *
 * "data" is NULL
 * "response" is a const char * containing the
 * price per unit and currency
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_GET_PPU_AND_CURRENCY (RIL_REQUEST_VENDOR_BASE + 6)

/**
 * RIL_REQUEST_SET_ACMMAX
 *
 * set maximum value of ACM
 *
 * ((const char **)data)[0] = acmmax
 * ((const char **)data)[1] = pin2
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_ACMMAX (RIL_REQUEST_VENDOR_BASE + 7)

/**
 * RIL_REQUEST_RESET_ACM
 *
 * reset value of ACM
 *
 * ((const char **)data)[0] = pin2
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_RESET_ACM (RIL_REQUEST_VENDOR_BASE + 8)

/**
 * RIL_REQUEST_SET_PPU_AND_CURRENCY
 *
 * set ppu and currency
 *
 * ((const char **)data)[0] = currency
 * ((const char **)data)[1] = ppu
 * ((const char **)data)[2] = pin2
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_PPU_AND_CURRENCY (RIL_REQUEST_VENDOR_BASE + 9)


/**
 * RIL_REQUEST_MODEM_POWEROFF
 *
 * Power off modem
 * "data" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_MODEM_POWEROFF (RIL_REQUEST_VENDOR_BASE + 10)

/**
 * RIL_REQUEST_DUAL_SIM_MODE_SWITCH
 *
 * Dual SIM mode switch
 * "data" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_DUAL_SIM_MODE_SWITCH (RIL_REQUEST_VENDOR_BASE + 11)

/**
 * RIL_REQUEST_QUERY_PHB_STORAGE_INFO
 *
 * query the information of specified storage
 *
 * "data" is int *
 * ((int *)data)[0] is the type of the storage, refer to RIL_PhbStorageType
 *
 * "response" is a "int *"
 * ((int *)response)[0] is current number of used entries of the storage.
 * ((int *)response)[1] is number of total entries of the storage.
 * ((int *)response)[2] is maximum supported length of phone number.
 * ((int *)response)[3] is maximum supported length of alpha id.
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_QUERY_PHB_STORAGE_INFO (RIL_REQUEST_VENDOR_BASE + 12)

/**
 * RIL_REQUEST_WRITE_PHB_ENTRY
 *
 * Write a phb entry into specified storage
 *
 * "data" is a const RIL_PhbEntryStrucutre *
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *  DIAL_STRING_TOO_LONG
 *  TEXT_STRING_TOO_LONG
 *  SIM_PIN2
 *  SIM_PUK2
 *  SIM_MEM_FULL
 */
#define RIL_REQUEST_WRITE_PHB_ENTRY (RIL_REQUEST_VENDOR_BASE + 13)

/**
 * RIL_REQUEST_READ_PHB_ENTRY
 *
 * Read a phb entry from specified storage and index
 *
 * "data" is int *
 * ((int *)data)[0] is the type of the storage, refer to RIL_PhbStorageType
 * ((int *)data)[1] is the begin index of the storage to be read
 * ((int *)data)[2] is the end index of the storage to be read
 *
 * "response" is "RIL_PhbEntryStrucutre **"
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_READ_PHB_ENTRY (RIL_REQUEST_VENDOR_BASE + 14)

/**
 * RIL_REQUEST_SET_GPRS_CONNECT_TYPE
 *
 * Set GPRS connect type
 *
 * "data" is a int
  * ((int *)data)[0]  0:WHEN_NEEDED , 1: ALWAYS
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_GPRS_CONNECT_TYPE (RIL_REQUEST_VENDOR_BASE + 15)

/**
 * RIL_REQUEST_SET_GPRS_TRANSFER_TYPE
 *
 * Set GPRS connect type
 *
 * "data" is a int
  * ((int *)data)[0]  0:data prefer , 1: call prefer
 *
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_SET_GPRS_TRANSFER_TYPE (RIL_REQUEST_VENDOR_BASE + 16)

#define RIL_REQUEST_MOBILEREVISION_AND_IMEI  (RIL_REQUEST_VENDOR_BASE + 17)
#define RIL_REQUEST_QUERY_SIM_NETWORK_LOCK	(RIL_REQUEST_VENDOR_BASE + 18)
#define RIL_REQUEST_SET_SIM_NETWORK_LOCK	(RIL_REQUEST_VENDOR_BASE + 19)
#define RIL_REQUEST_SET_SCRI	(RIL_REQUEST_VENDOR_BASE + 20)

#define RIL_REQUEST_BTSIM_CONNECT     (RIL_REQUEST_VENDOR_BASE + 21)
#define RIL_REQUEST_BTSIM_DISCONNECT_OR_POWEROFF   (RIL_REQUEST_VENDOR_BASE + 22)
#define RIL_REQUEST_BTSIM_POWERON_OR_RESETSIM   (RIL_REQUEST_VENDOR_BASE + 23)
#define RIL_REQUEST_BTSIM_TRANSFERAPDU   (RIL_REQUEST_VENDOR_BASE + 24)

/**
 * RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_ACT
 *
  * Manually select a specified network.
  *
  * "data" is const char * specifying MCCMNC of network to select (eg "310170")
  * and a int specifying Act type. (eg 0 is GSM)
  * "response" is NULL
  *
  * This request must not respond until the new operator is selected
  * and registered
  *
  * Valid errors:
  *  SUCCESS
  *  RADIO_NOT_AVAILABLE
  *  ILLEGAL_SIM_OR_ME
  *  GENERIC_FAILURE
  *
  * Note: Returns ILLEGAL_SIM_OR_ME when the failure is permanent and
  * 	  no retries needed, such as illegal SIM or ME.
  * 	  Returns GENERIC_FAILURE for all other causes that might be
  * 	  fixed by retries.
  *
  */
#define RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL_WITH_ACT   (RIL_REQUEST_VENDOR_BASE + 25)

/**
 * RIL_REQUEST_QUERY_ICCID
 *
  */
#define RIL_REQUEST_QUERY_ICCID   (RIL_REQUEST_VENDOR_BASE + 26)
#define RIL_REQUEST_USIM_AUTHENTICATION      (RIL_REQUEST_VENDOR_BASE + 27)

#define RIL_REQUEST_MODEM_POWERON   (RIL_REQUEST_VENDOR_BASE + 28)

/**
 * RIL_REQUEST_GET_SMS_SIM_MEM_STATUS
 *
  * GET SMS SIM Card Memory's total and used number.
  *
  * "data" is const RIL_SMS_Memory_Status, contain the total and used SIM Card SMS number.
  * "response" is NULL.
  *
  * Valid errors:
  *  SUCCESS
  *  RADIO_NOT_AVAILABLE
  *  GENERIC_FAILURE
  */
#define RIL_REQUEST_GET_SMS_SIM_MEM_STATUS   (RIL_REQUEST_VENDOR_BASE + 29)
#define RIL_REQUEST_GET_PHONE_CAPABILITY (RIL_REQUEST_VENDOR_BASE + 30)
#define RIL_REQUEST_SET_PHONE_CAPABILITY (RIL_REQUEST_VENDOR_BASE + 31)
#define RIL_REQUEST_GET_POL_CAPABILITY (RIL_REQUEST_VENDOR_BASE + 32)
#define RIL_REQUEST_GET_POL_LIST (RIL_REQUEST_VENDOR_BASE + 33)
#define RIL_REQUEST_SET_POL_ENTRY (RIL_REQUEST_VENDOR_BASE + 34)
#define RIL_REQUEST_QUERY_UPB_CAPABILITY (RIL_REQUEST_VENDOR_BASE + 35)
#define RIL_REQUEST_EDIT_UPB_ENTRY (RIL_REQUEST_VENDOR_BASE + 36)
#define RIL_REQUEST_DELETE_UPB_ENTRY (RIL_REQUEST_VENDOR_BASE + 37)
#define RIL_REQUEST_READ_UPB_GAS_LIST (RIL_REQUEST_VENDOR_BASE + 38)
#define RIL_REQUEST_READ_UPB_GRP  (RIL_REQUEST_VENDOR_BASE + 39)
#define RIL_REQUEST_WRITE_UPB_GRP  (RIL_REQUEST_VENDOR_BASE + 40)
#define RIL_REQUEST_SET_SIM_RECOVERY_ON (RIL_REQUEST_VENDOR_BASE + 41)
#define RIL_REQUEST_GET_SIM_RECOVERY_ON (RIL_REQUEST_VENDOR_BASE + 42)
#define RIL_REQUEST_SET_TRM (RIL_REQUEST_VENDOR_BASE + 43)
#define RIL_REQUEST_DETECT_SIM_MISSING (RIL_REQUEST_VENDOR_BASE + 44)

/* whether download calibration data or not */
#define RIL_REQUEST_GET_CALIBRATION_DATA (RIL_REQUEST_VENDOR_BASE + 45)
#define RIL_REQUEST_GET_PHB_STRING_LENGTH (RIL_REQUEST_VENDOR_BASE + 46)

/**
 * at+cpbs?
 */
#define RIL_REQUEST_GET_PHB_MEM_STORAGE (RIL_REQUEST_VENDOR_BASE + 47)

/**
 * at+cpbs=<storage><passwork>
 */
#define RIL_REQUEST_SET_PHB_MEM_STORAGE (RIL_REQUEST_VENDOR_BASE + 48)

/**
 * at+cpbr=<index1>,<index2>
 * +CPBR:<indexn>,<number>,<type>,<text>,<hidden>,<group>,<adnumber>,<adtype>,<secondtext>,<email>
 * see RIL_PHB_ENTRY
 */
#define RIL_REQUEST_READ_PHB_ENTRY_EXT (RIL_REQUEST_VENDOR_BASE + 49)
/**
 * at+cpbw=<index>,<number>,<type>,<text>,<hidden>,<group>,<adnumber>,<adtype>,<secondtext>,<email>
 * see RIL_PHB_ENTRY
 */
#define RIL_REQUEST_WRITE_PHB_ENTRY_EXT (RIL_REQUEST_VENDOR_BASE + 50)

//support LGE API, 3gpp 27.007
/*
* Get sms revelant parameters from EFsmsp
*/
#define RIL_REQUEST_GET_SMS_PARAMS (RIL_REQUEST_VENDOR_BASE + 51)

/*
* Set sms revelant parameters into EFsmsp
*/
#define RIL_REQUEST_SET_SMS_PARAMS (RIL_REQUEST_VENDOR_BASE + 52)

// NFC SEEK start
// "data" is a const RIL_SIM_IO *
// "response" is a const RIL_SIM_IO_Response *
#define RIL_REQUEST_SIM_TRANSMIT_BASIC (RIL_REQUEST_VENDOR_BASE + 53)

// "data" is a const RIL_SIM_IO *
// "response" is a const RIL_SIM_IO_Response *
#define RIL_REQUEST_SIM_TRANSMIT_CHANNEL (RIL_REQUEST_VENDOR_BASE + 54)

// "data" is NULL *
// "response" is a const char * containing ATR in hexadecimal format
#define RIL_REQUEST_SIM_GET_ATR (RIL_REQUEST_VENDOR_BASE + 55)
// NFC SEEK end

// SMS part, CB extension
#define RIL_REQUEST_SET_CB_CHANNEL_CONFIG_INFO   (RIL_REQUEST_VENDOR_BASE + 56)
#define RIL_REQUEST_SET_CB_LANGUAGE_CONFIG_INFO  (RIL_REQUEST_VENDOR_BASE + 57)
#define RIL_REQUEST_GET_CB_CONFIG_INFO           (RIL_REQUEST_VENDOR_BASE + 58)
#define RIL_REQUEST_SET_ALL_CB_LANGUAGE_ON       (RIL_REQUEST_VENDOR_BASE + 59)
// SMS part, CB extension

#define RIL_REQUEST_SET_ETWS (RIL_REQUEST_VENDOR_BASE + 60)

//[New R8 modem FD]
//"data" is a const RIL_FD_MODE *
//"response" is a int
#define RIL_REQUEST_SET_FD_MODE (RIL_REQUEST_VENDOR_BASE + 61)

// detach PS service request
#define RIL_REQUEST_DETACH_PS (RIL_REQUEST_VENDOR_BASE + 62)

// NFC seek
// "data" is a const char * containing the AID of the applet
// "response" is a const RIL_SIM_IO_Response *
#define RIL_REQUEST_SIM_OPEN_CHANNEL_WITH_SW (RIL_REQUEST_VENDOR_BASE + 63) // NFC SEEK

#define RIL_REQUEST_SET_REG_SUSPEND_ENABLED (RIL_REQUEST_VENDOR_BASE + 64)
#define RIL_REQUEST_RESUME_REGISTRATION (RIL_REQUEST_VENDOR_BASE + 65)
#define RIL_REQUEST_STORE_MODEM_TYPE (RIL_REQUEST_VENDOR_BASE + 66)
#define RIL_REQUEST_QUERY_MODEM_TYPE (RIL_REQUEST_VENDOR_BASE + 67)

#define RIL_REQUEST_SIM_INTERFACE_SWITCH (RIL_REQUEST_VENDOR_BASE + 68)

// WiFi Calling start
#define RIL_REQUEST_UICC_SELECT_APPLICATION (RIL_REQUEST_VENDOR_BASE + 69)

#define RIL_REQUEST_UICC_DEACTIVATE_APPLICATION (RIL_REQUEST_VENDOR_BASE + 70)

#define RIL_REQUEST_UICC_APPLICATION_IO (RIL_REQUEST_VENDOR_BASE + 71)

#define RIL_REQUEST_UICC_AKA_AUTHENTICATE (RIL_REQUEST_VENDOR_BASE + 72)

#define RIL_REQUEST_UICC_GBA_AUTHENTICATE_BOOTSTRAP (RIL_REQUEST_VENDOR_BASE + 73)

#define RIL_REQUEST_UICC_GBA_AUTHENTICATE_NAF (RIL_REQUEST_VENDOR_BASE + 74)

#define RIL_REQUEST_STK_EVDL_CALL_BY_AP (RIL_REQUEST_VENDOR_BASE + 75)

// Femtocell (CSG)
#define RIL_REQUEST_GET_FEMTOCELL_LIST (RIL_REQUEST_VENDOR_BASE + 76)
#define RIL_REQUEST_ABORT_FEMTOCELL_LIST (RIL_REQUEST_VENDOR_BASE + 77)
#define RIL_REQUEST_SELECT_FEMTOCELL (RIL_REQUEST_VENDOR_BASE + 78)
//Network OPLMN
#define RIL_REQUEST_SEND_OPLMN (RIL_REQUEST_VENDOR_BASE + 79)
#define RIL_REQUEST_GET_OPLMN_VERSION (RIL_REQUEST_VENDOR_BASE + 80)
//PLMN List abort
#define RIL_REQUEST_ABORT_QUERY_AVAILABLE_NETWORKS (RIL_REQUEST_VENDOR_BASE + 81)
// Dial up for CSD
#define RIL_REQUEST_DIAL_UP_CSD (RIL_REQUEST_VENDOR_BASE + 82)
#define RIL_REQUEST_SET_TELEPHONY_MODE (RIL_REQUEST_VENDOR_BASE + 83)

/* M: call control part start */
/**
 * RIL_REQUEST_HANGUP_ALL
 *
 * Hang up all (like ATH, but use AT+CHLD=6 to prevent channel limitation)
 * For ATH, the channel usd to setup call and release must be the same.
 * AT+CHLD=6 has no such limitation
 *
 * "data" is NULL
 * "response" is NULL
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_HANGUP_ALL (RIL_REQUEST_VENDOR_BASE + 84)
#define RIL_REQUEST_FORCE_RELEASE_CALL (RIL_REQUEST_VENDOR_BASE + 85)
#define RIL_REQUEST_SET_CALL_INDICATION (RIL_REQUEST_VENDOR_BASE + 86)

/**
 * RIL_REQUEST_EMERGENCY_DIAL
 *
 * Initiate emergency call
 *
 * "data" is const RIL_Dial *
 * "response" is NULL
 *
 * This method is never used for supplementary service codes
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE (radio resetting)
 *  GENERIC_FAILURE
 */
#define RIL_REQUEST_EMERGENCY_DIAL (RIL_REQUEST_VENDOR_BASE + 87)
#define RIL_REQUEST_SET_ECC_SERVICE_CATEGORY (RIL_REQUEST_VENDOR_BASE + 88)
#define RIL_REQUEST_SET_ECC_LIST (RIL_REQUEST_VENDOR_BASE + 89)
/* M: call control part end */

//New SIM Authentication
#define RIL_REQUEST_GENERAL_SIM_AUTH (RIL_REQUEST_VENDOR_BASE + 90)
//ISIM
#define RIL_REQUEST_OPEN_ICC_APPLICATION (RIL_REQUEST_VENDOR_BASE + 91)
#define RIL_REQUEST_GET_ICC_APPLICATION_STATUS (RIL_REQUEST_VENDOR_BASE + 92)
#define RIL_REQUEST_SIM_IO_EX (RIL_REQUEST_VENDOR_BASE + 93)

// IMS
#define RIL_REQUEST_SET_IMS_ENABLE (RIL_REQUEST_VENDOR_BASE + 94)

/**
 * RIL_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT
 *
 * Scans for available networks with Act
 *
 * "data" is NULL
 * "response" is const char ** that should be an array of n*4 strings, where
 *    n is the number of available networks
 * For each available network:
 *
 * ((const char **)response)[n+0] is long alpha ONS or EONS (present only if NITZ operator name exist)
 * ((const char **)response)[n+1] is short alpha ONS or EONS (present only if NITZ operator name exist)
 * ((const char **)response)[n+2] is 5 or 6 digit numeric code (MCC + MNC)
 * ((const char **)response)[n+3] is a string value of the status:
 *           "unknown"
 *           "available"
 *           "current"
 *           "forbidden"
 * ((const char **)response)[n+4] is a string value of the Act: "2G","3G","4G"
 *
 * This request must not respond until the new operator is selected
 * and registered
 *
 * Valid errors:
 *  SUCCESS
 *  RADIO_NOT_AVAILABLE
 *  GENERIC_FAILURE
 *  RIL_E_CANCELLED
 *
 */
#define RIL_REQUEST_QUERY_AVAILABLE_NETWORKS_WITH_ACT (RIL_REQUEST_VENDOR_BASE + 95)

/* M: SS part */
///M: For query CNAP
#define RIL_REQUEST_SEND_CNAP (RIL_REQUEST_VENDOR_BASE + 96)
#define RIL_REQUEST_SET_CLIP (RIL_REQUEST_VENDOR_BASE + 97)
/* M: SS part end */

/** M: VoLTE data start */
#define RIL_REQUEST_SETUP_DEDICATE_DATA_CALL (RIL_REQUEST_VENDOR_BASE + 98)
#define RIL_REQUEST_DEACTIVATE_DEDICATE_DATA_CALL (RIL_REQUEST_VENDOR_BASE + 99)
#define RIL_REQUEST_MODIFY_DATA_CALL (RIL_REQUEST_VENDOR_BASE + 100)
#define RIL_REQUEST_ABORT_SETUP_DATA_CALL (RIL_REQUEST_VENDOR_BASE + 101)
#define RIL_REQUEST_PCSCF_DISCOVERY_PCO (RIL_REQUEST_VENDOR_BASE + 102)
#define RIL_REQUEST_CLEAR_DATA_BEARER (RIL_REQUEST_VENDOR_BASE + 103)
/** M: VoLTE data end */

// SMS part, CB extension
#define RIL_REQUEST_REMOVE_CB_MESSAGE (RIL_REQUEST_VENDOR_BASE + 104)
// SMS part, CB extension

// NAS configuration for voice call
// 0: voice centric
// 1: data centric
#define RIL_REQUEST_SET_DATA_CENTRIC (RIL_REQUEST_VENDOR_BASE + 105)

/// M: IMS feature. @{
#define RIL_REQUEST_ADD_IMS_CONFERENCE_CALL_MEMBER (RIL_REQUEST_VENDOR_BASE + 106)
#define RIL_REQUEST_REMOVE_IMS_CONFERENCE_CALL_MEMBER (RIL_REQUEST_VENDOR_BASE + 107)
#define RIL_REQUEST_DIAL_WITH_SIP_URI (RIL_REQUEST_VENDOR_BASE + 108)
#define RIL_REQUEST_RESUME_CALL (RIL_REQUEST_VENDOR_BASE + 109)
/// @}

//M: call control part - Speech Codec Info for HD voice icon
#define RIL_REQUEST_SET_SPEECH_CODEC_INFO (RIL_REQUEST_VENDOR_BASE + 110)

/// M: CC33 LTE
#define RIL_REQUEST_SET_DATA_ON_TO_MD (RIL_REQUEST_VENDOR_BASE + 111)
#define RIL_REQUEST_SET_REMOVE_RESTRICT_EUTRAN_MODE (RIL_REQUEST_VENDOR_BASE + 112)

/* M: call control part start */
#define RIL_REQUEST_SET_IMS_CALL_STATUS (RIL_REQUEST_VENDOR_BASE + 113)
/* M: call control part end */

/// M: For 3G VT only @{
#define RIL_REQUEST_VT_DIAL (RIL_REQUEST_VENDOR_BASE + 114)
#define RIL_REQUEST_VOICE_ACCEPT (RIL_REQUEST_VENDOR_BASE + 115)
#define RIL_REQUEST_REPLACE_VT_CALL (RIL_REQUEST_VENDOR_BASE + 116)
/// @}

// C2K SVLTE remote SIM access
#define RIL_REQUEST_CONFIG_MODEM_STATUS (RIL_REQUEST_VENDOR_BASE + 117)

/// M: [C2K] MD IRAT feature. @{
#define RIL_REQUEST_SET_ACTIVE_PS_SLOT (RIL_REQUEST_VENDOR_BASE + 118)
#define RIL_REQUEST_CONFIRM_INTER_3GPP_IRAT_CHANGE (RIL_REQUEST_VENDOR_BASE + 119)
/// @}

/// M: [C2K][SVLTE] Set the SVLTE RAT mode. @{
#define RIL_REQUEST_SET_SVLTE_RAT_MODE (RIL_REQUEST_VENDOR_BASE + 120)
/// M: [C2K][SVLTE] Set the SVLTE RAT mode. @}

/// M: [C2K] AP IRAT feature. @{
#define RIL_REQUEST_TRIGGER_LTE_BG_SEARCH (RIL_REQUEST_VENDOR_BASE + 121)
/// @}

/// M: VoLTE conference dial feature. @{
#define RIL_REQUEST_CONFERENCE_DIAL (RIL_REQUEST_VENDOR_BASE + 122)
/// @}


/// M: CC072: Add Customer proprietary-IMS RIL interface. @{
/* To transfer IMS call context to modem */
#define RIL_REQUEST_SET_SRVCC_CALL_CONTEXT_TRANSFER (RIL_REQUEST_VENDOR_BASE + 123)
/* To update IMS registration status to modem */
#define RIL_REQUEST_UPDATE_IMS_REGISTRATION_STATUS (RIL_REQUEST_VENDOR_BASE + 124)
/// @}

#define RIL_REQUEST_RELOAD_MODEM_TYPE (RIL_REQUEST_VENDOR_BASE + 125)

/// M: IMS feature. @{
#define RIL_REQUEST_HOLD_CALL (RIL_REQUEST_VENDOR_BASE + 126)
/// @}

/// M: [C2K][SVLTE] Set the STK UTK mode. @{
#define RIL_REQUEST_SET_STK_UTK_MODE (RIL_REQUEST_VENDOR_BASE + 127)
/// M: [C2K][SVLTE] Set the STK UTK mode. @}

#define RIL_REQUEST_SWITCH_ANTENNA (RIL_REQUEST_VENDOR_BASE + 128)

/// M: [C2K] Deactivate link down PDN for CT requirement. @{
#define RIL_REQUEST_DEACTIVATE_LINK_DOWN_PDN (RIL_REQUEST_VENDOR_BASE + 129)
///@}

#define RIL_REQUEST_AT_COMMAND_WITH_PROXY (RIL_REQUEST_VENDOR_BASE + 130)
/// M: [C2K] Switch ruim to sim or sim to ruim @{
#define RIL_REQUEST_SWITCH_CARD_TYPE (RIL_REQUEST_VENDOR_BASE + 131)
/// @}

/// M: [C2K] Enable or disable MD3 Sleep @{
#define RIL_REQUEST_ENABLE_MD3_SLEEP (RIL_REQUEST_VENDOR_BASE + 132)
/// @}

/// M: ViLTE feature. @{
#define RIL_REQUEST_VIDEO_CALL_ACCEPT (RIL_REQUEST_VENDOR_BASE + 133)
/// @}

// M: [LTE][Low Power][UL traffic shaping] Start
#define RIL_REQUEST_SET_LTE_ACCESS_STRATUM_REPORT (RIL_REQUEST_VENDOR_BASE + 134)
#define RIL_REQUEST_SET_LTE_UPLINK_DATA_TRANSFER (RIL_REQUEST_VENDOR_BASE + 135)
// M: [LTE][Low Power][UL traffic shaping] End

/// M: [EPDG] Hand over ind @{
#define RIL_REQUEST_HANDOVER_IND (RIL_REQUEST_VENDOR_BASE + 136)
/// @}

/******************************************************************/

#define RIL_LOCAL_REQUEST_VENDOR_BASE 2500

#define RIL_LOCAL_REQUEST_SIM_AUTHENTICATION (RIL_LOCAL_REQUEST_VENDOR_BASE + 0)
#define RIL_LOCAL_REQUEST_USIM_AUTHENTICATION (RIL_LOCAL_REQUEST_VENDOR_BASE + 1)
#define RIL_LOCAL_REQUEST_QUERY_MODEM_THERMAL (RIL_LOCAL_REQUEST_VENDOR_BASE + 2)
#define RIL_LOCAL_REQUEST_RILD_READ_IMSI (RIL_LOCAL_REQUEST_VENDOR_BASE + 3)

/* SIM ME Lock added request id */
#define RIL_LOCAL_REQUEST_GET_SHARED_KEY (RIL_LOCAL_REQUEST_VENDOR_BASE + 4)
#define RIL_LOCAL_REQUEST_UPDATE_SIM_LOCK_SETTINGS (RIL_LOCAL_REQUEST_VENDOR_BASE + 5)
#define RIL_LOCAL_REQUEST_GET_SIM_LOCK_INFO (RIL_LOCAL_REQUEST_VENDOR_BASE + 6)
#define RIL_LOCAL_REQUEST_RESET_SIM_LOCK_SETTINGS (RIL_LOCAL_REQUEST_VENDOR_BASE + 7)
#define RIL_LOCAL_REQUEST_GET_MODEM_STATUS (RIL_LOCAL_REQUEST_VENDOR_BASE + 8)

/* Request modem on/off from thermal */
#define RIL_LOCAL_REQUEST_SET_MODEM_THERMAL (RIL_LOCAL_REQUEST_VENDOR_BASE + 9)
/* External SIM request id*/
#define RIL_LOCAL_REQUEST_VSIM_NOTIFICATION (RIL_LOCAL_REQUEST_VENDOR_BASE + 10)
#define RIL_LOCAL_REQUEST_VSIM_OPERATION (RIL_LOCAL_REQUEST_VENDOR_BASE + 11)

/******************************************************************/

#define RIL_UNSOL_VENDOR_BASE 3000

#define RIL_UNSOL_NEIGHBORING_CELL_INFO (RIL_UNSOL_VENDOR_BASE + 0)

#define RIL_UNSOL_NETWORK_INFO (RIL_UNSOL_VENDOR_BASE + 1)

#define RIL_UNSOL_PHB_READY_NOTIFICATION (RIL_UNSOL_VENDOR_BASE + 2)

#define RIL_UNSOL_SIM_INSERTED_STATUS (RIL_UNSOL_VENDOR_BASE + 3)

#define RIL_UNSOL_RADIO_TEMPORARILY_UNAVAILABLE (RIL_UNSOL_VENDOR_BASE + 4)

#define RIL_UNSOL_ME_SMS_STORAGE_FULL (RIL_UNSOL_VENDOR_BASE + 5)

#define RIL_UNSOL_SMS_READY_NOTIFICATION (RIL_UNSOL_VENDOR_BASE + 6)

#define RIL_UNSOL_SCRI_RESULT (RIL_UNSOL_VENDOR_BASE + 7)

#define RIL_UNSOL_SIM_MISSING (RIL_UNSOL_VENDOR_BASE + 8)

#define RIL_UNSOL_GPRS_DETACH (RIL_UNSOL_VENDOR_BASE + 9)

#define RIL_UNSOL_ATCI_RESPONSE (RIL_UNSOL_VENDOR_BASE + 10)

#define RIL_UNSOL_SIM_RECOVERY (RIL_UNSOL_VENDOR_BASE + 11)
#define RIL_UNSOL_VIRTUAL_SIM_ON (RIL_UNSOL_VENDOR_BASE + 12)
#define RIL_UNSOL_VIRTUAL_SIM_OFF (RIL_UNSOL_VENDOR_BASE + 13)
#define RIL_UNSOL_INVALID_SIM (RIL_UNSOL_VENDOR_BASE + 14)
#define RIL_UNSOL_RESPONSE_PS_NETWORK_STATE_CHANGED (RIL_UNSOL_VENDOR_BASE + 15)

#define RIL_UNSOL_RESPONSE_ACMT (RIL_UNSOL_VENDOR_BASE + 16)

#define RIL_UNSOL_EF_CSP_PLMN_MODE_BIT (RIL_UNSOL_VENDOR_BASE + 17)
#define RIL_UNSOL_IMEI_LOCK (RIL_UNSOL_VENDOR_BASE + 18)

#define RIL_UNSOL_RESPONSE_MMRR_STATUS_CHANGED (RIL_UNSOL_VENDOR_BASE + 19)
#define RIL_UNSOL_SIM_PLUG_OUT (RIL_UNSOL_VENDOR_BASE + 20)
#define RIL_UNSOL_SIM_PLUG_IN (RIL_UNSOL_VENDOR_BASE + 21)
#define RIL_UNSOL_RESPONSE_ETWS_NOTIFICATION (RIL_UNSOL_VENDOR_BASE + 22)

#define RIL_UNSOL_RESPONSE_PLMN_CHANGED (RIL_UNSOL_VENDOR_BASE + 23)
#define RIL_UNSOL_RESPONSE_REGISTRATION_SUSPENDED (RIL_UNSOL_VENDOR_BASE + 24)

#define RIL_UNSOL_STK_EVDL_CALL (RIL_UNSOL_VENDOR_BASE + 25)

#define RIL_UNSOL_DATA_PACKETS_FLUSH (RIL_UNSOL_VENDOR_BASE + 26)

#define RIL_UNSOL_FEMTOCELL_INFO (RIL_UNSOL_VENDOR_BASE + 27)
#define RIL_UNSOL_STK_SETUP_MENU_RESET (RIL_UNSOL_VENDOR_BASE + 28)
#define RIL_UNSOL_APPLICATION_SESSION_ID_CHANGED (RIL_UNSOL_VENDOR_BASE + 29)
#define RIL_UNSOL_ECONF_SRVCC_INDICATION (RIL_UNSOL_VENDOR_BASE + 30)

// IMS
#define RIL_UNSOL_IMS_ENABLE_DONE (RIL_UNSOL_VENDOR_BASE + 31)
#define RIL_UNSOL_IMS_DISABLE_DONE (RIL_UNSOL_VENDOR_BASE + 32)
#define RIL_UNSOL_IMS_REGISTRATION_INFO (RIL_UNSOL_VENDOR_BASE + 33)

//VoLTE
#define RIL_UNSOL_DEDICATE_BEARER_ACTIVATED (RIL_UNSOL_VENDOR_BASE + 34)
#define RIL_UNSOL_DEDICATE_BEARER_MODIFIED (RIL_UNSOL_VENDOR_BASE + 35)
#define RIL_UNSOL_DEDICATE_BEARER_DEACTIVATED (RIL_UNSOL_VENDOR_BASE + 36)

// RAC update indication //sm cause rac
#define RIL_UNSOL_RAC_UPDATE (RIL_UNSOL_VENDOR_BASE + 37)

//[VoLTE]Conf. call merged/added result
#define RIL_UNSOL_ECONF_RESULT_INDICATION (RIL_UNSOL_VENDOR_BASE + 38)

//Remote SIM ME lock related APIs [Start]
#define RIL_UNSOL_MELOCK_NOTIFICATION (RIL_UNSOL_VENDOR_BASE + 39)
//Remote SIM ME lock related APIs [End]

/* M: call control part start */
#define RIL_UNSOL_CALL_FORWARDING (RIL_UNSOL_VENDOR_BASE + 40)
#define RIL_UNSOL_CRSS_NOTIFICATION (RIL_UNSOL_VENDOR_BASE + 41)
#define RIL_UNSOL_INCOMING_CALL_INDICATION (RIL_UNSOL_VENDOR_BASE + 42)
#define RIL_UNSOL_CIPHER_INDICATION (RIL_UNSOL_VENDOR_BASE + 43)
#define RIL_UNSOL_CNAP (RIL_UNSOL_VENDOR_BASE + 44) //obsolete
/* M: call control part end */
#define RIL_UNSOL_SIM_COMMON_SLOT_NO_CHANGED (RIL_UNSOL_VENDOR_BASE + 45)
//Notify if do combine attach
#define RIL_UNSOL_DATA_ALLOWED (RIL_UNSOL_VENDOR_BASE + 46)
#define RIL_UNSOL_STK_CALL_CTRL (RIL_UNSOL_VENDOR_BASE + 47)

// IMS VoLTE
// gives the last IMS Voice Over PS sessions (IMSVOPS) supported indicatyion from the network
#define RIL_UNSOL_VOLTE_EPS_NETWORK_FEATURE_SUPPORT (RIL_UNSOL_VENDOR_BASE + 48)

/// M: IMS feature. @{
#define RIL_UNSOL_CALL_INFO_INDICATION (RIL_UNSOL_VENDOR_BASE + 49)
/// @}

// IMS client on AP get the information of EPS network feature support from NAS signaling and SIB message
#define RIL_UNSOL_VOLTE_EPS_NETWORK_FEATURE_INFO (RIL_UNSOL_VENDOR_BASE + 50)
// IMS clinet on AP be notified for SRVCC handover event ; HandOver Types : Start / Complete / Fail / Canceled
#define RIL_UNSOL_SRVCC_HANDOVER_INFO_INDICATION (RIL_UNSOL_VENDOR_BASE + 51)
//M: call control part - Speech Codec Info for HD voice icon
#define RIL_UNSOL_SPEECH_CODEC_INFO (RIL_UNSOL_VENDOR_BASE + 52)


//MTK-START for MD state change
#define RIL_UNSOL_MD_STATE_CHANGE (RIL_UNSOL_VENDOR_BASE + 53)
/**
* RIL_UNSOL_MD_STATE_CHANGE
*
* Called when modem assert
*
* "data" is const int *
* ((const int *)data)[0] contains the modem state
*
*  0: modem reset stop (MD NORMAL)// not used
*  1: modem reset start (MD ASSERT)
*
*/
//MTK-END for MD state change
// M: CC33 URC
#define RIL_UNSOL_REMOVE_RESTRICT_EUTRAN (RIL_UNSOL_VENDOR_BASE + 54)

// IMS client on AP shall get the information of MO Data Barring and SSAC barring
#define RIL_UNSOL_MO_DATA_BARRING_INFO (RIL_UNSOL_VENDOR_BASE + 55)
#define RIL_UNSOL_SSAC_BARRING_INFO (RIL_UNSOL_VENDOR_BASE + 56)

// IMS VOLTE : send SIP related error message to AP
#define RIL_UNSOL_SIP_CALL_PROGRESS_INDICATOR (RIL_UNSOL_VENDOR_BASE + 57)

#define RIL_UNSOL_ABNORMAL_EVENT (RIL_UNSOL_VENDOR_BASE + 58)

/// M: CC071: Add Customer proprietary-IMS RIL interface. @{
#define RIL_UNSOL_EMERGENCY_BEARER_SUPPORT_NOTIFY (RIL_UNSOL_VENDOR_BASE + 59)
/// @}

/// M: [C2K] MD IRAT feature. @{
#define RIL_UNSOL_INTER_3GPP_IRAT_STATE_CHANGE (RIL_UNSOL_VENDOR_BASE + 60)
/// @}
/// M: [C2K] AP IRAT feature. @{
#define RIL_UNSOL_LTE_BG_SEARCH_STATUS (RIL_UNSOL_VENDOR_BASE + 61)
/// @}

/// M: for GMSS RAT
#define RIL_UNSOL_GMSS_RAT_CHANGED (RIL_UNSOL_VENDOR_BASE + 62)

#define RIL_UNSOL_CDMA_CARD_TYPE (RIL_UNSOL_VENDOR_BASE + 63)

// IMS
#define RIL_UNSOL_IMS_ENABLE_START (RIL_UNSOL_VENDOR_BASE + 64)
#define RIL_UNSOL_IMS_DISABLE_START (RIL_UNSOL_VENDOR_BASE + 65)

#define RIL_UNSOL_IMSI_REFRESH_DONE (RIL_UNSOL_VENDOR_BASE + 66)

// M: Notify RILJ that the AT+EUSIM was received
#define RIL_UNSOL_EUSIM_READY (RIL_UNSOL_VENDOR_BASE + 67)
/// M: BIP {
/**
 * RIL_UNSOL_STK_BIP_PROACTIVE_COMMAND
 *
 * Indicate when SIM issue a BIP proactive command to applications
 *
 * "data" is a const char * containing SAT/USAT proactive command
 * in hexadecimal format string starting with command tag
 *
 */
#define RIL_UNSOL_STK_BIP_PROACTIVE_COMMAND (RIL_UNSOL_VENDOR_BASE + 68)
/// M: BIP }

//WorldMode
#define RIL_UNSOL_WORLD_MODE_CHANGED (RIL_UNSOL_VENDOR_BASE + 69)

/// M: For 3G VT only @{
#define RIL_UNSOL_VT_STATUS_INFO (RIL_UNSOL_VENDOR_BASE + 70)
#define RIL_UNSOL_VT_RING_INFO (RIL_UNSOL_VENDOR_BASE + 71)
/// @}

//External SIM [Start]
#define RIL_UNSOL_VSIM_OPERATION_INDICATION (RIL_UNSOL_VENDOR_BASE + 72)
//External SIM [End]

// M: Reset Attach APN
#define RIL_UNSOL_SET_ATTACH_APN (RIL_UNSOL_VENDOR_BASE + 73)
/// M: MAL @{
#define RIL_UNSOL_MAL_AT_INFO (RIL_UNSOL_VENDOR_BASE + 74)
//info the current 3G SIM
#define RIL_UNSOL_MAIN_SIM_INFO (RIL_UNSOL_VENDOR_BASE + 75)
/// @}
// M: The event for pluggin in the tray in common slot project
#define RIL_UNSOL_TRAY_PLUG_IN (RIL_UNSOL_VENDOR_BASE + 76)

// IMS VOLTE : send call mode change message to AP
#define RIL_UNSOL_CALLMOD_CHANGE_INDICATOR (RIL_UNSOL_VENDOR_BASE + 77)

// IMS VILTE : send video capability message to AP
#define RIL_UNSOL_VIDEO_CAPABILITY_INDICATOR (RIL_UNSOL_VENDOR_BASE + 78)

// M: [LTE][Low Power][UL traffic shaping] Start
#define RIL_UNSOL_LTE_ACCESS_STRATUM_STATE_CHANGE (RIL_UNSOL_VENDOR_BASE + 79)
// M: [LTE][Low Power][UL traffic shaping] End

/// M: IMS/EPDG @{
#define RIL_UNSOL_SETUP_DATA_CALL_RESPONSE (RIL_UNSOL_VENDOR_BASE + 80)
/// M: IMS/EPDG @}

typedef enum {
    RIL_URC,
    RIL_CMD_1,
    RIL_CMD_2,
    RIL_CMD_3,
    RIL_CMD_4, /* split data and nw command channel */
    RIL_ATCI,
    RIL_CHANNEL_OFFSET,

    RIL_URC2 = RIL_CHANNEL_OFFSET,
    RIL_CMD2_1,
    RIL_CMD2_2,
    RIL_CMD2_3,
    RIL_CMD2_4, /* split data and nw command channel */
    RIL_ATCI2,

    RIL_CHANNEL_SET3_OFFSET,
    RIL_URC3 = RIL_CHANNEL_SET3_OFFSET,
    RIL_CMD3_1,
    RIL_CMD3_2,
    RIL_CMD3_3,
    RIL_CMD3_4,
    RIL_ATCI3,

    RIL_CHANNEL_SET4_OFFSET,
    RIL_URC4 = RIL_CHANNEL_SET4_OFFSET,
    RIL_CMD4_1,
    RIL_CMD4_2,
    RIL_CMD4_3,
    RIL_CMD4_4,
    RIL_ATCI4,

    RIL_SUPPORT_CHANNELS
} RILChannelId;


/**
 * RIL_UNSOL_VOICE_RADIO_TECH_CHANGED
 *
 * Indicates that voice technology has changed. Contains new radio technology
 * as a data in the message.
 *
 * "data" is int *
 * ((int *)data)[0] is of type const RIL_RadioTechnology
 *
 */
#define RIL_UNSOL_VOICE_RADIO_TECH_CHANGED 1035

/**
 * RIL_UNSOL_CELL_INFO_LIST
 *
 * Same information as returned by RIL_REQUEST_GET_CELL_INFO_LIST, but returned
 * at the rate no greater than specified by RIL_REQUEST_SET_UNSOL_CELL_INFO_RATE.
 *
 * "data" is NULL
 *
 * "response" is an array of RIL_CellInfo.
 */
#define RIL_UNSOL_CELL_INFO_LIST 1036

/**
 * RIL_UNSOL_RESPONSE_IMS_NETWORK_STATE_CHANGED
 *
 * Called when IMS registration state has changed
 *
 * To get IMS registration state and IMS SMS format, callee needs to invoke the
 * following request on main thread:
 *
 * RIL_REQUEST_IMS_REGISTRATION_STATE
 *
 * "data" is NULL
 *
 */
#define RIL_UNSOL_RESPONSE_IMS_NETWORK_STATE_CHANGED 1037


/**
 * RIL_UNSOL_UICC_SUBSCRIPTION_STATUS_CHANGED
 *
 * Indicated when there is a change in subscription status.
 * This event will be sent in the following scenarios
 *  - subscription readiness at modem, which was selected by telephony layer
 *  - when subscription is deactivated by modem due to UICC card removal
 *  - When network invalidates the subscription i.e. attach reject due to authentication reject
 *
 * "data" is const int *
 * ((const int *)data)[0] == 0 for Subscription Deactivated
 * ((const int *)data)[0] == 1 for Subscription Activated
 *
 */
#define RIL_UNSOL_UICC_SUBSCRIPTION_STATUS_CHANGED 1038

/**
 * RIL_UNSOL_SRVCC_STATE_NOTIFY
 *
 * Called when Single Radio Voice Call Continuity(SRVCC)
 * progress state has changed
 *
 * "data" is int *
 * ((int *)data)[0] is of type const RIL_SrvccState
 *
 */

#define RIL_UNSOL_SRVCC_STATE_NOTIFY 1039

/**
 * RIL_UNSOL_HARDWARE_CONFIG_CHANGED
 *
 * Called when the hardware configuration associated with the RILd changes
 *
 * "data" is an array of RIL_HardwareConfig
 *
 */
#define RIL_UNSOL_HARDWARE_CONFIG_CHANGED 1040

/**
 * RIL_UNSOL_DC_RT_INFO_CHANGED
 *
 * Sent when the DC_RT_STATE changes but the time
 * between these messages must not be less than the
 * value set by RIL_REQUEST_SET_DC_RT_RATE.
 *
 * "data" is the most recent RIL_DcRtInfo
 *
 */
#define RIL_UNSOL_DC_RT_INFO_CHANGED 1041

/***********************************************************************/

/**
 * RIL_Request Function pointer
 *
 * @param request is one of RIL_REQUEST_*
 * @param data is pointer to data defined for that RIL_REQUEST_*
 *        data is owned by caller, and should not be modified or freed by callee
 * @param t should be used in subsequent call to RIL_onResponse
 * @param datalen the length of data
 * @param socket_id the socket where the unsolicited message go
 *
 */
typedef void (*RIL_RequestFuncSocket) (int request, void *data,
                                    size_t datalen, RIL_Token t, RIL_SOCKET_ID socket_id);


/**
 * This function should return the current radio state synchronously
 * param socket_id the socket where the unsolicited message go
 */
typedef RIL_RadioState (*RIL_RadioStateRequestSocket)(RIL_SOCKET_ID socket_id);


/**
 * RIL_Request Function pointer
 *
 * @param request is one of RIL_REQUEST_*
 * @param data is pointer to data defined for that RIL_REQUEST_*
 *        data is owned by caller, and should not be modified or freed by callee
 * @param t should be used in subsequent call to RIL_onResponse
 * @param datalen the length of data
 *
 */
typedef void (*RIL_RequestFunc) (int request, void *data,
                                    size_t datalen, RIL_Token t);

/**
 * This function should return the current radio state synchronously
 */
typedef RIL_RadioState (*RIL_RadioStateRequest)();


/**
 * This function returns "1" if the specified RIL_REQUEST code is
 * supported and 0 if it is not
 *
 * @param requestCode is one of RIL_REQUEST codes
 */

typedef int (*RIL_Supports)(int requestCode);

/**
 * This function is called from a separate thread--not the
 * thread that calls RIL_RequestFunc--and indicates that a pending
 * request should be cancelled.
 *
 * On cancel, the callee should do its best to abandon the request and
 * call RIL_onRequestComplete with RIL_Errno CANCELLED at some later point.
 *
 * Subsequent calls to  RIL_onRequestComplete for this request with
 * other results will be tolerated but ignored. (That is, it is valid
 * to ignore the cancellation request)
 *
 * RIL_Cancel calls should return immediately, and not wait for cancellation
 *
 * Please see ITU v.250 5.6.1 for how one might implement this on a TS 27.007
 * interface
 *
 * @param t token wants to be canceled
 */

typedef void (*RIL_Cancel)(RIL_Token t);

typedef void (*RIL_TimedCallback) (void *param);

/**
 * Return a version string for your RIL implementation
 */
typedef const char * (*RIL_GetVersion) (void);

typedef struct {
    int version;        /* set to RIL_VERSION */
    RIL_RequestFunc onRequest;
    RIL_RadioStateRequest onStateRequest;
    RIL_Supports supports;
    RIL_Cancel onCancel;
    RIL_GetVersion getVersion;
} RIL_RadioFunctions;

typedef struct {
    int version;        /* set to RIL_VERSION */
    RIL_RequestFuncSocket onRequest;
    RIL_RadioStateRequestSocket onStateRequest;
    RIL_Supports supports;
    RIL_Cancel onCancel;
    RIL_GetVersion getVersion;
} RIL_RadioFunctionsSocket;

typedef struct {
    char *apn;
    char *protocol;
    int authtype;
    char *username;
    char *password;
    char *operatorNumeric;
    int canHandleIms;
    char** dualApnPlmnList;
} RIL_InitialAttachApn;

typedef struct {
    int authContext;            /* P2 value of authentication command, see P2 parameter in
                                   3GPP TS 31.102 7.1.2 */
    char *authData;             /* the challenge string in Base64 format, see 3GPP
                                   TS 31.102 7.1.2 */
    char *aid;                  /* AID value, See ETSI 102.221 8.1 and 101.220 4,
                                   NULL if no value. */
} RIL_SimAuthentication;

typedef struct {
    int sw1;
    int sw2;
    char *simResponse;          /* Response in Base64 format, see 3GPP TS 31.102 7.1.2 */
} RIL_SimAuthenticationResponse;

struct RIL_Env {
    /**
     * "t" is parameter passed in on previous call to RIL_Notification
     * routine.
     *
     * If "e" != SUCCESS, then response can be null/is ignored
     *
     * "response" is owned by caller, and should not be modified or
     * freed by callee
     *
     * RIL_onRequestComplete will return as soon as possible
     */
    void (*OnRequestComplete)(RIL_Token t, RIL_Errno e,
                           void *response, size_t responselen);

    /**
     * "unsolResponse" is one of RIL_UNSOL_RESPONSE_*
     * "data" is pointer to data defined for that RIL_UNSOL_RESPONSE_*
     *
     * "data" is owned by caller, and should not be modified or freed by callee
     */
    void (*OnUnsolicitedResponse)(int unsolResponse, const void *data, size_t datalen);

    /**
     * Call user-specifed "callback" function on on the same thread that
     * RIL_RequestFunc is called. If "relativeTime" is specified, then it specifies
     * a relative time value at which the callback is invoked. If relativeTime is
     * NULL or points to a 0-filled structure, the callback will be invoked as
     * soon as possible
     */

    void (*RequestTimedCallback) (RIL_TimedCallback callback,
                                   void *param, const struct timeval *relativeTime);

    /**
     * Same propose as RequestTimedCallback but executed in proxy thread
     */
    void (*RequestProxyTimedCallback) (RIL_TimedCallback callback, void *param,
    					const struct timeval *relativeTime, int proxyId);

    /**
     * Query Context from RIL_Token
     */
    RILChannelId (*QueryMyChannelId) (RIL_Token t);

    /**
     * Query current proxy according to thread
     */
    int (*QueryMyProxyIdByThread)();
};

struct RIL_EnvSocket {
    /**
     * "t" is parameter passed in on previous call to RIL_Notification
     * routine.
     *
     * If "e" != SUCCESS, then response can be null/is ignored
     *
     * "response" is owned by caller, and should not be modified or
     * freed by callee
     *
     * RIL_onRequestComplete will return as soon as possible
     */
    void (*OnRequestComplete)(RIL_Token t, RIL_Errno e,
                           void *response, size_t responselen);

    /**
     * "unsolResponse" is one of RIL_UNSOL_RESPONSE_*
     * "data" is pointer to data defined for that RIL_UNSOL_RESPONSE_*
     *
     * "data" is owned by caller, and should not be modified or freed by callee
     * "socket_id" the socket where the unsolicited message go
     */
    void (*OnUnsolicitedResponse)(int unsolResponse, const void *data, size_t datalen, RIL_SOCKET_ID socket_id);

    /**
     * Call user-specifed "callback" function on on the same thread that
     * RIL_RequestFunc is called. If "relativeTime" is specified, then it specifies
     * a relative time value at which the callback is invoked. If relativeTime is
     * NULL or points to a 0-filled structure, the callback will be invoked as
     * soon as possible
     */

    void (*RequestTimedCallback) (RIL_TimedCallback callback,
                                   void *param, const struct timeval *relativeTime);

    /**
     * Same propose as RequestTimedCallback but executed in proxy thread
     */
    void (*RequestProxyTimedCallback) (RIL_TimedCallback callback, void *param,
    					const struct timeval *relativeTime, int proxyId);

    /**
     * Query Context from RIL_Token
     */
    RILChannelId (*QueryMyChannelId) (RIL_Token t);

    /**
     * Query current proxy according to thread
     */
    int (*QueryMyProxyIdByThread)();
};


#ifdef RIL_SHLIB

/**
 *  For backward compatible, RIL implementations must defined RIL_GetRilVersion
 *  Return RIL_VERSION
 *
 *
 */

const int RIL_MultiSocketInRild(void);

/**
 *  RIL implementations must defined RIL_Init
 *  argc and argv will be command line arguments intended for the RIL implementation
 *  Return NULL on error
 *
 * @param env is environment point defined as RIL_Env
 * @param argc number of arguments
 * @param argv list fo arguments
 *
 */
const RIL_RadioFunctions *RIL_Init(const struct RIL_Env *env, int argc, char **argv);

/**
 *  RIL implementations must defined RIL_InitSocket if multi sockets within rild
 *  argc and argv will be command line arguments intended for the RIL implementation
 *  Return NULL on error
 *
 * @param env is environment point defined as RIL_Env
 * @param argc number of arguments
 * @param argv list fo arguments
 *
 */
const RIL_RadioFunctionsSocket *RIL_InitSocket(const struct RIL_EnvSocket *env, int argc, char **argv);

#else /* RIL_SHLIB */

/**
 * Call this once at startup to register notification routine
 *
 * @param callbacks user-specifed callback function
 */
void RIL_register (const RIL_RadioFunctions *callbacks);

/**
 * Call this once at startup to register notification routine
 *
 * @param callbacks user-specifed callback function
 */
void RIL_registerSocket (const RIL_RadioFunctionsSocket *callbacks);

/**
 * Call this once at startup to register ims socket
 */
void IMS_RILA_register(const RIL_RadioFunctionsSocket *callbacks);

/**
 * @param unsolResponse is one of RIL_UNSOL_RESPONSE_*
 * @param data is pointer to data defined for that RIL_UNSOL_RESPONSE_*
 *     "data" is owned by caller, and should not be modified or freed by callee
 * @param datalen the length of data in byte
 */
void IMS_RIL_onUnsolicitedResponse(int unsolResponse, void *data,
                                size_t datalen, RIL_SOCKET_ID socket_id);

/**
 * Set Whether Srvcc Procedure for AP is on-going or done
 * @param enable is the status for AP srvcc procedure
 */
void IMS_isSrvccProcedure(int enable);

/**
 *
 * IMS_RIL_onRequestComplete will return as soon as possible
 *
 * @param t is parameter passed in on previous call to RIL_Notification
 *          routine.
 * @param e error code
 *          if "e" != SUCCESS, then response can be null/is ignored
 * @param response is owned by caller, and should not be modified or
 *                 freed by callee
 * @param responselen the length of response in byte
 */

void IMS_RIL_onRequestComplete(RIL_Token t, RIL_Errno e,
                           void *response, size_t responselen);

/**
 * Check Whether the RIL request is from IMS
 * @param t is the token of request information from AP
 */
bool IMS_isRilRequestFromIms(RIL_Token t);

/**
 *
 * RIL_onRequestComplete will return as soon as possible
 *
 * @param t is parameter passed in on previous call to RIL_Notification
 *          routine.
 * @param e error code
 *          if "e" != SUCCESS, then response can be null/is ignored
 * @param response is owned by caller, and should not be modified or
 *                 freed by callee
 * @param responselen the length of response in byte
 */
void RIL_onRequestComplete(RIL_Token t, RIL_Errno e,
                           void *response, size_t responselen);

/**
 * @param unsolResponse is one of RIL_UNSOL_RESPONSE_*
 * @param data is pointer to data defined for that RIL_UNSOL_RESPONSE_*
 *     "data" is owned by caller, and should not be modified or freed by callee
 * @param datalen the length of data in byte
 * @param socket_id the socket where the unsolicited message go
 */
void RIL_onUnsolicitedResponseSocket(int unsolResponse, const void *data,
                                size_t datalen, RIL_SOCKET_ID socket_id);

/**
 * @param unsolResponse is one of RIL_UNSOL_RESPONSE_*
 * @param data is pointer to data defined for that RIL_UNSOL_RESPONSE_*
 *     "data" is owned by caller, and should not be modified or freed by callee
 * @param datalen the length of data in byte
 */
void RIL_onUnsolicitedResponse(int unsolResponse, const void *data,
                                size_t datalen);


/**
 * Call user-specifed "callback" function on on the same thread that
 * RIL_RequestFunc is called. If "relativeTime" is specified, then it specifies
 * a relative time value at which the callback is invoked. If relativeTime is
 * NULL or points to a 0-filled structure, the callback will be invoked as
 * soon as possible
 *
 * @param callback user-specifed callback function
 * @param param parameter list
 * @param relativeTime a relative time value at which the callback is invoked
 */

void RIL_requestTimedCallback (RIL_TimedCallback callback,
                               void *param, const struct timeval *relativeTime);

/*
get the 3G SIM info , reduce the usage of read of persist property
*/
int RIL_get3GSimInfo();

#endif /* RIL_SHLIB */

#ifdef __cplusplus
}
#endif

#endif /*MTK_RIL_H*/
