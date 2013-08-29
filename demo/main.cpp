/**
 * BIL demo
 */

#include <BIL/Application.hpp>
#include <BIL/Window.hpp>
#include <BIL/Button.hpp>
#include <BIL/ToggleButton.hpp>

int main (int argc, char* argv[])
{
	using namespace BIL;

	Cpp::Events::ProcessInit processInit;

	setlocale(LC_ALL, "");

	Application app;

	Window win (1024, 800, "Demo Window for BIL", NULL, NULL);

	app.setMainWindow(&win);
	app.initialize();

	Button normal_btn(L"Normal Button");
	ToggleButton toggle_btn(L"Toggle Button");

	normal_btn.set_pos(100, 100);
	toggle_btn.set_pos(100, 200);

	normal_btn.setParent(&win);
	toggle_btn.setParent(&win);

	app.run();

	return 0;
}
