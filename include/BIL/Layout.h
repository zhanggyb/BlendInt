/*
 * Layout.h
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#ifndef LAYOUT_H_
#define LAYOUT_H_

#include <BIL/Drawable.h>
#include <BIL/Controller.h>

namespace BIL {

	class Layout: public BIL::Drawable
	{
	public:
		Layout (BasicObject *parent = NULL);
		virtual ~Layout ();

		virtual bool addController (Controller *obj) = 0;

	private:
		Layout (const Layout& orig);
		Layout& operator = (const Layout& orig);
	};

} /* namespace BIL */
#endif /* LAYOUT_H_ */
