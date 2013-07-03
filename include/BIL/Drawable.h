/*
 * Drawable.h
 *
 *  Created on: 2013年7月1日
 *      Author: zhanggyb
 */

#ifndef _BIL_DRAWABLE_H_
#define _BIL_DRAWABLE_H_

#include <set>
#include <stddef.h>

#include <BIL/BasicObject.h>

namespace BIL {

	class Drawable: public BIL::BasicObject
	{
	public:
		Drawable (Drawable* parent = NULL);
		virtual ~Drawable ();

	private:

		Drawable (const Drawable& orig);
		Drawable& operator = (const Drawable& orig);

	};

} /* namespace BIL */
#endif /* _BIL_DRAWABLE_H_ */
