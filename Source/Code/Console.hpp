#pragma once
#include "ConsoleRaster.hpp"
#include "Field2D.hpp"



 #define RConsole_CLIP_CONSOLE //Define we want console clipping
 #define RConsole_NO_THREADING //Define we aren't threading- printf becomes unsafe, but faster.


namespace RConsole
{
  class Console
  {
  public:
    //Basic drawing calls
    static void Clear();
    static bool Update();
    static void Draw(char toWrite, int x, int y, Color color = PREVIOUS_COLOR);
    static void Draw(char toWrite, float x, float y, Color color = PREVIOUS_COLOR);
	static void DrawString(const char* toDraw, float xStart, float yStart, Color color = PREVIOUS_COLOR);
    static void DrawAlpha(int x, int y, Color color, float opacity);
    static void DrawAlpha(float x, float y, Color color, float opacity);

    //Advanced drawing calls
    static void DrawPartialPoint(float x, float y, Color color);
    static void SetCursorVisible(bool isVisible);

  private:
    //Hidden Constructors- no instantiating publicly!
    Console() { };
    Console(const Console &rhs) { *this = rhs; }
    
    //Private methods.
    static void ClearPrevious();
    static void FullClear();
    static void SetColor(const Color &color);
    static bool WriteRaster(ConsoleRaster &r);
    static int  PutC(int character, FILE * stream );

    //Any rasters we have. Could be expanded to have two, so you could "swap" them,
    //Although practicality of that is limited given the clearing technique.
    static ConsoleRaster r_;
    static ConsoleRaster prev_;

    //The tabs on what was last modified. This is important, because we will only update
    //what we care about.
    static unsigned int width_;
    static unsigned int height_;
    static Field2D<bool> modified_;
  };
}
