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

#include <BlendInt/Gui/Font.hpp>
#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Core/Margin.hpp>

namespace BlendInt {

	class ButtonGroup;

	class AbstractButton: public Widget
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

		Cpp::EventRef<> clicked() {return m_clicked;}

		Cpp::EventRef<bool> toggled() {return m_toggled;}

		bool pressed () const {return m_status[ButtonPressed];}

		static const Margin& DefaultButtonPadding ()
		{
			return default_button_padding;
		}

	protected:

		virtual ResponseType CursorEnterEvent (bool entered);

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

	private:

		enum ButtonStatusIndex {
			ButtonPressed = 0,
			ButtonDown,
			ButtonCheckable,
			ButtonChecked,
			ButtonCheckedOrigin
		};

		std::bitset<8> m_status;

		/**
		 * @brief press event
		 */
		Cpp::Event<> m_pressed;

		/**
		 * @brief release event
		 */
		Cpp::Event<> m_released;

		/**
		 * @brief click event
		 *
		 * Mouse press and release in the button causes a clicked event.
		 */
		Cpp::Event<> m_clicked;

		Cpp::Event<bool> m_toggled;

		static Margin default_button_padding;

	};

} /* namespace BIL */
#endif // _BLENDINT_ABSTRACTBUTTON_HPP_
