
#include <android/log.h>


#define fprintf(a,...) __android_log_print(ANDROID_LOG_DEBUG, "androaurex", __VA_ARGS__)
//#define printf(...) __android_log_print(ANDROID_LOG_DEBUG, "androaurex", __VA_ARGS__)

