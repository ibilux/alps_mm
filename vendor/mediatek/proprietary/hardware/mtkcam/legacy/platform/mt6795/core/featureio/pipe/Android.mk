# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.
#
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
#===============================================================================

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

#-----------------------------------------------------------
sinclude $(TOP)/$(MTK_MTKCAM_PLATFORM)/mtkcam.mk

#-----------------------------------------------------------
#LOCAL_SRC_FILES += \

#-----------------------------------------------------------  
#LOCAL_C_INCLUDES += \

#-----------------------------------------------------------
#LOCAL_STATIC_LIBRARIES := \
#     libfeatureiopipe_fdft

LOCAL_WHOLE_STATIC_LIBRARIES := \
    libfeatureiopipe_aaa \

ifneq ($(BUILD_MTK_LDVT),true)
    LOCAL_WHOLE_STATIC_LIBRARIES += libfeatureiopipe_eis
ifeq ($(MTK_CAM_VHDR_SUPPORT),yes)
    LOCAL_WHOLE_STATIC_LIBRARIES += libfeatureiopipe_vhdr
endif
    LOCAL_WHOLE_STATIC_LIBRARIES += libcam.featureio.pipe.motiontrack
    LOCAL_WHOLE_STATIC_LIBRARIES += libfeatureiopipe_vfb
    LOCAL_WHOLE_STATIC_LIBRARIES += libfeatureiopipe_fdft
    LOCAL_WHOLE_STATIC_LIBRARIES += libfeatureiopipe_ot
    LOCAL_WHOLE_STATIC_LIBRARIES += libfeatureiopipe_flk
    LOCAL_WHOLE_STATIC_LIBRARIES += libfeatureiopipe_hdr
    LOCAL_WHOLE_STATIC_LIBRARIES += libfeatureiopipe_mfll
    LOCAL_WHOLE_STATIC_LIBRARIES += libfeatureiopipe_asd
    LOCAL_WHOLE_STATIC_LIBRARIES += libfeatureiopipe_pip
    LOCAL_WHOLE_STATIC_LIBRARIES += libcam.featureio.pipe.facebeauty
    LOCAL_WHOLE_STATIC_LIBRARIES += libcam.featureio.pipe.panorama
    LOCAL_WHOLE_STATIC_LIBRARIES += libcam.featureio.pipe.gestureshot
    LOCAL_WHOLE_STATIC_LIBRARIES += libcam.featureio.pipe.utility
    LOCAL_WHOLE_STATIC_LIBRARIES += libcam.featureio.pipe.stereo
    LOCAL_WHOLE_STATIC_LIBRARIES += libcam.featureio.pipe.stereodepth
    LOCAL_WHOLE_STATIC_LIBRARIES += libcam.featureio.pipe.capturenr
    LOCAL_WHOLE_STATIC_LIBRARIES += libcam.featureio.pipe.hrd
endif

#-----------------------------------------------------------
LOCAL_MODULE := libfeatureiopipe


#-----------------------------------------------------------
include $(BUILD_STATIC_LIBRARY)

#-----------------------------------------------------------
include $(call all-makefiles-under,$(LOCAL_PATH))
