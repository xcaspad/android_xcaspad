LOCAL_PATH := $(call my-dir)

include jni/tommath.mk
include jni/giac.mk
include jni/libpng.mk
include jni/freetype.mk
include jni/pixman.mk
include jni/cairo.mk
include $(CLEAR_VARS)

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/../../src \
		$(LOCAL_PATH)/tommath \
		$(LOCAL_PATH)/cpp-base64 \

LOCAL_MODULE    := xcaspad
LOCAL_CFLAGS    := -O2 --std=c99 -I. -Ijni/freetype/include -Ijni/pixman/pixman -Ijni/cairo/src -Ijni/cairo-extra -Ijni/pixman-extra -Wno-missing-field-initializers
LOCAL_CPPFLAGS 	:= -w -g -fPIC -DPIC -fno-strict-aliasing -DUSE_GMP_REPLACEMENTS -D__ANDROID__ -DHAVE_CONFIG_H -DGIAC_GENERIC_CONSTANTS -DIN_GIAC -fpermissive
LOCAL_LDLIBS    := -lz -lm -llog -ljnigraphics
LOCAL_SRC_FILES := main.cpp equation.cpp prettyprint.cpp cpp-base64/base64.cpp
LOCAL_STATIC_LIBRARIES := libtommath libgiac lpng -lfreetype libcairo libpixman cpufeatures

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/cpufeatures)
