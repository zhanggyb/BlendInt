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

#ifndef _BLENDINT_GUI_CLOCK_HPP_
#define _BLENDINT_GUI_CLOCK_HPP_

#include <BlendInt/Core/Color.hpp>
#include <BlendInt/Core/Timer.hpp>

#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/OpenGL/GLBuffer.hpp>

namespace BlendInt {

	/**
	 * @brief A widget to show a clock
	 *
	 * This widget is used for demo or timer testing.
	 */
	class Clock: public Widget
	{
		DISALLOW_COPY_AND_ASSIGN(Clock);

	public:

		Clock();

		virtual ~Clock();

		void Start ();

		void Stop ();

		void set_background (const Color& color)
		{
			background_ = color;
		}

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (const Context* context);

		void GenerateClockVertices (int radius, float border,
		        std::vector<GLfloat>& inner_vertices,
		        std::vector<GLfloat>& outer_vertices);

	private:

		void OnUpdateClockHands(Timer* t);

		void InitializeClock ();

		// 0 - inner
		// 1 - outer
		// 2 - second hand
		GLuint vao_[3];

		GLBuffer<ARRAY_BUFFER, 3> buffer_;

		int angle_;

		RefPtr<Timer> timer_;

		Color background_;
	};

}

#endif /* _BLENDINT_GUI_CLOCK_HPP_ */
