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

#include <GL/glew.h>
#include <assert.h>
#include <algorithm>

#include <iostream>

#include <BlendInt/Widget.hpp>

#include <BlendInt/Types.hpp>
#include <BlendInt/Coord.hpp>
#include <BlendInt/Color.hpp>

#include <BlendInt/Interface.hpp>
#include <BlendInt/Theme.hpp>

namespace BlendInt {

	Widget::Widget()
	: AbstractWidget(),
	  m_border_width(1.0)
	{
		set_minimal_size(0, 0);
		resize(120, 80);
		set_preferred_size(120, 80);
	}

	Widget::Widget(AbstractWidget* parent)
	: AbstractWidget(parent),
	  m_border_width(1.0)
	{
		set_minimal_size(0, 0);
		resize(120, 80);
		set_preferred_size(120, 80);
	}

	Widget::~Widget()
	{

	}

	void Widget::set_border_width(float width)
	{
		if(width < 0.0) return;

		// TODO: call update
		update(WidgetBorderWidth, &width);

		m_border_width = width;
	}

	void Widget::update (int type, const void* data)
	{
		// do nothing in this base class
	}

	void Widget::render()
	{
		float outer_v[4][2];	// vertices for drawing outline
		float inner_v[4][2];	// vertices for drawing inner

		generate_rect_vertices(&(size()), border_width(), inner_v, outer_v);

		float quad_strip[4 * 2 + 2][2]; /* + 2 because the last pair is wrapped */

		verts_to_quad_strip (inner_v, outer_v, 4, quad_strip);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(),
					 position().y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// draw inner, simple fill
		glColor4ub(themes()->regular.inner.r(),
		        themes()->regular.inner.g(),
		        themes()->regular.inner.b(),
		        themes()->regular.inner.a());

		draw_inner(inner_v, 4);

		// draw outline
		unsigned char tcol[4] = { themes()->regular.outline.r(),
		        themes()->regular.outline.g(),
		        themes()->regular.outline.b(),
		        themes()->regular.outline.a()};
		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		glColor4ubv(tcol);

		draw_outline(quad_strip, 4 * 2 + 2);

		glDisable(GL_BLEND);
		glPopMatrix();
	}

	void Widget::press_key (KeyEvent* event)
	{
	}

	void Widget::press_context_menu (ContextMenuEvent* event)
	{
	}

	void Widget::release_context_menu (ContextMenuEvent* event)
	{
	}

	void Widget::press_mouse (MouseEvent* event)
	{
	}

	void Widget::release_mouse (MouseEvent* event)
	{
	}

	void Widget::move_mouse (MouseEvent* event)
	{
	}

	void Widget::draw_outline(const float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2], int num)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			glTranslatef(jit[j][0], jit[j][1], 0.0f);
			glVertexPointer(2, GL_FLOAT, 0, quad_strip);
			glDrawArrays(GL_QUAD_STRIP, 0, num);
			glTranslatef(-jit[j][0], -jit[j][1], 0.0f);
		}
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void Widget::draw_inner(const float inner_v[WIDGET_SIZE_MAX][2], int num)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, inner_v);

		glDrawArrays(GL_POLYGON, 0, num);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void Widget::draw_inner_buffer(GLBuffer* buffer, size_t index, int mode)
	{
		buffer->select(index);
		buffer->bind();
		glVertexPointer(2, GL_FLOAT, 0, 0);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawArrays(mode, 0, buffer->vertices());
		glDisableClientState(GL_VERTEX_ARRAY);
		buffer->unbind();
	}

	void Widget::draw_shaded_inner_buffer(GLBuffer* buffer, size_t index, int mode)
	{
		buffer->select(index);
		buffer->bind();

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
		glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 6, BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glDrawArrays(mode, 0, buffer->vertices());

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		buffer->unbind();
	}

	void Widget::draw_outline_buffer(GLBuffer* buffer, size_t index, int mode)
	{
		buffer->select (index);
		buffer->bind();

		glEnableClientState(GL_VERTEX_ARRAY);
		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			glTranslatef(jit[j][0], jit[j][1], 0.0f);
			glVertexPointer(2, GL_FLOAT, 0, 0);
			glDrawArrays(mode, 0, buffer->vertices());
			glTranslatef(-jit[j][0], -jit[j][1], 0.0f);
		}
		glDisableClientState(GL_VERTEX_ARRAY);
		buffer->unbind();
	}

	void Widget::generate_form_buffer(const Size* size, bool emboss, int round_type, float radius, GLBuffer* buffer)
	{
		float outer_v[WIDGET_SIZE_MAX][2];	// vertices for drawing outline
		float inner_v[WIDGET_SIZE_MAX][2];	// vertices for drawing inner

		VerticesSum vert_sum;

		vert_sum = generate_round_vertices(size, border_width(), round_type, radius, inner_v, outer_v);

		if(emboss)
			buffer->generate(3);
		else
			buffer->generate(2);

		buffer->select(0);
		buffer->set_property(vert_sum.total, sizeof(inner_v[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		buffer->bind();
		buffer->upload(inner_v);
		buffer->unbind();

		// the quad strip for outline

		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */

		verts_to_quad_strip (inner_v, outer_v, vert_sum.total, quad_strip);

		buffer->select(1);
		buffer->set_property(vert_sum.total * 2 + 2, sizeof(quad_strip[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

		buffer->bind();
		buffer->upload(quad_strip);
		buffer->unbind();

		if(emboss) {
			//float quad_strip_emboss[WIDGET_SIZE_MAX * 2][2]; /* only for emboss */
			verts_to_quad_strip_open(outer_v, vert_sum.half, quad_strip);

			buffer->select(2);
			buffer->set_property(vert_sum.half * 2, sizeof(quad_strip[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

			buffer->bind();
			buffer->upload(quad_strip);
			buffer->unbind();
		}
	}


	void Widget::generate_form_buffer(const Size* size, bool emboss, GLBuffer* buffer)
	{
		float outer_v[WIDGET_SIZE_MAX][2];	// vertices for drawing outline
		float inner_v[WIDGET_SIZE_MAX][2];	// vertices for drawing inner

		VerticesSum vert_sum;

		vert_sum = generate_round_vertices(size, border_width(), RoundNone, 1.0, inner_v, outer_v);

		if(emboss)
			buffer->generate(3);
		else
			buffer->generate(2);

		buffer->select(0);
		buffer->set_property(vert_sum.total, sizeof(inner_v[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		buffer->bind();
		buffer->upload(inner_v);
		buffer->unbind();

		// the quad strip for outline

		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */

		verts_to_quad_strip (inner_v, outer_v, vert_sum.total, quad_strip);

		buffer->select(1);
		buffer->set_property(vert_sum.total * 2 + 2, sizeof(quad_strip[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

		buffer->bind();
		buffer->upload(quad_strip);
		buffer->unbind();

		if(emboss) {
			//float quad_strip_emboss[WIDGET_SIZE_MAX * 2][2]; /* only for emboss */
			verts_to_quad_strip_open(outer_v, vert_sum.half, quad_strip);

			buffer->select(2);
			buffer->set_property(vert_sum.half * 2, sizeof(quad_strip[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

			buffer->bind();
			buffer->upload(quad_strip);
			buffer->unbind();
		}
	}

//	void Widget::update_shape(const Size* size)
//	{
//		// the basic widget don't use shaded color
//
//		float outer_v[WIDGET_SIZE_MAX][2];	// vertices for drawing outline
//		float inner_v[WIDGET_SIZE_MAX][2];	// vertices for drawing inner
//
//		VerticesSum vert_sum;
//
//		vert_sum = generate_vertices(size, border_width(), inner_v, outer_v);
//
//		m_inner_buffer->generate();
//		m_inner_buffer->select(0);
//		m_inner_buffer->set_property(vert_sum.total, sizeof(inner_v[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
//		m_inner_buffer->bind();
//		m_inner_buffer->upload(inner_v);
//		m_inner_buffer->unbind();
//
//		// the quad strip for outline
//
//		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */
//
//		verts_to_quad_strip (inner_v, outer_v, vert_sum.total, quad_strip);
//
//		m_outer_buffer->generate();
//		m_outer_buffer->select(0);
//		m_outer_buffer->set_property(vert_sum.total * 2 + 2, sizeof(quad_strip[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
//
//		m_outer_buffer->bind();
//		m_outer_buffer->upload(quad_strip);
//		m_outer_buffer->unbind();
//
//		if (m_emboss) {
//			//float quad_strip_emboss[WIDGET_SIZE_MAX * 2][2]; /* only for emboss */
//			verts_to_quad_strip_open(outer_v, vert_sum.half, quad_strip);
//
//			m_emboss_buffer->generate();
//			m_emboss_buffer->select(0);
//			m_emboss_buffer->set_property(vert_sum.half * 2, sizeof(quad_strip[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
//
//			m_emboss_buffer->bind();
//			m_emboss_buffer->upload(quad_strip);
//			m_emboss_buffer->unbind();
//		}
//	}

}
