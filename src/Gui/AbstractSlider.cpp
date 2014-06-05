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
#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

	SlideIcon::SlideIcon () :
					AbstractRoundForm(), m_highlight(false)
	{
		set_size(14, 14);
		set_round_type(RoundAll);
		set_radius(7.0);

		InitializeSliderIcon();
	}

	SlideIcon::~SlideIcon ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void SlideIcon::UpdateGeometry (const UpdateRequest& request)
	{
		switch (request.type()) {

			case FormSize: {
				const Size* size_p = static_cast<const Size*>(request.data());
				Orientation shadedir =
								size_p->width() < size_p->height() ?
												Horizontal : Vertical;
				const Color& color = Theme::instance->scroll().item;
				short shadetop = Theme::instance->scroll().shadetop;
				short shadedown = Theme::instance->scroll().shadedown;

				VertexTool tool;
				tool.Setup(*size_p, DefaultBorderWidth(), round_type(), radius(), color, shadedir, shadetop, shadedown);
				tool.UpdateInnerBuffer(m_inner_buffer.get());
				tool.UpdateOuterBuffer(m_outer_buffer.get());
				break;
			}

			case FormRoundType: {
				Orientation shadedir =
								size().width() < size().height() ?
												Horizontal : Vertical;
				const int* round_p =
								static_cast<const int*>(request.data());
				const Color& color = Theme::instance->scroll().item;
				short shadetop = Theme::instance->scroll().shadetop;
				short shadedown = Theme::instance->scroll().shadedown;

				VertexTool tool;
				tool.Setup(size(), DefaultBorderWidth(), *round_p, radius(), color, shadedir, shadetop, shadedown);
				tool.UpdateInnerBuffer(m_inner_buffer.get());
				tool.UpdateOuterBuffer(m_outer_buffer.get());
				break;
			}

			case FormRoundRadius: {
				Orientation shadedir =
								size().width() < size().height() ?
												Horizontal : Vertical;
				const int* radius_p =
								static_cast<const int*>(request.data());
				const Color& color = Theme::instance->scroll().item;
				short shadetop = Theme::instance->scroll().shadetop;
				short shadedown = Theme::instance->scroll().shadedown;

				VertexTool tool;
				tool.Setup(size(), DefaultBorderWidth(), round_type(), *radius_p, color, shadedir, shadetop, shadedown);
				tool.UpdateInnerBuffer(m_inner_buffer.get());
				tool.UpdateOuterBuffer(m_outer_buffer.get());
				break;
			}

			default:
				break;
		}

	}

	void SlideIcon::Draw (const glm::mat4& mvp)
	{
		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program =
						ShaderManager::instance->default_triangle_program();
		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));

		if (m_highlight) {
			program->SetUniform1i("Gamma", 15);
		} else {
			program->SetUniform1i("Gamma", 0);
		}

		program->SetUniform1i("AA", 0);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		m_inner_buffer->Bind();

		glVertexAttribPointer(0, // attribute
						2,			// number of elements per vertex, here (x,y)
						GL_FLOAT,			 // the type of each element
						GL_FALSE,			 // take our values as-is
						sizeof(GLfloat) * 6,				 // stride
						BUFFER_OFFSET(0)			// offset of first element
										);

		glVertexAttribPointer(1, // attribute
						4,			// number of elements per vertex, here (x,y)
						GL_FLOAT,			 // the type of each element
						GL_FALSE,			 // take our values as-is
						sizeof(GLfloat) * 6,				 // stride
						BUFFER_OFFSET(2 * sizeof(GLfloat))// offset of first element
										);

		glDrawArrays(GL_TRIANGLE_FAN, 0,
						m_inner_buffer->GetBufferSize()
										/ (6 * sizeof(GLfloat)));

		glDisableVertexAttribArray(1);

		GLArrayBuffer::Reset();
		program->SetVertexAttrib4fv("Color", Theme::instance->scroll().outline.data());
		program->SetUniform1i("Gamma", 0);
		program->SetUniform1i("AA", 1);

		m_outer_buffer->Bind();

		glVertexAttribPointer(0, // attribute
						2,			// number of elements per vertex, here (x,y)
						GL_FLOAT,			 // the type of each element
						GL_FALSE,			 // take our values as-is
						0,				 // no extra data between each position
						0					 // offset of first element
						);

		glDrawArrays(GL_TRIANGLE_STRIP, 0,
						m_outer_buffer->GetBufferSize()
						/ (2 * sizeof(GLfloat)));

		m_outer_buffer->Reset();

		glDisableVertexAttribArray(0);

		program->Reset();

		glBindVertexArray(0);
	}

	void SlideIcon::InitializeSliderIcon ()
	{
		glGenVertexArrays(1, &m_vao);

		Orientation shadedir =
						size().width() < size().height() ?
										Horizontal : Vertical;
		const Color& color = Theme::instance->scroll().item;
		short shadetop = Theme::instance->scroll().shadetop;
		short shadedown = Theme::instance->scroll().shadedown;

		VertexTool tool;
		tool.Setup(size(), DefaultBorderWidth(), round_type(), radius(), color, shadedir, shadetop, shadedown);
		m_inner_buffer = tool.GenerateInnerBuffer();
		m_outer_buffer = tool.GenerateOuterBuffer();
	}

}
