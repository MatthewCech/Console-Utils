#pragma once
#ifndef CANVAS_HPP
#define CANVAS_HPP

// Includes
#include <cstdio>           // PutC
#include <iostream>         // ostream access
#include <csignal>          // Signal termination.
#include <chrono>           // Time related info for sleeping.
#include <thread>           // Sleep on exit to allow for update to finish.
#include <string>           // String for parsing, storage, etc
#include <unordered_map>    // Storing Canvases and other data
#include <sstream>          // std::stringstream - string manipulation

#ifdef _WIN32
#include <windows.h>  // for WinAPI and Sleep()
#define _NO_OLDNAMES  // for MinGW compatibility
#endif 

// For strict unused variable warnings.
#define UNUSED(x) (void)(x)

// OS defines
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

// Compiler Defines
#if defined(_MSC_VER)
#define COMPILER_VS
#endif

// Console Settings
#define RConsole_NO_THREADING // Define we aren't threading- printing becomes unsafe, but faster.


// Definitions and tempates, etc
namespace RConsole
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Start namespace _rlutil_internal //////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /**
  * Namespace: _rlutil_internal
  *
  * About: Description
  * This file provides some useful utilities for console mode
  * roguelike game development with C and C++. It is aimed to
  * be cross-platform (at least Windows and Linux).
  *
  *
  * About: Copyright
  * (C) 2010 Tapio Vierros
  *
  * About: Under a "Do whatever TF you want" Licence for this namespace/section
  */

  /// Define: _rlutil_internal_USE_ANSI
  /// Define this to use ANSI escape sequences also on Windows
  /// (defaults to using WinAPI instead).
#if 0
#define _rlutil_internal_USE_ANSI
#endif

/// Namespace forward declarations
  namespace _rlutil_internal {
    void locate(int x, int y);
  }

#ifndef _rlutil_internal_INLINE
#ifdef _MSC_VER
#define _rlutil_internal_INLINE __inline
#else
#define _rlutil_internal_INLINE __inline__
#endif
#endif

  /// Namespace: _rlutil_internal
  /// In C++ all functions except <getch>, <kbhit> and <gotoxy> are arranged
  /// under namespace _rlutil_internal. That is because some platforms have them defined
  /// outside of _rlutil_internal.
  namespace _rlutil_internal {

    /**
    * Defs: Internal typedefs and macros
    * _rlutil_internal_STRING_T - String type depending on which one of C or C++ is used
    * _rlutil_internal_PRINT(str) - Printing macro independent of C/C++
    */

#ifndef _rlutil_internal_STRING_T
    typedef std::string _rlutil_internal_STRING_T;
#endif

    inline void _rlutil_internal_PRINT(_rlutil_internal_STRING_T st) { std::cout << st; }

    /**
    * Enums: Color codes
    *
    * BLACK - Black
    * BLUE - Blue
    * GREEN - Green
    * CYAN - Cyan
    * RED - Red
    * MAGENTA - Magenta / purple
    * BROWN - Brown / dark yellow
    * GREY - Grey / dark white
    * DARKGREY - Dark grey / light black
    * LIGHTBLUE - Light blue
    * LIGHTGREEN - Light green
    * LIGHTCYAN - Light cyan
    * LIGHTRED - Light red
    * LIGHTMAGENTA - Light magenta / light purple
    * YELLOW - Yellow (bright)
    * WHITE - White (bright)
    */
    enum {
      BLACK,
      BLUE,
      GREEN,
      CYAN,
      RED,
      MAGENTA,
      BROWN,
      GREY,
      DARKGREY,
      LIGHTBLUE,
      LIGHTGREEN,
      LIGHTCYAN,
      LIGHTRED,
      LIGHTMAGENTA,
      YELLOW,
      WHITE
    };

    /**
    * Consts: ANSI color strings
    *
    * ANSI_CLS - Clears screen
    * ANSI_BLACK - Black
    * ANSI_RED - Red
    * ANSI_GREEN - Green
    * ANSI_BROWN - Brown / dark yellow
    * ANSI_BLUE - Blue
    * ANSI_MAGENTA - Magenta / purple
    * ANSI_CYAN - Cyan
    * ANSI_GREY - Grey / dark white
    * ANSI_DARKGREY - Dark grey / light black
    * ANSI_LIGHTRED - Light red
    * ANSI_LIGHTGREEN - Light green
    * ANSI_YELLOW - Yellow (bright)
    * ANSI_LIGHTBLUE - Light blue
    * ANSI_LIGHTMAGENTA - Light magenta / light purple
    * ANSI_LIGHTCYAN - Light cyan
    * ANSI_WHITE - White (bright)
    */
    const _rlutil_internal_STRING_T ANSI_CLS = "\033[2J";
    const _rlutil_internal_STRING_T ANSI_BLACK = "\033[22;30m";
    const _rlutil_internal_STRING_T ANSI_RED = "\033[22;31m";
    const _rlutil_internal_STRING_T ANSI_GREEN = "\033[22;32m";
    const _rlutil_internal_STRING_T ANSI_BROWN = "\033[22;33m";
    const _rlutil_internal_STRING_T ANSI_BLUE = "\033[22;34m";
    const _rlutil_internal_STRING_T ANSI_MAGENTA = "\033[22;35m";
    const _rlutil_internal_STRING_T ANSI_CYAN = "\033[22;36m";
    const _rlutil_internal_STRING_T ANSI_GREY = "\033[22;37m";
    const _rlutil_internal_STRING_T ANSI_DARKGREY = "\033[01;30m";
    const _rlutil_internal_STRING_T ANSI_LIGHTRED = "\033[01;31m";
    const _rlutil_internal_STRING_T ANSI_LIGHTGREEN = "\033[01;32m";
    const _rlutil_internal_STRING_T ANSI_YELLOW = "\033[01;33m";
    const _rlutil_internal_STRING_T ANSI_LIGHTBLUE = "\033[01;34m";
    const _rlutil_internal_STRING_T ANSI_LIGHTMAGENTA = "\033[01;35m";
    const _rlutil_internal_STRING_T ANSI_LIGHTCYAN = "\033[01;36m";
    const _rlutil_internal_STRING_T ANSI_WHITE = "\033[01;37m";

    /// Function: getANSIColor
    /// Return ANSI color escape sequence for specified number 0-15.
    ///
    /// See <Color Codes>
    _rlutil_internal_INLINE _rlutil_internal_STRING_T getANSIColor(const int c) {
      switch (c) {
      case 0: return ANSI_BLACK;
      case 1: return ANSI_BLUE; // non-ANSI
      case 2: return ANSI_GREEN;
      case 3: return ANSI_CYAN; // non-ANSI
      case 4: return ANSI_RED; // non-ANSI
      case 5: return ANSI_MAGENTA;
      case 6: return ANSI_BROWN;
      case 7: return ANSI_GREY;
      case 8: return ANSI_DARKGREY;
      case 9: return ANSI_LIGHTBLUE; // non-ANSI
      case 10: return ANSI_LIGHTGREEN;
      case 11: return ANSI_LIGHTCYAN; // non-ANSI;
      case 12: return ANSI_LIGHTRED; // non-ANSI;
      case 13: return ANSI_LIGHTMAGENTA;
      case 14: return ANSI_YELLOW; // non-ANSI
      case 15: return ANSI_WHITE;
      default: return "";
      }
    }

    /// Function: setColor
    /// Change color specified by number (Windows / QBasic colors).
    ///
    /// See <Color Codes>
    _rlutil_internal_INLINE void setColor(int c) {
#if defined(_WIN32) && !defined(_rlutil_internal_USE_ANSI)
      HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
      SetConsoleTextAttribute(hConsole, (WORD)c);
#else
      _rlutil_internal_PRINT(getANSIColor(c));
#endif
    }

    /// Function: cls
    /// Clears screen and moves cursor home.
    _rlutil_internal_INLINE void cls(void) {
#if defined(_WIN32) && !defined(_rlutil_internal_USE_ANSI)
      // TODO: This is cheating...
      system("cls");
#else
      _rlutil_internal_PRINT("\033[2J\033[H");
#endif
    }

    /// Function: locate
    /// Sets the cursor position to 1-based x,y.
    _rlutil_internal_INLINE void locate(int x, int y) {
#if defined(_WIN32) && !defined(_rlutil_internal_USE_ANSI)
      COORD coord;
      coord.X = (SHORT)x - 1;
      coord.Y = (SHORT)y - 1; // Windows uses 0-based coordinates
      SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else // _WIN32 || USE_ANSI
      std::ostringstream oss;
      oss << "\033[" << y << ";" << x << "H";
      _rlutil_internal_PRINT(oss.str());
#endif // _WIN32 || USE_ANSI
    }

    /// Function: hidecursor
    /// Hides the cursor.
    _rlutil_internal_INLINE void hidecursor(void) {
#if defined(_WIN32) && !defined(_rlutil_internal_USE_ANSI)
      HANDLE hConsoleOutput;
      CONSOLE_CURSOR_INFO structCursorInfo;
      hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
      GetConsoleCursorInfo(hConsoleOutput, &structCursorInfo); // Get current cursor size
      structCursorInfo.bVisible = FALSE;
      SetConsoleCursorInfo(hConsoleOutput, &structCursorInfo);
#else // _WIN32 || USE_ANSI
      _rlutil_internal_PRINT("\033[?25l");
#endif // _WIN32 || USE_ANSI
    }

    /// Function: showcursor
    /// Shows the cursor.
    _rlutil_internal_INLINE void showcursor(void) {
#if defined(_WIN32) && !defined(_rlutil_internal_USE_ANSI)
      HANDLE hConsoleOutput;
      CONSOLE_CURSOR_INFO structCursorInfo;
      hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
      GetConsoleCursorInfo(hConsoleOutput, &structCursorInfo); // Get current cursor size
      structCursorInfo.bVisible = TRUE;
      SetConsoleCursorInfo(hConsoleOutput, &structCursorInfo);
#else // _WIN32 || USE_ANSI
      _rlutil_internal_PRINT("\033[?25h");
#endif // _WIN32 || USE_ANSI
    }

    /// Function: trows
    /// Get the number of rows in the terminal window or -1 on error.
    _rlutil_internal_INLINE int trows(void) {
#ifdef _WIN32
      CONSOLE_SCREEN_BUFFER_INFO csbi;
      if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return -1;
      else
        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1; // Window height
                                                             // return csbi.dwSize.Y; // Buffer height
#else
#ifdef TIOCGSIZE
      struct ttysize ts;
      ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
      return ts.ts_lines;
#elif defined(TIOCGWINSZ)
      struct winsize ts;
      ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
      return ts.ws_row;
#else // TIOCGSIZE
      return -1;
#endif // TIOCGSIZE
#endif // _WIN32
    }

    /// Function: tcols
    /// Get the number of columns in the terminal window or -1 on error.
    _rlutil_internal_INLINE int tcols(void) {
#ifdef _WIN32
      CONSOLE_SCREEN_BUFFER_INFO csbi;
      if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return -1;
      else
        return csbi.srWindow.Right - csbi.srWindow.Left + 1; // Window width
                                                             // return csbi.dwSize.X; // Buffer width
#else
#ifdef TIOCGSIZE
      struct ttysize ts;
      ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
      return ts.ts_cols;
#elif defined(TIOCGWINSZ)
      struct winsize ts;
      ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
      return ts.ws_col;
#else // TIOCGSIZE
      return -1;
#endif // TIOCGSIZE
#endif // _WIN32
    }

#ifndef min
    /// Function: min
    /// Returns the lesser of the two arguments.
    template <class T> const T& min(const T& a, const T& b) { return (a < b) ? a : b; }
#endif

#ifndef max
    /// Function: max
    /// Returns the greater of the two arguments.
    template <class T> const T& max(const T& a, const T& b) { return (b < a) ? a : b; }
#endif

    // Classes are here at the end so that documentation is pretty.

    /// Class: CursorHider
    /// RAII OOP wrapper for <_rlutil_internal.hidecursor>.
    /// Hides the cursor and shows it again
    /// when the object goes out of scope.
    struct CursorHider {
      CursorHider() { hidecursor(); }
      ~CursorHider() { showcursor(); }
    };

  } 


  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // End namespace _rlutil_internal ////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
#define DEFAULT_WIDTH (_rlutil_internal::tcols() - 1)
#define DEFAULT_HEIGHT _rlutil_internal::trows()

  //Colors!
  enum Color
  {
    //Acquire _rlutil_internal info where possible--
    BLACK = _rlutil_internal::BLACK,
    BLUE = _rlutil_internal::BLUE,
    GREEN = _rlutil_internal::GREEN,
    CYAN = _rlutil_internal::CYAN,
    RED = _rlutil_internal::RED,
    MAGENTA = _rlutil_internal::MAGENTA,
    BROWN = _rlutil_internal::BROWN,
    GREY = _rlutil_internal::GREY,
    DARKGREY = _rlutil_internal::DARKGREY,
    LIGHTBLUE = _rlutil_internal::LIGHTBLUE,
    LIGHTGREEN = _rlutil_internal::LIGHTGREEN,
    LIGHTCYAN = _rlutil_internal::LIGHTCYAN,
    LIGHTRED = _rlutil_internal::LIGHTRED,
    LIGHTMAGENTA = _rlutil_internal::LIGHTMAGENTA,
    YELLOW = _rlutil_internal::YELLOW,
    WHITE = _rlutil_internal::WHITE,

    // Add custom values
    //DEFAULT = _rlutil_internal::DEFAULT, // BROKEN //Added custom to the _rlutil_internal header.
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
    // Hidden Constructors
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


  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // Implementation //////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////


namespace RConsole
{
  // Forward declaration of functions for later.
  namespace RConsoleConfig
  {
    void AddObject(Canvas *c);
    void RemoveObject(Canvas *c);
    void SignalHandler(int signalNum);
    void SetCloseHandler();
  }

  /////////////////////////
 // Raster info object //
////////////////////////
//constructor, no character and just the previous color.
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
  CanvasRaster::CanvasRaster(unsigned int width, unsigned int height)
    : width_(width)
    , height_(height)
    , data_(width, height, RasterInfo(' ', RConsole::WHITE))
  {  }

  // Draws a character to the screen. Returns if it was successful or not.
  bool CanvasRaster::WriteChar(char toDraw, unsigned int x, unsigned int y, Color color)
  {
    data_.GoTo(static_cast<int>(x), static_cast<int>(y));
    data_.Set(RasterInfo(toDraw, color));

    //Everything completed correctly.
    return true;
  }

  // Writes a string to the field
  bool CanvasRaster::WriteString(const char *toWrite, size_t len, unsigned int x, unsigned int y, Color color)
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
  unsigned int CanvasRaster::GetRasterWidth() const
  {
    return width_;
  }

  // Get console height
  unsigned int CanvasRaster::GetRasterHeight() const
  {
    return height_;
  }

  // Constructor
  Canvas::Canvas(unsigned int width, unsigned int height, int xOffset, int yOffset)
    : r_(CanvasRaster(width, height))
    , prev_(CanvasRaster(width, height))
    , isDrawing_(true)
    , width_(width)
    , height_(height)
    , xOffset_(xOffset)
    , yOffset_(yOffset)
    , modified_(Field2D<bool>(width, height))
    , memoryId_(reinterpret_cast<unsigned long>(this))
  {
    RConsoleConfig::AddObject(this);
  }

  /////////////////////////////
 // Public Member Functions //
/////////////////////////////
// Setup with width and height. Can be re-init
  void Canvas::ReInit(unsigned int width, unsigned int height, int xOffset, int yOffset)
  {
    width_ = width;
    height_ = height;
    xOffset_ = xOffset;
    yOffset_ = yOffset;
    r_ = CanvasRaster(width, height);
    prev_ = CanvasRaster(width, height);
    modified_ = Field2D<bool>(width, height);
  }

  // Clear out the screen that the user sees.
  // Note: More expensive than clearing just the previous spaces
  // but less expensive than clearing entire buffer with command.
  void Canvas::FillCanvas(const RasterInfo &ri)
  {
    r_.Fill(ri);
  }

  // Alternate interpretation
  void Canvas::Draw(char toWrite, float x, float y, Color color)
  {
    Draw(toWrite, static_cast<int>(x), static_cast<int>(y), color);
  }

  // Write the specific character in a specific color to a specific location on the console.
  void Canvas::Draw(char toWrite, int x, int y, Color color)
  {
    if (x < 0) return;
    if (y < 0) return;
    if (static_cast<unsigned int>(x) >= width_) return;
    if (static_cast<unsigned int>(y) >= height_) return;

    modified_.GoTo(static_cast<int>(x), static_cast<int>(y));
    modified_.Set(true);
    r_.WriteChar(toWrite, x, y, color);
  }

  // Draw a string with alternate arguments
  void Canvas::DrawString(const char* toDraw, float xStart, float yStart, Color color)
  {
    DrawString(toDraw, static_cast<int>(xStart), static_cast<int>(yStart), color);
  }

  // Draw a string at the given coordinates
  void Canvas::DrawString(const char* toDraw, int xStart, int yStart, Color color)
  {
    size_t len = strlen(toDraw);
    if (len <= 0) return;

    // Bounds check.
    if (xStart < 0) return;
    if (yStart < 0) return;
    if (static_cast<unsigned int>(xStart) >= width_) return;
    if (static_cast<unsigned int>(yStart) >= height_) return;

    // Set the memory we are using to modified.
    modified_.GoTo(static_cast<int>(xStart), static_cast<int>(yStart));
    unsigned int index = modified_.GetIndex();
    if (static_cast<unsigned int>(xStart) >= width_)
      return;

    // Checks the length and adjusts if it will be past.
    int writeLen = len;
    if (writeLen + index > modified_.Length())
      writeLen = modified_.Length() - index;

    // If our length plus the index we are at exceeds the end of the buffer, only write what we can.
    memset(modified_.GetHead() + index, true, writeLen);

    // Write string
    r_.WriteString(toDraw, len, static_cast<int>(xStart), static_cast<int>(yStart), color);
  }

  // Updates the current raster by drawing it to the screen.
  bool Canvas::Update()
  {
    if (!isDrawing_) return false;

    clearPrevious();
    writeRaster(r_);

    // Write and reset the raster.
    memcpy(prev_.GetRasterData().GetHead(), r_.GetRasterData().GetHead(), width_ * height_ * sizeof(RasterInfo));
    r_.Zero();

    _rlutil_internal::setColor(WHITE);

    return true;
  }

  // Draws a point with ASCII to attempt to represent alpha values in 4 steps.
  // Tends to work with windows only...
  void Canvas::DrawAlpha(int x, int y, Color color, float opacity)
  {
    // All characters use represent alt-codes. 
    if (opacity < .25)
      Draw(static_cast<unsigned char>(176), x, y, color);
    else if (opacity < .5)
      Draw(static_cast<unsigned char>(177), x, y, color);
    else if (opacity < .75)
      Draw(static_cast<unsigned char>(178), x, y, color);
    else
      Draw(static_cast<unsigned char>(219), x, y, color);
  }

  void Canvas::DrawAlpha(float x, float y, Color color, float opacity)
  {
    Canvas::DrawAlpha(static_cast<int>(x), static_cast<int>(y), color, opacity);
  }

  // Stops the update loop.
  void Canvas::Shutdown()
  {
    isDrawing_ = false;
  }


  // Draws a point with ASCII to attempt to represent location in a square.
  void Canvas::DrawPartialPoint(float x, float y, Color color)
  {
    // Get first two decimal places from location.
    int xDec = static_cast<int>(x * 100) % 100;
    int yDec = static_cast<int>(x * 100) % 100;

    UNUSED(xDec);
    UNUSED(yDec);

    //If Y is closer to a border, use it for placement.
    if (abs(50 - static_cast<int>(x)) < abs(50 - static_cast<int>(y)))
    {
      if (x > 50)
        return Draw(static_cast<unsigned char>(222), x, y, color);
      return Draw(static_cast<unsigned char>(221), x, y, color);
    }
    //Otherwise, X is closer to a border.
    else
    {
      if (y > 50)
        return Draw(static_cast<unsigned char>(223), x, y, color);
      return Draw(static_cast<unsigned char>(220), x, y, color);
    }
  }

  // Drawing box
  void Canvas::DrawBox(char toWrite, float x1, float y1, float x2, float y2, Color color)
  {

    if (x1 > x2)
    {
      float temp = x1;
      x2 = x1;
      temp = x2;
    }
    if (y1 > y2)
    {
      float temp = y1;
      y2 = y1;
      temp = y2;
    }

    // At this point it can be assumed that x1 and y1 and lower than x2 and y2 respectively.

    for (unsigned int x = static_cast<unsigned int>(x1); x < x2; ++x)
    {
      for (unsigned int y = static_cast<unsigned int>(x1); y < y2; ++y)
      {
        Draw(toWrite, static_cast<float>(x), static_cast<float>(y), color);
      }
    }
  }

  //Set visibility of cursor to specified bool.
  void Canvas::SetCursorVisible(bool isVisible)
  {
    if (!isVisible)
      _rlutil_internal::hidecursor();
    else
      _rlutil_internal::showcursor();
  }

  // Gets the width of the console
  unsigned int Canvas::GetConsoleWidht()
  {
    return width_;
  }

  // Gets the height of the console
  unsigned int Canvas::GetConsoleHeight()
  {
    return height_;
  }

  //////////////////////////////
 // Private Member Functions //
//////////////////////////////
  int Canvas::abs(int x)
  {
    if (x < 0)
      return -x;

    return x;
  }

  // Clears out the screen based on the previous items written. Clear character is a space.
  void Canvas::clearPrevious()
  {
    // Walk through, write over only what was modified.
    modified_.SetIndex(0);
    unsigned int maxIndex = modified_.Length();
    unsigned int index = 0;

    while (true)
    {
      index = modified_.GetIndex();
      const RasterInfo &curr = r_.GetRasterData().Peek(index);
      const RasterInfo &prev = prev_.GetRasterData().Peek(index);

      // If we have not modified the space,
      // and we don't have the same character as last time,
      // and we don't have the same color.
      if (!modified_.Get() && curr != prev)
      {
        // Compute X and Y location
        unsigned int xLoc = (index % width_) + 1 + xOffset_;
        unsigned int yLoc = (index / width_) + 1 + yOffset_;

        if ((yLoc > height_ + yOffset_) == false && (xLoc > width_ + xOffset_) == false)
        {
          // locate on screen and set color
          _rlutil_internal::locate(xLoc, yLoc);

          putC(' ', stdout);
        }
      }

      if (index + 1 >= maxIndex)
        break;

      modified_.IncrementX();
    }

    // Set things back to zero.
    modified_.Zero();
  }


  // Explicitly clears every possible index. 
  // This is expensive, and wipes ALL canvases! 
  void Canvas::fullClear()
  {
    _rlutil_internal::cls();
  }


  // Set the color in the console using utility, if applicable.
  void Canvas::setColor(const Color &color)
  {
    if (color != PREVIOUS_COLOR)
      _rlutil_internal::setColor(color);
  }


  // Write the raster we were attempting to write.
  bool Canvas::writeRaster(CanvasRaster &r)
  {
    // Set initial position.
    Field2D<RConsole::RasterInfo> &data = r.GetRasterData();
    unsigned int maxIndex = data.Length();
    unsigned int index = 0;
    data.SetIndex(0);

    while (true)
    {
      index = data.GetIndex();
      const RasterInfo& ri = data.Get();

      if (ri.Value != 0 && prev_.GetRasterData().Peek(index) != ri)
      {
        unsigned int xLoc = (index % width_) + 1 + xOffset_;
        unsigned int yLoc = (index / width_) + 1 + yOffset_;

        // Handle X
        if ((xLoc > width_ + xOffset_) == false && (yLoc > height_ + yOffset_) == false)
        {
          // locate on screen and set color
          _rlutil_internal::locate(xLoc, yLoc);

          // Set color of cursor
          setColor(ri.C);

          // Print out to the console in the preferred fashion
          int retVal = 0;

          retVal = putC(ri.Value, stdout);

          if (!retVal)
            return false;
        }
      }

      if (index + 1 >= maxIndex)
        break;

      data.IncrementX();
    }

    // Return we successfully printed the raster!
    return true;
  }

  // Cross-platform putc
  int Canvas::putC(int character, FILE * stream)
  {
#if defined(RConsole_NO_THREADING) && defined(OS_WINDOWS)
    return _putc_nolock(character, stream);
#else
    return putc(character, stream);
#endif
  }


  // print out the formatted raster.
  // Note that because of console color formatting, we use the _rlutil_internal coloring option when
  // we are printing to the console, or have no file output specified.
  void Canvas::DumpRaster(FILE * fp)
  {
    // Dump only relevant part of stream.
    for (unsigned int i = 0; i < height_; ++i)
    {
      for (unsigned int j = 0; j < width_; ++j)
      {
        const RasterInfo &ri = r_.GetRasterData().Peek(j, i);
        if (fp == stdout)
        {
          _rlutil_internal::setColor(ri.C);
          std::cout << ri.Value;//fprintf(fp, "%c", ri.Value);
        }
        else
        {
          std::string line = _rlutil_internal::getANSIColor(ri.C) + ri.Value;
          fprintf(fp, "%s", line.c_str());
        }
      }

      std::cout << '\n';
      //fprintf(fp, "\n");
    }

    // Set end color to white when we're done.
    if (fp == stdout)
      _rlutil_internal::setColor(WHITE);
  }


  // Crops all of the raster
  void Canvas::CropRaster(FILE *fp, char toTrim)
  {
    // Establish borders.
    unsigned int Xmin = width_;
    unsigned int Xmax = 0;
    unsigned int Ymin = height_;
    unsigned int Ymax = 0;

    for (unsigned int i = 0; i < width_; ++i)
    {
      for (unsigned int j = 0; j < height_; ++j)
      {
        if (r_.GetRasterData().Peek(i, j).Value != toTrim)
        {
          if (i < Xmin) Xmin = i;
          if (j < Ymin) Ymin = j;
          if (i > Xmax) Xmax = i;
          if (j > Ymax) Ymax = j;
        }
      }
    }

    // If we're trimming everything, don't even bother.
    if (Xmin > Xmax) return;
    if (Ymin > Ymax) return;

    // Dump only relevant part of stream.
    for (unsigned int j = Ymin; j <= Ymax; ++j)
    {
      for (unsigned int i = Xmin; i <= Xmax; ++i)
      {
        const RasterInfo &ri = r_.GetRasterData().Peek(i, j);
        if (fp == stdout)
        {
          _rlutil_internal::setColor(ri.C);
          fprintf(fp, "%c", ri.Value);
        }
        else
        {
          std::string line = _rlutil_internal::getANSIColor(ri.C) + ri.Value;
          fprintf(fp, "%s", line.c_str());
        }
      }

      fprintf(fp, "\n");
    }

    // Set end color to white when we're done.
    if (fp == stdout)
      _rlutil_internal::setColor(WHITE);
  }


  // Returns the location in memory of the object when it was created.
  unsigned long Canvas::GetMemID()
  {
    return memoryId_;
  }

  namespace RConsoleConfig
  {
    // tracks all active canvases in a hashmap.
    std::unordered_map<long, Canvas *> ActiveCanvases = std::unordered_map<long, Canvas *>();
    bool HasInitializedGlobalSignals = false;

    void SignalHandler(int signalNum)
    {
      for (auto &pair : ActiveCanvases)
        pair.second->Shutdown();

      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      int height = _rlutil_internal::trows();
      _rlutil_internal::locate(0, height);
      _rlutil_internal::setColor(WHITE);
      std::cout << '\n';
      exit(signalNum);
    }

    void SetCloseHandler()
    {
      signal(SIGTERM, SignalHandler);
      signal(SIGINT, SignalHandler);
    }

    void RemoveObject(Canvas *c) { ActiveCanvases.erase(c->GetMemID()); }
    void AddObject(Canvas *c)
    {
      ActiveCanvases[c->GetMemID()] = c;
      if (!HasInitializedGlobalSignals)
      {
        SetCloseHandler();
        HasInitializedGlobalSignals = true;
      }
    }
  }
}

#endif
