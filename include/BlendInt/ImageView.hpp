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

#ifndef _BLENDINT_IMAGEVIEW_HPP_
#define _BLENDINT_IMAGEVIEW_HPP_

#include <BlendInt/Frame.hpp>

namespace BlendInt {

	class ImageView: public Widget
	{
	public:

		ImageView ();

		ImageView (AbstractWidget* parent);

		virtual ~ImageView ();

	protected:

		virtual void Render ();

	private:

		void makeCheckImage ();

		static const int checkImageWidth = 64;
		static const int checkImageHeight = 64;

		GLubyte _checkImage[checkImageHeight][checkImageWidth][4];

	};
}


#endif /* _BIL_IMAGEVIEW_HPP_ */
