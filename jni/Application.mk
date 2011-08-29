# The ARMv7 is significanly faster due to the use of the hardware FPU
APP_ABI := armeabi armeabi-v7a
APP_CFLAGS 		= -O3 -pipe -fomit-frame-pointer -fstrict-aliasing
APP_CPPFLAGS	= -O3 -pipe -fomit-frame-pointer -fstrict-aliasing -fexceptions -fno-rtti -fprefetch-loop-arrays