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

#include <BIL/Widget.h>
#include <BIL/TextBuffer.h>
#include <BIL/Font.h>

using namespace std;

namespace BIL {

	class Label: public BIL::Widget
	{
	public:

		Label (const wstring& label, Drawable * parent = NULL);

		virtual ~Label ();

		void setText (const wstring& label);

		void setFont (const Font& font)
		{
			_text.setFont(font);
			calculateBox();
		}

		void setTextColor (const RGBAf& fg,
				const RGBAf& bg = RGBAf(0.0, 0.0, 0.0, 0.0))
		{
			_text.setForeground(fg);
			_text.setBackground(bg);
		}

		void setBackground (const RGBAf& color)
		{
			_bg = color;
		}

		virtual void render (void);

	private:	// member functions

		virtual void cursorPosEvent (double xpos, double ypos);

		void calculateBox (void);

	private:

		TextBuffer _text;

		/** Background color, default: transparent */
		RGBAf _bg;

	};

} /* namespace BIL */
#endif /* LABEL_H_ */
