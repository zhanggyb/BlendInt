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

/*
 * AbstractButton.h
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#ifndef _BIL_ABSTRACTBUTTON_H_
#define _BIL_ABSTRACTBUTTON_H_

#include <string>

#include <BIL/Widget.h>
#include <BIL/Font.h>
#include <BIL/Tuple.h>
#include <BIL/TextBuffer.h>

using namespace std;

namespace BIL {

	class AbstractButton: public BIL::Widget
	{
	public:

		AbstractButton (const wstring& text, Drawable* parent = NULL);

		virtual ~AbstractButton ();

		void setText (const wstring& text);

		void setFont (const Font& font);

		// void setCheckable (bool checkable);

		void setBackground (const RGBAf& color)
		{
			_bg = color;
		}

		void setCornerRadius (float rad)
		{
			_cornerRadius = rad;
		}

		void render (void);

	private:	// member functions

		void calculateBox (void);

	private:	// member functions (disabled)

		AbstractButton (const AbstractButton& orig);
		AbstractButton& operator = (const AbstractButton& orig);

	private:

		TextBuffer _text;

		RGBAf _bg;

		float _cornerRadius;

	};

} /* namespace BIL */
#endif /* ABSTRACTBUTTON_H_ */
