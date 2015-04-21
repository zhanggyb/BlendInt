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

#include <blendint/opengl/opengl.hpp>

#include <blendint/gui/separator.hpp>

namespace BlendInt {

	Separator::Separator (bool expandx, bool expandy)
	: AbstractWidget(),
	  expand_x_(expandx),
	  expand_y_(expandy),
	  preferred_size_(10, 10)
	{
		set_size(10, 10);
	}

	Separator::~Separator ()
	{
	}

	Size Separator::GetPreferredSize() const
	{
		return preferred_size_;
	}

	void Separator::SetExpandX (bool expand)
	{
		expand_x_ = expand;
		// TODO: call superview widget to update layout
	}

	void Separator::SetExpandY (bool expand)
	{
		expand_y_ = expand;
		// TODO: call superview widget to update layout
	}

	void Separator::SetExpand (bool expand_x, bool expand_y)
	{
		expand_x_ = expand_x;
		expand_y_ = expand_y;
		// TODO: call superview widget to update layout
	}

	bool Separator::IsExpandX() const
	{
		return expand_x_;
	}

	bool Separator::IsExpandY() const
	{
		return expand_y_;
	}

	Response Separator::PerformMousePress (AbstractWindow* context)
	{
		return Ignore;
	}

	Response Separator::PerformMouseRelease (AbstractWindow* context)
	{
		return Ignore;
	}

	bool Separator::PreDraw(AbstractWindow* context)
	{
		return true;
	}

	Response Separator::Draw(AbstractWindow* context)
	{
		return Ignore;
	}

	void Separator::PostDraw(AbstractWindow* context)
	{
		return;
	}

}
