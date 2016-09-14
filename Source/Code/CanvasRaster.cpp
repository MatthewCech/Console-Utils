#include "CanvasRaster.hpp"
#include "RFuncs.hpp"




namespace RConsole
{
   /////////////////////////
   // Raster info object //
  ////////////////////////
  // Default constructor, no character and just the previous color.
  RasterInfo::RasterInfo() : Value(0), C(Color::PREVIOUS_COLOR) 
  {  }

  
  // Non-Default constructor, specifies const character and color.
  RasterInfo::RasterInfo(const char val, Color col) : Value(val), C(col) 
  {  }


  // Overloaded comparision operator that checks all fields.
  bool RasterInfo::operator ==(const RasterInfo &rhs) const
  {
    if (rhs.C == C && rhs.Value == Value)
      return true;
    return false;
  }


  // Overloaded comparison operator that checks all fields.
  bool RasterInfo::operator !=(const RasterInfo &rhs) const
  {
    return !(*this == rhs);
  }


    ///////////////////////////
   // Console Raster object //
  ///////////////////////////
  // Default constructor for the CanvasRaster- Zeros data and gets width and height.
  CanvasRaster::CanvasRaster()
    : width_(CONSOLE_WIDTH)
    , height_(CONSOLE_HEIGHT)
    , data_(width_, height_, RASTERINFO_DEFAULT)
  {  }

  
  // Non-default constructor for canvas raster.
  CanvasRaster::CanvasRaster(unsigned int width, unsigned int height, const RasterInfo &r)
    : width_(width)
    , height_(height)
    , data_(width, height, r)
  {  }

  // Forces the canvas raster to re-initalize with the new size.
  void CanvasRaster::ForceResize(unsigned int newWidth, unsigned int newHeight)
  {
    data_ = Field2D<RasterInfo>(width_, height_, RASTERINFO_DEFAULT);
    width_ = newWidth;
    height_ = newHeight;
  }

  // Draws a character to the screen. Returns if it was successful or not.
  bool CanvasRaster::WriteChar(char toDraw, float x, float y, Color color)
  {
    #ifdef RConsole_CLIP_CONSOLE

    if (x > width_) return false;
    if (y > height_) return false;

    #endif // RConsole_CLIP_CONSOLE

    data_.GoTo(static_cast<int>(x), static_cast<int>(y));
    data_.Set(RasterInfo(toDraw, color));
  
    //Everything completed correctly.
    return true;
  }


  // Writes a string to the field
  bool CanvasRaster::WriteString(const char *toWrite, size_t len, float x, float y, Color color)
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


  // Writes a mass of spaces to the screen.
  void CanvasRaster::Fill(const RasterInfo &ri)
  {
    data_.Fill(ri);
  }
  

  // Clears out all of the data written to the raster. Does NOT move cursor to 0,0.
  void CanvasRaster::Zero()
  {
    data_.Zero();
  }


  // Get a constant reference to the existing raster.
  const Field2D<RasterInfo>& CanvasRaster::GetRasterData() const
  {
    return data_;
  }


  // Underlying raster exposing.
  Field2D<RasterInfo>& CanvasRaster::GetRasterData()
  {
    return data_;
  }
  

  // Gets console width.
  unsigned int CanvasRaster::GetConsoleWidth() const 
  { 
    return rlutil::tcols(); 
  }  


  // Get console height
  unsigned int CanvasRaster::GetConsoleHeight() const
  { 
    return rlutil::trows(); 
  } 
}
