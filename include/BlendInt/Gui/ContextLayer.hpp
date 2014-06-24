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

#include <boost/smart_ptr.hpp>
#include <Cpp/Events.hpp>

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	/**
	 * @brief A set stores widgets, used in ContextLayer only
	 */
	class WidgetSet: public Object
	{
	public:

		WidgetSet ();

		virtual ~WidgetSet ();

		void Insert (AbstractWidget* widget);

		void Remove (AbstractWidget* widget);

		Cpp::EventRef<AbstractWidget*> widget_destroyed ()
		{
			return m_widget_destroyed;
		}

	private:

		std::set<AbstractWidget*> m_widgets;

		boost::scoped_ptr<Cpp::ConnectionScope> m_events;

		Cpp::Event<AbstractWidget*> m_widget_destroyed;
	};

	class ContextLayerExt
	{
	public:

		ContextLayerExt ();

		~ContextLayerExt ();

		ContextLayerExt& operator = (const ContextLayerExt& orig);

	private:

		/** If refresh this layer */
		bool refresh;

		/** A set to store sub widgets in this layer */
		RefPtr<WidgetSet> widgets;

		/** The OpenGL Texture as a buffer for display */
		RefPtr<GLTexture2D> tex_buf_ptr;
	};

}

#endif /* _BLENDINT_GUI_CONTEXTLAYER_HPP_ */
