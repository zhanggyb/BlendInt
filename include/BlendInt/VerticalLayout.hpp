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

#ifndef _BLENDINT_VERTICALLAYOUT_HPP_
#define _BLENDINT_VERTICALLAYOUT_HPP_

#include <BlendInt/AbstractLayout.hpp>
#include <BlendInt/Types.hpp>

namespace BlendInt {

	class VerticalLayout: public AbstractLayout
	{
		DISALLOW_COPY_AND_ASSIGN(VerticalLayout);

	public:

		VerticalLayout(int align = AlignVerticalCenter);

		VerticalLayout(int align, AbstractWidget* parent);

		virtual ~VerticalLayout ();

	protected:

		virtual void Update (int type, const void* data);

		virtual void Render ();

		virtual void KeyPressEvent (KeyEvent* event);

		virtual void ContextMenuPressEvent (ContextMenuEvent* event);

		virtual void ContextMenuReleaseEvent (ContextMenuEvent* event);

		virtual void MousePressEvent (MouseEvent* event);

		virtual void MouseReleaseEvent (MouseEvent* event);

		virtual void MouseMoveEvent (MouseEvent* event);

		virtual void add_item (AbstractWidget* object);

		virtual void remove_item (AbstractWidget* object);

	private:

		/**
		 * @brief scan, distribute and align the items
		 */
		void make_layout ();

		/**
		 * @brief scan, distribute and align the items
		 */
		void make_layout (const Size* size);

		/**
		 * @brief distribute vertically with preferred size
		 */
		void distribute_with_preferred_height ();

		/**
		 * @brief distribute vertically with preferred size
		 */
		void distribute_with_preferred_height (const Size* size);

		/**
		 * @brief distribute vertically with small size
		 */
		void distribute_with_small_height ();

		/**
		 * @brief distribute vertically with small size
		 */
		void distribute_with_small_height (const Size* size);

		/**
		 * @brief distribute vertically with large size
		 */
		void distribute_with_large_height ();

		/**
		 * @brief distribute vertically with large size
		 */
		void distribute_with_large_height (const Size* size);

		/**
		 * @brief align vertically
		 */
		void align ();

		/**
		 * @brief align vertically
		 */
		void align (const Size* size);

		/**
		 * @brief reset the height of unexpandable items
		 * @param[in] items
		 * @param[in] height
		 */
		void reset_height_of_fixed_items (std::set<AbstractWidget*>* items, unsigned int height);

		/**
		 * @brief calculate and return the minimal height of the expandable items
		 * @return
		 */
		unsigned int minimal_expandable_height ();

		/**
		 * @brief calculate and return the height of fixed items
		 * @return
		 */
		unsigned int fixed_height ();

		/**
		 * @brief scan the children and get the total size hint
		 * @param count_margin if count margin
		 * @param count_space if count space
		 * @param size the current size of this layout
		 * @param min the minimal size of this layout
		 * @param prefer the preferred size of this layout
		 */
		void get_size_hint (bool count_margin,
				bool count_space,
				Size* size,
				Size* min,
				Size* prefer);

		/**
		 * @brief set of vertically expandable items
		 */
		std::set<AbstractWidget*> m_expandable_items;

		/**
		 * @brief set of vertically fixed items (along y)
		 */
		std::set<AbstractWidget*> m_fixed_items;
	};

}

#endif	// _BLENDINT_VERTICALLAYOUT_HPP_
