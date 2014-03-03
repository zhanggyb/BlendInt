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

#ifndef _BLENDINT_CONTAINER_HPP_
#define _BLENDINT_CONTAINER_HPP_

#include <set>

#include <BlendInt/Gui/AbstractWidget.hpp>
#include <BlendInt/Service/ContextManager.hpp>

namespace BlendInt {

	/**
	 * @brief Base class for widgets which contain other widgets
	 */
	class AbstractContainer: public AbstractWidget
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractContainer);

	public:

		AbstractContainer();

		virtual ~AbstractContainer ();

		const Margin& margin () const {return m_margin;}

		void SetMargin (const Margin& margin);

		void SetMargin (int left, int right, int top, int bottom);

		void Mount (AbstractWidget* widget);

		void Unmount (AbstractWidget* widget);

	private:

		friend class ContextManager;

		Margin m_margin;

		/**
		 * @brief Sub widgets which build a tree to accept render and device events
		 */
		std::set<AbstractWidget*> m_sub_widgets;
	};

}

#endif /* _BLENDINT_CONTAINER_HPP_ */
