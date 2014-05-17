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

#define SETBIT(x,y) (x |= y)
#define CLRBIT(x,y) (x &= ~y)

namespace BlendInt {

	Font::Font (const std::string& name, unsigned int size, int flag, int dpi)
	{
		m_data.name = name;
		m_data.size = size;
		m_data.flag = flag;
		m_data.dpi = dpi;

		m_color = 0x000000FF;

		m_cache = FontCacheExt::Create(m_data);
	}

	Font::Font (const Font& orig)
	: m_data(orig.m_data),
	  m_pen(orig.m_pen),
	  m_color(orig.m_color)
	{
		m_cache = orig.m_cache;
	}

	Font& Font::operator = (const Font& orig)
	{
		m_data = orig.m_data;
		m_pen = orig.m_pen;
		m_color = orig.m_color;
		m_cache = orig.m_cache;

		return *this;
	}

	void Font::SetName (const std::string& name)
	{
		m_data.name = name;
		m_cache = FontCacheExt::Create(m_data);
	}

	void Font::SetSize (unsigned int size)
	{
		m_data.size = size;
		m_cache = FontCacheExt::Create(m_data);
	}

	void Font::SetBold (bool bold)
	{
		if(bold) {
			SETBIT(m_data.flag, FontStyleBold);
		} else {
			CLRBIT(m_data.flag, FontStyleBold);
		}

		m_cache = FontCacheExt::Create(m_data);
	}

	void Font::SetItalic (bool italic)
	{
		if(italic) {
			SETBIT(m_data.flag, FontStyleItalic);
		} else {
			CLRBIT(m_data.flag, FontStyleItalic);
		}

		m_cache = FontCacheExt::Create(m_data);
	}

	int Font::Print (const glm::mat4& mvp, const std::string& string,
	        size_t start) const
	{
		return Print(mvp, string, string.length(), start);
	}

	int Font::Print (const glm::mat4& mvp, const std::string& string, size_t length,
	        size_t start) const
	{
		if(length == 0)	return 0;

		int advance = 0;	// the return value

		glBindVertexArray(m_cache->m_vao);
		glm::mat4 glyph_pos = glm::translate(mvp, glm::vec3(m_pen.x(), m_pen.y(), 0.0));
		RefPtr<GLSLProgram> program = ShaderManager::instance->text_program();

		program->Use();

		glActiveTexture(GL_TEXTURE0);

		program->SetUniform1i("tex", 0);

		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, m_cache->m_vbo);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

		size_t str_length = std::min(string.length(), length);

		// TODO: support left->right, and right->left text
		std::string::const_iterator it;

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(glyph_pos));
		program->SetUniform4f("color", m_color.r() / 255.f,
				m_color.g() / 255.f, m_color.b() / 255.f,
				m_color.a() / 255.f);

		it = string.begin();
		std::advance(it, start);

		const GlyphExt* glyph_ref = 0;
		for (size_t i = 0; i < str_length; it++, i++) {

			glyph_ref = m_cache->Query(m_data, *it);
			glBindTexture(GL_TEXTURE_2D, glyph_ref->texture_atlas->texture());

			advance += glyph_ref->advance_x;

			glBufferData(GL_ARRAY_BUFFER, sizeof(GlyphVertex) * 4,
							&(glyph_ref->vertices[0]),
							GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glyph_pos = glm::translate(glyph_pos, glm::vec3(glyph_ref->advance_x, 0, 0));
			program->SetUniformMatrix4fv("MVP", 1, GL_FALSE,
			        glm::value_ptr(glyph_pos));
		}

		glDisableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		program->Reset();
		glBindVertexArray(0);

		return advance;
	}

	int Font::Print (const glm::mat4& mvp, float x, float y,
	        const std::string& string, size_t start) const
	{
		return Print(mvp, x, y, string, string.length(), start);
	}

	int Font::Print (const glm::mat4& mvp, float x, float y,
	        const std::string& string, size_t length, size_t start) const
	{
		glm::mat4 translated_mvp = glm::translate(mvp, glm::vec3(x, y, 0.f));
		return Print(translated_mvp, string, length, start);
	}

	int Font::Print (const glm::mat4& mvp, const String& string,
	        size_t start) const
	{
		return Print(mvp, string, string.length(), start);
	}

	int Font::Print (const glm::mat4& mvp, const String& string,
					size_t length, size_t start) const
	{
		if(length == 0)	return 0;

		int advance = 0;	// the return value

		glBindVertexArray(m_cache->m_vao);
		glm::mat4 glyph_pos = glm::translate(mvp, glm::vec3(m_pen.x(), m_pen.y(), 0.0));
		RefPtr<GLSLProgram> program = ShaderManager::instance->text_program();

		program->Use();

		glActiveTexture(GL_TEXTURE0);

		program->SetUniform1i("tex", 0);

		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, m_cache->m_vbo);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

		size_t str_length = std::min(string.length(), length);

		// TODO: support left->right, and right->left text
		String::const_iterator it;

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(glyph_pos));
		program->SetUniform4f("color", m_color.r() / 255.f,
				m_color.g() / 255.f, m_color.b() / 255.f,
				m_color.a() / 255.f);

		it = string.begin();
		std::advance(it, start);

		const GlyphExt* glyph_ref = 0;
		for (size_t i = 0; i < str_length; it++, i++) {

			glyph_ref = m_cache->Query(m_data, *it);
			glBindTexture(GL_TEXTURE_2D, glyph_ref->texture_atlas->texture());

			advance += glyph_ref->advance_x;

			glBufferData(GL_ARRAY_BUFFER, sizeof(GlyphVertex) * 4,
							&(glyph_ref->vertices[0]),
							GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glyph_pos = glm::translate(glyph_pos, glm::vec3(glyph_ref->advance_x, 0, 0));
			program->SetUniformMatrix4fv("MVP", 1, GL_FALSE,
			        glm::value_ptr(glyph_pos));
		}

		glDisableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		program->Reset();
		glBindVertexArray(0);

		return advance;
	}

	int Font::Print(const glm::mat4& mvp, float x, float y, const String& string, size_t start) const
	{
		return Print(mvp, x, y, string, string.length(), start);
	}

	int Font::Print(const glm::mat4& mvp, float x, float y, const String& string, size_t length, size_t start) const
	{
		glm::mat4 translated_mvp = glm::translate(mvp, glm::vec3(x, y, 0.f));
		return Print(translated_mvp, string, length, start);
	}

	Rect Font::GetTextOutline (const String& string) const
	{
		String::const_iterator it;

		int xmin = 0;
		int ymin = 0;
		int xmax = 0;
		int ymax = 0;

		for (it = string.begin(); it != string.end(); it++)
		{
			xmax = m_cache->Query(m_data, *it)->advance_x + xmax;
			ymin = std::min(static_cast<int>(m_cache->Query(m_data, *it)->bitmap_top - m_cache->Query(m_data, *it)->bitmap_height), ymin);
			ymax = std::max(static_cast<int>(m_cache->Query(m_data, *it)->bitmap_top), ymax);
		}

		return Rect(Point(xmin, ymin), Point(xmax, ymax));
	}

	size_t Font::GetTextWidth (const std::string& string, size_t start) const
	{
		size_t width = 0;

		assert(start < string.length());

		std::string::const_iterator it = string.begin();
		std::advance(it, start);

		while(it != string.end()) {
			width += m_cache->Query(m_data, *it)->advance_x;
			it++;
		}

		return width;
	}

	size_t Font::GetTextWidth (const std::string& string, size_t length,
	        size_t start) const
	{
		size_t width = 0;

		assert(start < string.length() && length <= string.length());

		std::string::const_iterator it = string.begin();
		std::advance(it, start);
		size_t i = 0;

		while(it != string.end() && (i < length)) {
			width += m_cache->Query(m_data, *it)->advance_x;
			it++;
			i++;
		}

		return width;
	}

	size_t Font::GetTextWidth (const String& string,
	        size_t start) const
	{
		size_t width = 0;

		assert(start < string.length());

		String::const_iterator it = string.begin();
		std::advance(it, start);

		while(it != string.end()) {
			width += m_cache->Query(m_data, *it)->advance_x;
			it++;
		}

		return width;
	}

	size_t Font::GetTextWidth (const String& string, size_t length,
	        size_t start) const
	{
		size_t width = 0;

		assert(start < string.length() && length <= string.length());

		String::const_iterator it = string.begin();
		std::advance(it, start);
		size_t i = 0;

		while(it != string.end() && (i < length)) {
			width += m_cache->Query(m_data, *it)->advance_x;
			it++;
			i++;
		}

		return width;
	}

	size_t Font::GetReversedTextWidth (const std::string& string,
	        size_t start) const
	{
		size_t width = 0;

		assert(start < string.length());

		std::string::const_reverse_iterator it = string.rbegin();
		std::advance(it, start);

		while (it != string.rend()) {
			width += m_cache->Query(m_data, *it)->advance_x;
			it++;
		}

		return width;
	}

	size_t Font::GetReversedTextWidth (const std::string& string,
	        size_t length, size_t start) const
	{
		size_t width = 0;

		assert(start < string.length() && length <= string.length());

		std::string::const_reverse_iterator it = string.rbegin();
		std::advance(it, start);
		size_t i = 0;

		while (it != string.rend() && (i < length)) {
			width += m_cache->Query(m_data, *it)->advance_x;
			it++;
			i++;
		}

		return width;
	}

	size_t Font::GetReversedTextWidth (const String& string,
	        size_t start) const
	{
		size_t width = 0;

		assert(start < string.length());

		String::const_reverse_iterator it = string.rbegin();
		std::advance(it, start);

		while (it != string.rend()) {
			width += m_cache->Query(m_data, *it)->advance_x;
			it++;
		}

		return width;
	}

	size_t Font::GetReversedTextWidth (const String& string,
	        size_t length, size_t start) const
	{
		size_t width = 0;

		assert(start < string.length() && length <= string.length());

		String::const_reverse_iterator it = string.rbegin();
		std::advance(it, start);
		size_t i = 0;

		while (it != string.rend() && (i < length)) {
			width += m_cache->Query(m_data, *it)->advance_x;
			it++;
			i++;
		}

		return width;
	}

}
