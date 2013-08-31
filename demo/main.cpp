/**
 * BIL demo
 */

#include <GL/glew.h>

#include <BIL/Application.hpp>
#include <BIL/Window.hpp>

#include "ShaderWidget.hpp"

int main (int argc, char* argv[])
{
	using namespace BIL;

	Cpp::Events::ProcessInit processInit;

	BIL::initialize(true);

	Window win (1200, 800, "Demo Window for BIL", NULL, NULL);

	BIL::run(&win);

	BIL::terminate();

	return 0;
}
