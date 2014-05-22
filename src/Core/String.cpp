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

#include <BlendInt/Types.hpp>
#include <BlendInt/Core/String.hpp>

namespace BlendInt {

#if __cplusplus <= 199711L

	String::String ()
	: std::basic_string<uint32_t>()
	{

	}

	String::String (const char* str)
	: std::basic_string<uint32_t>()
	{
		size_t len = strlen(str);
		resize(len);

		for(size_t i = 0; i < len; i++)
		{
			at(i) = *(str + i);
		}
	}

	String::String (const wchar_t* str)
	: std::basic_string<uint32_t>()
	{
		size_t len = wcslen(str);
		resize(len);

		for(size_t i = 0; i < len; i++)
		{
			at(i) = *(str + i);
		}
	}

	String::String (const char* str, size_t n)
	: std::basic_string<uint32_t>()
	{
		size_t len = strlen(str);
		len = std::min(len, n);

		resize(len);
		for(size_t i = 0; i < len; i++)
		{
			at(i) = *(str + i);
		}
	}

	String::String (const std::string& str)
	: std::basic_string<uint32_t>()
	{
		size_t len = str.length();
		resize(len);

		for(size_t i = 0; i < len; i++)
		{
			at(i) = str[i];
		}
	}

	String::String (const std::wstring& str)
	: std::basic_string<uint32_t>()
	{
		size_t len = str.length();
		resize(len);

		for(size_t i = 0; i < len; i++)
		{
			at(i) = str[i];
		}
	}

#else	// C++ 11

	String::String ()
	: std::u32string()
	{

	}

	String::String (const char* str)
	: std::u32string()
	{
		size_t len = strlen(str);
		resize(len);

		for(size_t i = 0; i < len; i++)
		{
			at(i) = *(str + i);
		}
	}

	String::String (const wchar_t* str)
	: std::u32string()
	{
		size_t len = wcslen(str);
		resize(len);

		for(size_t i = 0; i < len; i++)
		{
			at(i) = *(str + i);
		}
	}

	String::String (const char* str, size_t n)
	: std::u32string()
	{
		size_t len = strlen(str);
		len = std::min(len, n);

		resize(len);
		for(size_t i = 0; i < len; i++)
		{
			at(i) = *(str + i);
		}
	}

	String::String (const std::string& str)
	: std::u32string()
	{
		size_t len = str.length();
		resize(len);

		for(size_t i = 0; i < len; i++)
		{
			at(i) = str[i];
		}
	}

	String::String (const std::wstring& str)
	: std::u32string()
	{
		size_t len = str.length();
		resize(len);

		for(size_t i = 0; i < len; i++)
		{
			at(i) = str[i];
		}
	}

#endif

	String::String (const String& orig)
	{
		size_t len = orig.length();
		resize(len);

		memcpy (&at(0), orig.data(), sizeof(uint32_t) * (len + 1));
	}

	String& String::operator = (const char* str)
	{
		size_t len = strlen(str);
		resize(len);

		for(size_t i = 0; i < len; i++)
		{
			at(i) = *(str + i);
		}

		return *this;
	}

	String& String::operator = (const wchar_t* str)
	{
		size_t len = wcslen(str);
		resize(len);

		for(size_t i = 0; i < len; i++)
		{
			at(i) = *(str + i);
		}

		return *this;
	}

	String& String::operator = (const std::string& str)
	{
		size_t len = str.length();
		resize(len);

		for(size_t i = 0; i < len; i++)
		{
			at(i) = str[i];
		}

		return *this;
	}

	String& String::operator = (const std::wstring& str)
	{
		size_t len = str.length();
		resize(len);

		for(size_t i = 0; i < len; i++)
		{
			at(i) = str[i];
		}

		return *this;
	}

	String& String::operator = (const String& orig)
	{
		size_t len = orig.length();
		resize(len);

		memcpy (&at(0), orig.data(), sizeof(uint32_t) * (len + 1));

		return *this;
	}

	std::string ConvertFromString (const String& src)
	{
		std::string str;

		size_t len = src.length();
		str.resize(len);

		for(size_t i = 0; i < len; i++)
		{
			str[i] = src[i];
		}
		return str;
	}

}
