# BlendInt

------

## What is BlendInt

**BlendInt** is a lightweight, open-source GUI library for building
application with [Blender](http://www.blender.org)-like style. It's
written in C++ and make use of the modern OpenGL technology, and
provides a comprehensive set of widgets that are easily extensible via
inheritance.

BlendInt is free software distributed under the GNU Library General
Public License (LGPL) v3.

**Warning: BlendInt is under heavy development. Support cannot be
  guaranteed and it may not function as you expect.**

## Features

Like Blender, all widgets in BlendInt are drawn in OpenGL context and
use the same color schemes. But BlendInt is totally rewritten and
improved with some new technologies:

- It's based on new OpenGL API (3.3 and later), all widgets are drawn
  through vertex/fragment/geometry shaders. (Legacy OpenGL APIs which
  are being used in Blender were removed since GL 3.3)
- It's OOP designed and written by C++, every widget is now a C++
  object, you can use the pre-defined Button, Slider, Menu, etc or
  subclass one of them and customize it easily.
- It uses [CppEvent](http://code.google.com/p/cpp-events/) to provide
  a fast Event/Delegate (signal/slot) mechanism.
- It just needs an OpenGL Context, so theoretically you can integrate
  it in any other OpenGL Window or GUI toolkit, for example, Glut,
  GLFW, Qt, Gtk, Cocoa. Currently BlendInt uses GLFW3 to display and
  manage OpenGL context for some demos and unit tests.

Here're some example widgets:

* Widget
* Button
* ToggleButton
* Label

## Usage

Useing **BlendInt** is as simple as any C++ object-oriented GUI
toolkit like Qt, gtkmm. Of course, you must create an OpenGL window
first.

For example, create a button with BlendInt:

```cpp
#include <BlendInt/Interface.hpp>    // The main header to start
#include <BlendInt/Gui/Button.hpp>	// Button is in Gui module

int main (int argc, char* argv[])
{
	using namespace BlendInt;
	
	// Don't forget to initialize CppEvents in main stack
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	// Create opengl context and make it current in your favoriate
	// OpenGL window system such as GLUT, GLFW, Cocoa

	Interface::Initialize();

	// Create a root container called Context too in BlendInt
	Context* context = Manage(new Context);
	Instance::instance->SetCurrentContext(context);

	// Create a button
	Button* btn = Manage(new Button);
	btn->SetText("Hello World!");
	btn->SetPosition(200, 200); 

	// Add widgets to the context
	context->PushBack(btn);

	// In the event-render loop
	Interface::instance->Draw();

	// Ready to close application
	Interface::Release();
}
```

## Requirements

BlendInt make use of the modern OpenGL technology, to run your
application linked with BlendInt, you must have a graphics card which
supports OpenGL 3.3 or newer.

To build BlendInt, make sure the following development tools and
libraries are installed:

* cmake 2.8.11 or newer
* OpenGL header and libraries, in Linux, Mesa 9.2 or newer
* OpenImageIO 1.1+
* boost
* freetype
* fontconfig (recommanded)

The source code is verified to be built and workable on:

* Mac OS X 10.9 (Mavericks)
* ArchLinux
* Ubuntu 13.10, 14.04(LTS)
* Fedora 20

## Build the Source Code

### Build source code in Mac OS X

### Build source code in Linux

### Build and Try the Demos

To build and run the demos, use additional cmake options:

```shell
$ cmake <source dir> -DWITH_GLFW3_DEMO=TRUE
```

To run glfw3 demo:

```shell
$ ./bin/glfw_demo
```

### Build and Run Unit Tests

To build the unit test, add `-DWITH_UNIT_TEST=TRUE`:

```shell
$ cmake <source dir> -DWITH_UNIT_TEST=TRUE
```


To run the unit tests:

```shell
$ ./bin/test<class name>
```


## Installation

## Contributing

## Credits

## License

* Most BlendInt (Blender Interface Library in OpenGL) source code is
  released under the LGPL v3 License. See the bundled LICENSE file for
  details.
* BlendInt use [CppEvents](http://code.google.com/p/cpp-events/) for
  sending/receiving events between objects, it's released under MIT
  License, developed by Nickolas V. Pohilets (pohil...@gmail.com).
* GLFW3 - used in some demos and unit tests, released under
  zlib/libpng license.
* glm - a header only C++ mathematics library for graphics software
  based on the OpenGL Shading Language (GLSL) specification and
  released under the MIT license.
* rapidxml - The author of RapidXml is Marcin Kalicinski, use of this
  software is granted under one of the following two licenses: boost
  software license or MIT license.
