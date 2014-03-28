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
#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

	const char* ToolBar::vertex_shader =
					"#version 330\n"
					""
					"layout(location=0) in vec2 Coord2D;"
					"layout(location=1) in vec4 Color;"
					"out vec4 VertexColor;"
					""
					"void main(void) {"
					"	gl_Position = vec4(Coord2D, 0.0, 1.0);"
					"	VertexColor = Color;"
					"}";

	const char* ToolBar::geometry_shader =
					"#version 330\n"
					""
					"layout (triangles) in;"
					"layout (triangle_strip, max_vertices = 24) out;"
					"in vec4 VertexColor[];"
					"uniform bool AA = false;"
					"uniform mat4 MVP;"
					"out vec4 PreFragColor;"
					""
					"const vec2 AA_JITTER[8] = vec2[8]("
					"	vec2(0.468813, -0.481430),"
					"	vec2(-0.155755, -0.352820),"
					"	vec2(0.219306, -0.238501),"
					"	vec2(-0.393286,-0.110949),"
					"	vec2(-0.024699, 0.013908),"
					"	vec2(0.343805, 0.147431),"
					"	vec2(-0.272855, 0.269918),"
					"	vec2(0.095909, 0.388710));"
					""
					"void main()"
					"{"
					"	vec4 vertex;"
					""
					"	if(AA) {"
					"		mat4 trans_matrix = mat4(1.0);"
					"		vec4 col_calib;"
					"		for(int jit = 0; jit < 8; jit++) {"
					"			trans_matrix[3] = vec4(AA_JITTER[jit], 0.0, 1.0);"
					"			for(int n = 0; n < gl_in.length(); n++)"
					"			{"
					"				vertex = MVP * trans_matrix * gl_in[n].gl_Position;"
					"				col_calib = VertexColor[n];"
					"				col_calib.a = col_calib.a/8;"
					"				PreFragColor = col_calib;"
					"				gl_Position = vertex;"
					"				EmitVertex();"
					"			}"
					"			EndPrimitive();"
					"		}"
					"		return;"
					"	} else {"
					"		for(int n = 0; n < gl_in.length(); n++) {"
					"			vertex = MVP * gl_in[n].gl_Position;"
					"			PreFragColor = VertexColor[n];"
					"			gl_Position = vertex;"
					"			EmitVertex();"
					"		}"
					"		EndPrimitive();"
					"		return;"
					"	}"
					""
					"}";

	const char* ToolBar::fragment_shader =
					"#version 330\n"
					""
					"in vec4 PreFragColor;"
					"uniform bool AA = false;"
					"uniform int Gamma = 0;"
					"out vec4 FragmentColor;"
					""
					"void main(void) {"
					"	vec4 color_calib = vec4(0.0);"
					"	if(AA) {"
					"		color_calib = vec4(vec3(min(max(-1.0, Gamma/255.0/8.0), 1.0)), 0.0);"
					"	} else {"
					"		color_calib = vec4(vec3(min(max(-1.0, Gamma/255.0), 1.0)), 0.0);"
					"	}"
					"	FragmentColor = PreFragColor + color_calib;"
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
		m_program->SetUniform1i("AA", 0);
		m_program->SetVertexAttrib4f("Color", 0.2f, 0.4f, 0.3f, 1.f);

		m_program->SetUniform1i("Gamma", 20);

		glEnableVertexAttribArray(0);

		m_inner->Bind();

		glVertexAttribPointer(0, // attribute
							  2,			// number of elements per vertex, here (x,y)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  0,				 // no extra data between each position
							  0					 // offset of first element
							  );

		glDrawArrays(GL_TRIANGLE_FAN, 0,
							m_inner->GetBufferSize()
											/ (2 * sizeof(GLfloat)));

		m_inner->Reset();

		m_program->SetUniform1i("AA", 1);
		m_program->SetUniform1i("Gamma", 0);
		m_program->SetVertexAttrib4f("Color", 0.f, 0.f, 0.f, 1.f);

		m_outer->Bind();

		glVertexAttribPointer(0, // attribute
							  2,			// number of elements per vertex, here (x,y)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  0,				 // no extra data between each position
							  0					 // offset of first element
							  );

		/*
		glDrawArrays(GL_TRIANGLES, 0,
							m_buffer->GetBufferSize()
											/ (2 * sizeof(GLfloat)));
		*/
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
							m_outer->GetBufferSize()
											/ (2 * sizeof(GLfloat)));
		m_outer->Reset();

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
		set_preferred_size(200, 200);
		set_size(200, 200);
		m_inner.reset(new GLArrayBuffer);
		m_outer.reset(new GLArrayBuffer);

		m_program.reset(new GLSLProgram);
		m_program->Create();
		m_program->Use();
		m_program->AttachShader(vertex_shader, GL_VERTEX_SHADER);
		m_program->AttachShader(geometry_shader, GL_GEOMETRY_SHADER);
		m_program->AttachShader(fragment_shader, GL_FRAGMENT_SHADER);
		m_program->Link();
		m_program->Reset();

		//m_program = ShaderManager::instance->default_widget_program();

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		m_inner->Generate();
		m_outer->Generate();

		//GenerateSingleTriangle(m_buffer.get());
		GenerateFormBuffer(size(), RoundAll, 5.0, m_inner.get(), m_outer.get(), 0);

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
		vert[5] = 200.f;

		buffer->Bind();
		buffer->SetData(sizeof(GLfloat) * vert.size(), &vert[0]);
		buffer->Reset();
	}

}
