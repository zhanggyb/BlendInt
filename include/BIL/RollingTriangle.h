/*
 * RollingTriangle.h
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#ifndef ROLLINGTRIANGLE_H_
#define ROLLINGTRIANGLE_H_

#include <BIL/Drawable.h>

namespace BIL {

	/**
	 * This object is for test only
	 */
	class RollingTriangle: public BIL::Drawable
	{
	public:
		RollingTriangle (BasicObject *parent = NULL);
		virtual ~RollingTriangle ();

	protected:
		void render (void);

	private:
		RollingTriangle (const RollingTriangle& orig);
		RollingTriangle& operator = (const RollingTriangle& orig);
	};

} /* namespace BIL */
#endif /* ROLLINGTRIANGLE_H_ */
