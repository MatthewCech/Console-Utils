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
  Canvas canvas(10, 10, 3, 5);
  Canvas canvas2(10, 10, 20, 5);
  Canvas::SetCursorVisible(false);
  srand(0);
  //Canvas::ReInit(20, 10, 10, 10);


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
        canvas.Draw('a', static_cast<float>(i), static_cast<float>(j), static_cast<RConsole::Color>(rnd));
        canvas2.Draw('a', static_cast<float>(i), static_cast<float>(j), static_cast<RConsole::Color>(WHITE));
      }

    // Flip if we get too far to the side.
    if (flip)
      Xstart--;
    else
      Xstart++;

    // Establish going back the other direction.
    if (Xstart >= 45 || Xstart <= 0)
      flip = !flip;

    // Clear
    canvas.Update();
    canvas2.Update();
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
    //  rlutil::locate(rlutil::tcols() - 3, 1);
    //  rlutil::setColor(rlutil::MAGENTA);
    //  printf("%3i", RTest::Timekeeper::GetLastTimeMS());
    //  rlutil::locate(rlutil::tcols() - 3, 2);
    //  printf("%3i", RTest::Timekeeper::GetAvgTimeMS());
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
