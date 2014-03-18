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

#include <BlendInt/Gui/MenuBar.hpp>

#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

	MenuBar::MenuBar ()
	: AbstractContainer(), m_vao(0), m_space(2)
	{
		set_expand_x(true);
		set_expand_y(false);

		set_size(200, 20);
		set_preferred_size(200, 20);

		InitOnce();
	}

	MenuBar::~MenuBar ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	bool MenuBar::Update (const UpdateRequest& request)
	{
		if(request.id() == Predefined) {

			switch (request.type()) {

			case FormSize: {

				const Size* size_p = static_cast<const Size*>(request.data());

				glBindVertexArray(m_vao);

				std::vector<GLfloat> vertices(12);

				GenerateFlatRectVertices(*size_p, 0.f, &vertices);

				m_buffer->Bind();
				m_buffer->SetData(sizeof(GLfloat) * vertices.size(), &vertices[0]);
				m_buffer->Reset();

				glBindVertexArray(0);

				return true;
			}

			case FormPosition: {
				return true;
			}

			default:

				return false;
			}

		} else {
			return true;
		}
	}

	void MenuBar::Draw (RedrawEvent* event)
	{
		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program = ShaderManager::instance->default_widget_program();
		program->Use();

		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event->pv_matrix(), pos);

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetVertexAttrib1f("z", (float)z());

		ThemeManager* tm = ThemeManager::instance();

		float r, g, b, a;

		r = tm->themes()->regular.inner.r() / 255.f;
		g = tm->themes()->regular.inner.g() / 255.f;
		b = tm->themes()->regular.inner.b() / 255.f;
		a = tm->themes()->regular.inner.a() / 255.f;

		program->SetVertexAttrib4f("color", r, g, b, a);

		glEnableVertexAttribArray(0);

		DrawTriangleFan(0, m_buffer.get());

		glDisableVertexAttribArray(0);

		program->Reset();

		m_buffer->Reset();

		glBindVertexArray(0);

		event->accept(this);
	}

	void MenuBar::CursorEnterEvent (bool entered)
	{
	}

	void MenuBar::KeyPressEvent (KeyEvent* event)
	{
	}

	void MenuBar::ContextMenuPressEvent (ContextMenuEvent* event)
	{
	}

	void MenuBar::ContextMenuReleaseEvent (ContextMenuEvent* event)
	{
	}

	void MenuBar::MousePressEvent (MouseEvent* event)
	{
	}

	void MenuBar::MouseReleaseEvent (MouseEvent* event)
	{
	}

	void MenuBar::MouseMoveEvent (MouseEvent* event)
	{
	}

	void MenuBar::InitOnce()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		std::vector<GLfloat> vertices(12);

		GenerateFlatRectVertices(size(), 0.f, &vertices);

		m_buffer.reset(new GLArrayBuffer);
		m_buffer->Generate();

		m_buffer->Bind();
		m_buffer->SetData(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		m_buffer->Reset();

		glBindVertexArray(0);
	}

}
