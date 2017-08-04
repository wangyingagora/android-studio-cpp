#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jstring JNICALL
Java_io_agora_rtc_lib_RTCEngine_getVersion(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "version 1.13.0, lalallalalalall";
    return env->NewStringUTF(hello.c_str());
}
