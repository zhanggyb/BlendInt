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

#ifndef _BIL_FONT_H_
#define _BIL_FONT_H_

#include <BIL/String.hpp>

using namespace std;

namespace BILO {

	struct Font;

	// friend function
	extern bool operator < (const Font& src, const Font& dist);
	extern bool operator == (const Font& src, const Font& dist);

	struct Font
	{
		Font (const String& family = String("Sans"),
				unsigned int size = 9,
        bool bold = false,
        bool italic = false);

		/*
		Font (const std::string& family,
				unsigned int size = 9,
        bool bold = false,
        bool italic = false);
		*/

		Font (const wchar_t* family, unsigned int size = 9, bool bold = false, bool italic = false);

		Font (const char* family, unsigned int size = 9, bool bold = false, bool italic = false);

		Font (const Font& orig);

		Font& operator = (const Font& orig);

		/**
		 * @brief the font family, e.g. "Droid Sans"
		 *
		 * @note Currently cannot support non-English family
		 */
		String family;

		/** font size */
		unsigned int size;

		/** whether text is bold */
		bool bold;

		/** whether text is italic */
		bool italic;
	};

} /* namespace BIL */
#endif /* FONT_H_ */
