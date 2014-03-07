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
#include <BlendInt/Gui/Button.hpp>	// Button is in Gui module

namespace BI = BlendInt;	// namespace alias

int main (int argc, char* argv[])
{
    // Don't forget to initialize CppEvents in main stack
    BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

    // Create opengl context and make it current in your favoriate
    // OpenGL window system such as GLUT, GLFW, Cocoa

    BI::Interface::Initialize();

    // Create a button
    Button* btn = BI::Manage(new Button);
    btn->SetText("Hello World!");
    btn->SetPosition(200, 200); 

    // In the event-Render loop
    BI::Interface::instance->Draw();

    // Ready to close application
    BI::Interface::Release();
}
```

## Requirements

BlendInt make use of the modern OpenGL technology, to run your
application linked with BlendInt, you must have a graphics card which
supports OpenGL 3.1.

To build BlendInt, make sure the following libraries are installed:

* cmake 2.8.11 or newer
* OpenGL header and libraries, in Linux, Mesa 9.2 or newer
* OpenImageIO 1.1+
* boost

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
