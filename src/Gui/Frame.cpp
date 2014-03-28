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

#include <BlendInt/Types.hpp>
#include <BlendInt/Core/Color.hpp>
#include <BlendInt/Utilities-inl.hpp>

#include <BlendInt/Gui/Frame.hpp>

#include <BlendInt/Interface.hpp>
#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

	Frame::Frame ()
			: AbstractContainer()
	{
		set_minimal_size(margin().left() + margin().right(),
		        margin().top() + margin().bottom());
		set_size(120, 80);
		set_preferred_size(120, 80);
	}

	Frame::~Frame ()
	{
		// TODO Auto-generated destructor stub
	}

	void Frame::Add (AbstractWidget* widget)
	{
		if(sub_widgets().size() >= 1) {
			ClearSubWidgets();
		}

		if (AddSubWidget(widget)) {
			SetPosition(widget, position().x() + margin().left(),
			        position().y() + margin().bottom());
			Resize(widget, size().width() - margin().left() - margin().right(),
			        size().height() - margin().top() - margin().bottom());
		}
	}

	bool Frame::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {
			switch (request.type()) {

				case FormSize: {
					if (sub_widgets().size()) {
						const Size* size_p = static_cast<const Size*>(request.data());
						Resize(sub_widgets().front(),
						        size_p->width() - margin().left()
						                - margin().right(),
						        size_p->height() - margin().top()
						                - margin().bottom());
					}
					return true;
				}

				case FormPosition: {
					if (sub_widgets().size()) {
						const Point* pos_p = static_cast<const Point*>(request.data());
						SetPosition(sub_widgets().front(),
						        pos_p->x() + margin().left(),
						        pos_p->y() + margin().bottom());
					}
					return true;
				}

				case ContainerMargin: {

					if (sub_widgets().size()) {
						const Margin* margin_p = static_cast<const Margin*>(request.data());
						SetPosition(sub_widgets().front(),
						        position().x() + margin_p->left(),
						        position().y() + margin_p->bottom());
						Resize(sub_widgets().front(),
						        size().width() - margin_p->left()
						                - margin_p->right(),
						        size().height() - margin_p->top()
						                - margin_p->bottom());
					}
					return true;
				}

				default:
					return true;
			}
		} else {
			return false;
		}
	}

	ResponseType Frame::CursorEnterEvent (bool entered)
	{
		return Accept;
	}

	ResponseType Frame::KeyPressEvent (const KeyEvent& event)
	{
		return Accept;
	}

	ResponseType Frame::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType Frame::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType Frame::MousePressEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType Frame::MouseReleaseEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType Frame::MouseMoveEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType Frame::Draw (const RedrawEvent& event)
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

		ThemeManager* tm = ThemeManager::instance();

		float r, g, b, a;

		r = tm->themes()->regular.inner.r() / 255.f;
		g = tm->themes()->regular.inner.g() / 255.f;
		b = tm->themes()->regular.inner.b() / 255.f;
		a = tm->themes()->regular.inner.a() / 255.f;

		program->SetVertexAttrib4f("Color", r, g, b, a);
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

		return Accept;
	}

} /* namespace BlendInt */

