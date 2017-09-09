#pragma once

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <string.h>
#include <android/log.h>

#define SAFE_DELETE( p ) { if( p ) { delete ( p ); ( p ) = nullptr; } }
#define SAFE_DELETE_ARRAY( p ) { if( p ) { delete [] ( p ); ( p ) = nullptr; } }

#define UINT unsigned int

#define LOG_TAG "Connections"

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)