/*
 * This file is part of BILO (Blender-like Interface Library in
 * OpenGL).
 *
 * BILO (Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BILO_ABSTRACTLAYOUT_HPP_
#define _BILO_ABSTRACTLAYOUT_HPP_

#include <list>

#include <BILO/Drawable.hpp>
#include <BILO/Widget.hpp>

namespace BILO {

	class AbstractLayout: public Drawable
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractLayout);

	public:

		AbstractLayout ();

		AbstractLayout (Drawable *parent);

		virtual ~AbstractLayout ();

		virtual void add_widget (Widget* widget) = 0;

		virtual void add_layout (AbstractLayout* layout) = 0;

		/**
		 * @brief remove the object from layout
		 * @param widget
		 * @return
		 *
		 * @warning: after removing from layout, the drawable object will bind to nothing, it must be deleted manually
		 */
		virtual bool remove (Drawable* object) = 0;

		/**
		 * @brief erase the object from layout
		 * @param widget
		 * @return
		 *
		 * Same as remove but will delete the child object
		 */
		virtual bool erase (Drawable* object) = 0;

		int alignment () const {return alignment_;}

		void set_alignment (int align) {alignment_ = align;}

	protected:


		virtual void render () = 0;

		void update (int property) {}

		int alignment_;
	};

} /* namespace BIL */
#endif /* _BIL_ABSTRACTLAYOUT_H_ */
