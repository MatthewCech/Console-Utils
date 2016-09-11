#pragma once
#include "ConsoleColors.hpp"
#include "Field2D.hpp"



namespace RConsole
{
  //The raster info struct, holds info on what is to be drawn at a location and the color.
  struct RasterInfo
  {
    RasterInfo();
    RasterInfo(const char val, Color col);
    bool operator ==(const RasterInfo &rhs) const;
    bool operator !=(const RasterInfo &rhs) const;
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
	  bool WriteString(const char *toWrite, size_t len, float x, float y, Color color = PREVIOUS_COLOR);
    const Field2D<RasterInfo>& GetRasterData() const;
    void Clear();

    //General
    unsigned int GetConsoleWidth() const;
    unsigned int GetConsoleHeight() const;

  private:
    //private member functions
    Field2D<RasterInfo>& GetRasterData();

    //Variables
    unsigned int width_;
    unsigned int height_;
    Field2D<RasterInfo> data_;

  };
}
