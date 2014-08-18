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
#endif	// __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <algorithm>

#include <BlendInt/Gui/VertexTool.hpp>

#include <BlendInt/Gui/StackPanel.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	using Stock::Shaders;

	StackPanel::StackPanel()
	: Stack(),
	  m_vao(0)
	{
		set_drop_shadow(true);
		InitializeStackPanel();
	}
	
	StackPanel::~StackPanel ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}
	
	ResponseType StackPanel::Draw (const Profile& profile)
	{
		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->Use();

		glUniform3f(Shaders::instance->triangle_uniform_position(),
		        (float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 0);

		glVertexAttrib4f(Shaders::instance->triangle_attrib_color(), 0.447f,
		        0.447f, 0.447f, 1.0f);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
		        GetOutlineVertices(round_type()) * 2 + 2);
		glBindVertexArray(0);

		program->Reset();
		return Ignore;
	}
	
	void StackPanel::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.Setup(*request.size(), 0, RoundNone, 0);
			m_inner->Bind();
			tool.SetInnerBufferData(m_inner.get());
			m_inner->Reset();
		}

		Stack::PerformSizeUpdate(request);
	}

	void StackPanel::InitializeStackPanel()
	{
		glGenVertexArrays(1, &m_vao);

		VertexTool tool;
		tool.Setup(size(), 0, RoundNone, 0);

		glBindVertexArray(m_vao);

		m_inner.reset(new GLArrayBuffer);

		m_inner->Generate();
		m_inner->Bind();
		tool.SetInnerBufferData(m_inner.get());

		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::Reset();

	}

}
