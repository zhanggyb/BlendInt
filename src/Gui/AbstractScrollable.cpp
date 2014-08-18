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
	: AbstractWidget()
	{
		m_hbar.reset(new NativeScrollBar(Horizontal));
		m_vbar.reset(new NativeScrollBar(Vertical));

		events()->connect(m_hbar->slider_moved(), &m_hbar_moved, &Cpp::Event<int>::fire);
		events()->connect(m_vbar->slider_moved(), &m_vbar_moved, &Cpp::Event<int>::fire);
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
	        const RefPtr<NativeScrollBar>& scrollbar, const Profile& profile)
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
		int bh = m_hbar->visiable() ? m_hbar->size().height() : 0;	// height of the bottom hbar
		int rw = m_vbar->visiable() ? m_vbar->size().width() : 0;	// width of the right vbar

		if(m_hbar->visiable()) {
			m_hbar->SetPosition(left, bottom);
			m_hbar->Resize(width - rw, bh);
		} else {
			m_hbar->SetPosition(left, bottom);
			m_hbar->Resize(width - m_vbar->size().width(), m_hbar->size().height());
		}

		if(m_vbar->visiable()) {
			m_vbar->SetPosition(left + width - rw, bottom + bh);
			m_vbar->Resize (rw, height - bh);
		} else {
			m_vbar->SetPosition(left + width - m_vbar->size().width(), bottom + m_hbar->size().height());
			m_vbar->Resize (m_vbar->size().width(), height - m_hbar->size().height());
		}
	}

}
