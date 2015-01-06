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
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/ProgressBar.hpp>

namespace BlendInt {

	using Stock::Shaders;

	ProgressBar::ProgressBar(Orientation orientation)
	: Widget(),
	  m_orientation(orientation)
	{
		if(orientation == Horizontal) {
			set_size(200, 20);
		} else {
			set_size(20, 200);
		}

		glGenVertexArrays(2, m_vao);

		InitializeProgressBar();
	}

	ProgressBar::~ProgressBar ()
	{
		glDeleteVertexArrays(2, m_vao);
	}
	
	bool ProgressBar::IsExpandX () const
	{
		if(m_orientation == Horizontal) {
			return true;
		} else {
			return false;
		}
	}
	
	bool ProgressBar::IsExpandY () const
	{
		if(m_orientation == Vertical) {
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
			VertexTool tool;
			tool.GenerateVertices(*request.size(), default_border_width(),
			        round_type(), round_radius());
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());

			set_size(*request.size());
			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void ProgressBar::PerformRoundTypeUpdate (int round_type)
	{
		VertexTool tool;
		tool.GenerateVertices(size(), default_border_width(), round_type,
				round_radius());
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());

		set_round_type(round_type);
		RequestRedraw();
	}

	void ProgressBar::PerformRoundRadiusUpdate (float radius)
	{
		VertexTool tool;
		tool.GenerateVertices(size(), default_border_width(),
				round_type(), radius);
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());

		set_round_radius(radius);
		RequestRedraw();
	}

	ResponseType ProgressBar::Draw(const Context* context)
	{
		using Stock::Shaders;

		RefPtr<GLSLProgram> program =
						Shaders::instance->widget_triangle_program();
		program->use();

		glUniform2f(Shaders::instance->location(Stock::WIDGET_TRIANGLE_POSITION), 0.f, 0.f);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_ANTI_ALIAS), 0);

		glBindVertexArray(m_vao[0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

		glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_ANTI_ALIAS), 1);
		glVertexAttrib4fv(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COLOR), Context::theme->regular().outline.data());

		glBindVertexArray(m_vao[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_type()) * 2 + 2);

		glBindVertexArray(0);
		program->reset();

		return Finish;
	}

	void ProgressBar::InitializeProgressBar ()
	{
		float minxi = default_border_width();
		float maxxi = size().width() - default_border_width();
		float minyi = default_border_width();
		float maxyi = size().height() - default_border_width();

		float midx = (minxi + maxxi) / 2.f;

		GLfloat verts[] = {

				minxi, minyi,	0.25f, 0.25f, 0.25f, 0.75f,
				minxi, maxyi,	0.25f, 0.25f, 0.25f, 0.75f,

				midx, minyi,	0.25f, 0.25f, 0.25f, 0.25f,
				midx, maxyi, 	0.25f, 0.25f, 0.25f, 0.25f,

				maxxi, minyi,	0.25f, 0.25f, 0.25f, 0.025f,
				maxxi, maxyi,	0.25f, 0.25f, 0.25f, 0.025f

		};

		VertexTool tool;
		tool.GenerateVertices (size(), default_border_width(), round_type(), round_radius());

		glGenVertexArrays(2, m_vao);
		glBindVertexArray(m_vao[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();

		inner_->set_data(36 * sizeof(GLfloat), verts);
		//tool.SetInnerBufferData(m_inner.get());

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COORD));
		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COLOR));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COORD), 2,	GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COLOR), 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, BUFFER_OFFSET(2 * sizeof(GLfloat)));

		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(m_vao[1]);
		outer_.reset(new GLArrayBuffer);
		outer_->generate();
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());
		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COORD), 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();

	}

}
