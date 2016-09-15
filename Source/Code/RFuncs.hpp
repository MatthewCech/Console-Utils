#pragma once
#include "Definitions.hpp"

#ifdef COMPILER_VS
// VS complains about unused functions.
#pragma warning(disable: 4505) //Unreferenced local function has been removed
#else
// g++/clang++ complain about unused functions.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif



namespace RFuncs
{

  // Absolute value of int.
  static int Abs(int x)
  {
    if (x < 0) 
      return -x; 
    return x;
  }
}



#ifdef COMPILER_VS
// restore VS warning
#pragma warning(default: 4505) //Unreferenced local function has been removed
#else
// restore g++/glang warnings
#pragma GCC diagnostic pop
#endif
