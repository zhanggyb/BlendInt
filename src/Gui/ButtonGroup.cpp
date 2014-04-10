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

#include <algorithm>

#include <BlendInt/Gui/ButtonGroup.hpp>

namespace BlendInt {

	ButtonGroup::ButtonGroup ()
	: m_last_active(0), m_mode (SingleSelection)
	{
		m_events.reset(new Cpp::ConnectionScope);
	}

	ButtonGroup::~ButtonGroup ()
	{
	}
	
	void ButtonGroup::Add (AbstractButton* button)
	{
		if(!button) return;

		std::deque<AbstractButton*>::iterator it = std::find(m_group.begin(), m_group.end(), button);
		if(it != m_group.end()) {
			DBG_PRINT_MSG("Button %s already in button group.", button->name().c_str());
			return;
		}

		m_group.push_back(button);

		m_events->connect(button->destroyed(), this, &ButtonGroup::OnButtonDestroyed);
		m_events->connect(button->clicked(), this, &ButtonGroup::OnButtonClicked);
		m_events->connect(button->toggled(), this, &ButtonGroup::OnButtonToggled);
	}
	
	void ButtonGroup::Remove (AbstractButton* button)
	{
		if(!button) return;

		std::deque<AbstractButton*>::iterator it = std::find(m_group.begin(), m_group.end(), button);
		if(it == m_group.end()) {
			DBG_PRINT_MSG("Button %s is not in button group.", button->name().c_str());
			return;
		}

		button->toggled().disconnectOne(this, &ButtonGroup::OnButtonToggled);
		button->clicked().disconnectOne(this, &ButtonGroup::OnButtonClicked);
		button->destroyed().disconnectOne(this, &ButtonGroup::OnButtonDestroyed);
		m_group.erase(it);
	}
	
	void ButtonGroup::OnButtonClicked ()
	{
		DBG_PRINT_MSG("%s", "get button clicked event");

		if(m_last_active) {
			m_last_active->SetDown(false);
		}

		int i = 0;
		for(std::deque<AbstractButton*>::iterator it = m_group.begin(); it != m_group.end(); it++)
		{
			m_last_active = *it;
			if(m_last_active->pressed()) {
				break;
			}
			i++;
		}

		m_button_clicked.fire(m_last_active);
		m_button_index_clicked.fire(i);
	}
	
	void ButtonGroup::OnButtonToggled (bool toggled)
	{
		if(m_mode == SingleSelection && m_last_active) {
			m_last_active->set_checked(false);
		}

		int i = 0;
		for(std::deque<AbstractButton*>::iterator it = m_group.begin(); it != m_group.end(); it++)
		{
			m_last_active = *it;
			if(m_last_active->pressed()) {
				break;
			}
			i++;
		}

		if(m_mode == SingleSelection) {
			m_last_active->set_checked(true);
			toggled = true;
		} else {
			m_last_active->set_checked(toggled);
		}

		DBG_PRINT_MSG("Button %s toggled: %d", m_last_active->name().c_str(), toggled? 1 : 0);

		m_button_toggled.fire(m_last_active, toggled);
		m_button_index_toggled.fire(i, toggled);
	}

	void ButtonGroup::OnButtonDestroyed (AbstractWidget* button)
	{
		AbstractButton* p = dynamic_cast<AbstractButton*>(button);
		Remove(p);
	}

}
