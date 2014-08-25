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

#ifndef _BLENDINT_GUI_TEXTENTRY_HPP_
#define _BLENDINT_GUI_TEXTENTRY_HPP_

#include <BlendInt/Core/Rect.hpp>
#include <BlendInt/Core/Timer.hpp>
#include <BlendInt/Core/String.hpp>
#include <BlendInt/Core/Point.hpp>
#include <BlendInt/Core/Margin.hpp>

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>

#include <BlendInt/Gui/AbstractWidget.hpp>
#include <BlendInt/Gui/Font.hpp>

namespace BlendInt {

	/**
	 * @brief A one-line text editor
	 */
	class TextEntry: public AbstractWidget
	{
		DISALLOW_COPY_AND_ASSIGN(TextEntry);

	public:

		TextEntry();

		virtual ~TextEntry();

		/**
		 * @brief Clear the text
		 */
		void Clear ();

		void SetText (const String& text);

		void SetFont (const Font& font);

		const String& text () const {return text_;}

		virtual Size GetPreferredSize () const;

		virtual bool IsExpandX () const;

		static const Margin& DefaultPadding ()
		{
			return default_padding;
		}

	protected:

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

		void InitializeTextEntry ();

		void DisposeBackspacePress ();

		void DisposeDeletePress ();

		void DisposeLeftPress ();

		void DisposeRightPress ();

		int GetValidTextSize ();

		int GetCursorPosition (const MouseEvent& event);

		/**
		 * @brief Get the index and length of the text to show
		 * @param[out] start The index in the text to print
		 * @param[out] length The length of the text to print
		 */
		void GetVisibleTextRange (size_t* start, size_t* length);

		/**
		 * @brief Vertex array objects
		 *
		 * 	- 0: for inner buffer
		 * 	- 1: for outline buffer
		 * 	- 2: for cursor buffer
		 */
		GLuint vaos_[3];

		Font font_;

		String text_;

		int start_;	// where start print the text

		int length_;	// the text length inside this widget

		/**
		 * @brief Where display the cursor and insert new text
		 */
		int index_;

		RefPtr<GLArrayBuffer> inner_;
		RefPtr<GLArrayBuffer> outer_;

		RefPtr<GLArrayBuffer> cursor_buffer_;

		// the space between the text and the top
		// o the text and the bottom
		static const int vertical_space = 2;

		static Margin default_padding;
	};

}

#endif /* _BLENDINT_GUI_TEXTENTRY_HPP_ */
