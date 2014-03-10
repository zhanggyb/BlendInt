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
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <iostream>
#include <assert.h>

#include <deque>
#include <queue>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <iterator>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/FontCache.hpp>
#include <BlendInt/Service/ShaderManager.hpp>
#include <BlendInt/Types.hpp>

using namespace std;

namespace BlendInt {

	unsigned int FontCache::maxCaches = 32;

	map<Font, FontCache*> FontCache::cacheDB;
	map<Font, unsigned long> FontCache::cacheCountDB;

	FontCache* FontCache::create (const Font& font, unsigned int dpi,
	        bool force)
	{
		// Don't repeatedly create, cause memory leak
		map<Font, FontCache*>::const_iterator it;
		it = cacheDB.find(font);

		if (it != cacheDB.end()) {
			unsigned long count = cacheCountDB[font];
			cacheCountDB[font] = count + 1;

			it->second->set_dpi(dpi);
			return it->second;
		}

		if (cacheDB.size() >= maxCaches) {

			if (!force)
				return NULL;

			// Remove mostly unused cache
			typedef std::pair<Font, unsigned long> data_t;
			typedef std::priority_queue<data_t, std::deque<data_t>,
			        greater_second<data_t> > queue_t;
			queue_t q(cacheCountDB.begin(), cacheCountDB.end());

			Font font_of_cache = q.top().first;
			//wcout << "Remove " << q.top().first.family << " from cache DB."
			//        << std::endl;

			delete cacheDB[font_of_cache];
			cacheDB.erase(font_of_cache);
			cacheCountDB.erase(font_of_cache);
		}

		FontCache * cache = new FontCache(font, dpi);

		cacheDB[font] = cache;
		unsigned long count = cacheCountDB[font];
		cacheCountDB[font] = count + 1;

		return cache;
	}

	FontCache* FontCache::getCache (const Font& font, unsigned int dpi)
	{
		map<Font, unsigned long>::const_iterator it;
		it = cacheCountDB.find(font);

		if (it == cacheCountDB.end()) {
			return NULL;
		} else {
			unsigned long count = cacheCountDB[font];
			cacheCountDB[font] = count + 1;
			cacheDB[font]->set_dpi(dpi);
			return cacheDB[font];
		}
	}

	bool FontCache::release (const Font& font)
	{
		map<Font, FontCache*>::iterator it;
		it = cacheDB.find(font);

		if (it == cacheDB.end())
			return false;

		FontCache* cache = it->second;
		if (cache != NULL) {
			delete cache;
		}

		// now erase the key-value
		cacheDB.erase(it);
		cacheCountDB.erase(font);

		return true;
	}

	void FontCache::releaseAll (void)
	{
		map<Font, FontCache*>::iterator it;

		for (it = cacheDB.begin(); it != cacheDB.end(); it++) {
			delete it->second;
		}
		cacheDB.clear();
		cacheCountDB.clear();
	}

#ifdef DEBUG
	void FontCache::list (void)
	{
		map<Font, unsigned long>::const_iterator it;
		cout << endl;
		for (it = cacheCountDB.begin(); it != cacheCountDB.end(); it++) {
			cout << ConvertFromString(it->first.family) << " of " << it->first.size
			        << " is used: " << it->second << endl;
		}
	}
#endif

	FontCache::FontCache (const Font& font, unsigned int dpi)
			: m_freetype(NULL)
	{
		m_freetype = new Freetype;
		m_freetype->open(font, dpi);

		if(!setup()) {
			throw std::runtime_error("Fail to setup FontCache");
		}
	}

	FontCache::~FontCache ()
	{
		if (m_freetype) {

			map<wchar_t, TextureFont*>::iterator it;
			for (it = m_texture_fonts.begin(); it != m_texture_fonts.end();
			        it++) {
				if (it->second) {
					delete it->second;
					it->second = 0;
				}
			}
			m_texture_fonts.clear();

			delete m_freetype;
		}
	}

	bool FontCache::setup (void)
	{
		if (!m_freetype->valid()) {
			return false;
		}

		m_atlas.generate(m_freetype, 32, 96);

		return true;
	}

	const Glyph& FontCache::query (wchar_t charcode, bool create)
	{
		if (m_atlas.contains(charcode)) {
			return m_atlas.glyph(charcode);
		}

		map<wchar_t, TextureFont*>::iterator it;
		it = m_texture_fonts.find(charcode);

		// if the glyph is not found and need to be created
		if (it == m_texture_fonts.end()) {

			if (create) {

				TextureFont* new_font = new TextureFont;
				new_font->generate(m_freetype, charcode);
				m_texture_fonts[charcode] = new_font;

			} else {
				// TODO: return an default font glyph to show unknown character
			}
		}

		return m_texture_fonts[charcode]->glyph();

	}

//	const GLuint FontCache::queryTexture (wchar_t charcode, bool create)
//	{
		//if (atlas_.contains(charcode)) {
//			return atlas_.texture();
		//}

		/*
		map<wchar_t, TextureFont*>::iterator it;
		it = texture_fonts_.find(charcode);

		// if the glyph is not found and need to be created
		if (it == texture_fonts_.end()) {

			if (create) {

				TextureFont* new_font = new TextureFont;
				new_font->generate(fontengine_, charcode);
				texture_fonts_[charcode] = new_font;

			} else {
				// TODO: return an default font glyph to show unknown character
			}
		}

		return texture_fonts_[charcode]->texture();
		*/
//	}

//	unsigned int FontCache::queryWidth (wchar_t charcode, bool create)
//	{
		//if (atlas_.contains(charcode)) {
//			return atlas_.width();
		//}

		/*
		map<wchar_t, TextureFont*>::iterator it;
		it = texture_fonts_.find(charcode);

		// if the glyph is not found and need to be created
		if (it == texture_fonts_.end()) {

			if (create) {

				TextureFont* new_font = new TextureFont;
				new_font->generate(fontengine_, charcode);
				texture_fonts_[charcode] = new_font;

			} else {
				// TODO: return an default font glyph to show unknown character
			}
		}

		return texture_fonts_[charcode]->width();
		*/
//	}

//	unsigned int FontCache::queryHeight (wchar_t charcode, bool create)
//	{
//		if (atlas_.contains(charcode)) {
//			return atlas_.height();
//		}

		/*
		map<wchar_t, TextureFont*>::iterator it;
		it = texture_fonts_.find(charcode);

		// if the glyph is not found and need to be created
		if (it == texture_fonts_.end()) {

			if (create) {

				TextureFont* new_font = new TextureFont;
				new_font->generate(fontengine_, charcode);
				texture_fonts_[charcode] = new_font;

			} else {
				// TODO: return an default font glyph to show unknown character
			}
		}

		return texture_fonts_[charcode]->height();
		*/
//	}

	void FontCache::Print (const glm::mat4& mvp, const String& string, size_t start)
	{
		Print (mvp, string, string.length(), start);
	}

	void FontCache::Print (const glm::mat4& mvp, const String& string, size_t length, size_t start)
	{
		glm::mat4 m = mvp;
		RefPtr<GLSLProgram> program = ShaderManager::instance->text_program();

		GLfloat black[4] = { 0, 0, 0, 1 };	// font color, customizable later

		program->Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_atlas.texture());

		program->SetUniform1i("tex", 0);
		program->SetUniform4fv("color", 1, black);

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(m));

		GLint text_attrib_coord = program->GetAttributeLocation("coord");

		glEnableVertexAttribArray(text_attrib_coord);

		glBindBuffer(GL_ARRAY_BUFFER, ShaderManager::instance->text_vbo());
		glVertexAttribPointer(text_attrib_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);

		//Vertex2D vertex[6];

		// TODO: read text in TextureFont map
		size_t str_length = std::min(string.length(), length);

		// TODO: support left->right, and right->left text
		String::const_iterator it = string.begin();
		std::advance(it, start);

		for (size_t i = 0; i < str_length; it++, i++)
		{
			//memncpy (&vertex[0], &(atlas_.glyph(*it).vertexes[0]), sizeof(Vertex2D)*6);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * 6, &(m_atlas.glyph(*it).vertexes[0]), GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			m = glm::translate(m, glm::vec3(m_atlas.glyph(*it).advance_x, 0, 0));

			program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(m));
		}

		glDisableVertexAttribArray(text_attrib_coord);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		program->Reset();
	}

	void FontCache::Print (const glm::mat4& mvp, float x, float y, const String& string, size_t start)
	{
		glm::mat4 translated_mvp = glm::translate(mvp, glm::vec3(x, y, 0.0));

		Print (translated_mvp, string, string.length(), start);
	}

	void FontCache::Print (const glm::mat4& mvp, float x, float y, const String& string, size_t length, size_t start)
	{
		glm::mat4 translated_mvp = glm::translate(mvp, glm::vec3(x, y, 0.0));

		Print (translated_mvp, string, length, start);
	}

	void FontCache::print (const String& string, size_t length)
	{
		ShaderManager* sm = ShaderManager::instance;
		GLfloat black[4] = { 0, 0, 0, 1 };

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glUseProgram(sm->text_program()->id());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_atlas.texture());
		glUniform1i(sm->text_uniform_tex(), 0);

		glUniform4fv(sm->text_uniform_color(), 1, black);

		/* Set up the VBO for our vertex data */
		glEnableVertexAttribArray(sm->text_attribute_coord());
		glBindBuffer(GL_ARRAY_BUFFER, sm->text_vbo());
		glVertexAttribPointer(sm->text_attribute_coord(), 4, GL_FLOAT, GL_FALSE, 0, 0);

		//Vertex2D vertex[6];

		/* Loop through all characters */
		// TODO: read text in TextureFont map
		size_t str_length = std::min(string.length(), length);
		size_t i = 0;
		for (String::const_iterator it = string.begin(); i < str_length; it++, i++)
		{
			/* Draw the character on the screen */
			//memncpy (&vertex[0], &(atlas_.glyph(*it).vertexes[0]), sizeof(Vertex2D)*6);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * 6, &(m_atlas.glyph(*it).vertexes[0]), GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			glTranslatef(m_atlas.glyph(*it).advance_x, 0, 0);
		}

		glDisableVertexAttribArray(sm->text_attribute_coord());

		glBindBuffer(GL_ARRAY_BUFFER, 0);	// do not forget unbind buffer
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);

		glPopMatrix();
	}

	void FontCache::print (const String& string)
	{
		print (string, string.length());
	}

	void FontCache::print (float x, float y, const String& string)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(x, y, 0);

		print (string, string.length());

		glPopMatrix();
	}

	void FontCache::print (float x, float y, const String& string, size_t length)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(x, y, 0);

		print (string, length);

		glPopMatrix();
	}

	Rect FontCache::get_text_outline (const String& string)
	{
		if(!m_freetype->valid()) {
			return Rect();
		}
		String::const_iterator it;
		// String::const_iterator next;
		// Tuple2l kerning;
		int xmin = 0;
		int ymin = 0;
		int xmax = 0;
		int ymax = 0;

		for (it = string.begin(); it != string.end(); it++)
		{
			xmax = query(*it).advance_x + xmax;
			ymin = std::min(static_cast<int>(query(*it).bitmap_top - query(*it).bitmap_height), ymin);
			ymax = std::max(static_cast<int>(query(*it).bitmap_top), ymax);
		}

		return Rect(Point(xmin, ymin), Point(xmax, ymax));
	}

	unsigned int FontCache::GetTextWidth (const String& string, size_t length, size_t start)
	{
		unsigned int width = 0;

		if(!m_freetype->valid()) {
			return width;
		}

		String::const_iterator it = string.begin();
		std::advance(it, start);
		size_t i = 0;

		for (; (it != string.end()) && (i < length); it++, i++)
		{
			width = query(*it).advance_x + width;
		}

		return width;
	}


#ifdef DEBUG
	void FontCache::printcount (void)
	{
		//std::cout << "fonts in texture_fonts_: " << texture_fonts_.size() << std::endl;
	}
#endif

} /* namespace BlendInt */

