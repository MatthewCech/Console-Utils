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


/////// COMPILER DEFINES /////////

#if defined(_MSC_VER)
  #define COMPILER_VS
#endif


/////// CONSOLE SETTINGS /////////

// Efficiency settings
#define RConsole_CLIP_CONSOLE // Define we want console clipping
#define RConsole_NO_THREADING // Define we aren't threading- printf becomes unsafe, but faster.

// Defaults
#define CONSOLE_WIDTH rlutil::tcols()                       // Default Canvas width
#define CONSOLE_HEIGHT rlutil::trows()                      // Default Canvas height
#define RASTERINFO_DEFAULT RasterInfo(' ', RConsole::WHITE) // Default raster info object.
