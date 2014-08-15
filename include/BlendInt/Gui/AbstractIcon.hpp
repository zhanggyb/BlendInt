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

#ifndef _BLENDINT_GUI_ICON_HPP_
#define _BLENDINT_GUI_ICON_HPP_

#include <glm/vec3.hpp>
#include <BlendInt/Core/Object.hpp>
#include <BlendInt/Core/Size.hpp>

namespace BlendInt {

	class AbstractIcon: public Object
	{
	public:

		AbstractIcon (int width, int height);

		virtual ~AbstractIcon ();

		void Draw (float x, float y, short gamma = 0);

		virtual void Draw (const glm::vec3& pos, short gamma = 0) const = 0;

		const Size& size() const
		{
			return size_;
		}

	protected:

		void set_size (int w, int h)
		{
			size_.reset(w, h);
		}

		void set_size (const Size& size)
		{
			size_ = size;
		}

	private:

		Size size_;

	};

}

#endif /* _BLENDINT_GUI_ICON_HPP_ */
