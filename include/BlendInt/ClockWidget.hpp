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

#ifndef _BLENDINT_CLOCKWIDGET_HPP_
#define _BLENDINT_CLOCKWIDGET_HPP_

#include <BlendInt/Widget.hpp>
#include <BlendInt/Timer.hpp>
#include <BlendInt/Color.hpp>

namespace BlendInt {

	/**
	 * @brief A widget to show a clock
	 *
	 * This widget is used for demo or test also.
	 */
	class ClockWidget: public Widget
	{
		DISALLOW_COPY_AND_ASSIGN(ClockWidget);

	public:

		ClockWidget();

		ClockWidget(AbstractWidget* parent);

		void set_background (const Color& color)
		{
			m_background = color;
		}

	protected:

		virtual ~ClockWidget();

//		virtual void Update (int type, const void* data);

		virtual void Draw ();

	private:

		void UpdateClockHands();

		void DrawArc (float x, float y, float start_angle, float end_angle, float delta_angle, float radius,bool fill);

		void DrawCircle(float x, float y, float radius,bool fill);

		void DrawPie(float x,float y,float start_angle,float end_angle,float delta_angle,float radius,bool fill);

		void Init ();

		int m_angle;

		Timer* m_timer;

		Color m_background;
	};

}

#endif /* _BLENDINT_CLOCKWIDGET_HPP_ */
