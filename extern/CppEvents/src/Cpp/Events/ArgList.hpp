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

#ifndef __CPP__EVENTS__ARG_LIST__HPP
#define __CPP__EVENTS__ARG_LIST__HPP

#include <FastDelegate/FastDelegate.h>

namespace Cpp {
    namespace Private {
	namespace Events {
	    //------------------------------------------------------------------------------

	    template<int N> struct Num
	    {

		enum
		{
		    value = N
		};
	    };
	    //------------------------------------------------------------------------------
	    template<int N, class T> struct GetType;

	    template<class T> struct GetType<0, T>
	    {
		typedef typename T::Type0 Type;
	    };

	    template<class T> struct GetType<1, T>
	    {
		typedef typename T::Type1 Type;
	    };

	    template<class T> struct GetType<2, T>
	    {
		typedef typename T::Type2 Type;
	    };

	    template<class T> struct GetType<3, T>
	    {
		typedef typename T::Type3 Type;
	    };

	    template<class T> struct GetType<4, T>
	    {
		typedef typename T::Type4 Type;
	    };

	    template<class T> struct GetType<5, T>
	    {
		typedef typename T::Type5 Type;
	    };

	    template<class T> struct GetType<6, T>
	    {
		typedef typename T::Type6 Type;
	    };

	    template<class T> struct GetType<7, T>
	    {
		typedef typename T::Type7 Type;
	    };
	    //------------------------------------------------------------------------------

	    class ArgList0
	    {
	    public:

		enum
		{
		    size = 0
		};

		ArgList0 ()
		{
		}
	    };
	    //------------------------------------------------------------------------------

	    template<class T0>
	    class ArgList1 : public ArgList0
	    {
	    public:

		enum
		{
		    size = 1
		};
		typedef T0 Type0;

		T0 x0;

		ArgList1 (T0 a0)
		: ArgList0 (), x0 (a0)
		{
		}

		T0 get (Num<0>) const
		{
		    return x0;
		}
	    };
	    //------------------------------------------------------------------------------

	    template<class T0, class T1>
	    class ArgList2 : public ArgList1<T0>
	    {
	    public:

		enum
		{
		    size = 2
		};
		typedef T1 Type1;

		T1 x1;

		ArgList2 (T0 a0, T1 a1)
		: ArgList1<T0>(a0), x1 (a1)
		{
		}

		T1 get (Num<1>) const
		{
		    return x1;
		}
	    };
	    //------------------------------------------------------------------------------

	    template<class T0, class T1, class T2>
	    class ArgList3 : public ArgList2<T0, T1>
	    {
	    public:

		enum
		{
		    size = 3
		};
		typedef T2 Type2;

		T2 x2;

		ArgList3 (T0 a0, T1 a1, T2 a2)
		: ArgList2<T0, T1>(a0, a1), x2 (a2)
		{
		}

		T2 get (Num<2>) const
		{
		    return x2;
		}
	    };
	    //------------------------------------------------------------------------------

	    template<class T0, class T1, class T2, class T3>
	    class ArgList4 : public ArgList3<T0, T1, T2>
	    {
	    public:

		enum
		{
		    size = 4
		};
		typedef T3 Type3;

		T3 x3;

		ArgList4 (T0 a0, T1 a1, T2 a2, T3 a3)
		: ArgList3<T0, T1, T2>(a0, a1, a2), x3 (a3)
		{
		}

		T3 get (Num<3>) const
		{
		    return x3;
		}
	    };
	    //------------------------------------------------------------------------------

	    template<class T0, class T1, class T2, class T3, class T4>
	    class ArgList5 : public ArgList4<T0, T1, T2, T3>
	    {
	    public:

		enum
		{
		    size = 5
		};
		typedef T4 Type4;

		T4 x4;

		ArgList5 (T0 a0, T1 a1, T2 a2, T3 a3, T4 a4)
		: ArgList4<T0, T1, T2, T3>(a0, a1, a2, a3), x4 (a4)
		{
		}

		T4 get (Num<4>) const
		{
		    return x4;
		}
	    };
	    //------------------------------------------------------------------------------

	    template<class T0, class T1, class T2, class T3, class T4, class T5>
	    class ArgList6 : public ArgList5<T0, T1, T2, T3, T4>
	    {
	    public:

		enum
		{
		    size = 6
		};
		typedef T5 Type5;

		T5 x5;

		ArgList6 (T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
		: ArgList5<T0, T1, T2, T3, T4>(a0, a1, a2, a3, a4), x5 (a5)
		{
		}

		T5 get (Num<5>) const
		{
		    return x5;
		}
	    };
	    //------------------------------------------------------------------------------

	    template<class T0, class T1, class T2, class T3, class T4, class T5, class T6>
	    class ArgList7 : public ArgList6<T0, T1, T2, T3, T4, T5>
	    {
	    public:

		enum
		{
		    size = 7
		};
		typedef T6 Type6;

		T6 x6;

		ArgList7 (T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6)
		: ArgList6<T0, T1, T2, T3, T4, T5>(a0, a1, a2, a3, a4, a5), x6 (a6)
		{
		}

		T6 get (Num<6>) const
		{
		    return x6;
		}
	    };
	    //------------------------------------------------------------------------------

	    template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	    class ArgList8 : public ArgList7<T0, T1, T2, T3, T4, T5, T6>
	    {
	    public:

		enum
		{
		    size = 8
		};
		typedef T7 Type7;

		T7 x7;

		ArgList8 (T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7)
		: ArgList7<T0, T1, T2, T3, T4, T5, T6>(a0, a1, a2, a3, a4, a5, a6), x7 (a7)
		{
		}

		T7 get (Num<7>) const
		{
		    return x7;
		}
	    };
	    //------------------------------------------------------------------------------
	    template<class StoredList, class ParamList, int N, bool UseStored> struct GetArgHelper;

	    template<class StoredList, class ParamList, int N> struct GetArgHelper<StoredList, ParamList, N, true>
	    {

		enum
		{
		    index = N
		};
		typedef StoredList ListType;
		typedef typename GetType<index, ListType>::Type Type;

		static Type get(StoredList const & list, ParamList const &)
		{
		    return list.get(Num<N>());
		}
	    };

	    template<class StoredList, class ParamList, int N> struct GetArgHelper<StoredList, ParamList, N, false>
	    {

		enum
		{
		    index = N - StoredList::size
		};
		typedef ParamList ListType;
		typedef typename GetType<index, ListType>::Type Type;

		static Type get(StoredList const &, ParamList const & list)
		{
		    return list.get(Num<index>());
		}
	    };
	    //------------------------------------------------------------------------------

	    template<class StoredList, class ParamList, int N> struct GetArg
	    {

		enum
		{
		    useStored = (N < StoredList::size)
		};
		typedef GetArgHelper<StoredList, ParamList, N, useStored > HelperType;

		static typename HelperType::Type get(StoredList const & s, ParamList const & p)
		{
		    return HelperType::get(s, p);
		}
	    };
	    //------------------------------------------------------------------------------

	    template<class R, class StoredList, class ParamList>
	    R invokeDelegate (fastdelegate::FastDelegate0<R> const & deleg, StoredList const & s, ParamList const & p)
	    {
		return deleg();
	    }
	    //------------------------------------------------------------------------------

	    template<class R, class T0, class StoredList, class ParamList>
	    R invokeDelegate (fastdelegate::FastDelegate1<T0, R> const & deleg, StoredList const & s, ParamList const & p)
	    {
		return deleg(
			     GetArg<StoredList, ParamList, 0>::get(s, p)
			     );
	    }
	    //------------------------------------------------------------------------------

	    template<class R, class T0, class T1, class StoredList, class ParamList>
	    R invokeDelegate (fastdelegate::FastDelegate2<T0, T1, R> const & deleg, StoredList const & s, ParamList const & p)
	    {
		return deleg(
			     GetArg<StoredList, ParamList, 0>::get(s, p),
			     GetArg<StoredList, ParamList, 1>::get(s, p)
			     );
	    }
	    //------------------------------------------------------------------------------

	    template<class R, class T0, class T1, class T2, class StoredList, class ParamList>
	    R invokeDelegate (fastdelegate::FastDelegate3<T0, T1, T2, R> const & deleg, StoredList const & s, ParamList const & p)
	    {
		return deleg(
			     GetArg<StoredList, ParamList, 0>::get(s, p),
			     GetArg<StoredList, ParamList, 1>::get(s, p),
			     GetArg<StoredList, ParamList, 2>::get(s, p)
			     );
	    }
	    //------------------------------------------------------------------------------

	    template<class R, class T0, class T1, class T2, class T3, class StoredList, class ParamList>
	    R invokeDelegate (fastdelegate::FastDelegate4<T0, T1, T2, T3, R> const & deleg, StoredList const & s, ParamList const & p)
	    {
		return deleg(
			     GetArg<StoredList, ParamList, 0>::get(s, p),
			     GetArg<StoredList, ParamList, 1>::get(s, p),
			     GetArg<StoredList, ParamList, 2>::get(s, p),
			     GetArg<StoredList, ParamList, 3>::get(s, p)
			     );
	    }
	    //------------------------------------------------------------------------------

	    template<class R, class T0, class T1, class T2, class T3, class T4, class StoredList, class ParamList>
	    R invokeDelegate (fastdelegate::FastDelegate5<T0, T1, T2, T3, T4, R> const & deleg, StoredList const & s, ParamList const & p)
	    {
		return deleg(
			     GetArg<StoredList, ParamList, 0>::get(s, p),
			     GetArg<StoredList, ParamList, 1>::get(s, p),
			     GetArg<StoredList, ParamList, 2>::get(s, p),
			     GetArg<StoredList, ParamList, 3>::get(s, p),
			     GetArg<StoredList, ParamList, 4>::get(s, p)
			     );
	    }
	    //------------------------------------------------------------------------------

	    template<class R, class T0, class T1, class T2, class T3, class T4, class T5, class StoredList, class ParamList>
	    R invokeDelegate (fastdelegate::FastDelegate6<T0, T1, T2, T3, T4, T5, R> const & deleg, StoredList const & s, ParamList const & p)
	    {
		return deleg(
			     GetArg<StoredList, ParamList, 0>::get(s, p),
			     GetArg<StoredList, ParamList, 1>::get(s, p),
			     GetArg<StoredList, ParamList, 2>::get(s, p),
			     GetArg<StoredList, ParamList, 3>::get(s, p),
			     GetArg<StoredList, ParamList, 4>::get(s, p),
			     GetArg<StoredList, ParamList, 5>::get(s, p)
			     );
	    }
	    //------------------------------------------------------------------------------

	    template<class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class StoredList, class ParamList>
	    R invokeDelegate (fastdelegate::FastDelegate7<T0, T1, T2, T3, T4, T5, T6, R> const & deleg, StoredList const & s, ParamList const & p)
	    {
		return deleg(
			     GetArg<StoredList, ParamList, 0>::get(s, p),
			     GetArg<StoredList, ParamList, 1>::get(s, p),
			     GetArg<StoredList, ParamList, 2>::get(s, p),
			     GetArg<StoredList, ParamList, 3>::get(s, p),
			     GetArg<StoredList, ParamList, 4>::get(s, p),
			     GetArg<StoredList, ParamList, 5>::get(s, p),
			     GetArg<StoredList, ParamList, 6>::get(s, p)
			     );
	    }
	    //------------------------------------------------------------------------------

	    template<class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class StoredList, class ParamList>
	    R invokeDelegate (fastdelegate::FastDelegate8<T0, T1, T2, T3, T4, T5, T6, T7, R> const & deleg, StoredList const & s, ParamList const & p)
	    {
		return deleg(
			     GetArg<StoredList, ParamList, 0>::get(s, p),
			     GetArg<StoredList, ParamList, 1>::get(s, p),
			     GetArg<StoredList, ParamList, 2>::get(s, p),
			     GetArg<StoredList, ParamList, 3>::get(s, p),
			     GetArg<StoredList, ParamList, 4>::get(s, p),
			     GetArg<StoredList, ParamList, 5>::get(s, p),
			     GetArg<StoredList, ParamList, 6>::get(s, p),
			     GetArg<StoredList, ParamList, 7>::get(s, p)
			     );
	    }
	    //------------------------------------------------------------------------------
	} //namespace Events
    } //namespace Private
} //namespace Cpp

#endif //__CPP__EVENTS__ARG_LIST__HPP
