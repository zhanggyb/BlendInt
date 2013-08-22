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

#ifndef _BIL_ABSTRACTBUTTON_H_
#define _BIL_ABSTRACTBUTTON_H_

#include <string>

#include <BIL/Widget.hpp>
#include <BIL/Font.hpp>
#include <BIL/Tuple.hpp>
#include <BIL/TextBuffer.hpp>

using namespace std;

namespace BIL {

	class AbstractButton: public BIL::Widget
	{
	public:

		explicit AbstractButton (Widget* parent = NULL);

		virtual ~AbstractButton ();

		void set_text (const wstring& text);

		void set_font (const Font& font)
		{
			text_.set_font(font);
			calculateBox();
			Update();
		}

		bool down () const {return down_;}

		void set_down (bool down) {down_ = down;}

		bool checked () const {return checked_;}

		bool checkable () const {return checkable_;}

		void set_checkable (bool checkable) {checkable_ = checkable;}

	protected:

		virtual void Update () = 0;

		virtual void Render () = 0;

	protected:

		bool down_;

		bool checkable_;
		
		bool checked_;

		// if the mouse is hover on the button
		bool hover_;

		TextBuffer text_;

	private:	// member functions

		void calculateBox ();

	private:	// member functions (disabled)

		AbstractButton (const AbstractButton& orig);
		AbstractButton& operator = (const AbstractButton& orig);
	};

} /* namespace BIL */
#endif /* ABSTRACTBUTTON_H_ */
