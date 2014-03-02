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

#include <BlendInt/Gui/AbstractLayout.hpp>

namespace BlendInt {

	AbstractLayout::AbstractLayout ()
			: AbstractContainer(),
			  m_alignment(0),
			  m_space(1)
	{
		set_preferred_size(margin().left() + margin().right(), margin().top() + margin().bottom());
		set_minimal_size(margin().left() + margin().right(), margin().top() + margin().bottom());
		set_size(preferred_size());
	}

	AbstractLayout::~AbstractLayout ()
	{
		//m_items.clear();
	}

	void AbstractLayout::Add (AbstractWidget* obj)
	{
//		if(subordinates()->count(obj)) return;

		Size pre_preferred_size = preferred_size();

		AddItem (obj);

		if(! (pre_preferred_size == preferred_size())) {
			// fire events
		}

		// if minimal size changed
		// fire events

	}

	bool AbstractLayout::Remove (AbstractWidget* object)
	{
//		if (!subordinates()->count(object)) return false;

		RemoveItem(object);

		return true;
	}

	bool AbstractLayout::Erase (AbstractWidget* object)
	{
//		if (!subordinates()->count(object)) return false;

		RemoveItem(object);

		delete object;

		return true;
	}

	/*
	bool AbstractLayout::Update(int type, const void* property)
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

	void AbstractLayout::CursorEnterEvent (bool entered)
	{

	}

	void AbstractLayout::KeyPressEvent (KeyEvent* event)
	{

	}

	void AbstractLayout::ContextMenuPressEvent (ContextMenuEvent* event)
	{

	}

	void AbstractLayout::ContextMenuReleaseEvent (ContextMenuEvent* event)
	{

	}

	void AbstractLayout::MousePressEvent(MouseEvent* event)
	{

	}

	void AbstractLayout::MouseReleaseEvent(MouseEvent* event)
	{

	}

	void AbstractLayout::MouseMoveEvent(MouseEvent* event)
	{

	}

	void AbstractLayout::SetMargin (const Margin& margin)
	{
		if(m_margin.equal(margin)) return;

		Update(LayoutPropertyMargin, &margin);

		m_margin = margin;
	}

	void AbstractLayout::SetMargin (int left, int right, int top, int bottom)
	{
		if(m_margin.equal(left, right, top, bottom)) return;

		Margin new_margin (left, right, top, bottom);

		Update (LayoutPropertyMargin, &new_margin);

		m_margin.set_left(left);
		m_margin.set_right(right);
		m_margin.set_top(top);
		m_margin.set_bottom(bottom);
	}

	void AbstractLayout::SetSpace (int space)
	{
		if(m_space == space) return;

		Update (LayoutPropertySpace, &space);

		m_space = space;
	}

	AbstractLayout* AbstractLayout::root_layout ()
	{
		AbstractLayout* root = 0;

//		if(locked()) {
//			root = dynamic_cast<AbstractLayout*>(parent().object.form);
//			while(root->locked()) {
//				root = dynamic_cast<AbstractLayout*>(root->parent().object.form);
//				if(!root) break;
//			}
//		}

		return root;
	}

} /* namespace BlendInt */
