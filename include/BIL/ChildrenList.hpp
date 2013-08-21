/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

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

	/**
	 * class to store children in BasicObject
	 */
	template<typename T>
	class ChildrenList
	{
	public:

		typedef typename std::list<T>::iterator iterator;
		typedef typename std::list<T>::const_iterator const_iterator;
		typedef typename std::list<T>::reverse_iterator reverse_iterator;
		typedef typename std::list<T>::const_reverse_iterator const_reverse_iterator;

		ChildrenList ();

		virtual ~ChildrenList ();

		iterator begin ()
		{
			return list_.begin();
		}

		 const_iterator begin () const
		 {
		 return list_.begin();
		 }

		iterator end ()
		{
			return list_.end();
		}

		 const_iterator end () const
		 {
		 return list_.end();
		 }

		 reverse_iterator rbegin ()
		 {
			 return list_.rbegin();
		 }

		 const_reverse_iterator rbegin () const
		 {
			 return list_.rbegin();
		 }

		 /**
		  * @brief Return reverse iterator to reverse end
		  * @return
		  *
		  * Returns a reverse iterator pointing to the theoretical element
		  * preceding the first element in the list container
		  */
		 reverse_iterator rend ()
		 {
			 return list_.rend();
		 }

		 const_reverse_iterator rend () const
		 {
			 return list_.rend();
		 }

		T operator [] (int n);

		T& front ()
		{
			return list_.front();
		}

		T& back ()
		{
			return list_.back();
		}

		bool push_front (const T& val);

		bool push_front (T& val);

		bool push_back (const T& val);

		bool push_back (T& val);

		void pop_front ();

		void pop_back ();

		bool find (const T& val);

		void erase (const T& val);

		void clear ();

		int size () const;

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

		std::list<T> list_; /** ordered list */
		std::set<T> set_; /** for fast search */
	};

	template<typename T>
	ChildrenList<T>::ChildrenList ()
	{
		// TODO Auto-generated constructor stub

	}

	template<typename T>
	ChildrenList<T>::~ChildrenList ()
	{
		list_.clear();
		set_.clear();
	}

	template<typename T>
	inline T BIL::ChildrenList<T>::operator [] (int n)
	{
		return list_[n];
	}

	template<typename T>
	inline bool BIL::ChildrenList<T>::push_front (const T& val)
	{
		if (!find(val)) {
			list_.push_front(val);
			set_.insert(val);
			return true;
		} else {
			return false;
		}
	}

	template<typename T>
	inline bool BIL::ChildrenList<T>::push_front (T& val)
	{
		if (!find(val)) {
			list_.push_front(val);
			set_.insert(val);
			return true;
		} else {
			return false;
		}
	}

	template<typename T>
	inline bool BIL::ChildrenList<T>::push_back (const T& val)
	{
		if (!find(val)) {
			list_.push_back(val);
			set_.insert(val);
			return true;
		} else {
			return false;
		}
	}

	template<typename T>
	inline bool BIL::ChildrenList<T>::push_back (T& val)
	{
		if (!find(val)) {
			list_.push_back(val);
			set_.insert(val);
			return true;
		} else {
			return false;
		}
	}

	template<typename T>
	inline void BIL::ChildrenList<T>::pop_front ()
	{
		if (size() > 0) {
			T front = list_.front();
			set_.erase(front);
			list_.pop_front();
		}
	}

	template<typename T>
	inline void BIL::ChildrenList<T>::pop_back ()
	{
		if (size() > 0) {
			T back = list_.back();
			set_.erase(back);
			list_.pop_back();
		}
	}

	template<typename T>
	inline bool BIL::ChildrenList<T>::find (const T& val)
	{
		return set_.count(val) > 0 ? true : false;
	}

	template<typename T>
	inline void ChildrenList<T>::erase (const T& val)
	{
		set_.erase(val);
		list_.remove(val);
	}

	template<typename T>
	inline void ChildrenList<T>::clear ()
	{
		set_.clear();
		list_.clear();
	}

	template<typename T>
	inline int BIL::ChildrenList<T>::size () const
	{
		int listsize, setsize;

		listsize = list_.size();
		setsize = set_.size();

		if (listsize != setsize)
			return -1;

		return listsize;
	}

} /* namespace BIL */

#endif /* _BIL_CHILDRENLIST_H_ */
