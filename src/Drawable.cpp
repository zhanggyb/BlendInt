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

Drawable::Drawable (Drawable* parent)
		: _parent(parent)
{
	if (_parent != NULL) {
		(_parent->_children).insert(this);
	}
}

Drawable::~Drawable ()
{
	// delete all child object in list

	set<Drawable*>::iterator it;
	Drawable * item = NULL;

	for (it = _children.begin(); it != _children.end(); it++) {
		item = dynamic_cast<Drawable*>(*it);
		if (item != NULL) {
			delete item;
			item = NULL;
		}
	}

	_children.clear();

	if (_parent != NULL) {
		_parent->removeChild(this);
	}

#ifdef DEBUG
	std::cout << "Now object " << getId() << " will be deleted" << std::endl;
#endif
}

void Drawable::setParent (Drawable* parent)
{
	_parent = parent;

	if (_parent != NULL)
		_parent->addChild(this);

	return;
}

bool Drawable::addChild (Drawable* child)
{
	if (child == NULL)
		return false;

	if(child->_parent != NULL) {
		(child->_parent)->removeChild(child);
	}

	_children.insert(child);

	child->_parent = this;

	return true;
}

bool Drawable::removeChild (Drawable* child)
{
	if (child == NULL)
		return false;

	_children.erase(child);
	child->setParent(NULL);

	return true;
}

bool Drawable::deleteChild (Drawable* child)
{
	if (child == NULL)
		return false;

	removeChild(child);

	// TODO: confirm if should delete
	delete child;

	return true;
}

} /* namespace BIL */
