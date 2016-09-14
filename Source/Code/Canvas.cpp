#include <cstdio>           // PutC
#include <iostream>         // ostream access
#include <csignal>          // Signal termination.
#include <rlutil.h>         // Console drawing 
#include <chrono>           // Time related info for sleeping.
#include <thread>           // Sleep on exit to allow for update to finish.
#include <string>           // String for parsing.
#include "Definitions.hpp"
#include "RFuncs.hpp"
#include "Canvas.hpp"
#include "CanvasRaster.hpp"
#include "Definitions.hpp"



namespace RConsole
{
  // Static initialization in non-guaranteed order.
  CanvasRaster Canvas::r_         = CanvasRaster();
  CanvasRaster Canvas::prev_      = CanvasRaster();
  bool Canvas::hasLazyInit_       = false;
  bool Canvas::isDrawing_         = true;
  unsigned int Canvas::width_     = CONSOLE_WIDTH;
  unsigned int Canvas::height_    = CONSOLE_HEIGHT;
  Field2D<bool> Canvas::modified_ = Field2D<bool>(CONSOLE_WIDTH, CONSOLE_HEIGHT);

    /////////////////////////////
   // Public Member Functions //
  /////////////////////////////
  // Clear out the screen that the user sees.
  // Note: More expensive than clearing just the previous spaces
  // but less expensive than clearing entire buffer with command.
  void Canvas::FillCanvas(const RasterInfo &ri)
  {
    r_.Fill(ri);
  }

  // Write the specific character in a specific color to a specific location on the console.
  void Canvas::Draw(char toWrite, float x, float y, Color color)
  {
    #ifdef RConsole_CLIP_CONSOLE

    if (x > width_) return;
    if (y > height_) return;

    #endif // RConsole_CLIP_CONSOLE

    modified_.GoTo(static_cast<int>(x), static_cast<int>(y));
    modified_.Set(true);
    r_.WriteChar(toWrite, x, y, color);
  }


  // Call previous draw with int instead.
  void Canvas::Draw(char toWrite, int x, int y, Color color)
  {
    Draw(toWrite, static_cast<float>(x), static_cast<float>(y), color);
  }

  // Draw a string
  void Canvas::DrawString(const char* toDraw, float xStart, float yStart, Color color)
  {
	  size_t len = strlen(toDraw);
	  if (len <= 0) return;

    #ifdef RConsole_CLIP_CONSOLE

    // Bounds check.
    if (xStart > width_) return;
    if (yStart > height_) return;

	  // Set the memory we are using to modified.
	  modified_.GoTo(static_cast<int>(xStart), static_cast<int>(yStart));
	  unsigned int index = modified_.GetIndex();
    if (xStart > width_) return;

    // Checks the length and adjusts if it will be past.
    int writeLen = len;
    if (writeLen + index > modified_.Length())
      writeLen = modified_.Length() - index;

    // If our length plus the index we are at exceeds the end of the buffer,
    memset(modified_.GetHead() + index, true, writeLen);

    #else

    // Just blindly set modified for the length.
    modified_.GoTo(static_cast<int>(xStart), static_cast<int>(yStart));
    unsigned int index = modified_.GetIndex();
	  memset(modified_.GetHead() + index, true, len);

    #endif


	  // Write string
	  r_.WriteString(toDraw, len, xStart, yStart, color);
  }

  // Updates the current raster by drawing it to the screen.
  bool Canvas::Update()
  {
    if (!isDrawing_) return false;
    
    if (!hasLazyInit_)
    {
      setCloseHandler();
      hasLazyInit_ = true;
    }

    clearPrevious();
    writeRaster(r_);
    
    // Write and reset the raster.
    memcpy(prev_.GetRasterData().GetHead(), r_.GetRasterData().GetHead(), width_ * height_ * sizeof(RasterInfo));
    r_.Zero();

    return true;
  }


  // Draws a point with ASCII to attempt to represent alpha values in 4 steps.
  void Canvas::DrawAlpha(float x, float y, Color color, float opacity)
  {
    // All characters use represent alt-codes. 
    if (opacity < .25)
      Draw(static_cast<unsigned char>(176), x, y, color);
    else if (opacity < .5)
      Draw(static_cast<unsigned char>(177), x, y, color);
    else if (opacity < .75)
      Draw(static_cast<unsigned char>(178), x, y, color);
    else
      Draw(static_cast<unsigned char>(219), x, y, color);
  }


  // Int version of above function
  void Canvas::DrawAlpha(int x, int y, Color color, float opacity)
  {
    DrawAlpha(static_cast<float>(x), static_cast<float>(y), color, opacity);
  }


  // Draws a point with ASCII to attempt to represent location in a square.
  void Canvas::DrawPartialPoint(float x, float y, Color color)
  {
    // Get first two decimal places from location.
    int xDec = static_cast<int>(x * 100) % 100;
    int yDec = static_cast<int>(x * 100) % 100;


    UNUSED(xDec);
    UNUSED(yDec);


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

  // Drawing box
  void Canvas::DrawBox(char toWrite, float x1, float y1, float x2, float y2, Color color)
  {
    
    if(x1 > x2)
    {
      float temp = x1;
      x2 = x1;
      temp = x2;
    }
    if(y1 > y2)
    {
      float temp = y1;
      y2 = y1;
      temp = y2;
    }

    // At this point it can be assumed that x1 and y1 and lower than x2 and y2 respectively.

    for(int x = static_cast<int>(x1); x < x2; ++x)
    {
      for(int y = static_cast<int>(x1); y < y2; ++y)
      {
        Draw(toWrite, x, y, color);
      }
    }
  }



  //Set visibility of cursor to specified bool.
  void Canvas::SetCursorVisible(bool isVisible)
  {
    if (!isVisible)
      rlutil::hidecursor();
    else
      rlutil::showcursor();
  }


    //////////////////////////////
   // Private Member Functions //
  //////////////////////////////
  // Clears out the screen based on the previous items written. Clear character is a space.
  void Canvas::clearPrevious()
  {
    // Walk through, write over only what was modified.
    modified_.SetIndex(0);
    unsigned int maxIndex = width_ * height_;
    unsigned int index = 0;
    while (index < maxIndex)
    {
      index = modified_.GetIndex();
      const RasterInfo &curr = r_.GetRasterData().Peek(index);
      const RasterInfo &prev = prev_.GetRasterData().Peek(index);

      // If we have not modified the space,
      // and we don't have the same character as last time,
      // and we don't have the same color.
      if (!modified_.Get() && curr != prev)
      {
        // Compute X and Y location
        unsigned int xLoc = (index % width_) + 1;
        unsigned int yLoc = (index / width_) + 1;

        // locate on screen and set color
        rlutil::locate(xLoc, yLoc);

        putC(' ', stdout);
      }
      modified_.IncrementX();
    }

    // Set things back to zero.
    modified_.Zero();
  }


  // Explicitly clears every possible index. This is expensive! 
  void Canvas::fullClear()
  {
    rlutil::cls();
  }

  
  // Set the color in the console using utility, if applicable.
  void Canvas::setColor(const Color &color)
  {
    if (color != PREVIOUS_COLOR)
      rlutil::setColor(color);
  }


  // Write the raster we were attempting to write.
  bool Canvas::writeRaster(CanvasRaster &r)
  {
    // Set initial position.
    unsigned int maxIndex = width_ * height_;
    r.GetRasterData().SetIndex(0);
    unsigned int index = 0;
    while(index < maxIndex)
    {
      index = r.GetRasterData().GetIndex();
      const RasterInfo& ri = r.GetRasterData().Get();

      if (ri.Value != 0 && ri.Value != ' ' && prev_.GetRasterData().Peek(index) != ri)
      {
        unsigned int xLoc = (index % width_) + 1;
        unsigned int yLoc = (index / width_) + 1;

        // Handle clipping the console if we define that tag.
      #ifdef RConsole_CLIP_CONSOLE
        // Handle X
        if (xLoc > width_)
          return false;

        // Handle Y
        if (yLoc > height_)
          return false;
      #endif


        // locate on screen and set color
        rlutil::locate(xLoc, yLoc);

        // Set color of cursor
        setColor(ri.C);

        // Print out to the console in the preferred fashion
        int retVal = 0;

        retVal = putC(ri.Value, stdout);

        if (!retVal)
          return false;
      }

      // Increment X location
      r.GetRasterData().IncrementX();
    }

    // Return we successfully printed the raster!
    return true;
  }

  // Cross-platform putc
  int Canvas::putC(int character, FILE * stream )
  {
    #if defined(RConsole_NO_THREADING) && defined(OS_WINDOWS)
      return _putc_nolock(character, stream);
    #else
      return putc(character, stream);
    #endif
  }


  // print out the formatted raster.
  // Note that because of console color formatting, we use the RLUTIL coloring option when
  // we are printing to the console, or have no file output specified.
  void Canvas::DumpRaster(FILE * fp)
  {
    for (unsigned int i = 0; i < r_.height_; ++i)
    {
      // Assemble raw line.
      std::string beingTrimmed = "";
      for (unsigned int j = 0; j < r_.width_; ++j)
      {
        const RasterInfo &ri = r_.GetRasterData().Get(j, i);
        beingTrimmed += ri.Value;
      }

      // Strip trailing spaces agressively.
      unsigned int trimmedLen = beingTrimmed.length();
      for (;;)
      {
        if (beingTrimmed[trimmedLen - 1] == ' ')
        {
          if (trimmedLen > 1)
            --trimmedLen;
          else
            break;
        }
        else
          break;
      }


      // Construct full line with colors.
      for (unsigned int j = 0; j < trimmedLen; ++j)
      {
        const RasterInfo &ri = r_.GetRasterData().Get(j, i);
        if (fp == stdout)
        {
          rlutil::setColor(ri.C);
          fprintf(fp, "%c", ri.Value);
        }
        else
        {
          std::string line = rlutil::getANSIColor(ri.C) + ri.Value;
          fprintf(fp, "%s", line.c_str());
        }
      }
      fprintf(fp, "\n");
      if(fp == stdout)
        rlutil::setColor(WHITE);
    }
  }


  // Handle closing the window
  static void signalHandler(int signalNum)
  {
    Canvas::Shutdown();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    int height = rlutil::trows();
    rlutil::locate(0, height);
    rlutil::setColor(WHITE);
    std::cout << std::endl;
    exit(signalNum);
  }
  void Canvas::setCloseHandler()
  {
    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);
  }


    ///////////////////////
   // Canvas Management //
  ///////////////////////
  // Shuts down the canvas, preventing further drawing.
  void Canvas::Shutdown()
  {
    isDrawing_ = false;
  }


  // Sets the size of the canvas, wiping everything and initalizing all data structures again.
  void Canvas::SetSize(int width, int height)
  {
    width_ = width;
    height_ = height;
    modified_ = Field2D<bool>(width, height);
    r_.ForceResize(width, height);
    prev_.ForceResize(width, height);
  }
}
