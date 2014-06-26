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

#ifndef _BLENDINT_GUI_CONTEXTLAYER_HPP_
#define _BLENDINT_GUI_CONTEXTLAYER_HPP_

#include <set>
#include <deque>

#include <BlendInt/Gui/AbstractWidget.hpp>

namespace BlendInt {

	class Context;

	/**
	 * @brief A set stores widgets, used in ContextLayer and updated in Context only
	 */
	class WidgetSet: public Object
	{
	public:

		WidgetSet ();

		virtual ~WidgetSet ();

	private:

		friend class Context;

		std::set<AbstractWidget*> m_widgets;
	};

	class WidgetDeque: public Object
	{
	public:

		WidgetDeque ();

		virtual ~WidgetDeque ();

	private:

		friend class Context;

		std::deque<AbstractWidget*> m_widgets;
	};

	class ContextLayer
	{
	public:

		ContextLayer ();

		~ContextLayer ();

		ContextLayer& operator = (const ContextLayer& orig);

	private:

		friend class Context;

		/** If refresh this layer */
		bool m_refresh;

		/** If the rescan the tail of hover_widgets
		 */
		//bool m_hover_list_valid;

		/** A set to store sub widgets in this layer */
		RefPtr<WidgetSet> m_widget_set;

		/** The OpenGL Texture as a buffer for display */
		RefPtr<GLTexture2D> m_texture_buffer;

		/**
		 * @brief A widget list under current cursor position.
		 *
		 * Used for AbstractWidget::CursorEnterEvent
		 */
		RefPtr<WidgetDeque> m_hover_list;
	};

}

#endif /* _BLENDINT_GUI_CONTEXTLAYER_HPP_ */
