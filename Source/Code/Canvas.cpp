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
  #define DEFAULT_WIDTH_SIZE (rlutil::tcols() - 1)
  #define DEFAULT_HEIGHT_SIZE rlutil::trows()

  // Static initialization in non-guaranteed order.
  CanvasRaster Canvas::r_         = CanvasRaster(DEFAULT_WIDTH_SIZE, DEFAULT_HEIGHT_SIZE);
  CanvasRaster Canvas::prev_      = CanvasRaster(DEFAULT_WIDTH_SIZE, DEFAULT_HEIGHT_SIZE);
  bool Canvas::hasLazyInit_       = false;
  bool Canvas::isDrawing_         = true;
  unsigned int Canvas::width_     = DEFAULT_WIDTH_SIZE;
  unsigned int Canvas::height_    = DEFAULT_HEIGHT_SIZE;
  Field2D<bool> Canvas::modified_ = Field2D<bool>(DEFAULT_WIDTH_SIZE, DEFAULT_HEIGHT_SIZE);


    /////////////////////////////
   // Public Member Functions //
  /////////////////////////////
  // Setup with width and height. Can be re-init
  void Canvas::ReInit(unsigned int width, unsigned int height)
  {
    width_ = width;
    height_ = height;
    r_ = CanvasRaster(width, height);
    prev_ = CanvasRaster(width, height);
    modified_ = Field2D<bool>(width, height);
  }


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

    if (x >= width_) return;
    if (y >= height_) return;

    #endif // RConsole_CLIP_CONSOLE

    modified_.GoTo(static_cast<int>(x), static_cast<int>(y));
    modified_.Set(true);
    r_.WriteChar(toWrite, x, y, color);
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

    rlutil::setColor(WHITE);

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


  // Stops the update loop.
  void Canvas::Shutdown()
  {
    isDrawing_ = false;
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

    for(unsigned int x = static_cast<unsigned int>(x1); x < x2; ++x)
    {
      for(unsigned int y = static_cast<unsigned int>(x1); y < y2; ++y)
      {
        Draw(toWrite, static_cast<float>(x), static_cast<float>(y), color);
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


  // Gets the width of the console
  unsigned int Canvas::GetConsoleWidht()
  {
    return width_;
  }


  // Gets the height of the console
  unsigned int Canvas::GetConsoleHeight()
  {
    return height_;
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

      if (ri.Value != 0 && prev_.GetRasterData().Peek(index) != ri)
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
    // Dump only relevant part of stream.
    for (unsigned int i = 0; i < height_; ++i)
    {
      for (unsigned int j = 0; j < width_; ++j)
      {
        const RasterInfo &ri = r_.GetRasterData().Peek(j, i);
        if (fp == stdout)
        {
          rlutil::setColor(ri.C);
          std::cout << ri.Value;//fprintf(fp, "%c", ri.Value);
        }
        else
        {
          std::string line = rlutil::getANSIColor(ri.C) + ri.Value;
          fprintf(fp, "%s", line.c_str());
        }
      }

      std::cout << '\n';
      //fprintf(fp, "\n");
    }

    // Set end color to white when we're done.
    if (fp == stdout)
      rlutil::setColor(WHITE);
  }


  // Crops all of the raster
  void Canvas::CropRaster(FILE *fp, char toTrim)
  {
    // Establish borders.
    unsigned int Xmin = width_;
    unsigned int Xmax = 0;
    unsigned int Ymin = height_;
    unsigned int Ymax = 0;

    for (unsigned int i = 0; i < width_; ++i)
    {
      for (unsigned int j = 0; j < height_; ++j)
      {
        if (r_.GetRasterData().Peek(i, j).Value != toTrim)
        {
          if (i < Xmin) Xmin = i;
          if (j < Ymin) Ymin = j;
          if (i > Xmax) Xmax = i;
          if (j > Ymax) Ymax = j;
        }
      }
    }

    // If we're trimming everything, don't even bother.
    if (Xmin > Xmax) return;
    if (Ymin > Ymax) return;

    // Dump only relevant part of stream.
    for (unsigned int j = Ymin; j <= Ymax; ++j)
    {
      for (unsigned int i = Xmin; i <= Xmax; ++i)
      {
        const RasterInfo &ri = r_.GetRasterData().Peek(i, j);
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
    }

    // Set end color to white when we're done.
    if (fp == stdout)
      rlutil::setColor(WHITE);
  }


  // Handle closing the window
  static void signalHandler(int signalNum)
  {
    Canvas::Shutdown();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    int height = rlutil::trows();
    rlutil::locate(0, height);
    rlutil::setColor(WHITE);
    std::cout << '\n';
    exit(signalNum);
  }
  void Canvas::setCloseHandler()
  {
    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);
  }
}
