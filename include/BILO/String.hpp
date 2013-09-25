/*
 * This file is part of BILO (Blender-like Interface Library in
 * OpenGL).
 *
 * BILO (Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_STRING_HPP_
#define _BIL_STRING_HPP_

#include <string>
#include <cwchar>

namespace BILO {

	class String;

	extern std::string ConvertFromString (const String& src);

	class String: public std::wstring
	{
	public:

		String ()
				: std::wstring()
		{
		}

		String (const char* str);

		String (const wchar_t* str)
				: std::wstring(str)
		{
		}

		String (const char* str, size_t n);

		String (const wchar_t* str, size_t n)
				: std::wstring(str, n)
		{
		}

		String (const std::string& str);

		String (const std::wstring& str)
				: std::wstring(str)
		{
		}

		String (const String& orig)
		{
			assign(orig);
		}

		String& operator = (const char* str);

		String& operator = (const std::string& str);

	};
}

#endif /* _BIL_STRING_HPP_ */
