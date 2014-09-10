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

		virtual void Draw (const glm::vec3& pos, short gamma = 0);

	protected:

		virtual void UpdateGeometry (const UpdateRequest& request);

	private:

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

	class ShadowExt: public AbstractRoundForm
	{
		DISALLOW_COPY_AND_ASSIGN(ShadowExt);

	public:

		ShadowExt();

		ShadowExt(const Size& size, int round_type, float radius);

		virtual ~ShadowExt ();

		virtual void Draw (const glm::vec3& pos, short gamma = 0);

	protected:

		virtual void UpdateGeometry (const UpdateRequest& request);

	private:

		void InitializeShadow ();

		void GenerateShadowVertices (const Size& size, int round_type, float radius, std::vector<GLfloat>& vertices);

		float make_shaded_offset (short shadetop, short shadedown, float fact)
		{
			float faci = glm::clamp(fact - 0.5f / 255.f, 0.f, 1.f);
			float facm = 1.f - fact;

			return faci * (shadetop / 255.f) + facm * (shadedown / 255.f);
		}

		GLuint vao_;

		RefPtr<GLArrayBuffer> buffer_;

		Color color_;
	};

}

#endif /* _BLENDINT_GUI_SHADOW_HPP_ */
