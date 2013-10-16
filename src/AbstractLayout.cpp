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
		resize(margin().left() + margin().right(), margin().top() + margin().bottom());
		set_minimal_size(margin().left() + margin().right(), margin().top() + margin().bottom());
	}

	AbstractLayout::AbstractLayout (Drawable *parent)
			: Drawable (parent), m_alignment(0), m_space(1)
	{
		resize(margin().left() + margin().right(), margin().top() + margin().bottom());
		set_minimal_size(margin().left() + margin().right(), margin().top() + margin().bottom());
	}

	AbstractLayout::~AbstractLayout ()
	{
		m_items.clear();
	}

	void AbstractLayout::add (Drawable* object)
	{
		if(m_children.count(object)) return;

		set_in_layout(object, true);

		ItemData item;
		item.action = Add;
		item.object = object;

		if(update(LayoutPropertyItem, &item)) set_in_layout(object, true);
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

		ItemData item;
		item.action = Remove;
		item.object = object;

		update(LayoutPropertyItem, &item);

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

		ItemData item;
		item.action = Remove;
		item.object = object;

		update(LayoutPropertyItem, &item);

		delete object;

		return true;
	}

	bool AbstractLayout::update(int type, const void* property)
	{
		switch (type) {

			case BasicPropertyPosition: {
				const Point* new_pos = static_cast<const Point*>(property);

				for (size_t i = 0; i < m_items.size(); i++)
				{
					set_pos_priv(m_items[i], m_items[i]->pos().x() + (new_pos->x() - m_pos.x()),
							m_items[i]->pos().y() + (new_pos->y() - m_pos.y()));
				}
				return true;
			}

			case BasicPropertySize: {
				// always return true in Base Layout
				return true;
			}

			case LayoutPropertyMargin: {
				const Margin* new_margin = static_cast<const Margin*>(property);

				m_size.add_width(new_margin->left() - m_margin.left() + new_margin->right() - m_margin.right());
				m_size.add_height(new_margin->top() - m_margin.top() + new_margin->bottom() - m_margin.bottom());

				return true;
			}

			default:
				break;
		}

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

