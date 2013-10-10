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

#ifndef _BILO_ABSTRACTBUTTON_HPP_
#define _BILO_ABSTRACTBUTTON_HPP_

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

		bool down () const {return m_status_down;}

		void setDown (bool down) {m_status_down = down;}

		bool checked () const {return m_status_checked;}

		bool checkable () const {return m_checkable;}

		void setCheckable (bool checkable) {m_checkable = checkable;}

		Cpp::EventRef<> clicked() {return m_clicked;}

		Cpp::EventRef<bool> toggled() {return m_toggled;}

	protected:

		virtual void render () = 0;

		virtual void press_mouse (MouseEvent* event);

		virtual void release_mouse (MouseEvent* event);

		virtual void move_mouse (MouseEvent* event);

	protected:

		bool m_status_down;

		bool m_checkable;
		
		bool m_status_checked;

		// if the mouse is hover on the button
		bool m_status_hover;

		// TextBuffer text_;
		String m_text;

		Font m_font;

		/**
		 * @brief Box in which hold the text
		 */
		Rect m_text_outline;

	protected:	// Events

		Cpp::Event<> m_clicked;

		Cpp::Event<bool> m_toggled;
	};

} /* namespace BIL */
#endif // _BILO_ABSTRACTBUTTON_HPP_
