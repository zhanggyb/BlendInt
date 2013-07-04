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

#ifndef __CPP__EVENTS__EVENT__HPP
#define __CPP__EVENTS__EVENT__HPP

#include <Cpp/Events/AbstractEvent.hpp>
#include <Cpp/Events/ConnectionEx.hpp>
#include <Cpp/Utils/TypeTraits.hpp>

namespace Cpp {
    //------------------------------------------------------------------------------
    template<
    class Param0 = Meta::NullType,
    class Param1 = Meta::NullType,
    class Param2 = Meta::NullType,
    class Param3 = Meta::NullType,
    class Param4 = Meta::NullType,
    class Param5 = Meta::NullType,
    class Param6 = Meta::NullType,
    class Param7 = Meta::NullType,
    class Param8 = Meta::NullType
    > class Event;
    //------------------------------------------------------------------------------
    template<
    class Param0 = Meta::NullType,
    class Param1 = Meta::NullType,
    class Param2 = Meta::NullType,
    class Param3 = Meta::NullType,
    class Param4 = Meta::NullType,
    class Param5 = Meta::NullType,
    class Param6 = Meta::NullType,
    class Param7 = Meta::NullType,
    class Param8 = Meta::NullType
    > class EventRef;
    //------------------------------------------------------------------------------

    template<>
    class Event<Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType> : public AbstractEvent
    {
    public:
	typedef Private::Events::Connection0 ConnectionType;

	Event ()
	{
	}

	~Event ()
	{
	}

	void addConnection (Private::Events::ConnectionList * tracker, ConnectionType * conn)
	{
	    AbstractEvent::addConnection(tracker, conn);
	}

	void fire () const
	{
	    FireLock lock(this);
	    typedef Private::Events::ConnectionsVector CV;
	    CV const & conns = lock.connections();
	    for (CV::const_iterator it = conns.begin(); it != conns.end(); ++it) {
		static_cast<ConnectionType const *> (*it)->invoke();
	    }
	}
    };
    //------------------------------------------------------------------------------

    template<>
    class EventRef<Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType> : public AbstractEventRef
    {
    public:
	typedef Event<Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType> EventType;
	typedef EventType::ConnectionType ConnectionType;

	EventRef (EventType & ev) : AbstractEventRef (&ev)
	{
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T * obj, void (Y::*pmf)())
	{
	    ConnectionType * conn = new ConnectionType(
						       fastdelegate::MakeDelegate(obj, pmf)
						       );
	    addConnection(tracker, conn);
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T const * obj, void (Y::*pmf)() const)
	{
	    ConnectionType * conn = new ConnectionType(
						       fastdelegate::MakeDelegate(obj, pmf)
						       );
	    addConnection(tracker, conn);
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf)
	{
	    Private::Events::ArgList0 stored;
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    Private::Events::ArgList1<S1> stored(x1);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    Private::Events::ArgList2<S1, S2> stored(x1, x2);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    Private::Events::ArgList3<S1, S2, S3> stored(x1, x2, x3);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    Private::Events::ArgList4<S1, S2, S3, S4> stored(x1, x2, x3, x4);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    Private::Events::ArgList5<S1, S2, S3, S4, S5> stored(x1, x2, x3, x4, x5);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5, class T6> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    typedef typename TypeTraits::StorageType<T6>::Type S6;
	    Private::Events::ArgList6<S1, S2, S3, S4, S5, S6> stored(x1, x2, x3, x4, x5, x6);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5, class T6, class T7> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6, T7 x7)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    typedef typename TypeTraits::StorageType<T6>::Type S6;
	    typedef typename TypeTraits::StorageType<T7>::Type S7;
	    Private::Events::ArgList7<S1, S2, S3, S4, S5, S6, S7> stored(x1, x2, x3, x4, x5, x6, x7);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	void connectEvent (Private::Events::ConnectionList * tracker, Event<> * ev)
	{
	    connect(tracker, ev, &Event<>::fire);
	}

	template<class T0> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0> * ev, T0 x0)
	{
	    connect(tracker, ev, &Event<T0>::fire, x0);
	}

	template<class T0, class T1> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1> * ev, T0 x0, T1 x1)
	{
	    connect(tracker, ev, &Event<T0, T1>::fire, x0, x1);
	}

	template<class T0, class T1, class T2> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2> * ev, T0 x0, T1 x1, T2 x2)
	{
	    connect(tracker, ev, &Event<T0, T1, T2>::fire, x0, x1, x2);
	}

	template<class T0, class T1, class T2, class T3> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3> * ev, T0 x0, T1 x1, T2 x2, T3 x3)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3>::fire, x0, x1, x2, x3);
	}

	template<class T0, class T1, class T2, class T3, class T4> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3, T4> * ev, T0 x0, T1 x1, T2 x2, T3 x3, T4 x4)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3, T4>::fire, x0, x1, x2, x3, x4);
	}

	template<class T0, class T1, class T2, class T3, class T4, class T5> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3, T4, T5> * ev, T0 x0, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3, T4, T5>::fire, x0, x1, x2, x3, x4, x5);
	}

	template<class T0, class T1, class T2, class T3, class T4, class T5, class T6> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3, T4, T5, T6> * ev, T0 x0, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3, T4, T5, T6>::fire, x0, x1, x2, x3, x4, x5, x6);
	}
    private:

	void addConnection (Private::Events::ConnectionList * tracker, ConnectionType * conn)
	{
	    static_cast<EventType*> (senderEvent())->addConnection(tracker, conn);
	}

	template<class DelegateClass, class StoredListClass> void connectEx (Private::Events::ConnectionList * tracker, DelegateClass const & deleg, StoredListClass const & stored)
	{
	    ConnectionType * conn = new Private::Events::ConnectionEx0<DelegateClass, StoredListClass>(
		    deleg, stored
		    );
	    addConnection(tracker, conn);
	}
    };
    //------------------------------------------------------------------------------

    template<class Param0>
    class Event<Param0, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType> : public AbstractEvent
    {
    public:
	typedef Private::Events::Connection1<Param0> ConnectionType;

	Event ()
	{
	}

	~Event ()
	{
	}

	void addConnection (Private::Events::ConnectionList * tracker, ConnectionType * conn)
	{
	    AbstractEvent::addConnection(tracker, conn);
	}

	void fire (Param0 p0) const
	{
	    FireLock lock(this);
	    typedef Private::Events::ConnectionsVector CV;
	    CV const & conns = lock.connections();
	    for (CV::const_iterator it = conns.begin(); it != conns.end(); ++it) {
		static_cast<ConnectionType const *> (*it)->invoke(p0);
	    }
	}
    };
    //------------------------------------------------------------------------------

    template<class Param0>
    class EventRef<Param0, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType> : public AbstractEventRef
    {
    public:
	typedef Event<Param0, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType> EventType;
	typedef typename EventType::ConnectionType ConnectionType;

	EventRef (EventType & ev) : AbstractEventRef (&ev)
	{
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T * obj, void (Y::*pmf)(Param0 p0))
	{
	    ConnectionType * conn = new ConnectionType(
						       fastdelegate::MakeDelegate(obj, pmf)
						       );
	    addConnection(tracker, conn);
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T const * obj, void (Y::*pmf)(Param0 p0) const)
	{
	    ConnectionType * conn = new ConnectionType(
						       fastdelegate::MakeDelegate(obj, pmf)
						       );
	    addConnection(tracker, conn);
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf)
	{
	    Private::Events::ArgList0 stored;
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    Private::Events::ArgList1<S1> stored(x1);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    Private::Events::ArgList2<S1, S2> stored(x1, x2);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    Private::Events::ArgList3<S1, S2, S3> stored(x1, x2, x3);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    Private::Events::ArgList4<S1, S2, S3, S4> stored(x1, x2, x3, x4);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    Private::Events::ArgList5<S1, S2, S3, S4, S5> stored(x1, x2, x3, x4, x5);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5, class T6> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    typedef typename TypeTraits::StorageType<T6>::Type S6;
	    Private::Events::ArgList6<S1, S2, S3, S4, S5, S6> stored(x1, x2, x3, x4, x5, x6);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5, class T6, class T7> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6, T7 x7)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    typedef typename TypeTraits::StorageType<T6>::Type S6;
	    typedef typename TypeTraits::StorageType<T7>::Type S7;
	    Private::Events::ArgList7<S1, S2, S3, S4, S5, S6, S7> stored(x1, x2, x3, x4, x5, x6, x7);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T0> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0> * ev)
	{
	    connect(tracker, ev, &Event<T0>::fire);
	}

	template<class T0, class T1> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1> * ev, T0 x0)
	{
	    connect(tracker, ev, &Event<T0, T1>::fire, x0);
	}

	template<class T0, class T1, class T2> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2> * ev, T0 x0, T1 x1)
	{
	    connect(tracker, ev, &Event<T0, T1, T2>::fire, x0, x1);
	}

	template<class T0, class T1, class T2, class T3> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3> * ev, T0 x0, T1 x1, T2 x2)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3>::fire, x0, x1, x2);
	}

	template<class T0, class T1, class T2, class T3, class T4> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3, T4> * ev, T0 x0, T1 x1, T2 x2, T3 x3)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3, T4>::fire, x0, x1, x2, x3);
	}

	template<class T0, class T1, class T2, class T3, class T4, class T5> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3, T4, T5> * ev, T0 x0, T1 x1, T2 x2, T3 x3, T4 x4)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3, T4, T5>::fire, x0, x1, x2, x3, x4);
	}

	template<class T0, class T1, class T2, class T3, class T4, class T5, class T6> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3, T4, T5, T6> * ev, T0 x0, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3, T4, T5, T6>::fire, x0, x1, x2, x3, x4, x5);
	}
    private:

	void addConnection (Private::Events::ConnectionList * tracker, ConnectionType * conn)
	{
	    static_cast<EventType*> (senderEvent())->addConnection(tracker, conn);
	}

	template<class DelegateClass, class StoredListClass> void connectEx (Private::Events::ConnectionList * tracker, DelegateClass const & deleg, StoredListClass const & stored)
	{
	    ConnectionType * conn = new Private::Events::ConnectionEx1<Param0, DelegateClass, StoredListClass>(
		    deleg, stored
		    );
	    addConnection(tracker, conn);
	}
    };
    //------------------------------------------------------------------------------

    template<class Param0, class Param1>
    class Event<Param0, Param1, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType> : public AbstractEvent
    {
    public:
	typedef Private::Events::Connection2<Param0, Param1> ConnectionType;

	Event ()
	{
	}

	~Event ()
	{
	}

	void addConnection (Private::Events::ConnectionList * tracker, ConnectionType * conn)
	{
	    AbstractEvent::addConnection(tracker, conn);
	}

	void fire (Param0 p0, Param1 p1) const
	{
	    FireLock lock(this);
	    typedef Private::Events::ConnectionsVector CV;
	    CV const & conns = lock.connections();
	    for (CV::const_iterator it = conns.begin(); it != conns.end(); ++it) {
		static_cast<ConnectionType const *> (*it)->invoke(p0, p1);
	    }
	}
    };
    //------------------------------------------------------------------------------

    template<class Param0, class Param1>
    class EventRef<Param0, Param1, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType> : public AbstractEventRef
    {
    public:
	typedef Event<Param0, Param1, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType> EventType;
	typedef typename EventType::ConnectionType ConnectionType;

	EventRef (EventType & ev) : AbstractEventRef (&ev)
	{
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T * obj, void (Y::*pmf)(Param0 p0, Param1 p1))
	{
	    ConnectionType * conn = new ConnectionType(
						       fastdelegate::MakeDelegate(obj, pmf)
						       );
	    addConnection(tracker, conn);
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T const * obj, void (Y::*pmf)(Param0 p0, Param1 p1) const)
	{
	    ConnectionType * conn = new ConnectionType(
						       fastdelegate::MakeDelegate(obj, pmf)
						       );
	    addConnection(tracker, conn);
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf)
	{
	    Private::Events::ArgList0 stored;
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    Private::Events::ArgList1<S1> stored(x1);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    Private::Events::ArgList2<S1, S2> stored(x1, x2);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    Private::Events::ArgList3<S1, S2, S3> stored(x1, x2, x3);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    Private::Events::ArgList4<S1, S2, S3, S4> stored(x1, x2, x3, x4);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    Private::Events::ArgList5<S1, S2, S3, S4, S5> stored(x1, x2, x3, x4, x5);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5, class T6> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    typedef typename TypeTraits::StorageType<T6>::Type S6;
	    Private::Events::ArgList6<S1, S2, S3, S4, S5, S6> stored(x1, x2, x3, x4, x5, x6);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5, class T6, class T7> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6, T7 x7)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    typedef typename TypeTraits::StorageType<T6>::Type S6;
	    typedef typename TypeTraits::StorageType<T7>::Type S7;
	    Private::Events::ArgList7<S1, S2, S3, S4, S5, S6, S7> stored(x1, x2, x3, x4, x5, x6, x7);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T0, class T1> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1> * ev)
	{
	    connect(tracker, ev, &Event<T0, T1>::fire);
	}

	template<class T0, class T1, class T2> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2> * ev, T0 x0)
	{
	    connect(tracker, ev, &Event<T0, T1, T2>::fire, x0);
	}

	template<class T0, class T1, class T2, class T3> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3> * ev, T0 x0, T1 x1)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3>::fire, x0, x1);
	}

	template<class T0, class T1, class T2, class T3, class T4> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3, T4> * ev, T0 x0, T1 x1, T2 x2)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3, T4>::fire, x0, x1, x2);
	}

	template<class T0, class T1, class T2, class T3, class T4, class T5> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3, T4, T5> * ev, T0 x0, T1 x1, T2 x2, T3 x3)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3, T4, T5>::fire, x0, x1, x2, x3);
	}

	template<class T0, class T1, class T2, class T3, class T4, class T5, class T6> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3, T4, T5, T6> * ev, T0 x0, T1 x1, T2 x2, T3 x3, T4 x4)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3, T4, T5, T6>::fire, x0, x1, x2, x3, x4);
	}
    private:

	void addConnection (Private::Events::ConnectionList * tracker, ConnectionType * conn)
	{
	    static_cast<EventType*> (senderEvent())->addConnection(tracker, conn);
	}

	template<class DelegateClass, class StoredListClass> void connectEx (Private::Events::ConnectionList * tracker, DelegateClass const & deleg, StoredListClass const & stored)
	{
	    ConnectionType * conn = new Private::Events::ConnectionEx2<Param0, Param1, DelegateClass, StoredListClass>(
		    deleg, stored
		    );
	    addConnection(tracker, conn);
	}
    };
    //------------------------------------------------------------------------------

    template<class Param0, class Param1, class Param2>
    class Event<Param0, Param1, Param2, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType> : public AbstractEvent
    {
    public:
	typedef Private::Events::Connection3<Param0, Param1, Param2> ConnectionType;

	Event ()
	{
	}

	~Event ()
	{
	}

	void addConnection (Private::Events::ConnectionList * tracker, ConnectionType * conn)
	{
	    AbstractEvent::addConnection(tracker, conn);
	}

	void fire (Param0 p0, Param1 p1, Param2 p2) const
	{
	    FireLock lock(this);
	    typedef Private::Events::ConnectionsVector CV;
	    CV const & conns = lock.connections();
	    for (CV::const_iterator it = conns.begin(); it != conns.end(); ++it) {
		static_cast<ConnectionType const *> (*it)->invoke(p0, p1, p2);
	    }
	}
    };
    //------------------------------------------------------------------------------

    template<class Param0, class Param1, class Param2>
    class EventRef<Param0, Param1, Param2, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType> : public AbstractEventRef
    {
    public:
	typedef Event<Param0, Param1, Param2, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType> EventType;
	typedef typename EventType::ConnectionType ConnectionType;

	EventRef (EventType & ev) : AbstractEventRef (&ev)
	{
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T * obj, void (Y::*pmf)(Param0 p0, Param1 p1, Param2 p2))
	{
	    ConnectionType * conn = new ConnectionType(
						       fastdelegate::MakeDelegate(obj, pmf)
						       );
	    addConnection(tracker, conn);
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T const * obj, void (Y::*pmf)(Param0 p0, Param1 p1, Param2 p2) const)
	{
	    ConnectionType * conn = new ConnectionType(
						       fastdelegate::MakeDelegate(obj, pmf)
						       );
	    addConnection(tracker, conn);
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf)
	{
	    Private::Events::ArgList0 stored;
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    Private::Events::ArgList1<S1> stored(x1);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    Private::Events::ArgList2<S1, S2> stored(x1, x2);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    Private::Events::ArgList3<S1, S2, S3> stored(x1, x2, x3);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    Private::Events::ArgList4<S1, S2, S3, S4> stored(x1, x2, x3, x4);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    Private::Events::ArgList5<S1, S2, S3, S4, S5> stored(x1, x2, x3, x4, x5);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5, class T6> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    typedef typename TypeTraits::StorageType<T6>::Type S6;
	    Private::Events::ArgList6<S1, S2, S3, S4, S5, S6> stored(x1, x2, x3, x4, x5, x6);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5, class T6, class T7> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6, T7 x7)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    typedef typename TypeTraits::StorageType<T6>::Type S6;
	    typedef typename TypeTraits::StorageType<T7>::Type S7;
	    Private::Events::ArgList7<S1, S2, S3, S4, S5, S6, S7> stored(x1, x2, x3, x4, x5, x6, x7);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T0, class T1, class T2> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2> * ev)
	{
	    connect(tracker, ev, &Event<T0, T1, T2>::fire);
	}

	template<class T0, class T1, class T2, class T3> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3> * ev, T0 x0)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3>::fire, x0);
	}

	template<class T0, class T1, class T2, class T3, class T4> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3, T4> * ev, T0 x0, T1 x1)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3, T4>::fire, x0, x1);
	}

	template<class T0, class T1, class T2, class T3, class T4, class T5> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3, T4, T5> * ev, T0 x0, T1 x1, T2 x2)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3, T4, T5>::fire, x0, x1, x2);
	}

	template<class T0, class T1, class T2, class T3, class T4, class T5, class T6> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3, T4, T5, T6> * ev, T0 x0, T1 x1, T2 x2, T3 x3)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3, T4, T5, T6>::fire, x0, x1, x2, x3);
	}
    private:

	void addConnection (Private::Events::ConnectionList * tracker, ConnectionType * conn)
	{
	    static_cast<EventType*> (senderEvent())->addConnection(tracker, conn);
	}

	template<class DelegateClass, class StoredListClass> void connectEx (Private::Events::ConnectionList * tracker, DelegateClass const & deleg, StoredListClass const & stored)
	{
	    ConnectionType * conn = new Private::Events::ConnectionEx3<Param0, Param1, Param2, DelegateClass, StoredListClass>(
		    deleg, stored
		    );
	    addConnection(tracker, conn);
	}
    };
    //------------------------------------------------------------------------------

    template<class Param0, class Param1, class Param2, class Param3>
    class Event<Param0, Param1, Param2, Param3, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType> : public AbstractEvent
    {
    public:
	typedef Private::Events::Connection4<Param0, Param1, Param2, Param3> ConnectionType;

	Event ()
	{
	}

	~Event ()
	{
	}

	void addConnection (Private::Events::ConnectionList * tracker, ConnectionType * conn)
	{
	    AbstractEvent::addConnection(tracker, conn);
	}

	void fire (Param0 p0, Param1 p1, Param2 p2, Param3 p3) const
	{
	    FireLock lock(this);
	    typedef Private::Events::ConnectionsVector CV;
	    CV const & conns = lock.connections();
	    for (CV::const_iterator it = conns.begin(); it != conns.end(); ++it) {
		static_cast<ConnectionType const *> (*it)->invoke(p0, p1, p2, p3);
	    }
	}
    };
    //------------------------------------------------------------------------------

    template<class Param0, class Param1, class Param2, class Param3>
    class EventRef<Param0, Param1, Param2, Param3, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType> : public AbstractEventRef
    {
    public:
	typedef Event<Param0, Param1, Param2, Param3, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType> EventType;
	typedef typename EventType::ConnectionType ConnectionType;

	EventRef (EventType & ev) : AbstractEventRef (&ev)
	{
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T * obj, void (Y::*pmf)(Param0 p0, Param1 p1, Param2 p2, Param3 p3))
	{
	    ConnectionType * conn = new ConnectionType(
						       fastdelegate::MakeDelegate(obj, pmf)
						       );
	    addConnection(tracker, conn);
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T const * obj, void (Y::*pmf)(Param0 p0, Param1 p1, Param2 p2, Param3 p3) const)
	{
	    ConnectionType * conn = new ConnectionType(
						       fastdelegate::MakeDelegate(obj, pmf)
						       );
	    addConnection(tracker, conn);
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf)
	{
	    Private::Events::ArgList0 stored;
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    Private::Events::ArgList1<S1> stored(x1);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    Private::Events::ArgList2<S1, S2> stored(x1, x2);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    Private::Events::ArgList3<S1, S2, S3> stored(x1, x2, x3);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    Private::Events::ArgList4<S1, S2, S3, S4> stored(x1, x2, x3, x4);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    Private::Events::ArgList5<S1, S2, S3, S4, S5> stored(x1, x2, x3, x4, x5);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5, class T6> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    typedef typename TypeTraits::StorageType<T6>::Type S6;
	    Private::Events::ArgList6<S1, S2, S3, S4, S5, S6> stored(x1, x2, x3, x4, x5, x6);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5, class T6, class T7> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6, T7 x7)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    typedef typename TypeTraits::StorageType<T6>::Type S6;
	    typedef typename TypeTraits::StorageType<T7>::Type S7;
	    Private::Events::ArgList7<S1, S2, S3, S4, S5, S6, S7> stored(x1, x2, x3, x4, x5, x6, x7);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T0, class T1, class T2, class T3> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3> * ev)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3>::fire);
	}

	template<class T0, class T1, class T2, class T3, class T4> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3, T4> * ev, T0 x0)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3, T4>::fire, x0);
	}

	template<class T0, class T1, class T2, class T3, class T4, class T5> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3, T4, T5> * ev, T0 x0, T1 x1)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3, T4, T5>::fire, x0, x1);
	}

	template<class T0, class T1, class T2, class T3, class T4, class T5, class T6> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3, T4, T5, T6> * ev, T0 x0, T1 x1, T2 x2)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3, T4, T5, T6>::fire, x0, x1, x2);
	}
    private:

	void addConnection (Private::Events::ConnectionList * tracker, ConnectionType * conn)
	{
	    static_cast<EventType*> (senderEvent())->addConnection(tracker, conn);
	}

	template<class DelegateClass, class StoredListClass> void connectEx (Private::Events::ConnectionList * tracker, DelegateClass const & deleg, StoredListClass const & stored)
	{
	    ConnectionType * conn = new Private::Events::ConnectionEx4<Param0, Param1, Param2, Param3, DelegateClass, StoredListClass>(
		    deleg, stored
		    );
	    addConnection(tracker, conn);
	}
    };
    //------------------------------------------------------------------------------

    template<class Param0, class Param1, class Param2, class Param3, class Param4>
    class Event<Param0, Param1, Param2, Param3, Param4, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType> : public AbstractEvent
    {
    public:
	typedef Private::Events::Connection5<Param0, Param1, Param2, Param3, Param4> ConnectionType;

	Event ()
	{
	}

	~Event ()
	{
	}

	void addConnection (Private::Events::ConnectionList * tracker, ConnectionType * conn)
	{
	    AbstractEvent::addConnection(tracker, conn);
	}

	void fire (Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4) const
	{
	    FireLock lock(this);
	    typedef Private::Events::ConnectionsVector CV;
	    CV const & conns = lock.connections();
	    for (CV::const_iterator it = conns.begin(); it != conns.end(); ++it) {
		static_cast<ConnectionType const *> (*it)->invoke(p0, p1, p2, p3, p4);
	    }
	}
    };
    //------------------------------------------------------------------------------

    template<class Param0, class Param1, class Param2, class Param3, class Param4>
    class EventRef<Param0, Param1, Param2, Param3, Param4, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType> : public AbstractEventRef
    {
    public:
	typedef Event<Param0, Param1, Param2, Param3, Param4, Meta::NullType, Meta::NullType, Meta::NullType, Meta::NullType> EventType;
	typedef typename EventType::ConnectionType ConnectionType;

	EventRef (EventType & ev) : AbstractEventRef (&ev)
	{
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T * obj, void (Y::*pmf)(Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4))
	{
	    ConnectionType * conn = new ConnectionType(
						       fastdelegate::MakeDelegate(obj, pmf)
						       );
	    addConnection(tracker, conn);
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T const * obj, void (Y::*pmf)(Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4) const)
	{
	    ConnectionType * conn = new ConnectionType(
						       fastdelegate::MakeDelegate(obj, pmf)
						       );
	    addConnection(tracker, conn);
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf)
	{
	    Private::Events::ArgList0 stored;
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    Private::Events::ArgList1<S1> stored(x1);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    Private::Events::ArgList2<S1, S2> stored(x1, x2);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    Private::Events::ArgList3<S1, S2, S3> stored(x1, x2, x3);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    Private::Events::ArgList4<S1, S2, S3, S4> stored(x1, x2, x3, x4);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    Private::Events::ArgList5<S1, S2, S3, S4, S5> stored(x1, x2, x3, x4, x5);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5, class T6> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    typedef typename TypeTraits::StorageType<T6>::Type S6;
	    Private::Events::ArgList6<S1, S2, S3, S4, S5, S6> stored(x1, x2, x3, x4, x5, x6);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5, class T6, class T7> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6, T7 x7)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    typedef typename TypeTraits::StorageType<T6>::Type S6;
	    typedef typename TypeTraits::StorageType<T7>::Type S7;
	    Private::Events::ArgList7<S1, S2, S3, S4, S5, S6, S7> stored(x1, x2, x3, x4, x5, x6, x7);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T0, class T1, class T2, class T3, class T4> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3, T4> * ev)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3, T4>::fire);
	}

	template<class T0, class T1, class T2, class T3, class T4, class T5> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3, T4, T5> * ev, T0 x0)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3, T4, T5>::fire, x0);
	}

	template<class T0, class T1, class T2, class T3, class T4, class T5, class T6> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3, T4, T5, T6> * ev, T0 x0, T1 x1)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3, T4, T5, T6>::fire, x0, x1);
	}
    private:

	void addConnection (Private::Events::ConnectionList * tracker, ConnectionType * conn)
	{
	    static_cast<EventType*> (senderEvent())->addConnection(tracker, conn);
	}

	template<class DelegateClass, class StoredListClass> void connectEx (Private::Events::ConnectionList * tracker, DelegateClass const & deleg, StoredListClass const & stored)
	{
	    ConnectionType * conn = new Private::Events::ConnectionEx5<Param0, Param1, Param2, Param3, Param4, DelegateClass, StoredListClass>(
		    deleg, stored
		    );
	    addConnection(tracker, conn);
	}
    };
    //------------------------------------------------------------------------------

    template<class Param0, class Param1, class Param2, class Param3, class Param4, class Param5>
    class Event<Param0, Param1, Param2, Param3, Param4, Param5, Meta::NullType, Meta::NullType, Meta::NullType> : public AbstractEvent
    {
    public:
	typedef Private::Events::Connection6<Param0, Param1, Param2, Param3, Param4, Param5> ConnectionType;

	Event ()
	{
	}

	~Event ()
	{
	}

	void addConnection (Private::Events::ConnectionList * tracker, ConnectionType * conn)
	{
	    AbstractEvent::addConnection(tracker, conn);
	}

	void fire (Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const
	{
	    FireLock lock(this);
	    typedef Private::Events::ConnectionsVector CV;
	    CV const & conns = lock.connections();
	    for (CV::const_iterator it = conns.begin(); it != conns.end(); ++it) {
		static_cast<ConnectionType const *> (*it)->invoke(p0, p1, p2, p3, p4, p5);
	    }
	}
    };
    //------------------------------------------------------------------------------

    template<class Param0, class Param1, class Param2, class Param3, class Param4, class Param5>
    class EventRef<Param0, Param1, Param2, Param3, Param4, Param5, Meta::NullType, Meta::NullType, Meta::NullType> : public AbstractEventRef
    {
    public:
	typedef Event<Param0, Param1, Param2, Param3, Param4, Param5, Meta::NullType, Meta::NullType, Meta::NullType> EventType;
	typedef typename EventType::ConnectionType ConnectionType;

	EventRef (EventType & ev) : AbstractEventRef (&ev)
	{
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T * obj, void (Y::*pmf)(Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5))
	{
	    ConnectionType * conn = new ConnectionType(
						       fastdelegate::MakeDelegate(obj, pmf)
						       );
	    addConnection(tracker, conn);
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T const * obj, void (Y::*pmf)(Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const)
	{
	    ConnectionType * conn = new ConnectionType(
						       fastdelegate::MakeDelegate(obj, pmf)
						       );
	    addConnection(tracker, conn);
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf)
	{
	    Private::Events::ArgList0 stored;
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    Private::Events::ArgList1<S1> stored(x1);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    Private::Events::ArgList2<S1, S2> stored(x1, x2);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    Private::Events::ArgList3<S1, S2, S3> stored(x1, x2, x3);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    Private::Events::ArgList4<S1, S2, S3, S4> stored(x1, x2, x3, x4);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    Private::Events::ArgList5<S1, S2, S3, S4, S5> stored(x1, x2, x3, x4, x5);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5, class T6> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    typedef typename TypeTraits::StorageType<T6>::Type S6;
	    Private::Events::ArgList6<S1, S2, S3, S4, S5, S6> stored(x1, x2, x3, x4, x5, x6);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5, class T6, class T7> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6, T7 x7)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    typedef typename TypeTraits::StorageType<T6>::Type S6;
	    typedef typename TypeTraits::StorageType<T7>::Type S7;
	    Private::Events::ArgList7<S1, S2, S3, S4, S5, S6, S7> stored(x1, x2, x3, x4, x5, x6, x7);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T0, class T1, class T2, class T3, class T4, class T5> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3, T4, T5> * ev)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3, T4, T5>::fire);
	}

	template<class T0, class T1, class T2, class T3, class T4, class T5, class T6> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3, T4, T5, T6> * ev, T0 x0)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3, T4, T5, T6>::fire, x0);
	}
    private:

	void addConnection (Private::Events::ConnectionList * tracker, ConnectionType * conn)
	{
	    static_cast<EventType*> (senderEvent())->addConnection(tracker, conn);
	}

	template<class DelegateClass, class StoredListClass> void connectEx (Private::Events::ConnectionList * tracker, DelegateClass const & deleg, StoredListClass const & stored)
	{
	    ConnectionType * conn = new Private::Events::ConnectionEx6<Param0, Param1, Param2, Param3, Param4, Param5, DelegateClass, StoredListClass>(
		    deleg, stored
		    );
	    addConnection(tracker, conn);
	}
    };
    //------------------------------------------------------------------------------

    template<class Param0, class Param1, class Param2, class Param3, class Param4, class Param5, class Param6>
    class Event<Param0, Param1, Param2, Param3, Param4, Param5, Param6, Meta::NullType, Meta::NullType> : public AbstractEvent
    {
    public:
	typedef Private::Events::Connection7<Param0, Param1, Param2, Param3, Param4, Param5, Param6> ConnectionType;

	Event ()
	{
	}

	~Event ()
	{
	}

	void addConnection (Private::Events::ConnectionList * tracker, ConnectionType * conn)
	{
	    AbstractEvent::addConnection(tracker, conn);
	}

	void fire (Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6) const
	{
	    FireLock lock(this);
	    typedef Private::Events::ConnectionsVector CV;
	    CV const & conns = lock.connections();
	    for (CV::const_iterator it = conns.begin(); it != conns.end(); ++it) {
		static_cast<ConnectionType const *> (*it)->invoke(p0, p1, p2, p3, p4, p5, p6);
	    }
	}
    };
    //------------------------------------------------------------------------------

    template<class Param0, class Param1, class Param2, class Param3, class Param4, class Param5, class Param6>
    class EventRef<Param0, Param1, Param2, Param3, Param4, Param5, Param6, Meta::NullType, Meta::NullType> : public AbstractEventRef
    {
    public:
	typedef Event<Param0, Param1, Param2, Param3, Param4, Param5, Param6, Meta::NullType, Meta::NullType> EventType;
	typedef typename EventType::ConnectionType ConnectionType;

	EventRef (EventType & ev) : AbstractEventRef (&ev)
	{
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T * obj, void (Y::*pmf)(Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6))
	{
	    ConnectionType * conn = new ConnectionType(
						       fastdelegate::MakeDelegate(obj, pmf)
						       );
	    addConnection(tracker, conn);
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T const * obj, void (Y::*pmf)(Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6) const)
	{
	    ConnectionType * conn = new ConnectionType(
						       fastdelegate::MakeDelegate(obj, pmf)
						       );
	    addConnection(tracker, conn);
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf)
	{
	    Private::Events::ArgList0 stored;
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    Private::Events::ArgList1<S1> stored(x1);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    Private::Events::ArgList2<S1, S2> stored(x1, x2);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    Private::Events::ArgList3<S1, S2, S3> stored(x1, x2, x3);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    Private::Events::ArgList4<S1, S2, S3, S4> stored(x1, x2, x3, x4);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    Private::Events::ArgList5<S1, S2, S3, S4, S5> stored(x1, x2, x3, x4, x5);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5, class T6> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    typedef typename TypeTraits::StorageType<T6>::Type S6;
	    Private::Events::ArgList6<S1, S2, S3, S4, S5, S6> stored(x1, x2, x3, x4, x5, x6);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5, class T6, class T7> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6, T7 x7)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    typedef typename TypeTraits::StorageType<T6>::Type S6;
	    typedef typename TypeTraits::StorageType<T7>::Type S7;
	    Private::Events::ArgList7<S1, S2, S3, S4, S5, S6, S7> stored(x1, x2, x3, x4, x5, x6, x7);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T0, class T1, class T2, class T3, class T4, class T5, class T6> void connectEvent (Private::Events::ConnectionList * tracker, Event<T0, T1, T2, T3, T4, T5, T6> * ev)
	{
	    connect(tracker, ev, &Event<T0, T1, T2, T3, T4, T5, T6>::fire);
	}
    private:

	void addConnection (Private::Events::ConnectionList * tracker, ConnectionType * conn)
	{
	    static_cast<EventType*> (senderEvent())->addConnection(tracker, conn);
	}

	template<class DelegateClass, class StoredListClass> void connectEx (Private::Events::ConnectionList * tracker, DelegateClass const & deleg, StoredListClass const & stored)
	{
	    ConnectionType * conn = new Private::Events::ConnectionEx7<Param0, Param1, Param2, Param3, Param4, Param5, Param6, DelegateClass, StoredListClass>(
		    deleg, stored
		    );
	    addConnection(tracker, conn);
	}
    };
    //------------------------------------------------------------------------------

    template<class Param0, class Param1, class Param2, class Param3, class Param4, class Param5, class Param6, class Param7>
    class Event<Param0, Param1, Param2, Param3, Param4, Param5, Param6, Param7, Meta::NullType> : public AbstractEvent
    {
    public:
	typedef Private::Events::Connection8<Param0, Param1, Param2, Param3, Param4, Param5, Param6, Param7> ConnectionType;

	Event ()
	{
	}

	~Event ()
	{
	}

	void addConnection (Private::Events::ConnectionList * tracker, ConnectionType * conn)
	{
	    AbstractEvent::addConnection(tracker, conn);
	}

	void fire (Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7) const
	{
	    FireLock lock(this);
	    typedef Private::Events::ConnectionsVector CV;
	    CV const & conns = lock.connections();
	    for (CV::const_iterator it = conns.begin(); it != conns.end(); ++it) {
		static_cast<ConnectionType const *> (*it)->invoke(p0, p1, p2, p3, p4, p5, p6, p7);
	    }
	}
    };
    //------------------------------------------------------------------------------

    template<class Param0, class Param1, class Param2, class Param3, class Param4, class Param5, class Param6, class Param7>
    class EventRef<Param0, Param1, Param2, Param3, Param4, Param5, Param6, Param7, Meta::NullType> : public AbstractEventRef
    {
    public:
	typedef Event<Param0, Param1, Param2, Param3, Param4, Param5, Param6, Param7, Meta::NullType> EventType;
	typedef typename EventType::ConnectionType ConnectionType;

	EventRef (EventType & ev) : AbstractEventRef (&ev)
	{
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T * obj, void (Y::*pmf)(Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7))
	{
	    ConnectionType * conn = new ConnectionType(
						       fastdelegate::MakeDelegate(obj, pmf)
						       );
	    addConnection(tracker, conn);
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T const * obj, void (Y::*pmf)(Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7) const)
	{
	    ConnectionType * conn = new ConnectionType(
						       fastdelegate::MakeDelegate(obj, pmf)
						       );
	    addConnection(tracker, conn);
	}

	template<class T, class Y> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf)
	{
	    Private::Events::ArgList0 stored;
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    Private::Events::ArgList1<S1> stored(x1);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    Private::Events::ArgList2<S1, S2> stored(x1, x2);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    Private::Events::ArgList3<S1, S2, S3> stored(x1, x2, x3);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    Private::Events::ArgList4<S1, S2, S3, S4> stored(x1, x2, x3, x4);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    Private::Events::ArgList5<S1, S2, S3, S4, S5> stored(x1, x2, x3, x4, x5);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5, class T6> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    typedef typename TypeTraits::StorageType<T6>::Type S6;
	    Private::Events::ArgList6<S1, S2, S3, S4, S5, S6> stored(x1, x2, x3, x4, x5, x6);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

	template<class T, class Y, class T1, class T2, class T3, class T4, class T5, class T6, class T7> void connect (Private::Events::ConnectionList * tracker, T * obj, Y pmf, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6, T7 x7)
	{
	    typedef typename TypeTraits::StorageType<T1>::Type S1;
	    typedef typename TypeTraits::StorageType<T2>::Type S2;
	    typedef typename TypeTraits::StorageType<T3>::Type S3;
	    typedef typename TypeTraits::StorageType<T4>::Type S4;
	    typedef typename TypeTraits::StorageType<T5>::Type S5;
	    typedef typename TypeTraits::StorageType<T6>::Type S6;
	    typedef typename TypeTraits::StorageType<T7>::Type S7;
	    Private::Events::ArgList7<S1, S2, S3, S4, S5, S6, S7> stored(x1, x2, x3, x4, x5, x6, x7);
	    connectEx(tracker, fastdelegate::MakeDelegate(obj, pmf), stored);
	}

    private:

	void addConnection (Private::Events::ConnectionList * tracker, ConnectionType * conn)
	{
	    static_cast<EventType*> (senderEvent())->addConnection(tracker, conn);
	}

	template<class DelegateClass, class StoredListClass> void connectEx (Private::Events::ConnectionList * tracker, DelegateClass const & deleg, StoredListClass const & stored)
	{
	    ConnectionType * conn = new Private::Events::ConnectionEx8<Param0, Param1, Param2, Param3, Param4, Param5, Param6, Param7, DelegateClass, StoredListClass>(
		    deleg, stored
		    );
	    addConnection(tracker, conn);
	}
    };
    //------------------------------------------------------------------------------
} //namespace Cpp

#endif //__CPP__EVENTS__EVENT__HPP