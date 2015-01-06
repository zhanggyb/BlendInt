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

#ifndef _BLENDINT_GUI_SCROLLBAR_HPP_
#define _BLENDINT_GUI_SCROLLBAR_HPP_

#include <BlendInt/OpenGL/GLBuffer.hpp>
#include <BlendInt/Gui/Slider.hpp>

#include <BlendInt/Gui/AbstractButton.hpp>

namespace BlendInt {

	/**
	 * @brief ScrollBar
	 *
	 * Usually used in ScrollArea
	 */
	class ScrollBar: public AbstractSlider<int>
	{
	public:

		ScrollBar (Orientation orientation = Horizontal);

		virtual ~ScrollBar ();

		/**
		 * @brief Set the how many percentage of area the slide covers
		 */
		void SetSliderPercentage (int percentage);

		int GetSliderPercentage () const;

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual void PerformOrientationUpdate (Orientation orientation);

		virtual void PerformMinimumUpdate (int minimum);

		virtual void PerformMaximumUpdate (int maximum);

		virtual void PerformValueUpdate (int value);

		virtual void PerformStepUpdate (int step);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (const Context* context);

		virtual void PerformHoverOut (const Context* context);

		virtual ResponseType PerformMousePress (const Context* context);

		virtual ResponseType PerformMouseMove (const Context* context);

		virtual ResponseType PerformMouseRelease (const Context* context);

		/**
		 * @brief calculate the space width in which slider can move
		 * @return
		 */
		int GetSpace ();

	private:

		void InitScrollBarOnce ();

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
		int GetSlidePosition ();

		GLuint vao_[2];

		GLBuffer<ARRAY_BUFFER, 2> buffer_;

		Point m_cursor_origin;
		SlideIcon m_slide;

		int m_last_value;

	};

}

#endif /* _BLENDINT_GUI_SCROLLBAR_HPP_ */
