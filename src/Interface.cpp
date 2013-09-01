/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>
#include <iostream>

#include <BIL/Interface.hpp>

#include <BIL/FontConfig.hpp>
#include <BIL/FontCache.hpp>
#include <BIL/Theme.hpp>
#include <BIL/Size.hpp>
#include <BIL/Traceable.hpp>
#include <BIL/Drawable.hpp>

namespace BIL {

	Interface* Interface::interface = 0;

	Interface* Interface::instance()
	{
		if (!interface) {
			interface = new Interface();
		}

		return interface;
	}

	Interface::Interface ()
	: cursor_pos_x_ (0.0), cursor_pos_y_ (0.0)
	{

	}

	Interface::~Interface()
	{

	}

	bool Interface::initialize ()
	{
		glewExperimental = true;	// Needed in core profile
		if(glewInit() != GLEW_OK) {
			std::cerr << "Fail to initialize GLEW" << endl;
			return false;
		}

		bool result = true;
		if (!FontConfig::instance()) {
			std::cerr << "Cannot initialize FontConfig" << std::endl;
			result = false;
		}

		FontConfig* ftconfig = FontConfig::getService();
		bool fontinit = ftconfig->initialize();
		if (!fontinit) {
			cerr << "Cannot initialize font service" << endl;
			result = false;
		}

		fontinit = ftconfig->loadDefaultFontToMem();
		if (!fontinit) {
			cerr << "Cannot load default font into memory" << endl;
			result = false;
		}

		Theme* theme = Theme::instance();
		if (theme) {
			theme->initialize();
		} else {
			cerr << "Cannot initialize themes" << endl;
			result = false;
		}

		return result;
	}

	void Interface::release ()
	{
		FontConfig::release();
		FontCache::releaseAll();
		Theme::release();

		if (interface) {
			delete interface;
			interface = 0;
		}
	}

	void Interface::render()
	{
		int width = size_.width();
		int height = size_.height();
		// float ratio = width / (float) height;

		// float bg = 114.0 / 255;	// the default blender background color
		glClearColor(0.447, 0.447, 0.447, 1.00);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glColor4f(1.00, 1.00, 1.00, 1.00);

		// enable anti-alias
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		//glEnable (GL_POINT_SMOOTH);
		//glEnable (GL_LINE_SMOOTH);
		//glEnable (GL_POLYGON_SMOOTH);

		glViewport(0, 0, width, height);
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.f, (float) width, 0.f, (float) height, 100.f, -100.f);

		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity();

#ifdef DEBUG
		drawGrid (width, height);
#endif

		Drawable *item = NULL;
		list<Traceable*>::const_iterator j;
		for (j = Traceable::getList().begin(); j != Traceable::getList().end(); j++)
		{
			item = dynamic_cast<Drawable*>(*j);
			if (item)	render(item);
		}

		glDisable(GL_BLEND);
	}

	void Interface::render (Drawable* obj)
	{
		list<Traceable*>::const_iterator it;
		Drawable *item = NULL;
		for (it = obj->children().begin(); it != obj->children().end(); it++) {
			item = dynamic_cast<Drawable*>(*it);
			if (item) {
				render (item);
			}
		}

		obj->render();
	}

#ifdef DEBUG
	void Interface::drawGrid (int width, int height)
	{
		// Draw grid for debug
		const int small_step = 20;
		const int big_step = 100;

		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		glColor4f (1.0f, 1.0f, 1.0f, 0.1f);
		glLineStipple (1, 0xAAAA);
		for (int num = 1; num < width; num++)
		{
			int step = num * small_step;
			glBegin (GL_LINES);
				glVertex2i(0, step);
				glVertex2i(width, step);
			glEnd();

		}
		for (int num = 1; num < height; num++)
		{
			int step = num * small_step;
			glBegin (GL_LINES);
				glVertex2i(step, 0);
				glVertex2i(step, height);
			glEnd();
		}

		glColor4f (1.0f, 1.0f, 1.0f, 0.25f);
		glLineStipple (1, 0xAAAA);
		for (int num = 1; num < width; num++)
		{
			int step = num * big_step;
			glBegin (GL_LINES);
			glVertex2i(0, step);
			glVertex2i(width, step);
			glEnd();
		}

		for (int num = 1; num < height; num++)
		{
			int step = num * big_step;
			glBegin (GL_LINES);
				glVertex2i(step, 0);
				glVertex2i(step, height);
			glEnd();
		}

		glDisable(GL_LINE_STIPPLE);

	}
#endif

	void Interface::resizeEvent (int width, int height)
	{
		// TODO: resize all widgets/layouts in children
		size_.set_width(width);
		size_.set_height(height);
	}

}
