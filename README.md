# BlendInt

**This project is deprecated, I realized it's better to use [Electron](http://electron.atom.io/)+HTML5+CSS3 to develop cross-platform application.**

------

## What is BlendInt

**BlendInt** is a lightweight C++ GUI toolkit inspired by
[Blender](http://www.blender.org) for building cross-platform
application. It makes use of the modern OpenGL technology, and
provides a comprehensive set of widgets that are easily extensible via
inheritance.

BlendInt is free software and covered by the GNU Library General
Public License (LGPL) v3.

<font color="#FF1F1F">**Warning: BlendInt is under heavy
development. Support cannot be guaranteed and it may not function as
you expect.**</font>

## Features

Like [Blender](http://www.blender.org), all widgets in BlendInt are
drawn in OpenGL context and use the same color schemes. Application
built with BlendInt has the similar look and feel of
[Blender](http://www.blender.org), but this project is totally
rewritten and only focused on user interface with some new
technologies:

- It's based on new OpenGL API (3.3 and later), all widgets are drawn
  through vertex/fragment/geometry shaders. (Legacy OpenGL APIs which
  are being used in [Blender](http://www.blender.org) were removed
  since GL 3.3)
- It's OOP designed and written by C++, every widget is now a C++
  object, you can use the pre-defined UI classes (from a simple button
  to a MVC based item view), or subclass one of them and customize it
  easily.
- It uses a fast Event/Delegate (signal/slot) framework.
- It just needs an OpenGL Context, so theoretically you can integrate
  it in any other GUI framework or toolkit, for example, Glut, GLFW,
  Qt, Gtk, Cocoa. Currently BlendInt uses GLFW 3.1 to display and
  manage OpenGL context for some demos and unit tests.

## Usage

Using **BlendInt** is as simple as any C++ object-oriented GUI toolkit
like Qt, gtkmm. Of course, you must create an OpenGL window first.

Here is a simple example to create an 240x180 pixel window, and show a
MessageBox within.

```cpp
#include <blendint/gui/window.hpp>
#include <blendint/gui/message-box.hpp>

int main(int argc, char* argv[])
{
  using namespace BlendInt;

  if(Window::Initialize()) {

    Window win(240, 180, "Tutorial 01");

    MessageBox* message = new MessageBox("Hello World!",
                                         "Click the \"close\" button.");
    // 'message' will be destroyed when the window is closed.
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

Compile this code snippet with BlendInt and run in different OS
platforms, you'll get a small window with the same context. (In this
example,
[Adobe Source Han Sans CN](https://github.com/adobe-fonts/source-han-sans)
was loaded in both platforms)

![Hello World in Linux](https://github.com/zhanggyb/BlendInt/blob/master/doc/images/hello_linux.png
 "Hello World in Linux")
![Hello World in OS X](https://github.com/zhanggyb/BlendInt/blob/master/doc/images/hello_osx.png
 "Hello World in OS X")

See [Wiki page](https://github.com/zhanggyb/BlendInt/wiki) for more
information of how to program with BlendInt.

## Dependencies

BlendInt is developed with modern OpenGL technology, to run your
application linked with BlendInt, you must have a graphics card which
supports OpenGL 3.3 or newer.

To build BlendInt, make sure the following development tools and
libraries are installed:

Required:

* C++11 compiler
* [cmake](http://www.cmake.org) 2.8.11 or newer
* [OpenGL](http://www.khronos.org) header and libraries, in Linux,
  Mesa 9.2 or newer
* [OpenImageIO](https://sites.google.com/site/openimageio/home) 1.1+
* [boost](http://www.boost.org) (system, filesystem, thread)
* [Freetype2](http://www.freetype.org)
* [GLM](http://glm.g-truc.net) 0.9.6.0+
* [Fontconfig](http://freedesktop.org/wiki/Software/fontconfig/)
* [GLFW](http://www.glfw.org) 3.1+

Optional:

* [Qt](http://www.qt-project.org) 5.x+
* [Opencv](http://www.opencv.org)
* [Doxygen](http://www.stack.nl/~dimitri/doxygen/) (to build api
  documentation)

The source code is verified to be built and workable on:

* Mac OS X 10.9 (Mavericks), 10.10 (Yosemite)
* ArchLinux
* Ubuntu 13.10, 14.04(LTS), 15.04
* Fedora 20, 21

## Build the Source Code

See
[Build the Source](https://github.com/zhanggyb/BlendInt/wiki/build) in
Wiki.

## Contributing

See the
[contributor](https://github.com/zhanggyb/BlendInt/graphs/contributors)
page.

## License

* Most source code in this project is released under the LGPL v3
  License. See the bundled LICENSE file for details.
* BlendInt use [libCppEvent](http://github.com/zhanggyb/libCppEvent)
  for sending/receiving events between objects, which is released
  under MIT License.
* [RapidXml](http://rapidxml.sourceforge.net/) - The author of
  RapidXml is Marcin Kalicinski, use of this software is granted under
  one of the following two licenses: boost software license or MIT
  license.
