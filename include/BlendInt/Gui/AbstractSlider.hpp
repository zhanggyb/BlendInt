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

#include <BlendInt/Core/Types.hpp>

#include <Cpp/Events.hpp>

#include <BlendInt/Gui/AbstractWidget.hpp>
#include <BlendInt/Gui/AbstractRoundForm.hpp>

namespace BlendInt {

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
		virtual void Draw (const glm::vec3& pos, short gamma = 0);

		/**
		 * @brief Set the highlight status of this icon
		 * @param[in] highlight The highlight status
		 */
		void set_highlight (bool highlight)
		{
			m_highlight = highlight;
		}

	protected:

		virtual void UpdateGeometry (const UpdateRequest& request);

	private:

		void InitializeSliderIcon ();

		GLuint m_vao[2];

		RefPtr<GLArrayBuffer> m_inner_buffer;
		RefPtr<GLArrayBuffer> m_outer_buffer;

		bool m_highlight;
	};

	template<typename T>
	class AbstractSlider: public AbstractWidget
	{
	public:

		AbstractSlider (Orientation orientation = Horizontal);

		virtual ~AbstractSlider ();

		void SetValue (T value);

		void SetRange (T value1, T value2);

		void SetMinimum (T minimum);

		void SetMaximum (T maximum);

		void SetOrientation (Orientation orientation);

		T minimum () const {return m_minimum;}

		T maximum () const {return m_maximum;}

		T value () const {return m_value;}

		T step () const {return m_step;}

		Orientation orientation () const {return m_orientation;}

		Cpp::EventRef<T> slider_moved () {return m_slider_moved;}

		Cpp::EventRef<> slider_pressed () {return m_slider_pressed;}

		Cpp::EventRef<> slider_released () {return m_slider_released;}

		Cpp::EventRef<T> value_changed () {return m_value_changed;}

	protected:

		virtual void PerformOrientationUpdate (Orientation orientation);

		virtual void PerformMinimumUpdate (T minimum);

		virtual void PerformMaximumUpdate (T maximum);

		virtual void PerformValueUpdate (T value);

		virtual void PerformStepUpdate (T step);

		void set_value (T value)
		{
			m_value = value;
		}

		void set_step (T step)
		{
			m_step = step;
		}

		void set_orientation (Orientation orientation)
		{
			m_orientation = orientation;
		}

		void fire_slider_moved_event (T value)
		{
			m_slider_moved.fire(value);
		}

		void fire_slider_pressed ()
		{
			m_slider_pressed.fire();
		}

		void fire_slider_released ()
		{
			m_slider_released.fire();
		}

		void fire_value_changed_event (T value)
		{
			m_value_changed.fire(value);
		}

	private:

		Orientation m_orientation;

		T m_value;

		T m_minimum;
		T m_maximum;
		T m_step;

		Cpp::Event<T> m_slider_moved;

		Cpp::Event<> m_slider_pressed;

		Cpp::Event<> m_slider_released;

		Cpp::Event<T> m_value_changed;

	};

	template <typename T>
	AbstractSlider<T>::AbstractSlider (Orientation orientation)
	: AbstractWidget(),
	  m_orientation(orientation),
	  m_value(T(0)),
	  m_minimum(T(0)),
	  m_maximum(T(100)),
	  m_step(T(5))
	{
	}

	template <typename T>
	AbstractSlider<T>::~AbstractSlider ()
	{
	}

	template <typename T>
	void AbstractSlider<T>::SetValue (T value)
	{
		if (value == m_value) {
			return;
		}

		if (value < m_minimum || value > m_maximum)
			return;

		PerformValueUpdate(value);
		m_value = value;
		m_value_changed.fire(m_value);
	}

	template <typename T>
	void AbstractSlider<T>::SetRange (T value1, T value2)
	{
		int minimum = std::min(value1, value2);
		int maximum = std::max(value1, value2);

		if (m_minimum != minimum) {
			m_minimum = minimum;
			//Update(SliderPropertyMinimum, 0);
		}
		if (m_maximum != maximum) {
			m_maximum = maximum;
			//Update(SliderPropertyMaximum, 0);
		}
	}

	template <typename T>
	void AbstractSlider<T>::SetMinimum (T minimum)
	{
		if (m_minimum == minimum)
			return;

		if (minimum >= m_maximum)
			return;

		PerformMinimumUpdate(minimum);
		m_minimum = minimum;
	}

	template <typename T>
	void AbstractSlider<T>::SetMaximum (T maximum)
	{
		if (m_maximum == maximum)
			return;

		if (maximum <= m_minimum)
			return;

		PerformMaximumUpdate(maximum);
		m_maximum = maximum;
	}

	template <typename T>
	void AbstractSlider<T>::SetOrientation (Orientation orientation)
	{
		if(m_orientation == orientation) return;

		PerformOrientationUpdate(orientation);
		m_orientation = orientation;
	}

	template <typename T>
	void AbstractSlider<T>::PerformOrientationUpdate(Orientation orientation)
	{

	}

	template <typename T>
	void AbstractSlider<T>::PerformMinimumUpdate(T minimum)
	{

	}

	template <typename T>
	void AbstractSlider<T>::PerformMaximumUpdate(T maximum)
	{

	}

	template <typename T>
	void AbstractSlider<T>::PerformValueUpdate(T value)
	{

	}

	template <typename T>
	void AbstractSlider<T>::PerformStepUpdate(T step)
	{

	}

}

#endif	// _BLENDINT_GUI_ABSTRACTSLIDER_HPP_
