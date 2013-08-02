/*
 * Drawable.cpp
 *
 *  Created on: 2013年7月1日
 *      Author: zhanggyb
 */

#include <algorithm>
#include <iostream>

#include <BIL/Drawable.h>

namespace BIL {

	using namespace std;

	Drawable::Drawable (BasicObject* parent)
			: BasicObject(parent)
	{

	}

	Drawable::~Drawable ()
	{
		// delete all child object in list

	}

	void Drawable::resize (const Vec2ui& size)
	{
		_size = size;
		render();
		return;
	}

} /* namespace BIL */

