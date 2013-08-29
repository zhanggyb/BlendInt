// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include "LayoutTest.h"

#include <BIL/Label.hpp>
#include <BIL/Button.hpp>
#include <BIL/FontConfig.hpp>
#include <BIL/Rect.hpp>
#include <BIL/HorizontalLayout.hpp>
#include <BIL/Traceable.hpp>


using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(LayoutTest);

LayoutTest::LayoutTest ()
{

}

LayoutTest::~LayoutTest ()
{

}

void LayoutTest::setUp ()
{
	int mapsize = Traceable::mapSize();
	int listsize = Traceable::getList().size();

	if(listsize > 0) {
		list<Traceable*>::const_iterator it;
		for (it = Traceable::getList().begin(); it != Traceable::getList().end(); it++)
		{
			cout << "id: " << (*it)->id() << " was not deleted!" << endl;
		}
	}

	CPPUNIT_ASSERT(mapsize == 0 && listsize == 0);
}

void LayoutTest::tearDown ()
{
	int mapsize = Traceable::mapSize();
	int listsize = Traceable::getList().size();

	if(mapsize > 0) {
		map<uint64_t, Traceable*>::const_iterator it;
		for (it = Traceable::getMap().begin(); it != Traceable::getMap().end(); it++)
		{
			cout << "id: " << it->first << " was not deleted!" << endl;
		}
	}

	if(listsize > 0) {
		list<Traceable*>::const_iterator it;
		for (it = Traceable::getList().begin(); it != Traceable::getList().end(); it++)
		{
			cout << "id: " << (*it)->id() << " was not deleted!" << endl;
		}
	}

	CPPUNIT_ASSERT(mapsize == 0 && listsize == 0);
}

void LayoutTest::horizontal_layout1()
{
	Application app;

	Window win(640, 480, "640 x 480 Window", NULL, NULL);

	app.setMainWindow(&win);
	app.initialize();

	HorizontalLayout layout;
	layout.set_pos(100, 100);
	layout.setParent(&win);

	Label label(L"Hello World!");
	label.set_pos(100, 100);

	Button button(L"Sample Button");
	button.set_pos(100, 200);

	layout.addWidget(&label);
	layout.addWidget(&button);

	app.run();
	CPPUNIT_ASSERT(true);
}


