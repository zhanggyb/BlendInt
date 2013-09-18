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

#ifndef ROLLINGTRIANGLE_H_
#define ROLLINGTRIANGLE_H_

#include <BILO/Drawable.hpp>

namespace BILO {

	/**
	 * This object is for test only
	 */
	class RollingTriangle: public BILO::Drawable
	{
	public:
		RollingTriangle (Drawable *parent = 0);
		virtual ~RollingTriangle ();

	protected:
		virtual void Render ();

	private:
		RollingTriangle (const RollingTriangle& orig);
		RollingTriangle& operator = (const RollingTriangle& orig);
	};

} /* namespace BILO */
#endif /* ROLLINGTRIANGLE_H_ */
