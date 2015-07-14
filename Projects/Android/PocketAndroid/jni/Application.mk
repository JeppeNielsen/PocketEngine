#when building for simulator use x86, much faster than arm, uncomment when building for device
APP_ABI := x86
APP_CPPFLAGS += -std=c++11
APP_STL := gnustl_static
NDK_TOOLCHAIN_VERSION := 4.9