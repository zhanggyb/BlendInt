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

#ifndef _BLENDINT_GUI_SECTION_HPP_
#define _BLENDINT_GUI_SECTION_HPP_

#include <set>

#include <BlendInt/OpenGL/ScissorStatus.hpp>
#include <BlendInt/Gui/AbstractContainer.hpp>

namespace BlendInt {

	/**
	 * @brief A special container used in Context as a layer
	 */
	class Section: public AbstractContainer
	{
	public:

		enum Mode {
			Normal,
			Modal,
			Popup
		};

		friend class Context;

		Section ();

		virtual ~Section ();

		void PushFront (AbstractWidget* widget);

		void PushBack (AbstractWidget* widget);

		/**
		 * @brief Remove sub widget
		 *
		 * If the widget is last one in this section, call this funciton will delete this.
		 */
		void Remove (AbstractWidget* widget);

		/**
		 * @brief Always return true
		 */
		bool Contain (const Point& point) const;

		AbstractWidget* last_hover_widget () const
		{
			return last_hover_widget_;
		}

		void set_mode (Mode mode)
		{
			mode_ = mode;
		}

		static Section* GetSection (AbstractWidget* widget);

		static void RenderToTexture (AbstractWidget* widget, GLTexture2D* texture);

		static void RenderToFile (AbstractWidget* widget, const char* filename);

		static void DispatchDrawEvent (AbstractWidget* widget, Profile& profile, ScissorStatus& scissor);

	protected:

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual bool PositionUpdateTest (const PositionUpdateRequest& request);

		virtual bool RoundTypeUpdateTest (const RoundTypeUpdateRequest& request);

		virtual bool RoundRadiusUpdateTest (const RoundRadiusUpdateRequest& request);

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request);

		virtual void PerformRoundRadiusUpdate (const RoundRadiusUpdateRequest& request);

		virtual ResponseType Draw (Profile& profile);

		virtual ResponseType FocusEvent (bool focus);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

	private:

		void DispatchDrawEvent (AbstractWidget* widget, Profile& profile);

		ResponseType DispatchMousePressEvent (AbstractWidget* widget, const MouseEvent& event);

		ResponseType DispatchMouseReleaseEvent (AbstractWidget* widget, const MouseEvent& event);

		bool CheckAndUpdateHoverWidget (const MouseEvent& event);

		void UpdateHoverWidgetSubs (const MouseEvent& event);

		void OnSubWidgetDestroyedInSection (AbstractWidget* widget);

		void OnHoverWidgetDestroyed (AbstractWidget* widget);

		void ClearHoverWidgets ();

		AbstractWidget* m_focused_widget;

		AbstractWidget* last_hover_widget_;

		ScissorStatus m_scissor_status;

		Mode mode_;

		static AbstractWidget* iterator_ptr;
	};
}

#endif /* _BLENDINT_GUI_SECTION_HPP_ */
