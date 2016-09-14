#pragma once
#include "CanvasRaster.hpp"
#include "Field2D.hpp"



namespace RConsole
{
  class Canvas
  {
  public:
    // Basic drawing calls
    static bool Update();
    static void FillCanvas(const RasterInfo &ri = RasterInfo(' ', WHITE));
    static void Draw(char toWrite, int x, int y, Color color = PREVIOUS_COLOR);
    static void Draw(char toWrite, float x, float y, Color color = PREVIOUS_COLOR);
	  static void DrawString(const char* toDraw, float xStart, float yStart, Color color = PREVIOUS_COLOR);

    // Advanced drawing calls
    static void DrawPartialPoint(float x, float y, Color color);
    static void DrawBox(char toWrite, float x1, float y1, float x2, float y2, Color color);
    static void SetCursorVisible(bool isVisible);
    static void DumpRaster(FILE * fp = stdout);

    // Canvas Management
    static void Shutdown();
    static void SetSize(int width = rlutil::tcols(), int height = rlutil::trows());

  private:
    // Hidden Constructors- no instantiating publicly!
    Canvas() { };
    Canvas(const Canvas &rhs) { *this = rhs; }

    // Temporarily removed functions
    static void DrawAlpha(int x, int y, Color color, float opacity);
    static void DrawAlpha(float x, float y, Color color, float opacity);

    // Private methods.
    static void clearPrevious();
    static void fullClear();
    static void setColor(const Color &color);
    static bool writeRaster(CanvasRaster &r);
    static int  putC(int character, FILE * stream );
    static void setCloseHandler();

    // Any rasters we have. Could be expanded to have two, so you could "swap" them,
    // Although practicality of that is limited given the clearing technique.
    static CanvasRaster r_;
    static CanvasRaster prev_;

    // The tabs on what was last modified. This is important, because we will only update
    // what we care about.
    static bool hasLazyInit_;
    static bool isDrawing_;
    static unsigned int width_;
    static unsigned int height_;
    static Field2D<bool> modified_;
  };
}
