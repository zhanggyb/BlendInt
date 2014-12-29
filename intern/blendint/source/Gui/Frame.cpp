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
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Frame::Frame ()
	: AbstractFrame()
	{
		set_size(500, 400);
	}

	Frame::~Frame()
	{
	}

	AbstractView* Frame::GetFocusedView () const
	{
		return nullptr;
	}

	bool Frame::PreDraw(Profile& profile)
	{
		return visiable();
	}

	ResponseType Frame::Draw (Profile& profile)
	{
		DrawSubViewsOnce(profile);

		return subs_count() ? Ignore : Finish;
	}

	void Frame::PostDraw(Profile& profile)
	{

	}

	void Frame::FocusEvent(bool focus)
	{
	}

	void Frame::MouseHoverInEvent(const MouseEvent& event)
	{
	}

	void Frame::MouseHoverOutEvent(const MouseEvent& event)
	{
	}

	ResponseType Frame::KeyPressEvent(const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Frame::MousePressEvent(const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Frame::MouseReleaseEvent(const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Frame::MouseMoveEvent(const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Frame::DispatchHoverEvent(const MouseEvent& event)
	{
		return Ignore;
	}

}
