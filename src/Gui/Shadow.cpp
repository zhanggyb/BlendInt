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

#include <cmath>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>

#include <BlendInt/Gui/Shadow.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Shadow::Shadow()
	: AbstractRoundForm(),
	  vao_(0)
	{
		set_size(100, 100);
		color_.set_value(0.f, 0.f, 0.f, 0.33f);

		InitializeShadow();
	}

	Shadow::Shadow(const Size& size, int round_type, float radius)
	: AbstractRoundForm(),
	  vao_(0)
	{
		set_size(size);
		set_round_type(round_type);
		set_radius(radius);
		color_.set_value(0.f, 0.f, 0.f, 0.33f);

		InitializeShadow();
	}

	Shadow::~Shadow()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void Shadow::SetColor(const Color& color)
	{
		if(color == color_) return;

		color_ = color;

		std::vector<GLfloat> vertices;
		GenerateShadowVertices(size(), round_type(), radius(), vertices);
		buffer_->bind();
		buffer_->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		buffer_->reset();
	}

	void Shadow::Draw (float x, float y, short gamma) const
	{
		Shaders::instance->frame_shadow_program()->use();

		int count = GetOutlineVertices(round_type());

		glUniform3f(Shaders::instance->location(Stock::FRAME_SHADOW_POSITION), x, y, 0.f);
		//glUniform1i(Shaders::instance->location(Stock::FRAME_SHADOW_GAMMA), gamma);
		//glUniform1i(Shaders::instance->location(Stock::FRAME_SHADOW_ANTI_ALIAS), 1);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, (count + 1) * 2);
		glBindVertexArray(0);

		GLSLProgram::reset();
	}

	void Shadow::PerformSizeUpdate(const Size& size)
	{
		std::vector<GLfloat> vertices;
		GenerateShadowVertices(size, round_type(), radius(), vertices);
		buffer_->bind();
		buffer_->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		buffer_->reset();

		set_size(size);
	}

	void Shadow::PerformRoundTypeUpdate(int type)
	{
		std::vector<GLfloat> vertices;
		GenerateShadowVertices(size(), type, radius(), vertices);
		buffer_->bind();
		buffer_->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		buffer_->reset();

		set_round_type(type);
	}

	void Shadow::PerformRoundRadiusUpdate(float radius)
	{
		std::vector<GLfloat> vertices;
		GenerateShadowVertices(size(), round_type(), radius, vertices);
		buffer_->bind();
		buffer_->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		buffer_->reset();

		set_radius(radius);
	}

	void Shadow::InitializeShadow()
	{
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		std::vector<GLfloat> vertices;
		GenerateShadowVertices(size(), round_type(), radius(), vertices);

		buffer_.reset(new GLArrayBuffer);
		buffer_->generate();
		buffer_->bind();
		buffer_->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::FRAME_SHADOW_COORD));
		//glEnableVertexAttribArray(Shaders::instance->location(Stock::FRAME_SHADOW_COLOR));

		glVertexAttribPointer(Shaders::instance->location(Stock::FRAME_SHADOW_COORD), 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6,
		        BUFFER_OFFSET(0));
		//glVertexAttribPointer(Shaders::instance->location(Stock::FRAME_SHADOW_COLOR), 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6,
		 //       BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

	void Shadow::GenerateShadowVertices(const Size& size, int round_type,
			float radius, std::vector<GLfloat>& vertices)
	{
		const float minx = 0.0f;
		const float miny = 0.0f;
		const float maxx = size.width();
		const float maxy = size.height();

		short x_offset = Theme::instance->shadow_offset_x();
		short y_offset = Theme::instance->shadow_offset_y();

		if (std::abs(x_offset) >= Theme::instance->shadow_width()) {
			x_offset = x_offset > 0 ? (Theme::instance->shadow_width() - 1) : (Theme::instance->shadow_width() + 1);
		}

		if (std::abs(x_offset) >= Theme::instance->shadow_width()) {
			y_offset = y_offset > 0 ? (Theme::instance->shadow_width() - 1) : (Theme::instance->shadow_width() + 1);
		}

		if (2.0f * radius > size.height())
			radius = 0.5f * size.height();

		int edge_vertex_count = GetOutlineVertices(round_type);
		unsigned int max_count = (edge_vertex_count + 1) * 6 * 2;

		if(vertices.size() != max_count) vertices.resize(max_count);

		int count = 0;

		float offset = Theme::instance->shadow_width();
		float radi = radius;
		float rado = radi + Theme::instance->shadow_width();

		// TODO: fine tune shade 1 ~ 2 for better look
		const float shade1 = -5.f / 255.f;
		const float shade2 = 27.5 / 255.f;

		short shadetop_x = x_offset * 5;
		short shadedown_x = -x_offset * 5;
		short shadetop_y = y_offset * 5;
		short shadedown_y = -y_offset * 5;

		const float facx = (maxx != minx) ? 1.0f / (maxx - minx) : 0.0f;
		const float facy = (maxy != miny) ? 1.0f / (maxy - miny) : 0.0f;

		float fx = 0.f;
		float fy = 0.f;

		/* start with left-top, anti clockwise */
		if (round_type & RoundTopLeft) {
			for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {

				vertices[count + 0] = minx + radi - radi * cornervec[j][0];
				vertices[count + 1] = maxy - radi * cornervec[j][1];

				fx = make_shaded_offset(shadetop_x, shadedown_x, facx * (vertices[count + 0] - minx));
				fy = make_shaded_offset(shadetop_y, shadedown_y, facy * (vertices[count + 1] - miny));

				vertices[count + 2] = color_.red() + shade1 + fx - fy;
				vertices[count + 3] = color_.green() + shade1 + fx - fy;
				vertices[count + 4] = color_.blue() + shade1 + fx - fy;
				vertices[count + 5] = color_.alpha();

				vertices[count + 6] = minx - offset + rado - rado * cornervec[j][0] + x_offset;
				vertices[count + 7] = maxy + offset - rado * cornervec[j][1] + y_offset;
				vertices[count + 8] = color_.red() + shade2;
				vertices[count + 9] = color_.green() + shade2;
				vertices[count + 10] = color_.blue() + shade2;
				vertices[count + 11] = 0.f;

				count += 12;
			}
		} else {

			vertices[count + 0] = minx;
			vertices[count + 1] = maxy;

			fx = make_shaded_offset(shadetop_x, shadedown_x, 0.f);
			fy = make_shaded_offset(shadetop_y, shadedown_y, 1.f);

			vertices[count + 2] = color_.red() + shade1 + fx - fy;
			vertices[count + 3] = color_.green() + shade1 + fx - fy;
			vertices[count + 4] = color_.blue() + shade1 + fx - fy;
			vertices[count + 5] = color_.alpha();

			vertices[count + 6] = minx - offset + x_offset;
			vertices[count + 7] = maxy + offset + y_offset;
			vertices[count + 8] = color_.red() + shade2;
			vertices[count + 9] = color_.green() + shade2;
			vertices[count + 10] = color_.blue() + shade2;
			vertices[count + 11] = 0.f;

			count += 12;
		}

		if (round_type & RoundBottomLeft) {
			for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {

				vertices[count + 0] = minx + radi * cornervec[j][1];
				vertices[count + 1] = miny + radi - radi * cornervec[j][0];

				fx = make_shaded_offset(shadetop_x, shadedown_x, facx * (vertices[count + 0] - minx));
				fy = make_shaded_offset(shadetop_y, shadedown_y, facy * (vertices[count + 1] - miny));

				vertices[count + 2] = color_.red() + shade1 + fx + fy;
				vertices[count + 3] = color_.green() + shade1 + fx + fy;
				vertices[count + 4] = color_.blue() + shade1 + fx + fy;
				vertices[count + 5] = color_.alpha();

				vertices[count + 6] = minx - offset + rado * cornervec[j][1] + x_offset;
				vertices[count + 7] = miny - offset + rado - rado * cornervec[j][0] + y_offset;
				vertices[count + 8] = color_.red() + shade2;
				vertices[count + 9] = color_.green() + shade2;
				vertices[count + 10] = color_.blue() + shade2;
				vertices[count + 11] = 0.f;

				count += 12;
			}
		} else {

			vertices[count + 0] = minx;
			vertices[count + 1] = miny;

			fx = make_shaded_offset(shadetop_x, shadedown_x, 0.f);
			fy = make_shaded_offset(shadetop_y, shadedown_y, 0.f);

			vertices[count + 2] = color_.red() + shade1 + fx + fy;
			vertices[count + 3] = color_.green() + shade1 + fx + fy;
			vertices[count + 4] = color_.blue() + shade1 + fx + fy;
			vertices[count + 5] = color_.alpha();

			vertices[count + 6] = minx - offset + x_offset;
			vertices[count + 7] = miny - offset + y_offset;
			vertices[count + 8] = color_.red() + shade2;
			vertices[count + 9] = color_.green() + shade2;
			vertices[count + 10] = color_.blue() + shade2;
			vertices[count + 11] = 0.f;

			count += 12;
		}

		if (round_type & RoundBottomRight) {
			for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {

				vertices[count + 0] = maxx - radi + radi * cornervec[j][0];
				vertices[count + 1] = miny + radi * cornervec[j][1];

				fx = make_shaded_offset(shadetop_x, shadedown_x, facx * (vertices[count + 0] - minx));
				fy = make_shaded_offset(shadetop_y, shadedown_y, facy * (vertices[count + 1] - miny));

				vertices[count + 2] = color_.red() + shade1 - fx + fy;
				vertices[count + 3] = color_.green() + shade1 - fx + fy;
				vertices[count + 4] = color_.blue() + shade1 - fx + fy;
				vertices[count + 5] = color_.alpha();

				vertices[count + 6] = maxx + offset - rado + rado * cornervec[j][0] + x_offset;
				vertices[count + 7] = miny - offset + rado * cornervec[j][1] + y_offset;
				vertices[count + 8] = color_.red() + shade2;
				vertices[count + 9] = color_.green() + shade2;
				vertices[count + 10] = color_.blue() + shade2;
				vertices[count + 11] = 0.f;

				count += 12;
			}
		} else {

			vertices[count + 0] = maxx;
			vertices[count + 1] = miny;

			fx = make_shaded_offset(shadetop_x, shadedown_x, 1.f);
			fy = make_shaded_offset(shadetop_y, shadedown_y, 0.f);

			vertices[count + 2] = color_.red() + shade1 - fx + fy;
			vertices[count + 3] = color_.green() + shade1 - fx + fy;
			vertices[count + 4] = color_.blue() + shade1 - fx + fy;
			vertices[count + 5] = color_.alpha();

			vertices[count + 6] = maxx + offset + x_offset;
			vertices[count + 7] = miny - offset + y_offset;
			vertices[count + 8] = color_.red() + shade2;
			vertices[count + 9] = color_.green() + shade2;
			vertices[count + 10] = color_.blue() + shade2;
			vertices[count + 11] = 0.f;

			count += 12;
		}

		if (round_type & RoundTopRight) {
			for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {

				vertices[count + 0] = maxx - radi * cornervec[j][1];
				vertices[count + 1] = maxy - radi + radi * cornervec[j][0];

				fx = make_shaded_offset(shadetop_x, shadedown_x, facx * (vertices[count + 0] - minx));
				fy = make_shaded_offset(shadetop_y, shadedown_y, facy * (vertices[count + 1] - miny));

				vertices[count + 2] = color_.red() + shade1 - fx - fy;
				vertices[count + 3] = color_.green() + shade1 - fx - fy;
				vertices[count + 4] = color_.blue() + shade1 - fx - fy;
				vertices[count + 5] = color_.alpha();

				vertices[count + 6] = maxx + offset - rado * cornervec[j][1] + x_offset;
				vertices[count + 7] = maxy + offset - rado + rado * cornervec[j][0] + y_offset;
				vertices[count + 8] = color_.red() + shade2;
				vertices[count + 9] = color_.green() + shade2;
				vertices[count + 10] = color_.blue() + shade2;
				vertices[count + 11] = 0.f;

				count += 12;
			}
		} else {

			vertices[count + 0] = maxx;
			vertices[count + 1] = maxy;

			fx = make_shaded_offset(shadetop_x, shadedown_x, 1.f);
			fy = make_shaded_offset(shadetop_y, shadedown_y, 1.f);

			vertices[count + 2] = color_.red() + shade1 - fx - fy;
			vertices[count + 3] = color_.green() + shade1 - fx - fy;
			vertices[count + 4] = color_.blue() + shade1 - fx - fy;
			vertices[count + 5] = color_.alpha();

			vertices[count + 6] = maxx + offset + x_offset;
			vertices[count + 7] = maxy + offset + y_offset;
			vertices[count + 8] = color_.red() + shade2;
			vertices[count + 9] = color_.green() + shade2;
			vertices[count + 10] = color_.blue() + shade2;
			vertices[count + 11] = 0.f;

			count += 12;
		}

		vertices[count + 0] = vertices[count - edge_vertex_count * 12 + 0];
		vertices[count + 1] = vertices[count - edge_vertex_count * 12 + 1];
		vertices[count + 2] = vertices[count - edge_vertex_count * 12 + 2];
		vertices[count + 3] = vertices[count - edge_vertex_count * 12 + 3];
		vertices[count + 4] = vertices[count - edge_vertex_count * 12 + 4];
		vertices[count + 5] = vertices[count - edge_vertex_count * 12 + 5];

		vertices[count + 6] = vertices[count - edge_vertex_count * 12 + 6];
		vertices[count + 7] = vertices[count - edge_vertex_count * 12 + 7];
		vertices[count + 8] = vertices[count - edge_vertex_count * 12 + 8];
		vertices[count + 9] = vertices[count - edge_vertex_count * 12 + 9];
		vertices[count + 10] = vertices[count - edge_vertex_count * 12 + 10];
		vertices[count + 11] = vertices[count - edge_vertex_count * 12 + 11];

		count += 12;

	}

	inline float Shadow::make_shaded_offset(short shadetop, short shadedown,
			float fact)
	{
		float faci = glm::clamp(fact - 0.5f / 255.f, 0.f, 1.f);
		float facm = 1.f - fact;

		return faci * (shadetop / 255.f) + facm * (shadedown / 255.f);
	}

	// --------------------------------------------

	ShadowMap::ShadowMap()
	: AbstractRoundForm(),
	  vao_(0)
	{
		set_size(100, 100);

		InitializeShadowMap();
	}

	ShadowMap::~ShadowMap()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void ShadowMap::Draw(float x, float y, short gamma) const
	{
		Shaders::instance->frame_shadow_program()->use();

		//int count = GetOutlineVertices(round_type());

		glUniform2f(Shaders::instance->location(Stock::FRAME_SHADOW_POSITION), x, y);
		glUniform1f(Shaders::instance->location(Stock::FRAME_SHADOW_FACTOR), 1.f);
		Theme::instance->shadow_texture()->bind();

		glBindVertexArray(vao_);
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4 * 2 * 3 + 2);

		for(int i = 0; i < 8; i++) {
			glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
		}

		glBindVertexArray(0);

		Theme::instance->shadow_texture()->reset();

		GLSLProgram::reset();
	}

	void ShadowMap::PerformSizeUpdate(const Size& size)
	{
		set_size(size);

		std::vector<GLfloat> vertices;
		GenerateShadowVertices(vertices);
		buffer_.bind();
		buffer_.set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		buffer_.reset();
	}

	void ShadowMap::PerformRoundTypeUpdate(int type)
	{
		set_round_type(type);

		std::vector<GLfloat> vertices;
		GenerateShadowVertices(vertices);
		buffer_.bind();
		buffer_.set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		buffer_.reset();
	}

	void ShadowMap::PerformRoundRadiusUpdate(float radius)
	{
		set_radius(radius);

		std::vector<GLfloat> vertices;
		GenerateShadowVertices(vertices);
		buffer_.bind();
		buffer_.set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		buffer_.reset();
	}

	void ShadowMap::InitializeShadowMap()
	{
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		std::vector<GLfloat> vertices;
		GenerateShadowVertices(vertices);

		buffer_.generate();
		buffer_.bind();
		buffer_.set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::FRAME_SHADOW_COORD));
		glEnableVertexAttribArray(Shaders::instance->location(Stock::FRAME_SHADOW_UV));

		glVertexAttribPointer(Shaders::instance->location(Stock::FRAME_SHADOW_COORD), 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
		        BUFFER_OFFSET(0));
		glVertexAttribPointer(Shaders::instance->location(Stock::FRAME_SHADOW_UV), 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
		        BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		buffer_.reset();
	}

	void ShadowMap::GenerateShadowVertices(std::vector<GLfloat>& vertices)
	{
		const float minx = 0.0f;
		const float miny = 0.0f;
		const float maxx = size().width();
		const float maxy = size().height();

		float width = 25.f;

		if(vertices.size() != 4 * 4 * 8) {
			vertices.resize(4 * 4 * 8);
		}

		// top-left

		vertices[0] = minx;
		vertices[1] = maxy;
		vertices[2] = 100 / 512.f;
		vertices[3] = 100 / 512.f;

		vertices[4] = minx;
		vertices[5] = maxy + width;
		vertices[6] = 100 / 512.f;
		vertices[7] = 0.f;

		vertices[8] = minx - width;
		vertices[9] = maxy;
		vertices[10] = 0.f;
		vertices[11] = 100 / 512.f;

		vertices[12] = minx - width;
		vertices[13] = maxy + width;
		vertices[14] = 0.f;
		vertices[15] = 0.f;

		// left

		vertices[16] = minx;
		vertices[17] = maxy;
		vertices[18] = 100 / 512.f;
		vertices[19] = 100 / 512.f;

		vertices[20] = minx - width;
		vertices[21] = maxy;
		vertices[22] = 0.f;
		vertices[23] = 100 / 512.f;

		vertices[24] = minx;
		vertices[25] = miny;
		vertices[26] = 100 / 512.f;
		vertices[27] = 101 / 512.f;

		vertices[28] = minx - width;
		vertices[29] = miny;
		vertices[30] = 0.f;
		vertices[31] = 101 / 512.f;

		// bottom-left

		vertices[32] = minx;
		vertices[33] = miny;
		vertices[34] = 100 / 512.f;
		vertices[35] = (512 - 100) / 512.f;

		vertices[36] = minx - width;
		vertices[37] = miny;
		vertices[38] = 0.f;
		vertices[39] = (512 - 100) / 512.f;

		vertices[40] = minx;
		vertices[41] = miny - width;
		vertices[42] = 100 / 512.f;
		vertices[43] = 1.f;

		vertices[44] = minx - width;
		vertices[45] = miny - width;
		vertices[46] = 0.f;
		vertices[47] = 1.f;

		// bottom

		vertices[48] = minx;
		vertices[49] = miny;
		vertices[50] = 100 / 512.f;
		vertices[51] = (512 - 100) / 512.f;

		vertices[52] = minx;
		vertices[53] = miny - width;
		vertices[54] = 100 / 512.f;
		vertices[55] = 1.f;

		vertices[56] = maxx;
		vertices[57] = miny;
		vertices[58] = 101 / 512.f;
		vertices[59] = (512 - 100) / 512.f;

		vertices[60] = maxx;
		vertices[61] = miny - width;
		vertices[62] = 101 / 512.f;
		vertices[63] = 1.f;

		// bottom right

		vertices[64] = maxx;
		vertices[65] = miny;
		vertices[66] = (512 - 100) / 512.f;
		vertices[67] = (512 - 100) / 512.f;

		vertices[68] = maxx;
		vertices[69] = miny - width;
		vertices[70] = (512 - 100) / 512.f;
		vertices[71] = 1.f;

		vertices[72] = maxx + width;
		vertices[73] = miny;
		vertices[74] = 1.f;
		vertices[75] = (512 - 100) / 512.f;

		vertices[76] = maxx + width;
		vertices[77] = miny - width;
		vertices[78] = 1.f;
		vertices[79] = 1.f;


		// right

		vertices[80] = maxx;
		vertices[81] = miny;
		vertices[82] = (512 - 100) / 512.f;
		vertices[83] = 100 / 512.f;

		vertices[84] = maxx + width;
		vertices[85] = miny;
		vertices[86] = 1.f;
		vertices[87] = 100 / 512.f;

		vertices[88] = maxx;
		vertices[89] = maxy;
		vertices[90] = (512 - 100) / 512.f;
		vertices[91] = 101 / 512.f;

		vertices[92] = maxx + width;
		vertices[93] = maxy;
		vertices[94] = 1.f;
		vertices[95] = 101 / 512.f;

		// top right

		vertices[96] = maxx;
		vertices[97] = maxy;
		vertices[98] = (512 - 100) / 512.f;
		vertices[99] = 100 / 512.f;

		vertices[100] = maxx + width;
		vertices[101] = maxy;
		vertices[102] = 1.f;
		vertices[103] = 100 / 512.f;

		vertices[104] = maxx;
		vertices[105] = maxy + width;
		vertices[106] = (512 - 100) / 512.f;
		vertices[107] = 0.f;

		vertices[108] = maxx + width;
		vertices[109] = maxy + width;
		vertices[110] = 1.f;
		vertices[111] = 0.f;

		// top

		vertices[112] = maxx;
		vertices[113] = maxy;
		vertices[114] = (512 - 100) / 512.f;
		vertices[115] = 100 / 512.f;

		vertices[116] = maxx;
		vertices[117] = maxy + width;
		vertices[118] = (512 - 100) / 512.f;
		vertices[119] = 0.f;

		vertices[120] = minx;
		vertices[121] = maxy;
		vertices[122] = (512 - 100 - 1) / 512.f;
		vertices[123] = 100 / 512.f;

		vertices[124] = minx;
		vertices[125] = maxy + width;
		vertices[126] = (512 - 100 - 1) / 512.f;
		vertices[127] = 0.f;

		/*

		if(vertices.size() != (4 * 2 * 4 * 3 + 4 * 2)) {
			vertices.resize(4 * 2 * 4 * 3 + 4 * 2);
		}

		float width = 100.f;

		// ---- top-left

		vertices[0] = minx + radius();
		vertices[1] = maxy;
		vertices[2] = 200 / 512.f;
		vertices[3] = 100 / 512.f;

		vertices[4] = minx + radius();
		vertices[5] = maxy + width;
		vertices[6] = 200 / 512.f;
		vertices[7] = 0.f;

		vertices[8] = minx;
		vertices[9] = maxy;
		vertices[10] = 100 / 512.f;
		vertices[11] = 100 / 512.f;

		vertices[12] = minx - width;
		vertices[13] = maxy + width;
		vertices[14] = 0.f;
		vertices[15] = 0.f;

		vertices[16] = minx;
		vertices[17] = maxy - radius();
		vertices[18] = 100 / 512.f;
		vertices[19] = 200 / 512.f;

		vertices[20] = minx - width;
		vertices[21] = maxy - radius();
		vertices[22] = 0.f;
		vertices[23] = 200 / 512.f;

		// ---- bottom-left

		vertices[24] = minx;
		vertices[25] = miny + radius();
		vertices[26] = 100 / 512.f;
		vertices[27] = (512 - 200) / 512.f;

		vertices[28] = minx - width;
		vertices[29] = miny + radius();
		vertices[30] = 0.f;
		vertices[31] = (512 - 200) / 512.f;

		vertices[32] = minx;
		vertices[33] = miny;
		vertices[34] = 100 / 512.f;
		vertices[35] = (512 - 100) / 512.f;

		vertices[36] = minx - width;
		vertices[37] = miny - width;
		vertices[38] = 0.f;
		vertices[39] = 1.f;

		vertices[40] = minx + radius();
		vertices[41] = miny;
		vertices[42] = 200 / 512.f;
		vertices[43] = (512 - 100) / 512.f;

		vertices[44] = minx + radius();
		vertices[45] = miny - width;
		vertices[46] = 200 / 512.f;
		vertices[47] = 1.f;

		// ---- bottom-right

		vertices[48] = maxx - radius();
		vertices[49] = miny;
		vertices[50] = (512 - 200) / 512.f;
		vertices[51] = (512 - 100) / 512.f;

		vertices[52] = maxx - radius();
		vertices[53] = miny - width;
		vertices[54] = (512 - 200) / 512.f;
		vertices[55] = 1.f;

		vertices[56] = maxx;
		vertices[57] = miny;
		vertices[58] = (512 - 100) / 512.f;
		vertices[59] = (512 - 100) / 512.f;

		vertices[60] = maxx + width;
		vertices[61] = miny - width;
		vertices[62] = 1.f;
		vertices[63] = 1.f;

		vertices[64] = maxx;
		vertices[65] = miny + radius();
		vertices[66] = (512 - 100) / 512.f;
		vertices[67] = (512 - 200) / 512.f;

		vertices[68] = maxx + width;
		vertices[69] = miny + radius();
		vertices[70] = 1.f;
		vertices[71] = (512 - 200) / 512.f;

		// ---- top-right

		vertices[72] = maxx;
		vertices[73] = maxy - radius();
		vertices[74] = (512 - 100) / 512.f;
		vertices[75] = 200 / 512.f;

		vertices[76] = maxx + width;
		vertices[77] = maxy - radius();
		vertices[78] = 1.f;
		vertices[79] = 200 / 512.f;

		vertices[80] = maxx;
		vertices[81] = maxy;
		vertices[82] = (512 - 100) / 512.f;
		vertices[83] = 100 / 512.f;

		vertices[84] = maxx + width;
		vertices[85] = maxy + width;
		vertices[86] = 1.f;
		vertices[87] = 0.f;

		vertices[88] = maxx - radius();
		vertices[89] = maxy;
		vertices[90] = (512 - 200) / 512.f;
		vertices[91] = 100 / 512.f;

		vertices[92] = maxx - radius();
		vertices[93] = maxy + width;
		vertices[94] = (512 - 200) / 512.f;
		vertices[95] = 0.f;

		// ---- top-left

		vertices[96] = vertices[0];
		vertices[97] = vertices[1];
		vertices[98] = vertices[2];
		vertices[99] = vertices[3];

		vertices[100] = vertices[4];
		vertices[101] = vertices[5];
		vertices[102] = vertices[6];
		vertices[103] = vertices[7];

		*/

	}

}
