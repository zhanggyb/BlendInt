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

#include <assert.h>
#include <algorithm>

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/Widget.hpp>

#include <BlendInt/Types.hpp>
#include <BlendInt/Core/Color.hpp>

#include <BlendInt/Interface.hpp>
#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/ContextManager.hpp>
#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

	Widget::Widget()
	: AbstractWidget()
	{
		set_minimal_size(0, 0);
		set_size(120, 80);
		set_preferred_size(120, 80);

		ContextManager::instance->AddSubWidget(this);
	}

	Widget::~Widget()
	{

	}

	bool Widget::Update (const UpdateRequest& request)
	{
		// do nothing in this base class
		return true;
	}

	ResponseType Widget::Draw(const RedrawEvent& event)
	{
		std::vector<GLfloat> inner;
		std::vector<GLfloat> outer;

		VerticesSum sum = GenerateRoundVertices(size(), 1.0, RoundNone, 0.0, &inner, &outer);

		std::vector<GLfloat> strip;

		GenerateTriangleStripVertices(inner, outer, sum.total, &strip);

		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		RefPtr<GLSLProgram> program = ShaderManager::instance->default_widget_program();
		program->Use();

		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetVertexAttrib1f("z", (float)z());

		ThemeManager* tm = ThemeManager::instance();

		float r, g, b, a;

		r = tm->themes()->regular.inner.r() / 255.f;
		g = tm->themes()->regular.inner.g() / 255.f;
		b = tm->themes()->regular.inner.b() / 255.f;
		a = tm->themes()->regular.inner.a() / 255.f;

		program->SetVertexAttrib4f("color", r, g, b, a);

		GLuint vbo[2];
		glGenBuffers(2, vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * inner.size(), &inner[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);

		glVertexAttribPointer(0, // attribute
							  2,		// number of elements per vertex, here (x,y)
							  GL_FLOAT,	// the type of each element
							  GL_FALSE,	// take our values as-is
							  0,		// no extra data between each position
							  BUFFER_OFFSET(0)	// the first element
							  );

		glDrawArrays(GL_TRIANGLE_FAN, 0, sum.total + 2);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * strip.size(), &strip[0],
						GL_STATIC_DRAW);

		GLfloat outline_color[4] = { themes()->regular.outline.r() / 255.f,
						themes()->regular.outline.g() / 255.f,
						themes()->regular.outline.b() / 255.f,
						(themes()->regular.outline.a() / WIDGET_AA_JITTER)
										/ 255.f };

		program->SetVertexAttrib4fv("color", outline_color);

		glm::mat4 jitter_matrix;

		glVertexAttribPointer(0, // attribute
							  2,		// number of elements per vertex, here (x,y)
							  GL_FLOAT,	// the type of each element
							  GL_FALSE,	// take our values as-is
							  0,		// no extra data between each position
							  BUFFER_OFFSET(0)	// the first element
							  );

		for(Jitter::const_iterator it = kJit.begin(); it != kJit.end(); it++)
		{
			jitter_matrix = glm::translate(glm::mat4(1.0), glm::vec3((*it), 0.f));
			program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp * jitter_matrix));
			glDrawArrays(GL_TRIANGLE_STRIP, 0, sum.total * 2 + 2);
		}

		/*
		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			jitter.x = jit[j][0]; jitter.y = jit[j][1]; jitter.z = 0.0f;
			jitter_matrix = glm::translate(glm::mat4(1.0), jitter);
			program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp * jitter_matrix));
			glDrawArrays(GL_TRIANGLE_STRIP, 0, sum.total * 2 + 2);
		}
		*/

		glDisableVertexAttribArray(0);

		program->Reset();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glDeleteBuffers(2, vbo);
		glDeleteVertexArrays(1, &vao);

		return Accept;
	}

	ResponseType Widget::CursorEnterEvent(bool entered)
	{
		return Accept;
	}

	ResponseType Widget::KeyPressEvent (const KeyEvent& event)
	{
		return Accept;
	}

	ResponseType Widget::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType Widget::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType Widget::MousePressEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType Widget::MouseReleaseEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType Widget::MouseMoveEvent (const MouseEvent& event)
	{
		return Accept;
	}

}

