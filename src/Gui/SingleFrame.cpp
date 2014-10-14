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

#include <BlendInt/Gui/SingleFrame.hpp>

namespace BlendInt {

	SingleFrame::SingleFrame()
	: Frame()
	{

	}

	SingleFrame::~SingleFrame()
	{

	}

	void SingleFrame::Setup(Widget* widget)
	{
		if(widget == 0) return;

		if(widget->parent() == this) return;

		if(subs_count() > 0) ClearSubWidgets();

		Resize(widget->size());

		if(PushBackSubWidget(widget)) {
			assert(subs_count() == 1);
			FillSingleWidget(0, 0, 0, size().width(), size().height());
		}
	}

	void SingleFrame::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			FillSingleWidget(0, 0, 0, request.size()->width(), request.size()->height());
		}

		Frame::PerformSizeUpdate(request);
	}

}
