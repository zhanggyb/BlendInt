/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Freeman Zhang <zhanggyb@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <string.h>

namespace CppEvent {

// generic classes to calculate method pointer:
class GenericBase1 {};
class GenericBase2 {};
class GenericMultiInherit: GenericBase1, GenericBase2 {};

typedef void (GenericMultiInherit::*GenericMethodPointer)();

/**
 * @brief Delegate with variadic template
 *
 * Inspired by http://www.codeproject.com/Articles/11015/The-Impossibly-Fast-C-Delegates
 *
 * @note only support method in this project, which means you cannot
 * create a delegate to a static member function
 */
template<typename ReturnType, typename ... ParamTypes>
class Delegate
{
  typedef ReturnType (*MethodStubType) (void* object_ptr, GenericMethodPointer, ParamTypes...);

  struct PointerData
  {

    inline PointerData()
        : object_pointer(0),
          method_stub(0),
          method_pointer(0)
    { }

    inline PointerData(const PointerData& orig)
        : object_pointer(orig.object_pointer),
          method_stub(orig.method_stub),
          method_pointer(orig.method_pointer)
    { }
    
    inline PointerData& operator = (const PointerData& orig)
    {
      object_pointer = orig.object_pointer;
      method_stub = orig.method_stub;
      method_pointer = orig.method_pointer;
      return *this;
    }
    
    void* object_pointer;
    MethodStubType method_stub;
    GenericMethodPointer method_pointer;
  };
  
  template<typename T, typename TFxn>
  struct MethodStub
  {
    static void invoke (void* obj_ptr, GenericMethodPointer any, ParamTypes ... Args)
    {
      T* obj = static_cast<T*>(obj_ptr);
      (obj->*reinterpret_cast<TFxn>(any))(Args...);
    }
  };

 public:

  template<typename T>
  static inline Delegate from_method (T* object_ptr,
                                      ReturnType (T::*method) (ParamTypes...))
  {
    typedef ReturnType (T::*TMethod)(ParamTypes...);

    Delegate d;
    d.data_.object_pointer = object_ptr;
    d.data_.method_stub = &MethodStub<T, TMethod>::invoke;
    d.data_.method_pointer = reinterpret_cast<GenericMethodPointer>(method);

    return d;
  }

  template<typename T>
  static inline Delegate from_method (T* object_ptr,
                                      ReturnType (T::*method) (ParamTypes...) const)
  {
    typedef ReturnType (T::*TMethod)(ParamTypes...) const;

    Delegate d;
    d.data_.object_pointer = object_ptr;
    d.data_.method_stub = &MethodStub<T, TMethod>::invoke;
    d.data_.method_pointer = reinterpret_cast<GenericMethodPointer>(method);

    return d;
  }
  
  inline Delegate (const Delegate& orig)
      : data_(orig.data_)
  { }

  inline ~Delegate () {}

  inline void reset ()
  {
    memset(&data_, 0, sizeof(PointerData));
  }
  
  inline Delegate& operator = (const Delegate& orig)
  {
    data_ = orig.data_;
    return *this;
  }
  
  inline ReturnType operator () (ParamTypes... Args) const
  {
    return (*data_.method_stub)(data_.object_pointer, data_.method_pointer, Args...);
  }
  
  inline ReturnType invoke(ParamTypes... Args) const
  {
    return (*data_.method_stub)(data_.object_pointer, data_.method_pointer, Args...);
  }
  
  inline operator bool () const
  {
    // Support method delegate only, no need to check other members:
    return data_.method_pointer != 0;
  }
  
  inline bool operator == (const Delegate& other) const
  {
    return memcmp(&data_, &other.data_, sizeof(PointerData)) == 0;
  }
  
  inline bool operator != (const Delegate& other) const
  {
    return memcmp(&data_, &other.data_, sizeof(PointerData)) != 0;
  }
  
  template<typename T>
  inline bool equal(T* object_ptr, ReturnType(T::*method) (ParamTypes...)) const
  {
    typedef ReturnType (T::*TMethod)(ParamTypes...);
    
    return (data_.object_pointer == object_ptr) &&
        (data_.method_stub == &MethodStub<T, TMethod>::invoke) &&
        (data_.method_pointer == reinterpret_cast<GenericMethodPointer>(method));
  }
  
  template<typename T>
  inline bool equal(T* object_ptr, ReturnType(T::*method) (ParamTypes...) const) const
  {
    typedef ReturnType (T::*TMethod)(ParamTypes...) const;
    
    return (data_.object_pointer == object_ptr) &&
        (data_.method_stub == &MethodStub<T, TMethod>::invoke) &&
        (data_.method_pointer == reinterpret_cast<GenericMethodPointer>(method));
  }

 private:

  inline Delegate () { }
  
  template<typename ReturnTypeAlias, typename ... ParamTypesAlias>
  friend inline bool operator == (const Delegate<ReturnTypeAlias,
                                  ParamTypesAlias...>& src,
                                  const Delegate<ReturnTypeAlias,
                                  ParamTypesAlias...>& dst);

  template<typename ReturnTypeAlias, typename ... ParamTypesAlias>
  friend inline bool operator < (const Delegate<ReturnTypeAlias,
                                 ParamTypesAlias...>& src,
                                 const Delegate<ReturnTypeAlias,
                                 ParamTypesAlias...>& dst);

  template<typename ReturnTypeAlias, typename ... ParamTypesAlias>
  friend inline bool operator > (const Delegate<ReturnTypeAlias,
                                 ParamTypesAlias...>& src,
                                 const Delegate<ReturnTypeAlias,
                                 ParamTypesAlias...>& dst);

  PointerData data_;
};

template<typename ReturnType, typename ... ParamTypes>
inline bool operator == (const Delegate<ReturnType, ParamTypes...>& src,
                         const Delegate<ReturnType, ParamTypes...>& dst)
{
  return memcmp(&src.data_, &dst.data_,
                sizeof(typename Delegate<ReturnType, ParamTypes...>::PointerData)
                ) == 0;
}

template<typename ReturnType, typename ... ParamTypes>
inline bool operator < (const Delegate<ReturnType, ParamTypes...>& src,
                        const Delegate<ReturnType, ParamTypes...>& dst)
{
  return memcmp(&src.data_, &dst.data_,
                sizeof(typename Delegate<ReturnType, ParamTypes...>::PointerData)
                ) < 0;
}

template<typename ReturnType, typename ... ParamTypes>
inline bool operator > (const Delegate<ReturnType, ParamTypes...>& src,
                        const Delegate<ReturnType, ParamTypes...>& dst)
{
  return memcmp(&src.data_, &dst.data_,
                sizeof(typename Delegate<ReturnType, ParamTypes...>::PointerData)
                ) > 0;
}

} // namespace CppEvent
