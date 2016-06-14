#include <cstdio>
#include <iostream>
#include "RFuncs.hpp"
#include "Console.hpp"
#include <rlutil.h>
#include "ConsoleRaster.hpp"


namespace RConsole
{
  //Static initialization in non-guaranteed order.
  ConsoleRaster Console::r_           = ConsoleRaster();
  ConsoleRaster Console::prev_        = ConsoleRaster();
  unsigned int Console::width_        = rlutil::tcols();
  unsigned int Console::height_       = rlutil::trows();;
  Field2D<bool> Console::modified_    = Field2D<bool>(rlutil::tcols(), rlutil::trows());


    ///////////////////////////
   //Public Member Functions//
  ///////////////////////////
  //Write the specific character in a specific color to a specific location on the console.
  void Console::Draw(char toWrite, float x, float y, Color color)
  {
    modified_.GoTo(static_cast<int>(x), static_cast<int>(y));
    modified_.Set(true);
    r_.WriteChar(toWrite, x, y, color);
  }


  //Call previous draw with int instead.
  void Console::Draw(char toWrite, int x, int y, Color color)
  {
    Draw(toWrite, static_cast<float>(x), static_cast<float>(y), color);
  }

  //Updates the current raster by drawing it to the screen.
  bool Console::Update()
  {

    ClearPrevious();
    WriteRaster(r_);
    //Write and reset the raster.
    memcpy(prev_.GetRasterData().GetHead(), r_.GetRasterData().GetHead(), width_ * height_ * sizeof(RasterInfo));
    r_.Clear();
    return true;
  }


  //Draws a point with ASCII to attempt to represent alpha values in 4 steps.
  void Console::DrawAlpha(float x, float y, Color color, float opacity)
  {
    //All characters use represent alt-codes. 
    if (opacity < .25)
      Draw(static_cast<unsigned char>(176), x, y, color);
    else if (opacity < .5)
      Draw(static_cast<unsigned char>(177), x, y, color);
    else if (opacity < .75)
      Draw(static_cast<unsigned char>(178), x, y, color);
    else
      Draw(static_cast<unsigned char>(219), x, y, color);
  }


  //Int version of above function
  void Console::DrawAlpha(int x, int y, Color color, float opacity)
  {
    DrawAlpha(static_cast<float>(x), static_cast<float>(y), color, opacity);
  }


  //Draws a point with ASCII to attempt to represent location in a square.
  void Console::DrawPartialPoint(float x, float y, Color color)
  {
    //Get first two decimal places from location.
    int xDec = static_cast<int>(x * 100) % 100;
    int yDec = static_cast<int>(x * 100) % 100;


    //If Y is closer to a border, use it for placement.
    if (RFuncs::Abs(50 - static_cast<int>(x)) < RFuncs::Abs(50 - static_cast<int>(y)))
    {
      if (x > 50)
        return Draw(static_cast<unsigned char>(222), x, y, color);
      return Draw(static_cast<unsigned char>(221), x, y, color);
    }
    //Otherwise, X is closer to a border.
    else
    {
      if (y > 50)
        return Draw(static_cast<unsigned char>(223), x, y, color);
      return Draw(static_cast<unsigned char>(220), x, y, color);
    }
  }


  //Set visibility of cursor to specified bool.
  void Console::SetCursorVisible(bool isVisible)
  {
    if (!isVisible)
      rlutil::hidecursor();
    else
      rlutil::showcursor();
  }


    ////////////////////////////
   //Private Member Functions//
  ////////////////////////////
  //Clears out the screen based on the previous items written. Clear character is a space.
  void Console::ClearPrevious()
  {
    //Walk through, write over only what was modified.
    modified_.SetIndex(0);
    unsigned int maxIndex = width_ * height_;
    unsigned int index = 0;
    while (index < maxIndex)
    {
      index = modified_.GetIndex();
      char curr = r_.GetRasterData().Peek(index).Value;
      if (curr == 176)
        index++;
      char prev = prev_.GetRasterData().Peek(index).Value;
      //If we modified this one...
      if (!modified_.Get() && prev != curr)
      {
        //Compute X and Y location
        unsigned int xLoc = (index % width_) + 1;
        unsigned int yLoc = (index / width_) + 1;

        //locate on screen and set color
        rlutil::locate(xLoc, yLoc);

        #ifdef RConsole_NO_THREADING
          _putc_nolock(' ', stdout);
        #else
          retVal = putc(' ', stdout);
        #endif
        }
        modified_.IncrementX();
    }

    //Set things back to zero.
    modified_.Zero();
  }


  //Explicitly clears every possible index. This is expensive! 
  void Console::FullClear()
  {
    rlutil::cls();
  }

  
  //Set the color in the console using utility, if applicable.
  void Console::SetColor(const Color &color)
  {
    if (color != PREVIOUS_COLOR)
      rlutil::setColor(color);
  }


  //Write the raster we were attempting to write.
  bool Console::WriteRaster(ConsoleRaster &r)
  {
    //Set initial position.
    unsigned int maxIndex = width_ * height_;
    r.GetRasterData().SetIndex(0);
    unsigned int index = 0;
    while(index < maxIndex)
    {
      index = r.GetRasterData().GetIndex();
      const RasterInfo& ri = r.GetRasterData().Get();

      if (ri.Value != 0 && prev_.GetRasterData().Peek(index).Value != ri.Value)
      {
        unsigned int xLoc = (index % width_) + 1;
        unsigned int yLoc = (index / width_) + 1;

        //Handle clipping the console if we define that tag.
      #ifdef RConsole_CLIP_CONSOLE
        //Handle X

        if (xLoc > width_)
          return false;
        else if (xLoc < 0)
          return false;

        //Handle Y
        if (yLoc > height_)
          return false;
        else if (yLoc < 0)
          return false;
      #endif


        //locate on screen and set color
        rlutil::locate(xLoc, yLoc);

        //Set color of cursor
        SetColor(ri.C);

        //Print out to the console in the preferred fashion
        int retVal = 0;
      #ifdef RConsole_NO_THREADING
        retVal = _putc_nolock(ri.Value, stdout);
      #else
        retVal = putc(ri.Value, stdout);
      #endif
        if (!retVal)
          return false;
      }

      //Increment X location
      r.GetRasterData().IncrementX();
    }

    //Return we successfully printed the raster!
    return true;
  }


  //Clear out the screen that the user sees.
  //Note: More expensive than clearing just the previous spaces
  //but less expensive than clearing entire buffer with command.
  void Console::Clear()
  {
    r_.Clear();
  }
}
