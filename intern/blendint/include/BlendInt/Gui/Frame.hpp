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

#ifndef _BLENDINT_GUI_SCREEN_HPP_
#define _BLENDINT_GUI_SCREEN_HPP_

#include <BlendInt/Gui/AbstractFrame.hpp>
#include <BlendInt/Gui/AbstractLayout.hpp>

#include <BlendInt/OpenGL/GLBuffer.hpp>

namespace BlendInt {

	class Frame: public AbstractFrame
	{
	public:

		Frame ();

		Frame (int width, int height);

		virtual ~Frame ();

		virtual AbstractView* GetFocusedView () const;

	protected:

		virtual bool PreDraw (const Context* context);

		virtual ResponseType Draw (const Context* context);

		virtual void PostDraw (const Context* context);

		virtual void PerformFocusOn (const Context* context);

		virtual void PerformFocusOff (const Context* context);

		virtual void PerformHoverIn (const Context* context);

		virtual void PerformHoverOut (const Context* context);

		virtual ResponseType PerformKeyPress (const Context* context);

		virtual ResponseType PerformMousePress (const Context* context);

		virtual ResponseType PerformMouseRelease (const Context* context);

		virtual ResponseType PerformMouseMove (const Context* context);

		virtual ResponseType DispatchHoverEvent (const Context* context);

	};

}

 #endif	// _BLENDINT_GUI_SCREEN_HPP_
