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

#ifndef _BLENDINT_SCROLLBAR_HPP_
#define _BLENDINT_SCROLLBAR_HPP_

#include <BlendInt/Slider.hpp>

#include <BlendInt/opengl/GLArrayBuffer.hpp>

namespace BlendInt {

	/**
	 * The controller in the scrollbar
	 */
	class ScrollControl: public AbstractButton
	{
	public:

		ScrollControl ();

		ScrollControl (AbstractWidget* parent);

	protected:

		virtual ~ScrollControl ();

		virtual void Update (int type, const void* data);

		virtual void Draw ();

		virtual void MouseMoveEvent (MouseEvent* event);

		virtual void MousePressEvent (MouseEvent* event);

		virtual void MouseReleaseEvent (MouseEvent* event);

	private:

		void Init ();

		Point m_move_start;

		Point m_position_origin;

		GLArrayBuffer* m_inner_buffer;
		GLArrayBuffer* m_outer_buffer;
		GLArrayBuffer* m_highlight_buffer;
	};

	/**
	 * @brief A slider used in ScrollBar with a dark background
	 */
	class SliderBar: public AbstractSlider
	{
		DISALLOW_COPY_AND_ASSIGN(SliderBar);

	public:

		SliderBar (Orientation orientation = Horizontal);

		SliderBar (Orientation orientation, AbstractWidget* parent);

		virtual ~SliderBar ();

	protected:

		virtual void Update (int type, const void* data);

		virtual void Draw ();

		virtual void MouseMoveEvent (MouseEvent* event);

		virtual void MousePressEvent (MouseEvent* event);

		virtual void MouseReleaseEvent (MouseEvent* event);

	private:

		/**
		 * @brief calculate and return the space width in which scroll control can move
		 * @return
		 */
		int GetSpace ();

		void Init ();

		void set_control_size (size_t size);

		GLArrayBuffer* m_inner_buffer;
		GLArrayBuffer* m_outer_buffer;

		ScrollControl* m_control_button;
	};

	class ScrollBar: public AbstractSlider
	{
		DISALLOW_COPY_AND_ASSIGN(ScrollBar);

	public:

		ScrollBar (Orientation orientation = Horizontal);

		ScrollBar (Orientation orientation, AbstractWidget* parent);

		virtual ~ScrollBar ();

	protected:

		virtual void Update (int type, const void* data);

		virtual void Draw ();

		virtual void MouseMoveEvent (MouseEvent* event);

		virtual void MousePressEvent (MouseEvent* event);

		virtual void MouseReleaseEvent (MouseEvent* event);

	private:

		void update_shape (const Size* size);

		/**
		 * @brief calculate and return the space width in which scroll control can move
		 * @return
		 */
		int get_space ();

		ScrollControl* m_scroll_control;

		GLArrayBuffer* m_inner_buffer;
		GLArrayBuffer* m_outer_buffer;
	};

}

#endif /* _BIL_SCROLLBAR_HPP_ */
