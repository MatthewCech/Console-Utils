#include "ConsoleRaster.hpp"



namespace RConsole
{
  //Define static variables. I have to call the methods repeatedly because static
  //initialization order is not guaranteed.
  Field2D<bool> ConsoleRaster::modified_ = Field2D<bool>(rlutil::tcols(), rlutil::trows());
  unsigned int ConsoleRaster::width_ = rlutil::tcols();
  unsigned int ConsoleRaster::height_ = rlutil::trows();;


  //Absolute value of int.
  int abs(int x) { if (x < 0) return -x; return x; }
  ConsoleRaster()
  {

  }
      ConsoleRaster(const ConsoleRaster &rhs) { *this = rhs; }

  //Draws a point with ASCII to attempt to represent location in a square.
  bool ConsoleRaster::DrawPartialPoint(float x, float y, Color color)
  {
    //Get first two decimal places from location.
    int xDec = static_cast<int>(x * 100) % 100;
    int yDec = static_cast<int>(x * 100) % 100;


    //If Y is closer to a border, use it for placement.
    if (abs(50 - x) < abs(50 - y))
    {
      if (x > 50)
        return DrawChar(222, x, y, color);
      return DrawChar(221, x, y, color);
    }
    //Otherwise, X is closer to a border.
    else
    {
      if (y > 50)
        return DrawChar(223, x, y, color);
      return DrawChar(220, x, y, color);
    }
  }


  //Draws a point with ASCII to attempt to represent alpha values in 4 steps.
  bool ConsoleRaster::DrawAlphaPoint(float x, float y, Color color, float opacity)
  {
    //All characters use represent alt-codes. 
    if (opacity < .25)
      return DrawChar(176, x, y, color);
    else if (opacity < .5)
      return DrawChar(177, x, y, color);
    else if (opacity < .75)
      return DrawChar(178, x, y, color);
    return DrawChar(219, x, y, color);
  }


  //Draws a character to the screen. Returns if it was successful or not.
  bool ConsoleRaster::DrawChar(char toDraw, float x, float y, Color color)
  {
    //Truncate floats
    int xLoc = static_cast<int>(x + 1);
    int yLoc = static_cast<int>(y + 1);

    //Handle clipping the console if we define that tag.
#ifdef RConsole_CLIP_CONSOLE
  //Handle X
    int width = GetConsoleWidth();
    if (xLoc > width)
      return false;
    else if (xLoc < 0)
      return false;

    //Handle Y
    int height = GetConsoleHeight();
    if (yLoc > height)
      return false;
    else if (yLoc < 0)
      return false;
#endif

    //locate on screen and set color
    rlutil::locate(xLoc, yLoc);

    //Set color of cursor
    SetColor(color);

    //Print out to the console in the preferred fashion
    int retVal = 0;
#ifdef RConsole_NO_THREADING
    retVal = _putc_nolock(toDraw, stdout);
#else
    retVal = putc(toDraw, stdout);
#endif
    if (!retVal)
      return false;

    //Everything completed correctly.
    return true;
  }


  //Set the color
  void ConsoleRaster::SetColor(Color color)
  {
    if(color != PREVIOUS_COLOR)
      rlutil::setColor(color);
  }


  //Explicitly clears every possible index. This is expensive! 
  void ConsoleRaster::FullClear()
  {
    rlutil::cls();
  }


  //Clears out the screen based on the previous items written. Clear character is a space.
  void ConsoleRaster::ClearPrevious()
  {
    //Walk through, write over only what was modified.
    modified_.SetIndex(0);
    for (int i = 0; i < width_; ++i)
      for (int j = 0; j < height_; ++j)
      {
        if (modified_.Get())
        {
          //locate on screen and set color
          rlutil::locate(i + 1, j + 1);

        #ifdef RConsole_NO_THREADING
          _putc_nolock(' ', stdout);
        #else
          retVal = putc(toDraw, stdout);
        #endif
        }
        modified_.IncrementX();
      }

    //Set things back to zero.
    modified_.Zero();
    modified_.SetIndex(0);
  }


  //Set visibility of cursor to specified bool.
  void ConsoleRaster::SetCursorVisible(bool isVisible)
  {
    if(!isVisible)
      rlutil::hidecursor();
    else
      rlutil::showcursor();
  }

  static bool Update()
  {

  }

  
  //Gets console dimensions.
  unsigned int ConsoleRaster::GetConsoleWidth() { return rlutil::tcols(); }
  unsigned int ConsoleRaster::GetConsoleHeight() { return rlutil::trows(); }
}
