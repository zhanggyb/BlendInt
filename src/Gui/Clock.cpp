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
#include <algorithm>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/Clock.hpp>

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	Clock::Clock()
	: AbstractWidget(), angle_(0), timer_(0)
	{
		InitializeClock();
	}

	Clock::~Clock ()
	{
		glDeleteVertexArrays(2, vao_);
	}

	ResponseType Clock::Draw(const RedrawEvent& event)
	{
		using Stock::Shaders;

		RefPtr<GLSLProgram> program =
						Shaders::instance->triangle_program();
		program->Use();

		program->SetUniform3f("u_position",
		        (float) (position().x() + size().width() / 2.f),
		        (float) (position().y() + size().height() / 2.f), 0.f);
		program->SetUniform1i("u_gamma", 0);
		program->SetUniform1i("u_AA", 0);

		program->SetVertexAttrib4f("a_color", 0.75f, 0.95f, 0.75f, 1.f);

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 72 + 2);

		program->SetVertexAttrib4fv("a_color", Theme::instance->regular().outline.data());
		program->SetUniform1i("u_AA", 1);

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 72 * 2 + 2);

		glBindVertexArray(0);

		program->Reset();

		return Accept;
	}

	void Clock::UpdateClockHands()
	{
		angle_ = angle_ + 6;
		if(angle_ > 360) {
			angle_ = angle_ % 360;
		}

		Refresh();
	}

	void Clock::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			int radius = std::min(request.size()->width(), request.size()->height());
			radius = radius / 2;

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> outer_verts;
			GenerateClockVertices(160, 1.f, inner_verts, outer_verts);

			inner_->Bind();
			inner_->SetData(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			outer_->Bind();
			outer_->SetData(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
			GLArrayBuffer::Reset();

			set_size(*request.size());
			Refresh();
		}

		ReportSizeUpdate(request);
	}

	ResponseType Clock::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType Clock::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Clock::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Clock::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Clock::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Clock::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Clock::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Clock::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	void Clock::GenerateClockVertices (int radius, float border,
	        std::vector<GLfloat>& inner_vertices,
	        std::vector<GLfloat>& outer_vertices)
	{
		if(inner_vertices.size() != (72 * 2 + 2 + 2))
			inner_vertices.resize(72 * 2 + 2 + 2);

		if(outer_vertices.size() != (72 * 4 + 4))
			outer_vertices.resize(72 * 4 + 4);

		double rad = 0.0;
		float x1 = 0.f;
		float y1 = 0.f;
		float x2 = 0.f;
		float y2 = 0.f;

		// 0 1 2 3 4 5
		// x y r g b a

		// the center point
		inner_vertices[0] = 0.f;
		inner_vertices[1] = 0.f;

		int i = 1;
		int j = 0;
		for(int angle = 0; angle < 360; angle = angle + 5) {
			rad = angle * M_PI / 180.0;

			x1 = (radius - border) * cos(rad);
			y1 = (radius - border) * sin(rad);
			x2 = radius * cos(rad);
			y2 = radius * sin(rad);

			inner_vertices[i * 2 + 0] = x1;
			inner_vertices[i * 2 + 1] = y1;

			outer_vertices[j * 4 + 0] = x1;
			outer_vertices[j * 4 + 1] = y1;
			outer_vertices[j * 4 + 2] = x2;
			outer_vertices[j * 4 + 3] = y2;

			i++; j++;
		}

		rad = 360 * M_PI / 180.0;
		x1 = (radius - border) * cos(rad);
		y1 = (radius - border) * sin(rad);
		x2 = radius * cos(rad);
		y2 = radius * sin(rad);

		inner_vertices[i * 2 + 0] = x1;
		inner_vertices[i * 2 + 1] = y1;

		outer_vertices[j * 4 + 0] = x1;
		outer_vertices[j * 4 + 1] = y1;
		outer_vertices[j * 4 + 2] = x2;
		outer_vertices[j * 4 + 3] = y2;
	}

	void Clock::InitializeClock ()
	{
		set_size(160, 160);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		GenerateClockVertices(80, 1.f, inner_verts, outer_verts);

		glGenVertexArrays(2, vao_);

		glBindVertexArray(vao_[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->Generate();
		inner_->Bind();
		inner_->SetData(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glBindVertexArray(vao_[1]);

		outer_.reset(new GLArrayBuffer);
		outer_->Generate();
		outer_->Bind();
		outer_->SetData(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glBindVertexArray(0);
		GLArrayBuffer::Reset();

		timer_.reset(new Timer);
		timer_->SetInterval(1000);

		events()->connect(timer_->timeout(), this, &Clock::UpdateClockHands);
		timer_->Start();
	}

}
