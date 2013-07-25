/*
 * TextBuffer.h
 *
 *  Created on: 2013年7月12日
 *      Author: zhanggyb
 */

#ifndef _BIL_TEXTBUFFER_H_
#define _BIL_TEXTBUFFER_H_

#include <stddef.h>
#include <boost/array.hpp>
#include <string.h>
#include <vector>
#include <map>

#include <BIL/Tuple.h>
//#include <BIL/BasicObject.h>

using namespace boost;
using namespace std;

namespace BIL {

    class TextBuffer
    {
    public:

        /**
         * @brief Default Constructor
         *
         * create a new empty text buffer
         */
        TextBuffer ();

        /**/
        TextBuffer (const wstring& text);

        void append (const wstring& text);

        void append (wchar_t charcode);

        void setRowSpacing (int space);

        void render (void);     /* render the text */

        void renderAt (const Coord3f& pos);

        void clear (void);      /* clear the text */

        virtual ~TextBuffer ();

    private:

        struct CharData
        {
            GLuint texture;
            GLuint displist;
        };

        TextBuffer (const TextBuffer& orig);

        TextBuffer& operator = (const TextBuffer& orig);

        /**
         * @brief create display list for each character in the text
         */
        bool makeDisplayList (wchar_t charcode);

        ///This function gets the first power of 2 >= the
        ///int that we pass it.
        inline int next_p2 (int a)
        {
            int rval = 1;
            while (rval < a)
                rval <<= 1;
            return rval;
        }

        /** Character DB  */
        map<wchar_t, CharData> _chardb;

        map<wchar_t, unsigned int> _advancemap; /* each character's advance.x */

        /** Row spacing */
        float _rowspacing;

        /** Base color for text */
        Color _baseColor;

        /** Pen origin */
        Coord3f _origin;

        /** Index (in the vertex buffer) of the line start */
        size_t _lineStart;

        /** Current line ascender */
        GLfloat _lineAscender;

        /** Current line decender */
        GLfloat _lineDescender;

        /** Shader handler */

        /** Texture location */

        /** Pixel location */

        /** string */
        wstring _text;
    };

} /* namespace BIL */
#endif /* TEXTBUFFER_H_ */
