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

#ifndef _BLENDINT_GUI_ABSTRACTFLOATINGFRAME_HPP_
#define _BLENDINT_GUI_ABSTRACTFLOATINGFRAME_HPP_

#include <BlendInt/Gui/AbstractFrame.hpp>

namespace BlendInt {

	class AbstractFloatingFrame: public AbstractFrame
	{
	public:

		AbstractFloatingFrame ();

		virtual ~AbstractFloatingFrame ();

		virtual AbstractView* GetFocusedView () const;

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

		inline void set_round_radius (float radius)
		{
			round_radius_ = radius;
		}

	private:

		float round_radius_;

	};

}

#endif /* _BLENDINT_GUI_ABSTRACTFLOATINGFRAME_HPP_ */
