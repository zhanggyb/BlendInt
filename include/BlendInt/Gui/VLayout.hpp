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

#include <list>
#include <set>

#include <BlendInt/Gui/AbstractLayout.hpp>
#include <BlendInt/Types.hpp>

namespace BlendInt {

	class VLayout: public AbstractLayout
	{
		DISALLOW_COPY_AND_ASSIGN(VLayout);

	public:

		VLayout(int align = AlignVerticalCenter);

		virtual ~VLayout ();

	protected:

		virtual bool Update (const UpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual void AddItem (AbstractWidget* object);

		virtual void RemoveItem (AbstractWidget* object);

	private:

		void MakeLayout (const Size* size, const Margin* margin, int space);

		/**
		 * @brief distribute vertically with preferred size
		 */
		void DistributeWithPreferredHeight (const Size* size, const Margin* margin, int space);

		/**
		 * @brief distribute vertically with small size
		 */
		void DistributeWithSmallHeight (const Size* size, const Margin* margin, int space);

		/**
		 * @brief distribute vertically with large size
		 */
		void DistributeWithLargeHeight (const Size* size, const Margin* margin, int space);

		void Distribute (int space, int start = 0);

		/**
		 * @brief align vertically
		 */
		void Align (const Size* size, const Margin* margin);

		unsigned int AdjustExpandableHeight (std::list<AbstractWidget*>* item_list_p, unsigned int height_plus);

		unsigned int AdjustMinimalHeight (std::list<AbstractWidget*>* item_list_p, unsigned int height_plus);

		/**
		 * @brief calculate and return the minimal height of the expandable items
		 * @return
		 */
		unsigned int GetAllMinimalExpandableHeight ();

		unsigned int GetAllMaximalExpandableHeight ();

		/**
		 * @brief calculate and return the height of fixed items
		 * @return
		 */
		unsigned int GetAllFixedHeight ();

		/**
		 * @brief scan the children and get the total size hint
		 * @param count_margin if count margin
		 * @param count_space if count space
		 * @param size the current size of this layout
		 * @param min the minimal size of this layout
		 * @param prefer the preferred size of this layout
		 */
		void GetSizeHint (bool count_margin,
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

		//std::vector<AbstractWidget*> m_items;
		std::vector<AbstractWidgetPtr> m_items;
	};

}

#endif	// _BLENDINT_VERTICALLAYOUT_HPP_
