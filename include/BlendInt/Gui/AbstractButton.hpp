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

#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Gui/Font.hpp>
#include <BlendInt/Gui/TextBuffer.hpp>

using namespace std;

namespace BlendInt {

	class AbstractButton: public Widget
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractButton);

	public:

		AbstractButton ();

		virtual ~AbstractButton ();

		void SetText (const String& text);

		Font& font () {return m_font;}

		void SetFont (const Font& font);

		bool down () const {return m_status_down;}

		void set_down (bool down) {m_status_down = down;}

		bool checked () const {return m_status_checked;}

		bool checkable () const {return m_checkable;}

		void set_checkable (bool checkable) {m_checkable = checkable;}

		const String& text () const {return m_text;}

		size_t text_length () const {return m_text_length;}

		Cpp::EventRef<> clicked() {return m_clicked;}

		Cpp::EventRef<bool> toggled() {return m_toggled;}

	protected:

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

		inline void set_text (const String& text)
		{
			m_text = text;
		}

		inline void set_font (const Font& font)
		{
			m_font = font;
		}

		void set_text_outline (const Rect& outline)
		{
			m_text_outline = outline;
		}

		const Rect& text_outline () const
		{
			return m_text_outline;
		}

		void set_origin (const Point& origin)
		{
			m_origin = origin;
		}

		void set_origin (int x, int y)
		{
			m_origin.set_x(x);
			m_origin.set_y(y);
		}

		void set_text_length (size_t length)
		{
			m_text_length = length;
		}

		size_t GetValidTextSize ();

		const Point& origin () const {return m_origin;}

	private:

		bool m_status_down;

		bool m_checkable;
		
		bool m_status_checked;

		size_t m_text_length;	// How many text to be printed, depends on the button size

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
