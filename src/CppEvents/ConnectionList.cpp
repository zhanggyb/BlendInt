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

#include <Cpp/Events/ConnectionList.hpp>

namespace Cpp {
	namespace Private {
		namespace Events {
			//------------------------------------------------------------------------------

			void AbstractConnection::disconnect ()
			{
				ThreadDataLocker lock1(outerLock_);
				ThreadDataLocker lock2(innerLock_);
				doDisconnect();
			}
			//------------------------------------------------------------------------------

			bool AbstractConnection::tryDisconnectWithLock (
			        ThreadDataRef const & lock)
			{
				if (lock != outerLock_)
					return false;
				ThreadDataLocker lock2(innerLock_);
				doDisconnect();
				return true;
			}
			//------------------------------------------------------------------------------

			void AbstractConnection::doDisconnect ()
			{
				if (!sourceList_ && !targetList_)
					return;
				assert(sourceList_ && targetList_);

				{
					ConnectionsVector & connections =
					        sourceList_->data_.mutableRef();
					AbstractConnection * & xs = connections[sourceIndex_];
					assert(xs == this);
					xs = connections.back();
					assert(xs->sourceList_ == sourceList_);
					assert(xs->sourceIndex_ == connections.size() - 1);
					xs->sourceIndex_ = sourceIndex_;
					connections.pop_back();
					sourceIndex_ = npos;
					sourceList_ = 0;
					release();
				}
				{
					ConnectionsVector & connections =
					        targetList_->data_.mutableRef();
					AbstractConnection * & xt = connections[targetIndex_];
					assert(xt == this);
					xt = connections.back();
					assert(xt->targetList_ == targetList_);
					assert(xt->targetIndex_ == connections.size() - 1);
					xt->targetIndex_ = targetIndex_;
					connections.pop_back();
					targetIndex_ = npos;
					targetList_ = 0;
					release();
				}
			}
			//------------------------------------------------------------------------------

			ConnectionList::ConnectionList ()
					: lock_(ThreadDataRef::current()), data_()
			{
			}
			//------------------------------------------------------------------------------

			ConnectionList::~ConnectionList ()
			{
				assert(!data_.isBorrowed());
				disconnectAll();
			}
			//------------------------------------------------------------------------------

			void ConnectionList::connect (ConnectionList * peer,
			        AbstractConnection * conn)
			{
				conn->outerLock_ = this->lock_;
				conn->innerLock_ = peer->lock_;
				conn->outerLock_.makeBefore(conn->innerLock_);

				ThreadDataLocker lock1(conn->outerLock_);
				ThreadDataLocker lock2(conn->innerLock_);

				{
					assert(
					        !conn->sourceList_
					                && conn->sourceIndex_
					                        == AbstractConnection::npos);
					ConnectionsVector & connections = data_.mutableRef();
					conn->sourceList_ = this;
					conn->sourceIndex_ = connections.size();
					connections.push_back(conn);
					conn->retain();
				}
				{
					assert(
					        !conn->targetList_
					                && conn->targetIndex_
					                        == AbstractConnection::npos);
					ConnectionsVector & connections = peer->data_.mutableRef();
					conn->targetList_ = peer;
					conn->targetIndex_ = connections.size();
					connections.push_back(conn);
					conn->retain();
				}
			}
			//------------------------------------------------------------------------------
#define mutable_iterate(it, container) \
	ConnectionsVector::iterator it = (container).begin(); it != (container).end(); ++it

#define const_iterate(it, container) \
	ConnectionsVector::const_iterator it = (container).begin(); it != (container).end(); ++it
			//------------------------------------------------------------------------------

			class ConnectionList::NullComparer
			{
			public:

				bool operator() (AbstractConnection const * conn) const
				{
					(void) conn;
					return true;
				}
			};
			//------------------------------------------------------------------------------

			class ConnectionList::DelegateComparer
			{
			public:

				DelegateComparer (AbstractDelegate const & deleg)
						: deleg_(deleg)
				{
				}

				bool operator() (AbstractConnection const * conn) const
				{
					return conn->recieverDelegate() == deleg_;
				}
			private:
				AbstractDelegate const & deleg_;
			};
			//------------------------------------------------------------------------------

			class ConnectionList::PeerComparer
			{
			public:

				PeerComparer (ConnectionList * peer)
						: peer_(peer)
				{
				}

				bool operator() (AbstractConnection const * conn) const
				{
					return conn->hasPeer(peer_);
				}
			private:
				ConnectionList * const peer_;
			};
			//------------------------------------------------------------------------------

			class ConnectionList::FullComparer
			{
			public:

				FullComparer (ConnectionList * peer,
				        AbstractDelegate const & deleg)
						: e_(peer), d_(deleg)
				{
				}

				bool operator() (AbstractConnection const * conn) const
				{
					return e_(conn) && d_(conn);
				}
			private:
				PeerComparer e_;
				DelegateComparer d_;
			};
			//------------------------------------------------------------------------------

			size_t ConnectionList::connectionCount () const
			{
				ThreadDataLocker lock(lock_);
				return data_.constRef().size();
			}
			//------------------------------------------------------------------------------

			template<class Comparer> inline size_t ConnectionList::getConnectionCount (
			        Comparer const & comp) const
			{
				ThreadDataLocker lock(lock_);
				ConnectionsVector const & connections = data_.constRef();
				size_t retVal = 0;
				for (const_iterate(it, connections)) {
					AbstractConnection const * conn = *it;
					if (comp(conn))
						++retVal;
				}
				return retVal;
			}
			//------------------------------------------------------------------------------

			size_t ConnectionList::connectionCount (
			        AbstractDelegate const & deleg) const
			{
				DelegateComparer comp(deleg);
				return getConnectionCount(comp);
			}
			//------------------------------------------------------------------------------

			size_t ConnectionList::connectionCount (ConnectionList * peer) const
			{
				PeerComparer comp(peer);
				return getConnectionCount(comp);
			}
			//------------------------------------------------------------------------------

			size_t ConnectionList::connectionCount (ConnectionList * peer,
			        AbstractDelegate const & deleg) const
			{
				FullComparer comp(peer, deleg);
				return getConnectionCount(comp);
			}
			//------------------------------------------------------------------------------

			bool ConnectionList::hasConnections () const
			{
				ThreadDataLocker lock(lock_);
				return !data_.constRef().empty();
			}
			//------------------------------------------------------------------------------

			template<class Comparer> inline bool ConnectionList::getHasConnections (
			        Comparer const & comp) const
			{
				ThreadDataLocker lock(lock_);
				ConnectionsVector const & connections = data_.constRef();
				for (const_iterate(it, connections)) {
					AbstractConnection const * conn = *it;
					if (comp(conn))
						return true;
				}
				return false;
			}
			//------------------------------------------------------------------------------

			bool ConnectionList::hasConnections (
			        AbstractDelegate const & deleg) const
			{
				DelegateComparer comp(deleg);
				return getHasConnections(comp);
			}
			//------------------------------------------------------------------------------

			bool ConnectionList::hasConnections (ConnectionList * peer) const
			{
				PeerComparer comp(peer);
				return getHasConnections(comp);
			}
			//------------------------------------------------------------------------------

			bool ConnectionList::hasConnections (ConnectionList * peer,
			        AbstractDelegate const & deleg) const
			{
				FullComparer comp(peer, deleg);
				return getHasConnections(comp);
			}
			//------------------------------------------------------------------------------

			template<class Comparer> inline size_t ConnectionList::doDisconnectAll (
			        Comparer const & comp)
			{
				ConnectionsVector needRelock;
				size_t retVal = 0;
				{
					ThreadDataLocker lock(lock_);
					ConnectionsVector & connections = data_.mutableRef();
					for (size_t i = 0; i < connections.size();) {
						AbstractConnection * conn = connections.at(i);
						if (!comp(conn)) {
							++i;
							continue;
						}
						++retVal;
						bool done = conn->tryDisconnectWithLock(lock_);
						if (!done) {
							conn->retain();
							needRelock.push_back(conn);
							++i;
						}
					}
				}
				for (const_iterate(it, needRelock)) {
					AbstractConnection * conn = *it;
					conn->disconnect();
					conn->release();
				}
				return retVal;
			}
			//------------------------------------------------------------------------------

			size_t ConnectionList::disconnectAll ()
			{
				NullComparer comp;
				return doDisconnectAll(comp);
			}
			//------------------------------------------------------------------------------

			size_t ConnectionList::disconnectAll (
			        AbstractDelegate const & deleg)
			{
				DelegateComparer comp(deleg);
				return doDisconnectAll(comp);
			}
			//------------------------------------------------------------------------------

			size_t ConnectionList::disconnectAll (ConnectionList * peer,
			        AbstractDelegate const & deleg)
			{
				FullComparer comp(peer, deleg);
				return doDisconnectAll(comp);
			}
			//------------------------------------------------------------------------------

			size_t ConnectionList::disconnectAll (ConnectionList * peer)
			{
				PeerComparer comp(peer);
				return doDisconnectAll(comp);
			}
			//------------------------------------------------------------------------------

			template<class Comparer> inline bool ConnectionList::doDisconnectOne (
			        Comparer const & comp)
			{
				AbstractConnection * needRelock = 0;
				{
					ThreadDataLocker lock(lock_);
					ConnectionsVector & connections = data_.mutableRef();
					for (size_t i = 0; i < connections.size();) {
						AbstractConnection * conn = connections.at(i);
						if (!comp(conn)) {
							++i;
							continue;
						}
						bool done = conn->tryDisconnectWithLock(lock_);
						if (done)
							return true;
						needRelock = conn;
						conn->retain();
						break;
					}
				}

				if (!needRelock)
					return false;

				needRelock->disconnect();
				needRelock->release();
				return true;
			}
			//------------------------------------------------------------------------------

			bool ConnectionList::disconnectOne (AbstractDelegate const & deleg)
			{
				DelegateComparer comp(deleg);
				return doDisconnectOne(comp);
			}
			//------------------------------------------------------------------------------

			bool ConnectionList::disconnectOne (ConnectionList * peer)
			{
				PeerComparer comp(peer);
				return doDisconnectOne(comp);
			}
			//------------------------------------------------------------------------------

			bool ConnectionList::disconnectOne (ConnectionList * peer,
			        AbstractDelegate const & deleg)
			{
				FullComparer comp(peer, deleg);
				return doDisconnectOne(comp);
			}
		//------------------------------------------------------------------------------
		}//namespace Events
	} //namespace Private
} //namespace Cpp
