#pragma once
#include "Definitions.hpp"

#ifdef OS_WINDOWS
// VS complains about unused functions.
#pragma warning(disable: 4505) //Unreferenced local function has been removed
#endif

namespace RFuncs
{


  //Absolute value of int.
  static int Abs(int x)
  {
    if (x < 0) 
      return -x; 
    return x;
  }
}
