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

		VerticalLayout(int align, AbstractForm* parent);

		virtual ~VerticalLayout ();

	protected:

		virtual void update (int property_type);

		virtual void render ();

		virtual void press_key (KeyEvent* event);

		virtual void press_context_menu (ContextMenuEvent* event);

		virtual void release_context_menu (ContextMenuEvent* event);

		virtual void press_mouse (MouseEvent* event);

		virtual void release_mouse (MouseEvent* event);

		virtual void move_mouse (MouseEvent* event);

		virtual void add_item (Widget* widget);

		virtual void add_item (AbstractLayout* layout);

		virtual void remove_item (AbstractForm* object);

	private:

		/**
		 * @brief scan, distribute and align the items
		 */
		void make_layout ();

		/**
		 * @brief distribute vertically with preferred size
		 */
		void distribute_with_preferred_size ();

		/**
		 * @brief distribute vertically with small size
		 */
		void distribute_with_small_size ();

		/**
		 * @brief distribute vertically with large size
		 */
		void distribute_with_large_size ();

		/**
		 * @brief align vertically with preferred size
		 */
		void align_with_preferred_size ();

		/**
		 * @brief align vertically
		 */
		void align ();

		/**
		 * @brief reset the height of unexpandable items
		 * @param[in] items
		 * @param[in] height
		 */
		void reset_height_of_fixed_items (std::set<AbstractForm*>* items, unsigned int height);

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

		void change_layout (const Size* size);

		void generate_default_layout ();

		Size get_minimal_size ();

		/**
		 * @brief align the objects in layout according to alignment
		 * @param width[in] the max width of area contains children (width - (left + right margin))
		 */
		void align_along_y (unsigned int width);

		/**
		 * @brief set of vertically expandable items
		 */
		std::set<AbstractForm*> m_expandable_items;

		/**
		 * @brief set of vertically fixed items (along y)
		 */
		std::set<AbstractForm*> m_fixed_items;
	};

}

#endif	// _BLENDINT_VERTICALLAYOUT_HPP_
