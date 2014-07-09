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
#include <BlendInt/OpenGL/GLArrayBuffer.hpp>

#include <BlendInt/Gui/AbstractRoundForm.hpp>

namespace BlendInt {

	enum ShadowDirection {
		ShadowNone = 0x0,
		ShadowLeft = (0x1 << 0),
		ShadowRight = (0x1 << 1),
		ShadowTop = (0x1 << 2),
		ShadowBottom = (0x1 << 3),
		ShadowAll = (ShadowLeft | ShadowRight | ShadowTop | ShadowBottom)
	};

	/**
	 * @brief Shadow form
	 */
	class Shadow: public AbstractRoundForm
	{
		DISALLOW_COPY_AND_ASSIGN(Shadow);

	public:

		Shadow (const Size& size, int round_type, float radius);

		virtual ~Shadow ();

		void Update (int width, int height, int round_type, float radius);

		void Update (const Size& size, int round_type, float radius);

		virtual void Draw (const glm::mat4& mvp, short gamma = 0);

		void DrawAt (const glm::mat4& mvp, int x, int y);

		void DrawAt (const glm::mat4& mvp, const Point& pos);

	protected:

		virtual void UpdateGeometry (const UpdateRequest& request);

	private:

		// Disable default constructor
		Shadow();

		void InitializeShadow ();

		/**
		 * @brief Create shadow vertices
		 * @param[in] size The shadow inner size
		 * @param[in] round_type Round type, same as widget
		 * @param[in] radius Round radius
		 * @param[in] depth The shadow size
		 * @param[out] vertices The vertices created
		 */
		void GenerateShadowVerticesExt (const Size& size, int round_type, float radius, std::vector<GLfloat>& vertices);

		GLuint m_vao;

		RefPtr<GLArrayBuffer> m_buffer;

	};

}

#endif /* _BLENDINT_GUI_SHADOW_HPP_ */
