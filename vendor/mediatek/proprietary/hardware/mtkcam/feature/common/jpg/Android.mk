# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.

# MediaTek Inc. (C) 2015. All rights reserved.
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

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

# == Only build without LDVT, because SImager not exist in LDVT
ifneq ($(BUILD_MTK_LDVT),yes)
#
# Start of common part ------------------------------------
sinclude $(TOP)/$(MTK_PATH_SOURCE)/hardware/mtkcam/mtkcam.mk

# == Only build if not legcacy ==
ifeq ($(IS_LEGACY), 0)

PLATFORM := $(shell echo $(MTK_PLATFORM) | tr A-Z a-z)

ifeq ($(PLATFORM), $(filter $(PLATFORM),mt6797))
JPG_DIR := JPG1.0
else
# TODO now all use 1.0
JPG_DIR := JPG1.0
endif

LOCAL_SRC_FILES += $(call all-c-cpp-files-under, ./$(JPG_DIR)/)

# ------- INCLUDE  PATHES -----------------------------

LOCAL_C_INCLUDES += $(MTKCAM_C_INCLUDES)
#for ISImager.h, BufAllocUtil.h
LOCAL_C_INCLUDES += $(MTKCAM_DRV_INCLUDE)
LOCAL_C_INCLUDES += $(MTK_PATH_SOURCE)/hardware/mtkcam
#for ISImagerDataTypes.h
LOCAL_C_INCLUDES += $(MTKCAM_DRV_INCLUDE)/iopipe
#for camera3.h
LOCAL_C_INCLUDES += $(TOP)/system/media/camera/include
#for IEffectHal related mmsdk header
LOCAL_C_INCLUDES += $(MTK_PATH_SOURCE)/frameworks/av/services/mmsdk/include
#for gralloc_extra include
LOCAL_C_INCLUDES += $(MTK_PATH_SOURCE)/hardware/gralloc_extra/include

# Test 
#$(warning !!!LOCAL C PATH = $(LOCAL_C_INCLUDES))

LOCAL_WHOLE_STATIC_LIBRARIES += 
#
LOCAL_STATIC_LIBRARIES += 

#-----------------------------------------------------------
LOCAL_SHARED_LIBRARIES += libcutils
LOCAL_SHARED_LIBRARIES += libstdc++
LOCAL_SHARED_LIBRARIES += libutils
LOCAL_SHARED_LIBRARIES += libcam.exif.v3
# For ISImager
LOCAL_SHARED_LIBRARIES += libcam.iopipe
# For ImageBuffer
LOCAL_SHARED_LIBRARIES += libcam_utils
LOCAL_SHARED_LIBRARIES += libmmsdkservice.feature
LOCAL_SHARED_LIBRARIES += libeffecthal.utils

#-----------------------------------------------------------
LOCAL_MODULE := libeffecthal.jpg
#-----------------------------------------------------------
include $(BUILD_SHARED_LIBRARY)

endif
# if LEGACY == 0 end if
endif # ifneq ($(BUILD_MTK_LDVT),yes) end if
