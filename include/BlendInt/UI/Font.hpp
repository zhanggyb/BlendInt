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

#ifndef _BLENDINT_FONT_HPP_
#define _BLENDINT_FONT_HPP_

#include <BlendInt/String.hpp>

using namespace std;

namespace BlendInt {

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

} /* namespace BlendInt */
#endif /* FONT_H_ */
