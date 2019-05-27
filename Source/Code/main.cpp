#include <stdio.h>
#include <ostream>
#include "Canvas.hpp"
#include "Field2D.hpp"
#include <rlutil.h>
#include <chrono>
#include <thread>
#include <csignal>
#include "Definitions.hpp"
#include <string>



// Defines for asserts doing floating point math
#define RTest_NEAR_FLOAT  .0006
#define RTest_NEAR_DOUBLE .00006

// Turns on or off an assert. If there is an assert that fails, a custom exception type is thrown. 
// Place the following line in your project or compile with the following defined: #define RTest_ASSERT_ON
#if defined (DEBUG) | (_DEBUG)
#define RTest_ASSERT_Active 1
#else
#define RTest_ASSERT_Active 0
#endif

#define RTest_ASSERT(a) do{ if(RTest_ASSERT_Active){if(!(a)) { throw(RTest::RException("Assert Failed!")); }} } while (0)                                                  



namespace RTest
{
  // A class for timing frames.
  class Timekeeper
  {
  public:
    // Static member functions
    static void StartFrame();
    static void EndFrame();
    static int GetLastTimeMS();
    static int GetAvgTimeMS();
    static void SetMaxSamples(unsigned int samples);

  private:
    // Frame time markers
    static std::chrono::milliseconds startTime_;
    static std::chrono::milliseconds endTime_;

    // Other data for averaging
    static double frameSamples_;
    static double frameAvg_;
    static long long lastTime_;
    static unsigned int maxFrameSamples_;
  };


    //////////////////////
   // Timekeeper Class //
  //////////////////////
  // Static time initialization
  std::chrono::milliseconds Timekeeper::startTime_ = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
  std::chrono::milliseconds Timekeeper::endTime_ = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
  double Timekeeper::frameSamples_ = 0;
  double Timekeeper::frameAvg_ = 0;
  long long Timekeeper::lastTime_ = 0;
  unsigned int Timekeeper::maxFrameSamples_ = 50;


  // Start frame marker. Should be called at the start of a single cycle of the program.
  void Timekeeper::StartFrame()
  {
    startTime_ = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::high_resolution_clock::now().time_since_epoch());
  }


  // End frame marker. Should be called at the end of a single cycle of the program.
  void Timekeeper::EndFrame()
  {
    endTime_ = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::high_resolution_clock::now().time_since_epoch());

    lastTime_ = (endTime_ - startTime_).count();
    frameAvg_ = (frameSamples_ * frameAvg_ + lastTime_) / static_cast<double>(frameSamples_ + 1);
    if (frameSamples_ < maxFrameSamples_)
      frameSamples_ += 1;
  }


  // Get the last frame time in MS as an int.
  int Timekeeper::GetLastTimeMS()
  {
    return static_cast<int>(lastTime_);
  }


  // Returns the average frame time that has been calculated.
  int Timekeeper::GetAvgTimeMS()
  {
    return static_cast<int>(frameAvg_);
  }


  // Sets the total number of previous frames to take into account when averaging.
  void Timekeeper::SetMaxSamples(unsigned int samples)
  {
    maxFrameSamples_ = samples;
    frameSamples_ = 0;
  }
}


// Awesome.
int main(int, char**)
{
  // Variables
  char letter = 'a';

  // Setup
  RConsole::Canvas canvas(50, 20, 3, 5);
  RConsole::Canvas::SetCursorVisible(false);
  srand(0);

  // Main loop
  int cycles = 50;
  while (--cycles > 0)
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
    rlutil::locate(rlutil::tcols() - 3, 2);
    printf("c%2i", cycles);
  }
  
  // Return success
  return 0;
}
