#pragma once
#include <string>
#include <ostream>


#define RTest_NEAR_FLOAT  .0006
#define RTest_NEAR_DOUBLE .00006
#define RTest_ASSERT_ON  //Defined Asserts
#ifdef RTest_ASSERT_ON
  #define RTest_ASSERT(a) do{                                  \
      if(!(a)) { throw(RTest::RException("Assert Failed!")); } \
  } while (0)                                                  \
#endif

namespace RTest
{
  //this is rh
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

  //Define print formatting 
  std::ostream &operator<<(std::ostream &os, const RException &rhs)
  {
    os << "!! EXCEPTION: " << rhs.File << " line " << rhs.Line << ": " << rhs.Message;
    return os;
  }
  #define RException(a) RException(__FILE__, __LINE__, a)

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
}
