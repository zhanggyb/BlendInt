/*
 * Pen.h
 *
 *  Created on: 2013年7月8日
 *      Author: zhanggyb
 */

#ifndef _BIL_PEN_H_
#define _BIL_PEN_H_

#include <BIL/Font.h>
#include <BIL/Tuple.h>

#include <string>

namespace BIL {

	/**
	 * C++ wrapper of some OpenGL functions
	 */
	class Pen
	{
	public:

		Pen (float size = 1.0, float width = 1.0);

		virtual ~Pen ();

		void setPointSize (GLfloat size);

		void setLineWidth (GLfloat width);

		void setLineStyle (GLfloat style);

		/**
		 * @brief Draw line from s to t
		 */
		void draw (const Coord2f& s, const Coord2f& t);

		/**
		 * @brief Draw rectangle
		 */
		void draw (const Rectf& rect);

		/**
		 * @brief Draw round rectangle
		 */
		void draw (const Rectf& rect, const Vec4f round);

	private:

		/** point size */
		float _size;

		/** line width */
		float _width;
	};

} /* namespace BIL */
#endif /* PEN_H_ */
