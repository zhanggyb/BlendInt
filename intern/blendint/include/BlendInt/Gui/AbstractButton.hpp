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

#pragma once

#include <string>
#include <bitset>

#include <Cpp/Events.hpp>

#include <BlendInt/Core/Margin.hpp>

#include <BlendInt/Gui/Text.hpp>
#include <BlendInt/Gui/Widget.hpp>

namespace BlendInt {

	class ButtonGroup;

	class AbstractButton: public Widget
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractButton);

	public:

		AbstractButton ();

		virtual ~AbstractButton ();

		void SetDown (bool down);

		void SetCheckable (bool checkabel);

		void SetChecked (bool checked);

		virtual Size GetPreferredSize () const;

		void SetText (const String& text);

		void SetFont (const Font& font);

		bool is_down () const {return m_status[ButtonDown];}

		bool is_checked () const {return m_status[ButtonChecked];}

		bool is_checkable () const {return m_status[ButtonCheckable];}

		bool is_pressed () const {return m_status[ButtonPressed];}

		Cpp::EventRef<AbstractButton*> clicked() {return clicked_;}

		Cpp::EventRef<AbstractButton*, bool> toggled() {return toggled_;}

		Cpp::EventRef<AbstractButton*> pressed () {return pressed_;}

		static Margin kPadding;

		static int kIconTextSpace;	// the space between icon and text

		const RefPtr<Text>& text () const
		{
			return text_;
		}

	protected:

		virtual ResponseType Draw (const Context* context) = 0;

		virtual void PerformHoverIn (const Context* context);

		virtual void PerformHoverOut (const Context* context);

		virtual ResponseType PerformMousePress (const Context* context);

		virtual ResponseType PerformMouseRelease (const Context* context);

		virtual ResponseType PerformMouseMove (const Context* context);

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

		void set_text (const RefPtr<Text>& text)
		{
			text_ = text;
		}

	private:

		friend class ButtonGroup;

		enum ButtonStatusIndex {
			ButtonPressed = 0,
			ButtonDown,
			ButtonCheckable,
			ButtonChecked,
			ButtonLastChecked
		};

		RefPtr<Text> text_;

		std::bitset<8> m_status;

		/**
		 * @brief press event
		 */
		Cpp::Event<AbstractButton*> pressed_;

		/**
		 * @brief release event
		 */
		Cpp::Event<AbstractButton*> released_;

		/**
		 * @brief click event
		 *
		 * Mouse press and release in the button causes a clicked event.
		 */
		Cpp::Event<AbstractButton*> clicked_;

		Cpp::Event<AbstractButton*, bool> toggled_;

		ButtonGroup* group_;
	};

} /* namespace BIL */
