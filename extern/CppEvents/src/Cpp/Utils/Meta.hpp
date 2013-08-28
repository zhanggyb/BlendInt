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

#ifndef __CPP__UTILS__META__HPP
#define __CPP__UTILS__META__HPP

namespace Cpp {
#ifdef DOXYGEN_RUN

	class Meta
	{
	public:
#else
	namespace Meta {
#endif
		//------------------------------------------------------------------------------

		struct NullType
		{
		};

		struct InvalidType
		{
		};
		//------------------------------------------------------------------------------
		typedef char SmallType;

		struct BigType
		{
			char dummy[16];
		};
		//------------------------------------------------------------------------------

		template<class T> struct TypeForType
		{
			typedef T Type;
		};

		template<int N> struct TypeForInt
		{

			enum
			{
				value = N
			};
		};

		template<bool b> struct TypeForBool
		{

			enum
			{
				value = b
			};
		};
		//------------------------------------------------------------------------------
		typedef TypeForBool<true> TrueType;
		typedef TypeForBool<false> FalseType;
		//------------------------------------------------------------------------------

		template<class T> struct BoolNot
		{
			typedef TypeForBool<!T::value> Type;
		};

		template<class T> struct IntNeg
		{
			typedef TypeForInt<-T::value> Type;
		};

		template<class T> struct IntInv
		{
			typedef TypeForInt<~int(T::value)> Type;
		};

		template<class T> struct IntInc
		{
			typedef TypeForInt<T::value + 1> Type;
		};

		template<class T> struct IntDec
		{
			typedef TypeForInt<T::value - 1> Type;
		};
		//------------------------------------------------------------------------------

		template<class T0, class T1, class T2 = TrueType, class T3 = TrueType,
		        class T4 = TrueType, class T5 = TrueType, class T6 = TrueType,
		        class T7 = TrueType> struct BoolAnd
		{
			typedef TypeForBool<
			        T0::value && T1::value && T2::value && T3::value
			                && T4::value && T5::value && T6::value && T7::value> Type;
		};
		//------------------------------------------------------------------------------

		template<class T0, class T1, class T2 = FalseType, class T3 = FalseType,
		        class T4 = FalseType, class T5 = FalseType,
		        class T6 = FalseType, class T7 = FalseType> struct BoolOr
		{
			typedef TypeForBool<
			        T0::value || T1::value || T2::value || T3::value
			                || T4::value || T5::value || T6::value || T7::value> Type;
		};
		//------------------------------------------------------------------------------
		template<class T0, class T1 = FalseType, class T2 = FalseType,
		        class T3 = FalseType, class T4 = FalseType,
		        class T5 = FalseType, class T6 = FalseType, class T7 = FalseType> struct BoolXor;

		template<class T0> struct BoolXor<T0, FalseType, FalseType, FalseType,
		        FalseType, FalseType, FalseType, FalseType>
		{
			typedef T0 Type;
		};

		template<class T0> struct BoolXor<T0, TrueType, FalseType, FalseType,
		        FalseType, FalseType, FalseType, FalseType>
		{
			typedef typename BoolNot<T0>::Type Type;
		};

		template<class T0, class T1> struct BoolXor<T0, T1, TrueType, FalseType,
		        FalseType, FalseType, FalseType, FalseType>
		{
			typedef typename BoolNot<typename BoolXor<T0, T1>::Type>::Type Type;
		};

		template<class T0, class T1, class T2> struct BoolXor<T0, T1, T2,
		        TrueType, FalseType, FalseType, FalseType, FalseType>
		{
			typedef typename BoolNot<typename BoolXor<T0, T1, T2>::Type>::Type Type;
		};

		template<class T0, class T1, class T2, class T3> struct BoolXor<T0, T1,
		        T2, T3, TrueType, FalseType, FalseType, FalseType>
		{
			typedef typename BoolNot<typename BoolXor<T0, T1, T2, T3>::Type>::Type Type;
		};

		template<class T0, class T1, class T2, class T3, class T4> struct BoolXor<
		        T0, T1, T2, T3, T4, TrueType, FalseType, FalseType>
		{
			typedef typename BoolNot<typename BoolXor<T0, T1, T2, T3, T4>::Type>::Type Type;
		};

		template<class T0, class T1, class T2, class T3, class T4, class T5> struct BoolXor<
		        T0, T1, T2, T3, T4, T5, TrueType, FalseType>
		{
			typedef typename BoolNot<
			        typename BoolXor<T0, T1, T2, T3, T4, T5>::Type>::Type Type;
		};

		template<class T0, class T1, class T2, class T3, class T4, class T5,
		        class T6> struct BoolXor<T0, T1, T2, T3, T4, T5, T6, TrueType>
		{
			typedef typename BoolNot<
			        typename BoolXor<T0, T1, T2, T3, T4, T5, T6>::Type>::Type Type;
		};
		//------------------------------------------------------------------------------

		template<class T0, class T1, class T2 = TypeForInt<0>,
		        class T3 = TypeForInt<0>, class T4 = TypeForInt<0>,
		        class T5 = TypeForInt<0>, class T6 = TypeForInt<0>,
		        class T7 = TypeForInt<0> > struct IntAdd
		{
			typedef typename TypeForInt<
			        T0::value + T1::value + T2::value + T3::value + T4::value
			                + T5::value + T6::value + T7::value>::Type Type;
		};
		//------------------------------------------------------------------------------

		template<class T0, class T1, class T2 = TypeForInt<1>,
		        class T3 = TypeForInt<1>, class T4 = TypeForInt<1>,
		        class T5 = TypeForInt<1>, class T6 = TypeForInt<1>,
		        class T7 = TypeForInt<1> > struct IntMul
		{
			typedef typename TypeForInt<
			        T0::value * T1::value * T2::value * T3::value * T4::value
			                * T5::value * T6::value * T7::value>::Type Type;
		};
		//------------------------------------------------------------------------------

		template<class T0, class T1> struct IntSub
		{
			typedef TypeForInt<T0::value - T1::value> Type;
		};

		template<class T0, class T1> struct IntDiv
		{
			typedef TypeForInt<T0::value / T1::value> Type;
		};

		template<class T0, class T1> struct IntMod
		{
			typedef TypeForInt<T0::value % T1::value> Type;
		};
		//------------------------------------------------------------------------------

		template<class T0, class T1> struct IntPow
		{
			typedef typename IntMul<T0,
			        typename IntPow<T0, typename IntDec<T1>::Type>::Type>::Type Type;
		};

		template<class T0> struct IntPow<T0, TypeForInt<0> >
		{
			typedef TypeForInt<1> Type;
		};
		//------------------------------------------------------------------------------

		template<class T0, class T1> struct IntCompare
		{

			enum
			{
				isEqual = (T0::value == T1::value),
				isNotEqual = (T0::value != T1::value),
				isLess = (T0::value < T1::value),
				isNotLess = (T0::value >= T1::value),
				isGreater = (T0::value < T1::value),
				isNotGreater = (T0::value >= T1::value)
			};

			typedef TypeForBool<isEqual> Equal;
			typedef TypeForBool<isNotEqual> NotEqual;
			typedef TypeForBool<isLess> Less;
			typedef TypeForBool<isNotLess> NotLess;
			typedef TypeForBool<isGreater> Greater;
			typedef TypeForBool<isNotGreater> NotGreater;
		};
		//------------------------------------------------------------------------------
		template<class Cond, class ThenClass, class ElseClass> struct TypeIf;

		template<class ThenClass, class ElseClass> struct TypeIf<TrueType,
		        ThenClass, ElseClass>
		{
			typedef ThenClass Type;
		};

		template<class ThenClass, class ElseClass> struct TypeIf<FalseType,
		        ThenClass, ElseClass>
		{
			typedef ElseClass Type;
		};
	//------------------------------------------------------------------------------
#ifdef DOXYGEN_RUN
}; //class Meta
#else
	} //namespace Meta
#endif
} //namespace Cpp

#endif //__CPP__UTILS__META__HPP
