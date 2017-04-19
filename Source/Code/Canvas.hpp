#pragma once
#include "CanvasRaster.hpp"
#include "Field2D.hpp"

namespace RConsole
{
  #define DEFAULT_WIDTH (rlutil::tcols() - 1)
  #define DEFAULT_HEIGHT rlutil::trows()
}

namespace RConsole
{
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
    void Draw(char toWrite, float x, float y, Color color = PREVIOUS_COLOR);
	  void DrawString(const char* toDraw, float xStart, float yStart, Color color = PREVIOUS_COLOR);
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
    //Canvas(const Canvas &rhs) { *this = rhs; }
    
    // Private methods.
    void clearPrevious();
    void setColor(const Color &color);
    bool writeRaster(CanvasRaster &r);
    int  putC(int character, FILE * stream );

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
