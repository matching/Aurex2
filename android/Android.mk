LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := jniaurex

LOCAL_SRC_FILES  := \
	./main.cpp	\
	./androidruputer.cpp	\
	./graphic/androidlcd.cpp	\
	./log/androidlogoutput.cpp

LOCAL_CPPFLAGS   := -Iandroid/include -Icommon/include -Iunixlib/include  -fexceptions

# for debug
#LOCAL_CPPFLAGS += -DDEBUG -include android/include/androidlog.h
LOCAL_LDLIBS    := -llog -ljnigraphics
LOCAL_STATIC_LIBRARIES := common


include $(BUILD_SHARED_LIBRARY)

