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

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	Frame::Frame ()
	: AbstractFrame()
	{
	}

	Frame::Frame (int width, int height)
	: AbstractFrame(width, height)
	{
	}

	Frame::~Frame()
	{
	}

	AbstractView* Frame::GetFocusedView () const
	{
		return nullptr;
	}

	bool Frame::PreDraw(const Context* context)
	{
		return visiable();
	}

	ResponseType Frame::Draw (const Context* context)
	{
		DrawSubViewsOnce(context);

		return subs_count() ? Ignore : Finish;
	}

	void Frame::PostDraw(const Context* context)
	{

	}

	void Frame::PerformFocusOn(const Context* context)
	{
	}

	void Frame::PerformFocusOff (const Context* context)
	{

	}

	void Frame::PerformHoverIn(const Context* context)
	{
	}

	void Frame::PerformHoverOut(const Context* context)
	{
	}

	ResponseType Frame::PerformKeyPress(const Context* context)
	{
		return Ignore;
	}

	ResponseType Frame::PerformMousePress(const Context* context)
	{
		return Ignore;
	}

	ResponseType Frame::PerformMouseRelease(const Context* context)
	{
		return Ignore;
	}

	ResponseType Frame::PerformMouseMove(const Context* context)
	{
		return Ignore;
	}

	ResponseType Frame::DispatchHoverEvent(const Context* context)
	{
		return Ignore;
	}

}
