#include "RTest.hpp"
#include <chrono>


namespace RTest
{




  //////////////////////
  //RException
  ////////////////////
  #undef RException
  //Define print formatting 
  std::ostream &operator<<(std::ostream &os, const RException &rhs)
  {
    os << "!! EXCEPTION: " << rhs.File << " line " << rhs.Line << ": " << rhs.Message;
    return os;
  }

  #define RException(a) RException(__FILE__, __LINE__, a)


    /////////////////////////////////
   //Testing functions for asserts//
  /////////////////////////////////
  bool Near(double a, double b)
  {
    if (a + RTest_NEAR_DOUBLE > b && a - RTest_NEAR_DOUBLE < b)
      return true;
    return false;
  }

  bool Near(float a, float b)
  {
    if (a + RTest_NEAR_FLOAT > b && a - RTest_NEAR_FLOAT < b)
      return true;
    return false;
  }


    ////////////////////
   //Timekeeper Class//
  ////////////////////
  //Static time initialization
  std::chrono::milliseconds Timekeeper::startTime_ = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
  std::chrono::milliseconds Timekeeper::endTime_ = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
  double Timekeeper::frameSamples_ = 0;
  double Timekeeper::frameAvg_ = 0;
  long long Timekeeper::lastTime_ = 0;
  unsigned int Timekeeper::maxFrameSamples_ = 50;


  //Start frame marker. Should be called at the start of a single cycle of the program.
  void Timekeeper::StartFrame()
  {
    startTime_ = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::high_resolution_clock::now().time_since_epoch());
  }


  //End frame marker. Should be called at the end of a single cycle of the program.
  void Timekeeper::EndFrame()
  {
    endTime_ = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::high_resolution_clock::now().time_since_epoch());

    lastTime_ = (endTime_ - startTime_).count();
    frameAvg_ = (frameSamples_ * frameAvg_ + lastTime_) / static_cast<double>(frameSamples_ + 1);
    if (frameSamples_ < maxFrameSamples_)
      frameSamples_ += 1;
  }


  //Get the last frame time in MS as an int.
  int Timekeeper::GetLastTimeMS()
  {
    return lastTime_;
  }


  //Returns the average frame time that has been calculated.
  int Timekeeper::GetAvgTimeMS()
  {
    return static_cast<int>(frameAvg_);
  }


  //Sets the total number of previous frames to take into account when averaging.
  void Timekeeper::SetMaxSamples(unsigned int samples)
  {
    maxFrameSamples_ = samples;
    frameSamples_ = 0;
  }
}