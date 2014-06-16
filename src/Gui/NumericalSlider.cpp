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
	: AbstractSlider<double>(orientation)
	{
		InitOnce ();
	}

	NumericalSlider::~NumericalSlider ()
	{
		glDeleteVertexArrays(3, m_vao);
	}

	void NumericalSlider::SetTitle (const String& title)
	{
		m_title = title;
		Rect text_outline = m_font.GetTextOutline(m_title);

		m_font.set_pen(round_corner_radius(),
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

		if ((round_corner_type() & RoundTopLeft) ||
						(round_corner_type() & RoundBottomLeft))
		{
			radius_plus += round_corner_radius();
		}

		if((round_corner_type() & RoundTopRight) ||
						(round_corner_type() & RoundBottomRight))
		{
			radius_plus += round_corner_radius();
		}

		int max_font_height = m_font.GetHeight();

		preferred_size.set_height(
		        max_font_height + default_numberslider_padding.vsum());	// top padding: 2, bottom padding: 2

		preferred_size.set_width(
		        max_font_height + default_numberslider_padding.hsum()
		                + radius_plus + 100);

		return preferred_size;
	}

	void NumericalSlider::UpdateSlider(const WidgetUpdateRequest& request)
	{

	}

	void NumericalSlider::UpdateGeometry (const WidgetUpdateRequest& request)
	{
		switch (request.type()) {

			case WidgetSize: {
				const Size* size_p = static_cast<const Size*>(request.data());
				VertexTool tool;
				tool.Setup(*size_p, DefaultBorderWidth(), round_corner_type(),
								round_corner_radius());
				m_inner->Bind();
				tool.SetInnerBufferData(m_inner.get());
				m_outer->Bind();
				tool.SetOuterBufferData(m_outer.get());

				m_slider->Bind();
				std::vector<GLfloat> slide_verts;
				GenerateSliderVertices(*size_p, DefaultBorderWidth(), round_corner_type(), round_corner_radius(), slide_verts);
				m_slider->SetData(sizeof(GLfloat) * slide_verts.size(), &slide_verts[0]);

				GLArrayBuffer::Reset();
				Refresh();
				break;
			}

			case WidgetRoundCornerType: {
				const int* type_p = static_cast<const int*>(request.data());
				VertexTool tool;
				tool.Setup(size(), DefaultBorderWidth(), *type_p,
								round_corner_radius());
				m_inner->Bind();
				tool.SetInnerBufferData(m_inner.get());
				m_outer->Bind();
				tool.SetOuterBufferData(m_outer.get());

				m_slider->Bind();
				std::vector<GLfloat> slide_verts;
				GenerateSliderVertices(size(), DefaultBorderWidth(), *type_p, round_corner_radius(), slide_verts);
				m_slider->SetData(sizeof(GLfloat) * slide_verts.size(), &slide_verts[0]);

				GLArrayBuffer::Reset();
				Refresh();
				break;
			}

			case WidgetRoundCornerRadius: {
				const float* radius_p =
								static_cast<const float*>(request.data());
				VertexTool tool;
				tool.Setup(size(), DefaultBorderWidth(), round_corner_type(),
								*radius_p);
				m_inner->Bind();
				tool.SetInnerBufferData(m_inner.get());
				m_outer->Bind();
				tool.SetOuterBufferData(m_outer.get());

				m_slider->Bind();
				std::vector<GLfloat> slide_verts;
				GenerateSliderVertices(size(), DefaultBorderWidth(), round_corner_type(), *radius_p, slide_verts);
				m_slider->SetData(sizeof(GLfloat) * slide_verts.size(), &slide_verts[0]);

				GLArrayBuffer::Reset();
				Refresh();
				break;
			}

			default:
				AbstractSlider<double>::UpdateGeometry(request);
		}
	}
	
	ResponseType NumericalSlider::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		RefPtr<GLSLProgram> program =
				Shaders::instance->default_triangle_program();
		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetVertexAttrib4fv("Color", Theme::instance->number_slider().inner_sel.data());
		program->SetUniform1i("AA", 0);

		if(hover()) {
			program->SetUniform1i("Gamma", 15);
		} else {
			program->SetUniform1i("Gamma", 0);
		}

		glBindVertexArray(m_vao[0]);

		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_corner_type()) + 2);

		program->SetUniform1i("Gamma", 0);
		program->SetVertexAttrib4f("Color", 0.4f, 0.4f, 0.4f, 1.f);

		glBindVertexArray(m_vao[2]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_corner_type() & ~(RoundTopRight | RoundBottomRight)) + 2);

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetVertexAttrib4fv("Color", Theme::instance->number_slider().outline.data());
		program->SetUniform1i("AA", 1);

		glBindVertexArray(m_vao[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_corner_type()) * 2 + 2);

		glBindVertexArray(0);

		program->Reset();

		if(m_title.size()) {
			m_font.Print(mvp, m_title);
		}

		return Accept;
	}
	
	void NumericalSlider::GenerateSliderVertices (const Size& size,
					float border,
					int round_type,
					float radius,
					std::vector<GLfloat>& vertices)
	{
		float rad = radius * Theme::instance->pixel();
		float radi = rad - border * Theme::instance->pixel();

		float veci[WIDGET_CURVE_RESOLU][2];

		round_type = round_type & ~(RoundTopRight | RoundBottomRight);

		float minx = 0.0;
		float miny = 0.0;
		float maxx = size.width();	// test code
		float maxy = size.height();

		float minxi = minx + border * Theme::instance->pixel();		// U.pixelsize; // boundbox inner
		float maxxi = maxx - border * Theme::instance->pixel(); 	// U.pixelsize;
		float minyi = miny + border * Theme::instance->pixel();		// U.pixelsize;
		float maxyi = maxy - border * Theme::instance->pixel();		// U.pixelsize;

		maxxi = (maxxi - minxi) * value() / (maximum() - minimum());

		int count = 0;
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

		unsigned int corner = round_type & RoundAll;
		while (corner != 0) {
			count += corner & 0x1;
			corner = corner >> 1;
		}
		unsigned int outline_vertex_number = 4 - count + count * WIDGET_CURVE_RESOLU;

		minsize = std::min(size.width() * hnum, size.height() * vnum);

		if (2.0f * rad > minsize)
			rad = 0.5f * minsize;

		if (2.0f * (radi + border * Theme::instance->pixel()) > minsize)
			radi = 0.5f * minsize - border * Theme::instance->pixel();	// U.pixelsize;

		// mult
		for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			veci[i][0] = radi * VertexTool::cornervec[i][0];
			veci[i][1] = radi * VertexTool::cornervec[i][1];
		}

			if (vertices.size() != ((outline_vertex_number + 2) * 2)) {
			vertices.resize((outline_vertex_number + 2) * 2);
		}

		// inner[0, 0] is the center of a triangle fan
		vertices[0] = (maxxi - minxi) / 2.f;
		vertices[1] = (maxyi - minyi) / 2.f;

		count = 1;

		// corner left-bottom
		if (round_type & RoundBottomLeft) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				vertices[count * 2] = minxi + veci[i][1];
				vertices[count * 2 + 1] = minyi + radi - veci[i][0];
			}
		} else {
			vertices[count * 2] = minxi;
			vertices[count * 2 + 1] = minyi;
			count++;
		}

		// corner right-bottom
		if (round_type & RoundBottomRight) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				vertices[count * 2] = maxxi - radi + veci[i][0];
				vertices[count * 2 + 1] = minyi + veci[i][1];
			}
		} else {
			vertices[count * 2] = maxxi;
			vertices[count * 2 + 1] = minyi;
			count++;
		}

		// corner right-top
		if (round_type & RoundTopRight) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				vertices[count * 2] = maxxi - veci[i][1];
				vertices[count * 2 + 1] = maxyi - radi + veci[i][0];
			}
		} else {
			vertices[count * 2] = maxxi;
			vertices[count * 2 + 1] = maxyi;
			count++;
		}

		// corner left-top
		if (round_type & RoundTopLeft) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				vertices[count * 2] = minxi + radi - veci[i][0];
				vertices[count * 2 + 1] = maxyi - veci[i][1];
			}

		} else {
			vertices[count * 2] = minxi;
			vertices[count * 2 + 1] = maxyi;
			count++;
		}

		vertices[count * 2] = vertices[2];
		vertices[count * 2 + 1] = vertices[3];

	}
	
	ResponseType NumericalSlider::CursorEnterEvent (bool entered)
	{
		Refresh();
		return Accept;
	}

	void NumericalSlider::InitOnce()
	{
		set_round_corner_type(RoundAll);
		int h = m_font.GetHeight();
		set_size(h + round_corner_radius() * 2 + default_numberslider_padding.hsum(),
						h + default_numberslider_padding.vsum());
		set_round_corner_radius(size().height() / 2);

		set_value(50.0);

		VertexTool tool;
		tool.Setup(size(), DefaultBorderWidth(), round_corner_type(),
						round_corner_radius());

		glGenVertexArrays(3, m_vao);

		// generate buffer for inner
		glBindVertexArray(m_vao[0]);

		m_inner.reset(new GLArrayBuffer);
		m_inner->Generate();
		m_inner->Bind();

		tool.SetInnerBufferData(m_inner.get());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		// generate buffer for outer
		glBindVertexArray(m_vao[1]);

		m_outer.reset(new GLArrayBuffer);
		m_outer->Generate();
		m_outer->Bind();
		tool.SetOuterBufferData(m_outer.get());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		// generate buffer for slide bar
		glBindVertexArray(m_vao[2]);

		m_slider.reset(new GLArrayBuffer);
		m_slider->Generate();
		m_slider->Bind();

		std::vector<GLfloat> slide_verts;
		GenerateSliderVertices(size(), DefaultBorderWidth(), round_corner_type(), round_corner_radius(), slide_verts);
		m_slider->SetData(sizeof(GLfloat) * slide_verts.size(), &slide_verts[0]);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::Reset();
	}
}
