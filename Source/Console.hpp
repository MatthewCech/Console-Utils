#pragma once
#include "External/rlutil.h"
#include "Field2D.hpp"


#define RConsole_CLIP_CONSOLE //Define we want console clipping
#define RConsole_NO_THREADING //Define we aren't threading- printf becomes unsafe, but faster.


namespace RConsole
{
  class Console
  {
  public:

  private:
    //Hidden Constructors- no instantiating publicly!
    Console() { };
    Console(const Console &rhs) { *this = rhs; }
  };
}
