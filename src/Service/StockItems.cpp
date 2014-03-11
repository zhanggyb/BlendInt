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

#include <stdlib.h>

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Service/StockItems.hpp>
#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

SlideIcon::SlideIcon()
: AbstractRoundForm()
{
	set_size(14, 14);
	set_round_type(RoundAll);
	set_radius(7.0);

	InitOnce();
}

SlideIcon::~SlideIcon()
{

}

bool SlideIcon::Update(int type, const void* data)
{
	switch(type)	{

		case FormSize: {
			const Size* size_p = static_cast<const Size*>(data);
			Orientation shadedir = size_p->width() < size_p->height() ? Horizontal : Vertical;
			const Color& color = themes()->scroll.item;
			short shadetop = themes()->scroll.shadetop;
			short shadedown = themes()->scroll.shadedown;

			GenerateShadedFormBuffers(size_p,
					round_type(),
					radius(),
					color,
					shadetop,
					shadedown,
					shadedir,
					5,
					m_inner_buffer.get(),
					m_outer_buffer.get(),
					m_highlight_buffer.get()
					);

			return true;
		}

		case FormRoundType: {
			const Size* size_p = &(size());
			Orientation shadedir = size_p->width() < size_p->height() ? Horizontal : Vertical;
			const RoundType* round_p = static_cast<const RoundType*>(data);
			const Color& color = themes()->scroll.item;
			short shadetop = themes()->scroll.shadetop;
			short shadedown = themes()->scroll.shadedown;

			GenerateShadedFormBuffers(size_p,
					*round_p,
					radius(),
					color,
					shadetop,
					shadedown,
					shadedir,
					5,
					m_inner_buffer.get(),
					m_outer_buffer.get(),
					m_highlight_buffer.get()
					);
			return true;
		}

		case FormRoundRadius: {
			const Size* size_p = &(size());
			Orientation shadedir = size_p->width() < size_p->height() ? Horizontal : Vertical;
			const float* radius_p = static_cast<const float*>(data);
			const Color& color = themes()->scroll.item;
			short shadetop = themes()->scroll.shadetop;
			short shadedown = themes()->scroll.shadedown;

			GenerateShadedFormBuffers(size_p,
					round_type(),
					*radius_p,
					color,
					shadetop,
					shadedown,
					shadedir,
					5,
					m_inner_buffer.get(),
					m_outer_buffer.get(),
					m_highlight_buffer.get()
					);
			return true;
		}

		default:
			return false;
	}
}

void SlideIcon::Draw(const glm::mat4& mvp)
{
	RefPtr<GLSLProgram> program = ShaderManager::instance->default_widget_program();
	program->Use();

	GLint xy_attrib = program->GetAttributeLocation("xy");
	GLint color_attrib = program->GetAttributeLocation("color");

	program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));

	glEnableVertexAttribArray(xy_attrib);
	glEnableVertexAttribArray(color_attrib);

	m_inner_buffer->Bind();

	glVertexAttribPointer(xy_attrib, // attribute
						  2,			// number of elements per vertex, here (x,y)
						  GL_FLOAT,			 // the type of each element
						  GL_FALSE,			 // take our values as-is
						  sizeof(GLfloat) * 6,				 // stride
						  BUFFER_OFFSET(0)					 // offset of first element
						  );

	glVertexAttribPointer(color_attrib, // attribute
						  4,			// number of elements per vertex, here (x,y)
						  GL_FLOAT,			 // the type of each element
						  GL_FALSE,			 // take our values as-is
						  sizeof(GLfloat) * 6,				 // stride
						  BUFFER_OFFSET(2 * sizeof(GLfloat))					 // offset of first element
						  );

	glDrawArrays(GL_POLYGON, 0, m_inner_buffer->GetBufferSize() / (6 * sizeof(GLfloat)));

	glDisableVertexAttribArray(color_attrib);

	m_inner_buffer->Reset();

	GLfloat outline_color[4] = {themes()->scroll.outline.r() / 255.f,
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
		jitter.x = jit[j][0]; jitter.y = jit[j][1]; jitter.z = 0.0f;
		jitter_matrix = glm::translate(glm::mat4(1.0), jitter);
		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp * jitter_matrix));

		glDrawArrays(GL_QUAD_STRIP, 0, m_outer_buffer->GetBufferSize() / (2 * sizeof(GLfloat)));
	}

	m_outer_buffer->Reset();

	glDisableVertexAttribArray(xy_attrib);

	program->Reset();

	return;

	//if(down()) {
		DrawShadedInnerBuffer(m_inner_buffer.get());
	//} else {
		//DrawShadedInnerBuffer(m_highlight_buffer.get());
	//}

	// draw outline
	unsigned char tcol[4] = { themes()->scroll.outline.r(),
	        themes()->scroll.outline.g(), themes()->scroll.outline.b(),
	        themes()->scroll.outline.a() };

	tcol[3] = tcol[3] / WIDGET_AA_JITTER;
	glColor4ubv(tcol);

	DrawOutlineBuffer(m_outer_buffer.get());

}

void SlideIcon::InitOnce()
{
	m_inner_buffer.reset(new GLArrayBuffer);
	m_outer_buffer.reset(new GLArrayBuffer);
	m_highlight_buffer.reset(new GLArrayBuffer);


	Orientation shadedir = size().width() < size().height() ? Horizontal : Vertical;
	const Color& color = themes()->scroll.item;
	short shadetop = themes()->scroll.shadetop;
	short shadedown = themes()->scroll.shadedown;

	GenerateShadedFormBuffers(&size(),
			round_type(),
			radius(),
			color,
			shadetop,
			shadedown,
			shadedir,
			5,
			m_inner_buffer.get(),
			m_outer_buffer.get(),
			m_highlight_buffer.get()
			);
}

	StockItems* StockItems::instance = 0;

	bool StockItems::Initialize()
	{
		if (!instance)
			instance = new StockItems;

		return true;
	}

	void StockItems::Release()
	{
		if (instance) {
			delete instance;
			instance = 0;
		}
	}

	StockItems::StockItems()
	{
		CreateIcons();
	}

	StockItems::~StockItems()
	{
	}

	void StockItems::CreateIcons()
	{
		float vec[16][2];

		m_icon_menu.reset(new VertexIcon);
		m_icon_menu->set_name("Menu Icon");

		for(size_t i = 0; i < 6; i++)
		{
			vec[i][0] = 0.5 * 16 * VertexIcon::menu_tria_vert[i][0];
			vec[i][1] = 0.5 * 16 * VertexIcon::menu_tria_vert[i][1];
		}

		m_icon_menu->load (vec, 6, VertexIcon::menu_tria_face, 2);

		m_icon_circle.reset(new VertexIcon);
		m_icon_circle->set_name("Circle Icon");

		for(size_t i = 0; i < 16; i++)
		{
			vec[i][0] = 0.5 * 10 * VertexIcon::scroll_circle_vert[i][0];
			vec[i][1] = 0.5 * 10 * VertexIcon::scroll_circle_vert[i][1];
		}

		m_icon_circle->load (vec, 16, VertexIcon::scroll_circle_face, 14);

		m_icon_check.reset(new VertexIcon);
		m_icon_check->set_name("Checkbox Icon");

		for(size_t i = 0; i < 6; i++)
		{
			vec[i][0] = 0.5 * 14 * VertexIcon::check_tria_vert[i][0];
			vec[i][1] = 0.5 * 14 * VertexIcon::check_tria_vert[i][1];
		}

		m_icon_check->load (vec, 6, VertexIcon::check_tria_face, 4);

		m_icon_num.reset(new VertexIcon);
		m_icon_num->set_name("Number slider Icon");

		for(size_t i = 0; i < 3; i++)
		{
			vec[i][0] = 0.5 * 10 * VertexIcon::num_tria_vert[i][0];
			vec[i][1] = 0.5 * 10 * VertexIcon::num_tria_vert[i][1];
		}

		m_icon_num->load (vec, 3, VertexIcon::num_tria_face, 1);

		m_icon_slide.reset(new SlideIcon);
		m_icon_slide->set_name("Slide Icon");

	}
}


