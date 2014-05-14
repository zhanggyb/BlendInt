// TexAtlasViewer.cpp

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

#include "TexAtlasViewer.hpp"

#include <BlendInt/Core/Freetype.hpp>

const char* TexAtlasViewer::vertex_shader =
		"#version 330\n"
		"layout(location = 0) in vec2 Coord2D;"
		"layout(location = 1) in vec2 UVCoord;"
		"uniform mat4 MVP;"
		"out vec2 f_texcoord;"
		""
		"void main(void) {"
		"	gl_Position = MVP * vec4(Coord2D, 0.0, 1.0);"
		"	f_texcoord = UVCoord;"
		"}";

const char* TexAtlasViewer::fragment_shader =
		"#version 330\n"
		"in vec2 f_texcoord;"
		"uniform sampler2D TexID;"
		"out vec4 FragmentColor;"
		""
		"void main(void) {"
		"	FragmentColor = texture(TexID, f_texcoord);"
		"}";

TexAtlasViewer::TexAtlasViewer()
	: BI::Widget(), m_vao(0)
{
	set_size(256, 256);
	set_expand_x(true);
	set_expand_y(true);
	
	glGenVertexArrays(1, &m_vao);
	
	glBindVertexArray(m_vao);

	m_program.reset(new BI::GLSLProgram);
	m_program->Create();
	m_program->AttachShaderPair(vertex_shader, fragment_shader);
	m_program->Link();

	BI::FTLibrary ft_lib;
	BI::FTFace ft_face;

	ft_lib.Initialize();
	ft_face.New(ft_lib, "/usr/share/fonts/truetype/droid/DroidSans.ttf");
	ft_face.SetCharSize(12 << 6, 0, 96, 0);

	int cell_x = ft_face.face()->size->metrics.max_advance >> 6;
	int cell_y = ft_face.face()->size->metrics.height >> 6;

	DBG_PRINT_MSG("cell_x: %d, cell_y: %d", cell_x, cell_y);

	m_atlas.Generate(256, 256, cell_x, cell_y, 2);
	FT_GlyphSlot g = ft_face.face()->glyph;
	m_atlas.Bind();

	for(char i = 0; i < 127; i++) {
		if(ft_face.LoadChar(i, FT_LOAD_RENDER)) {
			m_atlas.Push(g->bitmap.width, g->bitmap.rows, g->bitmap.buffer);
		}
	}

	m_atlas.Reset();
	ft_face.Done();
	ft_lib.Done();

	GLfloat vertices[] = {
		0.0, 0.0,
		256.f, 0.0,
		0.0, 256.f,
		256.f, 256.f,
	};

	m_vbo.reset(new BI::GLArrayBuffer);
	m_vbo->Generate();
	m_vbo->Bind();
	m_vbo->SetData(sizeof(vertices), vertices);
	m_vbo->Reset();

	GLfloat uv[] = {
			0.0, 1.0,
			1.0, 1.0,
			0.0, 0.0,
			1.0, 0.0
	};

	m_tbo.reset(new BI::GLArrayBuffer);

	m_tbo->Generate();
	m_tbo->Bind();
	m_tbo->SetData(sizeof(uv), uv);
	m_tbo->Reset();

	glBindVertexArray(0);
}

TexAtlasViewer::~TexAtlasViewer()
{
	glDeleteVertexArrays(1, &m_vao);
}

void TexAtlasViewer::Update(const BI::UpdateRequest& request)
{
	if(request.source() == BI::Predefined) {

		switch (request.type()) {

			case BI::FormSize: {

				//const Size* size_p = static_cast<const Size*>(request.data());

				break;
			}

			default:
				break;
		}

	}
}

BI::ResponseType TexAtlasViewer::Draw (const BI::RedrawEvent& event)
{
	glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
	glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

	glBindVertexArray(m_vao);

	m_program->Use();

	glActiveTexture(GL_TEXTURE0);
	m_atlas.Bind();

	if (m_atlas.GetWidth() > 0) {
		m_program->SetUniform1i("TexID", 0);
		m_program->SetUniformMatrix4fv("MVP", 1, GL_FALSE,
		        glm::value_ptr(mvp));

		glEnableVertexAttribArray(0);
		m_vbo->Bind();
		glVertexAttribPointer(0,
						2,
						GL_FLOAT,
						GL_FALSE,
						0,
						BUFFER_OFFSET(0));

		glEnableVertexAttribArray(1);
		m_tbo->Bind();
		glVertexAttribPointer(1,
						2,
						GL_FLOAT,
						GL_FALSE,
						0,
						BUFFER_OFFSET(0));

		m_vbo->Bind();

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		m_vbo->Reset();
		m_atlas.Reset();
	}
	m_program->Reset();

	glBindVertexArray(0);
	
	return BI::Accept;
}
