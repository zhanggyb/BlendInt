/*
 * Drawable.cpp
 *
 *  Created on: 2013年7月1日
 *      Author: zhanggyb
 */

#include <algorithm>
#include <iostream>

#include "Drawable.h"

namespace BIL {

using namespace std;

Drawable::Drawable(Drawable* parent) :
		_parent(parent) {
	// TODO Auto-generated constructor stub
	if (_parent != NULL)
		_parent->addChild(this);
}

Drawable::~Drawable() {
	// TODO Auto-generated destructor stub
}

void Drawable::setParent(Drawable*& parent) {

}

bool Drawable::addChild(Drawable* child) {
	if (child == NULL) return false;
	list<Drawable*>::iterator it = std::find(_children.begin(), _children.end(), child);

	if(it != _children.end()) {
		return false;
	} else {
		_children.push_back(child);
	}

	child->_parent = this;

	return true;
}

bool Drawable::removeChild(Drawable* child) {
	return true;
}

bool Drawable::deleteChild(Drawable* child) {
	return true;
}

} /* namespace BIL */
