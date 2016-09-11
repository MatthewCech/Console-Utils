#pragma once

// For strict unused variable warnings.
#define UNUSED(x) (void)(x)


/////// OS DEFINES /////////


// Windows
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
  #define OS_WINDOWS
#endif

// MacOSX
#if defined(__APPLE__)
  #define OS_MACOSX
#endif

// Linux
#if defined(__linux__)
  #define OS_LINUX
#endif

#if defined(OS_MACOSX) || defined(OS_LINUX)
  #define OS_POSIX
#endif


