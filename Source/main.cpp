#include <stdio.h>
#include <ostream>
#include "RConsole.hpp"
#include "RTest.hpp"
#include "Field2D.hpp"


void TestField2D();

//Awesome.
using namespace RConsole;
int main(void)
{
  try
  {
    TestField2D();
  }
  catch(RTest::RException e)
  {
    std::cout << e << std::endl;
    system("Pause");
  }

  //return 0;
  Console::SetCursorVisible(false);
  while (1)
  {
    //RConsole::DrawAlphaPoint(prev2X, prev2Y, RConsole::WHITE, .3);
    for (float i = 0; i < 50; ++i)
      for (float j = 0; j < 20; ++j)
        Console::DrawAlphaPoint(i, j, RConsole::WHITE, 1 - (j/20.0  + i/20.0)/2);

    //Console::FullClear();
    Console::ClearPrevious();
  }

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
