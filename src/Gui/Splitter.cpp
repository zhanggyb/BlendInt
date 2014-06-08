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
#include <glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <boost/smart_ptr.hpp>

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>
#include <BlendInt/Gui/Splitter.hpp>
#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

	SplitterHandle::SplitterHandle (Orientation orientation)
	: Widget(),
	  m_orientation(orientation),
	  m_vao(0),
	  m_highlight(false),
	  m_pressed(false),
	  m_prev_size(0),
	  m_next_size(0),
	  m_nearby_pos(0),
	  m_prev_widget(0),
	  m_next_widget(0)
	{
		if(orientation == Horizontal) {
			set_size(200, 3);
		} else {
			set_size(3, 200);
		}

		glGenVertexArrays(1, &m_vao);
		m_buffer.reset(new GLArrayBuffer);

		glBindVertexArray(m_vao);

		m_buffer->Generate();
		m_buffer->Bind();

		std::vector<GLfloat> vertices(4, 0);

		if(orientation == Horizontal) {
			vertices[2] = 200.f;

		} else {
			vertices[3] = 200.f;
		}

		m_buffer->SetData(sizeof(GLfloat) * 4, &vertices[0], GL_STATIC_DRAW);
		m_buffer->Reset();

		glBindVertexArray(0);
	}

	SplitterHandle::~SplitterHandle ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	Size SplitterHandle::GetPreferredSize () const
	{
		Size preferred_size(3, 3);

		if(m_orientation == Horizontal) {
			preferred_size.set_width(200);
		} else {
			preferred_size.set_height(200);
		}

		return preferred_size;
	}

	bool SplitterHandle::IsExpandX () const
	{
		if(m_orientation == Horizontal) {
			return true;
		} else {
			return false;
		}
	}

	bool SplitterHandle::IsExpandY () const
	{
		if(m_orientation == Vertical) {
			return true;
		} else {
			return false;
		}
	}

	void SplitterHandle::UpdateGeometry (const WidgetUpdateRequest& request)
	{
		switch (request.type()) {

			case WidgetSize: {

				const Size* size_p = static_cast<const Size*>(request.data());

				std::vector<GLfloat> vertices(4, 0);

				if (m_orientation == Horizontal) {
					vertices[2] = size_p->width();

				} else {
					vertices[3] = size_p->height();
				}

				m_buffer->Bind();
				m_buffer->SetData(sizeof(GLfloat) * 4, &vertices[0],
								GL_STATIC_DRAW);
				m_buffer->Reset();

				break;
			}

			default:
				break;
		}

	}

	ResponseType SplitterHandle::Draw (const RedrawEvent& event)
	{
		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program = ShaderManager::instance->default_line_program();
		program->Use();

		program->SetUniform1i("AA", 0);
		if(m_highlight) {
			program->SetUniform1i("Gamma", 20);
		} else {
			program->SetUniform1i("Gamma", 0);
		}

		glm::vec4 color;
		glm::mat4 local;

		glEnableVertexAttribArray(0);	// 0 is the locaiton in shader
		m_buffer->Bind();
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		if(m_orientation == Horizontal) {

			for(int i = 0; i < 3; i++) {
				local = glm::translate(mvp, glm::vec3(0.f, i + 1, 0.f));

				if((i % 2) == 0) {
					color.r = 0.4; color.g = 0.4; color.b = 0.4; color.a = 0.9;
				} else {
					color.r = 0.2; color.g = 0.2; color.b = 0.2; color.a = 1.0;
				}

				program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(local));
				program->SetVertexAttrib4fv("Color", glm::value_ptr(color));
				glDrawArrays(GL_LINES, 0, 2);

			}

		} else {

			for(int i = 0; i < 3; i++) {
				local = glm::translate(mvp, glm::vec3(i + 1, 0.f, 0.f));

				if((i % 2) == 0) {
					color.r = 0.4; color.g = 0.4; color.b = 0.4; color.a = 0.9;
				} else {
					color.r = 0.2; color.g = 0.2; color.b = 0.2; color.a = 1.0;
				}

				program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(local));
				program->SetVertexAttrib4fv("Color", glm::value_ptr(color));
				glDrawArrays(GL_LINES, 0, 2);
			}

		}

		m_buffer->Reset();

		glDisableVertexAttribArray(0);

		program->Reset();

		glBindVertexArray(0);

		return Accept;
	}

	ResponseType SplitterHandle::CursorEnterEvent (bool entered)
	{
		if(entered) {
			m_highlight = true;
		} else {
			m_highlight = false;
		}

		Refresh();
		return Accept;
	}

	ResponseType SplitterHandle::MousePressEvent (const MouseEvent& event)
	{
		m_last = position();
		m_cursor = event.position();
		m_pressed = true;

		if(m_orientation == Horizontal) {
			m_prev_size = m_prev_widget->size().height();
			m_next_size = m_next_widget->size().height();
			m_nearby_pos = m_prev_widget->position().y();
		} else {
			m_prev_size = m_prev_widget->size().width();
			m_next_size = m_next_widget->size().width();
			m_nearby_pos = m_next_widget->position().x();
		}

		return Accept;
	}

	ResponseType SplitterHandle::MouseReleaseEvent (const MouseEvent& event)
	{
		m_pressed = false;

		return Accept;
	}

	ResponseType SplitterHandle::MouseMoveEvent (const MouseEvent& event)
	{
		if(m_pressed) {

			Splitter* splitter = dynamic_cast<Splitter*>(container());

			if(m_orientation == Horizontal) {

				int offset = event.position().y() - m_cursor.y();
				int oy1 = m_prev_size - offset;
				int oy2 = m_next_size + offset;

				if((oy1 < 0) || (oy2 < 0)) {
					return Ignore;
				}

				splitter->SetSubWidgetPosition(this, m_last.x(), m_last.y() + offset);

				splitter->ResizeSubWidget(m_prev_widget, m_prev_widget->size().width(), oy1);
				splitter->SetSubWidgetPosition(m_prev_widget, m_prev_widget->position().x(), m_nearby_pos + offset);
				splitter->ResizeSubWidget(m_next_widget, m_next_widget->size().width(), oy2);

			} else {

				int offset = event.position().x() - m_cursor.x();
				int oy1 = m_prev_size + offset;
				int oy2 = m_next_size - offset;

				if((oy1 < 0) || (oy2 < 0)) {
					return Ignore;
				}

				splitter->SetSubWidgetPosition(this, m_last.x() + offset, m_last.y());

				splitter->ResizeSubWidget(m_prev_widget, oy1, m_prev_widget->size().height());
				splitter->ResizeSubWidget(m_next_widget, oy2, m_next_widget->size().height());
				splitter->SetSubWidgetPosition(m_next_widget, m_nearby_pos + offset, m_next_widget->position().y());

			}

			Refresh();
			return Accept;
		}
		return Ignore;
	}

	Splitter::Splitter(Orientation orientation)
	: AbstractDequeContainer(),
	  m_orientation(orientation)
	{
		set_size(400, 400);
	}

	Splitter::~Splitter ()
	{

	}
	
	void Splitter::PushFront (AbstractWidget* widget)
	{
		if(widget && widget->container() != this) {

			if(sub_widget_size() == 0) {
				PushFrontSubWidget(widget);
			} else {
				SplitterHandle* handle = 0;
				if(m_orientation == Horizontal) {
					handle = Manage(new SplitterHandle(Vertical));
				} else {
					handle = Manage(new SplitterHandle(Horizontal));
				}

				AbstractWidget* first = sub_widgets()->at(0);
				PushFrontSubWidget(handle);
				handle->m_prev_widget = widget;
				handle->m_next_widget = first;

				PushFrontSubWidget(widget);
			}

			AlignSubWidgets(m_orientation, size(), margin());
		}
	}

	void Splitter::PushBack (AbstractWidget* widget)
	{
		if(widget && widget->container() != this) {

			if(sub_widget_size() == 0) {
				PushBackSubWidget(widget);
			} else {
				SplitterHandle* handle = 0;
				if(m_orientation == Horizontal) {
					handle = Manage(new SplitterHandle(Vertical));
				} else {
					handle = Manage(new SplitterHandle(Horizontal));
				}

				AbstractWidget* last = sub_widgets()->at(sub_widget_size() - 1);
				PushBackSubWidget(handle);
				handle->m_prev_widget = last;
				handle->m_next_widget = widget;

				PushBackSubWidget(widget);
			}

			FillSubWidgetsInSplitter(position(), size(), margin(), m_orientation);
		}
	}

	void Splitter::Remove (AbstractWidget* widget)
	{
		if(RemoveSubWidget(widget)) {
			if(m_orientation == Horizontal) {
				FillSubWidgetsInSplitter(position(), size(), margin(), m_orientation);
			} else {
				FillSubWidgetsInSplitter(position(), size(), margin(), m_orientation);
			}
		}
	}

	Size Splitter::GetPreferredSize() const
	{
		Size preferred_size;

		if(sub_widget_size() == 0) {
			preferred_size.set_width(400);
			preferred_size.set_height(400);
		} else {
			AbstractWidget* widget = 0;
			Size tmp;

			if (m_orientation == Horizontal) {
				for (WidgetDeque::iterator it = sub_widgets()->begin();
								it != sub_widgets()->end(); it++) {
					widget = *it;
					if (widget->visiable()) {
						tmp = widget->GetPreferredSize();
						preferred_size.add_width(tmp.width());
						preferred_size.set_height(
										std::max(preferred_size.height(),
														tmp.height()));
					}
				}
			} else {
				for(WidgetDeque::iterator it = sub_widgets()->begin();
								it != sub_widgets()->end(); it++) {
					widget = *it;
					if(widget->visiable()) {
						tmp = widget->GetPreferredSize();
						preferred_size.add_height(tmp.height());
						preferred_size.set_width(
										std::max(preferred_size.width(), tmp.width()));
					}
				}
			}

			preferred_size.add_width(margin().hsum());
			preferred_size.add_height(margin().vsum());
		}

		return preferred_size;
	}

	bool Splitter::IsExpandX() const
	{
		return true;
	}

	bool Splitter::IsExpandY() const
	{
		return true;
	}


	int Splitter::GetWidgetIndex (AbstractWidget* widget) const
	{
		int index = 0;
		if(widget->container() != this) return -1;

		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end();)
		{
			if(*it == widget) break;

			index++;
			it += 2;
		}

		return index;
	}

	int Splitter::GetHandleIndex (SplitterHandle* handle) const
	{
		int index = 0;
		if(handle->container() != this) return -1;

		for(WidgetDeque::iterator it = (sub_widgets()->begin() + 1); it != sub_widgets()->end();)
		{
			if(*it == handle) break;

			index++;
			it += 2;
		}

		return index;
	}

	AbstractWidget* Splitter::GetWidget (int index) const
	{
		if(sub_widget_size() == 0) return 0;

		int max = (sub_widget_size() + 1) / 2;
		if(index > max) return 0;

		index = index * 2;

		return sub_widgets()->at(index);
	}

	SplitterHandle* Splitter::GetHandle (int index) const
	{
		if(sub_widget_size() <= 1) return 0;

		int max = (sub_widget_size() - 1) / 2;
		if(index > max) return 0;

		index = index * 2 + 1;

		return dynamic_cast<SplitterHandle*>(sub_widgets()->at(index));
	}

	int Splitter::GetWidgetCount () const
	{
		return (sub_widget_size() / 2 + 1);
	}

	void Splitter::MoveHandle (int index, int x, int y)
	{
	}

	void Splitter::UpdateContainer(const WidgetUpdateRequest& request)
	{
		switch(request.type()) {

			case ContainerMargin: {
				const Margin* margin_p = static_cast<const Margin*>(request.data());
				FillSubWidgetsInSplitter(position(), size(), *margin_p, m_orientation);
				break;
			}

			case ContainerRefresh: {
				Refresh();
				break;
			}

			default:
				break;
		}
	}

	bool Splitter::UpdateGeometryTest (const WidgetUpdateRequest& request)
	{
		if(request.source() == this) {

			return AbstractDequeContainer::UpdateGeometryTest(request);

		} else if (request.source() == container()) {

			return true;

		} else {	// called by sub widget

			switch(request.type()) {
				case WidgetSize:
					return false;

				case WidgetPosition: {
					return false;
				}

				default:
					return false;
			}
		}
	}
	
	void Splitter::UpdateGeometry (const WidgetUpdateRequest& request)
	{
		switch (request.type()) {

			case WidgetSize: {

				const Size* size_p = static_cast<const Size*>(request.data());
				FillSubWidgetsInSplitter(position(), *size_p, margin(), m_orientation);
				break;
			}

			case WidgetPosition: {

				const Point* pos_p = static_cast<const Point*>(request.data());

				int x = pos_p->x() - position().x();
				int y = pos_p->y() - position().y();

				MoveSubWidgets(x, y);

				break;
			}

			default:
				break;

		}
	}

	ResponseType Splitter::Draw (const RedrawEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType Splitter::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Splitter::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Splitter::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Splitter::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Splitter::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Splitter::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Splitter::MouseMoveEvent (const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}
	
	void Splitter::AlignSubWidgets (Orientation orienation, const Size& size, const Margin& margin)
	{
		int room = GetAverageRoom(orienation, size, margin);
		int x = position().x() + margin.left();

		if(orienation == Horizontal) {

			int y = position().y() + margin.bottom();
			int h = size.height() - margin.vsum();

			int i = 0;
			for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
			{
				if(i % 2 == 0) {
					ResizeSubWidget(*it, room, h);
					SetSubWidgetPosition(*it, x, y);
					x = x + room;
				} else {
					ResizeSubWidget(*it, 3, h);
					SetSubWidgetPosition(*it, x, y);
					x = x + 3;
				}
				i++;
			}

		} else {

			int y = position().y() + size.height() - margin.top();
			int w = size.width() - margin.hsum();

			int i = 0;
			for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
			{
				if(i % 2 == 0) {
					y = y - room;
					ResizeSubWidget(*it, w, room);
					SetSubWidgetPosition(*it, x, y);
				} else {
					y = y - 3;
					ResizeSubWidget(*it, w, 3);
					SetSubWidgetPosition(*it, x, y);
				}

				i++;
			}

		}
	}

	int Splitter::GetAverageRoom (Orientation orientation, const Size& size, const Margin& margin)
	{
		int room = 0;

		if(orientation == Horizontal) {
			room = size.width() - margin.hsum();
		} else {
			room = size.height() - margin.vsum();
		}

		if(sub_widget_size() == 0) {
			return room;
		}

		Size prefer;
		int space = 0;
		WidgetDeque::iterator it = sub_widgets()->begin();
		it++;
		while (it != sub_widgets()->end()) {
			prefer = (*it)->GetPreferredSize();
			if(orientation == Horizontal) {
				space = prefer.width();
			} else {
				space = prefer.height();
			}

			it = it + 2;
		}

		room = room - space;
		room = room / ((sub_widget_size() + 1) / 2);

		return room;
	}

	void Splitter::FillSubWidgetsInSplitter (const Point& out_pos,
	        const Size& out_size, const Margin& margin, Orientation orientation)
	{
		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();
		int width = out_size.width() - margin.left() - margin.right();
		int height = out_size.height() - margin.top() - margin.bottom();

		FillSubWidgetsInSplitter(x, y, width, height, orientation);
	}

	void Splitter::FillSubWidgetsInSplitter (const Point& pos, const Size& size,
			Orientation orientation)
	{
		FillSubWidgetsInSplitter(pos.x(), pos.y(), size.width(), size.height(), orientation);
	}

	void Splitter::FillSubWidgetsInSplitter (int x, int y, int width,
	        int height, Orientation orientation)
	{
		if(orientation == Horizontal) {

			DistributeHorizontally(x, width);
			AlignHorizontally(y, height);

		} else {

			DistributeVertically(y, height);
			AlignVertically(x, width);

		}
	}

	void Splitter::DistributeHorizontally (int x, int width)
	{
		boost::scoped_ptr<std::deque<int> > expandable_widths(new std::deque<int>);
		boost::scoped_ptr<std::deque<int> > unexpandable_widths(new std::deque<int>);

		int expandable_width_sum = 0;	// the width sum of the expandable widgets' size
		int unexpandable_width_sum = 0;	// the width sum of the unexpandable widgets' size
		int handlers_width_sum = 0;

		AbstractWidget* widget = 0;
		int i = 0;
		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			widget = *it;

			if(i % 2 == 0) {	// widgets

				if (widget->visiable()) {

					if(widget->IsExpandX()) {
						expandable_width_sum += widget->size().width();
						expandable_widths->push_back(widget->size().width());
					} else {
						unexpandable_width_sum += widget->size().width();
						unexpandable_widths->push_back(widget->size().width());
					}

				}

			} else {	// handlers

				if(widget->visiable()) {
					handlers_width_sum += widget->size().width();
				}

			}

			i++;
		}

		if((expandable_widths->size() + unexpandable_widths->size()) == 0) return;	// do nothing if all sub widgets are invisible

		if(expandable_widths->size() == 0) {
			// TODO: if no expandable sub widgets, resize the unexpandable ones based on the current size
		} else if(unexpandable_widths->size() == 0) {
			// TODO: if no unexpandable sub widgets, resize the expandable ones based on the current size
		} else {
			// TODO: if there're both expandable and unexpandable sub widgets, only resize expandable widgets.
		}

	}

	void Splitter::DistributeVertically (int y, int height)
	{
	}

	void Splitter::AlignHorizontally (int y, int height)
	{
	}

	void Splitter::AlignVertically (int x, int width)
	{
	}

	int Splitter::GetWidgetsRoom (Orientation orientation, const Size& size,
					const Margin& margin)
	{
		int room = 0;

		if(orientation == Horizontal) {
			room = size.width() - margin.hsum();
		} else {
			room = size.height() - margin.vsum();
		}

		if(sub_widget_size() == 0) {
			return room;
		}

		Size prefer;
		int space = 0;
		WidgetDeque::iterator it = sub_widgets()->begin();
		it++;
		while (it != sub_widgets()->end()) {
			prefer = (*it)->GetPreferredSize();
			if(orientation == Horizontal) {
				space = prefer.width();
			} else {
				space = prefer.height();
			}

			it = it + 2;
		}

		room = room - space;

		return room;
	}

}
