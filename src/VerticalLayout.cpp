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
#include <BILO/VerticalLayout.hpp>

#include <BILO/Interface.hpp>

namespace BILO {

	VerticalLayout::VerticalLayout (int align)
			: AbstractLayout()
	{
		set_alignment(align);
	}

	VerticalLayout::VerticalLayout (int align, Drawable* parent)
			: AbstractLayout(parent)
	{
		set_alignment(align);
	}

	VerticalLayout::~VerticalLayout ()
	{

	}

	bool VerticalLayout::update (int type, const void* property)
	{
		const Point* new_pos = &m_pos;
		if (type == WidgetPropertyPosition)
			new_pos = static_cast<const Point*>(property);

		unsigned int total_width = 0;
		unsigned int total_height = 0;
		unsigned int max_widget_width = 0;

		std::vector<Drawable*>::const_reverse_iterator rit;
		Drawable* child = 0;
		total_height = m_margin.bottom();
		for (rit = m_vector.rbegin(); rit != m_vector.rend(); rit++) {
			child = *rit;

			set_pos_priv(child, new_pos->x() + m_margin.left(),
			        new_pos->y() + total_height);
			total_width = std::max(total_width,
			        m_margin.left()
			                + child->size().width()
			                + m_margin.right());
			max_widget_width = std::max(max_widget_width,
			        child->size().width());
			total_height = total_height
			        + child->size().height();

		}
		total_height += m_margin.top();

		std::vector<Drawable*>::const_iterator it;
		for (it = m_vector.begin(); it != m_vector.end(); it++) {
			child = *it;

			if (m_alignment & AlignLeft) {
				set_pos_priv(child,
				        new_pos->x() + m_margin.left()
				                , child->pos().y());
			} else if (m_alignment & AlignRight) {
				set_pos_priv(child,
				        new_pos->x()
				                + (total_width
				                        - (m_margin.right()
				                                + child->size().width()
				                              )),
				        child->pos().y());
			} else if (m_alignment & AlignVerticalCenter) {
				set_pos_priv(child,
				        new_pos->x() + m_margin.left()
				                + (max_widget_width - child->size().width())
				                        / 2, child->pos().y());
			}

		}

		m_size.set_width(total_width);
		m_size.set_height(total_height);

		return true;
	}

	void VerticalLayout::render ()
	{

		std::vector<Drawable*>::const_iterator it;
		Drawable *item = 0;
		for (it = m_vector.begin(); it != m_vector.end(); it++) {
			item = *it;
			if (item) {
				Interface::instance()->dispatch_render_event(item);
			}
		}

#ifdef DEBUG
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(m_pos.x(), m_pos.y(), z());
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

	void VerticalLayout::press_key (KeyEvent* event)
	{

	}

	void VerticalLayout::press_context_menu (ContextMenuEvent* event)
	{
	}

	void VerticalLayout::release_context_menu (ContextMenuEvent* event)
	{
	}

	void VerticalLayout::press_mouse (MouseEvent* event)
	{
		std::vector<Drawable*>::iterator it;
		for(it = m_vector.begin(); it != m_vector.end(); it++)
		{
			Interface::instance()->dispatch_mouse_press_event(*it, event);
		}
	}

	void VerticalLayout::release_mouse (MouseEvent* event)
	{
		std::vector<Drawable*>::iterator it;
		for(it = m_vector.begin(); it != m_vector.end(); it++)
		{
			Interface::instance()->dispatch_mouse_release_event(*it, event);
		}
	}

	void VerticalLayout::move_mouse (MouseEvent* event)
	{
		std::vector<Drawable*>::iterator it;
		for(it = m_vector.begin(); it != m_vector.end(); it++)
		{
			Interface::instance()->dispatch_mouse_move_event(*it, event);
		}
	}

}
