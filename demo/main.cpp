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

	setlocale(LC_ALL, "");

	Application app;

	Window win (1024, 800, "Demo Window for BIL", NULL, NULL);

	app.setMainWindow(&win);
	app.initialize();

  ShaderWidget widget;
  widget.setParent(&win);
  widget.set_pos(100, 100);
  widget.resize(200, 200);

	app.run();

	return 0;
}
