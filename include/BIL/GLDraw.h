/*
 * GLDraw.h
 *
 *  Created on: 2013年7月8日
 *      Author: zhanggyb
 */

#ifndef _BIL_GLDRAW_H_
#define _BIL_GLDRAW_H_

#include <BIL/Drawable.h>

namespace BIL {

	/**
	 * C++ wrapper of some OpenGL functions
	 */
	class GLDraw
	{
	public:
		GLDraw (Drawable *area = NULL);
		virtual ~GLDraw ();

	private:
		Drawable* _area;
	};

} /* namespace BIL */
#endif /* GLDRAW_H_ */
