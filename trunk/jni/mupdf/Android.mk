LOCAL_PATH := $(call my-dir)
TOP_LOCAL_PATH := $(LOCAL_PATH)

MUPDF_ROOT := ..

include $(TOP_LOCAL_PATH)/Core.mk

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/draw \
	$(LOCAL_PATH)/fitz \
	$(LOCAL_PATH)/pdf
	
LOCAL_CFLAGS :=
LOCAL_MODULE    := pdf_plugin
LOCAL_SRC_FILES := pdf_handler.c
LOCAL_STATIC_LIBRARIES := mupdfcore jpeg jbig2dec openjpeg freetype

LOCAL_LDLIBS    := -lz -lm -llog

include $(BUILD_SHARED_LIBRARY)
