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
		"	vec4 color = vec4(0.9f, 0.9f, 0.9f, 1.f);"
		"	FragmentColor = vec4(color.rgb, texture(TexID, f_texcoord).r);"
		"}";

TexAtlasViewer::TexAtlasViewer()
	: BI::Widget()//, m_vao(0)
{
	set_size(384, 384);
	
	glGenVertexArrays(1, &m_vao);
	
	glBindVertexArray(m_vao);

	m_program.reset(new BI::GLSLProgram);
	m_program->Create();
	m_program->AttachShaderPair(vertex_shader, fragment_shader);
	m_program->Link();

	BI::FTLibrary ft_lib;
	BI::FTFace ft_face;

	ft_lib.Initialize();
#ifdef __APPLE__
	ft_face.New(ft_lib, "/System/Library/Fonts/LucidaGrande.ttc");
#else
	ft_face.New(ft_lib, "/usr/share/fonts/truetype/droid/DroidSansFallbackFull.ttf");
#endif
	ft_lib.SetLcdFilter(FT_LCD_FILTER_DEFAULT);
	ft_face.SetCharSize(12 << 6, 0, 96, 0);

	int cell_x = ft_face.face()->size->metrics.max_advance >> 6;
	int cell_y = (ft_face.face()->size->metrics.ascender - ft_face.face()->size->metrics.descender) >> 6;

	long l = ft_face.face()->bbox.xMax - ft_face.face()->bbox.xMin;

	DBG_PRINT_MSG("width: %ld, /64: %ld", l, l >> 6);

	m_atlas.Generate(384, 384, cell_x, cell_y, 1);
	FT_GlyphSlot slot = ft_face.face()->glyph;
	m_atlas.bind();

	FT_Int32 flags = 0;
	flags |= FT_LOAD_NO_BITMAP;
	flags |= FT_LOAD_FORCE_AUTOHINT;

	int max = m_atlas.GetMaxNumber();
	int j = 0;
	for(char i = 33; i < 127; i++) {
		if(ft_face.LoadChar(i, FT_LOAD_RENDER)) {

			if(j >= (max - 1)) {
				DBG_PRINT_MSG("%s", "atlas full");
				break;
			}

			m_atlas.SetSubImage(j, slot->bitmap.width, slot->bitmap.rows, slot->bitmap.buffer, 0, 0);
			j++;
		}
	}

	ft_face.LoadChar(L'仁', FT_LOAD_RENDER);
	m_atlas.SetSubImage(j, slot->bitmap.width, slot->bitmap.rows, slot->bitmap.buffer, 0, 0);
	j++;

	ft_face.LoadChar(L'义', FT_LOAD_RENDER);
	m_atlas.SetSubImage(j, slot->bitmap.width, slot->bitmap.rows, slot->bitmap.buffer, 0, 0);
	j++;

	ft_face.LoadChar(L'礼', FT_LOAD_RENDER);
	m_atlas.SetSubImage(j, slot->bitmap.width, slot->bitmap.rows, slot->bitmap.buffer, 0, 0);
	j++;

	ft_face.LoadChar(L'智', FT_LOAD_RENDER);
	m_atlas.SetSubImage(j, slot->bitmap.width, slot->bitmap.rows, slot->bitmap.buffer, 0, 0);
	j++;

	ft_face.LoadChar(L'信', FT_LOAD_RENDER);
	m_atlas.SetSubImage(j, slot->bitmap.width, slot->bitmap.rows, slot->bitmap.buffer, 0, 0);
	j++;

	ft_face.LoadChar(L'A', FT_LOAD_RENDER);
	m_atlas.SetSubImage(0, slot->bitmap.width, slot->bitmap.rows, slot->bitmap.buffer, 0, 0, true);

	ft_face.LoadChar(L'B', FT_LOAD_RENDER);
	m_atlas.SetSubImage(13, 0, slot->bitmap.width, slot->bitmap.rows, slot->bitmap.buffer, 0, 0, true);

	ft_face.LoadChar(L'C', FT_LOAD_RENDER);
	m_atlas.SetSubImage(1, 19, slot->bitmap.width, slot->bitmap.rows, slot->bitmap.buffer, 0, 0, true);

	/*
	if(ft_face.LoadChar('a', FT_LOAD_RENDER)) {
		m_atlas.Update(0, g->bitmap.width, g->bitmap.rows, g->bitmap.buffer, true);
		m_atlas.Update(1, 0, g->bitmap.width, g->bitmap.rows, g->bitmap.buffer, true);
	}
	*/

	/*
	FT_UInt glyph_index = ft_face.GetCharIndex(31);
	FT_Glyph ft_glyph;
    FT_BitmapGlyph ft_bitmap_glyph;
    FT_Matrix matrix = {
        (int)((1.0)		 * 0x10000L),
        (int)((0.12)      * 0x10000L),
        (int)((0.0)      * 0x10000L),
        (int)((1.0)      * 0x10000L)};

	if(ft_face.LoadGlyph(glyph_index, flags)) {

		// DO sth for outline

		BI::FTStroker ft_stroker;
		if(ft_stroker.New(ft_lib)) {
			ft_stroker.Set((int)(0.5 * 64), FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);

			FT_Error err = FT_Get_Glyph (ft_face.face()->glyph, &ft_glyph);
			if(err) {
				DBG_PRINT_MSG("%s", "Fail to get glyph");
			} else {
				//err = FT_Glyph_StrokeBorder(&ft_glyph, ft_stroker.stroker(), 0, 1);
				err = FT_Glyph_Stroke( &ft_glyph, ft_stroker.stroker(), 1 );
				//err = FT_Glyph_StrokeBorder(&ft_glyph, ft_stroker.stroker(), 1, 1);
				if(err) {
					DBG_PRINT_MSG("%s", "Fail to strokeborder");
				} else {

					err = FT_Glyph_Transform(ft_glyph, &matrix, 0);
					if(err) {
						DBG_PRINT_MSG("%s", "Fail to transform glyph");
 					} else {
 						err = FT_Glyph_To_Bitmap(&ft_glyph, FT_RENDER_MODE_NORMAL, 0, 1);
 						if(err) {
 							DBG_PRINT_MSG("%s", "Fail to glyph to bitmap");
 						} else {
 				            ft_bitmap_glyph = (FT_BitmapGlyph) ft_glyph;
 						}
 					}

				}
			}
		}

		ft_stroker.Done();
		// end of outline

		if(ft_face.LoadGlyph(glyph_index, FT_LOAD_RENDER | FT_LOAD_NO_HINTING)) {
			DBG_PRINT_MSG("top: %d, left: %d, width: %d, height: %d",
							slot->bitmap_top, slot->bitmap_left,
							slot->bitmap.width, slot->bitmap.rows);
			m_atlas.Push(slot->bitmap.width, slot->bitmap.rows, slot->bitmap.buffer);
			m_atlas.MoveNext();
			DBG_PRINT_MSG("top: %d, left: %d, width: %d, height: %d",
							ft_bitmap_glyph->top, ft_bitmap_glyph->left,
							ft_bitmap_glyph->bitmap.width, ft_bitmap_glyph->bitmap.rows);
			m_atlas.Push(ft_bitmap_glyph->bitmap.width, ft_bitmap_glyph->bitmap.rows, ft_bitmap_glyph->bitmap.buffer);
		}

		FT_Done_Glyph(ft_glyph);
	}
	 */

	DBG_PRINT_MSG("columns: %d, rows: %d", m_atlas.GetColumns(), m_atlas.GetRows());
	DBG_PRINT_MSG("max count: %d", m_atlas.GetMaxNumber());

	m_atlas.reset();
	ft_face.Done();
	ft_lib.Done();

	GLfloat vertices[] = {
		0.0, 0.0,
		384.f, 0.0,
		0.0, 384.f,
		384.f, 384.f,
	};

	m_vbo.reset(new BI::GLArrayBuffer);
	m_vbo->generate();
	m_vbo->bind();
	m_vbo->set_data(sizeof(vertices), vertices);
	m_vbo->reset();

	GLfloat uv[] = {
			0.0, 1.0,
			1.0, 1.0,
			0.0, 0.0,
			1.0, 0.0
	};

	m_tbo.reset(new BI::GLArrayBuffer);

	m_tbo->generate();
	m_tbo->bind();
	m_tbo->set_data(sizeof(uv), uv);
	m_tbo->reset();

	glBindVertexArray(0);
}

TexAtlasViewer::~TexAtlasViewer()
{
	glDeleteVertexArrays(1, &m_vao);
}

BI::ResponseType TexAtlasViewer::Draw (const BI::Profile& profile)
{
	//glm::vec3 pos((float)position().x(), (float)position().y(), 0.f);
	//glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

	glm::mat4 mvp;

	glBindVertexArray(m_vao);

	m_program->use();

	glActiveTexture(GL_TEXTURE0);
	m_atlas.bind();

	if (m_atlas.GetWidth() > 0) {
		m_program->SetUniform1i("TexID", 0);
		m_program->SetUniformMatrix4fv("MVP", 1, GL_FALSE,
		        glm::value_ptr(mvp));

		glEnableVertexAttribArray(0);
		m_vbo->bind();
		glVertexAttribPointer(0,
						2,
						GL_FLOAT,
						GL_FALSE,
						0,
						BUFFER_OFFSET(0));

		glEnableVertexAttribArray(1);
		m_tbo->bind();
		glVertexAttribPointer(1,
						2,
						GL_FLOAT,
						GL_FALSE,
						0,
						BUFFER_OFFSET(0));

		m_vbo->bind();

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		m_vbo->reset();
		m_atlas.reset();
	}
	m_program->reset();

	glBindVertexArray(0);
	
	return BI::Finish;
}
