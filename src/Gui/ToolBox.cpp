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
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif	// __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/ToolBox.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	ToolBox::ToolBox()
	: AbstractContainer(),
	  m_vao(0),
	  m_space(1)
	{
		set_size(200, 400);

		VertexTool tool;
		tool.Setup(size(), 0, RoundNone, 0);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		m_inner.reset(new GLArrayBuffer);
		m_inner->Generate();
		m_inner->Bind();

		tool.SetInnerBufferData(m_inner.get());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::Reset();
	}

	ToolBox::~ToolBox()
	{
		glDeleteVertexArrays(1, &m_vao);
	}
	
	void ToolBox::PushBack (AbstractWidget* widget)
	{
		int x = position().x() + margin().left();
		int y = GetLastPosition();
		int w = size().width() - margin().hsum();

		if(PushBackSubWidget(widget)) {

			Size prefer = widget->GetPreferredSize();

			y = y - prefer.height();

			SetSubWidgetPosition(widget, x, y);

			if(widget->IsExpandX()) {

				ResizeSubWidget(widget, w, prefer.height());

			} else {

				if(widget->size().width() > w) {
					ResizeSubWidget(widget, w, prefer.height());
				} else {
					ResizeSubWidget(widget, widget->size().width(), prefer.height());
				}
			}

			DisableShadow(widget);
		}
	}
	
	bool ToolBox::IsExpandY () const
	{
		return true;
	}
	
	Size ToolBox::GetPreferredSize () const
	{
		Size preferred_size;

		if(sub_widget_size() == 0) {

			preferred_size.set_width(200);
			preferred_size.set_height(400);

		} else {

			AbstractWidget* widget = 0;
			Size tmp_size;

			preferred_size.set_height(-m_space);
			for(AbstractWidgetDeque::const_iterator it = deque().begin(); it != deque().end(); it++)
			{
				widget = *it;

				if(widget->visiable()) {
					tmp_size = widget->GetPreferredSize();

					preferred_size.add_height(tmp_size.height() + m_space);
					preferred_size.set_width(std::max(preferred_size.width(), tmp_size.width()));
				}
			}

			preferred_size.add_width(margin().hsum());
			preferred_size.add_height(margin().vsum());
		}

		return preferred_size;
	}

	void ToolBox::PerformMarginUpdate (const Margin& request)
	{
		int x = position().x() + request.left();
		int y = position().y() + request.bottom();
		int w = size().width() - request.hsum();
		int h = size().height() - request.vsum();

		FillSubWidgets(x, y, w, h, m_space);
	}
	
	void ToolBox::PerformPositionUpdate (const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			int x = request.position()->x() - position().x();
			int y = request.position()->y() - position().y();

			set_position(*request.position());
			MoveSubWidgets(x, y);
		}

		ReportPositionUpdate(request);
	}

	void ToolBox::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.Setup(*request.size(), 0, RoundNone, 0);
			tool.UpdateInnerBuffer(m_inner.get());

			int x = position().x() + margin().left();
			int y = position().y() + margin().bottom();
			int w = request.size()->width() - margin().hsum();
			int h = request.size()->height() - margin().vsum();

			FillSubWidgets(x, y, w, h, m_space);

			set_size(*request.size());

		} else if (request.target()->container() == this) {
			FillSubWidgets(position(), size(), margin(), m_space);
		}

		ReportSizeUpdate(request);
	}

	ResponseType ToolBox::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		RefPtr<GLSLProgram> program = Shaders::instance->default_triangle_program();
		program->Use();

		program->SetUniform3f("u_position", (float) position().x(), (float) position().y(), 0.f);
		program->SetUniform1i("u_gamma", 0);
		program->SetUniform1i("u_AA", 0);
		program->SetVertexAttrib4f("a_color", 0.447f, 0.447f, 0.447f, 1.f);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);

		program->Reset();

		return Ignore;
	}

	ResponseType ToolBox::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType ToolBox::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType ToolBox::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ToolBox::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ToolBox::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType ToolBox::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType ToolBox::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	int ToolBox::GetLastPosition () const
	{
		int y = position().y() + size().height() - margin().top();

		if(sub_widget_size()) {
			y = deque().back()->position().y();
			y -= m_space;
		}

		return y;
	}

	void ToolBox::FillSubWidgets (const Point& out_pos, const Size& out_size,
					const Margin& margin, int space)
	{
		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();
		int width = out_size.width() - margin.hsum();
		int height = out_size.height() - margin.vsum();

		FillSubWidgets (x, y, width, height, space);
	}

	void ToolBox::FillSubWidgets (int x, int y, int width, int height,
					int space)
	{
		y = y + height + space;

		AbstractWidget* widget = 0;
		for(AbstractWidgetDeque::const_iterator it = deque().begin(); it != deque().end(); it++)
		{
			widget = *it;

			y = y - widget->size().height() - space;

			SetSubWidgetPosition(widget, x, y);

			if(widget->IsExpandX()) {
				ResizeSubWidget(widget, width, widget->size().height());
			} else {

				if(widget->size().width() > width) {
					ResizeSubWidget(widget, width, widget->size().height());
				} else {
					//SetSubWidgetPosition(widget, x,
					//				y + (height - widget->size().height()) / 2);
				}

			}
		}
	}

}
