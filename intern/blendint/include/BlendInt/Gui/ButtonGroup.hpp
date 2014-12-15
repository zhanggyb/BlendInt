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

#ifndef _BLENDINT_GUI_BUTTONGROUP_HPP_
#define _BLENDINT_GUI_BUTTONGROUP_HPP_

#include <deque>
#include <boost/smart_ptr.hpp>

#include <BlendInt/Core/Object.hpp>
#include <BlendInt/Gui/AbstractButton.hpp>

#include <Cpp/Events.hpp>

namespace BlendInt {

	enum ButtonGroupMode {
		SingleSelection,
		MultipleSelection
	};

	class ButtonGroup: public Object
	{
	public:

		ButtonGroup ();

		~ButtonGroup ();

		void AddButton (AbstractButton* button);

		void InsertButton (int index, AbstractButton* button);

		void RemoveButton (AbstractButton* button);

		void Clear ();

		Cpp::EventRef<AbstractButton*> button_clicked() {return button_clicked_;}

		Cpp::EventRef<int> button_index_clicked() {return button_index_clicked_;}

		Cpp::EventRef<AbstractButton*> button_pressed() {return button_pressed_;}

		Cpp::EventRef<int> button_index_pressed() {return button_index_pressed_;}

		Cpp::EventRef<AbstractButton*> button_released() {return button_released_;}

		Cpp::EventRef<int> button_index_released() {return button_index_released_;}

		Cpp::EventRef<AbstractButton*, bool> button_toggled() {return button_toggled_;}

		Cpp::EventRef<int, bool> button_index_toggled() {return button_index_toggled_;}

		size_t button_count () const {return buttons_.size();}

		const std::deque<AbstractButton*>& buttons () const
		{
			return buttons_;
		}

	private:

		friend class AbstractButton;

		void Click (AbstractButton* button);

		void Toggle (AbstractButton* button, bool toggled);

		void OnButtonDestroyed (AbstractView* button);

		std::deque<AbstractButton*> buttons_;

		boost::scoped_ptr<Cpp::ConnectionScope> events_;

		AbstractButton* last_active_;

		ButtonGroupMode mode_;

		Cpp::Event<AbstractButton*> button_clicked_;

		Cpp::Event<int> button_index_clicked_;

		Cpp::Event<AbstractButton*> button_pressed_;

		Cpp::Event<int> button_index_pressed_;

		Cpp::Event<AbstractButton*> button_released_;

		Cpp::Event<int> button_index_released_;

		Cpp::Event<AbstractButton*, bool> button_toggled_;

		Cpp::Event<int, bool> button_index_toggled_;

	};

}

#endif /* _BLENDINT_GUI_BUTTONGROUP_HPP_ */
