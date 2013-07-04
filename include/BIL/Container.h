/*
 * Container.h
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#ifndef _BIL_CONTAINER_H_
#define _BIL_CONTAINER_H_

#include <BIL/Drawable.h>

namespace BIL {

	class Container: public BIL::Drawable
	{
	public:
		Container (BasicObject *parent = NULL);
		virtual ~Container ();

	protected:
		void render (void);

	private:
		Container (const Container& orig);
		Container& operator = (const Container& orig);
	};

} /* namespace BIL */
#endif /* CONTAINER_H_ */
