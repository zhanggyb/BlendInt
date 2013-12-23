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

/**
 * @defgroup layouts Layouts
 *
 * @ingroup gui
 */

#include <vector>

#include <BlendInt/AbstractWidget.hpp>
#include <BlendInt/Frame.hpp>

#include <Cpp/Events.hpp>

namespace BlendInt {

	/**
	 * @brief Abstract layout class
	 *
	 * @ingroup layouts
	 */
	class AbstractLayout: public AbstractWidget
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractLayout);

	public:

		AbstractLayout ();

		AbstractLayout (AbstractWidget *parent);

		virtual ~AbstractLayout ();

		void Add (AbstractWidget* obj);

		/**
		 * @brief remove the object from layout
		 * @param widget
		 * @return
		 *
		 * @warning: after removing from layout, the drawable object will bind to nothing, it must be deleted manually
		 */
		bool Remove (AbstractWidget* object);

		/**
		 * @brief erase the object from layout
		 * @param widget
		 * @return
		 *
		 * Same as remove but will delete the child object
		 */
		bool Erase (AbstractWidget* object);

		int alignment () const {return m_alignment;}

		void set_alignment (int align) {m_alignment = align;}

		const Margin& margin() const {return m_margin;}

		void SetMargin (const Margin& margin);

		void SetMargin (int left, int right, int top, int bottom);

		void SetSpace (int space);

		int space () const {return m_space;}

		AbstractLayout* root_layout ();

	protected:

		void set_space (int space) {m_space = space;}

		virtual void initialize () {}

		virtual void Update (int type, const void* data);

		virtual void Render () = 0;

		virtual void AddItem (AbstractWidget* form) = 0;

		virtual void RemoveItem (AbstractWidget* object) = 0;

		std::vector<AbstractWidget*>& items ()
		{
			return m_items;
		}

	private:

		int m_alignment;

		int m_space;

		Margin m_margin;

		std::vector<AbstractWidget*> m_items;
	};

} /* namespace BIL */
#endif /* _BIL_ABSTRACTLAYOUT_H_ */
