/*
 * GLDraw.h
 *
 *  Created on: 2013年7月8日
 *      Author: zhanggyb
 */

#ifndef _BIL_GLDRAW_H_
#define _BIL_GLDRAW_H_

#include <BIL/Drawable.h>

#include <BIL/Point.h>
#include <BIL/Rect.h>
#include <BIL/Size.h>
#include <BIL/Font.h>
#include <BIL/Color.h>

#include <string>

namespace BIL {

	/**
	 * C++ wrapper of some OpenGL functions
	 */
	class GLDraw
	{
	public:

		GLDraw (Drawable *obj = NULL);

		virtual ~GLDraw ();

		void setPointSize (GLfloat size);

		void setLineWidth (GLfloat width);

		void setLineStyle (GLfloat style);

		void setColor (const Color& color);

		void line (const Point2Di& from, const Point2Di& to);

		void line (const Point2Df& from, const Point2Df& to);

		void line (const Point3Di& from, const Point3Di& to);

		void line (const Point3Df& from, const Point3Df& to);

		void rect (const Rect2Di& rect);

		void rect (const Rect2Df& rect);

		void rect (GLint x, GLint y, GLint w, GLint h);

		void rect (GLfloat x, GLfloat y, GLfloat w, GLfloat h);

		void rect (const Point2Di& from, const Point2Di& to);

		void rect (const Point2Df& from, const Point2Df& to);

		void roundRect (const Rect2Df& rect, GLfloat r);

		void text (const std::string& text, Point3Df position);

		void setFontSize (GLfloat size);

		void setFont (const std::string& fontname);

	private:

		///This function gets the first power of 2 >= the
		///int that we pass it.
		inline int next_p2 (int a)
		{
			int rval = 1;
			while (rval < a)
				rval <<= 1;
			return rval;
		}

		void makeDisplayList (char ch, GLuint list_base, GLuint *tex_base);

		Drawable* _drawArea;

	};

} /* namespace BIL */
#endif /* GLDRAW_H_ */
