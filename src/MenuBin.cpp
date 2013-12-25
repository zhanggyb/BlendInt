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
#include <BlendInt/MenuBin.hpp>

namespace BlendInt {

	int MenuBin::DefaultMenuItemHeight = 20;

	MenuBin::MenuBin ()
	: RoundWidget()
	{
		m_buffer.reset(new GLBuffer);
		m_menu.reset(new Menu);

		set_size(20, 20);

		GenerateRectFormBuffer(&size(), false, m_buffer.get());
	}

	MenuBin::MenuBin (AbstractWidget* parent)
	: RoundWidget(parent)
	{
		m_buffer.reset(new GLBuffer);
		set_size(20, 20);

		GenerateRectFormBuffer(&size(), false, m_buffer.get());
	}

	MenuBin::~MenuBin ()
	{
	}

	void MenuBin::SetTitle(const String& title)
	{
		m_menu->set_title(title);
	}

	void MenuBin::AddMenuItem(const String& text)
	{
		m_menu->Add(text);
		Resize(200, DefaultMenuItemHeight * m_menu->size() + radius() * 2);
	}

	void MenuBin::MouseMoveEvent(MouseEvent* event)
	{
		if(!contain(event->position())) {
			//if(m_select) m_select->set_highlight(false);
			//m_select = 0;
			return;
		}

		// int h = position().y() + size().height() - event->position().y();

		//size_t index = h / (size().height() / m_list.size());

		//if(m_select) m_select->set_highlight(false);

		//std::list<MenuItem*>::iterator it = m_list.begin();
		//std::advance(it, index);

		//m_select = *it;
		//m_select->set_highlight(true);
		event->accept(this);
	}

	void MenuBin::Update(int type, const void* data)
	{
		switch (type) {

			case FormSize: {
				const Size* size_p = static_cast<const Size*>(data);
				GenerateRectFormBuffer(size_p, false, m_buffer.get());
				break;
			}

			case FormRoundType: {
				const int* type_p = static_cast<const int*>(data);
				GenerateFormBuffer(&(size()), false, *type_p, radius(), m_buffer.get());
				break;
			}

			case FormRoundRadius: {
				const float* radius_p = static_cast<const float*>(data);
				GenerateFormBuffer(&(size()), false, round_type(), *radius_p, m_buffer.get());
				break;
			}

			default:
				Widget::Update(type, data);
				break;
		}
	}

	void MenuBin::Render ()
	{
		//RoundWidget::Render();

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
		draw_inner_buffer(m_buffer.get(), 0);

		// draw outline
		unsigned char tcol[4] = { themes()->menu.outline.r(),
		        themes()->menu.outline.g(),
		        themes()->menu.outline.b(),
		        themes()->menu.outline.a()};
		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		glColor4ubv(tcol);

		draw_outline_buffer(m_buffer.get(), 1);

		glDisable(GL_BLEND);

		//int h = 0;
		glTranslatef(0.0, size().height(), 0.0);

		std::list<MenuItem*>::iterator it;
		for (it = m_menu->list().begin(); it != m_menu->list().end(); it++) {
			//h = - DefaultMenuItemHeight;
			//glTranslatef(0.0, h, 0.0);
			//DispatchRender(*it);
		}

		glPopMatrix();
	}

}

