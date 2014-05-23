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

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>
#include <BlendInt/Gui/Splitter.hpp>
#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

	SplitterHandle::SplitterHandle (Orientation orientation)
	: Widget(), m_orientation(orientation), m_vao(0), m_highlight(false), m_pressed(false)
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

	void SplitterHandle::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch (request.type()) {

				case FormSize: {

					const Size* size_p = static_cast<const Size*>(request.data());

					std::vector<GLfloat> vertices(4, 0);

					if(m_orientation == Horizontal) {
						vertices[2] = size_p->width();

					} else {
						vertices[3] = size_p->height();
					}

					m_buffer->Bind();
					m_buffer->SetData(sizeof(GLfloat) * 4, &vertices[0], GL_STATIC_DRAW);
					m_buffer->Reset();

					break;
				}

				default:
					break;
			}

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
		glVertexAttribPointer(
						0, // attribute
						2,		// number of elements per vertex, here (x,y)
						GL_FLOAT,	// the type of each element
						GL_FALSE,	// take our values as-is
						0,		// no extra data between each position
						BUFFER_OFFSET(0)	// the first element
		);

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

			if(m_orientation == Horizontal) {

				int offset = event.position().y() - m_cursor.y();
				SetPosition(m_last.x(), m_last.y() + offset);

			} else {

				int offset = event.position().x() - m_cursor.x();
				SetPosition(m_last.x() + offset, m_last.y());
			}

			Refresh();
			return Accept;
		}
		return Ignore;
	}

	Splitter::Splitter(Orientation orientation)
	: AbstractDequeContainer(),
	  m_orientation(orientation),
	  m_space(2)
	{
		set_size(400, 400);
	}

	Splitter::~Splitter ()
	{

	}
	
	void Splitter::Add (AbstractWidget* widget)
	{
		if(widget && widget->container() != this) {

			if(sub_widget_size()== 0) {
				PushBackSubWidget(widget);
			} else {
				SplitterHandle* handle = 0;
				if(m_orientation == Horizontal) {
					handle = Manage(new SplitterHandle(Vertical));
				} else {
					handle = Manage(new SplitterHandle(Horizontal));
				}

				PushBackSubWidget(handle);
				PushBackSubWidget(widget);
			}

			AlignSubWidgets(m_orientation, size(), margin(), m_space);
		}
	}

	void Splitter::Remove (AbstractWidget* widget)
	{
		if(RemoveSubWidget(widget)) {
			AlignSubWidgets(m_orientation, size(), margin(), m_space);
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
				preferred_size.set_width(-m_space);
				for (WidgetDeque::iterator it = sub_widgets()->begin();
								it != sub_widgets()->end(); it++) {
					widget = *it;
					if (widget->visiable()) {
						tmp = widget->GetPreferredSize();
						preferred_size.add_width(tmp.width() + m_space);
						preferred_size.set_height(
										std::max(preferred_size.height(),
														tmp.height()));
					}
				}
			} else {
				preferred_size.set_height(-m_space);
				for(WidgetDeque::iterator it = sub_widgets()->begin();
								it != sub_widgets()->end(); it++) {
					widget = *it;
					if(widget->visiable()) {
						tmp = widget->GetPreferredSize();
						preferred_size.add_height(tmp.height() + m_space);
						preferred_size.set_width(
										std::max(preferred_size.width(), tmp.width()));
					}
				}
			}

			preferred_size.add_width(margin().left() + margin().right());
			preferred_size.add_height(margin().top() + margin().bottom());
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
		int max = (sub_widget_size() + 1) / 2;
		if(max == 0) return 0;

		index = index * 2;

		return sub_widgets()->at(index);
	}

	SplitterHandle* Splitter::GetHandle (int index) const
	{
		return 0;
	}

	int Splitter::GetWidgetCount () const
	{
		return 0;
	}

	void Splitter::MoveHandle (int index, int x, int y)
	{
	}

	bool Splitter::UpdateTest (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch (request.type()) {

				case SubWidgetSize:
					return false;	// DO not allow sub widget geometry reset outside

				case SubWidgetPosition: {

					const SplitterHandle* handle = static_cast<const SplitterHandle*>(request.data());
					if(handle) {
						return true;
					}

					return false;
				}

				default:
					return AbstractDequeContainer::UpdateTest(request);

			}

		} else {
			return false;
		}
	}
	
	void Splitter::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch(request.type()) {

				case FormSize: {

					const Size* size_p = static_cast<const Size*>(request.data());
					AlignSubWidgets(m_orientation,*size_p, margin(), m_space);

					break;
				}

				case FormPosition: {

					const Point* pos_p = static_cast<const Point*>(request.data());

					int x = pos_p->x() - position().x();
					int y = pos_p->y() - position().y();

					MoveSubWidgets(x, y);

					break;
				}

				case WidgetRefresh: {
					Refresh();
					break;
				}

				default:
					break;

			}

		}
	}

	ResponseType Splitter::Draw (const RedrawEvent& event)
	{
		return IgnoreAndContinue;
	}
	
	void Splitter::AlignSubWidgets (Orientation orienation, const Size& size, const Margin& margin, int space)
	{
		unsigned int room = GetAverageRoom(orienation, size, margin, space);
		int x = position().x() + margin.left();

		if(orienation == Horizontal) {

			int y = position().y() + margin.bottom();
			unsigned int h = size.height() - margin.top() - margin.bottom();

			int i = 0;
			for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
			{
				if(i % 2 == 0) {
					ResizeSubWidget(*it, room, h);
					SetSubWidgetPosition(*it, x, y);
					x = x + room + space;
				} else {
					ResizeSubWidget(*it, 3, h);
					SetSubWidgetPosition(*it, x, y);
					x = x + 3 + space;
				}
				i++;
			}

		} else {

			int y = position().y() + size.height() - margin.top();
			unsigned int w = size.width() - margin.left() - margin.right();

			int i = 0;
			for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
			{
				if(i % 2 == 0) {
					ResizeSubWidget(*it, w, room);
					SetSubWidgetPosition(*it, x, y);
					y = y - room - space;
				} else {
					ResizeSubWidget(*it, w, 3);
					SetSubWidgetPosition(*it, x, y);
					y = y - 3 - space;
				}

				i++;
			}

		}
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
	
	unsigned int Splitter::GetAverageRoom (Orientation orientation, const Size& size, const Margin& margin, int space)
	{
		unsigned int room = 0;

		if(orientation == Horizontal) {
			room = size.width() - margin.left() - margin.right();
		} else {
			room = size.height() - margin.top() - margin.bottom();
		}

		if(sub_widget_size() > 1) {
			room = room - (space * (sub_widget_size() - 1)) - 3 * (sub_widget_size() - 1) / 2;
			room = room / ((sub_widget_size() + 1) / 2);
		}

		return room;
	}

}
