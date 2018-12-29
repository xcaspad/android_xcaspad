LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LIBPNG_SRC := libpng

LOCAL_MODULE    := libpng
LOCAL_C_INCLUDES := $(LIBPNG_SRC)
LOCAL_SRC_FILES :=\
	$(LIBPNG_SRC)/png.c \
	$(LIBPNG_SRC)/pngerror.c \
	$(LIBPNG_SRC)/pngget.c \
	$(LIBPNG_SRC)/pngmem.c \
	$(LIBPNG_SRC)/pngpread.c \
	$(LIBPNG_SRC)/pngread.c \
	$(LIBPNG_SRC)/pngrio.c \
	$(LIBPNG_SRC)/pngrtran.c \
	$(LIBPNG_SRC)/pngrutil.c \
	$(LIBPNG_SRC)/pngset.c \
	$(LIBPNG_SRC)/pngtest.c \
	$(LIBPNG_SRC)/pngtrans.c \
	$(LIBPNG_SRC)/pngwio.c \
	$(LIBPNG_SRC)/pngwrite.c \
	$(LIBPNG_SRC)/pngwtran.c \
	$(LIBPNG_SRC)/pngwutil.c

#LOCAL_LDLIBS := -lz

include $(BUILD_STATIC_LIBRARY)
