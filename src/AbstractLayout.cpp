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
			: Drawable(), m_alignment(0), m_space(1)
	{
	}

	AbstractLayout::AbstractLayout (Drawable *parent)
			: Drawable (parent), m_alignment(0), m_space(1)
	{
	}

	AbstractLayout::~AbstractLayout ()
	{
		m_items.clear();
	}

	void AbstractLayout::add (Widget* widget)
	{
		if(m_children.count(widget)) return;

		m_items.push_back(widget);
		bind (widget);
		set_in_layout(widget, true);

		set_expand_x(widget->expand_x());
		set_expand_y(widget->expand_y());

		update(LayoutPropertyItem, 0);
	}

	void AbstractLayout::add (AbstractLayout* layout)
	{
		if(m_children.count(layout)) return;

		m_items.push_back(layout);
		bind (layout);
		set_in_layout(layout, true);

		set_expand_x(layout->expand_x());
		set_expand_y(layout->expand_y());

		update(LayoutPropertyItem, 0);
	}

	void AbstractLayout::refresh ()
	{
		Size size = m_size;

		update(BasicPropertySize, &size);
	}

	bool AbstractLayout::remove (Drawable* object)
	{
		if (!m_children.count(object)) return false;

		m_children.erase(object);

		unbind(object);
		set_in_layout(object, false);

		std::vector<Drawable*>::iterator it;
		for(it = m_items.begin(); it != m_items.end(); it++)
		{
			if (*it == object) {
				it = m_items.erase(it);
			}
		}

		// TODO: not use update()
		update(LayoutPropertyItem, 0);

		return true;
	}

	bool AbstractLayout::erase (Drawable* object)
	{
		if (!m_children.count(object)) return false;

		m_children.erase(object);

		unbind(object);
		set_in_layout(object, false);

		std::vector<Drawable*>::iterator it;
		for(it = m_items.begin(); it != m_items.end(); it++)
		{
			if (*it == object) {
				it = m_items.erase(it);
			}
		}

		delete object;

		// TODO: not use update()
		update(LayoutPropertyItem, 0);

		return true;
	}

	void AbstractLayout::set_margin (const Margin& margin)
	{
		Margin new_value = margin;

		if(update(LayoutPropertyMargin, &new_value)) m_margin = new_value;
	}

	void AbstractLayout::set_margin (int left, int right, int top, int bottom)
	{
		Margin new_value (left, right, top, bottom);

		if(update(LayoutPropertyMargin, &new_value)) m_margin = new_value;
	}

} /* namespace BlendInt */

