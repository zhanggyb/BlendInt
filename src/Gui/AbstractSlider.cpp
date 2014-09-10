/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
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

#include <algorithm>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/AbstractSlider.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	SlideIcon::SlideIcon ()
	: AbstractRoundForm(), m_highlight(false)
	{
		set_size(14, 14);
		set_round_type(RoundAll);
		set_radius(7.0);

		InitializeSliderIcon();
	}

	SlideIcon::~SlideIcon ()
	{
		glDeleteVertexArrays(2, m_vao);
	}

	void SlideIcon::PerformSizeUpdate(const Size& size)
	{
		Orientation shadedir =
						size.width() < size.height() ?
										Horizontal : Vertical;
		const Color& color = Theme::instance->scroll().item;
		short shadetop = Theme::instance->scroll().shadetop;
		short shadedown = Theme::instance->scroll().shadedown;

		VertexTool tool;
		tool.GenerateVertices(size, DefaultBorderWidth(), round_type(), radius(), color, shadedir, shadetop, shadedown);
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());
		GLArrayBuffer::reset();
	}

	void SlideIcon::PerformRoundTypeUpdate(int type)
	{
		Orientation shadedir =
						size().width() < size().height() ?
										Horizontal : Vertical;
		const Color& color = Theme::instance->scroll().item;
		short shadetop = Theme::instance->scroll().shadetop;
		short shadedown = Theme::instance->scroll().shadedown;

		VertexTool tool;
		tool.GenerateVertices(size(), DefaultBorderWidth(), type, radius(), color, shadedir, shadetop, shadedown);
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());
		GLArrayBuffer::reset();
	}

	void SlideIcon::PerformRoundRadiusUpdate(float radius)
	{
		Orientation shadedir =
						size().width() < size().height() ?
										Horizontal : Vertical;
		const Color& color = Theme::instance->scroll().item;
		short shadetop = Theme::instance->scroll().shadetop;
		short shadedown = Theme::instance->scroll().shadedown;

		VertexTool tool;
		tool.GenerateVertices(size(), DefaultBorderWidth(), round_type(), radius, color, shadedir, shadetop, shadedown);
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());
		GLArrayBuffer::reset();
	}

	void SlideIcon::Draw (const glm::vec3& pos, short gamma)
	{
		RefPtr<GLSLProgram> program =
						Shaders::instance->triangle_program();
		program->use();

		glUniform3f(Shaders::instance->triangle_uniform_position(), pos.x, pos.y, 0.f);

		if (m_highlight) {
			glUniform1i(Shaders::instance->triangle_uniform_gamma(), 15);
		} else {
			glUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
		}

		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 0);

		glBindVertexArray(m_vao[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		glUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 1);
		glVertexAttrib4fv(Shaders::instance->triangle_attrib_color(), Theme::instance->scroll().outline.data());

		glBindVertexArray(m_vao[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_type()) * 2 + 2);

		glBindVertexArray(0);
		program->reset();
	}

	void SlideIcon::InitializeSliderIcon ()
	{
		glGenVertexArrays(2, m_vao);

		Orientation shadedir =
						size().width() < size().height() ?
										Horizontal : Vertical;
		const Color& color = Theme::instance->scroll().item;
		short shadetop = Theme::instance->scroll().shadetop;
		short shadedown = Theme::instance->scroll().shadedown;

		VertexTool tool;
		tool.GenerateVertices(size(), DefaultBorderWidth(), round_type(), radius(), color, shadedir, shadetop, shadedown);

		glBindVertexArray(m_vao[0]);
		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));

		glVertexAttribPointer(1, 4,	GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(m_vao[1]);
		outer_.reset(new GLArrayBuffer);
		outer_->generate();
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

}
