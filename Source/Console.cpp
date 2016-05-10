#include "Console.hpp"


namespace RConsole
{
  //Static initialization in non-guaranteed order.
  ConsoleRaster Console::r         = ConsoleRaster();
  unsigned int Console::width_     = rlutil::tcols();
  unsigned int Console::height_    = rlutil::trows();;
  Field2D<bool> Console::modified_ = Field2D<bool>(rlutil::tcols(), rlutil::trows());


  //Clears out the screen based on the previous items written. Clear character is a space.
  void Console::ClearPrevious()
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
  bool Console::WriteRaster(const ConsoleRaster &r)
  {
    for (int i = 0; i < r.GetConsoleHeight(); ++i)
      for (int j = 0; j < r.GetConsoleWidth(); ++j)
      {
        const RasterInfo& ri = r.GetRaster().Peek(j, i);
        if (ri.Value != 0)
        {
          //locate on screen and set color
          rlutil::locate(j, i);

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
      }

    //Return we successfully printed the raster!
    return true;
  }


  //Clear out the screen that the user sees.
  //Note: More expensive than clearing just the previous spaces
  //but less expensive than clearing entire buffer with command.
  void Console::Clear()
  {
    for (int i = 0; i < r.GetConsoleHeight(); ++i)
      for (int j = 0; j < r.GetConsoleWidth(); ++j)
        r.DrawChar(' ', j, i);
  }
}