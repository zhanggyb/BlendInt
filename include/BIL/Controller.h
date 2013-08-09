/*
 * Controller.h
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <BIL/Drawable.h>

namespace BIL {

	// TODO: rename to Widget, confused to controller in MVC

	class Controller: public BIL::Drawable
	{
	public:
		Controller (BasicObject * parent = NULL);
		virtual ~Controller ();

	protected:
		virtual void render (void);
	};

} /* namespace BIL */
#endif /* CONTROLLER_H_ */
