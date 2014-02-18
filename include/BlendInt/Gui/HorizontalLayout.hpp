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

#include <list>

#include <BlendInt/Gui/AbstractLayout.hpp>

namespace BlendInt {

	class HorizontalLayout: public AbstractLayout
	{
		DISALLOW_COPY_AND_ASSIGN(HorizontalLayout);

	public:

		HorizontalLayout(int align = AlignHorizontalCenter);

		HorizontalLayout(int align, AbstractWidget* parent);

		virtual ~HorizontalLayout ();

	protected:

		virtual void Update (int type, const void* data);

		virtual void Draw ();

		/**
		 * @brief Add a Widget object into this layout
		 * @param object
		 *
		 * Change and lock the object geometry, and put this widget into HorizontalLayout
		 *
		 */
		virtual void AddItem (AbstractWidget* object);

		virtual void RemoveItem (AbstractWidget* object);

	private:

		/**
		 * @brief scan, distribute and align the items
		 */
		void MakeLayout (const Size* size, const Margin* margin, int space);

		/**
		 * @brief distribute horizontally with preferred size
		 */
		void DistributeWithPreferredWidth (const Margin* margin, int space);

		/**
		 * @brief distribute horizontally with small size
		 */
		void DistributeWithSmallWidth (const Size* size, const Margin* margin, int space);

		/**
		 * @brief distribute horizontally with large size
		 */
		void DistributeWithLargeWidth (const Size* size, const Margin* margin, int space);

		void Distribute (int space, int start = 0);

		/**
		 * @brief align horizontally
		 */
		void Align (const Size* size, const Margin* margin);

		unsigned int AdjustExpandableWidth (std::list<AbstractWidget*>* item_list_p, unsigned int width_plus);

		unsigned int AdjustMinimalWidth (std::list<AbstractWidget*>* item_list_p, unsigned int width_plus);

		/**
		 * @brief calculate and return the minimal width of the expandable items
		 * @return
		 */
		unsigned int GetAllMinimalExpandableWidth ();

		unsigned int GetAllMaximalExpandableWidth ();

		/**
		 * @brief calculate and return the width of fixed items
		 * @return
		 */
		unsigned int GetAllFixedWidth ();

		/**
		 * @brief scan the children and get the total size hint
		 * @param[in] if count margin
		 * @param[in] if count space
		 * @param[out] size the layout size
		 * @param[out] min the layout minimal size
		 * @param[out] prefer the layout preferred size
		 */
		void GetSizeHint (bool count_margin, bool count_space, Size* size, Size* min, Size* prefer);

		/**
		 * @brief set of horizontally expandable items
		 */
		std::set<AbstractWidget*> m_expandable_items;

		/**
		 * @brief set of horizontally fixed items (along x)
		 */
		std::set<AbstractWidget*> m_fixed_items;

		std::vector<AbstractWidget*> m_items;
	};

}

#endif	// _BIL_HORIZONTALLAYOUT_H_
