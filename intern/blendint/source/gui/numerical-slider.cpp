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

#include <opengl/opengl.hpp>

#include <gui/numerical-slider.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

	Margin NumericalSlider::kPadding(2, 2, 2, 2);

	NumericalSlider::NumericalSlider (Orientation orientation)
	: AbstractSlider<double>(orientation),
	  title_(""),
	  value_(""),
	  hover_(false),
	  pressed_(false),
	  moved_(false)
	{
		set_round_type(RoundAll);

		int h = title_.font().height();
		int w = h;
		w += h;

		set_size(w + pixel_size(kPadding.hsum()),
				h + pixel_size(kPadding.vsum()));
		set_round_radius(size().height() / 2);

		InitializeNumericalSlider ();
	}

	NumericalSlider::NumericalSlider (const String& title, Orientation orientation)
	: AbstractSlider<double>(orientation),
	  title_(title),
	  value_(""),
	  hover_(false),
	  pressed_(false),
	  moved_(false)
	{
		set_round_type(RoundAll);

		int h = title_.font().height();
		int w = std::max(title_.size().width(), h);
		w += h;

		set_size(w + pixel_size(kPadding.hsum()),
				h + pixel_size(kPadding.vsum()));
		set_round_radius(size().height() / 2);

		InitializeNumericalSlider ();
	}

	NumericalSlider::~NumericalSlider ()
	{
		vao_.clear();	// no need to call
	}

	void NumericalSlider::SetTitle (const String& title)
	{
		title_.SetText(title);

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

		int max_font_height = title_.font().height();

		preferred_size.set_height(
		        max_font_height + pixel_size(kPadding.vsum()));

		preferred_size.set_width(
				max_font_height + pixel_size(kPadding.hsum())
				+ radius_plus + title_.size().width());

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
		char buf[32];
		snprintf(buf, 32, "%.1f", value);

		value_.SetText(buf);
		RequestRedraw();
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
	
	Response NumericalSlider::Draw (AbstractWindow* context)
	{
		float x = context->active_frame()->GetRelativePosition(this).x()
				- context->viewport_origin().x();

		int outline_vertices = GetOutlineVertices(round_type());
		float len = GetSlidePosition(default_border_width(), value());

		AbstractWindow::shaders->widget_split_inner_program()->use();

		if(hover_) {
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

		Rect rect(0, 0, AbstractWindow::icons->num()->size().width() * 2, size().height());

		AbstractWindow::icons->num()->DrawInRect(rect,
				AlignCenter, Color(0x0F0F0FFF).data(), 0, 180.f);

		rect.set_x(rect.width() + pixel_size(kPadding.left()));
		rect.set_width(size().width() - pixel_size(kPadding.hsum()) - 4 * AbstractWindow::icons->num()->size().width() - value_.size().width());
		title_.DrawInRect(rect, AlignLeft | AlignJustify | AlignVerticalCenter | AlignBaseline);

		rect.set_x(rect.x() + title_.size().width());
		rect.set_width(size().width() - pixel_size(kPadding.hsum()) - 4 * AbstractWindow::icons->num()->size().width() - title_.size().width());
		value_.DrawInRect(rect, AlignCenter | AlignJustify | AlignBaseline);

		rect.set_x(size().width() - AbstractWindow::icons->num()->size().width() * 2);
		rect.set_width(AbstractWindow::icons->num()->size().width());

		AbstractWindow::icons->num()->DrawInRect(rect,
				AlignCenter,
				Color(0x0F0F0FFF).data(),
				0,
				0.f);

		return Finish;
	}
	
	void NumericalSlider::PerformHoverIn(AbstractWindow* context)
	{
		hover_ = true;
		RequestRedraw();
	}

	void NumericalSlider::PerformHoverOut(AbstractWindow* context)
	{
		hover_ = false;
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
		vbo_.set_data (sizeof(GLfloat) * inner_verts.size (), &inner_verts[0]);
		glEnableVertexAttribArray (AttributeCoord);
		glVertexAttribPointer (AttributeCoord, 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		// generate buffer for outer
		vao_.bind (1);
		vbo_.bind (1);
		vbo_.set_data (sizeof(GLfloat) * outer_verts.size (), &outer_verts[0]);
		glEnableVertexAttribArray (AttributeCoord);
		glVertexAttribPointer (AttributeCoord, 2,
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
