LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

FREETYPE_SRC := freetype

LOCAL_MODULE := freetype

LOCAL_CFLAGS := -DANDROID_NDK \
		-DFT2_BUILD_LIBRARY=1 \
		-Ijni/freetype/include

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include_all \
		$(FREETYPE_SRC)/include \
		$(FREETYPE_SRC)/src

LOCAL_SRC_FILES := \
	$(FREETYPE_SRC)/src/autofit/autofit.c \
	$(FREETYPE_SRC)/src/base/basepic.c \
	$(FREETYPE_SRC)/src/base/ftapi.c \
	$(FREETYPE_SRC)/src/base/ftbase.c \
	$(FREETYPE_SRC)/src/base/ftbbox.c \
	$(FREETYPE_SRC)/src/base/ftbitmap.c \
	$(FREETYPE_SRC)/src/base/ftdbgmem.c \
	$(FREETYPE_SRC)/src/base/ftdebug.c \
	$(FREETYPE_SRC)/src/base/ftglyph.c \
	$(FREETYPE_SRC)/src/base/ftinit.c \
	$(FREETYPE_SRC)/src/base/ftpic.c \
	$(FREETYPE_SRC)/src/base/ftstroke.c \
	$(FREETYPE_SRC)/src/base/ftsynth.c \
	$(FREETYPE_SRC)/src/base/ftsystem.c \
	$(FREETYPE_SRC)/src/cff/cff.c \
	$(FREETYPE_SRC)/src/pshinter/pshinter.c \
	$(FREETYPE_SRC)/src/psnames/psnames.c \
	$(FREETYPE_SRC)/src/raster/raster.c \
	$(FREETYPE_SRC)/src/sfnt/sfnt.c \
	$(FREETYPE_SRC)/src/smooth/smooth.c \
	$(FREETYPE_SRC)/src/truetype/truetype.c

#LOCAL_LDLIBS := -ldl -llog

include $(BUILD_STATIC_LIBRARY)

