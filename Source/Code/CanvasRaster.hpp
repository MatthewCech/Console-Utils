#pragma once
#include "Colors.hpp"
#include "Field2D.hpp"



namespace RConsole
{
  // The raster info struct, holds info on what is to be drawn at a location and the color.
  struct RasterInfo
  {
    RasterInfo();
    RasterInfo(const char val, Color col);
    bool operator ==(const RasterInfo &rhs) const;
    bool operator !=(const RasterInfo &rhs) const;
    char Value;
    Color C;
  };

  // Console raster class
  class Canvas;
  class CanvasRaster
  {
    friend Canvas;

  public:
    // Constructors
    CanvasRaster(unsigned int width, unsigned int height);

    // Method Prototypes
    bool WriteChar(char toDraw, float x, float y, Color color = PREVIOUS_COLOR);
	  bool WriteString(const char *toWrite, size_t len, float x, float y, Color color = PREVIOUS_COLOR);
    const Field2D<RasterInfo>& GetRasterData() const;
    void Fill(const RasterInfo &ri);
    void Zero();

    // General
    unsigned int GetRasterWidth() const;
    unsigned int GetRasterHeight() const;

  private:
    // Private member functions
    Field2D<RasterInfo>& GetRasterData();

    // Variables
    unsigned int width_;
    unsigned int height_;
    Field2D<RasterInfo> data_;

  };
}

