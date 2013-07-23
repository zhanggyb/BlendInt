/*
 * FontCache.h
 *
 *  Created on: 2013年7月16日
 *      Author: zhanggyb
 */

#ifndef _BIL_FONTCACHE_H_
#define _BIL_FONTCACHE_H_

#include <GL/glew.h>
#include <GL/gl.h>

#include <map>
#include <vector>
#include <wchar.h>

using namespace std;

namespace BIL {

    /**
     * @brief Class in charge of caching fonts
     */
    class FontCache
    {
    public:

        static FontCache* gFontCache;

        static FontCache* instance (void);

        bool initialize (void);

        bool isInitialized (void) const {
            return initialized;
        }

        void setCacheSize (unsigned int size)
        {
            cacheSize = size;
        }

        virtual ~FontCache ();

    private:

        struct FontData {
            GLuint texture;
            GLuint displist;
        };

        FontCache ();

        FontCache (const FontCache& orige);

        FontCache& operator = (const FontCache& orig);

        map<wchar_t, FontData> _fontdb;

        static bool initialized;

        static unsigned int cacheSize;

    };

} /* namespace BIL */
#endif /* FONTCACHE_H_ */
