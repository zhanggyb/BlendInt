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

#include <BlendInt/Gui/ToolBar.hpp>
#include <BlendInt/Service/ShaderManager.hpp>
#include <BlendInt/Service/Theme.hpp>

namespace BlendInt {

	ToolBar::ToolBar ()
	: AbstractDequeContainer(), m_vao(0), m_space(4)
	{
		InitOnce();
	}

	ToolBar::~ToolBar ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	bool ToolBar::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch (request.type()) {

				case FormPosition: {

					const Point* pos_p = static_cast<const Point*>(request.data());

					int x = pos_p->x() - position().x();
					int y = pos_p->y() - position().y();

					MoveSubWidgets(x, y);

					return true;
				}

				case FormSize: {
					const Size* size_p = static_cast<const Size*>(request.data());

					glBindVertexArray(m_vao);

					std::vector<GLfloat> vertices(12);

					GenerateFlatRectVertices(*size_p, 0.f, &vertices);

					m_inner->Bind();
					m_inner->SetData(sizeof(GLfloat) * vertices.size(), &vertices[0]);
					m_inner->Reset();

					glBindVertexArray(0);
					return true;
				}

				case ContextRefresh: {
					return Refresh();
				}

				default:
					return true;
			}

		} else {
			return false;
		}
	}

	ResponseType ToolBar::Draw (const RedrawEvent& event)
	{
		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		RefPtr<GLSLProgram> program = ShaderManager::instance->default_triangle_program();

		glBindVertexArray(m_vao);

		program->Use();

		glm::vec4 color;
		color.r = themes()->regular.inner.r() / 255.f;
		color.g = themes()->regular.inner.g() / 255.f;
		color.b = themes()->regular.inner.b() / 255.f;
		color.a = themes()->regular.inner.a() / 255.f;

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetUniform1i("AA", 0);
		program->SetVertexAttrib4fv("Color", glm::value_ptr(color));
		program->SetUniform1i("Gamma", 0);

		glEnableVertexAttribArray(0);

		m_inner->Bind();

		glVertexAttribPointer(0, // attribute
							  2,			// number of elements per vertex, here (x,y)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  0,				 // no extra data between each position
							  0					 // offset of first element
							  );

		glDrawArrays(GL_TRIANGLE_FAN, 0,
							m_inner->GetBufferSize()
											/ (2 * sizeof(GLfloat)));

		m_inner->Reset();

		program->SetUniform1i("AA", 1);
		program->SetUniform1i("Gamma", 0);
		program->SetVertexAttrib4f("Color", 0.f, 0.f, 0.f, 1.f);

		glDisableVertexAttribArray(0);

		program->Reset();

		glBindVertexArray(0);
		return Accept;
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
		return Accept;
	}

	ResponseType ToolBar::MouseReleaseEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType ToolBar::MouseMoveEvent (const MouseEvent& event)
	{
		return Accept;
	}

	void ToolBar::Add (AbstractWidget* widget)
	{
		int x = GetLastPosition();
		AppendSubWidget(widget);
		ResizeSubWidget(widget, widget->preferred_size().width(), 24);
		SetSubWidgetPosition(widget, x, position().y() + margin().bottom());
	}

	void ToolBar::AddButton (const RefPtr<ActionItem>& action)
	{
		ToolButton* button = Manage(new ToolButton);

		AppendSubWidget(button);

		int x = GetLastPosition();
		ResizeSubWidget(button, 24, 24);
		SetSubWidgetPosition(button, x, position().y() + margin().bottom());
	}

	void ToolBar::InitOnce ()
	{
		set_preferred_size(200, 32);
		set_size(200, 32);

		set_margin(4, 4, 4, 4);	// the same as MenuBar

		set_expand_x(true);
		set_expand_y(false);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		std::vector<GLfloat> vertices(12);

		GenerateFlatRectVertices(size(), 0.f, &vertices);

		m_inner.reset(new GLArrayBuffer);
		m_inner->Generate();

		m_inner->Bind();
		m_inner->SetData(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		m_inner->Reset();

		glBindVertexArray(0);
	}

	int ToolBar::GetLastPosition ()
	{
		int x = position().x() + margin().left();

		for(WidgetDeque::iterator it = sub_widgets()->begin(); it != sub_widgets()->end(); it++)
		{
			x += (*it)->size().width() + m_space;
		}

		return x;
	}

}
