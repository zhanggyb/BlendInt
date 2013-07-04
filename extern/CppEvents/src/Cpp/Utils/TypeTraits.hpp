// Copyright (c) 2010 Nickolas Pohilets
//
// This file is a part of the CppEvents library.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef __CPP__UTILS__TYPE_TRAITS__HPP
#define __CPP__UTILS__TYPE_TRAITS__HPP

#include <Cpp/Utils/Meta.hpp>

namespace Cpp {
#ifdef DOXYGEN_RUN

class TypeTraits
    {
    public:
#else
    namespace TypeTraits {
#endif
	//------------------------------------------------------------------------------

	template<class T> struct SizeOf
	{
	    typedef Meta::TypeForInt<sizeof (T) > Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsSigned
	{
	    typedef Meta::FalseType Type;
	};

	template<> struct IsSigned<char>
	{
	    typedef Meta::TypeForBool < (char(int(-1)) < 0) > Type;
	};

	template<> struct IsSigned<signed char>
	{
	    typedef Meta::TrueType Type;
	};

	template<> struct IsSigned<signed short>
	{
	    typedef Meta::TrueType Type;
	};

	template<> struct IsSigned<signed int>
	{
	    typedef Meta::TrueType Type;
	};

	template<> struct IsSigned<signed long>
	{
	    typedef Meta::TrueType Type;
	};

	template<> struct IsSigned<signed long long>
	{
	    typedef Meta::TrueType Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsUnsigned
	{
	    typedef Meta::FalseType Type;
	};

	template<> struct IsUnsigned<char>
	{
	    typedef Meta::BoolNot<IsSigned<char>::Type>::Type Type;
	};

	template<> struct IsUnsigned<unsigned char>
	{
	    typedef Meta::TrueType Type;
	};

	template<> struct IsUnsigned<unsigned short>
	{
	    typedef Meta::TrueType Type;
	};

	template<> struct IsUnsigned<unsigned int>
	{
	    typedef Meta::TrueType Type;
	};

	template<> struct IsUnsigned<unsigned long>
	{
	    typedef Meta::TrueType Type;
	};

	template<> struct IsUnsigned<unsigned long long>
	{
	    typedef Meta::TrueType Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsChar
	{
	    typedef Meta::FalseType Type;
	};

	template<> struct IsChar<char>
	{
	    typedef Meta::TrueType Type;
	};

	template<> struct IsChar<signed char>
	{
	    typedef Meta::TrueType Type;
	};

	template<> struct IsChar<unsigned char>
	{
	    typedef Meta::TrueType Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsWChar
	{
	    typedef Meta::FalseType Type;
	};

	template<> struct IsWChar<wchar_t>
	{
	    typedef Meta::TrueType Type;
	};
	//template<> struct IsWChar<signed wchar_t>   { typedef Meta::TrueType Type; };
	//template<> struct IsWChar<unsigned wchar_t> { typedef Meta::TrueType Type; };
	//------------------------------------------------------------------------------

	template<class T> struct IsCharType
	{
	    typedef typename Meta::BoolOr<
		    typename IsChar<T>::Type,
		    typename IsWChar<T>::Type
		    >::Type Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsShort
	{
	    typedef Meta::FalseType Type;
	};
	//template<> struct IsShort<short>          { typedef TrueType Type; };

	template<> struct IsShort<signed short>
	{
	    typedef Meta::TrueType Type;
	};

	template<> struct IsShort<unsigned short>
	{
	    typedef Meta::TrueType Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsInt
	{
	    typedef Meta::FalseType Type;
	};
	//template<> struct IsInt<int>          { typedef TrueType Type; };

	template<> struct IsInt<signed int>
	{
	    typedef Meta::TrueType Type;
	};

	template<> struct IsInt<unsigned int>
	{
	    typedef Meta::TrueType Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsLong
	{
	    typedef Meta::FalseType Type;
	};
	//template<> struct IsLong<long>          { typedef TrueType Type; };

	template<> struct IsLong<signed long>
	{
	    typedef Meta::TrueType Type;
	};

	template<> struct IsLong<unsigned long>
	{
	    typedef Meta::TrueType Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsLongLong
	{
	    typedef Meta::FalseType Type;
	};
	//template<> struct IsLongLong<long long>          { typedef TrueType Type; };

	template<> struct IsLongLong<signed long long>
	{
	    typedef Meta::TrueType Type;
	};

	template<> struct IsLongLong<unsigned long long>
	{
	    typedef Meta::TrueType Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsInteger
	{
	    typedef typename Meta::BoolOr<
		    typename IsSigned<T>::Type,
		    typename IsUnsigned<T>::Type
		    >::Type Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsFloating
	{
	    typedef Meta::FalseType Type;
	};

	template<> struct IsFloating<float>
	{
	    typedef Meta::TrueType Type;
	};

	template<> struct IsFloating<double>
	{
	    typedef Meta::TrueType Type;
	};

	template<> struct IsFloating<long double>
	{
	    typedef Meta::TrueType Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsBool
	{
	    typedef Meta::FalseType Type;
	};

	template<> struct IsBool<bool>
	{
	    typedef Meta::TrueType Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsVoid
	{
	    typedef Meta::FalseType Type;
	};

	template<> struct IsVoid<void>
	{
	    typedef Meta::TrueType Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsFundamental
	{
	    typedef typename Meta::BoolOr<
		    typename IsVoid<T>::Type,
		    typename IsBool<T>::Type,
		    typename IsChar<T>::Type,
		    typename IsInteger<T>::Type,
		    typename IsFloating<T>::Type
		    >::Type Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsPointer
	{
	    typedef Meta::FalseType Type;
	};

	template<class T> struct IsPointer<T*>
	{
	    typedef Meta::TrueType Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct UnPointer
	{
	    typedef T Type;
	};

	template<class T> struct UnPointer<T*>
	{
	    typedef T Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsReference
	{
	    typedef Meta::FalseType Type;
	};

	template<class T> struct IsReference<T&>
	{
	    typedef Meta::TrueType Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct UnReference
	{
	    typedef T Type;
	};

	template<class T> struct UnReference<T&>
	{
	    typedef T Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsConst
	{
	    typedef Meta::FalseType Type;
	};

	template<class T> struct IsConst<T const>
	{
	    typedef Meta::TrueType Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct UnConst
	{
	    typedef T Type;
	};

	template<class T> struct UnConst<T const>
	{
	    typedef T Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsVolatile
	{
	    typedef Meta::FalseType Type;
	};

	template<class T> struct IsVolatile<T volatile>
	{
	    typedef Meta::TrueType Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct UnVolatile
	{
	    typedef T Type;
	};

	template<class T> struct UnVolatile<T volatile>
	{
	    typedef T Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct Simplified
	{
	    typedef typename UnVolatile<
		    typename UnConst<T>::Type
		    >::Type Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct ParameterType
	{
	    typedef typename Meta::TypeIf<
		    typename IsReference<T>::Type, T,
		    typename Meta::TypeIf<
		    typename Meta::BoolOr<
		    typename IsFundamental<T>::Type,
		    typename IsPointer<T>::Type
		    >::Type, T const, T const &
		    >::Type
		    >::Type Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct StorageType
	{
	    typedef typename Simplified<
		    typename UnReference<T>::Type
		    >::Type Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsSmall
	{
	    typedef Meta::TypeForBool<sizeof (T) <= sizeof (void*)> Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsPOD
	{
	    //TODO: implement this
	    typedef Meta::FalseType Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsPrimitive
	{
	    typedef typename Meta::BoolOr<
		    typename IsFundamental<T>::Type,
		    typename IsPointer<T>::Type,
		    typename IsPOD<T>::Type
		    >::Type Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsCopiable
	{
	    typedef typename IsPrimitive<T>::Type Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsMovable
	{
	    typedef typename IsCopiable<T>::Type Type;
	};
	//------------------------------------------------------------------------------

	template<class T> struct IsComplex
	{
	    typedef typename Meta::BoolNot<typename IsMovable<T>::Type>::Type Type;
	};
	//------------------------------------------------------------------------------

	template<class T0, class T1> struct CanConvert
	{
	private:
	    static T0 makeSrc();
	    static Meta::SmallType convertTestHelper(T1);
	    static Meta::BigType convertTestHelper(...);
	    static bool const result = sizeof (convertTestHelper(makeSrc())) == sizeof (Meta::SmallType);
	public:
	    typedef Meta::TypeForBool<result> Type;
	};
	//------------------------------------------------------------------------------

	template<class T0, class T1> struct AreIdentical
	{
	    typedef Meta::FalseType Type;
	};

	template<class T> struct AreIdentical<T, T>
	{
	    typedef Meta::TrueType Type;
	};
	//------------------------------------------------------------------------------

	template<class T0, class T1> struct IsSubclass
	{
	    typedef typename Meta::BoolAnd<
		    typename CanConvert<T0 const*, T1 const*>::Type,
		    typename Meta::BoolNot<
		    typename AreIdentical<T1 const *, void const *>::Type
		    >::Type
		    >::Type Type;
	};
	//------------------------------------------------------------------------------
#ifdef DOXYGEN_RUN
    }; //class TypeTraits
#else
    } //namespace TypeTraits
#endif
} //namespace Cpp

#endif //__CPP__UTILS__TYPE_TRAITS__HPP
