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

	AbstractLayoutExt::AbstractLayoutExt (Context* context)
	: m_context(context)
	{
		m_events.reset(new Cpp::ConnectionScope);
#ifdef DEBUG
		if(!context) {
			DBG_PRINT_MSG("%s", "Critical Error: context MUST be assigned in layout");
		}
#endif
	}

	AbstractLayoutExt::~AbstractLayoutExt ()
	{
	}

	void AbstractLayoutExt::Add (AbstractWidget* widget)
	{
		if(AddLayoutItem(widget)) {
			m_events->connect(widget->destroyed(), this, &AbstractLayoutExt::OnItemDestroyed);
		}
	}

	void AbstractLayoutExt::Remove (AbstractWidget* widget)
	{
		if(RemoveLayoutItem(widget)) {
			widget->destroyed().disconnectOne(this, &AbstractLayoutExt::OnItemDestroyed);
		}
	}

	void AbstractLayoutExt::OnItemDestroyed (AbstractWidget* widget)
	{
		Remove(widget);
	}

	// ---------------------------------

	AbstractLayout::AbstractLayout ()
			: AbstractDequeContainer(),
			  m_alignment(0),
			  m_space(1)
	{
	}

	AbstractLayout::~AbstractLayout ()
	{
		//m_items.clear();
	}

	void AbstractLayout::Add (AbstractWidget* obj)
	{
//		if(subordinates()->count(obj)) return;

		//Size pre_preferred_size = preferred_size();

		AddItem (obj);

		//if(! (pre_preferred_size == preferred_size())) {
			// fire events
		//}

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

	ResponseType AbstractLayout::CursorEnterEvent (bool entered)
	{
		return IgnoreAndContinue;
	}

	ResponseType AbstractLayout::KeyPressEvent (const KeyEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType AbstractLayout::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType AbstractLayout::ContextMenuReleaseEvent (const ContextMenuEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType AbstractLayout::MousePressEvent(const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType AbstractLayout::MouseReleaseEvent(const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}

	ResponseType AbstractLayout::MouseMoveEvent(const MouseEvent& event)
	{
		return IgnoreAndContinue;
	}

	void AbstractLayout::SetSpace (int space)
	{
		if(m_space == space) return;

		UpdateRequest request(Predefined, LayoutPropertySpace, &space);

		if(UpdateTest(request)) {
			Update(request);
			m_space = space;
		}
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
