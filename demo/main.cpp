/**
 * BIL demo
 */

#include <GL/glew.h>

#include <BIL/Application.hpp>
#include <BIL/Window.hpp>
#include <BIL/Button.hpp>
#include <BIL/ToggleButton.hpp>

#include "ShaderWidget.hpp"

int main (int argc, char* argv[])
{
	using namespace BIL;

	Cpp::Events::ProcessInit processInit;

	BIL::initialize(true);

	Window win (1024, 800, "Demo Window for BIL", NULL, NULL);

  //ShaderWidget widget;
	Widget widget;
  widget.setParent(&win);
  widget.set_pos(100, 100);
  widget.resize(200, 200);

	BIL::run(&win);

	BIL::terminate();

	return 0;
}
