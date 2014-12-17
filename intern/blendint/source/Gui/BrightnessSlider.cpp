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

	using Stock::Shaders;

	BrightnessSlider::BrightnessSlider(Orientation orientation)
	: AbstractSlider<int>(orientation)
	{
		InitializeBrightnessSlider();
	}

	BrightnessSlider::~BrightnessSlider()
	{
		glDeleteVertexArrays(2, vao_);
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

	ResponseType BrightnessSlider::Draw (Profile& profile)
	{
		Shaders::instance->widget_triangle_program()->use();

		glUniform2f(Shaders::instance->location(Stock::WIDGET_TRIANGLE_POSITION), 0.f, 0.f);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_ANTI_ALIAS), 0);

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_type()) + 2);

		glVertexAttrib4fv(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COLOR), Theme::instance->number_slider().outline.data());
		glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_ANTI_ALIAS), 1);

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_type()) * 2 + 2);

		glBindVertexArray(0);
		GLSLProgram::reset();

		glm::mat4 icon_mvp;

//		icon_mvp = glm::translate(mvp,
//				glm::vec3(size().width() / 2 - m_dot.size().width() / 2,
//						12, 0.0));

		picker_->Draw (0.f + size ().width () / 2 - picker_->size ().width () / 2,
				0.f + 12.f);

		return Accept;
	}

	void BrightnessSlider::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(*request.size(), default_border_width(), round_type(),
			        round_radius(), Color(0x000000FF), orientation(), 255, 0);
			inner_->bind();
			inner_->set_sub_data(0, tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_sub_data(0, tool.outer_size(), tool.outer_data());

			set_size(*request.size());
			RequestRedraw();
		}

		ReportSizeUpdate(request);
	}

	void BrightnessSlider::PerformRoundTypeUpdate (int round_type)
	{
		VertexTool tool;
		tool.GenerateVertices(size(), default_border_width(), round_type,
				round_radius(), Color(0x000000FF), orientation(), 255, 0);
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());

		set_round_type(round_type);
		RequestRedraw();
	}

	void BrightnessSlider::PerformRoundRadiusUpdate (float radius)
	{
		VertexTool tool;
		tool.GenerateVertices(size(), default_border_width(), round_type(),
				radius, Color(0x000000FF), orientation(),
				255, 0);
		inner_->bind();
		inner_->set_sub_data(0, tool.inner_size(), tool.inner_data());
		outer_->bind();
		outer_->set_sub_data(0, tool.outer_size(), tool.outer_data());

		set_round_radius(radius);
		RequestRedraw();
	}

	void BrightnessSlider::InitializeBrightnessSlider()
	{
		set_round_type(RoundAll);
		set_round_radius(6);
		set_size(90, 14);

		picker_.reset(new CircularPicker(3));

		glGenVertexArrays(2, vao_);

		Color black(0x000000FF);

		VertexTool tool;
		tool.GenerateVertices(size(), default_border_width(), round_type(),
						round_radius(), black, orientation(), 255, 0);

		glBindVertexArray(vao_[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COORD));
		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COLOR));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COORD), 2,	GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COLOR), 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(vao_[1]);
		outer_.reset(new GLArrayBuffer);
		outer_->generate();
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COORD), 2,	GL_FLOAT, GL_FALSE, 0, 0);

		GLArrayBuffer::reset();
		glBindVertexArray(0);
	}

}
