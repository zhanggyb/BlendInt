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
	: AbstractContainer(), m_space(2)
	{
		set_expand_x(true);
		set_expand_y(false);

		set_size(200, 20);
		set_preferred_size(200, 20);
	}

	MenuBar::~MenuBar ()
	{
	}

	bool MenuBar::Update (const UpdateRequest& request)
	{
		return true;
	}

	void MenuBar::Draw (RedrawEvent* event)
	{
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		RefPtr<GLSLProgram> program = ShaderManager::instance->default_widget_program();
		program->Use();

		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event->pv_matrix(), pos);

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetVertexAttrib1f("z", (float)z());

		std::vector<GLfloat> vertices(12);

		GenerateFlatRectVertices(size(), 0.f, &vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		ThemeManager* tm = ThemeManager::instance();

		float r, g, b, a;

		r = tm->themes()->regular.inner.r() / 255.f;
		g = tm->themes()->regular.inner.g() / 255.f;
		b = tm->themes()->regular.inner.b() / 255.f;
		a = tm->themes()->regular.inner.a() / 255.f;

		program->SetVertexAttrib4f("color", r, g, b, a);

		glEnableVertexAttribArray(0);

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

}
