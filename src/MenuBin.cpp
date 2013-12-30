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

#include <BlendInt/MenuBin.hpp>
#include <BlendInt/Theme.hpp>

#include <iostream>

namespace BlendInt {

	int MenuBin::DefaultMenuItemHeight = 20;

	MenuBin::MenuBin ()
	: RoundWidget(), m_highlight(0)
	{
		m_menu.reset(new Menu);
		m_buffer.reset(new GLBufferMultiple);
		m_highlight_buffer.reset(new GLBufferSimple);

		set_size(20, 20);

		GenerateFormBuffer(&(size()), false, round_type(), radius(), m_buffer.get());

		ResetHighlightBuffer(20);
	}

	MenuBin::MenuBin (AbstractWidget* parent)
	: RoundWidget(parent), m_highlight(0)
	{
		m_menu.reset(new Menu);
		m_buffer.reset(new GLBufferMultiple);
		m_highlight_buffer.reset(new GLBufferSimple);

		set_size(20, 20);

		GenerateFormBuffer(&(size()), false, round_type(), radius(), m_buffer.get());

		ResetHighlightBuffer(20);
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
			m_highlight = 0;
			return;
		}

		if(!m_menu->size()) {
			m_highlight = 0;
			return;
		}

		m_highlight = GetHighlightNo(static_cast<int>(event->position().y()));

		event->accept(this);
	}

	void MenuBin::Update(int type, const void* data)
	{
		switch (type) {

			case FormSize: {
				const Size* size_p = static_cast<const Size*>(data);
				GenerateFormBuffer(size_p, false, round_type(), radius(), m_buffer.get());
				ResetHighlightBuffer(size_p->width());
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
				// Widget::Update(type, data);
				break;
		}
	}

	void MenuBin::Draw ()
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

//		int h = 0;
//		glTranslatef(0.0, size().height() - radius(), 0.0);
//
//		glColor4ub(0, 0, 225, 25);
//
//		for (std::list<MenuItem*>::iterator it = m_menu->list().begin(); it != m_menu->list().end(); it++) {
//			h = - DefaultMenuItemHeight;
//			glTranslatef(0.0, h, 0.0);
//			//DispatchRender(*it);
//			glRectf(0.0, 0.0, 200, DefaultMenuItemHeight);
//		}

		if(m_highlight) {
			glPopMatrix();
			glPushMatrix();
			glTranslatef(position().x(),
						 position().y() + size().height() - radius() - static_cast<float>(DefaultMenuItemHeight * m_highlight),
						 z());

//			glColor4ub(0, 0, 225, 25);
//			glRectf(0.0, 0.0, 200, DefaultMenuItemHeight);

			m_highlight_buffer->select(0);
			m_highlight_buffer->Bind();

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);

			glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
			glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 6, BUFFER_OFFSET(2 * sizeof(GLfloat)));
			glDrawArrays(GL_POLYGON, 0, m_highlight_buffer->Vertices());

			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);


			m_highlight_buffer->Unbind();
		}

		glDisable(GL_BLEND);

		glPopMatrix();
	}

	void MenuBin::ResetHighlightBuffer (unsigned int width)
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
				m_highlight_buffer.get());
	}

	unsigned int MenuBin::GetHighlightNo(int y)
	{
		int h = position().y() + size().height() - y;

		if(h < radius() || h > (size().height() - radius())) {
			return 0;
		}

		return (h - radius()) / (size().height() / m_menu->size()) + 1;
	}

}

