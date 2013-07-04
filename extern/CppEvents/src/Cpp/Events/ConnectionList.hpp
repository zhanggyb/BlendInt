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

#ifndef __CPP__EVENTS__CONNECTION_LIST__HPP
#define __CPP__EVENTS__CONNECTION_LIST__HPP

#include <Cpp/Events/BorrowableData.hpp>

namespace Cpp {
    namespace Private {
	namespace Events {
	    //------------------------------------------------------------------------------

	    class ConnectionList
	    {
		friend void AbstractConnection::doDisconnect ();
		CPP_DISABLE_COPY (ConnectionList)
	    public:
		class FireLock;

		ConnectionList ();
		~ConnectionList ();

		void connect (ConnectionList * peer, AbstractConnection * conn);

		size_t connectionCount () const;
		size_t connectionCount (AbstractDelegate const & deleg) const;
		size_t connectionCount (ConnectionList * peer) const;
		size_t connectionCount (ConnectionList * peer, AbstractDelegate const & deleg) const;

		bool hasConnections () const;
		bool hasConnections (AbstractDelegate const & deleg) const;
		bool hasConnections (ConnectionList * peer) const;
		bool hasConnections (ConnectionList * peer, AbstractDelegate const & deleg) const;

		size_t disconnectAll ();
		size_t disconnectAll (AbstractDelegate const & deleg);
		size_t disconnectAll (ConnectionList * peer);
		size_t disconnectAll (ConnectionList * peer, AbstractDelegate const & deleg);

		bool disconnectOne (AbstractDelegate const & deleg);
		bool disconnectOne (ConnectionList * peer);
		bool disconnectOne (ConnectionList * peer, AbstractDelegate const & deleg);
	    private:
		class NullComparer;
		class DelegateComparer;
		class PeerComparer;
		class FullComparer;

		mutable ThreadDataRef lock_;
		BorrowableData data_;

		template<class Comparer> inline size_t getConnectionCount (Comparer const &) const;
		template<class Comparer> inline bool getHasConnections (Comparer const &) const;
		template<class Comparer> inline size_t doDisconnectAll (Comparer const &);
		template<class Comparer> inline bool doDisconnectOne (Comparer const &);
	    };
	    //------------------------------------------------------------------------------

	    class ConnectionList::FireLock
	    {

		CPP_DISABLE_COPY (FireLock)
	    public:
		FireLock (ConnectionList const * list)
		: locker_ (list->lock_)
		, borrower_ (&list->data_)
		{
		}

		ConnectionsVector const & constData () const
		{
		    return borrower_.constData();
		}
	    private:
		ThreadDataLocker locker_;
		BorrowableData::Borrower borrower_;
	    };
	    //------------------------------------------------------------------------------
	} //namespace Events
    } //namespace Private
} //namespace Cpp

#endif //__CPP__EVENTS__CONNECTION_LIST__HPP
