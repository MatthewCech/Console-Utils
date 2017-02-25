# Console [Drawing] Utilities
This is a cross-platform console drawing project that only modifies relevant characters, inspired vaguely by some gif and video compression techniques. 

Features:

- Drawing at a specified location in the console!
- Simplified console drawing/write calls!
- Frame interpolation for higher update speeds!
- Foreground character colors!
- Flexible draw area sizing!

Not Features:

- Can not walk your dog.
- Can not do your laundry.
- Can not mend a broken heart.
- Really bad at providing cooking advice.

Known Issues:

- Program termination via Ctrl+C in a terminal yields mixed results. Mitigated by adding delay before exiting, which admittedly is not really a solution.
- Automatic sizing on startup is occasionally off-by-one off for window height.
- vs2015 Release with maximum optimizations on some systems can cause improper rendering.

Did you encounter one of these issues? Please feel free to submit them under the issues section of the github repo. More information is very appreciated!

###Sample Hello World:
This uses the single header and .cpp file in the `Conglomerated Source` Folder
```c++
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
