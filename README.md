# BlendInt

------

## What is BlendInt

**BlendInt** (Blender Interface Library in OpenGL) is a C++ library
for building application with Blender-like style.

This project is for anyone who like Blender and want to build a
similar GUI application.

Features of vertion 0.1 (developing):

* Event system
* Widgets:
  * Buttons:
        * Push Button
        * Toggle Button
        * Checkable Button
  * Scroll Widget
  * Label
  * Slider
  * Tab Widget
* Layout
  * Horizontal Layout
  * Vertical Layout

## Usage

Use **BlendInt** is as simple as any C++ object-oriented GUI toolkit
like Qt, gtkmm.

For example, create a button with BlendInt:

```cpp
#include <BlendInt/Interface.hpp>    // The main header to start
#include <BlendInt/Button.hpp>

int main (int argc, char* argv[])
{
    // initialize opengl context
    BlendInt::Interface::initialize();

    Button* my_button = new Button;
    my_button.set_pos(200, 200); 

    // in the event-Render loop
    app->Render();

    // ready to close application
    BlendInt::Interface::release();
}
```

## Requirements

## Installation

## Contributing

## Running the Tests

## Credits

## License

Most BlendInt (Blender Interface Library in OpenGL) source code is
released under the LGPL v3 License. See the bundled LICENSE file for
details.

BlendInt use [CppEvents](http://code.google.com/p/cpp-events/) for
sending/receiving events between objects, it's released under MIT
License, developed by Nickolas V. Pohilets (pohil...@gmail.com).
