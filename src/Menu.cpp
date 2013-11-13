/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>

#include <BlendInt/Menu.hpp>

#include <iostream>

namespace BlendInt {

	Menu::Menu()
	: Widget(), m_select (0)
	{

	}

	Menu::Menu (const String& title, AbstractWidget* parent)
	: Widget(parent), m_select(0)
	{
	}

	Menu::~Menu ()
	{
		std::list<MenuItem*>::iterator it;

		for(it = m_list.begin(); it != m_list.end(); it++)
		{
			delete *it;
		}

		m_list.clear();
	}

	void Menu::add (const String& text)
	{
		MenuItem* item = new MenuItem(text);

		int w_max = std::max(size().width(), item->size().width());
		int h = 0;
		if(m_list.size()) {
			h = size().height();
		}

		resize(w_max, item->size().height() + h);
		m_list.push_back(item);
	}

	void Menu::move_mouse(MouseEvent* event)
	{
		if(!contain(event->position())) {
			if(m_select) m_select->set_highlight(false);
			m_select = 0;
			return;
		}

		int h = event->position().y();

		size_t index = h / (size().height() / m_list.size()) - 1;

		if(m_select) m_select->set_highlight(false);

		std::list<MenuItem*>::iterator it = m_list.begin();

		std::advance(it, index);

		m_select = *it;

		m_select->set_highlight(true);

		event->accept(this);
	}

	void Menu::render()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(),
					 position().y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// draw inner, simple fill
		glColor4ub(themes()->menu.inner.r(),
		        themes()->menu.inner.g(),
		        themes()->menu.inner.b(),
		        themes()->menu.inner.a());
		draw_gl_buffer(inner_buffer().get());

		// draw outline
		unsigned char tcol[4] = { themes()->menu.outline.r(),
		        themes()->menu.outline.g(),
		        themes()->menu.outline.b(),
		        themes()->menu.outline.a()};
		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		glColor4ubv(tcol);

		draw_gl_buffer_anti_alias(outer_buffer().get());

		if(emboss()) {
			glColor4f(1.0f, 1.0f, 1.0f, 0.02f);
			draw_gl_buffer_anti_alias(emboss_buffer().get());
		}

		glDisable(GL_BLEND);

		int h = 0;
		glTranslatef(0.0, size().height(), 0.0);

		std::list<MenuItem*>::iterator it;
		for (it = m_list.begin(); it != m_list.end(); it++) {
			h = - (*it)->size().height();
			glTranslatef(0.0, h - 2, 0.0);
			dispatch_render(*it);
		}

		glPopMatrix();
	}

}
