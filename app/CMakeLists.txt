# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             native-engine-lib SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/native-engine-lib.cpp
             src/main/cpp/engine/engine.cpp
             src/main/cpp/engine/updater.cpp
             src/main/cpp/connections/engine.cpp
             src/main/cpp/connections/updater.cpp
             src/main/cpp/connections/point_shader.cpp
             src/main/cpp/connections/connection_shader.cpp
             )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log)

find_library( OPENGL_LIB GLESv3 )
find_library( EGL_LIB EGL )
find_library( ANDROID_LIB android)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-engine-lib
                       ${OPENGL_LIB}
                       ${EGL_LIB}
                       ${ANDROID_LIB}

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib})