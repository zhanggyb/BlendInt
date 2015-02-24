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

#include <core/margin.hpp>

#include <opengl/gl-vertexarrays.hpp>
#include <opengl/gl-buffer.hpp>

#include <gui/font.hpp>
#include <gui/abstract-slider.hpp>

namespace BlendInt {

	class NumericalSlider: public AbstractSlider<double>
	{
	DISALLOW_COPY_AND_ASSIGN(NumericalSlider);

	public:

		NumericalSlider (Orientation orientation = Horizontal);

		NumericalSlider (const String& title, Orientation orientation = Horizontal);

		virtual ~NumericalSlider ();

		void SetTitle (const String& title);

		virtual bool IsExpandX () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual void PerformOrientationUpdate (Orientation orientation);

		virtual void PerformMinimumUpdate (double minimum);

		virtual void PerformMaximumUpdate (double maximum);

		virtual void PerformValueUpdate (double value);

		virtual void PerformStepUpdate (double step);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (int round_type);

		virtual void PerformRoundRadiusUpdate (float radius);

		virtual Response Draw (AbstractWindow* context);

		virtual void PerformHoverIn (AbstractWindow* context);

		virtual void PerformHoverOut (AbstractWindow* context);

	private:

		void InitializeNumericalSlider ();

		float GetSlidePosition (float border, double value);

		/**
		 * @brief VertexArray objects used in this widget
		 *
		 * [0] - inner buffer
		 * [1] - outer buffer
		 */
		GLVertexArrays<2> vao_;

		GLBuffer<ARRAY_BUFFER, 2> vbo_;

		Font font_;

		String title_;

		bool hover_;

		static Margin default_numberslider_padding;
	};

}
