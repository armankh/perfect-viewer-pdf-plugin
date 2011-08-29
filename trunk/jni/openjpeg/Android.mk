LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := openjpeg
LOCAL_SRC_FILES := \
	libopenjpeg/bio.c \
	libopenjpeg/cio.c \
	libopenjpeg/dwt.c \
	libopenjpeg/event.c \
	libopenjpeg/image.c \
	libopenjpeg/j2k.c \
	libopenjpeg/j2k_lib.c \
	libopenjpeg/jp2.c \
	libopenjpeg/jpt.c \
	libopenjpeg/mct.c \
	libopenjpeg/mqc.c \
	libopenjpeg/openjpeg.c \
	libopenjpeg/pi.c \
	libopenjpeg/raw.c \
	libopenjpeg/t1.c \
	libopenjpeg/t1_generate_luts.c \
	libopenjpeg/t2.c \
	libopenjpeg/tcd.c \
	libopenjpeg/tgt.c


include $(BUILD_STATIC_LIBRARY)


# vim: set sts=8 sw=8 ts=8 noet:
