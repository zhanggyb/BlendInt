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
 * License along with BlendInt.	 If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_GUI_WIDGET_HPP_
#define _BLENDINT_GUI_WIDGET_HPP_

#include <BlendInt/Gui/AbstractWidget.hpp>

namespace BlendInt {

	/**
	 * @brief A Normal widget
	 *
	 * @ingroup widgets
	 */
	class Widget: public AbstractWidget
	{
		DISALLOW_COPY_AND_ASSIGN(Widget);

	public:

		Widget ();

		virtual ~Widget();

		void SetRoundRadius (float radius);

		inline float round_radius () const
		{
			return round_radius_;
		}

	protected:

		void GenerateRoundedVertices (
				std::vector<GLfloat>* inner,
				std::vector<GLfloat>* outer);

		void GenerateRoundedVertices (
				Orientation shadedir,
				short shadetop,
				short shadedown,
				std::vector<GLfloat>* inner,
				std::vector<GLfloat>* outer);

		virtual void PerformRoundRadiusUpdate (float radius);

		virtual ResponseType Draw (Profile& profile);

		inline void set_round_radius (float radius)
		{
			round_radius_ = radius;
		}

	private:

		float round_radius_;

	};

}

#endif /* _BLENDINT_GUI_WIDGET_HPP_ */
