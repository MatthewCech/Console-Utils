#pragma once
#include "Field2D.hpp"
#include "Colors.hpp"

namespace RConsole
{
  #define DEFAULT_WIDTH (rlutil::tcols() - 1)
  #define DEFAULT_HEIGHT rlutil::trows()

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
    bool WriteChar(char toDraw, unsigned int x, unsigned int y, Color color = PREVIOUS_COLOR);
    bool WriteString(const char *toWrite, size_t len, unsigned int x, unsigned int y, Color color = PREVIOUS_COLOR);
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

  class Canvas
  {
  public:
    // Constructor
    Canvas(unsigned int width = DEFAULT_WIDTH, unsigned int height = DEFAULT_HEIGHT, int xOffset = 0, int yOffset = 0);

    // Init call
    void ReInit(unsigned int width, unsigned int height, int xOffset = 0, int yOffset = 0);

    // Basic drawing calls
    bool Update();
    void FillCanvas(const RasterInfo &ri = RasterInfo(' ', WHITE));
    void Draw(char toWrite, int x, int y, Color color = PREVIOUS_COLOR);
    void Draw(char toWrite, float x, float y, Color color = PREVIOUS_COLOR);
    void DrawString(const char* toDraw, int xStart, int yStart, Color color);
	  void DrawString(const char* toDraw, float xStart, float yStart, Color color = PREVIOUS_COLOR);
    void DrawAlpha(int x, int y, Color color, float opacity);
    void DrawAlpha(float x, float y, Color color, float opacity);
    void Shutdown();

    // Advanced drawing calls
    void DrawPartialPoint(float x, float y, Color color);
    void DrawBox(char toWrite, float x1, float y1, float x2, float y2, Color color);
    void DumpRaster(FILE *fp = stdout);
    void CropRaster(FILE *fp = stdout, char toTrim = ' ');

    // Data related calls
    unsigned int GetConsoleWidht();
    unsigned int GetConsoleHeight();
    unsigned long GetMemID();

    // Global Settings
    static void SetCursorVisible(bool isVisible);
  private:
    // Hidden Constructors- no instantiating publicly!
    //Canvas(const Canvas &rhs);
    
    // Private methods.
    void clearPrevious();
    void setColor(const Color &color);
    bool writeRaster(CanvasRaster &r);
    int  abs(int x);
    int  putC(int character, FILE * stream );
    // Absolute value of int.

    // static information
    static void fullClear();

    // Any rasters we have. Could be expanded to have two, so you could "swap" them,
    // Although practicality of that is limited given the clearing technique.
    CanvasRaster r_;
    CanvasRaster prev_;

    // The tabs on what was last modified. This is important, because we will only update
    // what we care about.
    bool isDrawing_;
    unsigned int width_;
    unsigned int height_;
    unsigned long memoryId_;
    int xOffset_;
    int yOffset_;
    Field2D<bool> modified_;
  };
}
