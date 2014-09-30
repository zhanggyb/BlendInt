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
#include <BlendInt/Gui/NumericalSlider.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	Margin NumericalSlider::default_numberslider_padding(2, 2, 2, 2);

	NumericalSlider::NumericalSlider (Orientation orientation)
	: AbstractSlider<double>(orientation), m_right(false)
	{
		InitOnce ();
	}

	NumericalSlider::~NumericalSlider ()
	{
		glDeleteVertexArrays(4, m_vao);
	}

	void NumericalSlider::SetTitle (const String& title)
	{
		m_title = title;
		//Rect text_outline = m_font.GetTextOutline(m_title);

		m_font.set_pen(round_radius(),
				(size().height() - m_font.GetHeight()) / 2 + std::abs(m_font.GetDescender()));
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

		int max_font_height = m_font.GetHeight();

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
		std::vector<GLfloat> l_verts;
		std::vector<GLfloat> r_verts;
		GenerateSliderVertices(size(), DefaultBorderWidth(),
		        round_type(), round_radius(), value,
		        minimum(), maximum(), l_verts, r_verts);
		m_slider1->bind();
		m_slider1->set_data(sizeof(GLfloat) * l_verts.size(), &l_verts[0]);
		if(r_verts.size()) {
			m_right = true;
			m_slider2->bind();
			m_slider2->set_data(sizeof(GLfloat) * r_verts.size(), &r_verts[0]);
			DBG_PRINT_MSG("%s", "have right part of slider");
		} else {
			m_right = false;
		}
	}

	void NumericalSlider::PerformStepUpdate (double step)
	{
	}

	void NumericalSlider::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {

			VertexTool tool;
			tool.GenerateVertices(*request.size(), DefaultBorderWidth(), round_type(),
			        round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());

			std::vector<GLfloat> l_verts;
			std::vector<GLfloat> r_verts;
			GenerateSliderVertices(*request.size(), DefaultBorderWidth(),
			        round_type(), round_radius(), value(), minimum(), maximum(),
			        l_verts, r_verts);
			m_slider1->bind();
			m_slider1->set_data(sizeof(GLfloat) * l_verts.size(), &l_verts[0]);
			if (r_verts.size()) {
				m_right = true;
				m_slider2->bind();
				m_slider2->set_data(sizeof(GLfloat) * r_verts.size(),
				        &r_verts[0]);
				DBG_PRINT_MSG("%s", "have right part of slider");
			} else {
				m_right = false;
			}

			GLArrayBuffer::reset();

			set_size(*request.size());
			Refresh();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void NumericalSlider::PerformRoundTypeUpdate (
	        const RoundTypeUpdateRequest& request)
	{
		if (request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(size(), DefaultBorderWidth(), *request.round_type(),
			        round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());

			std::vector<GLfloat> l_verts;
			std::vector<GLfloat> r_verts;
			GenerateSliderVertices(size(), DefaultBorderWidth(),
			        *request.round_type(), round_radius(), value(), minimum(),
			        maximum(), l_verts, r_verts);
			m_slider1->bind();
			m_slider1->set_data(sizeof(GLfloat) * l_verts.size(), &l_verts[0]);
			if (r_verts.size()) {
				m_right = true;
				m_slider2->bind();
				m_slider2->set_data(sizeof(GLfloat) * r_verts.size(),
				        &r_verts[0]);
				DBG_PRINT_MSG("%s", "have right part of slider");
			} else {
				m_right = false;
			}

			GLArrayBuffer::reset();

			set_round_type(*request.round_type());
			Refresh();
		}

		if(request.source() == this) {
			ReportRoundTypeUpdate(request);
		}
	}

	void NumericalSlider::PerformRoundRadiusUpdate (
	        const RoundRadiusUpdateRequest& request)
	{
		if (request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(size(), DefaultBorderWidth(), round_type(),
			        *request.round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());

			std::vector<GLfloat> l_verts;
			std::vector<GLfloat> r_verts;
			GenerateSliderVertices(size(), DefaultBorderWidth(), round_type(),
			        *request.round_radius(), value(), minimum(), maximum(),
			        l_verts, r_verts);
			m_slider1->bind();
			m_slider1->set_data(sizeof(GLfloat) * l_verts.size(), &l_verts[0]);
			if (r_verts.size()) {
				m_right = true;
				m_slider2->bind();
				m_slider2->set_data(sizeof(GLfloat) * r_verts.size(),
				        &r_verts[0]);
				DBG_PRINT_MSG("%s", "have right part of slider");
			} else {
				m_right = false;
			}

			GLArrayBuffer::reset();
			set_round_radius(*request.round_radius());
			Refresh();
		}

		if(request.source() == this) {
			ReportRoundRadiusUpdate(request);
		}
	}
	
	ResponseType NumericalSlider::Draw (Profile& profile)
	{
		using Stock::Shaders;

		RefPtr<GLSLProgram> program =
				Shaders::instance->triangle_program();
		program->use();

		program->SetUniform3f("u_position", 0.f, 0.f, 0.f);
		if(hover()) {
			program->SetUniform1i("u_gamma", 15);
		} else {
			program->SetUniform1i("u_gamma", 0);
		}
		program->SetUniform1i("u_AA", 0);

		program->SetVertexAttrib4fv("a_color", Theme::instance->number_slider().inner_sel.data());

		glBindVertexArray(m_vao[0]);

		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_type()) + 2);

		program->SetUniform1i("u_gamma", 0);
		program->SetVertexAttrib4f("a_color", 0.4f, 0.4f, 0.4f, 1.f);

		glBindVertexArray(m_vao[2]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type() & ~(RoundTopRight | RoundBottomRight)) + 2);

		if(m_right) {
			glBindVertexArray(m_vao[3]);
			glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type() & ~(RoundTopLeft | RoundBottomLeft)) + 2);
		}

		program->SetUniform1i("u_AA", 1);
		program->SetVertexAttrib4fv("a_color", Theme::instance->number_slider().outline.data());

		glBindVertexArray(m_vao[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_type()) * 2 + 2);

		if (emboss()) {
			program->SetVertexAttrib4f("a_color", 1.0f, 1.0f, 1.0f, 0.16f);
			program->SetUniform3f("u_position", 0.f, 0.f - 1.f, 0.f);

			glDrawArrays(GL_TRIANGLE_STRIP, 0,
							GetHalfOutlineVertices(round_type()) * 2);
		}

		glBindVertexArray(0);

		program->reset();

		if(m_title.size()) {
			m_font.Print(0.f, 0.f, m_title);
		}

		return Accept;
	}
	
	void NumericalSlider::MouseHoverInEvent(const MouseEvent& event)
	{
		Refresh();
	}

	void NumericalSlider::MouseHoverOutEvent(const MouseEvent& event)
	{
		Refresh();
	}

	void NumericalSlider::InitOnce()
	{
		set_round_type(RoundAll);
		int h = m_font.GetHeight();
		set_size(h + round_radius() * 2 + default_numberslider_padding.hsum(),
						h + default_numberslider_padding.vsum());
		set_round_radius(size().height() / 2);

		VertexTool tool;
		tool.GenerateVertices(size(), DefaultBorderWidth(), round_type(),
						round_radius());

		glGenVertexArrays(4, m_vao);

		// generate buffer for inner
		glBindVertexArray(m_vao[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();

		inner_->set_data(tool.inner_size(), tool.inner_data());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		// generate buffer for outer
		glBindVertexArray(m_vao[1]);

		outer_.reset(new GLArrayBuffer);
		outer_->generate();
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		std::vector<GLfloat> l_verts;
		std::vector<GLfloat> r_verts;

		GenerateSliderVertices(size(), DefaultBorderWidth(),
		        round_type(), round_radius(),
		        value(), minimum(), maximum(),
		        l_verts, r_verts);
		if(r_verts.size()) {
			m_right = true;
			DBG_PRINT_MSG("%s", "have right vertices");
		} else {
			m_right = false;
		}

		// generate buffer for slide bar

		glBindVertexArray(m_vao[2]);

		m_slider1.reset(new GLArrayBuffer);
		m_slider1->generate();
		m_slider1->bind();

		m_slider1->set_data(sizeof(GLfloat) * l_verts.size(), &l_verts[0]);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(m_vao[3]);
		m_slider2.reset(new GLArrayBuffer);
		m_slider2->generate();
		m_slider2->bind();

		m_slider2->set_data(sizeof(GLfloat) * r_verts.size(), &r_verts[0]);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}
	
	float NumericalSlider::GetSlidePosition (float border, double v)
	{
		float minxi = 0.f + border * Theme::instance->pixel();
		float maxxi = size().width() - border * Theme::instance->pixel();
		float radi = (round_radius() - border) * Theme::instance->pixel();

		return round_radius() + (maxxi - minxi - radi) * v / (maximum() - minimum());
	}
	
	void NumericalSlider::GenerateLeftSliderVertices (float minx, float maxx,
					float miny, float maxy, int round_type, float radius,
					std::vector<GLfloat>& verts)
	{
		round_type = round_type & ~(RoundTopRight | RoundBottomRight);
		int minsize = 0;
		const int hnum = (
                (round_type & (RoundTopLeft | RoundTopRight)) == (RoundTopLeft | RoundTopRight)
                ||
                (round_type & (RoundBottomRight | RoundBottomLeft)) == (RoundBottomRight | RoundBottomLeft)
                ) ? 1 : 2;
		const int vnum = (
                (round_type & (RoundTopLeft | RoundBottomLeft)) == (RoundTopLeft | RoundBottomLeft)
                ||
                (round_type & (RoundTopRight | RoundBottomRight)) == (RoundTopRight | RoundBottomRight)
                ) ? 1 : 2;

		minsize = std::min((maxx - minx) * hnum, (maxy - miny) * vnum);

		if (2.0f * radius > minsize)
			radius = 0.5f * minsize;

		GenerateSliderVertices(minx, maxx, miny, maxy, round_type, radius, verts);
	}
	
	void NumericalSlider::GenerateRightSliderVertices (float minx, float maxx,
					float miny, float maxy, int round_type, float radius,
					std::vector<GLfloat>& vertices)
	{
		round_type = round_type & ~(RoundTopLeft | RoundBottomLeft);
		int minsize = 0;
		const int hnum = (
                (round_type & (RoundTopLeft | RoundTopRight)) == (RoundTopLeft | RoundTopRight)
                ||
                (round_type & (RoundBottomRight | RoundBottomLeft)) == (RoundBottomRight | RoundBottomLeft)
                ) ? 1 : 2;
		const int vnum = (
                (round_type & (RoundTopLeft | RoundBottomLeft)) == (RoundTopLeft | RoundBottomLeft)
                ||
                (round_type & (RoundTopRight | RoundBottomRight)) == (RoundTopRight | RoundBottomRight)
                ) ? 1 : 2;

		minsize = std::min((maxx - minx) * hnum, (maxy - miny) * vnum);

		if (2.0f * radius > minsize)
			radius = 0.5f * minsize;

		GenerateSliderVertices(minx, maxx, miny, maxy, round_type, radius, vertices);
	}
	
	void NumericalSlider::GenerateSliderVertices (const Size& out_size,
					float border, int round_type, float out_radius,
					double value, double minimum, double maximum,
					std::vector<GLfloat>& left_vertices,
					std::vector<GLfloat>& right_vertices)
	{
		float radi = (out_radius - border) * Theme::instance->pixel();

		float minx = 0.0;
		float miny = 0.0;
		float maxx = out_size.width();	// test code
		float maxy = out_size.height();

		float minxi = minx + border * Theme::instance->pixel();		// U.pixelsize; // boundbox inner
		float maxxi = maxx - border * Theme::instance->pixel(); 	// U.pixelsize;
		float minyi = miny + border * Theme::instance->pixel();		// U.pixelsize;
		float maxyi = maxy - border * Theme::instance->pixel();		// U.pixelsize;

		float mid = out_radius + (maxxi - minxi - radi) * value / (maximum - minimum);

		if(mid <= (maxxi - radi)) {
			GenerateLeftSliderVertices(minxi, mid, minyi, maxyi, round_type, radi, left_vertices);
		} else {
			GenerateLeftSliderVertices(minxi, maxxi - radi, minyi, maxyi, round_type, radi, left_vertices);
			GenerateRightSliderVertices(maxxi - radi, mid, minyi, maxyi, round_type, radi, right_vertices);
		}
	}

	void NumericalSlider::GenerateSliderVertices (
					float minx, float maxx,
					float miny, float maxy,
					int round_type, float radius,
					std::vector<GLfloat>& vertices)
	{
		float veci[WIDGET_CURVE_RESOLU][2];

		int count = 0;
		unsigned int corner = round_type & RoundAll;
		while (corner != 0) {
			count += corner & 0x1;
			corner = corner >> 1;
		}
		unsigned int outline_vertex_number = 4 - count + count * WIDGET_CURVE_RESOLU;

		// mult
		for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			veci[i][0] = radius * VertexTool::cornervec[i][0];
			veci[i][1] = radius * VertexTool::cornervec[i][1];
		}

		if (vertices.size() != ((outline_vertex_number + 2) * 2)) {
			vertices.resize((outline_vertex_number + 2) * 2);
		}

		// inner[0, 0] is the center of a triangle fan
		vertices[0] = minx + (maxx - minx) / 2.f;
		vertices[1] = miny + (maxy - miny) / 2.f;

		count = 1;

		// corner left-bottom
		if (round_type & RoundBottomLeft) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				vertices[count * 2] = minx + veci[i][1];
				vertices[count * 2 + 1] = miny + radius - veci[i][0];
			}
		} else {
			vertices[count * 2] = minx;
			vertices[count * 2 + 1] = miny;
			count++;
		}

		// corner right-bottom
		if (round_type & RoundBottomRight) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				vertices[count * 2] = maxx - radius + veci[i][0];
				vertices[count * 2 + 1] = miny + veci[i][1];
			}
		} else {
			vertices[count * 2] = maxx;
			vertices[count * 2 + 1] = miny;
			count++;
		}

		// corner right-top
		if (round_type & RoundTopRight) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				vertices[count * 2] = maxx - veci[i][1];
				vertices[count * 2 + 1] = maxy - radius + veci[i][0];
			}
		} else {
			vertices[count * 2] = maxx;
			vertices[count * 2 + 1] = maxy;
			count++;
		}

		// corner left-top
		if (round_type & RoundTopLeft) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				vertices[count * 2] = minx + radius - veci[i][0];
				vertices[count * 2 + 1] = maxy - veci[i][1];
			}

		} else {
			vertices[count * 2] = minx;
			vertices[count * 2 + 1] = maxy;
			count++;
		}

		vertices[count * 2] = vertices[2];
		vertices[count * 2 + 1] = vertices[3];
	}

}
