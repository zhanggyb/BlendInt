/*
 * TextBuffer.h
 *
 *  Created on: 2013年7月12日
 *      Author: zhanggyb
 */

#ifndef _BIL_TEXTBUFFER_H_
#define _BIL_TEXTBUFFER_H_

#include <GL/glew.h>
 #include <GL/gl.h>

#include <stddef.h>
#include <boost/array.hpp>

#include <BIL/VertexBuffer.h>
#include <BIL/Color.h>
#include <BIL/Point.h>

using namespace boost;

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

		TextBuffer ();
		
		virtual ~TextBuffer ();

	private:

		VertexBuffer _buffer;

		// Font

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
	};

} /* namespace BIL */
#endif /* TEXTBUFFER_H_ */
