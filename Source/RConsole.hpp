#pragma once
#include "External/rlutil.h"
#include "Field2D.hpp"


#define RConsole_CLIP_CONSOLE //Define we want console clipping
#define RConsole_NO_THREADING //Define we aren't threading- printf becomes unsafe, but faster.


namespace RConsole
{
  //Colors!
  enum ColorData
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


  class Console
  {
  public:
    //Method Prototypes
    static bool DrawPartialPoint(float x, float y, ColorData color = PREVIOUS_COLOR);
    static bool DrawAlphaPoint(float x, float y, ColorData color = PREVIOUS_COLOR, float opacity = 1);
    static bool DrawChar(char toDraw, float x, float y, ColorData color = PREVIOUS_COLOR);
    static void SetColor(ColorData color);
    static void SetCursorVisible(bool isVisible);
    static unsigned int GetConsoleWidth();
    static unsigned int GetConsoleHeight();
    
    //Necessary console values
    static void Update();
    static void Clear();

  private:
    //Hidden Constructors- no instantiating publicly!
    Console() { };
    Console(const Console &rhs) { *this = rhs; }

    //Variables
    static void FullClear();
    static void ClearPrevious();
    static Field2D<bool> modified_;
    static Field2D<char> activeBuffer_;
    static unsigned int width_;
    static unsigned int height_;
  };
}
