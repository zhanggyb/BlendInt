/*
 * This file is part of BILO (Blender Interface Library).
 *
 * BILO (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>
#include <algorithm>
#include <BILO/HorizontalLayout.hpp>

#include <BILO/Interface.hpp>

namespace BILO {

	HorizontalLayout::HorizontalLayout(int align)
		: AbstractLayout()
	{
		set_alignment(align);
	}

	HorizontalLayout::HorizontalLayout(int align, Drawable* parent)
		: AbstractLayout(parent)
	{
		set_alignment(align);
	}

	HorizontalLayout::~HorizontalLayout()
	{

	}

	void HorizontalLayout::add_widget (Widget* widget)
	{
		if (m_children.count(widget)) return;

		m_list.push_back(widget);
		bind (widget);

		update(WidgetPropertySize, 0);
	}

	void HorizontalLayout::add_layout(AbstractLayout* layout)
	{
		if (m_children.count(layout)) return;

		m_list.push_back(layout);
		bind (layout);

		update(WidgetPropertySize, 0);
	}

	bool HorizontalLayout::remove (Drawable* object)
	{
		if (!m_children.count(object)) return false;

		m_children.erase(object);

		unbind(object);

		update(WidgetPropertySize, 0);

		return true;
	}

	bool HorizontalLayout::erase (Drawable* object)
	{
		if (!m_children.count(object)) return false;

		m_children.erase(object);

		unbind(object);

		delete object;

		update(WidgetPropertySize, 0);

		return true;
	}

	bool HorizontalLayout::update (int type, const void* property)
	{
		const Point* new_pos = &m_pos;
		if (type == WidgetPropertyPosition)
			new_pos = static_cast<const Point*>(property);

		//if (property == WidgetPropertySize) {
		unsigned int total_width = 0;
		unsigned int total_height = 0;
		unsigned int max_widget_height = 0;

		std::list<Drawable*>::const_iterator it;
		Drawable* child = 0;
		total_width = m_margin.left();
		for (it = m_list.begin(); it != m_list.end(); it++) {
			child = *it;
			set_pos_priv(child, new_pos->x() + total_width,
			        new_pos->y() + m_margin.bottom());
			total_width = total_width + child->size().width();
			total_height = std::max(total_height,
			        m_margin.top() + child->size().height() + m_margin.bottom());
			max_widget_height = std::max(max_widget_height,
			        child->size().height());

		}
		total_width += m_margin.right();

		for (it = m_list.begin(); it != m_list.end(); it++) {
			child = *it;

			if (m_alignment & AlignTop) {
				set_pos_priv(child, child->pos().x(),
				        new_pos->y()
				                + (total_height
				                        - (m_margin.top()
				                                + child->size().height())));
			} else if (m_alignment & AlignBottom) {
				set_pos_priv(child, child->pos().x(),
				        new_pos->y()
				                + m_margin.bottom());
			} else if (m_alignment & AlignHorizontalCenter) {
				set_pos_priv(child, child->pos().x(),
				        new_pos->y() + m_margin.bottom()
				                + (max_widget_height - child->size().height())
				                        / 2 );
			}

		}

		m_size.set_width(total_width);
		m_size.set_height(total_height);
		//}

		return true;
	}

	void HorizontalLayout::render ()
	{
		list<Drawable*>::const_iterator it;
		for (it = m_list.begin(); it != m_list.end(); it++) {
				Interface::instance()->dispatch_render_event(*it);
		}

#ifdef DEBUG
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(m_pos.x(),
					 m_pos.y(),
					 z());
		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
		glLineStipple(1, 0xAAAA);
		glBegin(GL_LINE_LOOP);
			glVertex2i(0, 0);
			glVertex2i(m_size.width(), 0);
			glVertex2i(m_size.width(), m_size.height());
			glVertex2i(0, m_size.height());
		glEnd();

		glDisable(GL_LINE_STIPPLE);

		glPopMatrix();
#endif
	}

	void HorizontalLayout::press_key (KeyEvent* event)
	{
		list<Drawable*>::iterator it;
		for(it = m_list.begin(); it != m_list.end(); it++)
		{
			Interface::instance()->dispatch_key_press_event(*it, event);
		}
	}

	void HorizontalLayout::press_context_menu (ContextMenuEvent* event)
	{
	}

	void HorizontalLayout::release_context_menu (ContextMenuEvent* event)
	{
	}

	void HorizontalLayout::press_mouse (MouseEvent* event)
	{
		list<Drawable*>::iterator it;
		for(it = m_list.begin(); it != m_list.end(); it++)
		{
			Interface::instance()->dispatch_mouse_press_event(*it, event);
		}
	}

	void HorizontalLayout::release_mouse (MouseEvent* event)
	{
		list<Drawable*>::iterator it;
		for(it = m_list.begin(); it != m_list.end(); it++)
		{
			Interface::instance()->dispatch_mouse_release_event(*it, event);
		}
	}

	void HorizontalLayout::move_mouse (MouseEvent* event)
	{
		list<Drawable*>::iterator it;
		for(it = m_list.begin(); it != m_list.end(); it++)
		{
			Interface::instance()->dispatch_mouse_move_event(*it, event);
		}
	}

}

