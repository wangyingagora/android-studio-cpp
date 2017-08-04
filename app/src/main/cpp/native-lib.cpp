#include <jni.h>
#include <string>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include <android/log.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TAG "android-jni"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__)

const int NUM_CPU_STATES = 20;

enum CPUStates
{
    S_USER = 0,
    S_NICE,
    S_SYSTEM,
    S_IDLE,
    S_IOWAIT,
    S_IRQ,
    S_SOFTIRQ,
    S_STEAL,
    S_GUEST,
    S_GUEST_NICE
};

typedef struct CPUData
{
    std::string cpu;
    size_t times[NUM_CPU_STATES];
} CPUData;

void readStatsCPU(std::vector<CPUData> & entries);

size_t GetIdleTime(const CPUData & e);
size_t GetActiveTime(const CPUData & e);

void printStats(const std::vector<CPUData> & entries1, const std::vector<CPUData> & entries2);

void readStatsCPU(std::vector<CPUData> & entries)
{
    std::ifstream fileStat("/proc/stat");

    std::string line;

    const std::string STR_CPU("cpu");
    const std::size_t LEN_STR_CPU = STR_CPU.size();
    const std::string STR_TOT("tot");

    while(std::getline(fileStat, line))
    {
        // cpu stats line found
        if(!line.compare(0, LEN_STR_CPU, STR_CPU))
        {
            std::istringstream ss(line);

            // store entry
            entries.emplace_back(CPUData());
            CPUData & entry = entries.back();

            // read cpu label
            ss >> entry.cpu;

            // remove "cpu" from the label when it's a processor number
            if(entry.cpu.size() > LEN_STR_CPU) {
                entry.cpu.erase(0, LEN_STR_CPU);
                // replace "cpu" with "tot" when it's total values
            }
            else {
                entry.cpu = STR_TOT;
            }

            // read times
            for(int i = 0; i < NUM_CPU_STATES; ++i) {
                ss >> entry.times[i];
            }
        }
    }
}

size_t GetIdleTime(const CPUData & e)
{
    return	e.times[S_IDLE] + e.times[S_IOWAIT];
}

size_t GetActiveTime(const CPUData & e)
{
    return	e.times[S_USER] +
              e.times[S_NICE] +
              e.times[S_SYSTEM] +
              e.times[S_IRQ] +
              e.times[S_SOFTIRQ] +
              e.times[S_STEAL] +
              e.times[S_GUEST] +
              e.times[S_GUEST_NICE];
}

void printStats(const std::vector<CPUData> & entries1, const std::vector<CPUData> & entries2)
{
    const size_t NUM_ENTRIES = entries1.size();

    for(size_t i = 0; i < NUM_ENTRIES; ++i)
    {
        const CPUData & e1 = entries1[i];
        const CPUData & e2 = entries2[i];

        std::cout.width(3);
        std::cout << e1.cpu << "] ";

        const float ACTIVE_TIME	= static_cast<float>(GetActiveTime(e2) - GetActiveTime(e1));
        const float IDLE_TIME	= static_cast<float>(GetIdleTime(e2) - GetIdleTime(e1));
        const float TOTAL_TIME	= ACTIVE_TIME + IDLE_TIME;

        std::cout << "active: ";
        std::cout.setf(std::ios::fixed, std::ios::floatfield);
        std::cout.width(6);
        std::cout.precision(2);
        float activePercent = (100.f * ACTIVE_TIME / TOTAL_TIME);
        std::cout << activePercent << "%";

        std::cout << " - idle: ";
        std::cout.setf(std::ios::fixed, std::ios::floatfield);
        std::cout.width(6);
        std::cout.precision(2);
        float idlePercent = (100.f * IDLE_TIME / TOTAL_TIME);
        std::cout << idlePercent << "%" << std::endl;
        LOGE("active: %f, idle: %f", activePercent, idlePercent);
    }
}

JNIEXPORT jstring JNICALL Java_io_agrora_rtc_MainActivity_stringFromJNI(JNIEnv *env, jobject /* this */)
{
    std::string hello = "Hello from C++";

    std::vector<CPUData> entries1;
    std::vector<CPUData> entries2;

    // snapshot 1
    readStatsCPU(entries1);

    // 100ms pause
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // snapshot 2
    readStatsCPU(entries2);

    // print output
    printStats(entries1, entries2);

    return env->NewStringUTF(hello.c_str());
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv *env;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}

void JNI_OnUnload(JavaVM* vm, void* reserved) {
}

#ifdef __cplusplus
}
#endif
