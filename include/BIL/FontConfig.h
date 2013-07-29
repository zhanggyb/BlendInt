/*
 * FontManager.h
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#ifndef _BIL_FONTMANAGER_H_
#define _BIL_FONTMANAGER_H_

#include <map>
#include <string>
#include <iostream>

#include <fontconfig/fontconfig.h>

#include <BIL/FontType.h>
#include <BIL/Font.h>

using namespace std;

typedef map<string, BIL::FontType*> FontList;
typedef FontList::const_iterator FontIter;

namespace BIL {

	class FontConfig
	{
	public:

		/**
		 * @brief generate a FontConfig instance and assign to FontConfig::service
		 * @return true: success, false: already has instance
		 */
		static bool instance (void);

		/**
		 * @brief Release the object created in instance()
		 * @return true: success, false: already released
		 */
		static bool release (void);

		static FontConfig* getService (void)
		{
			return service;
		}

		bool initialize (void);

		bool isInitialized (void) const
		{
			return initialized;
		}

		const unsigned char* getBuffer (void) const
		{
			return _defaultFontBuf;
		}

		long getBufferSize (void) const
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
		string getFontPath (const string& family, float size = 10, bool bold =
		        false, bool italic = false);

		string getFontPath (const Font& font);

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
		virtual ~FontConfig ();

		bool loadFontFile (const string& file);

		/**
		 * @brief load all Truetype fonts in the directory
		 * @param path the target directory
		 *
		 * load all Truetype fonts (.ttf or .TTF) in the target directory
		 */
		void loadFontDir (const string& path);

		FontConfig& operator = (const FontConfig &orig);

	private:

		/**
		 * @brief The real object to provide font config service
		 */
		static FontConfig* service;

		// container for fonts
		FontList _fonts;

		map<string, FontType*> _namedb;

		static bool initialized;

		/** a buffer in memory for the default font file */
		unsigned char* _defaultFontBuf;

		long _defaultFontBufSize; /* size of the buffer */
	};

} /* namespace BIL */
#endif /* FONTMANAGER_H_ */
