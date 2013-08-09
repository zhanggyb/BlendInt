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

#ifndef LAYOUT_H_
#define LAYOUT_H_

#include <BIL/Drawable.h>
#include <BIL/Widget.h>

namespace BIL {

	class Layout: public BIL::Drawable
	{
	public:
		Layout (BasicObject *parent = NULL);
		virtual ~Layout ();

		virtual bool addController (Widget *obj) = 0;

	private:
		Layout (const Layout& orig);
		Layout& operator = (const Layout& orig);
	};

} /* namespace BIL */
#endif /* LAYOUT_H_ */
