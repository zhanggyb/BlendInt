/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_LABEL_H_
#define _BIL_LABEL_H_

#include <string>

#include <BIL/Widget.hpp>
#include <BIL/TextBuffer.hpp>
#include <BIL/Font.hpp>
#include <BIL/Color.hpp>

using namespace std;

namespace BIL {

	class Label: public BIL::Widget
	{
	public:

		Label (const wstring& label, Drawable * parent = NULL);

		virtual ~Label ();

		void set_text (const wstring& label);

		void setFont (const Font& font)
		{
			text_.set_font(font);
			calculateBox();
		}

		void setTextColor (const Color& fg,
						   const Color& bg = Color(0x00000000))
		{
			text_.set_foreground(fg);
			text_.set_background(bg);
		}

		void set_background (const Color& color)
		{
			background_ = color;
		}

	protected:

		virtual void Render (void);

	private:	// member functions

		virtual void cursorPosEvent (double xpos, double ypos);

		void calculateBox (void);

	private:

		TextBuffer text_;

		/** Background color, default: transparent */
		Color background_;

	};

} /* namespace BIL */
#endif /* LABEL_H_ */
