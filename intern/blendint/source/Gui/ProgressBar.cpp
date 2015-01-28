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

#include <BlendInt/Gui/AbstractWindow.hpp>
#include <BlendInt/Gui/ProgressBar.hpp>

namespace BlendInt {

	ProgressBar::ProgressBar(Orientation orientation)
	: Widget(),
	  orientation_(orientation)
	{
		if(orientation == Horizontal) {
			set_size(200, 20);
		} else {
			set_size(20, 200);
		}

		glGenVertexArrays(2, vao_);

		InitializeProgressBar();
	}

	ProgressBar::~ProgressBar ()
	{
		glDeleteVertexArrays(2, vao_);
	}
	
	bool ProgressBar::IsExpandX () const
	{
		if(orientation_ == Horizontal) {
			return true;
		} else {
			return false;
		}
	}
	
	bool ProgressBar::IsExpandY () const
	{
		if(orientation_ == Vertical) {
			return true;
		} else {
			return false;
		}
	}
	
	Size ProgressBar::GetPreferredSize () const
	{
		Size prefer;

		return prefer;
	}

	void ProgressBar::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

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

	void ProgressBar::PerformRoundTypeUpdate (int round_type)
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

	void ProgressBar::PerformRoundRadiusUpdate (float radius)
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

	ResponseType ProgressBar::Draw(const AbstractWindow* context)
	{
		float x = context->active_frame()->GetRelativePosition(this).x()
				- context->viewport_origin().x();

		int outline_vertices = GetOutlineVertices(round_type());
		float len = 20.f;

		AbstractWindow::shaders->widget_split_inner_program()->use();

		if(hover()) {
			glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_SPLIT_INNER_GAMMA), 15);
		} else {
			glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_SPLIT_INNER_GAMMA), 0);
		}

		glUniform1f(AbstractWindow::shaders->location(Shaders::WIDGET_SPLIT_INNER_PARTING), x + len);
		glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_SPLIT_INNER_COLOR0), 1, AbstractWindow::theme->number_slider().inner_sel.data());
		glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_SPLIT_INNER_COLOR1), 1, AbstractWindow::theme->number_slider().inner.data());

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertices + 2);

		AbstractWindow::shaders->widget_outer_program()->use();

		glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_POSITION),
				0.f, 0.f);
		glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_COLOR), 1, AbstractWindow::theme->number_slider().outline.data());

		glBindVertexArray(vao_[1]);
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

		return Finish;
	}

	void ProgressBar::InitializeProgressBar ()
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

		vbo_.generate();

		glGenVertexArrays(2, vao_);

		glBindVertexArray(vao_[0]);
		vbo_.bind(0);
		vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		glEnableVertexAttribArray(AbstractWindow::shaders->location(Shaders::WIDGET_SPLIT_INNER_COORD));
		glVertexAttribPointer(AbstractWindow::shaders->location(Shaders::WIDGET_SPLIT_INNER_COORD), 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		// generate buffer for outer
		glBindVertexArray(vao_[1]);
		vbo_.bind(1);
		vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		glEnableVertexAttribArray(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_COORD));
		glVertexAttribPointer(AbstractWindow::shaders->location(Shaders::WIDGET_OUTER_COORD), 2,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		vbo_.reset();
	}

}
