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

#include <BlendInt/Gui/AbstractForm.hpp>

#include <BlendInt/Core/String.hpp>
#include <BlendInt/Core/Color.hpp>
#include <BlendInt/OpenGL/GLBuffer.hpp>
#include <BlendInt/Gui/Font.hpp>

namespace BlendInt {

	class Text: public AbstractForm
	{
	public:

		Text (const String& text);

		Text (const Text& orig);

		virtual ~Text ();

		void SetText (const String& text);

		void SetFont (const Font& font);

		Text& operator = (const Text& orig);

		Text& operator = (const String& text);

		virtual void Draw (float x, float y, short gamma = 0) const;

		void Draw (float x, float y, const Color& color, short gamma = 0) const;

		void Draw (float x, float y, size_t length, size_t start = 0, short gamma = 0) const;

		void Draw (float x, float y, size_t length, size_t start, const Color& color, short gamma = 0) const;

		void Draw (float x, float y, float width, short gamma = 0) const;

		void Draw (float x, float y, float width, const Color& color, short gamma = 0) const;

		/**
		 * @brief Ascender in this text
		 *
		 * @note This is not the same as Font::ascender ()
		 */
		int ascender () const
		{
			return ascender_;
		}

		/**
		 * @brief Descender in this text
		 *
		 * @note This is not the same as Font::descender ()
		 */
		int descender () const
		{
			return descender_;
		}

		const String& text () const
		{
			return text_;
		}

		const Font& font () const
		{
			return font_;
		}

	protected:

		virtual void PerformSizeUpdate (const Size& size);

	private:

        void GenerateTextVertices (std::vector<GLfloat>& verts, int* ptr_width, int* ptr_ascender, int* ptr_descender);
        
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
