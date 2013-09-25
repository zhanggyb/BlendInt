/*
 * This file is part of BILO (Blender-like Interface Library in
 * OpenGL).
 *
 * BILO (Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>

#include <iostream>
#include <assert.h>

#include <deque>
#include <queue>
#include <utility>
#include <stdexcept>
#include <algorithm>

#include <BILO/FontCache.hpp>
#include <BILO/ShaderManager.hpp>
#include <BILO/Types.hpp>

using namespace std;

namespace BILO {

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

			/*
			map<wchar_t, TextureFont*>::iterator it;
			for (it = texture_fonts_.begin(); it != texture_fonts_.end(); it++) {
				if (it->second) {
					delete it->second;
					it->second = 0;
				}
			}
			texture_fonts_.clear();
			*/

			delete m_freetype;
		}
	}

	bool FontCache::setup (void)
	{
		if (!m_freetype->valid()) {
			return false;
		}

		atlas_.generate(m_freetype, 32, 96);

		return true;
	}

	const Glyph& FontCache::queryGlyph (wchar_t charcode, bool create)
	{
		//if (atlas_.contains(charcode)) {
			return atlas_.glyph(charcode);
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

		return texture_fonts_[charcode]->glyph();
		*/
	}

	const GLuint FontCache::queryTexture (wchar_t charcode, bool create)
	{
		//if (atlas_.contains(charcode)) {
			return atlas_.texture();
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
	}

	unsigned int FontCache::queryWidth (wchar_t charcode, bool create)
	{
		//if (atlas_.contains(charcode)) {
			return atlas_.width();
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
	}

	unsigned int FontCache::queryHeight (wchar_t charcode, bool create)
	{
//		if (atlas_.contains(charcode)) {
			return atlas_.height();
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
	}

	void FontCache::print (const String& string)
	{
		String::const_iterator it;
		ShaderManager* sm = ShaderManager::instance();
		GLfloat black[4] = { 0, 0, 0, 1 };

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(sm->text_program().id());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, atlas_.texture());
		glUniform1i(sm->text_uniform_tex(), 0);

		glUniform4fv(sm->text_uniform_color(), 1, black);

		/* Set up the VBO for our vertex data */
		glEnableVertexAttribArray(sm->text_attribute_coord());
		glBindBuffer(GL_ARRAY_BUFFER, sm->text_vbo());
		glVertexAttribPointer(sm->text_attribute_coord(), 4, GL_FLOAT, GL_FALSE, 0, 0);

		//Vertex2D vertex[6];

		/* Loop through all characters */
		for (it = string.begin(); it != string.end(); it++)
		{
			/* Draw the character on the screen */
			//memncpy (&vertex[0], &(atlas_.glyph(*it).vertexes[0]), sizeof(Vertex2D)*6);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * 6, &(atlas_.glyph(*it).vertexes[0]), GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			glTranslatef(atlas_.glyph(*it).advance_x, 0, 0);
		}

		glDisableVertexAttribArray(sm->text_attribute_coord());

		glUseProgram(0);
		glDisable(GL_BLEND);
	}

	void FontCache::print (const String& string, size_t length)
	{
		size_t str_length = std::min(string.length(), length);
		String::const_iterator it;
		ShaderManager* sm = ShaderManager::instance();
		GLfloat black[4] = { 0, 0, 0, 1 };

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(sm->text_program().id());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, atlas_.texture());
		glUniform1i(sm->text_uniform_tex(), 0);

		glUniform4fv(sm->text_uniform_color(), 1, black);

		/* Set up the VBO for our vertex data */
		glEnableVertexAttribArray(sm->text_attribute_coord());
		glBindBuffer(GL_ARRAY_BUFFER, sm->text_vbo());
		glVertexAttribPointer(sm->text_attribute_coord(), 4, GL_FLOAT, GL_FALSE, 0, 0);

		//Vertex2D vertex[6];

		/* Loop through all characters */
		size_t i = 0;
		for (it = string.begin(); it != string.end(); it++, i++)
		{
			if (i >= str_length) break;

			/* Draw the character on the screen */
			//memncpy (&vertex[0], &(atlas_.glyph(*it).vertexes[0]), sizeof(Vertex2D)*6);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * 6, &(atlas_.glyph(*it).vertexes[0]), GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			glTranslatef(atlas_.glyph(*it).advance_x, 0, 0);
		}

		glDisableVertexAttribArray(sm->text_attribute_coord());

		glUseProgram(0);
		glDisable(GL_BLEND);
	}

	void FontCache::print (float x, float y, const String& string)
	{
		String::const_iterator it;
		ShaderManager* sm = ShaderManager::instance();
		GLfloat black[4] = { 0, 0, 0, 1 };

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(x, y, 0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(sm->text_program().id());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, atlas_.texture());
		glUniform1i(sm->text_uniform_tex(), 0);

		glUniform4fv(sm->text_uniform_color(), 1, black);

		/* Set up the VBO for our vertex data */
		glEnableVertexAttribArray(sm->text_attribute_coord());
		glBindBuffer(GL_ARRAY_BUFFER, sm->text_vbo());
		glVertexAttribPointer(sm->text_attribute_coord(), 4, GL_FLOAT, GL_FALSE, 0, 0);

		//Vertex2D vertex[6];

		/* Loop through all characters */
		for (it = string.begin(); it != string.end(); it++)
		{
			/* Draw the character on the screen */
			//memncpy (&vertex[0], &(atlas_.glyph(*it).vertexes[0]), sizeof(Vertex2D)*6);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * 6, &(atlas_.glyph(*it).vertexes[0]), GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			glTranslatef(atlas_.glyph(*it).advance_x, 0, 0);
		}

		glDisableVertexAttribArray(sm->text_attribute_coord());

		glUseProgram(0);
		glDisable(GL_BLEND);

		glPopMatrix();

	}

	void FontCache::print (float x, float y, const String& string, size_t length)
	{
		size_t str_length = std::min(string.length(), length);
		String::const_iterator it;
		ShaderManager* sm = ShaderManager::instance();
		GLfloat black[4] = { 0, 0, 0, 1 };

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(x, y, 0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(sm->text_program().id());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, atlas_.texture());
		glUniform1i(sm->text_uniform_tex(), 0);

		glUniform4fv(sm->text_uniform_color(), 1, black);

		/* Set up the VBO for our vertex data */
		glEnableVertexAttribArray(sm->text_attribute_coord());
		glBindBuffer(GL_ARRAY_BUFFER, sm->text_vbo());
		glVertexAttribPointer(sm->text_attribute_coord(), 4, GL_FLOAT, GL_FALSE, 0, 0);

		//Vertex2D vertex[6];

		/* Loop through all characters */
		size_t i = 0;
		for (it = string.begin(); it != string.end(); it++, i++)
		{
			if (i >= str_length) break;

			/* Draw the character on the screen */
			//memncpy (&vertex[0], &(atlas_.glyph(*it).vertexes[0]), sizeof(Vertex2D)*6);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * 6, &(atlas_.glyph(*it).vertexes[0]), GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			glTranslatef(atlas_.glyph(*it).advance_x, 0, 0);
		}

		glDisableVertexAttribArray(sm->text_attribute_coord());

		glUseProgram(0);
		glDisable(GL_BLEND);

		glPopMatrix();
	}

	Rect FontCache::calculateOutline (const String& string)
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
			xmax = queryGlyph(*it).advance_x + xmax;
			ymin = std::min(static_cast<int>(queryGlyph(*it).bitmap_top - queryGlyph(*it).bitmap_height), ymin);
			ymax = std::max(static_cast<int>(queryGlyph(*it).bitmap_top), ymax);
		}

		return Rect(Point(xmin, ymin), Point(xmax, ymax));
	}


#ifdef DEBUG
	void FontCache::printcount (void)
	{
		//std::cout << "fonts in texture_fonts_: " << texture_fonts_.size() << std::endl;
	}
#endif

} /* namespace BILO */

