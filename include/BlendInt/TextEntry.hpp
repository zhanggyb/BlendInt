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

#ifndef _BLENDINT_TEXTENTRY_HPP_
#define _BLENDINT_TEXTENTRY_HPP_

#include <BlendInt/RoundWidget.hpp>
#include <BlendInt/Font.hpp>
#include <BlendInt/String.hpp>
#include <BlendInt/Point.hpp>
#include <BlendInt/Rect.hpp>

#include <BlendInt/Timer.hpp>

namespace BlendInt {

	class TextEntry: public RoundWidget
	{
		DISALLOW_COPY_AND_ASSIGN(TextEntry);

	public:

		TextEntry();

		TextEntry(AbstractWidget* parent);

		virtual ~TextEntry();

		void SetText (const String& text);

		void SetFont (const Font& font);

		const String& text () const {return m_text;}

	protected:

		virtual void KeyPressEvent (KeyEvent* event);

		virtual void Draw ();

	private:

		void Init ();

		void OnReverseCursor ();

		size_t GetValidTextSize ();

		/**
		 * @brief Get the index and length of the text to show
		 * @param[out] start The index in the text to print
		 * @param[out] length The length of the text to print
		 */
		void GetVisibleTextPlace (size_t* start, size_t* length);

		Font m_font;

		String m_text;

		size_t m_start;	// where start print the text

		size_t m_length;

		size_t m_cursor_position;

		/**
		 * @brief The position to print text
		 */
		Point m_origin;

		/**
		 * @brief Box in which hold the text
		 */
		Rect m_text_outline;

		Timer* m_timer;

		bool m_flicker;
	};

}

#endif /* _BLENDINT_TEXTENTRY_HPP_ */
