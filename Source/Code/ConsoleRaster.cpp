#include "ConsoleRaster.hpp"
#include "RFuncs.hpp"



namespace RConsole
{
  //Default constructor for the ConsoleRaster- Zeros data and gets width and height.
  ConsoleRaster::ConsoleRaster() 
    : width_(rlutil::tcols())
    , height_(rlutil::trows())
    , data_(width_, height_)
  {  }

  //Draws a character to the screen. Returns if it was successful or not.
  bool ConsoleRaster::WriteChar(char toDraw, float x, float y, Color color)
  {
    data_.GoTo(static_cast<int>(x), static_cast<int>(y));
    data_.Set(RasterInfo(toDraw, color));
  
    //Everything completed correctly.
    return true;
  }

  //Writes a string to the field
  bool ConsoleRaster::WriteString(const char *toWrite, size_t len, float x, float y, Color color)
  {
	  //Establish and check for a string of a usable size.
	  data_.GoTo(static_cast<int>(x), static_cast<int>(y));
	  for (unsigned int i = 0; i < len; ++i)
	  {
		  data_.Set(RasterInfo(toWrite[i], color));
		  data_.IncrementX();
	  }

	  //Return success.
	  return true;
  }

  //Clears out all of the data written to the raster. Does NOT move cursor to 0,0.
  void ConsoleRaster::Clear()
  {
    data_.Zero();
  }
  

  //Get a constant reference to the existing raster.
  const Field2D<RasterInfo>& ConsoleRaster::GetRasterData() const
  {
    return data_;
  }


  //Underlying raster exposing.
  Field2D<RasterInfo>& ConsoleRaster::GetRasterData()
  {
    return data_;
  }
  
  //Gets console width.
  unsigned int ConsoleRaster::GetConsoleWidth() const 
  { 
    return rlutil::tcols(); 
  }  


  //Get console height
  unsigned int ConsoleRaster::GetConsoleHeight() const
  { 
    return rlutil::trows(); 
  } 
}
