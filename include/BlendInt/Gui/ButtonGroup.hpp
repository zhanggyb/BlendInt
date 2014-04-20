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

#include <vector>
#include <boost/smart_ptr.hpp>

#include <Cpp/Events.hpp>

#include <BlendInt/Core/Object.hpp>

#include <BlendInt/Gui/AbstractButton.hpp>

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

		void Add (AbstractButton* button);

		void Remove (AbstractButton* button);

		Cpp::EventRef<AbstractButton*> button_clicked() {return m_button_clicked;}

		Cpp::EventRef<int> button_index_clicked() {return m_button_index_clicked;}

		Cpp::EventRef<AbstractButton*> button_pressed() {return m_button_pressed;}

		Cpp::EventRef<int> button_index_pressed() {return m_button_index_pressed;}

		Cpp::EventRef<AbstractButton*> button_released() {return m_button_released;}

		Cpp::EventRef<int> button_index_released() {return m_button_index_released;}

		Cpp::EventRef<AbstractButton*, bool> button_toggled() {return m_button_toggled;}

		Cpp::EventRef<int, bool> button_index_toggled() {return m_button_index_toggled;}

		size_t size () const {return m_group.size();}

		std::vector<AbstractButton*>* buttons ()
		{
			return &m_group;
		}

	private:

		void OnButtonClicked ();

		void OnButtonToggled (bool toggled);

		void OnButtonDestroyed (AbstractWidget* button);

		std::vector<AbstractButton*> m_group;

		boost::scoped_ptr<Cpp::ConnectionScope> m_events;

		AbstractButton* m_last_active;

		ButtonGroupMode m_mode;

		Cpp::Event<AbstractButton*> m_button_clicked;

		Cpp::Event<int> m_button_index_clicked;

		Cpp::Event<AbstractButton*> m_button_pressed;

		Cpp::Event<int> m_button_index_pressed;

		Cpp::Event<AbstractButton*> m_button_released;

		Cpp::Event<int> m_button_index_released;

		Cpp::Event<AbstractButton*, bool> m_button_toggled;

		Cpp::Event<int, bool> m_button_index_toggled;

	};

}

#endif /* _BLENDINT_GUI_BUTTONGROUP_HPP_ */
