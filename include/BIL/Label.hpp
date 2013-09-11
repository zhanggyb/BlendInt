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
#include <BIL/Font.hpp>
#include <BIL/Color.hpp>

#include <BIL/String.hpp>

using namespace std;

namespace BIL {

	class Label: public Widget
	{
	public:

		Label (const String& label, Drawable * parent = NULL);

		virtual ~Label ();

		void set_text (const String& label);

		void set_font (const Font& font)
		{
			font_ = font;
			calculateBox();
		}

		void setTextColor (const Color& fg)
		{
			foreground_ = fg;
		}

		void set_background (const Color& color = Color(0x00000000))
		{
			background_ = color;
		}

	protected:

		virtual void update ();

		virtual void render ();

	private:
		// member functions

		virtual void cursorPosEvent (double xpos, double ypos);

		void calculateBox (void);

	private:

		/**
		 * @brief the text of the label
		 *
		 * new line character is not allowed
		 */
		String text_;

		Font font_;

		Color foreground_;

		/** Background color, default: transparent */
		Color background_;

	private:

		Label (const Label& orig);

		Label& operator = (const Label& orig);
	};

} /* namespace BIL */
#endif /* LABEL_H_ */
