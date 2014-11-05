// Copyright (c) 2010 Nickolas Pohilets
//
// This file is a compile-time entry point into the library.
// Include it to use all the stuff provided by the library.
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

#ifndef __CPP__EVENTS__HPP
#define __CPP__EVENTS__HPP

#include <Cpp/Events/ConnectionScope.hpp>

namespace Cpp {
	//------------------------------------------------------------------------------

	class Events
	{
	public:
		//! Initializes internal data for entire process and the main thread.

		/**
		 Should be called once before constructing any instance of the library classes in any thread.
		 Should be called from the main thread before starting any other thread.
		 \see destructProcessData(), ProcessInit, constructThreadData()
		 */
		static void constructProcessData ()
		{
			Threading::constructProcessData();
		}

		//! Deinitializes internal data for entire process and the main thread.

		/**
		 Should be called once after destructing all instances of the library classes in all threads.
		 Should be called from the main thread after finishing all other threads.
		 \see constructProcessData(), ProcessInit, destructThreadData()
		 */
		static void destructProcessData ()
		{
			Threading::destructProcessData();
		}

		//! Initializes internal data for the current thread.

		/**
		 Should be called once for each created thread that constructs any instance of the library classes.
		 You may omit per-thread initialization for threads that use existing library objects, but does not create new one.
		 Do not call this function for the main thread.
		 \see destructThreadData(), ThreadInit, constructProcessData()
		 */
		static void constructThreadData ()
		{
			Threading::constructThreadData();
		}

		//! Deinitializes internal data for the current thread.

		/**
		 Should be called once for each thread that called constructThreadData().
		 No more instances of the library classes can be constructed in the current thread after calling this function,
		 but instances constructed earlier remain valid and can be used in different threads.
		 Do not call this function for the main thread.
		 \see constructThreadData(), ThreadInit, destructProcessData()
		 */
		static void destructThreadData ()
		{
			Threading::destructThreadData();
		}

		//! Convenience RAII wrapper for constructProcessData()/destructProcessData().

		/**
		 Create single instance of this class on the stack of the main() function.
		 This class simply calls constructProcessData() in the constructor and destructProcessData() in the destructor.
		 This class is non-copyable.
		 \see constructProcessData(), destructProcessData()
		 */
		class ProcessInit
		{
		public:

			ProcessInit ()
			{
				constructProcessData();
			}

			~ProcessInit ()
			{
				destructProcessData();
			}
		};

		//! Convenience exception-safe per-thread library initializer.

		/**
		 Create single instance of this class on the stack of the created thread.
		 This class simply calls constructThreadData() in the constructor and destructThreadData() in the destructor.
		 This class is non-copyable.
		 \see constructThreadData(), destructThreadData()
		 */
		class ThreadInit
		{
		public:

			ThreadInit ()
			{
				constructThreadData();
			}

			~ThreadInit ()
			{
				destructThreadData();
			}
		};
	};
//------------------------------------------------------------------------------
}//namespace Cpp

#endif //__CPP__EVENTS__HPP
