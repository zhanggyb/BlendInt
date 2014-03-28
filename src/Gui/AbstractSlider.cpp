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

#include <BlendInt/Gui/AbstractSlider.hpp>
#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

	SlideIcon::SlideIcon () :
					AbstractRoundForm(), m_highlight(false)
	{
		set_size(14, 14);
		set_round_type(RoundAll);
		set_radius(7.0);

		InitOnce();
	}

	SlideIcon::~SlideIcon ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	bool SlideIcon::Update (const UpdateRequest& request)
	{
		if (request.source() == Predefined) {

			switch (request.type()) {

				case FormSize: {
					const Size* size_p =
									static_cast<const Size*>(request.data());
					Orientation shadedir =
									size_p->width() < size_p->height() ?
													Horizontal : Vertical;
					const Color& color = themes()->scroll.item;
					short shadetop = themes()->scroll.shadetop;
					short shadedown = themes()->scroll.shadedown;

					glBindVertexArray(m_vao);
					GenerateShadedFormBuffers(*size_p, round_type(), radius(),
									color, shadetop, shadedown, shadedir,
									m_inner_buffer.get(), m_outer_buffer.get());
					glBindVertexArray(0);
					return true;
				}

				case FormRoundType: {
					const Size* size_p = &(size());
					Orientation shadedir =
									size_p->width() < size_p->height() ?
													Horizontal : Vertical;
					const RoundType* round_p =
									static_cast<const RoundType*>(request.data());
					const Color& color = themes()->scroll.item;
					short shadetop = themes()->scroll.shadetop;
					short shadedown = themes()->scroll.shadedown;

					glBindVertexArray(m_vao);
					GenerateShadedFormBuffers(*size_p, *round_p, radius(),
									color, shadetop, shadedown, shadedir,
									m_inner_buffer.get(), m_outer_buffer.get());
					glBindVertexArray(0);
					return true;
				}

				case FormRoundRadius: {
					const Size* size_p = &(size());
					Orientation shadedir =
									size_p->width() < size_p->height() ?
													Horizontal : Vertical;
					const float* radius_p =
									static_cast<const float*>(request.data());
					const Color& color = themes()->scroll.item;
					short shadetop = themes()->scroll.shadetop;
					short shadedown = themes()->scroll.shadedown;

					glBindVertexArray(m_vao);
					GenerateShadedFormBuffers(*size_p, round_type(), *radius_p,
									color, shadetop, shadedown, shadedir,
									m_inner_buffer.get(), m_outer_buffer.get());
					glBindVertexArray(0);
					return true;
				}

				default:
					return false;
			}
		} else {
			// no custom to update
			return false;
		}
	}

	void SlideIcon::Draw (const glm::mat4& mvp)
	{
		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program =
						ShaderManager::instance->default_widget_program();
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

		GLfloat outline_color[4] = { themes()->scroll.outline.r() / 255.f,
						themes()->scroll.outline.g() / 255.f,
						themes()->scroll.outline.b() / 255.f,
						themes()->scroll.outline.a() / 255.f };

		program->SetVertexAttrib4fv("Color", outline_color);
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

	void SlideIcon::InitOnce ()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		m_inner_buffer.reset(new GLArrayBuffer);
		m_outer_buffer.reset(new GLArrayBuffer);

		Orientation shadedir =
						size().width() < size().height() ?
										Horizontal : Vertical;
		const Color& color = themes()->scroll.item;
		short shadetop = themes()->scroll.shadetop;
		short shadedown = themes()->scroll.shadedown;

		GenerateShadedFormBuffers(size(), round_type(), radius(), color,
						shadetop, shadedown, shadedir, m_inner_buffer.get(),
						m_outer_buffer.get());

		glBindVertexArray(0);
	}

	AbstractSlider::AbstractSlider (Orientation orientation) :
					RoundWidget(), m_orientation(orientation), m_value(0), m_minimum(
									0), m_maximum(100), m_step(5)
	{
	}

	AbstractSlider::~AbstractSlider ()
	{

	}
	
	void AbstractSlider::SetValue (int value)
	{
		if (value == m_value) {
			return;
		} else {

			if (value < m_minimum || value > m_maximum)
				return;

			if (Update(
							UpdateRequest(Predefined, SliderPropertyValue,
											&value))) {
				m_value = value;
				m_value_changed.fire(m_value);
			}
		}
	}

	void AbstractSlider::SetRange (int value1, int value2)
	{
		int minimum = std::min(value1, value2);
		int maximum = std::max(value1, value2);

		if (m_minimum != minimum) {
			m_minimum = minimum;
			//Update(SliderPropertyMinimum, 0);
		}
		if (m_maximum != maximum) {
			m_maximum = maximum;
			//Update(SliderPropertyMaximum, 0);
		}
	}

	void AbstractSlider::SetMinimum (int minimum)
	{
		if (m_minimum == minimum)
			return;

		m_minimum = minimum;
		//Update (SliderPropertyMinimum, 0);
	}

	void AbstractSlider::SetMaximum (int maximum)
	{
		if (m_maximum == maximum)
			return;

		m_maximum = maximum;
		//Update(SliderPropertyMaximum, 0);
	}

	void AbstractSlider::SetOrientation (Orientation orientation)
	{
		if(m_orientation == orientation) return;

		if(Update(UpdateRequest(Predefined, SliderPropertyOrientation, &orientation))) {
			m_orientation = orientation;
			//fire_property_changed_event(FormPosition);
		}
	}

}
