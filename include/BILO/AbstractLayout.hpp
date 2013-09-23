/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BILO_ABSTRACTLAYOUT_H_
#define _BILO_ABSTRACTLAYOUT_H_

#include <list>

#include <BILO/Drawable.hpp>
#include <BILO/Widget.hpp>

namespace BILO {

	class AbstractLayout: public Drawable
	{
	public:

		AbstractLayout ();

		AbstractLayout (Drawable *parent);

		virtual ~AbstractLayout ();

		void add_widget (Widget* widget);

		void add_layout (AbstractLayout* layout);

		int alignment () const {return alignment_;}

		void set_alignment (int align) {alignment_ = align;}

	protected:

		virtual void press_key (KeyEvent* event);

		virtual void press_context_menu (ContextMenuEvent* event);

		virtual void release_context_menu (ContextMenuEvent* event);

		virtual void press_mouse (MouseEvent* event);

		virtual void release_mouse (MouseEvent* event);

		virtual void move_mouse (MouseEvent* event);

		virtual void render () = 0;

		void update (int property) {}

		int alignment_;

		std::list<Drawable*> m_list;

		DISALLOW_COPY_AND_ASSIGN(AbstractLayout);
	};

} /* namespace BIL */
#endif /* _BIL_ABSTRACTLAYOUT_H_ */
