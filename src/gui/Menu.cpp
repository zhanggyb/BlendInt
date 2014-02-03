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

#ifdef __UNIX__
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <BlendInt/Menu.hpp>
#include <BlendInt/Theme.hpp>
#include <BlendInt/FontCache.hpp>

#include <iostream>

namespace BlendInt {

	int Menu::DefaultMenuItemHeight = 20;

	Menu::Menu ()
	: RoundWidget(), m_highlight(0), m_inner_buffer(0), m_outer_buffer(0), m_highlight_buffer(0)
	{
		m_menubin.reset(new MenuItemBin);

		m_inner_buffer = new GLArrayBuffer;
		Retain(m_inner_buffer);
		m_outer_buffer = new GLArrayBuffer;
		Retain(m_outer_buffer);
		m_highlight_buffer = new GLArrayBuffer;
		Retain(m_highlight_buffer);

		set_size(20, 20);

		GenerateFormBuffer(&(size()), round_type(), radius(), m_inner_buffer, m_outer_buffer, 0);

		ResetHighlightBuffer(20);
	}

	Menu::Menu (AbstractWidget* parent)
	: RoundWidget(parent), m_highlight(0), m_inner_buffer(0), m_outer_buffer(0), m_highlight_buffer(0)
	{

		m_menubin.reset(new MenuItemBin);

		m_inner_buffer = new GLArrayBuffer;
		Retain(m_inner_buffer);
		m_outer_buffer = new GLArrayBuffer;
		Retain(m_outer_buffer);
		m_highlight_buffer = new GLArrayBuffer;
		Retain(m_highlight_buffer);

		set_size(20, 20);

		GenerateFormBuffer(&(size()), round_type(), radius(), m_inner_buffer, m_outer_buffer, 0);

		ResetHighlightBuffer(20);
	}

	Menu::~Menu ()
	{
		Destroy(m_inner_buffer);
		Destroy(m_outer_buffer);
		Destroy(m_highlight_buffer);
	}

	void Menu::SetTitle(const String& title)
	{
		m_menubin->set_title(title);
	}

	void Menu::AddMenuItem(const String& text)
	{
		m_menubin->Add(text);
		Resize(200, DefaultMenuItemHeight * m_menubin->size() + radius() * 2);
	}

	void Menu::AddMenuItem(Icon* icon, const String& text)
	{
		m_menubin->Add(icon, text);
		Resize(200, DefaultMenuItemHeight * m_menubin->size() + radius() * 2);
	}

	void Menu::AddMenuItem(Icon* icon, const String& text, const String& shortcut)
	{
		m_menubin->Add(icon, text, shortcut);
		Resize(200, DefaultMenuItemHeight * m_menubin->size() + radius() * 2);
	}

	void Menu::MouseMoveEvent(MouseEvent* event)
	{
		if(!contain(event->position())) {
			m_highlight = 0;
			return;
		}

		if(!m_menubin->size()) {
			m_highlight = 0;
			return;
		}

		m_highlight = GetHighlightNo(static_cast<int>(event->position().y()));

		event->accept(this);
	}

	void Menu::MousePressEvent (MouseEvent* event)
	{
		if(!contain(event->position())) {
			return;
		}

		if(!m_menubin->size()) {
			return;
		}

		event->accept(this);

		m_triggered.fire(m_menubin->GetMenuItem(m_highlight - 1));
	}

	void Menu::MouseReleaseEvent (MouseEvent* event)
	{
	}

	void Menu::Update(int type, const void* data)
	{
		switch (type) {

			case FormSize: {
				const Size* size_p = static_cast<const Size*>(data);
				GenerateFormBuffer(size_p, round_type(), radius(), m_inner_buffer, m_outer_buffer, 0);
				ResetHighlightBuffer(size_p->width());
				break;
			}

			case FormRoundType: {
				const int* type_p = static_cast<const int*>(data);
				GenerateFormBuffer(&(size()), *type_p, radius(), m_inner_buffer, m_outer_buffer, 0);
				break;
			}

			case FormRoundRadius: {
				const float* radius_p = static_cast<const float*>(data);
				GenerateFormBuffer(&(size()), round_type(), *radius_p, m_inner_buffer, m_outer_buffer, 0);
				break;
			}

			default:
				// Widget::Update(type, data);
				break;
		}
	}

	void Menu::Draw ()
	{
		//RoundWidget::Render();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// draw inner, simple fill
		glColor4ub(themes()->menu.inner.r(),
		        themes()->menu.inner.g(),
		        themes()->menu.inner.b(),
		        themes()->menu.inner.a());
		DrawInnerBuffer(m_inner_buffer);

		// draw outline
		unsigned char tcol[4] = { themes()->menu.outline.r(),
		        themes()->menu.outline.g(),
		        themes()->menu.outline.b(),
		        themes()->menu.outline.a()};
		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		glColor4ubv(tcol);

		DrawOutlineBuffer(m_outer_buffer);

		FontCache* fc = FontCache::create(Font("Sans"));

		/* Draw nomal menu item */
		int h = 0;
		glTranslatef(0.0, size().height() - radius(), 0.0);

		glColor4ub(225, 225, 225, 255);

		for (std::list<MenuItem*>::iterator it = m_menubin->list().begin(); it != m_menubin->list().end(); it++) {
			h = - DefaultMenuItemHeight;
			glTranslatef(0.0, h, 0.0);
			//DispatchRender(*it);
			//glRectf(0.0, 0.0, 200, DefaultMenuItemHeight);
			if((*it)->icon()) {
				DispatchRender ((*it)->icon());
			}
			fc->print(5 + 16, 5, (*it)->text());
			fc->print(120, 5, (*it)->shortcut());
		}

		/* draw highlight menu item */
		if(m_highlight) {
			glPopMatrix();
			glPushMatrix();
			glTranslatef(position().x(),
						 position().y() + size().height() - radius() - static_cast<float>(DefaultMenuItemHeight * m_highlight),
						 z());

//			glColor4ub(0, 0, 225, 25);
//			glRectf(0.0, 0.0, 200, DefaultMenuItemHeight);

			m_highlight_buffer->bind();

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);

			glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
			glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 6, BUFFER_OFFSET(2 * sizeof(GLfloat)));
			glDrawArrays(GL_POLYGON, 0, m_highlight_buffer->vertices());

			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);

			m_highlight_buffer->unbind();

			MenuItem* item = m_menubin->GetMenuItem(m_highlight - 1);

			if(item->icon()) {
				DispatchRender (item->icon());
			}
			fc->print(5 + 16, 5, item->text());
			fc->print(120, 5, item->shortcut());

		}

		glDisable(GL_BLEND);
	}

	void Menu::ResetHighlightBuffer (unsigned int width)
	{
		Size size(width, DefaultMenuItemHeight);

//		GenerateShadedFormBuffer (&size,
//				DefaultBorderWidth(),
//				RoundNone,
//				0.0,
//				&(themes()->menu_item),
//				Vertical,
//				m_highlight_buffer.get());

		GenerateShadedFormBuffer (&size,
				DefaultBorderWidth(),
				RoundNone,
				0.0,
				themes()->menu_item.inner_sel,
				themes()->menu_item.shadetop,
				themes()->menu_item.shadedown,
				Vertical,
				m_highlight_buffer);
	}

	unsigned int Menu::GetHighlightNo(int y)
	{
		int h = position().y() + size().height() - y;

		if(h < radius() || h > (size().height() - radius())) {
			return 0;
		}

		return (h - radius()) / (size().height() / m_menubin->size()) + 1;
	}

}

