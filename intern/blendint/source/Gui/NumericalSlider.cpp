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

#include <BlendInt/Gui/NumericalSlider.hpp>

#include <BlendInt/Gui/AbstractWindow.hpp>

namespace BlendInt {

	Margin NumericalSlider::default_numberslider_padding(2, 2, 2, 2);

	NumericalSlider::NumericalSlider (Orientation orientation)
	: AbstractSlider<double>(orientation)
	{
		set_round_type(RoundAll);
		int h = font_.height();
		set_size(h + round_radius() * 2 + default_numberslider_padding.hsum(),
						h + default_numberslider_padding.vsum());
		set_round_radius(size().height() / 2);

//		font_.set_pen(AbstractWindow::icons->num()->size().width() * 1.5f, (size().height() - font_.height()) / 2
//				+ std::abs(font_.descender()));

		InitializeNumericalSlider ();
	}

	NumericalSlider::NumericalSlider (const String& title, Orientation orientation)
	: AbstractSlider<double>(orientation),
	  title_(title)
	{
		set_round_type(RoundAll);
		int h = font_.height();
		set_size(h + round_radius() * 2 + default_numberslider_padding.hsum(),
						h + default_numberslider_padding.vsum());
		set_round_radius(size().height() / 2);

//		font_.set_pen(AbstractWindow::icons->num()->size().width() * 1.5f, (size().height() - font_.height()) / 2
//				+ std::abs(font_.descender()));

		InitializeNumericalSlider ();
	}

	NumericalSlider::~NumericalSlider ()
	{
		vao_.clear();	// no need to call
	}

	void NumericalSlider::SetTitle (const String& title)
	{
		if(title_ == title) return;

		title_ = title;
		//Rect text_outline = m_font.GetTextOutline(m_title);

		RequestRedraw();
	}

	bool NumericalSlider::IsExpandX() const
	{
		return true;
	}

	Size NumericalSlider::GetPreferredSize () const
	{
		Size preferred_size;

		int radius_plus = 0;

		if ((round_type() & RoundTopLeft) ||
						(round_type() & RoundBottomLeft))
		{
			radius_plus += round_radius();
		}

		if((round_type() & RoundTopRight) ||
						(round_type() & RoundBottomRight))
		{
			radius_plus += round_radius();
		}

		int max_font_height = font_.height();

		preferred_size.set_height(
		        max_font_height + default_numberslider_padding.vsum());	// top padding: 2, bottom padding: 2

		preferred_size.set_width(
		        max_font_height + default_numberslider_padding.hsum()
		                + radius_plus + 100);

		return preferred_size;
	}

	void NumericalSlider::PerformOrientationUpdate (Orientation orientation)
	{
	}

	void NumericalSlider::PerformMinimumUpdate (double minimum)
	{
	}

	void NumericalSlider::PerformMaximumUpdate (double maximum)
	{
	}

	void NumericalSlider::PerformValueUpdate (double value)
	{
	}

	void NumericalSlider::PerformStepUpdate (double step)
	{
	}

	void NumericalSlider::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {

			set_size(*request.size());

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> outer_verts;

			if (AbstractWindow::theme->number_slider().shaded) {
				GenerateRoundedVertices(Vertical,
						AbstractWindow::theme->number_slider().shadetop,
						AbstractWindow::theme->number_slider().shadedown,
						&inner_verts,
						&outer_verts);
			} else {
				GenerateRoundedVertices(&inner_verts, &outer_verts);
			}

			vbo_.bind(0);
			vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			vbo_.bind(1);
			vbo_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
			GLArrayBuffer::reset();

			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void NumericalSlider::PerformRoundTypeUpdate (int round_type)
	{
		set_round_type(round_type);
		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (AbstractWindow::theme->number_slider().shaded) {
			GenerateRoundedVertices(Vertical,
					AbstractWindow::theme->number_slider().shadetop,
					AbstractWindow::theme->number_slider().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		vbo_.bind(0);
		vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		vbo_.bind(1);
		vbo_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		GLArrayBuffer::reset();

		RequestRedraw();
	}

	void NumericalSlider::PerformRoundRadiusUpdate (float radius)
	{
		set_round_radius(radius);
		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (AbstractWindow::theme->number_slider().shaded) {
			GenerateRoundedVertices(Vertical,
					AbstractWindow::theme->number_slider().shadetop,
					AbstractWindow::theme->number_slider().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		vbo_.bind(0);
		vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		vbo_.bind(1);
		vbo_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		GLArrayBuffer::reset();

		RequestRedraw();
	}
	
	ResponseType NumericalSlider::Draw (AbstractWindow* context)
	{
		float x = context->active_frame()->GetRelativePosition(this).x()
				- context->viewport_origin().x();

		int outline_vertices = GetOutlineVertices(round_type());
		float len = GetSlidePosition(default_border_width(), value());

		AbstractWindow::shaders->widget_split_inner_program()->use();

		if(hover()) {
			glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_SPLIT_INNER_GAMMA), 15);
		} else {
			glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_SPLIT_INNER_GAMMA), 0);
		}

		glUniform1f(AbstractWindow::shaders->location(Shaders::WIDGET_SPLIT_INNER_PARTING), x + len);
		glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_SPLIT_INNER_COLOR0), 1, AbstractWindow::theme->number_slider().inner_sel.data());
		glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_SPLIT_INNER_COLOR1), 1, AbstractWindow::theme->number_slider().inner.data());

		vao_.bind(0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertices + 2);

		AbstractWindow::shaders->widget_outer_program()->use();

		glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_POSITION),
				0.f, 0.f);
		glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_COLOR), 1, AbstractWindow::theme->number_slider().outline.data());

		vao_.bind(1);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, outline_vertices * 2 + 2);

		if (emboss()) {
			glUniform4f(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_COLOR), 1.0f, 1.0f, 1.0f, 0.16f);

			glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_POSITION),
					0.f, - 1.f);

			glDrawArrays(GL_TRIANGLE_STRIP, 0,
							GetHalfOutlineVertices(round_type()) * 2);
		}

		glBindVertexArray(0);

		GLSLProgram::reset();

		float icon_x = AbstractWindow::icons->num()->size().width();

		float icon_y = size().height() / 2.f;
		AbstractWindow::icons->num()->Draw(icon_x, icon_y, 180.f, 1.f, Color(0x0F0F0FFF));

		icon_x = size().width() - AbstractWindow::icons->num()->size().width();
		AbstractWindow::icons->num()->Draw(icon_x, icon_y, 0.f, 1.f, Color(0x0F0F0FFF));

		//int last_text = 0;
		if(title_.size()) {
			//last_text = font_.Print(0.f, 0.f, title_);
		}

		char buf[32];
		snprintf(buf, 32, "%.1f", value());

		// font_.Print(last_text + 1, 0.f, buf);

		return Finish;
	}
	
	void NumericalSlider::PerformHoverIn(AbstractWindow* context)
	{
		RequestRedraw();
	}

	void NumericalSlider::PerformHoverOut(AbstractWindow* context)
	{
		RequestRedraw();
	}

	void NumericalSlider::InitializeNumericalSlider()
	{
		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (AbstractWindow::theme->number_slider().shaded) {
			GenerateRoundedVertices(Vertical,
					AbstractWindow::theme->number_slider().shadetop,
					AbstractWindow::theme->number_slider().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		vao_.generate();
		vbo_.generate();

		// generate buffer for inner
		vao_.bind(0);

		vbo_.bind(0);
		vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		glEnableVertexAttribArray(AbstractWindow::shaders->location(Shaders::WIDGET_SPLIT_INNER_COORD));
		glVertexAttribPointer(AbstractWindow::shaders->location(Shaders::WIDGET_SPLIT_INNER_COORD), 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		// generate buffer for outer
		vao_.bind(1);
		vbo_.bind(1);
		vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		glEnableVertexAttribArray(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_COORD));
		glVertexAttribPointer(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_COORD), 2,
				GL_FLOAT, GL_FALSE, 0, 0);

		vao_.reset();
		vbo_.reset();
	}
	
	float NumericalSlider::GetSlidePosition (float border, double v)
	{
		float minxi = 0.f + border * AbstractWindow::theme->pixel();
		float maxxi = size().width() - border * AbstractWindow::theme->pixel();
		//float radi = (round_radius() - border) * AbstractWindow::theme->pixel();

		return (maxxi - minxi) * v / (maximum() - minimum());
	}
	
}
