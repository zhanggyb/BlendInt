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
#include <glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <assert.h>
#include <algorithm>

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/Panel.hpp>

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Panel::Panel ()
	: BinLayout(),
	  vao_(0)
	{
		set_size(400, 300);
		InitializeFrame();
	}

	Panel::~Panel ()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void Panel::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			VertexTool tool;
			tool.GenerateVertices(*request.size(), 0, RoundNone, 0);
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			inner_->reset();

			set_size(*request.size());

			if (subs_count()) {
				assert(subs_count() == 1);
				FillSingleWidget(0, *request.size(), margin());
			}
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	ResponseType Panel::Draw (Profile& profile)
	{
		RefPtr<GLSLProgram> program = Shaders::instance->widget_triangle_program();
		program->use();

		glUniform2f(Shaders::instance->location(Stock::WIDGET_TRIANGLE_POSITION), 0.f, 0.f);
		//glUniform3f(Shaders::instance->location(Stock::TRIANGLE_POSITION), (float)position().x(), (float)position().y(), 0.f);
		glVertexAttrib4f(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COLOR), 0.447f, 0.447f, 0.447f, 1.0f);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_ANTI_ALIAS), 0);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);

		program->reset();

		return Ignore;
	}

	void Panel::InitializeFrame ()
	{
		glGenVertexArrays(1, &vao_);

		glBindVertexArray(vao_);
		VertexTool tool;
		tool.GenerateVertices(size(), 0, RoundNone, 0);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COORD), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		inner_->reset();
	}

} /* namespace BlendInt */
