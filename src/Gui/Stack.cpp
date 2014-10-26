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

#include <algorithm>

#include <BlendInt/Gui/Stack.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Stack::Stack()
	: StackLayout(),
	  vao_(0)
	{
		InitializeStack();
	}

	Stack::~Stack ()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void Stack::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			int w = request.size()->width() - margin().hsum();
			int h = request.size()->height() - margin().vsum();

			set_size(*request.size());
            
            std::vector<GLfloat> inner_verts;
            GenerateVertices(&inner_verts, 0);
            
            inner_.bind();
            inner_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
            inner_.reset();
            
			ResizeSubWidgets(w, h);
            
            Refresh();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	ResponseType Stack::Draw (Profile& profile)
	{
		Shaders::instance->triangle_program()->use();

		glUniform3f(Shaders::instance->location(Stock::TRIANGLE_POSITION),
		        0.f, 0.f, 0.f);
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_ANTI_ALIAS), 0);

		glVertexAttrib4f(Shaders::instance->location(Stock::TRIANGLE_COLOR), 0.447f,
		        0.447f, 0.447f, 1.0f);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
		        GetOutlineVertices(round_type()) * 2 + 2);
		glBindVertexArray(0);

		GLSLProgram::reset();

		return StackLayout::Draw(profile);
	}

	void Stack::InitializeStack()
	{
        std::vector<GLfloat> inner_verts;
        GenerateVertices(&inner_verts, 0);

		glGenVertexArrays(1, &vao_);

		glBindVertexArray(vao_);

        inner_.generate();

        inner_.bind();
		inner_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_INNER_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_INNER_COORD), 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

}
