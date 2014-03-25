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

#ifndef _BLENDINT_NUMBERSLIDER_HPP_
#define _BLENDINT_NUMBERSLIDER_HPP_

#include <BlendInt/Gui/AbstractSlider.hpp>

namespace BlendInt {

	class NumberSlider: public AbstractSlider
	{
		DISALLOW_COPY_AND_ASSIGN(NumberSlider);

	public:

		NumberSlider (Orientation orientation = Horizontal);

		virtual ~NumberSlider ();

	protected:

		virtual bool Update (const UpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

	private:

		void InitOnce ();

		RefPtr<GLArrayBuffer> m_inner_buffer;
		RefPtr<GLArrayBuffer> m_outer_buffer;

		GLuint m_vao;
	};
}

#endif /* _BLENDINT_NUMBERSLIDER_HPP_ */
