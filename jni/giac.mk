LOCAL_PATH := $(call my-dir)

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

LOCAL_STATIC_LIBRARIES := tommath

#LOCAL_LDLIBS :=  -fPIC -ldl -lm -lc -lgcc

include $(BUILD_STATIC_LIBRARY)
