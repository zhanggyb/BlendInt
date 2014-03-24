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

#ifndef _BLENDINT_LABEL_HPP_
#define _BLENDINT_LABEL_HPP_

#include <string>

#include <BlendInt/Types.hpp>
#include <BlendInt/Core/Color.hpp>
#include <BlendInt/Core/Rect.hpp>
#include <BlendInt/Core/String.hpp>

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>

#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Gui/Font.hpp>

using namespace std;

namespace BlendInt {

	class Rect;

	class Label: public Widget
	{
		DISALLOW_COPY_AND_ASSIGN(Label);

	public:

		Label (const String& label);

		virtual ~Label ();

		/**
		 * @brief set the label text
		 * @param label
		 *
		 * call this function will reset the size and preferred size of this object
		 */
		void SetText (const String& text);

		/**
		 * @brief set the text font
		 * @param font
		 *
		 * call this function will reset the preferred size of this object
		 */
		void SetFont (const Font& font);

		const Font& font () const
		{
			return m_font;
		}

		void set_foreground_color (const Color& fg)
		{
			m_font.set_color(fg);
		}

		void set_background_color (const Color& color)
		{
			m_background_color = color;
		}

	protected:

		virtual bool Update (const UpdateRequest& request);

		virtual void Draw (RedrawEvent* event);

	private:

		void InitOnce ();

		/**
		 * @brief get the valid text size to print
		 * @return how many characters to print
		 */
		size_t GetValidTextSize ();

		/**
		 * @brief get the valid text size to print
		 * @return how many characters to print
		 */
		size_t GetValidTextSize (const Size* size);

		/**
		 * @brief the text of the label
		 *
		 * new line character is not allowed
		 */
		String m_text;

		/**
		 * @brief the position to print text
		 */
		Point m_origin;

		/**
		 * @brief the text string length to be printed
		 */
		size_t m_length;

		int m_alignment;

		Font m_font;

		/** Background color, default: transparent */
		Color m_background_color;

		/**
		 * @brief Box in which hold the text
		 */
		Rect m_text_outline;

		GLuint m_vao;

		RefPtr<GLArrayBuffer> m_rect;
	};

} /* namespace BlendInt */
#endif /* _BlendIntLABEL_HPP_ */
