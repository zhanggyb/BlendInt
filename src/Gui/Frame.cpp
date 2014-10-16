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

	Frame::Frame ()
	: AbstractFrame()
		//cursor_followed_widget_(0),
	  //custom_focused_widget_(false)
	{
		set_size(500, 400);
	}

	Frame::~Frame()
	{
	}

	/*
	void Frame::Setup(Widget* widget)
	{
		if(widget == 0) return;

		if(widget->parent() == this) return;

		if(subs_count() > 0) ClearSubWidgets();

		Resize(widget->size());

		if(PushBackSubWidget(widget)) {
			LayoutWidgets();
		}
	}
	*/

	bool Frame::IsExpandX() const
	{
		bool expand = false;

		for(AbstractWidget* p = first_child(); p; p = p->next()) {
			if(p->IsExpandX()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	bool Frame::IsExpandY() const
	{
		bool expand = false;

		for(AbstractWidget* p = first_child(); p; p = p->next()) {
			if(p->IsExpandY()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	Size Frame::GetPreferredSize() const
	{
		Size prefer;

		if(subs_count()) {
			int minx = 0;
			int miny = 0;
			int maxx = 0;
			int maxy = 0;

			for(AbstractWidget* p = first_child(); p; p = p->next()) {
				minx = std::min(p->position().x(), minx);
				miny = std::min(p->position().y(), miny);
				maxx = std::max(p->position().x() + p->size().width(), maxx);
				maxy = std::max(p->position().y() + p->size().height(), maxy);

				prefer.reset(maxx - minx, maxy - miny);
			}

		} else {
			prefer.reset(500, 400);
		}

		return prefer;
	}

	Widget* Frame::GetFocusedWidget() const
	{
		return 0;
	}

	Widget* Frame::GetHoveredWidget() const
	{
		return 0;
	}

	void Frame::DispatchHoverEvent(const MouseEvent& event)
	{
	}

	bool Frame::SizeUpdateTest(const SizeUpdateRequest& request)
	{
		if(request.source()->parent() == this) {
			return false;
		} else {
			return true;
		}
	}

	bool Frame::PositionUpdateTest(const PositionUpdateRequest& request)
	{
		if(request.source()->parent() == this) {
			return false;
		} else {
			return true;
		}
	}

	void Frame::PreDraw(Profile& profile)
	{
		assign_profile_frame(profile);
	}

	ResponseType Frame::Draw (Profile& profile)
	{
		return subs_count() ? Ignore : Accept;
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
		return subs_count() ? Ignore : Accept;
	}

	ResponseType Frame::MousePressEvent(const MouseEvent& event)
	{
		return subs_count() ? Ignore : Accept;
	}

	ResponseType Frame::MouseReleaseEvent(const MouseEvent& event)
	{
		return subs_count() ? Ignore : Accept;
	}

	ResponseType Frame::MouseMoveEvent(const MouseEvent& event)
	{
		return subs_count() ? Ignore : Accept;
	}

	/*
	void Frame::OnCursorFollowedWidgetDestroyed(Widget* widget)
	{
		assert(cursor_followed_widget_ == widget);

		DBG_PRINT_MSG("cursor followed widget %s destroyed", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &Frame::OnCursorFollowedWidgetDestroyed);

		cursor_followed_widget_ = 0;
	}
	*/

}
