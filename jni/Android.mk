LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

TOMMATH_SRC := tommath

LOCAL_MODULE := tommath
LOCAL_CFLAGS := -Wall -W -Wsign-compare \
		-O3 -funroll-loops \
		-fomit-frame-pointer

LOCAL_C_INCLUDES := $(TOMMATH_SRC)

LOCAL_SRC_FILES := \
	$(TOMMATH_SRC)/bn_error.c \
	$(TOMMATH_SRC)/bn_fast_mp_invmod.c \
	$(TOMMATH_SRC)/bn_fast_mp_montgomery_reduce.c \
	$(TOMMATH_SRC)/bn_fast_s_mp_mul_digs.c \
	$(TOMMATH_SRC)/bn_fast_s_mp_mul_high_digs.c \
	$(TOMMATH_SRC)/bn_fast_s_mp_sqr.c \
	$(TOMMATH_SRC)/bn_mp_2expt.c \
	$(TOMMATH_SRC)/bn_mp_abs.c \
	$(TOMMATH_SRC)/bn_mp_add.c \
	$(TOMMATH_SRC)/bn_mp_add_d.c \
	$(TOMMATH_SRC)/bn_mp_addmod.c \
	$(TOMMATH_SRC)/bn_mp_and.c \
	$(TOMMATH_SRC)/bn_mp_clamp.c \
	$(TOMMATH_SRC)/bn_mp_clear.c \
	$(TOMMATH_SRC)/bn_mp_clear_multi.c \
	$(TOMMATH_SRC)/bn_mp_cmp.c \
	$(TOMMATH_SRC)/bn_mp_cmp_d.c \
	$(TOMMATH_SRC)/bn_mp_cmp_mag.c \
	$(TOMMATH_SRC)/bn_mp_cnt_lsb.c \
	$(TOMMATH_SRC)/bn_mp_copy.c \
	$(TOMMATH_SRC)/bn_mp_count_bits.c \
	$(TOMMATH_SRC)/bn_mp_div.c \
	$(TOMMATH_SRC)/bn_mp_div_2.c \
	$(TOMMATH_SRC)/bn_mp_div_2d.c \
	$(TOMMATH_SRC)/bn_mp_div_3.c \
	$(TOMMATH_SRC)/bn_mp_div_d.c \
	$(TOMMATH_SRC)/bn_mp_dr_is_modulus.c \
	$(TOMMATH_SRC)/bn_mp_dr_reduce.c \
	$(TOMMATH_SRC)/bn_mp_dr_setup.c \
	$(TOMMATH_SRC)/bn_mp_exch.c \
	$(TOMMATH_SRC)/bn_mp_expt_d.c \
	$(TOMMATH_SRC)/bn_mp_exptmod.c \
	$(TOMMATH_SRC)/bn_mp_exptmod_fast.c \
	$(TOMMATH_SRC)/bn_mp_exteuclid.c \
	$(TOMMATH_SRC)/bn_mp_fread.c \
	$(TOMMATH_SRC)/bn_mp_fwrite.c \
	$(TOMMATH_SRC)/bn_mp_gcd.c \
	$(TOMMATH_SRC)/bn_mp_get_int.c \
	$(TOMMATH_SRC)/bn_mp_grow.c \
	$(TOMMATH_SRC)/bn_mp_init.c \
	$(TOMMATH_SRC)/bn_mp_init_copy.c \
	$(TOMMATH_SRC)/bn_mp_init_multi.c \
	$(TOMMATH_SRC)/bn_mp_init_set.c \
	$(TOMMATH_SRC)/bn_mp_init_set_int.c \
	$(TOMMATH_SRC)/bn_mp_init_size.c \
	$(TOMMATH_SRC)/bn_mp_invmod.c \
	$(TOMMATH_SRC)/bn_mp_invmod_slow.c \
	$(TOMMATH_SRC)/bn_mp_is_square.c \
	$(TOMMATH_SRC)/bn_mp_jacobi.c \
	$(TOMMATH_SRC)/bn_mp_karatsuba_mul.c \
	$(TOMMATH_SRC)/bn_mp_karatsuba_sqr.c \
	$(TOMMATH_SRC)/bn_mp_lcm.c \
	$(TOMMATH_SRC)/bn_mp_lshd.c \
	$(TOMMATH_SRC)/bn_mp_mod.c \
	$(TOMMATH_SRC)/bn_mp_mod_2d.c \
	$(TOMMATH_SRC)/bn_mp_mod_d.c \
	$(TOMMATH_SRC)/bn_mp_montgomery_calc_normalization.c \
	$(TOMMATH_SRC)/bn_mp_montgomery_reduce.c \
	$(TOMMATH_SRC)/bn_mp_montgomery_setup.c \
	$(TOMMATH_SRC)/bn_mp_mul.c \
	$(TOMMATH_SRC)/bn_mp_mul_2.c \
	$(TOMMATH_SRC)/bn_mp_mul_2d.c \
	$(TOMMATH_SRC)/bn_mp_mul_d.c \
	$(TOMMATH_SRC)/bn_mp_mulmod.c \
	$(TOMMATH_SRC)/bn_mp_n_root.c \
	$(TOMMATH_SRC)/bn_mp_neg.c \
	$(TOMMATH_SRC)/bn_mp_or.c \
	$(TOMMATH_SRC)/bn_mp_prime_fermat.c \
	$(TOMMATH_SRC)/bn_mp_prime_is_divisible.c \
	$(TOMMATH_SRC)/bn_mp_prime_is_prime.c \
	$(TOMMATH_SRC)/bn_mp_prime_miller_rabin.c \
	$(TOMMATH_SRC)/bn_mp_prime_next_prime.c \
	$(TOMMATH_SRC)/bn_mp_prime_rabin_miller_trials.c \
	$(TOMMATH_SRC)/bn_mp_prime_random_ex.c \
	$(TOMMATH_SRC)/bn_mp_radix_size.c \
	$(TOMMATH_SRC)/bn_mp_radix_smap.c \
	$(TOMMATH_SRC)/bn_mp_rand.c \
	$(TOMMATH_SRC)/bn_mp_read_radix.c \
	$(TOMMATH_SRC)/bn_mp_read_signed_bin.c \
	$(TOMMATH_SRC)/bn_mp_read_unsigned_bin.c \
	$(TOMMATH_SRC)/bn_mp_reduce.c \
	$(TOMMATH_SRC)/bn_mp_reduce_2k.c \
	$(TOMMATH_SRC)/bn_mp_reduce_2k_l.c \
	$(TOMMATH_SRC)/bn_mp_reduce_2k_setup.c \
	$(TOMMATH_SRC)/bn_mp_reduce_2k_setup_l.c \
	$(TOMMATH_SRC)/bn_mp_reduce_is_2k.c \
	$(TOMMATH_SRC)/bn_mp_reduce_is_2k_l.c \
	$(TOMMATH_SRC)/bn_mp_reduce_setup.c \
	$(TOMMATH_SRC)/bn_mp_rshd.c \
	$(TOMMATH_SRC)/bn_mp_set.c \
	$(TOMMATH_SRC)/bn_mp_set_int.c \
	$(TOMMATH_SRC)/bn_mp_shrink.c \
	$(TOMMATH_SRC)/bn_mp_signed_bin_size.c \
	$(TOMMATH_SRC)/bn_mp_sqr.c \
	$(TOMMATH_SRC)/bn_mp_sqrmod.c \
	$(TOMMATH_SRC)/bn_mp_sqrt.c \
	$(TOMMATH_SRC)/bn_mp_sub.c \
	$(TOMMATH_SRC)/bn_mp_sub_d.c \
	$(TOMMATH_SRC)/bn_mp_submod.c \
	$(TOMMATH_SRC)/bn_mp_to_signed_bin.c \
	$(TOMMATH_SRC)/bn_mp_to_signed_bin_n.c \
	$(TOMMATH_SRC)/bn_mp_to_unsigned_bin.c \
	$(TOMMATH_SRC)/bn_mp_to_unsigned_bin_n.c \
	$(TOMMATH_SRC)/bn_mp_toom_mul.c \
	$(TOMMATH_SRC)/bn_mp_toom_sqr.c \
	$(TOMMATH_SRC)/bn_mp_toradix.c \
	$(TOMMATH_SRC)/bn_mp_toradix_n.c \
	$(TOMMATH_SRC)/bn_mp_unsigned_bin_size.c \
	$(TOMMATH_SRC)/bn_mp_xor.c \
	$(TOMMATH_SRC)/bn_mp_zero.c \
	$(TOMMATH_SRC)/bn_prime_tab.c \
	$(TOMMATH_SRC)/bn_reverse.c \
	$(TOMMATH_SRC)/bn_s_mp_add.c \
	$(TOMMATH_SRC)/bn_s_mp_exptmod.c \
	$(TOMMATH_SRC)/bn_s_mp_mul_digs.c \
	$(TOMMATH_SRC)/bn_s_mp_mul_high_digs.c \
	$(TOMMATH_SRC)/bn_s_mp_sqr.c \
	$(TOMMATH_SRC)/bn_s_mp_sub.c \
	$(TOMMATH_SRC)/bncore.c

include $(BUILD_SHARED_LIBRARY)

# second lib, which will depend on and include the first one
#
include $(CLEAR_VARS)

GIAC_SRC := ../../src

LOCAL_MODULE := giac
LOCAL_CPP_EXTENSION := .cc
LOCAL_CPP_FEATURES += exceptions
LOCAL_CPP_FEATURES += rtti

LOCAL_CFLAGS := -w -g -fPIC -DPIC -fno-strict-aliasing -D__ANDROID__ -DHAVE_CONFIG_H -DGIAC_GENERIC_CONSTANTS 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/tommath \
			$(GIAC_SRC)

LOCAL_SRC_FILES := \
	$(GIAC_SRC)/input_lexer.cc \
	$(GIAC_SRC)/sym2poly.cc \
	$(GIAC_SRC)/gausspol.cc \
	$(GIAC_SRC)/threaded.cc \
	$(GIAC_SRC)/moyal.cc \
	$(GIAC_SRC)/maple.cc \
	$(GIAC_SRC)/ti89.cc \
	$(GIAC_SRC)/mathml.cc \
	$(GIAC_SRC)/misc.cc \
	$(GIAC_SRC)/permu.cc \
	$(GIAC_SRC)/quater.cc \
	$(GIAC_SRC)/desolve.cc \
	$(GIAC_SRC)/input_parser.cc \
	$(GIAC_SRC)/symbolic.cc \
	$(GIAC_SRC)/index.cc \
	$(GIAC_SRC)/modpoly.cc \
	$(GIAC_SRC)/modfactor.cc \
	$(GIAC_SRC)/ezgcd.cc \
	$(GIAC_SRC)/derive.cc \
	$(GIAC_SRC)/solve.cc \
	$(GIAC_SRC)/intg.cc \
	$(GIAC_SRC)/intgab.cc \
	$(GIAC_SRC)/risch.cc \
	$(GIAC_SRC)/lin.cc \
	$(GIAC_SRC)/series.cc \
	$(GIAC_SRC)/subst.cc \
	$(GIAC_SRC)/vecteur.cc \
	$(GIAC_SRC)/csturm.cc \
	$(GIAC_SRC)/tex.cc \
	$(GIAC_SRC)/global.cc \
	$(GIAC_SRC)/ifactor.cc \
	$(GIAC_SRC)/alg_ext.cc \
	$(GIAC_SRC)/gauss.cc \
	$(GIAC_SRC)/isom.cc \
	$(GIAC_SRC)/help.cc \
	$(GIAC_SRC)/plot.cc \
	$(GIAC_SRC)/plot3d.cc \
	$(GIAC_SRC)/rpn.cc \
	$(GIAC_SRC)/prog.cc \
	$(GIAC_SRC)/pari.cc \
	$(GIAC_SRC)/cocoa.cc \
	$(GIAC_SRC)/usual.cc \
	$(GIAC_SRC)/identificateur.cc \
	$(GIAC_SRC)/gen.cc \
	$(GIAC_SRC)/unary.cc \
	$(GIAC_SRC)/tinymt32.cc \
	$(GIAC_SRC)/first.cc \
	$(GIAC_SRC)/sparse.cc

LOCAL_SHARED_LIBRARIES := tommath

LOCAL_LDLIBS :=  -fPIC -ldl -lm -lc -lgcc

include $(BUILD_SHARED_LIBRARY)

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

LOCAL_LDLIBS := -lz

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

FREETYPE_SRC := freetype

LOCAL_MODULE := freetype

LOCAL_CFLAGS := -DANDROID_NDK \
		-DFT2_BUILD_LIBRARY=1

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

LOCAL_LDLIBS := -ldl -llog

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

PNGWRITER_SRC := pngwriter

LOCAL_CPP_EXTENSION := .cc

LOCAL_MODULE := pngwriter

LOCAL_CFLAGS := -DFT2_BUILD_LIBRARY=1 \
		-DUSE_FREETYPE \
		 -O3 -Wall -Wno-deprecated

LOCAL_C_INCLUDES := \
		$(PNGWRITER_SRC) \
		$(LOCAL_PATH)/libpng \
		$(LOCAL_PATH)/freetype/include \
		$(LOCAL_PATH)/freetype/include/freetype

LOCAL_SRC_FILES := \
	$(PNGWRITER_SRC)/pngwriter.cc

LOCAL_SHARED_LIBRARIES := libpng freetype

LOCAL_LDLIBS := -L$(LOCAL_PATH)/lib -ldl -lz

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

XCASPAD_SRC := xcaspad

LOCAL_MODULE := xcaspad
LOCAL_CPP_EXTENSION := .cc .cpp
LOCAL_CPP_FEATURES += exceptions
LOCAL_CPP_FEATURES += rtti

LOCAL_C_INCLUDES := \
		$(XCASPAD_SRC) \
		$(LOCAL_PATH)/../../src \
		$(LOCAL_PATH)/tommath \
		$(LOCAL_PATH)/pngwriter \
		$(LOCAL_PATH)/libpng \
		$(LOCAL_PATH)/freetype/include \
		$(LOCAL_PATH)/freetype/include/freetype		

LOCAL_SRC_FILES := \
		$(XCASPAD_SRC)/main.cc \
		$(XCASPAD_SRC)/Equation.cc \
		$(XCASPAD_SRC)/ftpngwriter.cpp

LOCAL_CPPFLAGS := -w -g -fPIC -DPIC -fno-strict-aliasing -DUSE_GMP_REPLACEMENTS -D__ANDROID__ -DHAVE_CONFIG_H -DGIAC_GENERIC_CONSTANTS -DIN_GIAC -fpermissive  

LOCAL_SHARED_LIBRARIES := tommath giac libpng freetype pngwriter
  
LOCAL_LDLIBS := -fPIC -ldl -llog -lm -lc -lgcc -landroid

include $(BUILD_SHARED_LIBRARY)



