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

#ifndef _BILO_LABEL_HPP_
#define _BILO_LABEL_HPP_

#include <string>

#include <BILO/Widget.hpp>
#include <BILO/Font.hpp>
#include <BILO/Color.hpp>
#include <BILO/Rect.hpp>
#include <BILO/String.hpp>
#include <BILO/Types.hpp>

using namespace std;

namespace BILO {

	class Rect;

	class Label: public Widget
	{
		DISALLOW_COPY_AND_ASSIGN(Label);

	public:

		Label (const String& label);

		Label (const String& label, Drawable * parent);

		virtual ~Label ();

		void set_text (const String& label);

		void set_font (const Font& font);

		void set_foreground (const Color& fg)
		{
			m_foreground = fg;
		}

		void set_background (const Color& color = Color(0x00000000))
		{
			m_background = color;
		}

	protected:

		virtual void render ();

	private:
		// member functions

		virtual void cursorPosEvent (double xpos, double ypos);

	private:

		/**
		 * @brief the text of the label
		 *
		 * new line character is not allowed
		 */
		String m_text;

		Font m_font;

		Color m_foreground;

		/** Background color, default: transparent */
		Color m_background;

		/**
		 * @brief Box in which hold the text
		 */
		Rect m_text_outline;
	};

} /* namespace BILO */
#endif /* _BILOLABEL_HPP_ */
