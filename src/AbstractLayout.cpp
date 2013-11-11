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

#include <BlendInt/AbstractLayout.hpp>

namespace BlendInt {

	AbstractLayout::AbstractLayout ()
			: AbstractWidget(),
			  m_alignment(0),
			  m_space(1),
			  m_sizing_mode(LayoutFlow)
	{
		set_preferred_size(margin().left() + margin().right(), margin().top() + margin().bottom());
		set_minimal_size(margin().left() + margin().right(), margin().top() + margin().bottom());
		resize(preferred_size());
	}

	AbstractLayout::AbstractLayout (AbstractWidget *parent)
			: AbstractWidget (parent),
			  m_alignment(0),
			  m_space(1),
			  m_sizing_mode(LayoutFlow)
	{
		set_preferred_size(margin().left() + margin().right(), margin().top() + margin().bottom());
		set_minimal_size(margin().left() + margin().right(), margin().top() + margin().bottom());
		resize(preferred_size());
	}

	AbstractLayout::~AbstractLayout ()
	{
		m_items.clear();
	}

	void AbstractLayout::add (Widget* form)
	{
		if(children().count(form)) return;

		Size pre_preferred_size = preferred_size();

		add_item (form);

		if(! (pre_preferred_size == preferred_size())) {
			// fire events
		}

		// if minimal size changed
		// fire events

	}

	void AbstractLayout::add (AbstractLayout* object)
	{
		if(children().count(object)) return;

		bind(object);
		set_in_layout(object, true);
		m_items.push_back(object);
	}

	bool AbstractLayout::remove (AbstractWidget* object)
	{
		if (!children().count(object)) return false;

		remove_item(object);

		return true;
	}

	bool AbstractLayout::erase (AbstractWidget* object)
	{
		if (!children().count(object)) return false;

		remove_item(object);

		delete object;

		return true;
	}

	/*
	bool AbstractLayout::update(int type, const void* property)
	{
		switch (type) {

			case FormPropertyPosition: {
				const Point* new_pos = static_cast<const Point*>(property);

				for (size_t i = 0; i < m_items.size(); i++)
				{
					set_pos_priv(m_items[i], m_items[i]->position().x() + (new_pos->x() - m_pos.x()),
							m_items[i]->position().y() + (new_pos->y() - m_pos.y()));
				}
				return true;
			}

			case FormPropertySize: {
				// always return true in Base Layout
				return true;
			}

			case LayoutPropertyMargin: {
				const Margin* new_margin = static_cast<const Margin*>(property);
				int w = new_margin->left() - m_margin.left() + new_margin->right() - m_margin.right();
				int h = new_margin->top() - m_margin.top() + new_margin->bottom() - m_margin.bottom();
				m_size.add_width(w);
				m_size.add_height(h);
				m_minimal_size.add_width(w);
				m_minimal_size.add_height(h);

				return true;
			}

			default:
				break;
		}

		return true;
	}
	*/

	void AbstractLayout::set_margin (const Margin& margin)
	{
		Margin new_value = margin;

		m_margin = new_value;
	}

	void AbstractLayout::set_margin (int left, int right, int top, int bottom)
	{
		Margin new_value (left, right, top, bottom);

		m_margin = new_value;
	}

	void AbstractLayout::update (int type, const void* data)
	{
		switch (type) {

			case FormPosition: {
				const Point* new_pos = static_cast<const Point*>(data);

				for (size_t i = 0; i < m_items.size(); i++)
				{
					dynamic_cast<AbstractExpForm*>(m_items[i])->set_position(
							m_items[i]->position().x() + (new_pos->x() - position().x()),
							m_items[i]->position().y() + (new_pos->y() - position().y()));

				}
				break;
			}

			default:
				break;
		}
	}

	AbstractLayout* AbstractLayout::root_layout ()
	{
		AbstractLayout* root = 0;

		if(in_layout()) {
			root = dynamic_cast<AbstractLayout*>(parent().object.form);
			while(root->in_layout()) {
				root = dynamic_cast<AbstractLayout*>(root->parent().object.form);
				if(!root) break;
			}
		}

		return root;
	}

} /* namespace BlendInt */

