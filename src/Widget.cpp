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
	  m_border_width(1.0),
	  m_emboss(true)
	{
		set_minimal_size(0, 0);
		resize(120, 80);
		set_preferred_size(120, 80);
	}

	Widget::Widget(AbstractWidget* parent)
	: AbstractWidget(parent),
	  m_border_width(1.0),
	  m_emboss(true)
	{
		set_minimal_size(0, 0);
		resize(120, 80);
		set_preferred_size(120, 80);
	}

	Widget::~Widget()
	{

	}

	void Widget::set_round_type(int type)
	{
		if(AbstractExpForm::set_round_type(type)) {
			fire_property_changed_event(FormRoundType);
		}
	}

	void Widget::set_radius(float radius)
	{
		if(AbstractExpForm::set_radius(radius)) {
			fire_property_changed_event(FormRoundType);
		}
	}

	void Widget::set_emboss(bool emboss)
	{
		m_emboss = emboss;

		// TODO: call update
	}

	void Widget::set_border_width(float width)
	{
		if(width > 0.0)
			m_border_width = width;
		else
			return;

		// TODO: call update
	}


	void Widget::update (int type, const void* data)
	{
		switch (type) {

			case FormSize: {
				const Size* size_p = static_cast<const Size*>(data);
				update_shape(size_p);
				break;
			}

			default:
				break;
		}
	}

	void Widget::render()
	{
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
		draw_gl_buffer(WidgetBufferKeyInner);

		// draw outline
		unsigned char tcol[4] = { themes()->regular.outline.r(),
		        themes()->regular.outline.g(),
		        themes()->regular.outline.b(),
		        themes()->regular.outline.a()};
		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		glColor4ubv(tcol);

		draw_gl_buffer_anti_alias(WidgetBufferKeyOuter);

		if(m_emboss) {
			glColor4f(1.0f, 1.0f, 1.0f, 0.02f);
			draw_gl_buffer_anti_alias(WidgetBufferKeyEmboss);
		}

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

	void Widget::draw_gl_buffer(int key, int mode)
	{
		m_glbuffer.select(key);
		m_glbuffer.bind();
		glVertexPointer(2, GL_FLOAT, 0, 0);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawArrays(mode, 0, m_glbuffer.vertices());
		glDisableClientState(GL_VERTEX_ARRAY);
		m_glbuffer.unbind();
	}

	void Widget::draw_shaded_gl_buffer(int key, int mode)
	{
		m_glbuffer.select(key);
		m_glbuffer.bind();

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
		glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 6, BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glDrawArrays(mode, 0, glbuffer().vertices());

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		m_glbuffer.unbind();
	}

	void Widget::draw_gl_buffer_anti_alias(int key, int mode)
	{
		m_glbuffer.select (key);
		m_glbuffer.bind();

		glEnableClientState(GL_VERTEX_ARRAY);
		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			glTranslatef(jit[j][0], jit[j][1], 0.0f);
			glVertexPointer(2, GL_FLOAT, 0, 0);
			glDrawArrays(mode, 0, m_glbuffer.vertices());
			glTranslatef(-jit[j][0], -jit[j][1], 0.0f);
		}
		glDisableClientState(GL_VERTEX_ARRAY);
		m_glbuffer.unbind();
	}

	void Widget::update_shape(const Size* size)
	{
		// the basic widget don't use shaded color

		float outer_v[WIDGET_SIZE_MAX][2];	// vertices for drawing outline
		float inner_v[WIDGET_SIZE_MAX][2];	// vertices for drawing inner

		VerticesSum vert_sum;

		vert_sum = generate_vertices(size, border_width(), inner_v, outer_v);

		m_glbuffer.create(WidgetBufferKeyInner);
		m_glbuffer.select(WidgetBufferKeyInner);
		m_glbuffer.set_property(vert_sum.total, sizeof(inner_v[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		m_glbuffer.bind();
		m_glbuffer.upload(inner_v);
		m_glbuffer.unbind();

		// the quad strip for outline

		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */

		verts_to_quad_strip (inner_v, outer_v, vert_sum.total, quad_strip);

		m_glbuffer.create(WidgetBufferKeyOuter);
		m_glbuffer.select(WidgetBufferKeyOuter);
		m_glbuffer.set_property(vert_sum.total * 2 + 2, sizeof(quad_strip[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

		m_glbuffer.bind();
		m_glbuffer.upload(quad_strip);
		m_glbuffer.unbind();

		if (m_emboss) {
			//float quad_strip_emboss[WIDGET_SIZE_MAX * 2][2]; /* only for emboss */
			verts_to_quad_strip_open(outer_v, vert_sum.half, quad_strip);

			m_glbuffer.create(WidgetBufferKeyEmboss);
			m_glbuffer.select(WidgetBufferKeyEmboss);
			m_glbuffer.set_property(vert_sum.half * 2, sizeof(quad_strip[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

			m_glbuffer.bind();
			m_glbuffer.upload(quad_strip);
			m_glbuffer.unbind();
		}
	}

}
