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

#include <Cpp/Events.hpp>

#include <BIL/Widget.hpp>
#include <BIL/Font.hpp>
#include <BIL/Tuple.hpp>
#include <BIL/TextBuffer.hpp>

using namespace std;

namespace BIL {

	class AbstractButton: public Widget
	{
	public:

		AbstractButton (Widget* parent = NULL);

		virtual ~AbstractButton ();

		void set_text (const String& text);

		void set_font (const Font& font)
		{
			text_.setFont(font);
			calculateBox();
			update();
		}

		bool down () const {return down_;}

		void setDown (bool down) {down_ = down;}

		bool checked () const {return checked_;}

		bool checkable () const {return checkable_;}

		void setCheckable (bool checkable) {checkable_ = checkable;}

	public:	// event connection interface

		Cpp::EventRef<> clicked() {return clicked_;}

		Cpp::EventRef<bool> toggled() {return toggled_;}

	protected:

		void drawButton (WidgetVertexes* vertexes);

		virtual void update () = 0;

		virtual void render () = 0;

		virtual void mousePressEvent (MouseEvent* event);

		virtual void mouseReleaseEvent (MouseEvent* event);

		virtual void mouseMoveEvent (MouseEvent* event);

	protected:

		bool down_;

		bool checkable_;
		
		bool checked_;

		// if the mouse is hover on the button
		bool hover_;

		TextBuffer text_;

	protected:	// Events

		Cpp::Event<> clicked_;

		Cpp::Event<bool> toggled_;

	private:	// member functions

		void calculateBox ();

	private:	// member functions (disabled)

		DISALLOW_COPY_AND_ASSIGN(AbstractButton);
	};

} /* namespace BIL */
#endif /* ABSTRACTBUTTON_H_ */
