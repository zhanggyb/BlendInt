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

#include <BlendInt/Gui/ScrollArea.hpp>

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	ScrollArea::ScrollArea ()
	: Widget(),
	  layout_(nullptr)
	{
		set_size(360, 240);

		layout_ = Manage(new TableLayout(2, 2));
		PushBackSubView(layout_);

		InitializeScrollArea();
	}

	ScrollArea::~ScrollArea ()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void ScrollArea::SetViewport (AbstractScrollable* widget)
	{
		if (!widget)
			return;

	}

	bool ScrollArea::IsExpandX() const
	{
		return true;
	}

	bool ScrollArea::IsExpandY() const
	{
		return true;
	}

	ResponseType ScrollArea::Draw (const AbstractWindow* context)
	{
		Context::shaders->widget_inner_program()->use();

		glUniform1i(Context::shaders->location(Shaders::WIDGET_INNER_GAMMA), 0);
		glUniform4f(Context::shaders->location(Shaders::WIDGET_INNER_COLOR), 1.f, 0.447f, 0.447f, 1.0f);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);

		GLSLProgram::reset();

		return Ignore;
	}

	void ScrollArea::InitializeScrollArea ()
	{
		std::vector<GLfloat> inner_verts;
		GenerateVertices(size(), 0.f, round_type(), round_radius(), &inner_verts, 0);

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		vbo_.generate();
		vbo_.bind();
		vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(
		        Context::shaders->location(Shaders::WIDGET_INNER_COORD));
		glVertexAttribPointer(
		        Context::shaders->location(Shaders::WIDGET_INNER_COORD), 3,
		        GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}
	
	void ScrollArea::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			set_size(*request.size());

			std::vector<GLfloat> inner_verts;
			GenerateVertices(size(), 0.f, round_type(), round_radius(), &inner_verts, 0);

			vbo_.bind();
			vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			vbo_.reset();

			ResizeSubView(layout_, size());

		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

}
