#pragma once
#include <string>
#include <ostream>
#include <chrono>


#define RTest_NEAR_FLOAT  .0006
#define RTest_NEAR_DOUBLE .00006
#define RTest_ASSERT_ON  //Defined Asserts
#ifdef RTest_ASSERT_ON                                       
  #define RTest_ASSERT(a) do{ if(!(a)) { throw(RTest::RException("Assert Failed!")); } } while (0)                                                  
#endif

namespace RTest
{
  //Undefine the exception structure first to be safe.
  #undef RException
  struct RException
  {
    //Constructor
    RException(std::string file, unsigned int line, std::string message)
      : File(file)
      , Line(line)
      , Message(message)
    {  }

    friend std::ostream &operator<<(std::ostream &os, const RException &rhs);

    //Data
    std::string File;
    unsigned int Line;
    std::string Message;
  };
  #define RException(a) RException(__FILE__, __LINE__, a)


  //A class for timing frames.
  class Timekeeper
  {
  public:
    //Static member functions
    static void StartFrame();
    static void EndFrame();
    static int GetLastTimeMS();
    static int GetAvgTimeMS();
    static void SetMaxSamples(unsigned int samples);

  private:
    //Frame time markers
    static std::chrono::milliseconds startTime_;
    static std::chrono::milliseconds endTime_;

    //Other data for averaging
    static double frameSamples_;
    static double frameAvg_;
    static long long lastTime_;
    static unsigned int maxFrameSamples_;
  };
}
