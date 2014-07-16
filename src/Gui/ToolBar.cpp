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
#include <BlendInt/Gui/ToolBar.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	ToolBar::ToolBar ()
	: AbstractDequeContainer(),
	  m_vao(0),
	  m_space(4),
	  m_move_status(false),
	  m_last_x(0),
	  m_start_x(0)
	{
		set_size(200, 32);
		set_margin(4, 4, 4, 4);	// the same as MenuBar
		set_scissor_test(true);

		InitializeToolBar();
	}

	ToolBar::~ToolBar ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void ToolBar::PushBack (AbstractWidget* widget)
	{
		int x = GetLastPosition();
		int y = position().y() + margin().bottom();
		int h = size().height() - margin().vsum();

		if(PushBackSubWidget(widget)) {

			Size prefer = widget->GetPreferredSize();

			SetSubWidgetPosition(widget, x, y);

			if(widget->IsExpandY()) {

				ResizeSubWidget(widget, prefer.width(), h);

			} else {

				if(widget->size().height() > h) {
					ResizeSubWidget(widget, prefer.width(), h);
				} else {
					ResizeSubWidget(widget, prefer.width(), widget->size().height());
					SetSubWidgetPosition(widget, x,
									y + (h - widget->size().height()) / 2);
				}

			}
		}
	}

	void ToolBar::PushBack (const RefPtr<Action>& action)
	{
		ToolButton* button = Manage(new ToolButton);
		int x = GetLastPosition();
		int y = position().y() + margin().bottom();
		int h = size().height() - margin().top() - margin().bottom();

		if(PushBackSubWidget(button)) {

			Size prefer = button->GetPreferredSize();

			SetSubWidgetPosition(button, x, y);

			if(button->IsExpandY()) {

				ResizeSubWidget(button, prefer.width(), h);

			} else {

				if(button->size().height() > h) {
					ResizeSubWidget(button, prefer.width(), h);
				} else {
					ResizeSubWidget(button, prefer.width(), button->size().height());
					SetSubWidgetPosition(button, x,
									y + (h - button->size().height()) / 2);
				}

			}
		}
	}

	Size ToolBar::GetPreferredSize () const
	{
		Size preferred_size;

		if(sub_widget_size() == 0) {

			Font font;	// Get default font height
			preferred_size.set_width(200);

			int max_font_height = font.GetHeight();

			preferred_size.set_height(max_font_height + margin().vsum());

		} else {

			AbstractWidget* widget = 0;
			Size tmp_size;

			preferred_size.set_width(-m_space);
			for(AbstractWidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
			{
				widget = *it;

				if(widget->visiable()) {
					tmp_size = widget->GetPreferredSize();

					preferred_size.add_width(tmp_size.width() + m_space);
					preferred_size.set_height(std::max(preferred_size.height(), tmp_size.height()));
				}
			}

			preferred_size.add_width(margin().left() + margin().right());
			preferred_size.add_height(margin().top() + margin().bottom());
		}

		return preferred_size;
	}

	bool ToolBar::IsExpandX() const
	{
		return true;
	}

	void ToolBar::UpdateContainer (const ContainerUpdateRequest& request)
	{
		switch (request.type()) {

			case ContainerMargin: {
				const Margin* margin_p =
				        static_cast<const Margin*>(request.data());

				int x = position().x() + margin_p->left();
				if (sub_widget_size()) {
					x = sub_widgets()->front()->position().x();
				}

				int y = position().y() + margin_p->bottom();
				int w = size().width() - margin_p->left() - margin_p->right();
				int h = size().height() - margin_p->top() - margin_p->bottom();

				FillSubWidgets(x, y, w, h, m_space);

				break;
			}

			default:
				ReportContainerUpdate(request);
				break;
		}
	}

	void ToolBar::PerformPositionUpdate (const PositionUpdateRequest& request)
	{
		if (request.target() == this) {
			int x = request.position()->x() - position().x();
			int y = request.position()->y() - position().y();

			set_position (*request.position());
			MoveSubWidgets(x, y);
		}

		ReportPositionUpdate(request);
	}

	void ToolBar::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {

			VertexTool tool;
			tool.Setup(*request.size(), 0, RoundNone, 0);
			m_inner->Bind();
			tool.SetInnerBufferData(m_inner.get());
			m_inner->Reset();

			int x = position().x() + margin().left();
			if (sub_widget_size()) {
				x = sub_widgets()->front()->position().x();
			}

			int y = position().y() + margin().bottom();
			int w = request.size()->width() - margin().hsum();
			int h = request.size()->height() - margin().vsum();

			FillSubWidgets(x, y, w, h, m_space);

			set_size(*request.size());

		} else if (request.target()->container() == this) {
			// if a sub widget changed its size, re-align all
			FillSubWidgets(position(), size(), margin(), m_space);
		}

		ReportSizeUpdate(request);
	}

	ResponseType ToolBar::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		RefPtr<GLSLProgram> program = Shaders::instance->default_triangle_program();
		program->Use();

		program->SetUniform3f("u_position", (float) position().x(), (float) position().y(), 0.f);
		program->SetUniform1i("u_gamma", 0);
		program->SetUniform1i("u_AA", 0);
		program->SetVertexAttrib4f("a_color", 0.447f, 0.447f, 0.447f, 1.0f);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);

		program->Reset();

		return Ignore;
	}

	ResponseType ToolBar::CursorEnterEvent (bool entered)
	{
		return Accept;
	}

	ResponseType ToolBar::KeyPressEvent (const KeyEvent& event)
	{
		return Accept;
	}

	ResponseType ToolBar::ContextMenuPressEvent (
					const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType ToolBar::ContextMenuReleaseEvent (
					const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType ToolBar::MousePressEvent (const MouseEvent& event)
	{
		if(event.button() == MouseButtonMiddle) {
			if(sub_widget_size()) {
				m_move_status = true;
				m_start_x = event.position().x();
				m_last_x = sub_widgets()->front()->position().x();
			}
		}

		return Ignore;
	}

	ResponseType ToolBar::MouseReleaseEvent (const MouseEvent& event)
	{
		if(m_move_status) {
			m_move_status = false;
			Refresh();
		}
		return Ignore;
	}

	ResponseType ToolBar::MouseMoveEvent (const MouseEvent& event)
	{
		if(m_move_status && sub_widget_size()) {
			int xmin = sub_widgets()->front()->position().x();
			int direction = event.position().x() - m_start_x;
			int offset = m_last_x + event.position().x() - m_start_x;
			int width = size().width() - margin().left() - margin().right();
			int left = position().x() + margin().left();
			int right = position().x() + size().width() - margin().right();

			int xmax = xmin - m_space;
			for(AbstractWidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
			{
				xmax += (*it)->size().width() + m_space;
			}

			DBG_PRINT_MSG("direction: %d, xmin: %d, xmax: %d, offset: %d, left: %d, right: %d", direction, xmin, xmax, offset, left, right);

			if((xmax - xmin) > width) {

				if(direction < 0) {	// left

					if(xmax > right) {
						int x = m_last_x;
						for(AbstractWidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
						{
							SetSubWidgetPosition(*it, x + event.position().x() - m_start_x, (*it)->position().y());
							x += (*it)->size().width() + m_space;
						}
					}

				} else if (direction > 0) {

					if(xmin < left) {
						int x = m_last_x;
						for(AbstractWidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
						{
							SetSubWidgetPosition(*it, x + event.position().x() - m_start_x, (*it)->position().y());
							x += (*it)->size().width() + m_space;
						}
					}

				}

			} else if((xmax - xmin) < width) {

				if(direction < 0) { // left
					if(xmin > left) {
						int x = m_last_x;
						for(AbstractWidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
						{
							SetSubWidgetPosition(*it, x + event.position().x() - m_start_x, (*it)->position().y());
							x += (*it)->size().width() + m_space;
						}
					}
				} else if (direction > 0) {	// right
					if(xmax < right) {
						int x = m_last_x;
						for(AbstractWidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
						{
							SetSubWidgetPosition(*it, x + event.position().x() - m_start_x, (*it)->position().y());
							x += (*it)->size().width() + m_space;
						}
					}
				}
			}

			Refresh();
		}

		return Ignore;
	}

	void ToolBar::InitializeToolBar ()
	{
		glGenVertexArrays(1, &m_vao);

		VertexTool tool;
		tool.Setup(size(), 0, RoundNone, 0);

		glBindVertexArray(m_vao);

		m_inner.reset(new GLArrayBuffer);
		m_inner->Generate();
		m_inner->Bind();

		tool.SetInnerBufferData(m_inner.get());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glBindVertexArray(0);
		GLArrayBuffer::Reset();
	}
	
	void ToolBar::RealignSubWidgets (const Size& size, const Margin& margin,
					int space)
	{
		int x = position().x();
		int y = position().y();
		int h = size.height() - margin.top() - margin.bottom();

		if(h < 0) {
			DBG_PRINT_MSG("Error: the geometry for sub widget is not valid, height: %d", h);
		}

		x += margin.left();
		y += margin.bottom();
		for(AbstractWidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			SetSubWidgetPosition(*it, x, y);
			ResizeSubWidget(*it, (*it)->size().width(), h);
			x += (*it)->size().width() + space;
		}
	}
	
	void ToolBar::FillSubWidgets (const Point& out_pos, const Size& out_size,
					const Margin& margin, int space)
	{
		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();
		int width = out_size.width() - margin.left() - margin.right();
		int height = out_size.height() - margin.top() - margin.bottom();

		FillSubWidgets (x, y, width, height, space);
	}
	
	void ToolBar::FillSubWidgets (int x, int y, int width,
					int height, int space)
	{
		AbstractWidget* widget = 0;
		for(AbstractWidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			widget = *it;
			SetSubWidgetPosition(widget, x, y);

			if(widget->IsExpandY()) {
				ResizeSubWidget(widget, widget->size().width(), height);
			} else {

				if(widget->size().height() > height) {
					ResizeSubWidget(widget, widget->size().width(), height);
				} else {
					SetSubWidgetPosition(widget, x,
									y + (height - widget->size().height()) / 2);
				}

			}

			x += widget->size().width() + space;
		}
	}

	int ToolBar::GetLastPosition () const
	{
		int x = position().x() + margin().left();

		if(sub_widget_size()) {
			x = sub_widgets()->back()->position().x();
			x += sub_widgets()->back()->size().width() + m_space;
		}

		return x;
	}

}
