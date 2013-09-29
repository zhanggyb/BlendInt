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

#include <vector>

#include <BILO/Drawable.hpp>
#include <BILO/Widget.hpp>

#include <Cpp/Events.hpp>

namespace BILO {

	class AbstractLayout: public Drawable
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractLayout);

	public:

		AbstractLayout ();

		AbstractLayout (Drawable *parent);

		virtual ~AbstractLayout ();

		void add_widget (Widget* widget);

		void add_layout (AbstractLayout* layout);

		/**
		 * @brief remove the object from layout
		 * @param widget
		 * @return
		 *
		 * @warning: after removing from layout, the drawable object will bind to nothing, it must be deleted manually
		 */
		bool remove (Drawable* object);

		/**
		 * @brief erase the object from layout
		 * @param widget
		 * @return
		 *
		 * Same as remove but will delete the child object
		 */
		bool erase (Drawable* object);

		int alignment () const {return m_alignment;}

		void set_alignment (int align) {m_alignment = align;}

		const Margin& margin() const {return m_margin;}

		void set_margin (const Margin& margin);

		void set_margin (int left, int right, int top, int bottom);

		void set_space (int space) {m_space = space;}

		int space () const {return m_space;}

	protected:

		virtual void render () = 0;

		virtual bool update (int type, const void* property) {return false;}

		int m_alignment;

		int m_space;

		Margin m_margin;

		std::vector<Drawable*> m_vector;

		Cpp::ConnectionScope m_events;

	};

} /* namespace BIL */
#endif /* _BIL_ABSTRACTLAYOUT_H_ */
