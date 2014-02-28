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

	/**
	 * @brief Reference-counting shared smart pointer.
	 *
	 * RefPtr<> is a simplified reference-counting shared smart
	 * pointer, can store any class that inherits from
	 * BlendInt::Object.
	 *
	 * Reference counting means that a shared reference count is
	 * incremented each time a RefPtr is copied, and decremented each
	 * time a RefPtr is destroyed, for instance when it leaves its
	 * scope. When the reference count reaches zero, the contained
	 * object is deleted, meaning you don't need to remember to delete
	 * the object.
	 *
	 * See http://www.parashift.com/c++-faq/ref-count-simple.html, the
	 * source code in gtkmm (www.gtkmm.org), and Memory management
	 * page on
	 * https://developer.gnome.org/gtkmm-tutorial/stable/chapter-memory.html
	 * for more information.
	 */
	template <typename T>
	class RefPtr
	{
	public:

		/**
		 * @brief Default constructor
		 *
		 * Afterwards it will store a null pointer so use of -> or *
		 * will throw a run-time exception and cause a segmentation
		 * fault
		 */
		inline RefPtr ();

		/**
		 * @brief Constructor
		 * @param obj The instance of an Object created in heap by new
		 *
		 * Create a RefPtr with an instance of Object or sub classes.
		 *
		 * @note The parameter obj MUST be created in heap by new.
		 */
		explicit inline RefPtr (T* obj);

		/**
		 * @brief Copy constructor
		 * @param orig A RefPtr already exists
		 *
		 * Create from a already existing RefPtr, this will increase the
		 * shared reference count of the object stored.
		 */
		RefPtr (const RefPtr<T>& orig);

		template <typename T_CastFrom>
		inline RefPtr(const RefPtr<T_CastFrom>& orig);

		/**
		 * @brief Destructor
		 *
		 * If there's object managed, the destructor will decrease the
		 * reference count, if it reaches 0, delete it.
		 */
		inline ~RefPtr ();

		inline RefPtr<T>& operator = (const RefPtr<T>& src);

		template <typename T_CastFrom>
		inline RefPtr<T>& operator = (const RefPtr<T_CastFrom>& src);

		inline void reset (T* obj);

		inline void reset (const RefPtr<T>& other);

		inline void destroy ();

		inline RefPtr<T>& swap (RefPtr<T>& other);

		inline T* operator-> () const;

		inline T& operator* () const;

		inline T* get () const;

		inline bool operator== (const RefPtr<T>& src) const;

		inline bool operator!= (const RefPtr<T>& src) const;

		/**
		 * @brief Test whether the RefPtr<> points to any underlying instance.
		 *
		 * Mimics usage of ordinary pointers:
		 * @code
		 *   if (ptr)
		 *     do_something();
		 * @endcode
		 */
		inline operator bool () const;

		inline bool operator< (const RefPtr<T>& src) const;

		inline bool operator<= (const RefPtr<T>& src) const;

		inline bool operator> (const RefPtr<T>& src) const;

		inline bool operator>= (const RefPtr<T>& src) const;

		/**
		 * @brief Dynamic cast to derived class.
		 *
		 * The RefPtr can't be cast with the usual notation so instead you can use
		 * @code
		 *   ptr_derived = RefPtr<Derived>::cast_dynamic(ptr_base);
		 * @endcode
		 */
		template<typename T_CastFrom>
		static inline RefPtr<T> cast_dynamic (const RefPtr<T_CastFrom>& src);

		/**
		 * @brief Static cast to derived class.
		 *
		 * Like the dynamic cast; the notation is
		 * @code
		 *   ptr_derived = RefPtr<Derived>::cast_static(ptr_base);
		 * @endcode
		 */
		template<typename T_CastFrom>
		static inline RefPtr<T> cast_static (const RefPtr<T_CastFrom>& src);

		/**
		 * @brief Cast to non-const.
		 *
		 * The RefPtr can't be cast with the usual notation so instead you can use
		 * @code
		 *   ptr_unconst = RefPtr<UnConstType>::cast_const(ptr_const);
		 * @endcode
		 */
		template<typename T_CastFrom>
		static inline RefPtr<T> cast_const (const RefPtr<T_CastFrom>& src);

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

	template <typename T>
	template <typename T_CastFrom>
	inline
	RefPtr<T>::RefPtr(const RefPtr<T_CastFrom>& orig)
	: m_ptr(orig.get())
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
	RefPtr<T>& RefPtr<T>::operator =(const RefPtr<T>& src)
	{
		if(m_ptr) {

			T* const old = m_ptr;
			m_ptr = src.m_ptr;
			++m_ptr->m_count;

			if (--old->m_count == 0)
				delete old;

		} else {

			m_ptr = src.m_ptr;
			++m_ptr->m_count;
		}

		return *this;
	}

	template <class T>
	template <class T_CastFrom>
	inline
	RefPtr<T>& RefPtr<T>::operator=(const RefPtr<T_CastFrom>& src)
	{
		if(m_ptr) {

			T* const old = m_ptr;
			m_ptr = src.m_ptr;
			++m_ptr->m_count;

			if (--old->m_count == 0)
				delete old;

		} else {

			m_ptr = src.m_ptr;
			++m_ptr->m_count;
		}

		return *this;
	}

	template <typename T> inline
	void RefPtr<T>::reset (T* obj)
	{
		if(m_ptr) {

			T* const old = m_ptr;
			m_ptr = obj;
			++m_ptr->m_count;

			if (--old->m_count == 0)
				delete old;

		} else {

			m_ptr = obj;
			++m_ptr->m_count;
		}
	}

	template <typename T> inline
	void RefPtr<T>::reset (const RefPtr<T>& other)
	{
		if(m_ptr) {

			T* const old = m_ptr;
			m_ptr = other.m_ptr;
			++m_ptr->m_count;

			if (--old->m_count == 0)
				delete old;

		} else {

			m_ptr = other.m_ptr;
			++m_ptr->m_count;
		}
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
	T* RefPtr<T>::get () const
	{
		return m_ptr;
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

	template<class T>
	template<class T_CastFrom>
	inline
	RefPtr<T> RefPtr<T>::cast_dynamic (const RefPtr<T_CastFrom>& src)
	{
		T* const ptr = dynamic_cast<T*>(src.get());

		if (ptr)
			++ptr->m_count;

		return RefPtr<T>(ptr);
	}

	template<class T>
	template<class T_CastFrom>
	inline
	RefPtr<T> RefPtr<T>::cast_static (const RefPtr<T_CastFrom>& src)
	{
		T * const ptr = static_cast<T*>(src.get());

		if (ptr)
			++ptr->m_count;

		return RefPtr<T>(ptr);
	}

	template<class T>
	template<class T_CastFrom>
	inline
	RefPtr<T> RefPtr<T>::cast_const (const RefPtr<T_CastFrom>& src)
	{
		T * const ptr = const_cast<T*>(src.get());

		if (ptr)
			++ptr->m_count;

		return RefPtr<T>(ptr);
	}

}

#endif /* _BLENDINT_REFPTR_HPP_ */
