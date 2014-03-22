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

#include <string.h>
#include <cwchar>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/Font.hpp>
#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

	Font::Font (const std::string& name, unsigned int size, bool bold, bool italic)
			: m_name(name), m_size(size), m_bold(bold), m_italic(italic)
	{
		m_cache = FontCache::Create(name, size, themes()->dpi, bold, italic);
	}

	Font::Font (const char* name, unsigned int size, bool bold, bool italic)
		: m_name(name), m_size(size), m_bold(bold), m_italic(italic)
	{
		m_cache = FontCache::Create(name, size, themes()->dpi, bold, italic);
	}

	Font::Font (const Font& orig)
	{
		m_name = orig.m_name;
		m_size = orig.m_size;
		m_bold = orig.m_bold;
		m_italic = orig.m_italic;

		m_cache = orig.m_cache;
	}

	Font& Font::operator = (const Font& orig)
	{
		m_name = orig.m_name;
		m_size = orig.m_size;
		m_bold = orig.m_bold;
		m_italic = orig.m_italic;

		m_cache = orig.m_cache;

		return *this;
	}

	void Font::SetName (const std::string& name)
	{
		m_name = name;
		m_cache = FontCache::Create(m_name, m_size, themes()->dpi, m_bold, m_italic);
	}

	void Font::SetSize (unsigned int size)
	{
		m_size = size;
		m_cache = FontCache::Create(m_name, m_size, themes()->dpi, m_bold, m_italic);
	}

	void Font::SetBold (bool bold)
	{
		m_bold = bold;
		m_cache = FontCache::Create(m_name, m_size, themes()->dpi, m_bold, m_italic);
	}

	void Font::SetItalic (bool italic)
	{
		m_italic = italic;
		m_cache = FontCache::Create(m_name, m_size, themes()->dpi, m_bold, m_italic);
	}


	void Font::Print (const glm::mat4& mvp, const String& string, size_t start)
	{
		Print (mvp, string, string.length(), start);
	}

	void Font::Print (const glm::mat4& mvp, const String& string,
	        size_t length, size_t start)
	{
		glBindVertexArray(m_cache->m_vao);
		glm::mat4 m = mvp;
		RefPtr<GLSLProgram> program = ShaderManager::instance->text_program();

		program->Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_cache->m_atlas.texture());

		program->SetUniform1i("tex", 0);
		program->SetUniform4f("color", 0.f, 0.f, 0.f, 1.f);

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(m));

		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, m_cache->m_vbo);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

		//Vertex2D vertex[6];

		// TODO: read text in TextureFont map
		size_t str_length = std::min(string.length(), length);

		// TODO: support left->right, and right->left text
		String::const_iterator it = string.begin();
		std::advance(it, start);

		for (size_t i = 0; i < str_length; it++, i++)
		{
			//memncpy (&vertex[0], &(atlas_.glyph(*it).vertexes[0]), sizeof(Vertex2D)*6);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * 6, &(m_cache->m_atlas.glyph(*it).vertexes[0]), GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			m = glm::translate(m, glm::vec3(m_cache->m_atlas.glyph(*it).advance_x, 0, 0));

			program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(m));
		}

		glDisableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		program->Reset();
		glBindVertexArray(0);
	}

	void Font::Print (const glm::mat4& mvp, float x, float y, const String& string,
	        size_t start)
	{
		glm::mat4 translated_mvp = glm::translate(mvp, glm::vec3(x, y, 0.0));

		Print (translated_mvp, string, string.length(), start);
	}

	void Font::Print (const glm::mat4& mvp, float x, float y, const String& string,
	        size_t length, size_t start)
	{
		glm::mat4 translated_mvp = glm::translate(mvp, glm::vec3(x, y, 0.0));

		Print (translated_mvp, string, length, start);
	}

}

