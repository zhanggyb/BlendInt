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
#include <OpenGL/OpenGL.h>
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

	bool Widget::Update (int type, const void* data)
	{
		// do nothing in this base class
		return true;
	}

	void Widget::Draw(RedrawEvent* event)
	{
		RefPtr<GLSLProgram> program = ShaderManager::instance->default_widget_program();
		program->Use();

		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event->pv_matrix(), pos);

		GLint xy_attrib = program->GetAttributeLocation("xy");

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetVertexAttrib1f("z", (float)z());

		float outer_v[4][2];	// vertices for drawing outline
		float inner_v[4][2];	// vertices for drawing inner

		generate_rect_vertices(&(size()), DefaultBorderWidth(), inner_v, outer_v);

		float quad_strip[4 * 2 + 2][2]; // + 2 because the last pair is wrapped

		verts_to_quad_strip (inner_v, outer_v, 4, quad_strip);

		ThemeManager* tm = ThemeManager::instance();

		float r, g, b, a;

		r = tm->themes()->regular.inner.r() / 255.f;
		g = tm->themes()->regular.inner.g() / 255.f;
		b = tm->themes()->regular.inner.b() / 255.f;
		a = tm->themes()->regular.inner.a() / 255.f;

		program->SetVertexAttrib4f("color", r, g, b, a);

		glEnableVertexAttribArray(xy_attrib);
		//glEnableClientState(GL_VERTEX_ARRAY);

		glVertexAttribPointer(xy_attrib, // attribute
							  2,		// number of elements per vertex, here (x,y)
							  GL_FLOAT,	// the type of each element
							  GL_FALSE,	// take our values as-is
							  0,		// no extra data between each position
							  inner_v	// the first element
							  );

		glDrawArrays(GL_POLYGON, 0, 4);

		GLfloat outline_color[4] = {themes()->regular.outline.r() / 255.f,
									themes()->regular.outline.g() / 255.f,
									themes()->regular.outline.b() / 255.f,
									(themes()->regular.outline.a() / WIDGET_AA_JITTER) / 255.f
		};

		program->SetVertexAttrib4fv("color", outline_color);


		glm::vec3 jitter;
		glm::mat4 jitter_matrix;

		glVertexAttribPointer(xy_attrib, // attribute
							  2,		// number of elements per vertex, here (x,y)
							  GL_FLOAT,	// the type of each element
							  GL_FALSE,	// take our values as-is
							  0,		// no extra data between each position
							  quad_strip	// the first element
							  );

		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			jitter.x = jit[j][0]; jitter.y = jit[j][1]; jitter.z = 0.0f;
			jitter_matrix = glm::translate(glm::mat4(1.0), jitter);
			program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp * jitter_matrix));
			glDrawArrays(GL_QUAD_STRIP, 0, 4 * 2 + 2);
		}

		//glDisableClientState(GL_VERTEX_ARRAY);
		glDisableVertexAttribArray(xy_attrib);

		program->Reset();

		event->accept(this);
	}

	void Widget::CursorEnterEvent(bool entered)
	{

	}

	void Widget::KeyPressEvent (KeyEvent* event)
	{
	}

	void Widget::ContextMenuPressEvent (ContextMenuEvent* event)
	{
	}

	void Widget::ContextMenuReleaseEvent (ContextMenuEvent* event)
	{
	}

	void Widget::MousePressEvent (MouseEvent* event)
	{
	}

	void Widget::MouseReleaseEvent (MouseEvent* event)
	{
	}

	void Widget::MouseMoveEvent (MouseEvent* event)
	{
	}

}

