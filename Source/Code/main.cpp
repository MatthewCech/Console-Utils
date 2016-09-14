#include <stdio.h>
#include <ostream>
#include "Canvas.hpp"
#include "RTest.hpp"
#include "Field2D.hpp"
#include <rlutil.h>
#include <chrono>
#include <thread>
#include <csignal>
#include "Definitions.hpp"


void TestField2D();


// Awesome.
using namespace RConsole;
int main(int argc, char** argv) try
{
  // Begin with tests.
  TestField2D();

  // Variables
  int Xstart = 0;
  bool flip = false;
  char letter = 'a';
  bool displayFPS = false;
  if (argc == 2)
    if (strcmp(argv[1], "-fps") || strcmp(argv[1], "/fps") || strcmp(argv[1], "fps"))
      displayFPS = true;

  // Setup
  Canvas::SetCursorVisible(false);
  srand(0);



  // Main loop
  while (1)
  {
    ///////////////////////////////////////////////////////////////////////////////////////
    RTest::Timekeeper::StartFrame();
    /////////////////////////////////// [ TIMED BLOCK ] ///////////////////////////////////

    //RConsole::DrawAlphaPoint(prev2X, prev2Y, RConsole::WHITE, .3);
    for (int i = Xstart; i < 30 + Xstart; ++i)
    //for (int i = 0; i < 30; ++i)
      for (int j = 0; j < 20; ++j)
      {
        if (letter > 'z')
          letter = 'a';

        int rnd = rand() % RConsole::PREVIOUS_COLOR;
        //Console::Draw('@', i, j, static_cast<RConsole::Color>(rnd));
        Canvas::Draw('a', i, j, static_cast<RConsole::Color>(rnd));
      }

	  // Console text.
    Canvas::DrawString("\"Do not do a frighten- it is the OK.\" - Smol Snek", 15, 10, RConsole::CYAN);
    Canvas::DrawBox('a', 1, 1, 20, 200, WHITE);
    //Canvas::Draw('a', 20, 200, WHITE);
    Canvas::DumpRaster();
    Canvas::DrawBox('X', 1, 1, 20, 200, WHITE);
    //Canvas::Draw('X', 20, 200, GREEN);
    Canvas::DumpRaster();
    // Flip if we get too far to the side.
    if (flip)
      Xstart--;
    else
      Xstart++;

    // Establish going back the other direction.
    if (Xstart >= 45 || Xstart <= 0)
      flip = !flip;

    // Clear
    Canvas::Update();

    ///////////////////////////////////  [ END BLOCK ]  ///////////////////////////////////
    RTest::Timekeeper::EndFrame();
    ///////////////////////////////////////////////////////////////////////////////////////
    // Print cycle time in MS.
    // If you use CTRL-C, you will get a block of garbage if this is running.
    if (displayFPS)
    {
      rlutil::locate(rlutil::tcols() - 3, 1);
      rlutil::setColor(rlutil::MAGENTA);
      printf("%3i", RTest::Timekeeper::GetLastTimeMS());
      rlutil::locate(rlutil::tcols() - 3, 2);
      printf("%3i", RTest::Timekeeper::GetAvgTimeMS());
    }
  }
  
  // Return success
  return 0;
}
// Catch any exceptions from main.
catch (const RTest::RException &e)
{
  std::cout << e << std::endl;
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
