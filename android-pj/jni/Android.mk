LOCAL_PATH := $(call my-dir)

# include $(call all-subdir-makefiles)
include common/Android.mk

include $(CLEAR_VARS)
include android/Android.mk

include $(CLEAR_VARS)


LOCAL_MODULE     := AndroAurex
LOCAL_SRC_FILES  := 
LOCAL_CPPFLAGS   := 
LOCAL_LDLIBS     := 
 
include $(BUILD_SHARED_LIBRARY)

