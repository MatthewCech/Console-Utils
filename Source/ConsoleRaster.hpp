#pragma once
#include "ConsoleColors.hpp"
#include "Field2D.hpp"



namespace RConsole
{
  //The raster info struct, holds info on what is to be drawn at a location and the color.
  struct RasterInfo
  {
    RasterInfo(char val, Color col) : Value(val), C(col) { }
    char Value;
    Color C;
  };

  class ConsoleRaster
  {
  public:
    //Constructors
    ConsoleRaster();
    ConsoleRaster(const ConsoleRaster &rhs);

    //Method Prototypes
    bool DrawPartialPoint(float x, float y, Color color = PREVIOUS_COLOR);
    bool DrawAlphaPoint(float x, float y, Color color = PREVIOUS_COLOR, float opacity = 1);
    bool DrawChar(char toDraw, float x, float y, Color color = PREVIOUS_COLOR);
    void SetColor(Color color);
    void SetCursorVisible(bool isVisible);
    const Field2D<RasterInfo>& GetRaster() const;
    void Clear();

    //General
    const unsigned int GetConsoleWidth() const;
    const unsigned int GetConsoleHeight() const;

  private:
    //Variables
    unsigned int width_;
    unsigned int height_;
    Field2D<RasterInfo> data_;

  };
}
