LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_IS_HOST_MODULE =
LOCAL_MODULE = msensord
LOCAL_MODULE_CLASS = EXECUTABLES
LOCAL_MODULE_PATH = $(PRODUCT_OUT)/system/bin
LOCAL_MODULE_RELATIVE_PATH =
LOCAL_MODULE_SUFFIX =
LOCAL_MODULE_STEM =
LOCAL_MODULE_STEM_32 =
LOCAL_MODULE_STEM_64 =
LOCAL_SHARED_LIBRARIES = libc++
LOCAL_MULTILIB = 64
LOCAL_SRC_FILES_64 = msensord
include $(BUILD_PREBUILT)
