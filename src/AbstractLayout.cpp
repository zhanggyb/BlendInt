/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
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
		set_expand(true);
	}

	AbstractLayout::AbstractLayout (Drawable *parent)
			: Drawable (parent), m_alignment(0), m_space(1)
	{
		set_expand(true);
	}

	AbstractLayout::~AbstractLayout ()
	{
		m_vector.clear();
	}


	void AbstractLayout::add_widget (Widget* widget)
	{
		if (m_children.count(widget)) return;

		m_vector.push_back(widget);
		bind (widget);

		update(WidgetPropertySize, 0);
	}

	void AbstractLayout::add_layout (AbstractLayout* layout)
	{
		if (m_children.count(layout)) return;

		m_vector.push_back(layout);
		bind (layout);

		update(WidgetPropertySize, 0);
	}

	bool AbstractLayout::remove (Drawable* object)
	{
		if (!m_children.count(object)) return false;

		m_children.erase(object);

		unbind(object);

		std::vector<Drawable*>::iterator it;
		for(it = m_vector.begin(); it != m_vector.end(); it++)
		{
			if (*it == object) {
				it = m_vector.erase(it);
			}
		}

		update(WidgetPropertySize, 0);

		return true;
	}

	bool AbstractLayout::erase (Drawable* object)
	{
		if (!m_children.count(object)) return false;

		m_children.erase(object);

		unbind(object);

		std::vector<Drawable*>::iterator it;
		for(it = m_vector.begin(); it != m_vector.end(); it++)
		{
			if (*it == object) {
				it = m_vector.erase(it);
			}
		}

		delete object;

		update(WidgetPropertySize, 0);

		return true;
	}

	void AbstractLayout::set_margin (const Margin& margin)
	{
		Margin new_value = margin;

		if(update(WidgetPropertyMargin, &new_value)) m_margin = new_value;
	}

	void AbstractLayout::set_margin (int left, int right, int top, int bottom)
	{
		Margin new_value (left, right, top, bottom);

		if(update(WidgetPropertyMargin, &new_value)) m_margin = new_value;
	}



} /* namespace BlendInt */

