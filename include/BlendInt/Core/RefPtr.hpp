/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_REFPTR_HPP_
#define _BLENDINT_REFPTR_HPP_

#include <stdexcept>

namespace BlendInt {

	template <typename T>
	class RefPtr
	{
	public:

		inline RefPtr ();

		explicit inline RefPtr (T* obj);

		RefPtr (const RefPtr<T>& orig);

		inline ~RefPtr ();

		inline RefPtr<T>& operator = (const RefPtr<T>& orig);

		inline void destroy ();

		inline RefPtr<T>& swap (RefPtr<T>& other);

		inline T* operator-> () const;

		inline T& operator* () const;

		inline bool operator== (const RefPtr<T>& src) const;

		inline bool operator!= (const RefPtr<T>& src) const;

		inline operator bool() const;

		inline bool operator< (const RefPtr<T>& src) const;

		inline bool operator<= (const RefPtr<T>& src) const;

		inline bool operator> (const RefPtr<T>& src) const;

		inline bool operator>= (const RefPtr<T>& src) const;

	private:
		T* m_ptr;
	};

	template <typename T> inline
	RefPtr<T>::RefPtr ()
	: m_ptr(0)
	{
	}

	template <typename T> inline
	RefPtr<T>::RefPtr (T* obj)
	: m_ptr(obj)
	{
		if(m_ptr)
			++m_ptr->m_count;
	}

	template <typename T> inline
	RefPtr<T>::RefPtr(const RefPtr<T>& orig)
	: m_ptr(orig.m_ptr)
	{
		if(m_ptr)
			++m_ptr->m_count;
	}

	template <typename T> inline
	RefPtr<T>::~RefPtr()
	{
		destroy();
	}

	template <typename T> inline
	RefPtr<T>& RefPtr<T>::operator =(const RefPtr<T>& orig)
	{
		if(m_ptr) {

			T* const old = m_ptr;
			m_ptr = orig.m_ptr;
			++m_ptr->m_count;

			if (--old->m_count == 0)
				delete old;

		} else {

			m_ptr = orig.m_ptr;
			++m_ptr->m_count;
		}

		return *this;
	}

	template <typename T> inline
	void RefPtr<T>::destroy ()
	{
		if(m_ptr) {
			if(--m_ptr->m_count == 0)
				delete m_ptr;

			m_ptr = 0;
		}
	}

	template <typename T> inline
	RefPtr<T>& RefPtr<T>::swap (RefPtr<T>& other)
	{
		T* const temp = m_ptr;
		m_ptr = other.m_ptr;
		other.m_ptr = temp;
	}

	template <typename T> inline
	T* RefPtr<T>::operator-> () const
	{
		if(!m_ptr) {
			throw std::runtime_error("No object stored!");
		}

		return m_ptr;
	}

	template <typename T> inline
	T& RefPtr<T>::operator* () const
	{
		if(!m_ptr) {
			throw std::runtime_error("No object stored!");
		}

		return *m_ptr;
	}

	template <typename T> inline
	bool RefPtr<T>::operator== (const RefPtr<T>& src) const
	{
		return (m_ptr == src.m_ptr);
	}

	template <typename T> inline
	bool RefPtr<T>::operator!= (const RefPtr<T>& src) const
	{
		return (m_ptr != src.m_ptr);
	}

	template <typename T> inline
	RefPtr<T>::operator bool() const
	{
		return (m_ptr != 0);
	}

	template <typename T> inline
	bool RefPtr<T>::operator< (const RefPtr<T>& src) const
	{
		return (m_ptr < src.m_ptr);
	}

	template <typename T> inline
	bool RefPtr<T>::operator<= (const RefPtr<T>& src) const
	{
		return (m_ptr <= src.m_ptr);
	}

	template <typename T> inline
	bool RefPtr<T>::operator> (const RefPtr<T>& src) const
	{
		return (m_ptr > src.m_ptr);
	}

	template <typename T> inline
	bool RefPtr<T>::operator>= (const RefPtr<T>& src) const
	{
		return (m_ptr >= src.m_ptr);
	}

}

#endif /* _BLENDINT_REFPTR_HPP_ */
