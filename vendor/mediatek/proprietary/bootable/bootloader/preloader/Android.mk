LOCAL_PATH := $(call my-dir)
PRELOADER_ROOT_DIR := $(PWD)

ifdef PRELOADER_TARGET_PRODUCT
    PRELOADER_DIR := $(LOCAL_PATH)
    PRELOADER_OUT ?= $(if $(filter /% ~%,$(TARGET_OUT_INTERMEDIATES)),,$(PRELOADER_ROOT_DIR)/)$(TARGET_OUT_INTERMEDIATES)/PRELOADER_OBJ
  ifdef PL_MODE
    INSTALLED_PRELOADER_TARGET := $(PRODUCT_OUT)/preloader_$(PRELOADER_TARGET_PRODUCT).bin
    BUILT_PRELOADER_TARGET := $(PRELOADER_OUT)/bin/preloader_$(PRELOADER_TARGET_PRODUCT)_$(PL_MODE).bin
  else
    INSTALLED_PRELOADER_TARGET := $(PRODUCT_OUT)/preloader_$(PRELOADER_TARGET_PRODUCT).bin
    BUILT_PRELOADER_TARGET := $(PRELOADER_OUT)/bin/preloader_$(PRELOADER_TARGET_PRODUCT).bin
  endif
    ifeq ($(LK_CROSS_COMPILE),)
    ifeq ($(TARGET_ARCH), arm)
#      PRELOADER_CROSS_COMPILE := $(PRELOADER_ROOT_DIR)/$(TARGET_TOOLS_PREFIX)
    else ifeq ($(TARGET_2ND_ARCH), arm)
#      PRELOADER_CROSS_COMPILE := $(PRELOADER_ROOT_DIR)/$($(TARGET_2ND_ARCH_VAR_PREFIX)TARGET_TOOLS_PREFIX)
    endif
    endif

ifneq (MT8127,$(strip $(MTK_PLATFORM)))
    PRELOADER_CROSS_COMPILE := $(CURDIR)/prebuilts/gcc/$(HOST_OS)-x86/arm/arm-linux-androideabi-4.8/bin/arm-linux-androideabi-
else
    PRELOADER_CROSS_COMPILE := $(CURDIR)/prebuilts/gcc/$(HOST_OS)-x86/arm/arm-eabi-4.8/bin/arm-eabi-
endif
    PRELOADER_MAKE_OPTION := $(if $(SHOW_COMMANDS),,-s) -f Makefile $(if $(PRELOADER_CROSS_COMPILE),CROSS_COMPILE=$(PRELOADER_CROSS_COMPILE)) PRELOADER_OUT=$(PRELOADER_OUT) MTK_PROJECT=$(PRELOADER_TARGET_PRODUCT) TOOL_PATH=$(PRELOADER_ROOT_DIR)/device/mediatek/build/build/tools ROOTDIR=$(PRELOADER_ROOT_DIR)

  ifeq ($(wildcard $(TARGET_PREBUILT_PRELOADER)),)
$(BUILT_PRELOADER_TARGET): FORCE
	$(hide) mkdir -p $(dir $@)
	$(MAKE) -C $(PRELOADER_DIR) $(PRELOADER_MAKE_OPTION)

$(TARGET_PREBUILT_PRELOADER): $(BUILT_PRELOADER_TARGET) | $(ACP)
	$(copy-file-to-target)

  else
    BUILT_PRELOADER_TARGET := $(TARGET_PREBUILT_PRELOADER)
  endif#TARGET_PREBUILT_PRELOADER

  ifneq ($(INSTALLED_PRELOADER_TARGET),$(BUILT_PRELOADER_TARGET))
$(INSTALLED_PRELOADER_TARGET): $(BUILT_PRELOADER_TARGET) | $(ACP)
	$(copy-file-to-target)

  endif

.PHONY: preloader pl save-preloader %-preloader clean-preloader check-mtk-config check-pl-config

TARGET_PRELOADER := $(PRODUCT_OUT)/preloader.img
$(TARGET_PRELOADER): $(INSTALLED_PRELOADER_TARGET) $(PRELOADER_DIR)/tools/gen-preloader-img.py
	$(hide) $(PRELOADER_DIR)/tools/gen-preloader-img.py $< $@

droidcore: preloader
preloader pl: check-pl-config $(INSTALLED_PRELOADER_TARGET) $(TARGET_PRELOADER)
save-preloader: $(TARGET_PREBUILT_PRELOADER)

%-preloader:
	$(MAKE) -C $(PRELOADER_DIR) $(PRELOADER_MAKE_OPTION) $(patsubst %-preloader,%,$@)

clean-preloader:
	$(hide) rm -rf $(INSTALLED_PRELOADER_TARGET) $(TARGET_PRELOADER) $(PRELOADER_OUT)

check-mtk-config: check-pl-config
check-pl-config:
ifneq (yes,$(strip $(DISABLE_MTK_CONFIG_CHECK)))
	python device/mediatek/build/build/tools/check_kernel_config.py -c $(MTK_TARGET_PROJECT_FOLDER)/ProjectConfig.mk -b $(PRELOADER_DIR)/custom/$(PRELOADER_TARGET_PRODUCT)/$(PRELOADER_TARGET_PRODUCT).mk -p $(MTK_PROJECT_NAME)
else
	-python device/mediatek/build/build/tools/check_kernel_config.py -c $(MTK_TARGET_PROJECT_FOLDER)/ProjectConfig.mk -b $(PRELOADER_DIR)/custom/$(PRELOADER_TARGET_PRODUCT)/$(PRELOADER_TARGET_PRODUCT).mk -p $(MTK_PROJECT_NAME)
endif


endif#PRELOADER_TARGET_PRODUCT
