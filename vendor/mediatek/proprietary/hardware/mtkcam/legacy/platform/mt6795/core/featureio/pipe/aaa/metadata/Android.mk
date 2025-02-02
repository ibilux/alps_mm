# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.

# MediaTek Inc. (C) 2010. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
#
# The following software/firmware and/or related documentation ("MediaTek Software")
# have been modified by MediaTek Inc. All revisions are subject to any receiver's
# applicable license agreements with MediaTek Inc.

################################################################################

LOCAL_PATH := $(call my-dir)

################################################################################
#
################################################################################
include $(CLEAR_VARS)

#-----------------------------------------------------------
sinclude $(TOP)/$(MTK_MTKCAM_PLATFORM)/mtkcam.mk

#-----------------------------------------------------------
# version
MY_CUST_VERSION := $(shell date) rev:1.0.0

#-----------------------------------------------------------
#
MTKCAM_HAVE_LOCALTEST   := '0'  # test by local table if '1' ; otherwise custom table
#
#-----------------------------------------------------------
#
define my-all-config_xxx-under
$(patsubst ./%,%, \
  $(shell cd $(1); \
    find $(2)  -maxdepth 1 \( -name "config_static_metadata.*.h" \) -and -not -name ".*") \
 )
endef
#
# custom static metadata file list
ifeq "'1'" "$(strip $(MTKCAM_HAVE_LOCALTEST))"
#    MY_CUST_METADATA_FILE_LIST := $(call my-all-config_xxx-under, config/imgsensor)
    MY_CUST_METADATA_FILE_LIST := $(call my-all-config_xxx-under, $(LOCAL_PATH), config/imgsensor)
    MY_CUST_METADATA_FILE_FULL := $(addprefix $(LOCAL_PATH)/,$(MY_CUST_METADATA_FILE_LIST))
else
    MY_CUST_METADATA_FILE_LIST := $(call my-all-config_xxx-under, $(MTK_PATH_CUSTOM), hal/imgsensor)
    MY_CUST_METADATA_FILE_LIST += $(call my-all-config_xxx-under, $(MTK_PATH_CUSTOM), hal/lens)
    MY_CUST_METADATA_FILE_FULL := $(addprefix $(MTK_PATH_CUSTOM)/,$(MY_CUST_METADATA_FILE_LIST))
endif
#


#-----------------------------------------------------------
LOCAL_SRC_FILES += $(call all-c-cpp-files-under, .)

#-----------------------------------------------------------
LOCAL_C_INCLUDES += $(MTKCAM_C_INCLUDES)
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/hardware/gralloc_extra/include
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/hardware/mtkcam/ext/include

LOCAL_C_INCLUDES += $(TOP)/$(MTKCAM_C_INCLUDES)/..
LOCAL_C_INCLUDES += $(TOP)/$(MTK_MTKCAM_PLATFORM)/include
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_CUSTOM)
#

#-----------------------------------------------------------
LOCAL_CFLAGS += $(MTKCAM_CFLAGS)
#

#-----------------------------------------------------------
LOCAL_WHOLE_STATIC_LIBRARIES += 
#
LOCAL_STATIC_LIBRARIES += 

#-----------------------------------------------------------
LOCAL_MODULE := libfeatureio.aaa.custom
LOCAL_MODULE_CLASS := STATIC_LIBRARIES
#bobule workaround pdk build error, needing review
ifeq ($(TARGET_ARCH), arm)
INTERMEDIATES := $(call local-intermediates-dir)
else
INTERMEDIATES := $(call local-intermediates-dir,,$(TARGET_2ND_ARCH_VAR_PREFIX))
endif
# custom static metadata all-in-one file
MY_CUST_METADATA_FINAL_FILE := $(INTERMEDIATES)/custgen.config_metadata.h
#
# generate all-in-one file
#$(shell echo '//this file is auto-generated; do not modify it!' > $(MY_CUST_METADATA_FINAL_FILE) )
#$(shell echo '#define MY_CUST_VERSION "$(MY_CUST_VERSION)"' >> $(MY_CUST_METADATA_FINAL_FILE) )
#$(shell echo '#define MY_CUST_METADATA_FILE_LIST "$(MY_CUST_METADATA_FILE_LIST)"' >> $(MY_CUST_METADATA_FINAL_FILE) )
# write each custom file include path into all-in-one file.
#$(foreach file, $(MY_CUST_METADATA_FILE_LIST), $(shell echo "#include <$(file)>" >> $(MY_CUST_METADATA_FINAL_FILE)))

LOCAL_GENERATED_SOURCES += $(MY_CUST_METADATA_FINAL_FILE)
$(MY_CUST_METADATA_FINAL_FILE): PRIVATE_FILE_LIST := $(MY_CUST_METADATA_FILE_LIST)
$(MY_CUST_METADATA_FINAL_FILE): $(MY_CUST_METADATA_FILE_FULL)
    @mkdir -p $(dir $@)
    @echo '//this file is auto-generated; do not modify it!' > $@
    @echo '#define MY_CUST_VERSION "$(MY_CUST_VERSION)"' >> $@
    @echo '#define MY_CUST_METADATA_FILE_LIST "$(PRIVATE_FILE_LIST)"' >> $@
    @for x in $(PRIVATE_FILE_LIST); do echo "#include <$$x>" >> $@; done

$(info "MY_CUST_METADATA_FILE_LIST="$(MY_CUST_METADATA_FILE_LIST))
#-----------------------------------------------------------
include $(BUILD_STATIC_LIBRARY)

################################################################################
#
################################################################################
#include $(CLEAR_VARS)
#include $(call all-makefiles-under,$(LOCAL_PATH))

