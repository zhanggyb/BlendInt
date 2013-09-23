/*
 * This file is part of BILO (Blender Interface Library).
 *
 * BILO (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_IMAGEVIEW_HPP_
#define _BIL_IMAGEVIEW_HPP_

#include <BILO/Widget.hpp>

namespace BILO {

	class ImageView: public Widget
	{
	public:

		ImageView ();

		ImageView (Drawable* parent);

		virtual ~ImageView ();

	protected:

		virtual void render ();

	private:

		void makeCheckImage ();

		static const int checkImageWidth = 64;
		static const int checkImageHeight = 64;

		GLubyte _checkImage[checkImageHeight][checkImageWidth][4];

	};
}


#endif /* _BIL_IMAGEVIEW_HPP_ */
