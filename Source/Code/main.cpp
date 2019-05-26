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
int main(int, char**) try
{
  // Begin with tests.
  TestField2D();

  // Variables
  char letter = 'a';

  // Setup
  Canvas canvas(50, 20, 3, 5);
  Canvas::SetCursorVisible(false);
  srand(0);


  // Main loop
  while (1)
  {
    ///////////////////////////////////////////////////////////////////////////////////////
    RTest::Timekeeper::StartFrame();
    /////////////////////////////////// [ TIMED BLOCK ] ///////////////////////////////////

    for (int i = 0; i < 50; ++i)
    {
      for (int j = 0; j < 20; ++j)
      {
        if (++letter > 'z')
          letter = 'a';

        int rnd = rand() % RConsole::PREVIOUS_COLOR;
        canvas.Draw(letter, static_cast<float>(i), static_cast<float>(j), static_cast<RConsole::Color>(rnd));
      }
    }

    canvas.Update();

    ///////////////////////////////////  [ END BLOCK ]  ///////////////////////////////////
    RTest::Timekeeper::EndFrame();
    ///////////////////////////////////////////////////////////////////////////////////////

    rlutil::setColor(rlutil::MAGENTA);
    rlutil::locate(rlutil::tcols() - 3, 1);
    printf("%3i", RTest::Timekeeper::GetAvgTimeMS());
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
