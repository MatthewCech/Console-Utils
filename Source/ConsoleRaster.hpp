#pragma once
#include "ConsoleColors.hpp"
#include "Field2D.hpp"



namespace RConsole
{
  //The raster info struct, holds info on what is to be drawn at a location and the color.
  struct RasterInfo
  {
    RasterInfo() : Value(0), C(Color::PREVIOUS_COLOR) { }
    RasterInfo(char val, Color col) : Value(val), C(col) { }
    char Value;
    Color C;
  };

  //Console raster class
  class Console;
  class ConsoleRaster
  {
    friend Console;

  public:
    //Constructors
    ConsoleRaster();

    //Method Prototypes
    bool WriteChar(char toDraw, float x, float y, Color color = PREVIOUS_COLOR);
    const Field2D<RasterInfo>& GetRasterData() const;
    void Clear();

    //General
    const unsigned int GetConsoleWidth() const;
    const unsigned int GetConsoleHeight() const;

  private:
    //private member functions
    Field2D<RasterInfo>& GetRasterData();

    //Variables
    unsigned int width_;
    unsigned int height_;
    Field2D<RasterInfo> data_;

  };
}
