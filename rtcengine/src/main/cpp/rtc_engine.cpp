#include "rtc_engine.h"

std::string rtcVersion()
{
    std::string version = "RTC Version: 0.13.0\n";
    return version;
}
/*
extern "C"
JNIEXPORT jstring JNICALL
Java_io_agora_rtc_lib_RTCEngine_getVersion(
        JNIEnv *env,
        jobject) {
    std::string hello = "version 1.13.0, lalallalalalall";
    return env->NewStringUTF(hello.c_str());
}
*/



