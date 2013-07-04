/*
 * AbstractButton.h
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#ifndef _BIL_ABSTRACTBUTTON_H_
#define _BIL_ABSTRACTBUTTON_H_

#include <BIL/Controller.h>

namespace BIL {

	class AbstractButton: public BIL::Controller
	{
	public:

		AbstractButton (BasicObject * parent = NULL);
		virtual ~AbstractButton ();

	protected:

		void render (void);

	private:
		AbstractButton (const AbstractButton& orig);
		AbstractButton& operator = (const AbstractButton& orig);

	};

} /* namespace BIL */
#endif /* ABSTRACTBUTTON_H_ */
