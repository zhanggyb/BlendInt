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

#include <BlendInt/Gui/Expander.hpp>

#include <BlendInt/Gui/ToggleButton.hpp>
#include <BlendInt/Gui/Frame.hpp>

#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

	Expander::Expander ()
	: m_expand_button(0), m_frame(0), m_space (4)
	{
		m_expand_button = Manage(new ToggleButton);
		m_frame = Manage(new Frame);
		m_frame->SetExpand(true);

		PushBackSubWidget(m_expand_button);
		PushBackSubWidget(m_frame);

		int x = position().x();
		int y = position().y();
		unsigned int width = 0;
		unsigned int height = 0;

		Size sub_prefer = m_expand_button->GetPreferredSize();
		width = std::max(width, sub_prefer.width());
		height += sub_prefer.height();

		ResizeSubWidget(m_expand_button, width, sub_prefer.height());
		y -= sub_prefer.height();
		SetSubWidgetPosition(m_expand_button, x, y);

		height += m_space;
		y -= m_space;

		sub_prefer = m_frame->GetPreferredSize();
		width = std::max(width, sub_prefer.width());
		height += sub_prefer.height();

		ResizeSubWidget(m_expand_button, width, sub_prefer.height());
		y -= sub_prefer.height();
		SetSubWidgetPosition(m_expand_button, x, y);

		width = width + margin().left() + margin().right();
		height = height + margin().top() + margin().bottom();

		set_size(width, height);
	}

	Expander::~Expander ()
	{
	}

	bool Expander::Setup (AbstractWidget* widget)
	{
		bool ret = false;

		if(m_frame->Setup(widget)) {
			ret = true;
		}

		return ret;
	}

	Size Expander::GetPreferredSize() const
	{
		Size prefer;

		Size sub_prefer = m_expand_button->GetPreferredSize();
		prefer.set_width(std::max(prefer.width(), sub_prefer.width()));
		prefer.add_height(sub_prefer.height());

		prefer.add_height(m_space);

		sub_prefer = m_frame->GetPreferredSize();
		prefer.set_width(std::max(prefer.width(), sub_prefer.width()));
		prefer.add_height(sub_prefer.height());

		prefer.add_width(margin().left() + margin().right());
		prefer.add_height(margin().top() + margin().bottom());

		return prefer;
	}

	bool Expander::UpdateTest (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch (request.type()) {

				case SubWidgetSize:
					return false;	// DO not allow sub widget geometry reset outside

				case SubWidgetPosition:
					return false;

				default:
					return AbstractDequeContainer::UpdateTest(request);

			}

		} else {
			return false;
		}
	}

	void Expander::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {
			switch (request.type()) {

				case FormSize: {
					if (sub_widget_size()) {
						const Size* size_p = static_cast<const Size*>(request.data());
						FillWithPreferredHeight(position(), *size_p, margin(), m_space);
					}
					break;
				}

				case FormPosition: {
					if (sub_widget_size()) {
						const Point* pos_p = static_cast<const Point*>(request.data());

						int x = pos_p->x() - position().x();
						int y = pos_p->y() - position().y();

						MoveSubWidgets(x, y);
					}
					break;
				}

				case ContainerMargin: {

					const Margin* margin_p = static_cast<const Margin*>(request.data());
					set_margin(*margin_p);
					FillWithPreferredHeight(position(), size(), *margin_p, m_space);

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

	ResponseType Expander::Draw (const RedrawEvent& event)
	{
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		RefPtr<GLSLProgram> program = ShaderManager::instance->default_triangle_program();
		program->Use();

		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));

		std::vector<GLfloat> vertices(12);

		GenerateFlatRectVertices(size(), 0.f, &vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		program->SetVertexAttrib4f("Color", 0.85f, 0.85f, 0.85f, 0.75f);
		program->SetUniform1i("AA", 0);

		glEnableVertexAttribArray(0);	// 0 is the locaiton in shader

		glVertexAttribPointer(
						0, // attribute
						2,		// number of elements per vertex, here (x,y)
						GL_FLOAT,	// the type of each element
						GL_FALSE,	// take our values as-is
						0,		// no extra data between each position
						BUFFER_OFFSET(0)	// the first element
		);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

		glDisableVertexAttribArray(0);

		program->Reset();

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);

		return IgnoreAndContinue;
	}

	ResponseType Expander::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Expander::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Expander::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Expander::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Expander::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Expander::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Expander::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}
	
	void Expander::FillWithPreferredHeight (const Point& out_pos,
					const Size& out_size, const Margin& margin, int space)
	{
		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();
		unsigned int width = out_size.width() - margin.left() - margin.right();
		unsigned int height = out_size.height() - margin.top() - margin.bottom();

		FillWithPreferredHeight(x, y, width, height, space);
	}
	
	void Expander::FillWithPreferredHeight (int x, int y, unsigned int width,
					unsigned int height, int space)
	{
		y = y + height;

		Size prefer;

		unsigned int button_preferred_height = 0;
		unsigned int sum = 0;

		prefer = m_expand_button->GetPreferredSize();
		button_preferred_height = prefer.height();
		sum += prefer.height();

		prefer = m_frame->GetPreferredSize();
		sum += prefer.height();

		if(button_preferred_height < (height - space)) {
			y = y - button_preferred_height;
			ResizeSubWidget(m_expand_button, width, button_preferred_height);
			SetSubWidgetPosition(m_expand_button, x, y);

			y -= space;

			ResizeSubWidget(m_frame, width, height - button_preferred_height - space);
			y = y - (height - button_preferred_height - space);
			SetSubWidgetPosition(m_frame, x, y);
		} else {
			y = y - (height - space);
			ResizeSubWidget(m_expand_button, width, height - space);
			SetSubWidgetPosition(m_expand_button, x, y);

			y -= space;
			ResizeSubWidget(m_frame, width, 0);
			SetSubWidgetPosition(m_frame, x, y);
		}

	}

}
