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

#include <BlendInt/Gui/ToolBar.hpp>

namespace BlendInt {

	const char* ToolBar::vertex_shader =
			"#version 330\n"
			""
			"layout(location=0) in vec2 xy;"
			"layout(location=1) in vec4 color;"
			"in float z;"
			"uniform mat4 MVP;"
			"out vec4 g_color;"
			""
			"void main(void) {"
			"	gl_Position = MVP * vec4(xy, z, 1.0);"
			"	g_color = color;"
			"}";

	const char* ToolBar::geometry_shader =
			"#version 330\n"
			""
			"layout (triangles) in;"
			"layout (triangle_strip, max_vertices = 3) out;"
			"in vec4 g_color[];"
			"out vec4 f_color;"
			""
			"void main()"
			"{"
			"	int n;"
			"	for(n = 0; n < gl_in.length(); n++)"
			"	{"
			"		gl_Position = gl_in[0].gl_Position;"
			"		EmitVertex();"
			"		f_color = g_color[n];"
			"	}"
			"	EndPrimitive();"
			"}";

	const char* ToolBar::fragment_shader =
			"#version 330\n"
			""
			"in vec4 f_color;"
			"uniform int gamma = 0;"
			"out vec4 FragmentColor;"
			""
			"void main(void) {"
			"	vec4 color_calib = vec4(vec3(min(max(-1.0, gamma/255.0), 1.0)), 0.0);"
			"	FragmentColor = f_color + color_calib;"
			"}";

	ToolBar::ToolBar ()
	{
		InitOnce();
	}

	ToolBar::~ToolBar ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	bool ToolBar::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch (request.type()) {

				default:
					return true;
			}

		} else {
			return false;
		}
	}

	ResponseType ToolBar::Draw (const RedrawEvent& event)
	{
		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		glBindVertexArray(m_vao);

		m_program->Use();

		m_program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		m_program->SetVertexAttrib1f("z", (float) z());

		glm::vec4 color(0.5, 0.2, 0.2, 0.6);
		m_program->SetVertexAttrib4fv("color", glm::value_ptr(color));

		glEnableVertexAttribArray(0);

		m_buffer->Bind();

		glVertexAttribPointer(0, // attribute
							  2,			// number of elements per vertex, here (x,y)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  0,				 // no extra data between each position
							  0					 // offset of first element
							  );

		glDrawArrays(GL_TRIANGLES, 0,
							m_buffer->GetBufferSize()
											/ (2 * sizeof(GLfloat)));
		m_buffer->Reset();

		glDisableVertexAttribArray(0);

		m_program->Reset();

		glBindVertexArray(0);
		return Accept;
	}

	ResponseType ToolBar::CursorEnterEvent (bool entered)
	{
		return Accept;
	}

	ResponseType ToolBar::KeyPressEvent (const KeyEvent& event)
	{
		return Accept;
	}

	ResponseType ToolBar::ContextMenuPressEvent (
					const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType ToolBar::ContextMenuReleaseEvent (
					const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType ToolBar::MousePressEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType ToolBar::MouseReleaseEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType ToolBar::MouseMoveEvent (const MouseEvent& event)
	{
		return Accept;
	}

	void ToolBar::InitOnce ()
	{
		set_preferred_size(400, 400);
		set_size(400, 400);
		m_buffer.reset(new GLArrayBuffer);
		m_program.reset(new GLSLProgram);
		m_program->Create();
		m_program->Use();
		m_program->AttachShaderPair(vertex_shader, fragment_shader);
		m_program->AttachShader(geometry_shader, GL_GEOMETRY_SHADER);
		m_program->Link();
		m_program->Reset();

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		m_buffer->Generate();

		GenerateSingleTriangle(m_buffer.get());
		glBindVertexArray(0);
	}

	void ToolBar::GenerateSingleTriangle(GLArrayBuffer* buffer)
	{
		std::vector<GLfloat> vert;

		vert.resize(2 * 3);

		vert[0] = 50.f;
		vert[1] = 50.f;
		vert[2] = 250.f;
		vert[3] = 50.f;
		vert[4] = 150.f;
		vert[5] = 150.f;

		buffer->Bind();
		buffer->SetData(sizeof(GLfloat) * vert.size(), &vert[0]);
		buffer->Reset();
	}

}
