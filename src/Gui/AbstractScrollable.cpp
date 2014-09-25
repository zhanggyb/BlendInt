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

#include <BlendInt/Gui/AbstractScrollable.hpp>

namespace BlendInt {

	AbstractScrollable::AbstractScrollable()
	: Widget()
	{
		hbar_.reset(new NativeScrollBar(Horizontal));
		vbar_.reset(new NativeScrollBar(Vertical));

		events()->connect(hbar_->slider_moved(), &hbar_moved_, &Cpp::Event<int>::fire);
		events()->connect(vbar_->slider_moved(), &vbar_moved_, &Cpp::Event<int>::fire);
	}

	AbstractScrollable::~AbstractScrollable ()
	{
	}

	ResponseType AbstractScrollable::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType AbstractScrollable::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType AbstractScrollable::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType AbstractScrollable::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType AbstractScrollable::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType AbstractScrollable::MousePressEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType AbstractScrollable::MouseReleaseEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType AbstractScrollable::MouseMoveEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType AbstractScrollable::DispatchDrawEvent (
	        const RefPtr<NativeScrollBar>& scrollbar, Profile& profile)
	{
		if(scrollbar->visiable()) {
			return scrollbar->Draw(profile);
		} else {
			return Ignore;
		}
	}

	ResponseType AbstractScrollable::DispatchMousePressEvent (
	        const RefPtr<NativeScrollBar>& scrollbar, const MouseEvent& event)
	{
		if(scrollbar->visiable()) {
			return scrollbar->MousePressEvent(event);
		} else {
			return Ignore;
		}
	}

	ResponseType AbstractScrollable::DispatchMouseReleaseEvent (
	        const RefPtr<NativeScrollBar>& scrollbar, const MouseEvent& event)
	{
		if(scrollbar->visiable()) {
			return scrollbar->MouseReleaseEvent(event);
		} else {
			return Ignore;
		}
	}

	ResponseType AbstractScrollable::DispatchMouseMoveEvent (
	        const RefPtr<NativeScrollBar>& scrollbar, const MouseEvent& event)
	{
		if(scrollbar->visiable()) {
			return scrollbar->MouseMoveEvent(event);
		} else {
			return Ignore;
		}
	}

	void AbstractScrollable::AdjustScrollBarGeometries (int left, int bottom,
	        int width, int height)
	{
		int bh = hbar_->visiable() ? hbar_->size().height() : 0;	// height of the bottom hbar
		int rw = vbar_->visiable() ? vbar_->size().width() : 0;	// width of the right vbar

		if(hbar_->visiable()) {
			hbar_->SetPosition(left, bottom);
			hbar_->Resize(width - rw, bh);
		} else {
			hbar_->SetPosition(left, bottom);
			hbar_->Resize(width - vbar_->size().width(), hbar_->size().height());
		}

		if(vbar_->visiable()) {
			vbar_->SetPosition(left + width - rw, bottom + bh);
			vbar_->Resize (rw, height - bh);
		} else {
			vbar_->SetPosition(left + width - vbar_->size().width(), bottom + hbar_->size().height());
			vbar_->Resize (vbar_->size().width(), height - hbar_->size().height());
		}
	}

}
