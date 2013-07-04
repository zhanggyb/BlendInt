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

#ifndef __CPP__EVENTS__CONNECTION_EX__HPP
#define __CPP__EVENTS__CONNECTION_EX__HPP

#include <Cpp/Events/Connection.hpp>
#include <Cpp/Events/ArgList.hpp>

namespace Cpp {
    namespace Private {
	namespace Events {
	    //------------------------------------------------------------------------------

	    template<class DelegateClass, class StoredListClass>
	    class ConnectionEx0 : public Connection0
	    {
	    public:
		typedef Connection0 BaseType;
		typedef ConnectionEx0<DelegateClass, StoredListClass> ThisType;

		ConnectionEx0 (DelegateClass const & deleg, StoredListClass const & stored)
		: BaseType (AbstractDelegate (deleg))
		, deleg_ (deleg), stored_ (stored)
		{
		    BaseType::delegate_ = fastdelegate::MakeDelegate(this, &ThisType::invokeHelper);
		}
	    private:
		DelegateClass deleg_;
		StoredListClass stored_;

		void invokeHelper () const
		{
		    ArgList0 paramList;
		    return invokeDelegate(deleg_, stored_, paramList);
		}
	    };
	    //------------------------------------------------------------------------------

	    template<class Param0, class DelegateClass, class StoredListClass>
	    class ConnectionEx1 : public Connection1<Param0>
	    {
	    public:
		typedef Connection1<Param0> BaseType;
		typedef ConnectionEx1<Param0, DelegateClass, StoredListClass> ThisType;

		ConnectionEx1 (DelegateClass const & deleg, StoredListClass const & stored)
		: BaseType (AbstractDelegate (deleg))
		, deleg_ (deleg), stored_ (stored)
		{
		    BaseType::delegate_ = fastdelegate::MakeDelegate(this, &ThisType::invokeHelper);
		}
	    private:
		DelegateClass deleg_;
		StoredListClass stored_;

		void invokeHelper (Param0 p0) const
		{
		    ArgList1<Param0> paramList(p0);
		    return invokeDelegate(deleg_, stored_, paramList);
		}
	    };
	    //------------------------------------------------------------------------------

	    template<class Param0, class Param1, class DelegateClass, class StoredListClass>
	    class ConnectionEx2 : public Connection2<Param0, Param1>
	    {
	    public:
		typedef Connection2<Param0, Param1> BaseType;
		typedef ConnectionEx2<Param0, Param1, DelegateClass, StoredListClass> ThisType;

		ConnectionEx2 (DelegateClass const & deleg, StoredListClass const & stored)
		: BaseType (AbstractDelegate (deleg))
		, deleg_ (deleg), stored_ (stored)
		{
		    BaseType::delegate_ = fastdelegate::MakeDelegate(this, &ThisType::invokeHelper);
		}
	    private:
		DelegateClass deleg_;
		StoredListClass stored_;

		void invokeHelper (Param0 p0, Param1 p1) const
		{
		    ArgList2<Param0, Param1> paramList(p0, p1);
		    return invokeDelegate(deleg_, stored_, paramList);
		}
	    };
	    //------------------------------------------------------------------------------

	    template<class Param0, class Param1, class Param2, class DelegateClass, class StoredListClass>
	    class ConnectionEx3 : public Connection3<Param0, Param1, Param2>
	    {
	    public:
		typedef Connection3<Param0, Param1, Param2> BaseType;
		typedef ConnectionEx3<Param0, Param1, Param2, DelegateClass, StoredListClass> ThisType;

		ConnectionEx3 (DelegateClass const & deleg, StoredListClass const & stored)
		: BaseType (AbstractDelegate (deleg))
		, deleg_ (deleg), stored_ (stored)
		{
		    BaseType::delegate_ = fastdelegate::MakeDelegate(this, &ThisType::invokeHelper);
		}
	    private:
		DelegateClass deleg_;
		StoredListClass stored_;

		void invokeHelper (Param0 p0, Param1 p1, Param2 p2) const
		{
		    ArgList3<Param0, Param1, Param2> paramList(p0, p1, p2);
		    return invokeDelegate(deleg_, stored_, paramList);
		}
	    };
	    //------------------------------------------------------------------------------

	    template<class Param0, class Param1, class Param2, class Param3, class DelegateClass, class StoredListClass>
	    class ConnectionEx4 : public Connection4<Param0, Param1, Param2, Param3>
	    {
	    public:
		typedef Connection4<Param0, Param1, Param2, Param3> BaseType;
		typedef ConnectionEx4<Param0, Param1, Param2, Param3, DelegateClass, StoredListClass> ThisType;

		ConnectionEx4 (DelegateClass const & deleg, StoredListClass const & stored)
		: BaseType (AbstractDelegate (deleg))
		, deleg_ (deleg), stored_ (stored)
		{
		    BaseType::delegate_ = fastdelegate::MakeDelegate(this, &ThisType::invokeHelper);
		}
	    private:
		DelegateClass deleg_;
		StoredListClass stored_;

		void invokeHelper (Param0 p0, Param1 p1, Param2 p2, Param3 p3) const
		{
		    ArgList4<Param0, Param1, Param2, Param3> paramList(p0, p1, p2, p3);
		    return invokeDelegate(deleg_, stored_, paramList);
		}
	    };
	    //------------------------------------------------------------------------------

	    template<class Param0, class Param1, class Param2, class Param3, class Param4, class DelegateClass, class StoredListClass>
	    class ConnectionEx5 : public Connection5<Param0, Param1, Param2, Param3, Param4>
	    {
	    public:
		typedef Connection5<Param0, Param1, Param2, Param3, Param4> BaseType;
		typedef ConnectionEx5<Param0, Param1, Param2, Param3, Param4, DelegateClass, StoredListClass> ThisType;

		ConnectionEx5 (DelegateClass const & deleg, StoredListClass const & stored)
		: BaseType (AbstractDelegate (deleg))
		, deleg_ (deleg), stored_ (stored)
		{
		    BaseType::delegate_ = fastdelegate::MakeDelegate(this, &ThisType::invokeHelper);
		}
	    private:
		DelegateClass deleg_;
		StoredListClass stored_;

		void invokeHelper (Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4) const
		{
		    ArgList5<Param0, Param1, Param2, Param3, Param4> paramList(p0, p1, p2, p3, p4);
		    return invokeDelegate(deleg_, stored_, paramList);
		}
	    };
	    //------------------------------------------------------------------------------

	    template<class Param0, class Param1, class Param2, class Param3, class Param4, class Param5, class DelegateClass, class StoredListClass>
	    class ConnectionEx6 : public Connection6<Param0, Param1, Param2, Param3, Param4, Param5>
	    {
	    public:
		typedef Connection6<Param0, Param1, Param2, Param3, Param4, Param5> BaseType;
		typedef ConnectionEx6<Param0, Param1, Param2, Param3, Param4, Param5, DelegateClass, StoredListClass> ThisType;

		ConnectionEx6 (DelegateClass const & deleg, StoredListClass const & stored)
		: BaseType (AbstractDelegate (deleg))
		, deleg_ (deleg), stored_ (stored)
		{
		    BaseType::delegate_ = fastdelegate::MakeDelegate(this, &ThisType::invokeHelper);
		}
	    private:
		DelegateClass deleg_;
		StoredListClass stored_;

		void invokeHelper (Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const
		{
		    ArgList6<Param0, Param1, Param2, Param3, Param4, Param5> paramList(p0, p1, p2, p3, p4, p5);
		    return invokeDelegate(deleg_, stored_, paramList);
		}
	    };
	    //------------------------------------------------------------------------------

	    template<class Param0, class Param1, class Param2, class Param3, class Param4, class Param5, class Param6, class DelegateClass, class StoredListClass>
	    class ConnectionEx7 : public Connection7<Param0, Param1, Param2, Param3, Param4, Param5, Param6>
	    {
	    public:
		typedef Connection7<Param0, Param1, Param2, Param3, Param4, Param5, Param6> BaseType;
		typedef ConnectionEx7<Param0, Param1, Param2, Param3, Param4, Param5, Param6, DelegateClass, StoredListClass> ThisType;

		ConnectionEx7 (DelegateClass const & deleg, StoredListClass const & stored)
		: BaseType (AbstractDelegate (deleg))
		, deleg_ (deleg), stored_ (stored)
		{
		    BaseType::delegate_ = fastdelegate::MakeDelegate(this, &ThisType::invokeHelper);
		}
	    private:
		DelegateClass deleg_;
		StoredListClass stored_;

		void invokeHelper (Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6) const
		{
		    ArgList7<Param0, Param1, Param2, Param3, Param4, Param5, Param6> paramList(p0, p1, p2, p3, p4, p5, p6);
		    return invokeDelegate(deleg_, stored_, paramList);
		}
	    };
	    //------------------------------------------------------------------------------

	    template<class Param0, class Param1, class Param2, class Param3, class Param4, class Param5, class Param6, class Param7, class DelegateClass, class StoredListClass>
	    class ConnectionEx8 : public Connection8<Param0, Param1, Param2, Param3, Param4, Param5, Param6, Param7>
	    {
	    public:
		typedef Connection8<Param0, Param1, Param2, Param3, Param4, Param5, Param6, Param7> BaseType;
		typedef ConnectionEx8<Param0, Param1, Param2, Param3, Param4, Param5, Param6, Param7, DelegateClass, StoredListClass> ThisType;

		ConnectionEx8 (DelegateClass const & deleg, StoredListClass const & stored)
		: BaseType (AbstractDelegate (deleg))
		, deleg_ (deleg), stored_ (stored)
		{
		    BaseType::delegate_ = fastdelegate::MakeDelegate(this, &ThisType::invokeHelper);
		}
	    private:
		DelegateClass deleg_;
		StoredListClass stored_;

		void invokeHelper (Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7) const
		{
		    ArgList8<Param0, Param1, Param2, Param3, Param4, Param5, Param6, Param7> paramList(p0, p1, p2, p3, p4, p5, p6, p7);
		    return invokeDelegate(deleg_, stored_, paramList);
		}
	    };
	    //------------------------------------------------------------------------------
	} //namespace Events
    } //namespace Private
} //namespace Cpp

#endif //__CPP__EVENTS__CONNECTION_EX__HPP