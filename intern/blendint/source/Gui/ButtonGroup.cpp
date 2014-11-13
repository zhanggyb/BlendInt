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
		Clear();
	}
	
	void ButtonGroup::Prepend (AbstractButton* button)
	{
		if(!button) return;

		if(button->group_) {
			if(button->group_ == this) {
				DBG_PRINT_MSG("Button %s is already in this button group", button->name().c_str());
				return;
			} else {
				button->group_->Remove(button);
			}
		}

		assert(button->group_ == 0);

		buttons_.push_front(button);
		button->group_ = this;

		events_->connect(button->destroyed(), this, &ButtonGroup::OnButtonDestroyed);
	}

	void ButtonGroup::Append (AbstractButton* button)
	{
		if(!button) return;

		if(button->group_) {
			if(button->group_ == this) {
				DBG_PRINT_MSG("Button %s is already in this button group", button->name().c_str());
				return;
			} else {
				button->group_->Remove(button);
			}
		}

		assert(button->group_ == 0);

		buttons_.push_back(button);
		button->group_ = this;

		events_->connect(button->destroyed(), this, &ButtonGroup::OnButtonDestroyed);
	}
	
	void ButtonGroup::Insert (int index, AbstractButton* button)
	{
	}

	void ButtonGroup::Remove (AbstractButton* button)
	{
		if(!button) return;

		if(button->group_) {
			if(button->group_ != this) {
				DBG_PRINT_MSG("Button %s is NOT in this button group", button->name().c_str());
				return;
			}
		} else {
			return;
		}

		assert(button->group_ == this);

		std::deque<AbstractButton*>::iterator it = std::find(buttons_.begin(), buttons_.end(), button);
		if(it == buttons_.end()) {
			DBG_PRINT_MSG("Button %s is not in button group.", button->name().c_str());
			return;
		}

		button->destroyed().disconnectOne(this, &ButtonGroup::OnButtonDestroyed);

		buttons_.erase(it);
		button->group_ = 0;
	}
	
	void ButtonGroup::Clear ()
	{
		for(std::deque<AbstractButton*>::iterator it = buttons_.begin(); it != buttons_.end();)
		{
			(*it)->destroyed().disconnectOne(this, &ButtonGroup::OnButtonDestroyed);
			(*it)->group_ = 0;

			it = buttons_.erase(it);
		}
	}

	void ButtonGroup::Click (AbstractButton* button)
	{
		if(last_active_) {
			last_active_->set_down(false);
		}

		int i = 0;
		for(std::deque<AbstractButton*>::iterator it = buttons_.begin(); it != buttons_.end(); it++)
		{
			if((*it) == button) {
				last_active_ = *it;
				break;
			}

			i++;
		}

		button_clicked_.fire(last_active_);
		button_index_clicked_.fire(i);
	}

	void ButtonGroup::Toggle (AbstractButton* button, bool toggled)
	{
		AbstractButton* original_active_button = last_active_;

		if(mode_ == SingleSelection && last_active_) {
			last_active_->set_checked(false);
		}

		int i = 0;
		for(std::deque<AbstractButton*>::iterator it = buttons_.begin(); it != buttons_.end(); it++)
		{
			if((*it) == button) {
				last_active_ = *it;
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

	void ButtonGroup::OnButtonDestroyed (AbstractInteractiveForm* button)
	{
		AbstractButton* p = dynamic_cast<AbstractButton*>(button);
		Remove(p);
	}

}
