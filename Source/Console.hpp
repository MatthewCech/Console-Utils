#pragma once
#include <cstdio>
#include <iostream>
#include "External/rlutil.h"
#include "ConsoleRaster.hpp"
#include "Field2D.hpp"



#define RConsole_CLIP_CONSOLE //Define we want console clipping
#define RConsole_NO_THREADING //Define we aren't threading- printf becomes unsafe, but faster.


namespace RConsole
{
  class Console
  {
  public:
    //Write the raster we were attempting to write.
    static void Clear();

  private:
    //Hidden Constructors- no instantiating publicly!
    Console() { };
    Console(const Console &rhs) { *this = rhs; }
    
    //Private methods.
    static void ClearPrevious();
    static void FullClear();
    static void SetColor(const Color &color);
    static bool WriteRaster(const ConsoleRaster &r);

    //Any rasters we have. Could be expanded to have two, so you could "swap" them,
    //Although practicality of that is limited given the clearing technique.
    static ConsoleRaster r;

    //The tabs on what was last modified.
    static unsigned int width_;
    static unsigned int height_;
    static Field2D<bool> modified_;
  };
}
