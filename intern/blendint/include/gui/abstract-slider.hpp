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

#include <core/types.hpp>

#include <Cpp/Events.hpp>

#include <opengl/gl-buffer.hpp>

#include <gui/abstract-round-widget.hpp>
#include <gui/abstract-round-form.hpp>

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

		virtual void Draw (int x,
				int y,
				const float* color_ptr = Color(Color::Black).data(),
				short gamma = 0,
				float rotate = 0.f,
				float scale_x = 1.f,
				float scale_y = 1.f) const;

	protected:

		virtual void PerformSizeUpdate (const Size& size);

		virtual void PerformRoundTypeUpdate (int type);

		virtual void PerformRoundRadiusUpdate (float radius);

	private:

		void InitializeSliderIcon ();

		GLuint vao_[2];
		GLBuffer<ARRAY_BUFFER, 2> buffer_;
	};

	template<typename T>
	class AbstractSlider: public AbstractRoundWidget
	{
	public:

		AbstractSlider (Orientation orientation = Horizontal);

		virtual ~AbstractSlider ();

		void SetValue (T value);

		void SetRange (T value1, T value2);

		void SetMinimum (T minimum);

		void SetMaximum (T maximum);

		void SetOrientation (Orientation orientation);

		T minimum () const {return minimum_;}

		T maximum () const {return maximum_;}

		T value () const {return value_;}

		T step () const {return step_;}

		Orientation orientation () const {return orientation_;}

		Cpp::EventRef<T> slider_moved () {return slider_moved_;}

		Cpp::EventRef<> slider_pressed () {return slider_pressed_;}

		Cpp::EventRef<> slider_released () {return slider_released_;}

		Cpp::EventRef<T> value_changed () {return value_changed_;}

	protected:

		virtual void PerformOrientationUpdate (Orientation orientation);

		virtual void PerformMinimumUpdate (T minimum);

		virtual void PerformMaximumUpdate (T maximum);

		virtual void PerformValueUpdate (T value);

		virtual void PerformStepUpdate (T step);

		void set_value (T value)
		{
			value_ = value;
		}

		void set_step (T step)
		{
			step_ = step;
		}

		void set_orientation (Orientation orientation)
		{
			orientation_ = orientation;
		}

		void fire_slider_moved_event (T value)
		{
			slider_moved_.fire(value);
		}

		void fire_slider_pressed ()
		{
			slider_pressed_.fire();
		}

		void fire_slider_released ()
		{
			slider_released_.fire();
		}

		void fire_value_changed_event (T value)
		{
			value_changed_.fire(value);
		}

	private:

		Orientation orientation_;

		T value_;

		T minimum_;
		T maximum_;
		T step_;

		Cpp::Event<T> slider_moved_;

		Cpp::Event<> slider_pressed_;

		Cpp::Event<> slider_released_;

		Cpp::Event<T> value_changed_;

	};

	template <typename T>
	AbstractSlider<T>::AbstractSlider (Orientation orientation)
	: AbstractRoundWidget(),
	  orientation_(orientation),
	  value_(T(0)),
	  minimum_(T(0)),
	  maximum_(T(100)),
	  step_(T(5))
	{
	}

	template <typename T>
	AbstractSlider<T>::~AbstractSlider ()
	{
	}

	template <typename T>
	void AbstractSlider<T>::SetValue (T value)
	{
		if (value == value_) {
			return;
		}

		if (value < minimum_ || value > maximum_)
			return;

		PerformValueUpdate(value);
		value_ = value;
		value_changed_.fire(value_);
	}

	template <typename T>
	void AbstractSlider<T>::SetRange (T value1, T value2)
	{
		T minimum = std::min(value1, value2);
		T maximum = std::max(value1, value2);

		if(minimum == maximum)
			return;

		if(value1 == minimum) {
			PerformMinimumUpdate(minimum);
			PerformMaximumUpdate(maximum);
		} else {
			PerformMaximumUpdate(maximum);
			PerformMinimumUpdate(minimum);
		}

		minimum_ = minimum;
		maximum_ = maximum;
	}

	template <typename T>
	void AbstractSlider<T>::SetMinimum (T minimum)
	{
		if (minimum_ == minimum)
			return;

		if (minimum >= maximum_)
			return;

		PerformMinimumUpdate(minimum);
		minimum_ = minimum;
	}

	template <typename T>
	void AbstractSlider<T>::SetMaximum (T maximum)
	{
		if (maximum_ == maximum)
			return;

		if (maximum <= minimum_)
			return;

		PerformMaximumUpdate(maximum);
		maximum_ = maximum;
	}

	template <typename T>
	void AbstractSlider<T>::SetOrientation (Orientation orientation)
	{
		if(orientation_ == orientation) return;

		PerformOrientationUpdate(orientation);
		orientation_ = orientation;
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
