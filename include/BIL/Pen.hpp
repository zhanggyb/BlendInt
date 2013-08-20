/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_PEN_H_
#define _BIL_PEN_H_

#include <GL/gl.h>
#include <BIL/Font.hpp>
#include <BIL/Tuple.hpp>
#include <BIL/Coord.hpp>
#include <BIL/Rect.hpp>

#include <string>

namespace BIL {

	/**
	 * C++ wrapper of some OpenGL functions
	 */
	class Pen
	{
	public:

		Pen (float size = 1.0, float width = 1.0);

		virtual ~Pen ();

		void setPointSize (GLfloat size);

		void setLineWidth (GLfloat width);

		void setLineStyle (GLfloat style);

		/**
		 * @brief Draw line from s to t
		 */
		void draw (const Coord2f& s, const Coord2f& t);

		/**
		 * @brief Draw rectangle
		 */
		void draw (const Rectf& rect);

	private:

		/** point size */
		float _size;

		/** line width */
		float _width;
	};

} /* namespace BIL */
#endif /* PEN_H_ */
