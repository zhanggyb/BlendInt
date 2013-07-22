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

    class FontManager
    {
    public:

        static FontManager* gFontService;

        static FontManager* instance (void)
        {
            if (gFontService != NULL) {
                cerr << "Error: FontManager should generate only one instance" << endl;
                return NULL;
            }

            FontManager* tm = new FontManager;
            return tm;
        }

        virtual ~FontManager ();

        bool initialize (void);

#ifdef DEBUG
        void printfonts (void);
#endif

        const unsigned char* getBuffer (void) const {
            return _buf;
        }

        long getBufferSize (void) const {
            return _bufsize;
        }

        /**
         * @brief load truetype font into memory
         * @param family font family
         * @return true for success, false for error
         */
        bool loadFont (const string& family = string("Sans"));

        bool loadFont (const Font& font);

        /**
         * @brief get font path
         * @param family
         * @return
         *
         * get font path with fontconfig
         */
        string getFontPath (const string& family,
                            float size = 10,
                            bool bold = false,
                            bool italic = false);

        string getFontPath (const Font& font);

    private:

        /**
         * @brief Default constructor
         *
         * Move the default constructor to private
         */
        FontManager ()
            : _buf(NULL), _bufsize(0)
        {
        }

        bool loadFontFile (const string& file);

        /**
         * @brief load all Truetype fonts in the directory
         * @param path the target directory
         *
         * load all Truetype fonts (.ttf or .TTF) in the target directory
         */
        void loadFontDir (const string& path);

        FontManager& operator = (const FontManager &orig);

        // container for fonts
        FontList _fonts;

        map<string, FontType*> _namedb;

        static bool initialized;

        unsigned char* _buf;    /* a buffer in memory for the default
                                   font file */

        long _bufsize;          /* size of the buffer */
    };

} /* namespace BIL */
#endif /* FONTMANAGER_H_ */
