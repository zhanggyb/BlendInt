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

#include <BIL/String.hpp>

namespace BILO {

	String::String (const char* str)
	{
		for (const char* c = str; *c != '\0'; c++) {
			push_back(static_cast<wchar_t>(*c));
		}
	}

	String::String (const char* str, size_t n)
	{
		for (size_t i = 0; str[i] != '\0' && i < n; i++) {
			push_back(static_cast<wchar_t>(str[i]));
		}
	}

	String::String (const std::string& str)
	{
		for (std::string::const_iterator it = str.begin(); it != str.end();
		        it++) {
			push_back(static_cast<wchar_t>(*it));
		}
	}

	String& String::operator = (const char* str)
	{
		clear();

		for (const char* c = str; *c != '\0'; c++) {
			push_back(static_cast<wchar_t>(*c));
		}

		return *this;
	}

	String& String::operator = (const std::string& str)
	{
		clear();

		for (std::string::const_iterator it = str.begin(); it != str.end();
		        it++) {
			push_back(static_cast<wchar_t>(*it));
		}

		return *this;
	}

	std::string ConvertFromString (const String& src)
	{
		std::string str;
		for (String::const_iterator it = src.begin(); it != src.end(); it++)
		{
			str.push_back(static_cast<char>(*it));
		}
		return str;
	}

}
