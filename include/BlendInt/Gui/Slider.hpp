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

#ifndef _BLENDINT_SLIDER_HPP_
#define _BLENDINT_SLIDER_HPP_

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>
#include <BlendInt/Gui/AbstractSlider.hpp>

namespace BlendInt {

	/**
	 * @brief Slider widget
	 */
	class Slider: public AbstractSlider<int>
	{
	public:

		Slider (Orientation orientation = Horizontal);

		virtual ~Slider ();

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual void PerformOrientationUpdate (Orientation orientation);

		virtual void PerformMinimumUpdate (int minimum);

		virtual void PerformMaximumUpdate (int maximum);

		virtual void PerformValueUpdate (int value);

		virtual void PerformStepUpdate (int step);

		virtual ResponseType Draw (Profile& profile);

		virtual ResponseType FocusEvent (bool focus);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		/**
		 * @brief calculate the space width in which slider can move
		 * @return
		 */
		int GetSpace ();

	private:

		void InitOnce ();

		/**
		 * @brief Check if cursor is on the slide icon
		 */
		bool CursorOnSlideIcon (const Point& cursor);

		/**
		 * @brief Get the new value at the cursor position
		 * @param[in] cursor The cursor position
		 * @param[out] vout The new value
		 * @return
		 * 	- true if the cursor is in range
		 * 	- false if the cursor is out of range
		 */
		bool GetNewValue (const Point& cursor, int* vout);

		/**
		 * @brief
		 */
		inline float get_position ()
		{
			return value() * GetSpace()
			                / ((float) maximum() - (float) minimum());
		}

		//GLuint m_vao;

		//RefPtr<GLArrayBuffer> m_line;

		SlideIcon m_slide_icon;

		Point m_last_cursor;	// used for mouse move

		int m_last_value;

		/**
		 * @brief If the slide switch is pressed
		 */
		bool m_pressed;
	};

}

#endif // _BLENDINT_SLIDER_HPP_
