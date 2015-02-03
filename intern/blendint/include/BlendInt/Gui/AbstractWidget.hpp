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
 * License along with BlendInt.	 If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#pragma once

#include <glm/glm.hpp>
#include <boost/smart_ptr.hpp>

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/Gui/AbstractView.hpp>

namespace BlendInt {

	/**
	 * @brief A Normal widget
	 *
	 * @ingroup widgets
	 */
	class AbstractWidget: public AbstractView
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractWidget);

	public:

		AbstractWidget ();

		AbstractWidget (int width, int height);

		virtual ~AbstractWidget();

		Cpp::EventRef<AbstractWidget*> destroyed ()
		{
			return *destroyed_;
		}

	protected:

		virtual bool PreDraw (AbstractWindow* context);

		// virtual ResponseType Draw (Profile& profile);

		virtual void PostDraw (AbstractWindow* context);

		virtual void PerformFocusOn (AbstractWindow* context);

		virtual void PerformFocusOff (AbstractWindow* context);

		virtual void PerformHoverIn (AbstractWindow* context);

		virtual void PerformHoverOut (AbstractWindow* context);

		virtual ResponseType PerformKeyPress (AbstractWindow* context);

		virtual ResponseType PerformContextMenuPress (AbstractWindow* context);

		virtual ResponseType PerformContextMenuRelease (AbstractWindow* context);

		virtual ResponseType PerformMousePress (AbstractWindow* context);

		virtual ResponseType PerformMouseRelease (AbstractWindow* context);

		virtual ResponseType PerformMouseMove (AbstractWindow* context);

		const boost::scoped_ptr<Cpp::ConnectionScope>& events()
		{
			if(!events_) {
				events_.reset(new Cpp::ConnectionScope);
			}

			return events_;
		}

		static bool RenderSubWidgetsToTexture (
			AbstractWidget* widget,
			AbstractWindow* context,
			GLTexture2D* texture);

	private:

		boost::scoped_ptr<Cpp::ConnectionScope> events_;

		boost::scoped_ptr<Cpp::Event<AbstractWidget*> > destroyed_;

	};

}
