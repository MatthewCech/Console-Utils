#include "ConsoleRaster.hpp"
#include "RFuncs.hpp"



namespace RConsole
{
  //Absolute value of int.


  ConsoleRaster::ConsoleRaster() 
    : width_(rlutil::tcols())
    , height_(rlutil::trows())
    , data_(width_, height_)
    , modified_(width_, height_)
  {  }

  ConsoleRaster::ConsoleRaster(const ConsoleRaster &rhs) 
  { 
    if (this == &rhs)
    {

    }
  }

  //Draws a point with ASCII to attempt to represent location in a square.
  bool ConsoleRaster::DrawPartialPoint(float x, float y, Color color)
  {
    //Get first two decimal places from location.
    int xDec = static_cast<int>(x * 100) % 100;
    int yDec = static_cast<int>(x * 100) % 100;


    //If Y is closer to a border, use it for placement.
    if (RFuncs::Abs(50 - x) < RFuncs::Abs(50 - y))
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

    data_.GoTo(x, y);
    data_.Set(RasterInfo(toDraw, color));
  
    //Everything completed correctly.
    return true;
  }

  //Set visibility of cursor to specified bool.
  void ConsoleRaster::SetCursorVisible(bool isVisible)
  {
    if(!isVisible)
      rlutil::hidecursor();
    else
      rlutil::showcursor();
  }


  //Clears out all of the data written to the raster. Does NOT move cursor to 0,0.
  void ConsoleRaster::Clear()
  {
    data_.Zero();
  }
  

  //Get a constant reference to the existing raster.
  const Field2D<RasterInfo>& ConsoleRaster::GetRaster() const
  {
    return data_;
  }

  
  //Gets console width.
  const unsigned int ConsoleRaster::GetConsoleWidth() const 
  { 
    return rlutil::tcols(); 
  }  


  //Get console height
  const unsigned int ConsoleRaster::GetConsoleHeight() const
  { 
    return rlutil::trows(); 
  } 
}
