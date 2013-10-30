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

#ifndef _BLENDINT_ABSTRACTLAYOUT_HPP_
#define _BLENDINT_ABSTRACTLAYOUT_HPP_

#include <vector>

#include <BlendInt/AbstractForm.hpp>
#include <BlendInt/Widget.hpp>

#include <Cpp/Events.hpp>

namespace BlendInt {

	/**
	 * Sizing mode enumeration
	 * Flow - the layout size is not restricted by the parent widget
	 * Fixed - the layout size is fixed when add/remove its child
	 */
	enum LayoutSizingMode
	{
		LayoutFlow = 0,//!< LayoutFlow
		LayoutFixed    //!< LayoutFixed
	};

	enum LayoutPropertyType
	{
		LayoutPropertyAlignment = FormPropertyLast + 1,
		LayoutPropertySpace,
		LayoutPropertyMargin,
		LayoutPropertyItem,
		LayoutPropertyLast = LayoutPropertyItem
	};

	class AbstractLayout: public AbstractForm
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractLayout);

	public:

		AbstractLayout ();

		AbstractLayout (AbstractForm *parent);

		virtual ~AbstractLayout ();

		void add (Widget* widget);

		void add (AbstractLayout* layout);

		/**
		 * @brief remove the object from layout
		 * @param widget
		 * @return
		 *
		 * @warning: after removing from layout, the drawable object will bind to nothing, it must be deleted manually
		 */
		bool remove (AbstractForm* object);

		/**
		 * @brief erase the object from layout
		 * @param widget
		 * @return
		 *
		 * Same as remove but will delete the child object
		 */
		bool erase (AbstractForm* object);

		int alignment () const {return m_alignment;}

		void set_alignment (int align) {m_alignment = align;}

		const Margin& margin() const {return m_margin;}

		void set_margin (const Margin& margin);

		void set_margin (int left, int right, int top, int bottom);

		void set_space (int space) {m_space = space;}

		int space () const {return m_space;}

		int sizing_mode () const {return m_sizing_mode;}

		void set_sizing_mode (LayoutSizingMode mode)
		{
			m_sizing_mode = mode;
		}

		AbstractLayout* root_layout ();

	protected:

		virtual void update (int property_type);

		virtual void render () = 0;

		virtual void add_item (Widget* widget) = 0;

		virtual void add_item (AbstractLayout* layout) = 0;

		virtual void remove_item (AbstractForm* object) = 0;

		std::vector<AbstractForm*>& items_ref ()
		{
			return m_items;
		}

	private:

		int m_alignment;

		int m_space;

		/**
		 * @brief the sizing mode: flow or fixed
		 *
		 * This property is mostly used when this layout is the root layout in a widget
		 * But the size can still be changes with resize()
		 */
		LayoutSizingMode m_sizing_mode;

		Margin m_margin;

		std::vector<AbstractForm*> m_items;
	};

} /* namespace BIL */
#endif /* _BIL_ABSTRACTLAYOUT_H_ */
