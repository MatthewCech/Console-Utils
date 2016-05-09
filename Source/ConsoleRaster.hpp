#pragma once
#include "ConsoleColors.hpp"
#include "Field2D.hpp"



namespace RConsole
{
  //The raster info struct, holds info on what is to be drawn at a location and if it was modified.
  struct RasterInfo
  {
    bool ModFlag;
    char Value;
  };

  class ConsoleRaster
  {
  public:
    //Method Prototypes
    bool DrawPartialPoint(float x, float y, Color color = PREVIOUS_COLOR);
    bool DrawAlphaPoint(float x, float y, Color color = PREVIOUS_COLOR, float opacity = 1);
    bool DrawChar(char toDraw, float x, float y, Color color = PREVIOUS_COLOR);
    void SetColor(Color color);
    void SetCursorVisible(bool isVisible);
    unsigned int GetConsoleWidth();
    unsigned int GetConsoleHeight();
    void FullClear();
    void ClearPrevious();

  private:
    //Variables
    Field2D<RasterInfo> modified_;
    unsigned int width_;
    unsigned int height_;
  };
}
