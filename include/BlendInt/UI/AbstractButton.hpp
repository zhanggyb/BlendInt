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

#ifndef _BLENDINT_ABSTRACTBUTTON_HPP_
#define _BLENDINT_ABSTRACTBUTTON_HPP_

#include <string>

#include <Cpp/Events.hpp>

#include <BlendInt/UI/RoundWidget.hpp>
#include <BlendInt/UI/Font.hpp>
#include <BlendInt/UI/TextBuffer.hpp>

using namespace std;

namespace BlendInt {

	class AbstractButton: public RoundWidget
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractButton);

	public:

		AbstractButton ();

		AbstractButton (AbstractWidget* parent);

		virtual ~AbstractButton ();

		void set_text (const String& text);

		const Font& font () const {return m_font;}

		void set_font (const Font& font);

		bool down () const {return m_status_down;}

		bool hover () const {return m_status_hover;}

		void set_down (bool down) {m_status_down = down;}

		bool checked () const {return m_status_checked;}

		bool checkable () const {return m_checkable;}

		void set_checkable (bool checkable) {m_checkable = checkable;}

		const String& text () const {return m_text;}

		Cpp::EventRef<> clicked() {return m_clicked;}

		Cpp::EventRef<bool> toggled() {return m_toggled;}

	protected:

		virtual void CursorEnterEvent (bool entered);

		virtual void MousePressEvent (MouseEvent* event);

		virtual void MouseReleaseEvent (MouseEvent* event);

		virtual void MouseMoveEvent (MouseEvent* event);

		void set_hover (bool status)
		{
			m_status_hover = status;
		}

		size_t get_valid_text_size ();

		size_t valid_text_length() const {return m_length;}

		const Point& origin () const {return m_origin;}

	private:

		bool m_status_down;

		bool m_checkable;
		
		bool m_status_checked;

		// if the mouse is hover on the button
		bool m_status_hover;

		size_t m_length;

		Point m_origin;

		// TextBuffer text_;
		String m_text;

		Font m_font;

		/**
		 * @brief Box in which hold the text
		 */
		Rect m_text_outline;

		Cpp::Event<> m_clicked;

		Cpp::Event<bool> m_toggled;
	};

} /* namespace BIL */
#endif // _BLENDINT_ABSTRACTBUTTON_HPP_
