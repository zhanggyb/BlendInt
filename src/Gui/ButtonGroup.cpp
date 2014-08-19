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
	: last_active_(0),
	  mode_(SingleSelection)
	{
		events_.reset(new Cpp::ConnectionScope);
	}

	ButtonGroup::~ButtonGroup ()
	{
	}
	
	void ButtonGroup::Add (AbstractButton* button)
	{
		if(!button) return;

		std::deque<AbstractButton*>::iterator it = std::find(buttons_.begin(), buttons_.end(), button);
		if(it != buttons_.end()) {
			DBG_PRINT_MSG("Button %s already in button group.", button->name().c_str());
			return;
		}

		buttons_.push_back(button);

		events_->connect(button->destroyed(), this, &ButtonGroup::OnButtonDestroyed);
		events_->connect(button->clicked(), this, &ButtonGroup::OnButtonClicked);
		events_->connect(button->toggled(), this, &ButtonGroup::OnButtonToggled);
	}
	
	void ButtonGroup::Remove (AbstractButton* button)
	{
		if(!button) return;

		std::deque<AbstractButton*>::iterator it = std::find(buttons_.begin(), buttons_.end(), button);
		if(it == buttons_.end()) {
			DBG_PRINT_MSG("Button %s is not in button group.", button->name().c_str());
			return;
		}

		button->toggled().disconnectOne(this, &ButtonGroup::OnButtonToggled);
		button->clicked().disconnectOne(this, &ButtonGroup::OnButtonClicked);
		button->destroyed().disconnectOne(this, &ButtonGroup::OnButtonDestroyed);
		buttons_.erase(it);
	}
	
	void ButtonGroup::OnButtonClicked ()
	{
		DBG_PRINT_MSG("%s", "get button clicked event");

		if(last_active_) {
			last_active_->SetDown(false);
		}

		int i = 0;
		for(std::deque<AbstractButton*>::iterator it = buttons_.begin(); it != buttons_.end(); it++)
		{
			last_active_ = *it;
			if(last_active_->focused()) {
				break;
			}
			i++;
		}

		button_clicked_.fire(last_active_);
		button_index_clicked_.fire(i);
	}
	
	void ButtonGroup::OnButtonToggled (bool toggled)
	{
		AbstractButton* original_active_button = last_active_;

		if(mode_ == SingleSelection && last_active_) {
			last_active_->set_checked(false);
		}

		int i = 0;
		for(std::deque<AbstractButton*>::iterator it = buttons_.begin(); it != buttons_.end(); it++)
		{
			last_active_ = *it;
			if(last_active_->focused()) {
				break;
			}
			i++;
		}

		if(mode_ == SingleSelection) {
			last_active_->set_checked(true);
			toggled = true;
		} else {
			last_active_->set_checked(toggled);
		}

		if(original_active_button == last_active_) return;	// Do not fire events repeatedly

		button_toggled_.fire(last_active_, toggled);
		button_index_toggled_.fire(i, toggled);
	}

	void ButtonGroup::OnButtonDestroyed (AbstractWidget* button)
	{
		AbstractButton* p = dynamic_cast<AbstractButton*>(button);
		Remove(p);
	}

}
