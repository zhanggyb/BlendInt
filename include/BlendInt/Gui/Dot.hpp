/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_GUI_DOT_HPP_
#define _BLENDINT_GUI_DOT_HPP_

#include <BlendInt/Gui/AbstractRoundForm.hpp>
#include <BlendInt/OpenGL/GLArrayBuffer.hpp>

namespace BlendInt {

	/**
	 * @brief Dot icon used in ColorWheel or BrightnessSlider
	 */
	class Dot: public AbstractRoundForm
	{
	public:

		/**
		 * @brief Default constructor
		 */
		Dot ();

		/**
		 * @brief Constructor
		 */
		virtual ~Dot ();

		void Resize (unsigned int radius);

		/**
		 * @brief Draw the icon
		 */
		virtual void Draw (const glm::mat4& mvp);

	protected:

		virtual void UpdateGeometry (const UpdateRequest& request);

	private:

		GLuint m_vao;

		RefPtr<GLArrayBuffer> m_inner_buffer;
		RefPtr<GLArrayBuffer> m_outer_buffer;
	};

}

#endif /* _BLENDINT_GUI_DOT_HPP_ */
