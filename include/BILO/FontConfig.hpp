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

#ifndef _BILO_FONTMANAGER_HPP_
#define _BILO_FONTMANAGER_HPP_

#include <map>
#include <string>
#include <iostream>

#include <fontconfig/fontconfig.h>

#include <BILO/Freetype.hpp>
#include <BILO/Font.hpp>

using namespace std;

typedef map<string, BILO::Freetype*> FontList;
typedef FontList::const_iterator FontIter;

namespace BILO {

	class Interface;

	class FontConfig
	{
	public:

		friend class Interface;

		/**
		 * @brief generate a FontConfig instance and assign to FontConfig::service
		 * @return true: success, false: already has instance
		 */
		static FontConfig* instance ();

		const unsigned char* getBuffer () const
		{
			return _defaultFontBuf;
		}

		long getBufferSize () const
		{
			return _defaultFontBufSize;
		}

		/**
		 * @brief load truetype font into memory
		 * @param family font family
		 * @return true for success, false for error
		 */
		bool loadDefaultFontToMem (const string& family = string("Sans"));

		bool loadFont (const Font& font);

		/**
		 * @brief get font path
		 * @param family
		 * @return
		 *
		 * get font path with fontconfig
		 */
		string getFontPath (const string& family, float size = 9, bool bold =
		        false, bool italic = false);

		string getFontPath (const Font& font);

#ifdef DEBUG
	public:
#else
	private:
#endif

		static bool initialize ();

		/**
		 * @brief Release the object created in instance()
		 */
		static void release ();

	private:

		/**
		 * @brief Default constructor
		 *
		 * Move the default constructor to private
		 */
		FontConfig ()
				: _defaultFontBuf(NULL), _defaultFontBufSize(0)
		{
		}

		/**
		 * @brief Private destructor
		 *
		 * Move destructor to private area to disallow simply delete,
		 * use release() instead
		 */
		~FontConfig ();

		bool loadFontFile (const string& file);

		/**
		 * @brief load all Truetype fonts in the directory
		 * @param path the target directory
		 *
		 * load all Truetype fonts (.ttf or .TTF) in the target directory
		 */
		void loadFontDir (const string& path);

		FontConfig& operator = (const FontConfig &orig);

		/**
		 * @brief The real object to provide font config service
		 */
		static FontConfig* service;

		// container for fonts
		FontList _fonts;

		map<string, Freetype*> _namedb;

		/** a buffer in memory for the default font file */
		unsigned char* _defaultFontBuf;

		long _defaultFontBufSize; /* size of the buffer */
	};

} /* namespace BILO */
#endif /* FONTMANAGER_H_ */
