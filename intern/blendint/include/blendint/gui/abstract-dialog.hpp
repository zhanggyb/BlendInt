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

#include <gui/frame.hpp>
#include <gui/view-buffer.hpp>

namespace BlendInt {

    class AbstractDialog: public Frame
    {
    public:

    	enum DialogFlag {

			/**
			 * @brief If this dialog is a modal dialog
			 *
			 * 0 - modaless dialog
			 * 1 - modal dialog
			 */
			DialogModal = 0x1 << 0,

			DialogButtonOK = 0x1 << 1,

			DialogButtonApply = 0x1 << 2,

			DialogButtonCancel = 0x1 << 3

    	};

        AbstractDialog (int flags = 0);
        
        AbstractDialog (int width, int height, int flags = 0);
        
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
        
		virtual Response Draw (AbstractWindow* context) = 0;

		virtual Response PerformKeyPress (AbstractWindow* context);

		virtual Response PerformMousePress (AbstractWindow* context);

		virtual Response PerformMouseRelease (AbstractWindow* context);

		virtual Response PerformMouseMove (AbstractWindow* context);

		virtual Response DispatchHoverEvent (AbstractWindow* context);

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

		enum DialogFlagPrivate {

			/**
			 * @brief If the cursor is on border
			 */
			DialogCursorOnBorder = 0x1 << 4,

			/**
			 * @brief If mouse button pressed
			 */
			DialogMouseButtonPressed = 0x1 << 5
		};

		void OnFocusedWidgetDestroyed (AbstractWidget* widget);

		void OnHoverWidgetDestroyed (AbstractWidget* widget);

		Point last_position_;

		Size last_size_;

		Point cursor_point_;

		AbstractWidget* focused_widget_;

		AbstractWidget* hovered_widget_;

        int cursor_position_;

        unsigned int dialog_flags_;

        boost::scoped_ptr<Cpp::Event<AbstractDialog*> > applied_;
        
        boost::scoped_ptr<Cpp::Event<AbstractDialog*> > canceled_;

    };

}
