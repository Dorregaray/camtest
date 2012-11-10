LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES  := camtest.cpp
LOCAL_C_INCLUDES := $(TOP)/frameworks/base/include

ifeq ($(BOARD_CAMERA_USE_ENCODEDATA),true)
    LOCAL_CFLAGS += -DUSE_ENCODEDATA
endif
ifeq ($(BOARD_CAMERA_USE_GETBUFFERINFO),true)
    LOCAL_CFLAGS += -DUSE_GETBUFFERINFO
endif
ifeq ($(BOARD_USE_FROYO_LIBCAMERA), true)
    LOCAL_CFLAGS += -DBOARD_USE_FROYO_LIBCAMERA
endif

LOCAL_SHARED_LIBRARIES:= libutils libui libcamera_client liblog libcutils libcamera

LOCAL_MODULE:= camtest
LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)
