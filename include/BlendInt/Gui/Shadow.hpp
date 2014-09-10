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

#ifndef _BLENDINT_GUI_SHADOW_HPP_
#define _BLENDINT_GUI_SHADOW_HPP_

#include <deque>

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Core/Color.hpp>

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>

#include <BlendInt/Gui/AbstractRoundForm.hpp>

namespace BlendInt {

	class Shadow: public AbstractRoundForm
	{
		DISALLOW_COPY_AND_ASSIGN(Shadow);

	public:

		Shadow();

		Shadow(const Size& size, int round_type, float radius);

		virtual ~Shadow ();

		void SetColor (const Color& color);

		virtual void Draw (const glm::vec3& pos, short gamma = 0);

	protected:

		virtual void PerformSizeUpdate (const Size& size);

		virtual void PerformRoundTypeUpdate (int type);

		virtual void PerformRoundRadiusUpdate (float radius);

	private:

		void InitializeShadow ();

		void GenerateShadowVertices (const Size& size, int round_type, float radius, std::vector<GLfloat>& vertices);

		inline float make_shaded_offset (short shadetop, short shadedown, float fact);

		GLuint vao_;

		RefPtr<GLArrayBuffer> buffer_;

		Color color_;
	};

}

#endif /* _BLENDINT_GUI_SHADOW_HPP_ */
