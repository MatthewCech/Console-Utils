//#include "ConsoleLite.hpp"
#include <stdio.h>
#include <ostream>
#include "Canvas.hpp"
#include "RTest.hpp"
#include "Field2D.hpp"
#include <chrono>
#include <thread>
#include <csignal>
#include "Definitions.hpp"

//#pragma once
#ifndef ConsoleLiteH
#define ConsoleLiteH

#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <sstream>

#ifdef _WIN32
#include <windows.h>  // for WinAPI and Sleep()
#endif


namespace ConsoleLite
{
  // Typedefs
  typedef char PixelValue;
  typedef std::string PixelSet;
  typedef unsigned int RasterSize;


  // Returns Get the number of columns(width) or -1 on error.
  inline int ConsoleWidth(void)
  {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
      return -1;
    else
      return csbi.srWindow.Right - csbi.srWindow.Left + 1; // Window width
#else
#ifdef TIOCGSIZE
    struct ttysize ts;
    ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
    return ts.ts_cols;
#elif defined(TIOCGWINSZ)
    struct winsize ts;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
    return ts.ws_col;
#else
    return -1;
#endif
#endif
  }


  // Returns the number of rows (height) or -1 on error.
  inline int ConsoleHeight(void)
  {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
      return -1;
    else
      return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
#ifdef TIOCGSIZE
    struct ttysize ts;
    ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
    return ts.ts_lines;
#elif defined(TIOCGWINSZ)
    struct winsize ts;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
    return ts.ws_row;
#else
    return -1;
#endif
#endif
  }


  // Color Enum, ordered for windows.
  enum Color
  {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    GREY,
    DARKGREY,
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE
  };


  // Namespace global constants -- ansi color codes
  const std::string ANSI_CODE_BLACK = "\033[22;30m";
  const std::string ANSI_CODE_BROWN = "\033[22;33m";
  const std::string ANSI_CODE_DARKGREY = "\033[01;30m";
  const std::string ANSI_CODE_GREY = "\033[22;37m";
  const std::string ANSI_CODE_WHITE = "\033[01;37m";
  const std::string ANSI_CODE_RED = "\033[22;31m";
  const std::string ANSI_CODE_YELLOW = "\033[01;33m";
  const std::string ANSI_CODE_GREEN = "\033[22;32m";
  const std::string ANSI_CODE_CYAN = "\033[22;36m";
  const std::string ANSI_CODE_BLUE = "\033[22;34m";
  const std::string ANSI_CODE_MAGENTA = "\033[22;35m";
  const std::string ANSI_CODE_LIGHTRED = "\033[01;31m";
  const std::string ANSI_CODE_LIGHTGREEN = "\033[01;32m";
  const std::string ANSI_CODE_LIGHTCYAN = "\033[01;36m";
  const std::string ANSI_CODE_LIGHTBLUE = "\033[01;34m";
  const std::string ANSI_CODE_LIGHTMAGENTA = "\033[01;35m";


  // Namespace global constants -- windows color codes 
  const std::string WIN_CODE_BLACK = "\x1B[30m";
  const std::string WIN_CODE_RED = "\x1B[31m";
  const std::string WIN_CODE_GREEN = "\x1B[32m";
  const std::string WIN_CODE_YELLOW = "\x1B[33m";
  const std::string WIN_CODE_BLUE = "\x1B[34m";
  const std::string WIN_CODE_MAGENTA = "\x1B[35m";
  const std::string WIN_CODE_CYAN = "\x1B[36m";
  const std::string WIN_CODE_WHITE = "\x1B[37m";
  const std::string WIN_CODE_BRIGHT_BLACK = "\x1B[90m";
  const std::string WIN_CODE_BRIGHT_RED = "\x1B[91m";
  const std::string WIN_CODE_BRIGHT_GREEN = "\x1B[92m";
  const std::string WIN_CODE_BRIGHT_YELLOW = "\x1B[93m";
  const std::string WIN_CODE_BRIGHT_BLUE = "\x1B[94m";
  const std::string WIN_CODE_BRIGHT_MAGENTA = "\x1B[95m";
  const std::string WIN_CODE_BRIGHT_CYAN = "\x1B[96m";
  const std::string WIN_CODE_BRIGHT_WHITE = "\x1B[97m";
  const std::string WIN_CODE_BG_BLACK = "\033[0m";



  // The data structure that holds the current state of the canvas
  class SafeRaster
  {
  public:
    // Data for an individual "pixel" (character) on the canvas
    struct RasterData
    {
      // Variables
      PixelValue data;
      Color color;
    };

    // Constructor
    SafeRaster(const RasterSize w, const RasterSize h)
      : data_(w * h, { static_cast<PixelValue>(' '), WHITE })
      , width(w)
      , height(h)
    {  }

    // Sets the value at the specified X and Y position. No bounds checking.
    bool Set(int x, int y, PixelValue value, Color color)
    {
      if (x < 0 || y < 0 || x > static_cast<int>(width) || y > static_cast<int>(height))
        return false;

      data_[x + y * width] = { value, color };
      return true;
    }

    // Sets the value at the specified X and Y position. No bounds checking.
    bool Set(int x, int y, PixelSet set, Color color)
    {
      if (set.size() <= 0)
        return false;

      if (x < 0 || y < 0 || x > static_cast<int>(width) || y > static_cast<int>(height))
        return false;

      const RasterSize setStart = x + y * width;
      const size_t setLen = set.size();
      const size_t dataLen = data_.size();

      for (size_t i = 0; i < setLen; ++i)
      {
        const size_t current = setStart + i;

        if (current < dataLen)
          data_[current] = { set[i], color };
        else
          return false;
      }

      return true;
    }

    // Returns the value at the specified X and Y position. No bounds checking.
    RasterData Get(int x, int y)
    {
      return data_[x + y * width];
    }

    // Prints out the raster
    void Render()
    {
      std::stringstream ss;

      for (unsigned int i = 0; i < data_.size(); ++i)
      {
        ss << TranslateColor(data_[i].color);
        ss << data_[i].data;
#if _WIN32
        ss << WIN_CODE_BG_BLACK;
#endif

        if (((i + 1) % (width)) == 0)
          ss << "\n";
      }

      printf(ss.str().c_str());
    }

    // Variables
    const RasterSize width;
    const RasterSize height;

  private:
    // Variables
    std::vector<RasterData> data_;

    // Translates an enum value to the string it's represented as.
    static std::string TranslateColor(Color color)
    {
      switch (color)
      {
#ifdef _WIN32
      case BLACK: return WIN_CODE_BLACK; break;
      case BROWN: return WIN_CODE_BRIGHT_BLACK; break;
      case DARKGREY: return WIN_CODE_BRIGHT_BLACK; break;
      case GREY: return WIN_CODE_WHITE; break;
      case WHITE: return WIN_CODE_BRIGHT_WHITE; break;
      case RED: return WIN_CODE_RED; break;
      case YELLOW: return WIN_CODE_RED; break;
      case GREEN: return WIN_CODE_GREEN; break;
      case CYAN: return WIN_CODE_CYAN; break;
      case BLUE: return WIN_CODE_BLUE; break;
      case MAGENTA: return WIN_CODE_MAGENTA; break;
      case LIGHTRED: return WIN_CODE_BRIGHT_RED; break;
      case LIGHTGREEN: return WIN_CODE_BRIGHT_GREEN; break;
      case LIGHTCYAN: return WIN_CODE_BRIGHT_CYAN; break;
      case LIGHTBLUE: return WIN_CODE_BRIGHT_BLUE; break;
      case LIGHTMAGENTA: return WIN_CODE_BRIGHT_MAGENTA; break;
#else
      case BLACK: return ANSI_CODE_BLACK; break;
      case BROWN: return ANSI_CODE_BROWN; break;
      case DARKGREY: return ANSI_CODE_DARKGREY; break;
      case GREY: return ANSI_CODE_GREY; break;
      case WHITE: return ANSI_CODE_WHITE; break;
      case RED: return ANSI_CODE_RED; break;
      case YELLOW: return ANSI_CODE_YELLOW; break;
      case GREEN: return ANSI_CODE_GREEN; break;
      case CYAN: return ANSI_CODE_CYAN; break;
      case BLUE: return ANSI_CODE_BLUE; break;
      case MAGENTA: return ANSI_CODE_MAGENTA; break;
      case LIGHTRED: return ANSI_CODE_LIGHTRED; break;
      case LIGHTGREEN: return ANSI_CODE_LIGHTGREEN; break;
      case LIGHTCYAN: return ANSI_CODE_LIGHTCYAN; break;
      case LIGHTBLUE: return ANSI_CODE_LIGHTBLUE; break;
      case LIGHTMAGENTA: return ANSI_CODE_LIGHTMAGENTA; break;
#endif
      }

      return "";
    }
  };


  // A class for creating and drawing things to the screen
  class Canvas
  {
  public:
    // Constructor
    Canvas::Canvas(const RasterSize w, const RasterSize h)
      : width(w)
      , height(h)
      , raster(width, height)
    {  }

    // Default constructor
    Canvas::Canvas()
      : width(ConsoleWidth())
      , height(ConsoleHeight())
      , raster(width, height)
    {  }

    // Writes everything to the screen with the current contents
    void Canvas::Update()
    {
      //printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nn\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
#if _WIN32
      //printf("\x1B[2J\033[0m");
      system("echo.");
#else
      printf("\033[2J\033[H");
#endif   
      PositionCursor(0, 0);
      raster.Render();
      //std::cout << std::fflush;
    }

    // Alternate arguments cast to two ints for Draw(int, int, const RasterData) where RasterData's color is white by default.
    bool Draw(int xPos, int yPos, char data, const Color color = WHITE)
    {
      PixelValue converted = static_cast<PixelValue>(data);
      return raster.Set(xPos, yPos, converted, color);
    }

    // Draws a string to the canvas
    bool Draw(int xPos, int yPos, std::string data, const Color color = WHITE)
    {
      PixelSet converted = static_cast<PixelSet>(data);
      return raster.Set(xPos, yPos, converted, color);
    }

    // Size
    const RasterSize width;
    const RasterSize height;

  private:
    // Variables
    SafeRaster raster;

    // Sets the cursor's position to 0-based x,y, where 0,0 is the upper left.
    void PositionCursor(int x, int y)
    {
#ifdef _WIN32
      // Windows uses 0-based coordinates, *nix uses 1-based coordinates
      COORD coord;
      coord.X = (SHORT)x;
      coord.Y = (SHORT)y;
      SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
      std::ostringstream oss;
      oss << "\033[" << (y + 1) << ";" << (x + 1) << "H";
      std::cout << (oss.str());
#endif // _WIN32
    }

    /// Function: setColor
    /// Change color specified by number (Windows / QBasic colors).
    inline void SetColor(int c)
    {
#ifdef _WIN32
      HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
      SetConsoleTextAttribute(hConsole, (WORD)c);
#else
      RLUTIL_PRINT(getANSIColor(c));
#endif
    }
  };
}

#endif

#define MAIN_NEW
#ifdef MAIN_NEW

int main(int, char**)
{
  // adsf
  ConsoleLite::Canvas canvas = ConsoleLite::Canvas();
  char letter = 'a';
  srand(0);

  while (1)
  {
    ///////////////////////////////////////////////////////////////////////////////////////
    RTest::Timekeeper::StartFrame();
    /////////////////////////////////// [ TIMED BLOCK ] ///////////////////////////////////


    for (int i = 0; i < 110; ++i)
    {
      for (int j = 0; j < 25; ++j)
      {
        if (++letter > 'z')
          letter = 'a';

        int rnd = rand() % ConsoleLite::LIGHTRED;

        canvas.Draw(i, j, letter, static_cast<ConsoleLite::Color>(rnd));// 'a', static_cast<float>(i), static_cast<float>(j), static_cast<RConsole::Color>(rnd));
      }
    }

    /*
    for (size_t i = 0; i < 110; ++i)
    {
      for (size_t j = 0; j < 30; ++j)
      {
        //printf("\x1B[31mTexting\033[0m\t\t");
        //printf("\x1B[31mTexting\033[0m\t\t");
        canvas.Draw(i, j, "A", ConsoleLite::RED);
      }
    }
    */
    canvas.Update();

    /////////////////////////////////////  [ END BLOCK ]  ///////////////////////////////////
    RTest::Timekeeper::EndFrame();
    /////////////////////////////////////////////////////////////////////////////////////////
    //// Print cycle time in MS.
    //// If you use CTRL-C, you will get a block of garbage if this is running.
    rlutil::setColor(rlutil::MAGENTA);
    rlutil::locate(rlutil::tcols() - 3, 1);
    printf("%3i", RTest::Timekeeper::GetAvgTimeMS());

  }
  return 0;
}

#else MAIN_OLD
#include <stdio.h>
#include <ostream>
#include "Canvas.hpp"
#include "RTest.hpp"
#include "Field2D.hpp"
#include <chrono>
#include <thread>
#include <csignal>
#include "Definitions.hpp"


void TestField2D();
using namespace RConsole;
int main(int, char**) try
{
  // Begin with tests.
  TestField2D();

  // Variables
  //int Xstart = 0;
  //bool flip = false;
  char letter = 'a';
  bool displayFPS = false;
  //if (argc == 2)
    //if (strcmp(argv[1], "-fps") || strcmp(argv[1], "/fps") || strcmp(argv[1], "fps"))
  displayFPS = true;

  // Setup
  Canvas canvas(110, 25, 0, 0);
  //Canvas canvas2(10, 10, 20, 5);
  Canvas::SetCursorVisible(false);
  srand(0);
  //Canvas::ReInit(20, 10, 10, 10);


  // Main loop
  while (1)
  {
    ///////////////////////////////////////////////////////////////////////////////////////
    RTest::Timekeeper::StartFrame();
    /////////////////////////////////// [ TIMED BLOCK ] ///////////////////////////////////

    for (int i = 0; i < 110; ++i)
    {
      for (int j = 0; j < 25; ++j)
      {
        if (++letter > 'z')
          letter = 'a';

        int rnd = rand() % ConsoleLite::LIGHTRED;

        canvas.Draw(letter, i, j, static_cast<RConsole::Color>(rnd));
      }
    }

    /*
    //RConsole::DrawAlphaPoint(prev2X, prev2Y, RConsole::WHITE, .3);
    for (int i = Xstart; i < 30 + Xstart; ++i)
    //for (int i = 0; i < 30; ++i)
      for (int j = 0; j < 20; ++j)
      {
        if (letter > 'z')
          letter = 'a';

        int rnd = rand() % RConsole::PREVIOUS_COLOR;
        //Console::Draw('@', i, j, static_cast<RConsole::Color>(rnd));
        canvas.Draw('a', static_cast<float>(i), static_cast<float>(j), static_cast<RConsole::Color>(rnd));
        //canvas2.Draw('a', static_cast<float>(i), static_cast<float>(j), static_cast<RConsole::Color>(WHITE));
      }
      */
    // Flip if we get too far to the side.
    //if (flip)
    //  Xstart--;
    //else
    //  Xstart++;

    // Establish going back the other direction.
    //if (Xstart >= 45 || Xstart <= 0)
    //  flip = !flip;

    // Clear
    canvas.Update();
    //canvas2.Update();
    /*
      ////////////////////////////////////////////////////
     // Testing ReInit with a bunch of different sizes //
    ////////////////////////////////////////////////////
    Canvas::ReInit(20, 10);
    Canvas::DumpRaster();
    Canvas::DumpRaster();
    Canvas::ReInit(40, 30);
    Canvas::DumpRaster();
    Canvas::ReInit(1000, 200);

      ////////////////////////////////////////////////
     // Testing memory exception for out of bounds //
    ////////////////////////////////////////////////
    // image 1
    //Canvas::FillCanvas();
    Canvas::ReInit(10, 20);
    Canvas::DrawBox('?', 0, 0, 80, 40, WHITE);
    Canvas::DrawBox(' ', 1, 1, 79, 79, WHITE);
    Canvas::Draw('a', 2, 2, RED);
    Canvas::DrawString("\"Do not do a frighten- it is the OK.\" - Smol Snek", 5, 5, CYAN);
    Canvas::DumpRaster();
    std::cout << "Test1!" << '\n';

    Canvas::FillCanvas();
    Canvas::ReInit(40, 30);
    // image 2
    Canvas::DrawBox('?', 0, 0, 80, 40, GREEN);
    Canvas::DrawBox(' ', 1, 1, 79, 79, WHITE);
    Canvas::Draw('b', 10, 10, BLUE);
    Canvas::DrawString("\"ANOTHER THING!\"", 5, 5, CYAN);
    Canvas::DumpRaster();
    


    /*
	  // Console text.
    //Canvas::DrawString("\"Do not do a frighten- it is the OK.\" - Smol Snek", 15, 10, RConsole::CYAN);
    Canvas::DrawBox('a', 0, 0, 20, 20, WHITE);
    Canvas::Draw('a', 20000, 200, WHITE);
    Canvas::DumpRaster();
    Canvas::FillCanvas();
    Canvas::DrawBox('X', 0, 0, 10, 10, GREEN);
    //Canvas::Draw('X', 20, 200, GREEN);
    Canvas::DumpRaster();
      */

    ///////////////////////////////////  [ END BLOCK ]  ///////////////////////////////////
    RTest::Timekeeper::EndFrame();
    ///////////////////////////////////////////////////////////////////////////////////////
    // Print cycle time in MS.
    // If you use CTRL-C, you will get a block of garbage if this is running.
    //if (displayFPS)
    //{
      //rlutil::locate(rlutil::tcols() - 3, 1);
      rlutil::setColor(rlutil::MAGENTA);
      //printf("%3i", RTest::Timekeeper::GetLastTimeMS());
      rlutil::locate(rlutil::tcols() - 3, 1);
      printf("%3i", RTest::Timekeeper::GetAvgTimeMS());
    //}
    //system("pause");

    //break;
  }
  
  // Return success
  return 0;
}
// Catch any exceptions from main.
catch (const RTest::RException &e)
{
  std::cout << e << '\n';
  throw e;
}


void TestField2D()
{
  RConsole::Field2D<int> f(5, 10);

  // Test Set, increment and decrement.
  RTest_ASSERT(f.GetIndex() == 0);
  f.Set(5);
  RTest_ASSERT(f.Get() == 5);
  f.IncrementY();
  f.Set(4);
  RTest_ASSERT(f.GetIndex() == 5);
  RTest_ASSERT(f.Get() == 4);
  f.DecrementY();
  RTest_ASSERT(f.Get() == 5);
  f.IncrementX();
  RTest_ASSERT(f.Get() == 0);
  f.GoTo(1, 1);
  f.Set(6);
  RTest_ASSERT(f.Get() == 6);
  f.DecrementX();
  RTest_ASSERT(f.Get() == 4);

  // 5 0
  // 4 6
  // Test peeking
  RTest_ASSERT(f.Peek(0, 0) == 5);
  RTest_ASSERT(f.Peek(0, 1) == 4);
  RTest_ASSERT(f.Peek(1, 1) == 6);
  RTest_ASSERT(f.Peek(1, 0) == 0);

  // Test subscript operator
  f[2][3] = 20;
  RTest_ASSERT(f.Get() == 20);
  f.IncrementX();
  f.Set(23);
  RTest_ASSERT(f.Get() == 23);
  f[3][3] = 22;
  RTest_ASSERT(f.Get() == 22);
}
#endif