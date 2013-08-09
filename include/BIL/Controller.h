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

/*
 * Controller.h
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <BIL/Drawable.h>

namespace BIL {

	// TODO: rename to Widget, confused to controller in MVC

	class Controller: public BIL::Drawable
	{
	public:
		Controller (BasicObject * parent = NULL);
		virtual ~Controller ();

	protected:
		virtual void render (void);
	};

} /* namespace BIL */
#endif /* CONTROLLER_H_ */
