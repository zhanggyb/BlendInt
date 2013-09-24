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

#ifndef _BILO_ABSTRACTBUTTON_H_
#define _BILO_ABSTRACTBUTTON_H_

#include <string>

#include <Cpp/Events.hpp>

#include <BILO/Widget.hpp>
#include <BILO/Font.hpp>
#include <BILO/Tuple.hpp>
#include <BILO/TextBuffer.hpp>

using namespace std;

namespace BILO {

	class AbstractButton: public Widget
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractButton);

	public:

		AbstractButton ();

		AbstractButton (Drawable* parent);

		virtual ~AbstractButton ();

		void set_text (const String& text);

		void set_font (const Font& font);

		bool down () const {return down_;}

		void setDown (bool down) {down_ = down;}

		bool checked () const {return checked_;}

		bool checkable () const {return checkable_;}

		void setCheckable (bool checkable) {checkable_ = checkable;}

	public:	// event connection interface

		Cpp::EventRef<> clicked() {return clicked_;}

		Cpp::EventRef<bool> toggled() {return toggled_;}

	protected:

		void updateVertexArray (float x, float y, float sx, float sy);

		virtual void render () = 0;

		virtual void press_mouse (MouseEvent* event);

		virtual void release_mouse (MouseEvent* event);

		virtual void move_mouse (MouseEvent* event);

	protected:

		bool down_;

		bool checkable_;
		
		bool checked_;

		// if the mouse is hover on the button
		bool hover_;

		// TextBuffer text_;
		String text_;

		Font font_;

		Vertex2D* vertex_array_;

		int valid_text_length_;

	protected:	// Events

		Cpp::Event<> clicked_;

		Cpp::Event<bool> toggled_;
	};

} /* namespace BIL */
#endif // _BILO_ABSTRACTBUTTON_HPP_
