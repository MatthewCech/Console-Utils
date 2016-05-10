#include <stdio.h>
#include <ostream>
#include "Console.hpp"
#include "RTest.hpp"
#include "Field2D.hpp"
#include "External/rlutil.h"
#include <chrono>

void TestField2D();

//Awesome.
using namespace RConsole;
int main(void)
{
  //Framerate for main loop
  double samples = 0;
  double avg = 0;

  try
  {
    TestField2D();
  }
  catch(RTest::RException e)
  {
    std::cout << e << std::endl;
    system("Pause");
  }

  //Setup
  int Xstart = 0;
  bool flip = false;

  Console::SetCursorVisible(false);

  //Main loop
  while (1)
  {
    //Frame start
    std::chrono::milliseconds startTime = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::high_resolution_clock::now().time_since_epoch());



    /////////////////////////////////// [ TIMED BLOCK ] ///////////////////////////////////

    //RConsole::DrawAlphaPoint(prev2X, prev2Y, RConsole::WHITE, .3);
    for (float i = Xstart; i < 30 + Xstart; ++i)
      for (float j = 0; j < 20; ++j)
        Console::DrawAlpha(i, j, RConsole::WHITE, 1 - j / 20.0);

    //Flip if we get too far to the side.
    if (flip)
      Xstart--;
    else
      Xstart++;

    //Establish going back the other direction.
    if (Xstart >= 45 || Xstart <= 0)
      flip = !flip;

    //Console::FullClear();
    Console::Update();

    ///////////////////////////////////  [ END BLOCK ]  ///////////////////////////////////



    //Print cycle time in MS
    /*
    std::chrono::milliseconds endTime = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::high_resolution_clock::now().time_since_epoch());
    rlutil::locate(rlutil::tcols() - 3, 1);
    rlutil::setColor(rlutil::MAGENTA);
    long long current = (endTime - startTime).count();
    printf("%3i", current);
    rlutil::locate(rlutil::tcols() - 3, 2);
    avg = (samples * avg + current) / static_cast<double>(samples + 1);
    if(samples < 60)
      samples += 1;
    printf("%3i", (int)avg);*/
  }

  //Return success
  return 0;
}


void TestField2D()
{
  RConsole::Field2D<int> f(5, 10);

  //Test Set, increment and decrement.
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
  //Test peeking
  RTest_ASSERT(f.Peek(0, 0) == 5);
  RTest_ASSERT(f.Peek(0, 1) == 4);
  RTest_ASSERT(f.Peek(1, 1) == 6);
  RTest_ASSERT(f.Peek(1, 0) == 0);

  //Test subscript operator
  f[2][3] = 20;
  RTest_ASSERT(f.Get() == 20);
  f.IncrementX();
  f.Set(23);
  RTest_ASSERT(f.Get() == 23);
  f[3][3] = 22;
  RTest_ASSERT(f.Get() == 22);
}
