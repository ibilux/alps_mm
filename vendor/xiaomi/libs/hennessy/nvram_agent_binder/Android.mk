LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_IS_HOST_MODULE =
LOCAL_MODULE = nvram_agent_binder
LOCAL_MODULE_CLASS = EXECUTABLES
LOCAL_MODULE_PATH =
LOCAL_MODULE_RELATIVE_PATH =
LOCAL_MODULE_SUFFIX =
LOCAL_MODULE_STEM =
LOCAL_MODULE_STEM_32 =
LOCAL_MODULE_STEM_64 =
LOCAL_SHARED_LIBRARIES = libbinder libnvram libc++
LOCAL_MULTILIB = 64
LOCAL_SRC_FILES_64 = nvram_agent_binder
include $(BUILD_PREBUILT)
