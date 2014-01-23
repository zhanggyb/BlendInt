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
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

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

	float Widget::default_border_width = 1.0f;

	void Widget::SetDefaultBorderWidth(float border)
	{
		default_border_width = border;
	}

	float Widget::DefaultBorderWidth()
	{
		return default_border_width;
	}

	Widget::Widget()
	: AbstractWidget()
	{
		set_minimal_size(0, 0);
		set_size(120, 80);
		set_preferred_size(120, 80);
	}

	Widget::Widget(AbstractWidget* parent)
	: AbstractWidget(parent)
	{
		set_minimal_size(0, 0);
		set_size(120, 80);
		set_preferred_size(120, 80);
	}

	Widget::~Widget()
	{

	}

	void Widget::Update (int type, const void* data)
	{
		// do nothing in this base class
	}

	void Widget::Draw()
	{
		float outer_v[4][2];	// vertices for drawing outline
		float inner_v[4][2];	// vertices for drawing inner

		generate_rect_vertices(&(size()), DefaultBorderWidth(), inner_v, outer_v);

		float quad_strip[4 * 2 + 2][2]; /* + 2 because the last pair is wrapped */

		verts_to_quad_strip (inner_v, outer_v, 4, quad_strip);

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
	}

	void Widget::CursorEnterEvent(bool entered)
	{

	}

	void Widget::KeyPressEvent (KeyEvent* event)
	{
	}

	void Widget::ContextMenuPressEvent (ContextMenuEvent* event)
	{
	}

	void Widget::ContextMenuReleaseEvent (ContextMenuEvent* event)
	{
	}

	void Widget::MousePressEvent (MouseEvent* event)
	{
	}

	void Widget::MouseReleaseEvent (MouseEvent* event)
	{
	}

	void Widget::MouseMoveEvent (MouseEvent* event)
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

	void Widget::draw_inner_buffer(AbstractGLBuffer* buffer, size_t index, int mode)
	{
		buffer->select(index);
		buffer->Bind();
		glVertexPointer(2, GL_FLOAT, 0, 0);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawArrays(mode, 0, buffer->Vertices());
		glDisableClientState(GL_VERTEX_ARRAY);
		buffer->Unbind();
	}

	void Widget::DrawInnerBuffer (GLArrayBufferF* buffer, int mode)
	{
		//if(!buffer) return;

		buffer->bind();
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));
		glDrawArrays(mode, 0, buffer->vertices());
		glDisableClientState(GL_VERTEX_ARRAY);
		buffer->unbind();
	}

	void Widget::draw_shaded_inner_buffer(AbstractGLBuffer* buffer, size_t index, int mode)
	{
		buffer->select(index);
		buffer->Bind();

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
		glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 6, BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glDrawArrays(mode, 0, buffer->Vertices());

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		buffer->Unbind();
	}

	void Widget::DrawShadedInnerBuffer(GLArrayBufferF* buffer, int mode)
	{
		//if(!buffer) return;

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

	void Widget::draw_outline_buffer(AbstractGLBuffer* buffer, size_t index, int mode)
	{
		buffer->select (index);
		buffer->Bind();

		glEnableClientState(GL_VERTEX_ARRAY);
		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			glTranslatef(jit[j][0], jit[j][1], 0.0f);
			glVertexPointer(2, GL_FLOAT, 0, 0);
			glDrawArrays(mode, 0, buffer->Vertices());
			glTranslatef(-jit[j][0], -jit[j][1], 0.0f);
		}
		glDisableClientState(GL_VERTEX_ARRAY);
		buffer->Unbind();
	}

	void Widget::DrawOutlineBuffer(GLArrayBufferF* buffer, int mode)
	{
		//if(!buffer) return;

		buffer->bind();

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, 0);
		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			glTranslatef(jit[j][0], jit[j][1], 0.0f);
			glDrawArrays(mode, 0, buffer->vertices());
			glTranslatef(-jit[j][0], -jit[j][1], 0.0f);
		}
		glDisableClientState(GL_VERTEX_ARRAY);
		buffer->unbind();
	}

	void Widget::GenerateFormBuffer(const Size* size, int round_type,
			 float radius, GLArrayBufferF* inner_buffer,
			 GLArrayBufferF* outer_buffer,
			 GLArrayBufferF* emboss_buffer)
	{
		float outer_v[WIDGET_SIZE_MAX][2];	// vertices for drawing outline
		float inner_v[WIDGET_SIZE_MAX][2];	// vertices for drawing inner

		VerticesSum vert_sum;

		vert_sum = generate_round_vertices(size, DefaultBorderWidth(), round_type, radius, inner_v, outer_v);

		if(inner_buffer) {
			inner_buffer->Generate();
			inner_buffer->bind();
			inner_buffer->set_data(vert_sum.total, sizeof(inner_v[0]), inner_v[0]);
			inner_buffer->unbind();
		}

		// the quad strip for outline
		if(outer_buffer || emboss_buffer) {

			float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */

			if(outer_buffer) {

				verts_to_quad_strip (inner_v, outer_v, vert_sum.total, quad_strip);

				outer_buffer->Generate();
				outer_buffer->bind();
				outer_buffer->set_data((vert_sum.total * 2 + 2), sizeof(quad_strip[0]), quad_strip[0]);
				outer_buffer->unbind();
			}

			if(emboss_buffer) {

					//float quad_strip_emboss[WIDGET_SIZE_MAX * 2][2]; /* only for emboss */
				verts_to_quad_strip_open(outer_v, vert_sum.half, quad_strip);

				emboss_buffer->Generate();
				emboss_buffer->bind();
				emboss_buffer->set_data(vert_sum.half * 2, sizeof(quad_strip[0]), quad_strip[0]);
				emboss_buffer->unbind();
		}

		}
	}

	void Widget::GenerateFormBuffer(const Size* size, bool emboss, int round_type, float radius, GLArrayBufferMultiple* buffer)
	{
		float outer_v[WIDGET_SIZE_MAX][2];	// vertices for drawing outline
		float inner_v[WIDGET_SIZE_MAX][2];	// vertices for drawing inner

		VerticesSum vert_sum;

		vert_sum = generate_round_vertices(size, DefaultBorderWidth(), round_type, radius, inner_v, outer_v);

		if(emboss)
			buffer->Generate(3);
		else
			buffer->Generate(2);

		buffer->select(0);
		buffer->SetProperty(vert_sum.total, sizeof(inner_v[0]), GL_STATIC_DRAW);
		buffer->Bind();
		buffer->Upload(inner_v);
		buffer->Unbind();

		// the quad strip for outline

		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */

		verts_to_quad_strip (inner_v, outer_v, vert_sum.total, quad_strip);

		buffer->select(1);
		buffer->SetProperty(vert_sum.total * 2 + 2, sizeof(quad_strip[0]), GL_STATIC_DRAW);

		buffer->Bind();
		buffer->Upload(quad_strip);
		buffer->Unbind();

		if(emboss) {
			//float quad_strip_emboss[WIDGET_SIZE_MAX * 2][2]; /* only for emboss */
			verts_to_quad_strip_open(outer_v, vert_sum.half, quad_strip);

			buffer->select(2);
			buffer->SetProperty(vert_sum.half * 2, sizeof(quad_strip[0]), GL_STATIC_DRAW);

			buffer->Bind();
			buffer->Upload(quad_strip);
			buffer->Unbind();
		}
	}

	void Widget::GenerateRectFormBuffer(const Size* size, bool emboss, GLArrayBufferMultiple* buffer)
	{
		float outer_v[4][2];	// vertices for drawing outline
		float inner_v[4][2];	// vertices for drawing inner

		generate_rect_vertices(size, DefaultBorderWidth(), inner_v, outer_v);

		if(emboss)
			buffer->Generate(3);
		else
			buffer->Generate(2);

		buffer->select(0);
		buffer->SetProperty(4, sizeof(inner_v[0]), GL_STATIC_DRAW);
		buffer->Bind();
		buffer->Upload(inner_v);
		buffer->Unbind();

		// the quad strip for outline

		float quad_strip[4 * 2 + 2][2]; /* + 2 because the last pair is wrapped */

		verts_to_quad_strip (inner_v, outer_v, 4, quad_strip);

		buffer->select(1);
		buffer->SetProperty(4 * 2 + 2, sizeof(quad_strip[0]), GL_STATIC_DRAW);

		buffer->Bind();
		buffer->Upload(quad_strip);
		buffer->Unbind();

		if(emboss) {
			//float quad_strip_emboss[WIDGET_SIZE_MAX * 2][2]; /* only for emboss */
			verts_to_quad_strip_open(outer_v, 2, quad_strip);

			buffer->select(2);
			buffer->SetProperty(2 * 2, sizeof(quad_strip[0]), GL_STATIC_DRAW);

			buffer->Bind();
			buffer->Upload(quad_strip);
			buffer->Unbind();
		}
	}

	void Widget::GenerateShadedFormBuffer(const Size* size,
			float border,
			int round_type,
			float radius,
			const WidgetTheme* theme,
			Orientation shadedir,
			AbstractGLBuffer* buffer)
	{
		float inner_v[WIDGET_SIZE_MAX][6];	// vertices for drawing inner

		VerticesSum vert_sum;

		vert_sum = generate_round_vertices(size,
				border,
				round_type,
				radius,
				theme,
				shadedir,
				inner_v);

		buffer->Generate(1);
		buffer->select(0);

		buffer->SetProperty(vert_sum.total, sizeof(inner_v[0]), GL_STATIC_DRAW);
		buffer->Bind();
		buffer->Upload(inner_v);
		buffer->Unbind();
	}

	void Widget::GenerateShadedFormBuffers (const Size* size,
								   int round_type,
								   float radius,
								   const WidgetTheme* theme,
								   GLArrayBufferF* inner_buffer_p,
								   GLArrayBufferF* outer_buffer_p,
								   GLArrayBufferF* highlight_buffer_p)
	{
		/*
		float outer_v[WIDGET_SIZE_MAX][2];	// vertices for drawing outline
		float inner_v[WIDGET_SIZE_MAX][6];	// vertices for drawing inner

		VerticesSum vert_sum;

		vert_sum = generate_round_vertices(size,
				border,
				round_type,
				radius,
				theme,
				shadedir,
				inner_v, outer_v);

		if(highlight > 0)
			buffer->Generate(3);
		else
			buffer->Generate(2);

		buffer->select(0);

		buffer->SetProperty(vert_sum.total, sizeof(inner_v[0]), GL_STATIC_DRAW);
		buffer->Bind();
		buffer->Upload(inner_v);
		buffer->Unbind();

		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; // + 2 because the last pair is wrapped

		verts_to_quad_strip (inner_v, outer_v, vert_sum.total, quad_strip);

		buffer->select(1);
		buffer->SetProperty(vert_sum.total * 2 + 2, sizeof(quad_strip[0]), GL_STATIC_DRAW);

		buffer->Bind();
		buffer->Upload(quad_strip);
		buffer->Unbind();

		if(highlight > 0) {
			Color hcolor = theme->item;
			hcolor.highlight(hcolor, highlight);

			vert_sum = generate_round_vertices(size,
							border,
							round_type,
							radius,
							hcolor,
							theme->shadetop,
							theme->shadedown,
							shadedir,
							inner_v, outer_v);

			buffer->select(2);
			buffer->SetProperty(vert_sum.total, sizeof(inner_v[0]), GL_STATIC_DRAW);

			buffer->Bind();
			buffer->Upload(inner_v);
			buffer->Unbind();
		}
		*/
	}


	void Widget::GenerateShadedFormBuffers(const Size* size,
			float border,
			int round_type,
			float radius,
			const WidgetTheme* theme,
			Orientation shadedir,
			short highlight,
			GLArrayBufferMultiple* buffer)
	{
		float outer_v[WIDGET_SIZE_MAX][2];	// vertices for drawing outline
		float inner_v[WIDGET_SIZE_MAX][6];	// vertices for drawing inner

		VerticesSum vert_sum;

		vert_sum = generate_round_vertices(size,
				border,
				round_type,
				radius,
				theme,
				shadedir,
				inner_v, outer_v);

		if(highlight > 0)
			buffer->Generate(3);
		else
			buffer->Generate(2);

		buffer->select(0);

		buffer->SetProperty(vert_sum.total, sizeof(inner_v[0]), GL_STATIC_DRAW);
		buffer->Bind();
		buffer->Upload(inner_v);
		buffer->Unbind();

		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */

		verts_to_quad_strip (inner_v, outer_v, vert_sum.total, quad_strip);

		buffer->select(1);
		buffer->SetProperty(vert_sum.total * 2 + 2, sizeof(quad_strip[0]), GL_STATIC_DRAW);

		buffer->Bind();
		buffer->Upload(quad_strip);
		buffer->Unbind();

		if(highlight > 0) {
			Color hcolor = theme->item;
			hcolor.highlight(hcolor, highlight);

			vert_sum = generate_round_vertices(size,
							border,
							round_type,
							radius,
							hcolor,
							theme->shadetop,
							theme->shadedown,
							shadedir,
							inner_v, outer_v);

			buffer->select(2);
			buffer->SetProperty(vert_sum.total, sizeof(inner_v[0]), GL_STATIC_DRAW);

			buffer->Bind();
			buffer->Upload(inner_v);
			buffer->Unbind();
		}
	}

	void Widget::GenerateShadedFormBuffer (const Size* size,
			float border,
			int round_type,
			float radius,
			const Color& color,
			short shadetop,
			short shadedown,
			Orientation shadedir,
			GLArrayBufferF* buffer)
	{
		if(!buffer) return;

		float inner_v[WIDGET_SIZE_MAX][6];	// vertices for drawing inner

		VerticesSum vert_sum;

		vert_sum = generate_round_vertices(size,
				border,
				round_type,
				radius,
				color,
				shadetop,
				shadedown,
				shadedir,
				inner_v);

		buffer->Generate();
		buffer->bind();
		buffer->set_data(vert_sum.total, sizeof(inner_v[0]), inner_v[0], GL_STATIC_DRAW);
		buffer->unbind();
	}


	void Widget::GenerateShadedFormBuffer (const Size* size,
			float border,
			int round_type,
			float radius,
			const Color& color,
			short shadetop,
			short shadedown,
			Orientation shadedir,
			AbstractGLBuffer* buffer)
	{
		float inner_v[WIDGET_SIZE_MAX][6];	// vertices for drawing inner

		VerticesSum vert_sum;

		vert_sum = generate_round_vertices(size,
				border,
				round_type,
				radius,
				color,
				shadetop,
				shadedown,
				shadedir,
				inner_v);

		buffer->Generate(1);
		buffer->select(0);

		buffer->SetProperty(vert_sum.total, sizeof(inner_v[0]), GL_STATIC_DRAW);
		buffer->Bind();
		buffer->Upload(inner_v);
		buffer->Unbind();
	}

	void Widget::GenerateShadedFormBuffers (const Size* size, int round_type, float radius, const Color& color, short shadetop,
	        short shadedown, Orientation shadedir, short highlight,
	        GLArrayBufferF* inner_buffer,
	        GLArrayBufferF* outer_buffer,
	        GLArrayBufferF* highlight_buffer)
	{
		float outer_v[WIDGET_SIZE_MAX][2];	// vertices for drawing outline
		float inner_v[WIDGET_SIZE_MAX][6];	// vertices for drawing inner

		VerticesSum vert_sum;

		vert_sum = generate_round_vertices(size,
				default_border_width,
				round_type,
				radius,
				color,
				shadetop,
				shadedown,
				shadedir,
				inner_v, outer_v);

		if(inner_buffer) {
			inner_buffer->Generate();
			inner_buffer->bind();
			inner_buffer->set_data(vert_sum.total, sizeof(inner_v[0]), inner_v[0], GL_STATIC_DRAW);
			inner_buffer->unbind();
		}

		if(outer_buffer) {
			float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */
			verts_to_quad_strip (inner_v, outer_v, vert_sum.total, quad_strip);

			outer_buffer->Generate();
			outer_buffer->bind();
			outer_buffer->set_data((vert_sum.total * 2 + 2), sizeof(quad_strip[0]),
					quad_strip[0],
					GL_STATIC_DRAW);
			outer_buffer->unbind();
		}

		if(highlight_buffer) {
			Color hcolor = color;
			hcolor.highlight(hcolor, highlight);

			vert_sum = generate_round_vertices(size,
							default_border_width,
							round_type,
							radius,
							hcolor,
							shadetop,
							shadedown,
							shadedir,
							inner_v, outer_v);

			highlight_buffer->Generate();
			highlight_buffer->bind();
			highlight_buffer->set_data(vert_sum.total, sizeof(inner_v[0]), inner_v[0], GL_STATIC_DRAW);
			highlight_buffer->unbind();
		}

	}

	void Widget::GenerateShadedFormBuffers (const Size* size,
			float border,
			int round_type,
			float radius,
			const Color& color,
			short shadetop,
			short shadedown,
			Orientation shadedir,
			short highlight,
			GLArrayBufferMultiple* buffer)
	{
		float outer_v[WIDGET_SIZE_MAX][2];	// vertices for drawing outline
		float inner_v[WIDGET_SIZE_MAX][6];	// vertices for drawing inner

		VerticesSum vert_sum;

		vert_sum = generate_round_vertices(size,
				border,
				round_type,
				radius,
				color,
				shadetop,
				shadedown,
				shadedir,
				inner_v, outer_v);

		if(highlight > 0)
			buffer->Generate(3);
		else
			buffer->Generate(2);

		buffer->select(0);

		buffer->SetProperty(vert_sum.total, sizeof(inner_v[0]), GL_STATIC_DRAW);
		buffer->Bind();
		buffer->Upload(inner_v);
		buffer->Unbind();

		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */

		verts_to_quad_strip (inner_v, outer_v, vert_sum.total, quad_strip);

		buffer->select(1);
		buffer->SetProperty(vert_sum.total * 2 + 2, sizeof(quad_strip[0]), GL_STATIC_DRAW);

		buffer->Bind();
		buffer->Upload(quad_strip);
		buffer->Unbind();

		if(highlight > 0) {
			Color hcolor = color;
			hcolor.highlight(hcolor, highlight);

			vert_sum = generate_round_vertices(size,
							border,
							round_type,
							radius,
							hcolor,
							shadetop,
							shadedown,
							shadedir,
							inner_v, outer_v);

			buffer->select(2);
			buffer->SetProperty(vert_sum.total, sizeof(inner_v[0]), GL_STATIC_DRAW);

			buffer->Bind();
			buffer->Upload(inner_v);
			buffer->Unbind();
		}
	}

}

