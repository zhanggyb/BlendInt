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

#ifndef _BIL_VERTICALLAYOUT_H_
#define _BIL_VERTICALLAYOUT_H_

#include <BIL/AbstractLayout.hpp>
#include <BIL/Types.hpp>

namespace BIL {

	class VerticalLayout: public AbstractLayout
	{
	public:

		VerticalLayout(Drawable* parent = 0, int align = AlignVerticalCenter);

		virtual ~VerticalLayout ();

	protected:

		virtual void update (int property);

		virtual void render ();

	private:

		DISALLOW_COPY_AND_ASSIGN(VerticalLayout);
	};

}

#endif	// _BIL_VERTICALLAYOUT_H_
