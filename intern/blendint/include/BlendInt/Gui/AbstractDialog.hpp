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

#pragma once

#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/ViewBuffer.hpp>

namespace BlendInt {

    class AbstractDialog: public Frame
    {
        
    public:
        
        AbstractDialog ();
        
        AbstractDialog (int width, int height);
        
        virtual ~AbstractDialog ();

		Cpp::EventRef<AbstractDialog*> applied ()
		{
			return *applied_;
		}

		Cpp::EventRef<AbstractDialog*> canceled ()
		{
			return *canceled_;
		}

    protected:
        
		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual bool PreDraw (AbstractWindow* context);

		virtual ResponseType Draw (AbstractWindow* context);

		virtual void PostDraw (AbstractWindow* context);

		virtual void PerformFocusOn (AbstractWindow* context);

		virtual void PerformFocusOff (AbstractWindow* context);

		virtual void PerformHoverIn (AbstractWindow* context);

		virtual void PerformHoverOut (AbstractWindow* context);

		virtual ResponseType PerformKeyPress (AbstractWindow* context);

		virtual ResponseType PerformContextMenuPress (AbstractWindow* context);

		virtual ResponseType PerformContextMenuRelease (AbstractWindow* context);

		virtual ResponseType PerformMousePress (AbstractWindow* context);

		virtual ResponseType PerformMouseRelease (AbstractWindow* context);

		virtual ResponseType PerformMouseMove (AbstractWindow* context);

		virtual ResponseType DispatchHoverEvent (AbstractWindow* context);

		virtual void UpdateLayout () = 0;

		void SetFocusedWidget (AbstractWidget* widget, AbstractWindow* context);

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

        inline void fire_applied_event ()
        {
            applied_->fire(this);
        }
        
        inline void fire_canceled_event ()
        {
            canceled_->fire(this);
        }

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

		void OnFocusedWidgetDestroyed (AbstractWidget* widget);

		void OnHoverWidgetDestroyed (AbstractWidget* widget);

		void InitializeAbstractDialog ();

		glm::mat4 projection_matrix_;

		glm::mat3 model_matrix_;

		Point last_position_;

		Size last_size_;

		Point cursor_point_;

		AbstractWidget* focused_widget_;

		AbstractWidget* hovered_widget_;

		//
		// 0: inner
		// 1: outer
		// 2: texture buffer
		GLuint vao_[3];

		GLBuffer<ARRAY_BUFFER, 3> vbo_;

        GLTexture2D texture_buffer_;
        //RefPtr<ViewBuffer> buffer_;

        int cursor_position_;

        unsigned int dialog_flags_;

        boost::scoped_ptr<Cpp::Event<AbstractDialog*> > applied_;
        
        boost::scoped_ptr<Cpp::Event<AbstractDialog*> > canceled_;

    };

}
