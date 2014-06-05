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

#ifndef _BLENDINT_ICON_HPP_
#define _BLENDINT_ICON_HPP_

#include <glm/mat4x4.hpp>
#include <BlendInt/Gui/AbstractForm.hpp>

namespace BlendInt {

	class StockItems;

	class Icon: public AbstractForm
	{
	public:

		Icon ();

		Icon (const Icon& orig);

		virtual ~Icon ();

		Icon& operator = (const Icon& orig);

		void Scale (float ratio);

		virtual void Draw (const glm::mat4& mvp, int x, int y, int restrict_width, int restrict_height) = 0;

	protected:

		virtual void UpdateGeometry (const UpdateRequest& request);

	};

}

#endif /* _BLENDINT_ICON_HPP_ */
