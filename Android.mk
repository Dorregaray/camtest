BUILD_LIBCAMERA:=true

# When zero we link against libmmcamera; when 1, we dlopen libmmcamera.
DLOPEN_LIBMMCAMERA:=1


LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= camtest.cpp

LOCAL_CFLAGS:= -DDLOPEN_LIBMMCAMERA=$(DLOPEN_LIBMMCAMERA)

LOCAL_CFLAGS+= -DNUM_PREVIEW_BUFFERS=4 -D_ANDROID_

ifeq ($(BOARD_CAMERA_USE_ENCODEDATA),true)
    LOCAL_CFLAGS += -DUSE_ENCODEDATA
endif
ifeq ($(BOARD_CAMERA_USE_GETBUFFERINFO),true)
    LOCAL_CFLAGS += -DUSE_GETBUFFERINFO
endif

LOCAL_C_INCLUDES := $(TOP)/frameworks/base/include

LOCAL_SHARED_LIBRARIES:= libutils libui libcamera_client liblog libcutils libcamera

LOCAL_SHARED_LIBRARIES+= libbinder
ifneq ($(DLOPEN_LIBMMCAMERA),1)
LOCAL_SHARED_LIBRARIES+= liboemcamera
else
LOCAL_SHARED_LIBRARIES+= libdl
endif

LOCAL_MODULE:= camtest
LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)