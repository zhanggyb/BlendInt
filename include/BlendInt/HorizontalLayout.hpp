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

#ifndef _BLENDINT_HORIZONTALLAYOUT_HPP_
#define _BLENDINT_HORIZONTALLAYOUT_HPP_

#include <BlendInt/AbstractLayout.hpp>

namespace BlendInt {

	class HorizontalLayout: public AbstractLayout
	{
		DISALLOW_COPY_AND_ASSIGN(HorizontalLayout);

	public:

		HorizontalLayout(int align = AlignHorizontalCenter);

		HorizontalLayout(int align, AbstractForm* parent);

		virtual ~HorizontalLayout ();

	protected:

		virtual void update (int type, const void* data);

		virtual void render ();

		virtual void press_key (KeyEvent* event);

		virtual void press_context_menu (ContextMenuEvent* event);

		virtual void release_context_menu (ContextMenuEvent* event);

		virtual void press_mouse (MouseEvent* event);

		virtual void release_mouse (MouseEvent* event);

		virtual void move_mouse (MouseEvent* event);

		virtual void add_item (Form* form);

		virtual void add_item (AbstractLayout* layout);

		virtual void remove_item (AbstractForm* object);

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
		 * @brief distribute horizontally with preferred size
		 */
		void distribute_with_preferred_width ();

		/**
		 * @brief distribute horizontally with small size
		 */
		void distribute_with_small_width ();

		/**
		 * @brief distribute horizontally with small size
		 */
		void distribute_with_small_width (const Size* size);

		/**
		 * @brief distribute horizontally with large size
		 */
		void distribute_with_large_width ();

		/**
		 * @brief distribute horizontally with large size
		 */
		void distribute_with_large_width (const Size* size);

		/**
		 * @brief align horizontally
		 */
		void align ();

		/**
		 * @brief align horizontally
		 */
		void align (const Size* size);

		/**
		 * @brief reset the width of unexpandable items
		 * @param[in] items
		 * @param[in] width
		 */
		void reset_width_of_fixed_items (std::set<AbstractForm*>* items, unsigned int width);

		/**
		 * @brief calculate and return the minimal width of the expandable items
		 * @return
		 */
		unsigned int minimal_expandable_width ();

		/**
		 * @brief calculate and return the width of fixed items
		 * @return
		 */
		unsigned int fixed_width ();

		/**
		 * @brief scan the children and get the total size hint
		 * @param[in] if count margin
		 * @param[in] if count space
		 * @param[out] size the layout size
		 * @param[out] min the layout minimal size
		 * @param[out] prefer the layout preferred size
		 */
		void get_size_hint (bool margin, bool space, Size* size, Size* min, Size* prefer);

		/**
		 * @brief set of horizontally expandable items
		 */
		std::set<AbstractForm*> m_expandable_items;

		/**
		 * @brief set of horizontally fixed items (along x)
		 */
		std::set<AbstractForm*> m_fixed_items;
	};

}

#endif	// _BIL_HORIZONTALLAYOUT_H_
