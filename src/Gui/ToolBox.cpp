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
	: AbstractDequeContainer(),
	  m_vao(0),
	  m_space(1)
	{
		set_size(200, 400);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		VertexTool tool;
		tool.Setup(size(), 0, RoundNone, 0);
		m_inner = tool.GenerateInnerBuffer();
		glBindVertexArray(0);
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
			for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
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
	
	void ToolBox::UpdateContainer (const WidgetUpdateRequest& request)
	{
		switch(request.type()) {

			case ContainerMargin: {
				const Margin* margin_p = static_cast<const Margin*>(request.data());

				int x = position().x() + margin_p->left();
				int y = position().y() + margin_p->bottom();
				int w = size().width() - margin_p->hsum();
				int h = size().height() - margin_p->vsum();

				FillSubWidgets(x, y, w, h, m_space);

				break;
			}

			default: {
				ReportUpdateRequest(request);
				break;
			}
		}
	}
	
	bool ToolBox::UpdateGeometryTest(const WidgetUpdateRequest& request)
	{
		if(request.target() == this) {
			return true;
		} else if(request.target()->container() == this) {
			// A sub widget want to change it's geometry

			switch (request.type()) {

				case WidgetPosition: {
					return false;
				}

				case WidgetSize: {

					return true;
				}

				default: {
					return true;
				}
			}
		} else {
			return true;
		}
	}

	void ToolBox::UpdateGeometry (const WidgetUpdateRequest& request)
	{
		if(request.target() == this) {

			switch (request.type()) {

				case WidgetPosition: {

					const Point* pos_p = static_cast<const Point*>(request.data());

					int x = pos_p->x() - position().x();
					int y = pos_p->y() - position().y();

					MoveSubWidgets(x, y);

					break;
				}

				case WidgetSize: {
					const Size* size_p = static_cast<const Size*>(request.data());

					VertexTool tool;
					tool.Setup(*size_p, 0, RoundNone, 0);
					tool.UpdateInnerBuffer(m_inner.get());

					int x = position().x() + margin().left();
					int y = position().y() + margin().bottom();
					int w = size_p->width() - margin().hsum();
					int h = size_p->height() - margin().vsum();

					FillSubWidgets(x, y, w, h, m_space);
					break;
				}

				default:
					break;
			}

		} else if (request.target()->container() == this) {

			switch (request.type()) {

				case WidgetSize: {
					// a sub widget changed its size
					FillSubWidgets(position(), size(), margin(), m_space);

					break;
				}

				default:
					break;

			}

		}
	}
	
	ResponseType ToolBox::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		RefPtr<GLSLProgram> program = Shaders::instance->default_triangle_program();

		glBindVertexArray(m_vao);

		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetUniform1i("AA", 0);
		program->SetVertexAttrib4f("Color", 0.447f, 0.447f, 0.447f, 1.f);
		program->SetUniform1i("Gamma", 0);

		glEnableVertexAttribArray(0);
		m_inner->Bind();
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							m_inner->GetBufferSize()
											/ (2 * sizeof(GLfloat)));
		m_inner->Reset();
		glDisableVertexAttribArray(0);

		program->Reset();
		glBindVertexArray(0);
		return Accept;
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
			y = sub_widgets()->back()->position().y();
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
		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
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
