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

#ifndef _BIL_CONTAINER_H_
#define _BIL_CONTAINER_H_

#include <BIL/Drawable.hpp>

namespace BIL {

	class Container: public BIL::Drawable
	{
	public:
		Container (Traceable *parent = NULL);
		virtual ~Container ();

	protected:

		virtual void Render ();

	private:
		Container (const Container& orig);
		Container& operator = (const Container& orig);
	};

} /* namespace BIL */
#endif /* CONTAINER_H_ */
