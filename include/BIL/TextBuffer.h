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

#include <BIL/VertexBuffer.h>
#include <BIL/Color.h>
#include <BIL/Point.h>
//#include <BIL/BasicObject.h>

using namespace boost;
using namespace std;

namespace BIL {

    /**
     * Glyph vertex structure
     */
    struct GlyphVertex {

        /** Vertex x coordinates */
        float x;

        /** Vertex y coordinates */
        float y;

        /** Vertex z coordinates */
        float z;

        /** Texture first coordinate */
        float u;

        /** Texture second coordinate */
        float v;

        /** Color red component */
        float r;

        /** Color green component */
        float g;

        /** Color blue component */
        float b;

        /** Color alpha component */
        float a;

        /** Shift along x */
        float shift;

        /** Color gamma correction */

        float gamma;
    } ;

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

        void render (void);     /* render the text */

        void clear (void);      /* clear the text */

        virtual ~TextBuffer ();

    private:

        TextBuffer (const TextBuffer& orig);

        TextBuffer& operator = (const TextBuffer& orig);

        /**
         * @brief create display list for each character in the text
         */
        bool makeDisplayList (wchar_t charcode,
                              GLuint list_base,
                              GLuint* tex_base);

        /** Texture ids */
        GLuint *_textures;

        /** the first display list id */
        GLuint _displist;

        /** Base color for text */
        Color _baseColor;

        /** Pen origin */
        Point3Df _origin;

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

        VertexBuffer _buffer;
    };

} /* namespace BIL */
#endif /* TEXTBUFFER_H_ */
