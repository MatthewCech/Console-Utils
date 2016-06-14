#pragma once

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
