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

#pragma once

#include <gui/abstract-form.hpp>

#include <core/string.hpp>
#include <core/color.hpp>
#include <opengl/gl-buffer.hpp>
#include <gui/font.hpp>

namespace BlendInt {

	class Text: public AbstractForm
	{
	public:

		Text (const String& text);

		Text (const Text& orig);

		virtual ~Text ();

		void Add (const String& text);

		void Insert (size_t index, const String& text);

		void SetText (const String& text);
        
        /**
         * @brief Remove specified characters from text
         * @param[in] index first character to remove
         * @param[in] count number of characters to remove
         */
        void Erase (size_t index, size_t count = 1);
        
		void SetFont (const Font& font);

		Text& operator = (const Text& orig);

		Text& operator = (const String& text);

		/**
		 * @brief Get text width
		 * @param[in] length the text length
		 * @param[in] start the start character in the text
		 * @param[in] count_kerning if count kerning before start or after (start + length)
		 */
		size_t GetTextWidth (size_t length, size_t start, bool count_kerning) const;

		virtual void Draw (float x, float y) const;

		void Draw (float x, float y, short gamma) const;

		void Draw (float x, float y, const Color& color, short gamma = 0) const;

		void Draw (float x, float y, size_t length, size_t start = 0, short gamma = 0) const;

		void Draw (float x, float y, size_t length, size_t start, const Color& color, short gamma = 0) const;

		void DrawWithin (float x, float y, int width, short gamma = 0) const;

		void DrawWithin (float x, float y, int width, const Color& color, short gamma = 0) const;
        
        int DrawWithCursor (float x, float y, size_t cursor_index, size_t start, int width, const Color& color, short gamma = 0) const;
        
        int DrawWithCursor (float x, float y, size_t cursor_index, size_t start, int width, short gamma = 0) const;

		/**
		 * @brief Ascender in this text
		 *
		 * @note This is not the same as Font::ascender ()
		 */
        inline int ascender () const
		{
			return ascender_;
		}

		/**
		 * @brief Descender in this text
		 *
		 * @note This is not the same as Font::descender ()
		 */
        inline int descender () const
		{
			return descender_;
		}

        inline const String& text () const
		{
			return text_;
		}

        inline const Font& font () const
		{
			return font_;
		}

        inline bool empty () const
        {
        	return text_.empty();
        }

        inline size_t length () const
        {
        	return text_.length();
        }

	protected:

		virtual void PerformSizeUpdate (const Size& size);

	private:

        /**
         * @brief Generate vertices to be used in VBO for this text
         */
        void GenerateTextVertices (std::vector<GLfloat>& verts, int* ptr_width, int* ptr_ascender, int* ptr_descender);
        
        /**
         * @brief Re-calculate vertices and load to VBO
         *
         * Also reset the size of this form.
         */
        void ReloadBuffer ();
        
		// the ascender of this text
		int ascender_;

		// the descender of this text
		int descender_;

		GLuint vao_;
		GLBuffer<> vbo_;

		String text_;
		Font font_;
	};

}
