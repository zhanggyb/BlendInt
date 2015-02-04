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
#endif  // __UNIX__

#include <cmath>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/FrameShadow.hpp>

#include <BlendInt/Gui/AbstractWindow.hpp>

namespace BlendInt {

	FrameShadow::FrameShadow (const Size& size, int round_type,
	        float round_radius)
	: AbstractShadow(),
	  vao_(0)
	{
		set_size(size);

		if(round_radius < 1.f)
			round_radius = 1.f;

		set_radius(round_radius);

		round_type &= 0x0F;
		round_type |= (RoundTopLeft | RoundTopRight);

		set_round_type(round_type);

		InitializeFrameShadowOnce();
	}

	FrameShadow::~FrameShadow()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void FrameShadow::Draw(float x, float y) const
	{
		AbstractWindow::shaders->frame_shadow_program()->use();

		glUniform2f(AbstractWindow::shaders->location(Shaders::FRAME_SHADOW_POSITION),
		        x, y);
		glUniform2f(AbstractWindow::shaders->location(Shaders::FRAME_SHADOW_SIZE),
		        size().width(), size().height());

		glBindVertexArray(vao_);

		int count = GetOutlineVertexCount(round_type());

		int i = 0;
		if (i < AbstractWindow::theme->shadow_width()) {
			glUniform1i(
			        AbstractWindow::shaders->location(
			                Shaders::FRAME_SHADOW_ANTI_ALIAS), 1);
			glDrawElements(GL_TRIANGLE_STRIP, count * 2, GL_UNSIGNED_INT,
			        BUFFER_OFFSET(sizeof(GLuint) * count * 2 * i));
		}

		glUniform1i(
		        AbstractWindow::shaders->location(Shaders::FRAME_SHADOW_ANTI_ALIAS),
		        0);
		i++;
		for (; i < AbstractWindow::theme->shadow_width(); i++) {
			glDrawElements(GL_TRIANGLE_STRIP, count * 2, GL_UNSIGNED_INT,
			        BUFFER_OFFSET(sizeof(GLuint) * count * 2 * i));
		}
	}

	void FrameShadow::PerformSizeUpdate(const Size& size)
	{
		set_size(size);

		std::vector<GLfloat> vertices;
		std::vector<GLuint> elements;
		GenerateShadowVertices(vertices, elements);

		vertex_buffer_.bind();
		//buffer_.set_sub_data(0, sizeof(GLfloat) * vertices.size(), &vertices[0]);
		vertex_buffer_.set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		vertex_buffer_.reset();

		element_buffer_.bind();
		element_buffer_.set_data(sizeof(GLuint) * elements.size(), &elements[0]);
		element_buffer_.reset();
	}

	void FrameShadow::PerformRoundTypeUpdate(int type)
	{
		type &= 0x0F;
		type |= (RoundTopLeft | RoundTopRight);

		if(type == round_type()) return;

		set_round_type(type);

		std::vector<GLfloat> vertices;
		std::vector<GLuint> elements;
		GenerateShadowVertices(vertices, elements);

		vertex_buffer_.bind();
		vertex_buffer_.set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		vertex_buffer_.reset();

		element_buffer_.bind();
		element_buffer_.set_data(sizeof(GLuint) * elements.size(), &elements[0]);
		element_buffer_.reset();
	}

	void FrameShadow::PerformRoundRadiusUpdate(float radius)
	{
		if(radius < 1.f) radius = 1.f;

		if(radius == this->radius()) return;

		set_radius(radius);

		std::vector<GLfloat> vertices;
		std::vector<GLuint> elements;
		GenerateShadowVertices(vertices, elements);

		vertex_buffer_.bind();
		vertex_buffer_.set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		vertex_buffer_.reset();

		element_buffer_.bind();
		element_buffer_.set_data(sizeof(GLuint) * elements.size(), &elements[0]);
		element_buffer_.reset();

	}

	void FrameShadow::InitializeFrameShadowOnce()
	{
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		std::vector<GLfloat> vertices;
		std::vector<GLuint> elements;
		GenerateShadowVertices(vertices, elements);

		vertex_buffer_.generate();
		vertex_buffer_.bind();
		vertex_buffer_.set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);

		glEnableVertexAttribArray (AttributeCoord);

		glVertexAttribPointer (AttributeCoord, 3,
		GL_FLOAT, GL_FALSE, 0, 0);

		element_buffer_.generate();
		element_buffer_.bind();
		element_buffer_.set_data(sizeof(GLuint) * elements.size(), &elements[0]);

		glBindVertexArray(0);

		vertex_buffer_.reset();
		element_buffer_.reset();
	}

}
