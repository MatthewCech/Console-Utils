#pragma once
#include <rlutil.h>

// For strict unused variable warnings.
#define UNUSED(x) (void)(x)

/////// OS DEFINES /////////
// Windows
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define OS_WINDOWS
#endif

// MacOSX
#if defined(__APPLE__)
#define OS_MACOSX
#endif

// Linux
#if defined(__linux__)
#define OS_LINUX
#endif

// Define POSIX
#if defined(OS_MACOSX) || defined(OS_LINUX)
#define OS_POSIX
#endif

/////// COMPILER DEFINES /////////
#if defined(_MSC_VER)
#define COMPILER_VS
#endif

/////// CONSOLE SETTINGS /////////
#define RConsole_NO_THREADING // Define we aren't threading- printing becomes unsafe, but faster.

namespace RConsole
{
  // Forward declare Field2D for use later.
  template <typename T>
  class Field2D;

  // A proxy class for the [] operator, allowing you to use the [] operator
  template <typename T>
  class Field2DProxy
  {
    // Mark the Field2D as my friend!
    friend Field2D<T>;

  public:
    // Operator Overload
    T &operator[](unsigned int y);

  private:
    // Private constructor, friends only!
    Field2DProxy(Field2D<T> *parentField, unsigned int xPos);

    // Variables
    Field2D<T> *field_;
    const int x_;
  };


  // A 2D way to represent a 1D line of continuous memory.
  // The 2D Field keeps track of the current index you are at in memory,
  // allowing cheap reading if you have the spot selected, with a single add.
  // Note that this is not guarded- if you reach the "end" of the width, it will
  // let you freely step onto the next row of the 2D array you have set up.
  template <typename T>
  class Field2D
  {
    // Friensd can see ALL!
    friend Field2DProxy<T>;

  public:
    // Constructor
    Field2D(unsigned int w, unsigned int h);
    Field2D(unsigned int w, unsigned int h, const T defaultVal);
    Field2D &operator=(const Field2D &rhs);
    Field2D(const Field2D &rhs);
    ~Field2D();

    // Structure Info
    unsigned int Width() const;
    unsigned int Height() const;
    unsigned int Length() const;

    // Member Functions - Complex Manipulation
    void Zero();
    void Set(const T &newItem);
    T &Get(unsigned int x, unsigned int y);
    Field2DProxy<T> operator[](unsigned int xPos);
    void GoTo(unsigned int x, unsigned int y);
    const T& Get(unsigned int x, unsigned int y) const;
    const T& Peek(unsigned int x, unsigned int y) const;
    const T& Peek(unsigned int index) const;
    void Fill(const T &objToUse);
    void Fill(const T &objToUse, unsigned int startIndex, unsigned int endIndex);

    // Basic Manipulation
    T &Get();
    T* GetHead() { return data_; }
    const T &Get() const;
    void IncrementX();
    void IncrementY();
    void DecrementX();
    void DecrementY();
    unsigned int GetIndex();
    void SetIndex(unsigned int index);

  private:
    // Variables
    unsigned int index_;
    unsigned int width_;
    unsigned int height_;
    T *data_;
  };
}


// Template implementations: Place in separate file:
namespace RConsole
{
  //////////////////////////////////
 // Field2DProxy Methods and Co. //
//////////////////////////////////
  template <typename T>
  Field2DProxy<T>::Field2DProxy(Field2D<T> *parentField, unsigned int xPos)
    : field_(parentField)
    , x_(xPos)
  {  }

  // [] Operator Overload.
  // Note- This sets the current index!
  template <typename T>
  T &Field2DProxy<T>::operator[](unsigned int y)
  {
    field_->GoTo(x_, y);
    return field_->Get();
  }

  ////////////////////
 // Structure Info //
////////////////////
// Gets the width of the Field2D
  template <typename T>
  unsigned int Field2D<T>::Width() const
  {
    return width_;
  }

  // Gets the height of the Field2D
  template <typename T>
  unsigned int Field2D<T>::Height() const
  {
    return height_;
  }

  template <typename T>
  unsigned int Field2D<T>::Length() const
  {
    return width_ * height_;
  }


  /////////////////////////////
 // Field2D Methods and Co. //
/////////////////////////////
// Constructor
// Defaults by setting everything to 0.
  template <typename T>
  Field2D<T>::Field2D(unsigned int w, unsigned int h)
    : index_(0)
    , width_(w)
    , height_(h)
  {
    data_ = new T[w * h];
    Zero();
  };

  // Sets all values to given default.
  template <typename T>
  Field2D<T>::Field2D(unsigned int w, unsigned int h, const T defaultVal)
    : index_(0)
    , width_(w)
    , height_(h)
    , data_(nullptr)
  {
    T* adsf = new T[w * h];
    data_ = adsf;
    for (unsigned int i = 0; i < w; ++i)
      for (unsigned int j = 0; j < h; ++j)
      {
        Set(defaultVal);
        IncrementX();
      }
    index_ = 0;
  }

  template <typename T>
  Field2D<T>::Field2D(const Field2D<T> &rhs)
  {
    if (data_)
      delete[] data_;

    data_ = new T[rhs.width_ * rhs.height_];
    width_ = rhs.width_;
    height_ = rhs.height_;
    index_ = rhs.index_;

    for (unsigned int i = 0; i < width_ * height_; ++i)
      data_[i] = rhs.data_[i];
  }

  // Assignment operator
  template <typename T>
  Field2D<T> & Field2D<T>::operator=(const Field2D<T> &rhs)
  {
    if (&rhs != this)
    {
      if (data_)
        delete[] data_;

      data_ = new T[rhs.width_ * rhs.height_];
      width_ = rhs.width_;
      height_ = rhs.height_;
      index_ = rhs.index_;

      for (unsigned int i = 0; i < width_ * height_; ++i)
        data_[i] = rhs.data_[i];
    }
    return *this;
  }

  // Destructor
  template <typename T>
  Field2D<T>::~Field2D()
  {
    delete[] data_;
  }

  ////////////////////////
 // Complex Operations //
////////////////////////
// Get the item at the position X, Y.
// Does not set the actual index of the Field!
  template <typename T>
  T &Field2D<T>::Get(unsigned int x, unsigned int y)
  {
    GoTo(x, y);
    return Get();
  }

  // Const version of get that returns const reference.
  template <typename T>
  const T &Field2D<T>::Get(unsigned int x, unsigned int y) const
  {
    GoTo(x, y);
    return Get();
  }

  // Get the first part of a 2D array operator
  template <typename T>
  Field2DProxy<T> Field2D<T>::operator[](unsigned int xPos)
  {
    return Field2DProxy<T>(this, xPos);
  }

  // Set the value at the current index
  template <typename T>
  void Field2D<T>::Set(const T &newItem)
  {
    data_[index_] = newItem;
  }

  // Glance at a read-only version of a specified location. Does NOT set index.
  template <typename T>
  const T& Field2D<T>::Peek(unsigned int x, unsigned int y) const
  {
    return data_[x + y * width_];
  }

  // Get the value at the specified index.
  template <typename T>
  const T& Field2D<T>::Peek(unsigned int index) const
  {
    return data_[index];
  }

  // Chance selected index to specified point.
  template <typename T>
  void Field2D<T>::GoTo(unsigned int x, unsigned int y)
  {
    index_ = x + y * width_;
  }

  // Sets all memory to 0. Does NOT modify index!
  template <typename T>
  void Field2D<T>::Zero()
  {
    memset(data_, 0, sizeof(T) * width_ * height_);
  }

  // Sets all memory to whatever you want.
  template <typename T>
  void Field2D<T>::Fill(const T &objToUse)
  {
    Fill(objToUse, 0, Length());
  }

  // Fills a specific range to whatever I want, inclusive for start index and
  // excludes end index.
  template <typename T>
  void Field2D<T>::Fill(const T &objToUse, unsigned int startIndex, unsigned int endIndex)
  {
    unsigned int prevIndex = index_;
    index_ = startIndex;
    for (unsigned int i = startIndex; i < endIndex; ++i)
    {
      Set(objToUse);
      IncrementX();
    }
    index_ = prevIndex;
  }

  //////////////////////
 // Cheap operations //
//////////////////////
// Get the value at the current index.
  template <typename T>
  T &Field2D<T>::Get()
  {
    return data_[index_];
  }

  // Const get.
  template <typename T>
  const T& Field2D<T>::Get() const
  {
    return data_[index_];
  }

  // Increment X location by 1 in the 2D field
  template <typename T>
  void Field2D<T>::IncrementX()
  {
    ++index_;
  }

  // Increment Y location by 1 in the 2D field
  template <typename T>
  void Field2D<T>::IncrementY()
  {
    index_ += width_;
  }

  // Decrement X location by 1 in the 2D field
  template <typename T>
  void Field2D<T>::DecrementX()
  {
    --index_;
  }

  // Decrement Y location by 1 in the 2D Field
  template <typename T>
  void Field2D<T>::DecrementY()
  {
    index_ -= width_;
  }

  // Gets the index that the 2D Field currently has.
  template <typename T>
  unsigned int Field2D<T>::GetIndex()
  {
    return index_;
  }

  // Gets the index that the 2D Field currently has.
  template <typename T>
  void Field2D<T>::SetIndex(unsigned int index)
  {
    index_ = index;
  }
}


namespace RConsole
{
#define DEFAULT_WIDTH (rlutil::tcols() - 1)
#define DEFAULT_HEIGHT rlutil::trows()

  //Colors!
  enum Color
  {
    //Acquire rlutil info where possible--
    BLACK = rlutil::BLACK,
    BLUE = rlutil::BLUE,
    GREEN = rlutil::GREEN,
    CYAN = rlutil::CYAN,
    RED = rlutil::RED,
    MAGENTA = rlutil::MAGENTA,
    BROWN = rlutil::BROWN,
    GREY = rlutil::GREY,
    DARKGREY = rlutil::DARKGREY,
    LIGHTBLUE = rlutil::LIGHTBLUE,
    LIGHTGREEN = rlutil::LIGHTGREEN,
    LIGHTCYAN = rlutil::LIGHTCYAN,
    LIGHTRED = rlutil::LIGHTRED,
    LIGHTMAGENTA = rlutil::LIGHTMAGENTA,
    YELLOW = rlutil::YELLOW,
    WHITE = rlutil::WHITE,

    // Add custom values
    //DEFAULT = rlutil::DEFAULT, // BROKEN //Added custom to the rlutil header.
    PREVIOUS_COLOR
  };

  // The raster info struct, holds info on what is to be drawn at a location and the color.
  struct RasterInfo
  {
    RasterInfo();
    RasterInfo(const char val, Color col);
    bool operator ==(const RasterInfo &rhs) const;
    bool operator !=(const RasterInfo &rhs) const;
    char Value;
    Color C;
  };

  // Console raster class
  class Canvas;
  class CanvasRaster
  {
    friend Canvas;

  public:
    // Constructors
    CanvasRaster(unsigned int width, unsigned int height);

    // Method Prototypes
    bool WriteChar(char toDraw, unsigned int x, unsigned int y, Color color = PREVIOUS_COLOR);
    bool WriteString(const char *toWrite, size_t len, unsigned int x, unsigned int y, Color color = PREVIOUS_COLOR);
    const Field2D<RasterInfo>& GetRasterData() const;
    void Fill(const RasterInfo &ri);
    void Zero();

    // General
    unsigned int GetRasterWidth() const;
    unsigned int GetRasterHeight() const;

  private:
    // Private member functions
    Field2D<RasterInfo>& GetRasterData();

    // Variables
    unsigned int width_;
    unsigned int height_;
    Field2D<RasterInfo> data_;

  };

  class Canvas
  {
  public:
    // Constructor
    Canvas(unsigned int width = DEFAULT_WIDTH, unsigned int height = DEFAULT_HEIGHT, int xOffset = 0, int yOffset = 0);

    // Init call
    void ReInit(unsigned int width, unsigned int height, int xOffset = 0, int yOffset = 0);

    // Basic drawing calls
    bool Update();
    void FillCanvas(const RasterInfo &ri = RasterInfo(' ', WHITE));
    void Draw(char toWrite, int x, int y, Color color = PREVIOUS_COLOR);
    void Draw(char toWrite, float x, float y, Color color = PREVIOUS_COLOR);
    void DrawString(const char* toDraw, int xStart, int yStart, Color color);
	  void DrawString(const char* toDraw, float xStart, float yStart, Color color = PREVIOUS_COLOR);
    void DrawAlpha(int x, int y, Color color, float opacity);
    void DrawAlpha(float x, float y, Color color, float opacity);
    void Shutdown();

    // Advanced drawing calls
    void DrawPartialPoint(float x, float y, Color color);
    void DrawBox(char toWrite, float x1, float y1, float x2, float y2, Color color);
    void DumpRaster(FILE *fp = stdout);
    void CropRaster(FILE *fp = stdout, char toTrim = ' ');

    // Data related calls
    unsigned int GetConsoleWidht();
    unsigned int GetConsoleHeight();
    unsigned long GetMemID();

    // Global Settings
    static void SetCursorVisible(bool isVisible);
  private:
    // Hidden Constructors- no instantiating publicly!
    //Canvas(const Canvas &rhs);
    
    // Private methods.
    void clearPrevious();
    void setColor(const Color &color);
    bool writeRaster(CanvasRaster &r);
    int  abs(int x);
    int  putC(int character, FILE * stream );
    // Absolute value of int.

    // static information
    static void fullClear();

    // Any rasters we have. Could be expanded to have two, so you could "swap" them,
    // Although practicality of that is limited given the clearing technique.
    CanvasRaster r_;
    CanvasRaster prev_;

    // The tabs on what was last modified. This is important, because we will only update
    // what we care about.
    bool isDrawing_;
    unsigned int width_;
    unsigned int height_;
    unsigned long memoryId_;
    int xOffset_;
    int yOffset_;
    Field2D<bool> modified_;
  };
}
