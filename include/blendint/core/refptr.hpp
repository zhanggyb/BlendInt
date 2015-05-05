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

#pragma once

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
 *
 * @ingroup blendint_core
 */
template<typename T>
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
  inline RefPtr (T* obj);

  /**
   * @brief Copy constructor
   * @param orig A RefPtr already exists
   *
   * Create from an already existing RefPtr, this will increase the
   * shared reference count of the object stored.
   */
  inline RefPtr (const RefPtr<T>& orig);

  /**
   * @brief Copy constructor (from different, but castable type)
   * @param orig A different RefPtr which holds another Object inheritance
   *
   * Create from an already existing RefPtr which holds another Object inheritance,
   * and increase the shared reference count.
   */
  template<typename T_CastFrom>
  inline RefPtr (const RefPtr<T_CastFrom>& orig);

  /**
   * @brief Destructor
   *
   * If there's object managed, the destructor will decrease the
   * reference count, if it reaches 0, delete it.
   */
  inline ~RefPtr ();

  /**
   * @brief Assignment operator overloading
   * @param src A RefPtr
   * @return This RefPtr
   *
   * Copy from another RefPtr, and increase the reference count.
   */
  inline RefPtr<T>& operator = (const RefPtr<T>& src);

  /**
   * @brief Assignment operator overloading
   * @param src Another type of RefPtr
   * @return This RefPtr
   *
   * Copy from different but castable type of RefPtr, and increase the reference count.
   */
  template<typename T_CastFrom>
  inline RefPtr<T>& operator = (const RefPtr<T_CastFrom>& src);

  /**
   * @brief Reset the pointer
   * @param obj A new object pointer
   *
   * @note If obj = 0, this function is the same as destroy()
   */
  inline void reset (T* obj);

  /**
   * @brief Reset the pointer
   * @param other Another RefPtr
   */
  inline void reset (const RefPtr<T>& other);

  /**
   * @brief Try to delete the object pointer stored
   *
   * This function decrease the reference count of the object, if count is 0, delete it.
   *
   * After calling this function, the object pointer becomes 0 and
   * @code
   * 	if(ptr)
   * @endcode
   *
   * always returns false
   */
  inline void destroy ();

  /**
   * @brief Swap the contents of two RefPtr<>.
   *
   * This method swaps the internal pointers to object T. This can be
   * done safely without involving a reference/unreference cycle and is
   * therefore highly efficient.
   */
  inline RefPtr<T>& swap (RefPtr<T>& other);

  /**
   * @brief Overload "->" operator for dereferencing
   * @return The pointer to the T object
   *
   * Use the methods of the underlying instance like so:
   * @code
   * refptr->memberfun();
   * @endcode
   */
  inline T* operator-> () const;

  /**
   * @brief Get the reference of the object
   * @return A reference to the object stored
   */
  inline T& operator* () const;

  /**
   * @brief Get the pointer
   * @return A pointer of object T
   */
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

  T* ptr_;

};

template<typename T>
inline RefPtr<T>::RefPtr ()
    : ptr_(0)
{
}

template<typename T>
inline RefPtr<T>::RefPtr (T* obj)
    : ptr_(obj)
{
  if (ptr_) ++ptr_->reference_count_;
}

template<typename T>
inline RefPtr<T>::RefPtr (const RefPtr<T>& orig)
    : ptr_(orig.ptr_)
{
  if (ptr_) ++ptr_->reference_count_;
}

template<typename T>
template<typename T_CastFrom>
inline RefPtr<T>::RefPtr (const RefPtr<T_CastFrom>& orig)
    : ptr_(orig.get())
{
  if (ptr_) ++ptr_->reference_count_;
}

template<typename T>
inline RefPtr<T>::~RefPtr ()
{
  destroy();
}

template<typename T>
inline RefPtr<T>& RefPtr<T>::operator = (const RefPtr<T>& src)
{
  T* const old = ptr_;
  ptr_ = src.ptr_;

  if (ptr_) ++ptr_->reference_count_;

  if (old && (--old->reference_count_ == 0)) delete old;

  return *this;
}

template<class T>
template<class T_CastFrom>
inline RefPtr<T>& RefPtr<T>::operator= (const RefPtr<T_CastFrom>& src)
{
  T* const old = ptr_;
  ptr_ = src.get();

  if (ptr_) ++ptr_->reference_count_;

  if (old && (--old->reference_count_ == 0)) delete old;

  return *this;
}

template<typename T> inline
void RefPtr<T>::reset (T* obj)
{
  T* const old = ptr_;
  ptr_ = obj;

  if (ptr_) ++ptr_->reference_count_;

  if (old && (--old->reference_count_ == 0)) delete old;
}

template<typename T>
inline void RefPtr<T>::reset (const RefPtr<T>& other)
{
  T* const old = ptr_;
  ptr_ = other.ptr_;

  if (ptr_) ++ptr_->m_count;

  if (old && (--old->m_count == 0)) delete old;
}

template<typename T>
inline void RefPtr<T>::destroy ()
{
  if (ptr_ && (--ptr_->reference_count_ == 0)) {
    delete ptr_;

    ptr_ = 0;
  }
}

template<typename T>
inline RefPtr<T>& RefPtr<T>::swap (RefPtr<T>& other)
{
  T* const temp = ptr_;
  ptr_ = other.ptr_;
  other.ptr_ = temp;
}

template<typename T>
inline T* RefPtr<T>::operator-> () const
{
  return ptr_;
}

template<typename T>
inline T& RefPtr<T>::operator* () const
{
  return *ptr_;
}

template<typename T>
inline T* RefPtr<T>::get () const
{
  return ptr_;
}

template<typename T>
inline bool RefPtr<T>::operator== (const RefPtr<T>& src) const
{
  return (ptr_ == src.ptr_);
}

template<typename T>
inline bool RefPtr<T>::operator!= (const RefPtr<T>& src) const
{
  return (ptr_ != src.ptr_);
}

template<typename T>
inline RefPtr<T>::operator bool () const
{
  return (ptr_ != 0);
}

template<typename T>
inline bool RefPtr<T>::operator< (const RefPtr<T>& src) const
{
  return (ptr_ < src.ptr_);
}

template<typename T>
inline bool RefPtr<T>::operator<= (const RefPtr<T>& src) const
{
  return (ptr_ <= src.ptr_);
}

template<typename T>
inline bool RefPtr<T>::operator> (const RefPtr<T>& src) const
{
  return (ptr_ > src.ptr_);
}

template<typename T>
inline bool RefPtr<T>::operator>= (const RefPtr<T>& src) const
{
  return (ptr_ >= src.ptr_);
}

template<class T>
template<class T_CastFrom>
inline RefPtr<T> RefPtr<T>::cast_dynamic (const RefPtr<T_CastFrom>& src)
{
  T* const ptr = dynamic_cast<T*>(src.get());

  return RefPtr<T>(ptr);
}

template<class T>
template<class T_CastFrom>
inline RefPtr<T> RefPtr<T>::cast_static (const RefPtr<T_CastFrom>& src)
{
  T * const ptr = static_cast<T*>(src.get());

  return RefPtr<T>(ptr);
}

template<class T>
template<class T_CastFrom>
inline RefPtr<T> RefPtr<T>::cast_const (const RefPtr<T_CastFrom>& src)
{
  T * const ptr = const_cast<T*>(src.get());

  return RefPtr<T>(ptr);
}

}
