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

#include <BlendInt/Gui/BinLayout.hpp>

#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	BinLayout::BinLayout()
	: Container()
	{
	}

	BinLayout::~BinLayout()
	{
	}

	bool BinLayout::Setup (AbstractWidget* widget)
	{
		bool ret = false;

		if(!widget) return false;

		if(widget->container() == this) return true;

		if(subs_count() > 0) {
			ClearSubWidgets();
		}

		if (PushBackSubWidget(widget)) {
			FillSingleWidget(0, size(), margin());
			ret = true;
		}

		return ret;
	}

	bool BinLayout::Remove (AbstractWidget* widget)
	{
		return RemoveSubWidget(widget);
	}

	bool BinLayout::IsExpandX () const
	{
		if(subs_count() == 0) {
			return false;
		} else {
			assert(subs_count() == 1);	// DEBUG
			return first_sub_widget()->IsExpandX();
		}
	}

	bool BinLayout::IsExpandY () const
	{
		if(subs_count() == 0) {
			return false;
		} else {
			assert(subs_count() == 1);	// DEBUG
			return first_sub_widget()->IsExpandY();
		}
	}

	Size BinLayout::GetPreferredSize () const
	{
		Size prefer(400, 300);

		const AbstractWidget* widget = first_sub_widget();

		if(widget) {
			prefer = widget->GetPreferredSize();

			prefer.add_width(margin().hsum());
			prefer.add_height(margin().vsum());
		}

		return prefer;
	}

	void BinLayout::PerformMarginUpdate (const Margin& request)
	{
		set_margin(request);

		if(subs_count()) {
			assert(subs_count() == 1);
			FillSingleWidget(0, size(), request);
		}
	}

	void BinLayout::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			set_size(*request.size());

			if (subs_count()) {
				assert(subs_count() == 1);
				FillSingleWidget(0, *request.size(), margin());
			}
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	ResponseType BinLayout::Draw (Profile& profile)
	{
		return Ignore;
	}

	ResponseType BinLayout::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType BinLayout::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType BinLayout::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType BinLayout::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType BinLayout::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType BinLayout::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType BinLayout::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

}
