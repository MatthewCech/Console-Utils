#pragma once



namespace RConsole
{
  //Forward declare Field2D for use later.
  template <typename T>
  class Field2D;


  //A proxy class for the [] operator, allowing you to use the [] operator
  //again 
  template <typename T>
  class Field2DProxy
  {
    //Mark the Field2D as my friend!
    friend Field2D<T>;

  public:
    //Operator Overload
    T &operator[](unsigned int y);

  private:
    //Private constructor, friends only!
    Field2DProxy(Field2D<T> *parentField, unsigned int xPos);

    //Variables
    Field2D<T> *field_;
    const int x_;
  };


  //A 2D way to represent a 1D line of continuous memory.
  //The 2D Field keeps track of the current index you are at in memory, allowing really
  //cheap O(K) reading if you have the spot selected, with a single add.
  //Note that this is not guarded- if you reach the "end" of the width, it will
  //let you freely step onto the next row of the 2D array you have set up.
  template <typename T>
  class Field2D
  {
    //Friensd can see ALL!
    friend Field2DProxy<T>;

  public:
    //Constructor
    Field2D(unsigned int w, unsigned int h);
    Field2D(unsigned int w, unsigned int h, const T& defaultVal);
    ~Field2D();

    //Member Functions - Complex Manipulation
    void Zero();
    void Set(const T &newItem);
    T &Get(unsigned int x, unsigned int y);
    Field2DProxy<T> operator[](unsigned int xPos);
    void GoTo(unsigned int x, unsigned int y);
    const T& Get(unsigned int x, unsigned int y) const;
    const T& Peek(unsigned int x, unsigned int y) const;
    const T& Peek(unsigned int index) const;

    //Basic Manipulation
    T &Get();
    T* GetHead() { return data_;}
    const T &Get() const;
    void IncrementX();
    void IncrementY();
    void DecrementX();
    void DecrementY();
    unsigned int GetIndex();
    void SetIndex(unsigned int index);

  private:
    //Variables
    unsigned int index_;
    unsigned int width_;
    unsigned int height_;
    T *data_;
  };
}


//Template implementations: Place in separate file:
namespace RConsole
{


  /////////////////////////////////
 //Field2DProxy Methods and Co.//
////////////////////////////////
  template <typename T>
  Field2DProxy<T>::Field2DProxy(Field2D<T> *parentField, unsigned int xPos)
    : field_(parentField)
    , x_(xPos)
  {  }


  //[] Operator Overload.
  //Note- This sets the current index!
  template <typename T>
  T &Field2DProxy<T>::operator[](unsigned int y)
  {
    field_->GoTo(x_, y);
    return field_->Get();
  }


  ///////////////////////////
 //Field2D Methods and Co.//
///////////////////////////
  //Constructor
  //Defaults by setting everything to 0.
  template <typename T>
  Field2D<T>::Field2D(unsigned int w, unsigned int h)
    : index_(0)
    , width_(w)
    , height_(h)
  {
    data_ = new T[w * h];
    Zero();
  };

  //Sets all values to given default.
  template <typename T>
  Field2D<T>::Field2D(unsigned int w, unsigned int h, const T& defaultVal)
    : index_(0)
    , width_(w)
    , height_(h)
  {
    data_ = new T[w * h];
    for (int i = 0; i < w; ++i)
      for (int j = 0; j < h; ++j)
        Set(i, j, defaultVal);
    index_ = 0;
  }

  //Destructor
  template <typename T>
  Field2D<T>::~Field2D()
  {
    delete[] data_;
  }


  //////////////////////
 //Complex Operations//
//////////////////////
  //Get the item at the position X, Y.
  //Does not set the actual index of the Field!
  template <typename T>
  T &Field2D<T>::Get(unsigned int x, unsigned int y)
  {
    GoTo(x, y);
    return Get();
  }


  //Const version of get that returns const reference.
  template <typename T>
  const T &Field2D<T>::Get(unsigned int x, unsigned int y) const
  {
    return Get(x, y);
  }

  //Get the first part of a 2D array operator
  template <typename T>
  Field2DProxy<T> Field2D<T>::operator[](unsigned int xPos) 
  {
    return Field2DProxy<T>(this, xPos);
  }


  //Set the value at the current index
  template <typename T>
  void Field2D<T>::Set(const T &newItem)
  {
    data_[index_] = newItem;
  }


  //Glance at a read-only version of a specified location. Does NOT set index.
  template <typename T>
  const T& Field2D<T>::Peek(unsigned int x, unsigned int y) const
  {
    return data_[x + y * width_];
  }


  //Get the value at the specified index.
  template <typename T>
  const T& Field2D<T>::Peek(unsigned int index) const
  {
    return data_[index];
  }


  //Chance selected index to specified point.
  template <typename T>
  void Field2D<T>::GoTo(unsigned int x, unsigned int y)
  {
    index_ = x + y * width_;
  }


  //Sets all memory to 0. Does NOT modify index!
  template <typename T>
  void Field2D<T>::Zero()
  {
    memset(data_, 0, sizeof(T) * width_ * height_);
  }

  ////////////////////
 //Cheap operations//
////////////////////
  //Get the value at the current index.
  template <typename T>
  T &Field2D<T>::Get()
  {
    return data_[index_];
  }


  //Const get.
  template <typename T>
  const T& Field2D<T>::Get() const
  {
    return data_[index_];
  }


  //Increment X location by 1 in the 2D field
  template <typename T>
  void Field2D<T>::IncrementX()
  {
    ++index_;
  }


  //Increment Y location by 1 in the 2D field
  template <typename T>
  void Field2D<T>::IncrementY()
  {
    index_ += width_;
  }


  //Decrement X location by 1 in the 2D field
  template <typename T>
  void Field2D<T>::DecrementX()
  {
    --index_;
  }


  //Decrement Y location by 1 in the 2D Field
  template <typename T>
  void Field2D<T>::DecrementY()
  {
    index_ -= width_;
  }


  //Gets the index that the 2D Field currently has.
  template <typename T>
  unsigned int Field2D<T>::GetIndex()
  {
    return index_;
  }


  //Gets the index that the 2D Field currently has.
  template <typename T>
  void Field2D<T>::SetIndex(unsigned int index)
  {
    index_ = index;
  }
}
