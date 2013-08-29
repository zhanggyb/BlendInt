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

#ifndef __CPP__THREADING__THREAD_STORAGE__HPP
#define __CPP__THREADING__THREAD_STORAGE__HPP

#include <Cpp/Utils/DisableCopy.hpp>

namespace Cpp {
	//------------------------------------------------------------------------------
	template<class T> class ThreadStorage;
//------------------------------------------------------------------------------
}//namespace Cpp

#include <Cpp/Threading/Config.hpp>
#include CPP_THREADING_PLATFORM_PATH(ThreadStorage.hpp.inl)

namespace Cpp {
	//------------------------------------------------------------------------------

	template<class T> class ThreadStorage
	{
		CPP_DISABLE_COPY (ThreadStorage)
	public:

		ThreadStorage ()
		{
		}

		~ThreadStorage ()
		{
		}

		T data () const
		{
			return reinterpret_cast<T> (p_.data());
		}

		void setData (T x)
		{
			p_.setData(reinterpret_cast<void*> (x));
		}
	private:
		ThreadStorage<void*> p_;
	};
	//------------------------------------------------------------------------------
} //namespace Cpp

#endif //__CPP__THREADING__THREAD_STORAGE__HPP
