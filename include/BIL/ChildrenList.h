/*
 * ChildrenList.h
 *
 *  Created on: 2013年7月2日
 *      Author: zhanggyb
 */

#ifndef _BIL_CHILDRENLIST_H_
#define _BIL_CHILDRENLIST_H_

#include <stddef.h>
#include <set>
#include <list>
#include <algorithm>

namespace BIL {

template<typename T>
struct _List_node
{
	_List_node* _prev;
	_List_node* _next;
	T _data;
};

template<typename T>
struct _List_iterator
{
	_List_node<T> *_node;

	_List_iterator ()
			: _node()
	{
	}

	T& operator* () const
	{
		return _node->_data;
	}

	T* operator-> () const
	{
		return &(_node->_data);
	}

	_List_iterator<T>& operator++ ()
	{
		_node = _node->_next;
		return *this;
	}

	_List_iterator<T>& operator-- ()
	{
		_node = _node->_prev;
		return *this;
	}

	bool operator == (const _List_iterator<T>& x) const
	{
		return _node == x._node;
	}

	bool operator != (const _List_iterator<T>& x) const
	{
		return _node != x._node;
	}
};

/**
 * class to store children in BasicObject
 */
template<typename T>
class ChildrenList
{
public:

	ChildrenList ();

	virtual ~ChildrenList ();

	T operator [] (int n);

	bool push_front (const T& val);

	bool push_front (T& val);

	bool push_back (const T& val);

	bool push_back (T& val);

	void pop_front (void);

	void pop_back (void);

	bool find (const T& val);

	int size (void) const;

private:

	/**
	 *  Disable copy constructor
	 * @param orig
	 */
	ChildrenList (const ChildrenList<T>& orig);

	/**
	 * Disable assignment constructor
	 * @param orig
	 * @return
	 */
	ChildrenList<T>& operator = (const ChildrenList<T>& orig);

	std::list<T> _list; /** ordered list */
	std::set<T> _set; /** for fast search */
};

template<typename T>
ChildrenList<T>::ChildrenList ()
{
	// TODO Auto-generated constructor stub

}

template<typename T>
ChildrenList<T>::~ChildrenList ()
{
	// TODO Auto-generated destructor stub
	_list.clear();
	_set.clear();
}

template<typename T>
inline T BIL::ChildrenList<T>::operator [] (int n)
{
	return _list[n];
}

template<typename T>
inline bool BIL::ChildrenList<T>::push_front (const T& val)
{
	if (!find(val)) {
		_list.push_front(val);
		_set.insert(val);
		return true;
	} else {
		return false;
	}
}

template<typename T>
inline bool BIL::ChildrenList<T>::push_front (T& val)
{
	if (!find(val)) {
		_list.push_front(val);
		_set.insert(val);
		return true;
	} else {
		return false;
	}
}

template<typename T>
inline bool BIL::ChildrenList<T>::push_back (const T& val)
{
	if (!find(val)) {
		_list.push_back(val);
		_set.insert(val);
		return true;
	} else {
		return false;
	}
}

template<typename T>
inline bool BIL::ChildrenList<T>::push_back (T& val)
{
	if (!find(val)) {
		_list.push_back(val);
		_set.insert(val);
		return true;
	} else {
		return false;
	}
}

template<typename T>
inline void BIL::ChildrenList<T>::pop_front (void)
{
	if (size() > 0) {
		T front = _list.front();
		_set.remove(front);
		_list.pop_front();
	}
}

template<typename T>
inline void BIL::ChildrenList<T>::pop_back (void)
{
	if (size() > 0) {
		T back = _list.back();
		_set.erase(back);
		_list.pop_back();
	}
}

template<typename T>
inline bool BIL::ChildrenList<T>::find (const T& val)
{
	typename std::set<T>::iterator it;

	it = _set.find(val);

	if (it == _set.end()) {
		return false;
	} else {
		return true;
	}
}

template<typename T>
inline int BIL::ChildrenList<T>::size (void) const
{
	int listsize, setsize;

	listsize = _list.size();
	setsize = _set.size();

	if (listsize != setsize)
		return -1;

	return listsize;
}

} /* namespace BIL */

#endif /* _BIL_CHILDRENLIST_H_ */
