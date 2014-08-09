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
#include <bitset>

#include <Cpp/Events.hpp>

#include <BlendInt/Core/Margin.hpp>

#include <BlendInt/Gui/Font.hpp>
#include <BlendInt/Gui/AbstractWidget.hpp>

namespace BlendInt {

	class ButtonGroup;

	class AbstractButton: public AbstractWidget
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractButton);

	public:

		friend class ButtonGroup;

		AbstractButton ();

		virtual ~AbstractButton ();

		void SetDown (bool down);

		void SetCheckable (bool checkabel);

		void SetChecked (bool checked);

		bool down () const {return m_status[ButtonDown];}

		bool checked () const {return m_status[ButtonChecked];}

		bool checkable () const {return m_status[ButtonCheckable];}

		Cpp::EventRef<> clicked() {return clicked_;}

		Cpp::EventRef<bool> toggled() {return toggled_;}

		bool pressed () const {return m_status[ButtonPressed];}

		virtual Size GetPreferredSize () const;

		void SetText (const String& text);

		void SetFont (const Font& font);

		const String& text () const {return text_;}

		int text_length () const {return text_length_;}

		const Font& font () const {return font_;}

		static const Margin& DefaultButtonPadding ()
		{
			return default_button_padding;
		}

	protected:

		virtual ResponseType FocusEvent (bool focus);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

		void set_down (bool down)
		{
			m_status[ButtonDown] = down ? 1 : 0;
		}

		void set_checkable (bool checkable)
		{
			m_status[ButtonCheckable] = checkable ? 1 : 0;
		}

		void set_checked (bool checked)
		{
			m_status[ButtonChecked] = checked ? 1 : 0;
		}

		void UpdateTextPosition (const Size& size, int round_type, float radius, const String& text);

		int UpdateTextPosition (const Size& size, int round_type, float radius, const String& text, Font& font);

		inline void set_text (const String& text)
		{
			text_ = text;
		}

		inline void set_font (const Font& font)
		{
			font_ = font;
		}

		void set_text_length (int length)
		{
			text_length_ = length;
		}

		void set_pen (int x, int y)
		{
			font_.set_pen(x, y);
		}

		int GetValidTextLength (const String& text, const Font& font, int max_width);

	private:

		enum ButtonStatusIndex {
			ButtonPressed = 0,
			ButtonDown,
			ButtonCheckable,
			ButtonChecked,
			ButtonLastChecked
		};

		std::bitset<8> m_status;

		int text_length_;	// How many text to be printed, depends on the button size

		// TextBuffer text_;
		String text_;

		Font font_;

		/**
		 * @brief press event
		 */
		Cpp::Event<> pressed_;

		/**
		 * @brief release event
		 */
		Cpp::Event<> released_;

		/**
		 * @brief click event
		 *
		 * Mouse press and release in the button causes a clicked event.
		 */
		Cpp::Event<> clicked_;

		Cpp::Event<bool> toggled_;

		static Margin default_button_padding;

	};

} /* namespace BIL */
#endif // _BLENDINT_ABSTRACTBUTTON_HPP_
