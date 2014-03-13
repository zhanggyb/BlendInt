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
#include <OpenGL/OpenGL.h>
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

	}

	bool SlideIcon::Update (const UpdateRequest& request)
	{
		if (request.id() == Predefined) {

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

					GenerateShadedFormBuffers(size_p, round_type(), radius(),
					        color, shadetop, shadedown, shadedir, 5,
					        m_inner_buffer.get(), m_outer_buffer.get(),
					        m_highlight_buffer.get());

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

					GenerateShadedFormBuffers(size_p, *round_p, radius(), color,
					        shadetop, shadedown, shadedir, 5,
					        m_inner_buffer.get(), m_outer_buffer.get(),
					        m_highlight_buffer.get());
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

					GenerateShadedFormBuffers(size_p, round_type(), *radius_p,
					        color, shadetop, shadedown, shadedir, 5,
					        m_inner_buffer.get(), m_outer_buffer.get(),
					        m_highlight_buffer.get());
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
		RefPtr<GLSLProgram> program =
						ShaderManager::instance->default_widget_program();
		program->Use();

		GLint xy_attrib = program->GetAttributeLocation("xy");
		GLint color_attrib = program->GetAttributeLocation("color");

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));

		glEnableVertexAttribArray(xy_attrib);
		glEnableVertexAttribArray(color_attrib);

		if(m_highlight) {
			m_highlight_buffer->Bind();
		} else {
			m_inner_buffer->Bind();
		}

		glVertexAttribPointer(xy_attrib, // attribute
						2,			// number of elements per vertex, here (x,y)
						GL_FLOAT,			 // the type of each element
						GL_FALSE,			 // take our values as-is
						sizeof(GLfloat) * 6,				 // stride
						BUFFER_OFFSET(0)			// offset of first element
										);

		glVertexAttribPointer(color_attrib, // attribute
						4,			// number of elements per vertex, here (x,y)
						GL_FLOAT,			 // the type of each element
						GL_FALSE,			 // take our values as-is
						sizeof(GLfloat) * 6,				 // stride
						BUFFER_OFFSET(2 * sizeof(GLfloat))// offset of first element
										);

		if (m_highlight) {
			glDrawArrays(GL_POLYGON, 0,
			                m_highlight_buffer->GetBufferSize()
			                                / (6 * sizeof(GLfloat)));

		} else {
			glDrawArrays(GL_POLYGON, 0,
			                m_inner_buffer->GetBufferSize()
			                                / (6 * sizeof(GLfloat)));

		}

		glDisableVertexAttribArray(color_attrib);

		GLArrayBuffer::Reset();

		GLfloat outline_color[4] = {
						themes()->scroll.outline.r() / 255.f,
		                themes()->scroll.outline.g() / 255.f,
		                themes()->scroll.outline.b() / 255.f,
		                (themes()->scroll.outline.a() / WIDGET_AA_JITTER) / 255.f
		};

		program->SetVertexAttrib4fv("color", outline_color);

		glm::vec3 jitter;
		glm::mat4 jitter_matrix;

		m_outer_buffer->Bind();

		glVertexAttribPointer(xy_attrib, // attribute
						2,			// number of elements per vertex, here (x,y)
						GL_FLOAT,			 // the type of each element
						GL_FALSE,			 // take our values as-is
						0,				 // no extra data between each position
						0					 // offset of first element
						);

		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			jitter.x = jit[j][0];
			jitter.y = jit[j][1];
			//jitter.z = 0.0f;
			jitter_matrix = glm::translate(glm::mat4(1.0), jitter);
			program->SetUniformMatrix4fv("MVP", 1, GL_FALSE,
							glm::value_ptr(mvp * jitter_matrix));

			glDrawArrays(GL_QUAD_STRIP, 0,
							m_outer_buffer->GetBufferSize()
											/ (2 * sizeof(GLfloat)));
		}

		m_outer_buffer->Reset();

		glDisableVertexAttribArray(xy_attrib);

		program->Reset();
	}

	void SlideIcon::InitOnce ()
	{
		m_inner_buffer.reset(new GLArrayBuffer);
		m_outer_buffer.reset(new GLArrayBuffer);
		m_highlight_buffer.reset(new GLArrayBuffer);

		Orientation shadedir =
						size().width() < size().height() ?
										Horizontal : Vertical;
		const Color& color = themes()->scroll.item;
		short shadetop = themes()->scroll.shadetop;
		short shadedown = themes()->scroll.shadedown;

		GenerateShadedFormBuffers(&size(), round_type(), radius(), color,
						shadetop, shadedown, shadedir, 5, m_inner_buffer.get(),
						m_outer_buffer.get(), m_highlight_buffer.get());
	}

	AbstractSlider::AbstractSlider(Orientation orientation)
		: RoundWidget (), m_orientation(orientation),
		  m_value(0), m_minimum(0), m_maximum(100), m_step(5)
	{
	}

	AbstractSlider::~AbstractSlider()
	{
	
	}

	void AbstractSlider::SetValue (int value)
	{
		if(value == m_value) {
			return;
		} else {

			if(value < m_minimum || value > m_maximum)
				return;

			if(Update(UpdateRequest(Predefined, SliderPropertyValue, &value))) {
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
		if(m_minimum == minimum) return;

		m_minimum = minimum;
		//Update (SliderPropertyMinimum, 0);
	}

	void AbstractSlider::SetMaximum (int maximum)
	{
		if(m_maximum == maximum) return;

		m_maximum = maximum;
		//Update(SliderPropertyMaximum, 0);
	}

	void AbstractSlider::set_orientation (Orientation orientation)
	{
		if(m_orientation == orientation) return;

		m_orientation = orientation;

		//Update(SliderPropertyOrientation, 0);
	}

}
