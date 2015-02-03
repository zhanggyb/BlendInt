# BlendInt

------

## What is BlendInt

**BlendInt** is a lightweight C++ GUI toolkit for building
cross-platform application with [Blender](http://www.blender.org)-like
style. It makes use of the modern OpenGL technology, and provides a
comprehensive set of widgets that are easily extensible via
inheritance.

BlendInt is free software and covered by the GNU Library General
Public License (LGPL) v3.

**Warning: BlendInt is under heavy development. Support cannot be
  guaranteed and it may not function as you expect.**

## Features

Like Blender, all widgets in BlendInt are drawn in OpenGL context and
use the same color schemes. But BlendInt is totally rewritten and only
focused on user interface with some new technologies:

- It's based on new OpenGL API (3.3 and later), all widgets are drawn
  through vertex/fragment/geometry shaders. (Legacy OpenGL APIs which
  are being used in Blender were removed since GL 3.3)
- It's OOP designed and written by C++, every widget is now a C++
  object, you can use the pre-defined UI classes such as Button,
  Slider, Menu etc, or subclass one of them and customize it easily.
- It uses [CppEvent](http://code.google.com/p/cpp-events/) to provide
  a fast Event/Delegate (signal/slot) mechanism.
- It just needs an OpenGL Context, so theoretically you can integrate
  it in any other OpenGL Window or GUI toolkit, for example, Glut,
  GLFW, Qt, Gtk, Cocoa. Currently BlendInt uses GLFW 3.1 to display
  and manage OpenGL context for some demos and unit tests.

## Usage

Using **BlendInt** is as simple as any C++ object-oriented GUI toolkit
like Qt, gtkmm. Of course, you must create an OpenGL window first.

Here is a simple example to create an 240x180 pixel window, and show a
MessageBox within.

```cpp
#include <BlendInt/Gui/Window.hpp>
#include <BlendInt/Gui/MessageBox.hpp>

int main(int argc, char* argv[])
{
  using namespace BlendInt;

  BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

  if(Window::Initialize()) {

    Window win(240, 180, "Tutorial 01");

    MessageBox* message = new MessageBox("Hello World!",
        "Click the \"close\" button.");
    win.AddFrame(message);
    message->MoveTo(
        (win.size().width() - message->size().width()) / 2,
        (win.size().height() - message->size().height()) / 2);

    win.Exec();

    Window::Terminate();
  }

  return 0;
}
```

Don't worry about the "message" instance, it will be destroyed when
 the window is closed.

![Hello World in Linux](https://github.com/zhanggyb/BlendInt/blob/master/doc/images/hello_linux.png
 "Hello World in Linux")

See wiki page for more information of how to use BlendInt.

## Dependencies

BlendInt make use of the modern OpenGL technology, to run your
application linked with BlendInt, you must have a graphics card which
supports OpenGL 3.3 or newer.

To build BlendInt, make sure the following development tools and
libraries are installed:

Required:

* [cmake](http://www.cmake.org)  2.8.11 or newer
* [OpenGL](http://www.khronos.org) header and libraries, in Linux, Mesa 9.2 or newer
* [OpenImageIO](https://sites.google.com/site/openimageio/home) 1.1+
* [boost](http://www.boost.org) (system, filesystem, thread)
* [Freetype2](http://www.freetype.org)
* [GLM](http://glm.g-truc.net)
* [Fontconfig](http://freedesktop.org/wiki/Software/fontconfig/)
* [GLFW](http://www.glfw.org) 3.1+

Optional:

* [Qt](http://www.qt-project.org) 5.x+
* [Opencv](http://www.opencv.org)
* [Doxygen](http://www.stack.nl/~dimitri/doxygen/) (to build api documentation)

The source code is verified to be built and workable on:

* Mac OS X 10.9 (Mavericks), 10.10 (Yosemite)
* ArchLinux
* Ubuntu 13.10, 14.04(LTS)
* Fedora 20

### Install Dependencies in Mac OS X

The packages required above can be install through
[Homebrew](http://brew.sh):

```shell
$ brew install cmake openimageio boost freetype fontconfig
```

### Install Dependencies in ArchLinux

Use Pacman in ArchLinux:

```shell
$ sudo pacman -S cmake openimageio boost freetype fontconfig
```

### Install Dependencies in Ubuntu

Use apt-get in Ubuntu:

```shell
$ sudo apt-get install cmake libopenimageio-dev libboost-dev
```

### Install Dependencies in Fedora

Use yum in Fedora:

```shell
$ sudo yum install cmake OpenImageIO-devel boost-devel freetype-devel fontconfig-devel
```

## Build the Source Code

### Get the Source Code

Use git to checkout the source code from the repository on Bitbucket:

```shell
$ git clone https://zhanggyb@bitbucket.org/zhanggyb/blendint.git
```

or (to checkout submodules as well):

```shell
$ git clone --recursive https://zhanggyb@bitbucket.org/zhanggyb/blendint.git
```

### Useful cmake options and environment variables

````
-DCMAKE_BUILD_TYPE=[Debug|Release]

-DCMAKE_INSTALL_PREFIX=[base path to install BlendInt (default: /usr/local)]

-DBUILD_STATIC_LIBRARY		// Build static library instead of shared (default: OFF)
-DWITH_BUILTIN_GLFW3		// Build with built-in GLFW3.1 source (default: ON)
-DWITH_BUILTIN_GLM			// Build with built-in GLM headers (default: ON)
-DENABLE_OPENCV				// Enable OpenCV support (default: OFF)
-DWITH_GPERFTOOLS			// Build with Google perftools option (default: OFF)
-DWITH_GLFW3_DEMO			// Build GLFW3 demos (default: OFF)
-DWITH_UNIT_TEST			// Build unit test (default: OFF)
-DBUILD_DOCUMENTATION		// Use Doxygen to create the HTML based API documentation (default: OFF)
````

### Build the Library

```shell
$ cd <blendint dir>
$ mkdir build
$ cd build
$ cmake ..
$ make
```
### Build and Try the Demos

To build and run the demos, use additional cmake options:

```shell
$ cmake <source dir> -DWITH_GLFW3_DEMO=TRUE
```

To run a demo:

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

Install header files, binaries, html documentations with:

```shell
$ sudo make install
```

after build the source code.

By default it will install header files to /usr/include/BlendInt, and
install binary files to /usr/lib/libBlendInt.so.

To build static library, add cmake option:
`-DBUILD_STATIC_LIBRARY=TRUE` instead.

## Contributing

TBD

## License

* Most BlendInt (Blender Interface Library in OpenGL) source code is
  released under the LGPL v3 License. See the bundled LICENSE file for
  details.
* BlendInt use [CppEvents](http://code.google.com/p/cpp-events/) for
  sending/receiving events between objects, it's released under MIT
  License, developed by Nickolas V. Pohilets (pohil...@gmail.com).
* [GLFW](http://www.glfw.org) 3.1+ - used in some demos and unit
  tests, released under zlib/libpng license.
* [GLM](http://glm.g-truc.net) - a header only C++ mathematics library
  for graphics software based on the OpenGL Shading Language (GLSL)
  specification and released under the MIT license.
* [RapidXml](http://rapidxml.sourceforge.net/) - The author of
  RapidXml is Marcin Kalicinski, use of this software is granted under
  one of the following two licenses: boost software license or MIT
  license.
