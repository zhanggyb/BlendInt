/*
 * Drawable.h
 *
 *  Created on: 2013年7月1日
 *      Author: zhanggyb
 */

#ifndef _BIL_DRAWABLE_H_
#define _BIL_DRAWABLE_H_

#include <list>
#include <stddef.h>

#include "BasicObject.h"

namespace BIL {

class Drawable: public BIL::BasicObject {
public:
	Drawable(Drawable* parent = NULL);
	virtual ~Drawable();

	const Drawable* getParent(void) const {
		return _parent;
	}

	void setParent(Drawable*& parent);

	bool addChild (Drawable* child);

	bool removeChild (Drawable* child);

protected:

	bool deleteChild (Drawable* child);

	Drawable * _parent;

	std::list<Drawable*> _children;

private:

	Drawable (const Drawable& orig);
	Drawable& operator = (const Drawable& orig);

};

} /* namespace BIL */
#endif /* _BIL_DRAWABLE_H_ */
