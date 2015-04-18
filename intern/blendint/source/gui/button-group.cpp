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

#include <gui/abstract-widget.hpp>
#include <gui/abstract-button.hpp>

#include <gui/button-group.hpp>

namespace BlendInt {

	ButtonGroup::ButtonGroup ()
	: last_active_(0),
	  mode_(SingleSelection)
	{
	}

	ButtonGroup::~ButtonGroup ()
	{
		Clear();
	}
	
	void ButtonGroup::AddButton (AbstractButton* button)
	{
		if(!button) return;

		if(button->group_) {
			if(button->group_ == this) {
				DBG_PRINT_MSG("PushButton %s is already in this button group", button->name().c_str());
				return;
			} else {
				button->group_->RemoveButton(button);
			}
		}

		DBG_ASSERT(button->group_ == 0);

		buttons_.push_back(button);
		button->group_ = this;

		button->destroyed().connect(this, &ButtonGroup::OnButtonDestroyed);
	}
	
	void ButtonGroup::InsertButton (int index, AbstractButton* button)
	{
	}

	void ButtonGroup::RemoveButton (AbstractButton* button)
	{
		if(!button) return;

		if(button->group_) {
			if(button->group_ != this) {
				DBG_PRINT_MSG("PushButton %s is NOT in this button group", button->name().c_str());
				return;
			}
		} else {
			return;
		}

		DBG_ASSERT(button->group_ == this);

		std::deque<AbstractButton*>::iterator it = std::find(buttons_.begin(), buttons_.end(), button);
		if(it == buttons_.end()) {
			DBG_PRINT_MSG("PushButton %s is not in button group.", button->name().c_str());
			return;
		}

		button->destroyed().disconnect1(this, &ButtonGroup::OnButtonDestroyed);

		buttons_.erase(it);
		button->group_ = 0;
	}
	
	void ButtonGroup::Clear ()
	{
		for(std::deque<AbstractButton*>::iterator it = buttons_.begin(); it != buttons_.end();)
		{
			(*it)->destroyed().disconnect1(this, &ButtonGroup::OnButtonDestroyed);
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

		button_clicked_.Invoke(last_active_);
		button_index_clicked_.Invoke(i);
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

		button_toggled_.Invoke(last_active_, toggled);
		button_index_toggled_.Invoke(i, toggled);
	}

	void ButtonGroup::OnButtonDestroyed (AbstractWidget* button)
	{
		AbstractButton* p = dynamic_cast<AbstractButton*>(button);
		RemoveButton(p);
	}

}
