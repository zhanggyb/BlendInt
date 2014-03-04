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
#include <BlendInt/Gui/AbstractRoundForm.hpp>
#include <BlendInt/Gui/Button.hpp>

namespace BlendInt {

	class SlideSwitch: public AbstractRoundForm
	{
	public:

		SlideSwitch ();

		virtual ~SlideSwitch ();

	protected:

		virtual bool Update (int type, const void* data);

		virtual void Draw ();

	private:

		void InitOnce ();

		RefPtr<GLArrayBuffer> m_inner_buffer;
		RefPtr<GLArrayBuffer> m_outer_buffer;
	};

	class SlideButton: public AbstractButton
	{
	public:

		SlideButton ();

		virtual ~SlideButton ();

	protected:

		virtual bool Update (int type, const void* data);

		virtual void Draw ();

		virtual void MouseMoveEvent (MouseEvent* event);

		virtual void MousePressEvent (MouseEvent* event);

		virtual void MouseReleaseEvent (MouseEvent* event);

	private:

		void InitOnce ();

		Point m_move_start;

		Point m_position_origin;

		RefPtr<GLArrayBuffer> m_inner_buffer;
		RefPtr<GLArrayBuffer> m_outer_buffer;
		RefPtr<GLArrayBuffer> m_highlight_buffer;
	};

	/**
	 * @brief Slider widget
	 */
	class Slider: public AbstractSlider
	{
	public:

		Slider (Orientation orientation = Horizontal);

		Slider (Orientation orientation, AbstractWidget* parent);

		virtual ~Slider ();

		void set_control_size (size_t size);

		void set_control_widget (SlideButton *widget);

	protected:

		virtual bool Update (int type, const void* data);

		virtual void Draw ();

		virtual void MouseMoveEvent (MouseEvent* event);

		virtual void MousePressEvent (MouseEvent* event);

		virtual void MouseReleaseEvent (MouseEvent* event);

		/**
		 * @brief calculate the space width in which slider can move
		 * @return
		 */
		int get_space ();

		RefPtr<SlideButton> m_slide_button;

	private:

		inline float get_position ()
		{return value() * get_space() / ((float)maximum() - (float)minimum());}

	};

}

#endif // _BLENDINT_SLIDER_HPP_
