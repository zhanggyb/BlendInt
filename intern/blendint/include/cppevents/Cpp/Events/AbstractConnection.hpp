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

#ifndef __CPP__EVENTS__ABSTRACT_CONNECTION__HPP
#define __CPP__EVENTS__ABSTRACT_CONNECTION__HPP

#include <Cpp/Threading/AtomicInt.hpp>
#include <Cpp/Events/AbstractDelegate.hpp>
#include <Cpp/Events/ThreadData.hpp>
#include <Cpp/Utils/DisableCopy.hpp>
#include <Std/Assert.hpp>

namespace Cpp {
	namespace Private {
		namespace Events {
			//------------------------------------------------------------------------------
			class ConnectionList;
			//------------------------------------------------------------------------------
			//! Root class for all template connection classes.

			class AbstractConnection
			{
				friend class ConnectionList;

			CPP_DISABLE_COPY (AbstractConnection)
			public:
				// Typeless delegate of the receiver object that can be used for comparison, but cannot be invoked.
				AbstractDelegate recieverDelegate () const
				{
					return targetDelegate_;
				}
				// Checks if specified connection list is one of the connection peers

				bool hasPeer (ConnectionList * peer) const
				{
					return (peer == sourceList_) || (peer == targetList_);
				}

				// Performs disconnection with complete locking and calls release() twice.
				void disconnect ();
			protected:

				AbstractConnection (AbstractDelegate const & targetDelegate)
						: refCounter_(), outerLock_(ThreadDataRef::null()), innerLock_(
						        ThreadDataRef::null()), targetDelegate_(
						        targetDelegate), sourceList_(), sourceIndex_(
						        npos), targetList_(), targetIndex_(npos)
				{
				}

				virtual ~AbstractConnection ()
				{
					assert(!sourceList_ && !targetList_);
					assert(sourceIndex_ == npos && targetIndex_ == npos);
				}
			private:
				//Special value for null index
				static size_t const npos = size_t(0) - size_t(1);

				//Reference counter protects connection object from being removed untimely.
				AtomicInt refCounter_;
				//Each logical connection is protected by two mutexes in different connection lists.
				//They should be locked in specific order, in order to avoid deadlocks.
				//Thus, we need to access both mutexes before locking any of them.
				//But we cannot dereference neither sourceList_ nor targetList_ pointers until both mutexes are locked.
				//So we need to store reference to these mutexes explicitly.
				ThreadDataRef outerLock_;
				ThreadDataRef innerLock_;

				//Typeless target delegate. Used as a key for when searching for connections
				AbstractDelegate targetDelegate_;

				//Pointer for ConnectionList object inside the event
				ConnectionList * sourceList_;
				//Index in sourceList_->connections_
				size_t sourceIndex_;
				//Pointer for ConnectionList object inside the scope
				ConnectionList * targetList_;
				//Index in targetList_->connections_
				size_t targetIndex_;

				void retain ()
				{
					refCounter_.retain();
				}

				void release ()
				{
					if (refCounter_.release())
						delete this;
				}

				// If \p lock is connection's outer lock, disconnects connection and returns true.
				// If \p lock is not connection's outer lock, returns false and does nothing.
				bool tryDisconnectWithLock (ThreadDataRef const & lock);

				// Performs actual disconnecting. Both locks must be locked by the caller.
				void doDisconnect ();
			};
		//------------------------------------------------------------------------------
		}//namespace Events
	} //namespace Private
} //namespace Cpp

#endif //__CPP__EVENTS__ABSTRACT_CONNECTION__HPP
