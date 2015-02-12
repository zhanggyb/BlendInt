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

#include <core/rect.hpp>
#include <core/timer.hpp>
#include <core/point.hpp>
#include <core/margin.hpp>

#include <opengl/gl-buffer.hpp>

#include <gui/text.hpp>
#include <gui/widget.hpp>

namespace BlendInt {

	/**
	 * @brief A one-line text editor
	 */
	class TextEntry: public Widget
	{
		DISALLOW_COPY_AND_ASSIGN(TextEntry);

	public:

		TextEntry ();

		TextEntry (const String& text);

		virtual ~TextEntry();

		void SetText (const String& text);

        /**
         * @brief Clear the text
         */
        void ClearText ();
        
		void SetFont (const Font& font);

		virtual Size GetPreferredSize () const;

		virtual bool IsExpandX () const;

		static inline const Margin& padding ()
		{
			return kPadding;
		}

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (int round_type);

		virtual void PerformRoundRadiusUpdate (float radius);

        virtual void PerformFocusOn (AbstractWindow* context);
        
        virtual void PerformFocusOff (AbstractWindow* context);
        
        virtual void PerformHoverIn (AbstractWindow* context);
        
        virtual void PerformHoverOut (AbstractWindow* context);
        
		virtual Response PerformKeyPress (AbstractWindow* context);

		virtual Response PerformMousePress (AbstractWindow* context);

        virtual Response Draw (AbstractWindow* context);
        
	private:

		void InitializeTextEntry ();

		void DisposeBackspacePress ();

		void DisposeDeletePress ();

		void DisposeLeftPress ();

		void DisposeRightPress ();

		size_t GetTextCursorIndex (AbstractWindow* context);

		/**
		 * @brief Vertex array objects
		 *
		 * 	- 0: for inner buffer
		 * 	- 1: for outline buffer
		 * 	- 2: for cursor buffer
		 */
		GLuint vao_[3];

        GLBuffer<ARRAY_BUFFER, 3> vbo_;

        RefPtr<Text> text_;
        
		size_t text_start_;	// where start print the text

		/**
		 * @brief Where display the cursor and insert new text
		 */
		size_t cursor_index_;

		// the space between the text and the top
		// o the text and the bottom
		static const int vertical_space = 2;

		static Margin kPadding;
	};

}
