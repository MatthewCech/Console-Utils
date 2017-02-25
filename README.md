# console_drawing_utilities
This is a cross-platform console drawing project that only modifies relevant characters, as inspired by video compression techniques. 

Features:

- Simplified console draw/write calls.
- Frame interpolation for smoother images.
- Foreground character colors!
- Text rendering.

Not Features:

- Can not walk your dog.
- Can not do your laundry.
- Can not mend your broken heart.
- Really bad at providing cooking advice.


###Hello World:
This uses the single header and .cpp file in the `Assembled Library` Folder
```
#include "console-utils.hpp"

int main(void)
{
  RConsole::Canvas::ReInit(20, 10);
  RConsole::Canvas::SetCursorVisible(false);
  
  while(1)
  {
    RConsole::Canvas::DrawString("Hello World!", 2, 5, RConsole::CYAN);
    RConsole::Canvas::Update();
  }
  
  return 0;
}
```
