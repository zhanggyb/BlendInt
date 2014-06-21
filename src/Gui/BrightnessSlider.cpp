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

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/BrightnessSlider.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	BrightnessSlider::BrightnessSlider(Orientation orientation)
	: AbstractSlider<int>(orientation)
	{
		InitializeBrightnessSlider();
	}

	BrightnessSlider::~BrightnessSlider()
	{
		glDeleteVertexArrays(2, m_vao);
	}

	bool BrightnessSlider::IsExpandX() const
	{
		if(orientation() == Horizontal) {
			return true;
		} else {
			return false;
		}
	}

	bool BrightnessSlider::IsExpandY() const
	{
		if(orientation() == Vertical) {
			return true;
		} else {
			return false;
		}
	}

	Size BrightnessSlider::GetPreferredSize() const
	{
		// Same size in ColorWheel
		if(orientation() == Horizontal) {
			return Size(160, 14);
		} else {
			return Size(14, 160);
		}
	}

	ResponseType BrightnessSlider::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		RefPtr<GLSLProgram> program =
						Shaders::instance->default_triangle_program();
		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetUniform1i("AA", 0);
		program->SetUniform1i("Gamma", 0);

		glBindVertexArray(m_vao[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_corner_type()) + 2);

		program->SetVertexAttrib4fv("Color", Theme::instance->number_slider().outline.data());
		program->SetUniform1i("AA", 1);

		glBindVertexArray(m_vao[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_corner_type()) * 2 + 2);

		glBindVertexArray(0);
		program->Reset();

		glm::mat4 icon_mvp;

		icon_mvp = glm::translate(mvp,
				glm::vec3(size().width() / 2 - m_dot.size().width() / 2,
						12, 0.0));

		m_dot.Draw(icon_mvp);

		return Accept;
	}

	void BrightnessSlider::UpdateGeometry (const WidgetUpdateRequest& request)
	{
		switch (request.type()) {

			case WidgetSize: {
				const Size* size_p = static_cast<const Size*>(request.data());
				VertexTool tool;
				tool.Setup(*size_p, DefaultBorderWidth(), round_corner_type(),
								round_corner_radius(), Color(0x000000FF),
								orientation(), 255, 0);
				m_inner->Bind();
				tool.SetInnerBufferData(m_inner.get());
				m_outer->Bind();
				tool.SetOuterBufferData(m_outer.get());
				Refresh();
				break;
			}

			case WidgetRoundCornerType: {
				const int* type_p = static_cast<const int*>(request.data());
				VertexTool tool;
				tool.Setup(size(), DefaultBorderWidth(), *type_p,
								round_corner_radius(), Color(0x000000FF),
								orientation(), 255, 0);
				m_inner->Bind();
				tool.SetInnerBufferData(m_inner.get());
				m_outer->Bind();
				tool.SetOuterBufferData(m_outer.get());
				Refresh();
				break;
			}

			case WidgetRoundCornerRadius: {
				const float* radius_p =
								static_cast<const float*>(request.data());
				VertexTool tool;
				tool.Setup(size(), DefaultBorderWidth(), round_corner_type(),
								*radius_p, Color(0x000000FF), orientation(), 255, 0);
				m_inner->Bind();
				tool.SetInnerBufferData(m_inner.get());
				m_outer->Bind();
				tool.SetOuterBufferData(m_outer.get());
				Refresh();
				break;
			}

			default:
				AbstractSlider<int>::UpdateGeometry(request);
		}
	}

	void BrightnessSlider::UpdateSlider (const WidgetUpdateRequest& request)
	{
	}

	void BrightnessSlider::InitializeBrightnessSlider()
	{
		set_round_corner_type(RoundAll);
		set_round_corner_radius(6);
		set_size(90, 14);

		m_dot.Resize(3);

		glGenVertexArrays(2, m_vao);

		Color black(0x000000FF);

		VertexTool tool;
		tool.Setup(size(), DefaultBorderWidth(), round_corner_type(),
						round_corner_radius(), black, orientation(), 255, 0);

		glBindVertexArray(m_vao[0]);

		m_inner.reset(new GLArrayBuffer);
		m_inner->Generate();
		m_inner->Bind();
		tool.SetInnerBufferData(m_inner.get());

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(m_vao[1]);
		m_outer.reset(new GLArrayBuffer);
		m_outer->Generate();
		m_outer->Bind();
		tool.SetOuterBufferData(m_outer.get());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		GLArrayBuffer::Reset();
		glBindVertexArray(0);
	}

}
