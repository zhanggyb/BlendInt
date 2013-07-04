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

#ifndef __CPP__EVENTS__ABSTRACT_EVENT__HPP
#define __CPP__EVENTS__ABSTRACT_EVENT__HPP

#include <Cpp/Events/ConnectionList.hpp>

namespace Cpp {
    //------------------------------------------------------------------------------
    class ConnectionScope;
    //------------------------------------------------------------------------------

    class AbstractEvent
    {
	friend class ConnectionScope;

	CPP_DISABLE_COPY (AbstractEvent)
    protected:
	AbstractEvent ()
	{
	}

	~AbstractEvent ()
	{
	}
    public:

	size_t connectionCount () const
	{
	    return connectionList_.connectionCount();
	}

	size_t connectionCount (AbstractDelegate const & deleg) const
	{
	    return connectionList_.connectionCount(deleg);
	}

	template<class T, class Y> size_t connectionCount (T * obj, Y pMemberFunc) const
	{
	    return connectionCount(AbstractDelegate(obj, pMemberFunc));
	}

	bool hasConnections () const
	{
	    return connectionList_.hasConnections();
	}

	bool hasConnections (AbstractDelegate const & deleg) const
	{
	    return connectionList_.hasConnections(deleg);
	}

	template<class T, class Y> bool hasConnections (T * obj, Y pMemberFunc) const
	{
	    return hasConnections(AbstractDelegate(obj, pMemberFunc));
	}

	size_t disconnectAll ()
	{
	    return connectionList_.disconnectAll();
	}

	size_t disconnectAll (AbstractDelegate const & deleg)
	{
	    return connectionList_.disconnectAll(deleg);
	}

	bool disconnectOne (AbstractDelegate const & deleg)
	{
	    return connectionList_.disconnectOne(deleg);
	}

	template<class T, class Y> size_t disconnectAll (T * obj, Y pMemberFunc)
	{
	    return disconnectAll(AbstractDelegate(obj, pMemberFunc));
	}

	template<class T, class Y> bool disconnectOne (T * obj, Y pMemberFunc)
	{
	    return disconnectOne(AbstractDelegate(obj, pMemberFunc));
	}
    protected:

	class FireLock
	{
	public:

	    FireLock (AbstractEvent const * e)
	    : impl_ (&e->connectionList_)
	    {
	    }

	    Private::Events::ConnectionsVector const & connections () const
	    {
		return impl_.constData();
	    }
	private:
	    Private::Events::ConnectionList::FireLock impl_;
	};

	void addConnection (Private::Events::ConnectionList * tracker, Private::Events::AbstractConnection * conn)
	{
	    connectionList_.connect(tracker, conn);
	}
    private:
	Private::Events::ConnectionList connectionList_;
    };
    //------------------------------------------------------------------------------

    class AbstractEventRef
    {
	friend class ConnectionScope;
    public:

	AbstractEventRef (AbstractEvent * ev)
	: event_ (ev)
	{
	}

	AbstractEventRef (AbstractEventRef const & other)
	: event_ (other.event_)
	{
	}

	bool operator== (AbstractEventRef const & other) const
	{
	    return event_ == other.event_;
	}

	bool operator!= (AbstractEventRef const & other) const
	{
	    return event_ != other.event_;
	}

	size_t connectionCount () const
	{
	    return event_->connectionCount();
	}

	size_t connectionCount (AbstractDelegate const & deleg) const
	{
	    return event_->connectionCount(deleg);
	}

	template<class T, class Y> size_t connectionCount (T * reciever, Y pMemberFunc) const
	{
	    return event_->connectionCount(reciever, pMemberFunc);
	}

	bool hasConnections () const
	{
	    return event_->hasConnections();
	}

	bool hasConnections (AbstractDelegate const & deleg) const
	{
	    return event_->hasConnections(deleg);
	}

	template<class T, class Y> bool hasConnections (T * reciever, Y pMemberFunc) const
	{
	    return event_->hasConnections(reciever, pMemberFunc);
	}

	size_t disconnectAll ()
	{
	    return event_->disconnectAll();
	}

	size_t disconnectAll (AbstractDelegate const & deleg)
	{
	    return event_->disconnectAll(deleg);
	}

	template<class T, class Y> size_t disconnectAll (T * reciever, Y pMemberFunc)
	{
	    return event_->disconnectAll(reciever, pMemberFunc);
	}

	bool disconnectOne (AbstractDelegate const & deleg)
	{
	    return event_->disconnectOne(deleg);
	}

	template<class T, class Y> bool disconnectOne (T * reciever, Y pMemberFunc)
	{
	    return event_->disconnectOne(reciever, pMemberFunc);
	}
    protected:

	AbstractEvent * senderEvent () const
	{
	    return event_;
	}
    private:
	AbstractEvent * event_;
    };
    //------------------------------------------------------------------------------
} //namespace Cpp

#endif //__CPP__EVENTS__ABSTRACT_EVENT__HPP