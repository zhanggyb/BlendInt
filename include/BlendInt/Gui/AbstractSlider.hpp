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

#ifndef _BLENDINT_GUI_ABSTRACTSLIDER_HPP_
#define _BLENDINT_GUI_ABSTRACTSLIDER_HPP_

#include <BlendInt/Types.hpp>

#include <Cpp/Events.hpp>

#include <BlendInt/Gui/RoundWidget.hpp>
#include <BlendInt/Gui/AbstractRoundForm.hpp>

namespace BlendInt {

	class AbstractRoundForm;

	/**
	 * @brief Slide Icon used in Slider or ScrollBar
	 */
	class SlideIcon: public AbstractRoundForm
	{
	public:

		/**
		 * @brief Default constructor
		 */
		SlideIcon ();

		/**
		 * @brief Constructor
		 */
		virtual ~SlideIcon ();

		/**
		 * @brief Draw the icon
		 */
		virtual void Draw (const glm::mat4& mvp);

		/**
		 * @brief Set the highlight status of this icon
		 * @param[in] highlight The highlight status
		 */
		void set_highlight (bool highlight)
		{
			m_highlight = highlight;
		}

	protected:

		virtual bool Update (const UpdateRequest& request);

	private:

		void InitOnce ();

		GLuint m_vao;

		RefPtr<GLArrayBuffer> m_inner_buffer;
		RefPtr<GLArrayBuffer> m_outer_buffer;

		bool m_highlight;
	};

	/**
	 * @brief The abstract class for slider widgets
	 *
	 * The abstract class for Slider, ScrollBar etc.
	 */
	class AbstractSlider: public RoundWidget
	{
	public:

		AbstractSlider(Orientation orientation = Horizontal);

		virtual ~AbstractSlider ();

		void SetValue (int value);

		void SetRange (int value1, int value2);

		void SetMinimum (int minimum);

		void SetMaximum (int maximum);

		void SetOrientation (Orientation orientation);

		int minimum () const {return m_minimum;}

		int maximum () const {return m_maximum;}

		int value () const {return m_value;}
		
		int step () const {return m_step;}

		Orientation orientation () const {return m_orientation;}

		Cpp::EventRef<int> slider_moved () {return m_slider_moved;}

		Cpp::EventRef<> slider_pressed () {return m_slider_pressed;}

		Cpp::EventRef<> slider_released () {return m_slider_released;}

		Cpp::EventRef<int> value_changed () {return m_value_changed;}

	protected:

		virtual void Draw (RedrawEvent* event) = 0;

		inline void set_value (int value)
		{
			m_value = value;
		}

		inline void set_step (int step)
		{
			m_step = step;
		}

		inline void set_orientation (Orientation orientation)
		{
			m_orientation = orientation;
		}

		inline void fire_slider_moved_event (int value)
		{
			m_slider_moved.fire(value);
		}

		inline void fire_slider_pressed ()
		{
			m_slider_pressed.fire();
		}

		inline void fire_slider_released ()
		{
			m_slider_released.fire();
		}

		inline void fire_value_changed_event (int value)
		{
			m_value_changed.fire(value);
		}

	private:

		Orientation m_orientation;

		int m_value;

		int m_minimum;
		int m_maximum;
		int m_step;

		Cpp::Event<int> m_slider_moved;

		Cpp::Event<> m_slider_pressed;

		Cpp::Event<> m_slider_released;

		Cpp::Event<int> m_value_changed;

	};

}

#endif	// _BLENDINT_GUI_ABSTRACTSLIDER_HPP_
