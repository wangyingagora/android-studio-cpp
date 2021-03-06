# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.0.2)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

message(STATUS "App binary path: " ${PROJECT_BINARY_DIR})

include_directories(../rtcengine/src/main/cpp)

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.cpp )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

#find_library( # Sets the name of the path variable.
              # log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              # log )


# Import Static Library
#add_library(libRTCEngine STATIC IMPORTED)
#set_target_properties(libRTCEngine PROPERTIES IMPORTED_LOCATION ${RTC_ENGINE_DIR}/${ANDROID_ABI}/libRTCEngine.a)
#target_link_libraries(native-lib log z libRTCEngine)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

#set(RTC_LIB_DIR, ${PROJECT_SOURCE_DIR}/../rtcengine/build/intermediates/cmake/release/obj/armeabi-v7a)
#add_subdirectory(${PROJECT_SOURCE_DIR}/../rtcengine/)
#link_directories(${RTC_LIB_DIR})
#add_library(librtcengine STATIC IMPORTED)
#set_target_properties(librtcengine PROPERTIES IMPORTED_LOCATION ${RTC_LIB_DIR}/librtcengine.a)

target_link_libraries( # Specifies the target library.
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK.
                       log debug rtcengine)
