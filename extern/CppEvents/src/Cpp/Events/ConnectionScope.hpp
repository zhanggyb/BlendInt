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

#ifndef __CPP__EVENTS__CONNECTION_SCOPE__HPP
#define __CPP__EVENTS__CONNECTION_SCOPE__HPP

#include <Cpp/Events/Event.hpp>

namespace Cpp {
    //------------------------------------------------------------------------------
    //!High-level wrapper for ConnectionList

    class ConnectionScope
    {
    public:

	size_t connectionCount () const
	{
	    return connectionList_.connectionCount();
	}

	size_t connectionCount (AbstractDelegate const & deleg) const
	{
	    return connectionList_.connectionCount(deleg);
	}

	size_t connectionCount (AbstractEventRef const & ev) const
	{
	    return connectionList_.connectionCount(getConnList(ev));
	}

	size_t connectionCount (AbstractEventRef const & ev, AbstractDelegate const & deleg) const
	{
	    return connectionList_.connectionCount(getConnList(ev), deleg);
	}

	bool hasConnections () const
	{
	    return connectionList_.hasConnections();
	}

	bool hasConnections (AbstractDelegate const & deleg) const
	{
	    return connectionList_.hasConnections(deleg);
	}

	bool hasConnections (AbstractEventRef const & ev) const
	{
	    return connectionList_.hasConnections(getConnList(ev));
	}

	bool hasConnections (AbstractEventRef const & ev, AbstractDelegate const & deleg) const
	{
	    return connectionList_.hasConnections(getConnList(ev), deleg);
	}

	size_t disconnectAll ()
	{
	    return connectionList_.disconnectAll();
	}

	size_t disconnectAll (AbstractDelegate const & deleg)
	{
	    return connectionList_.disconnectAll(deleg);
	}

	size_t disconnectAll (AbstractEventRef const & ev)
	{
	    return connectionList_.disconnectAll(getConnList(ev));
	}

	size_t disconnectAll (AbstractEventRef const & ev, AbstractDelegate const & deleg)
	{
	    return connectionList_.disconnectAll(getConnList(ev), deleg);
	}

	bool disconnectOne (AbstractDelegate const & deleg)
	{
	    return connectionList_.disconnectOne(deleg);
	}

	bool disconnectOne (AbstractEventRef const & ev)
	{
	    return connectionList_.disconnectOne(getConnList(ev));
	}

	bool disconnectOne (AbstractEventRef const & ev, AbstractDelegate const & deleg)
	{
	    return connectionList_.disconnectOne(getConnList(ev), deleg);
	}

	template<class T, class Y> size_t connectionCount (T * obj, Y pMemberFunc) const
	{
	    return connectionCount(AbstractDelegate(obj, pMemberFunc));
	}

	template<class T, class Y> size_t connectionCount (AbstractEventRef const & ev, T * obj, Y pMemberFunc) const
	{
	    return connectionCount(ev, AbstractDelegate(obj, pMemberFunc));
	}

	template<class T, class Y> bool hasConnections (T * obj, Y pMemberFunc) const
	{
	    return hasConnections(AbstractDelegate(obj, pMemberFunc));
	}

	template<class T, class Y> bool hasConnections (AbstractEventRef const & ev, T * obj, Y pMemberFunc) const
	{
	    return hasConnections(ev, AbstractDelegate(obj, pMemberFunc));
	}

	template<class T, class Y> size_t disconnectOne (T * obj, Y pMemberFunc)
	{
	    return disconnectOne(AbstractDelegate(obj, pMemberFunc));
	}

	template<class T, class Y> size_t disconnectOne (AbstractEventRef const & ev, T * obj, Y pMemberFunc)
	{
	    return disconnectOne(ev, AbstractDelegate(obj, pMemberFunc));
	}

	template<class T, class Y> size_t disconnectAll (T * obj, Y pMemberFunc)
	{
	    return disconnectAll(AbstractDelegate(obj, pMemberFunc));
	}

	template<class T, class Y> size_t disconnectAll (AbstractEventRef const & ev, T * obj, Y pMemberFunc)
	{
	    return disconnectAll(ev, AbstractDelegate(obj, pMemberFunc));
	}

	template<class EvRefT, class T1, class T2>
	void connect (EvRefT evRef, T1 x1, T2 x2)
	{
	    evRef.connect(&connectionList_, x1, x2);
	}

	template<class EvRefT, class T1, class T2, class T3>
	void connect (EvRefT evRef, T1 x1, T2 x2, T3 x3)
	{
	    evRef.connect(&connectionList_, x1, x2, x3);
	}

	template<class EvRefT, class T1, class T2, class T3, class T4>
	void connect (EvRefT evRef, T1 x1, T2 x2, T3 x3, T4 x4)
	{
	    evRef.connect(&connectionList_, x1, x2, x3, x4);
	}

	template<class EvRefT, class T1, class T2, class T3, class T4, class T5>
	void connect (EvRefT evRef, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5)
	{
	    evRef.connect(&connectionList_, x1, x2, x3, x4, x5);
	}

	template<class EvRefT, class T1, class T2, class T3, class T4, class T5, class T6>
	void connect (EvRefT evRef, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6)
	{
	    evRef.connect(&connectionList_, x1, x2, x3, x4, x5, x6);
	}

	template<class EvRefT, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	void connect (EvRefT evRef, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6, T7 x7)
	{
	    evRef.connect(&connectionList_, x1, x2, x3, x4, x5, x6, x7);
	}

	template<class EvRefT, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	void connect (EvRefT evRef, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6, T7 x7, T8 x8)
	{
	    evRef.connect(&connectionList_, x1, x2, x3, x4, x5, x6, x7, x8);
	}

	template<class EvRefT, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
	void connect (EvRefT evRef, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6, T7 x7, T8 x8, T9 x9)
	{
	    evRef.connect(&connectionList_, x1, x2, x3, x4, x5, x6, x7, x8, x9);
	}

	template<class EvRefT, class T1>
	void connectEvent (EvRefT evRef, T1 x1)
	{
	    evRef.connectEvent(&connectionList_, x1);
	}

	template<class EvRefT, class T1, class T2>
	void connectEvent (EvRefT evRef, T1 x1, T2 x2)
	{
	    evRef.connectEvent(&connectionList_, x1, x2);
	}

	template<class EvRefT, class T1, class T2, class T3>
	void connectEvent (EvRefT evRef, T1 x1, T2 x2, T3 x3)
	{
	    evRef.connectEvent(&connectionList_, x1, x2, x3);
	}

	template<class EvRefT, class T1, class T2, class T3, class T4>
	void connectEvent (EvRefT evRef, T1 x1, T2 x2, T3 x3, T4 x4)
	{
	    evRef.connectEvent(&connectionList_, x1, x2, x3, x4);
	}

	template<class EvRefT, class T1, class T2, class T3, class T4, class T5>
	void connectEvent (EvRefT evRef, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5)
	{
	    evRef.connectEvent(&connectionList_, x1, x2, x3, x4, x5);
	}

	template<class EvRefT, class T1, class T2, class T3, class T4, class T5, class T6>
	void connectEvent (EvRefT evRef, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6)
	{
	    evRef.connectEvent(&connectionList_, x1, x2, x3, x4, x5, x6);
	}

	template<class EvRefT, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	void connectEvent (EvRefT evRef, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6, T7 x7)
	{
	    evRef.connectEvent(&connectionList_, x1, x2, x3, x4, x5, x6, x7);
	}

	template<class EvRefT, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	void connectEvent (EvRefT evRef, T1 x1, T2 x2, T3 x3, T4 x4, T5 x5, T6 x6, T7 x7, T8 x8)
	{
	    evRef.connectEvent(&connectionList_, x1, x2, x3, x4, x5, x6, x7, x8);
	}

    private:
	Private::Events::ConnectionList connectionList_;

	static Private::Events::ConnectionList * getConnList (AbstractEventRef const & ev)
	{
	    return &ev.event_->connectionList_;
	}
    };
    //------------------------------------------------------------------------------
} //namespace Cpp

#endif //__CPP__EVENTS__CONNECTION_SCOPE__HPP