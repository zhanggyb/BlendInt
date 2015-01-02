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

#ifndef _BLENDINT_GUI_DIALOG_HPP_
#define _BLENDINT_GUI_DIALOG_HPP_

#include <BlendInt/OpenGL/GLBuffer.hpp>
#include <BlendInt/OpenGL/GLTexture2D.hpp>

#include <BlendInt/Gui/AbstractFloatingFrame.hpp>
#include <BlendInt/Gui/AbstractLayout.hpp>
#include <BlendInt/Gui/FrameShadow.hpp>
#include <BlendInt/Gui/Decoration.hpp>

namespace BlendInt {

	class Dialog: public AbstractFloatingFrame
	{

	public:

		Dialog (const String& title = String(""), bool modal = false);

		virtual ~Dialog();

		void SetLayout (AbstractLayout* layout);

		void AddWidget (AbstractWidget* widget);

		void InsertWidget (int index, AbstractWidget* widget);

		void SetModal (bool modal);

		virtual AbstractView* GetFocusedView () const;

		AbstractLayout* layout () const
		{
			return layout_;
		}

		Cpp::EventRef<Dialog*> applied ()
		{
			return *applied_;
		}

		Cpp::EventRef<Dialog*> canceled ()
		{
			return *canceled_;
		}

	protected:

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual bool PositionUpdateTest (const PositionUpdateRequest& request);

		virtual bool PreDraw (const Context* context);

		virtual ResponseType Draw (const Context* context);

		virtual void PostDraw (const Context* context);

		virtual void FocusEvent (bool focus);

		virtual void MouseHoverInEvent (const Context* context);

		virtual void MouseHoverOutEvent (const Context* context);

		virtual ResponseType KeyPressEvent (const Context* context);

		virtual ResponseType ContextMenuPressEvent (const Context* context);

		virtual ResponseType ContextMenuReleaseEvent (const Context* context);

		virtual ResponseType MousePressEvent (const Context* context);

		virtual ResponseType MouseReleaseEvent (const Context* context);

		virtual ResponseType MouseMoveEvent (const Context* context);

		virtual ResponseType DispatchHoverEvent (const Context* context);

		virtual void UpdateLayout ();

	private:

		enum DialogFlagIndex {

			/**
			 * @brief If this dialog is a modal dialog
			 *
			 * 0 - modaless dialog
			 * 1 - modal dialog
			 */
			DialogModal = 0x1 << 0,

			/**
			 * @brief If the cursor is on border
			 */
			DialogCursorOnBorder = 0x1 << 1,

			/**
			 * @brief If mouse button pressed
			 */
			DialogMouseButtonPressed = 0x1 << 2
		};

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		void InitializeDialogOnce ();

		void SetFocusedWidget (AbstractWidget* widget);

		void OnFocusedWidgetDestroyed (AbstractWidget* widget);

		void OnHoverWidgetDestroyed (AbstractWidget* widget);

		void OnLayoutDestroyed (AbstractWidget* layout);

		void OnCloseButtonClicked (AbstractButton* button);

		inline bool modal () const
		{
			return dialog_flags_ & DialogModal;
		}

		inline bool cursor_on_border () const
		{
			return dialog_flags_ & DialogCursorOnBorder;
		}

		inline bool mouse_button_pressed () const
		{
			return dialog_flags_ & DialogMouseButtonPressed;
		}

		inline void set_modal (bool modal)
		{
			if(modal) {
				SETBIT(dialog_flags_, DialogModal);
			} else {
				CLRBIT(dialog_flags_, DialogModal);
			}
		}

		inline void set_cursor_on_border (bool cursor_on_border)
		{
			if(cursor_on_border) {
				SETBIT(dialog_flags_, DialogCursorOnBorder);
			} else {
				CLRBIT(dialog_flags_, DialogCursorOnBorder);
			}
		}

		inline void set_mouse_button_pressed (bool pressed)
		{
			if(pressed) {
				SETBIT(dialog_flags_, DialogMouseButtonPressed);
			} else {
				CLRBIT(dialog_flags_, DialogMouseButtonPressed);
			}
		}

		glm::mat4 projection_matrix_;

		glm::mat3 model_matrix_;

		Point last_position_;

		Size last_size_;

		Point cursor_point_;

		RefPtr<FrameShadow> shadow_;

		AbstractWidget* focused_widget_;

		AbstractWidget* hovered_widget_;

		Decoration* decoration_;

		AbstractLayout* layout_;

		//
		// 0: inner
		// 1: outer
		// 2: texture buffer
		GLuint vao_[3];

		GLBuffer<ARRAY_BUFFER, 3> buffer_;

        GLTexture2D texture_buffer_;

        int cursor_position_;

        unsigned int dialog_flags_;

        boost::scoped_ptr<Cpp::Event<Dialog*> > applied_;

        boost::scoped_ptr<Cpp::Event<Dialog*> > canceled_;

	};

}

#endif /* _BLENDINT_GUI_DIALOG_HPP_ */
