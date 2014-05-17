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

#ifndef _BLENDINT_GUI_ABSTRACTROUNDBUTTON_HPP_
#define _BLENDINT_GUI_ABSTRACTROUNDBUTTON_HPP_

#include <BlendInt/Gui/AbstractButton.hpp>
#include <BlendInt/Gui/RoundShapeBase.hpp>

namespace BlendInt {

	/**
	 * @brief The most common button class
	 *
	 * @ingroup widgets
	 */
	class AbstractRoundButton: public AbstractButton, public RoundShapeBase
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractRoundButton);

	public:

		AbstractRoundButton ();

		virtual ~AbstractRoundButton ();

		/**
		 * @brief set round type
		 * @param type
		 *
		 * Hide the same function in RoundBoxBase to call Update
		 */
		void SetRoundType (int type);

		/**
		 * @brief set round radius
		 * @param radius
		 * Hide the same function in RoundBoxBase to call Update
		 */
		void SetRadius (float radius);

		virtual Size GetPreferredSize () const;

		void SetText (const String& text);

		void SetFont (const Font& font);

		const String& text () const {return m_text;}

		size_t text_length () const {return m_text_length;}

		const Font& font () const {return m_font;}

	protected:

		void UpdateTextPosition (const Size& size, int round_type, float radius, const String& text, const Font& font);

		inline void set_text (const String& text)
		{
			m_text = text;
		}

		inline void set_font (const Font& font)
		{
			m_font = font;
		}

		void set_text_length (size_t length)
		{
			m_text_length = length;
		}

		void set_pen (int x, int y)
		{
			m_font.set_pen(x, y);
		}

		size_t GetValidTextSize ();

	private:

		size_t m_text_length;	// How many text to be printed, depends on the button size

		// TextBuffer text_;
		String m_text;

		Font m_font;

	};

}



#endif /* _BLENDINT_GUI_ABSTRACTROUNDBUTTON_HPP_ */
