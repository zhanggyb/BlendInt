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

#include <BIL/Drawable.hpp>
#include <BIL/Widget.hpp>

namespace BIL {

	class AbstractLayout: public Drawable
	{
	public:

		AbstractLayout (Drawable *parent = NULL);

		virtual ~AbstractLayout ();

		bool addWidget (Widget* widget);

		bool addLayout (AbstractLayout* layout);

	protected:

		void update () = 0;

		void render () = 0;

	private:

		DISALLOW_COPY_AND_ASSIGN(AbstractLayout);
	};

} /* namespace BIL */
#endif /* LAYOUT_H_ */
