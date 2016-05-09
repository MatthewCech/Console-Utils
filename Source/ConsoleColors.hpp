#pragma once
#include "External\rlutil.h"



namespace RConsole
{
  //Colors!
  enum Color
  {
    //Acquire rlutil info where possible--
    BLACK = rlutil::BLACK,
    BLUE = rlutil::BLUE,
    GREEN = rlutil::GREEN,
    CYAN = rlutil::CYAN,
    RED = rlutil::RED,
    MAGENTA = rlutil::MAGENTA,
    BROWN = rlutil::BROWN,
    GREY = rlutil::GREY,
    DARKGREY = rlutil::DARKGREY,
    LIGHTBLUE = rlutil::LIGHTBLUE,
    LIGHTGREEN = rlutil::LIGHTGREEN,
    LIGHTCYAN = rlutil::LIGHTCYAN,
    LIGHTRED = rlutil::LIGHTRED,
    LIGHTMAGENTA = rlutil::LIGHTMAGENTA,
    YELLOW = rlutil::YELLOW,
    WHITE = rlutil::WHITE,

    //Add custom values
    PREVIOUS_COLOR
  };
}
