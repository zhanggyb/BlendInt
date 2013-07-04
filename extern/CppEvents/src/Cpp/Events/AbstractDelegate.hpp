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

#ifndef __CPP__EVENTS__ABSTRACT_DELEGATE__HPP
#define __CPP__EVENTS__ABSTRACT_DELEGATE__HPP

#include <FastDelegate/FastDelegate.h>

namespace Cpp {
    //------------------------------------------------------------------------------
    //! Value type that represents arbitrary delegate in a common form.

    class AbstractDelegate
    {
    public:

	AbstractDelegate () : d_ ()
	{
	}

	AbstractDelegate (fastdelegate::DelegateMemento const & d) : d_ (d)
	{
	}

	template<class D> AbstractDelegate (D const & deleg)
	: d_ (deleg.GetMemento ())
	{
	}

	template<class T, class Y> AbstractDelegate (T obj, Y pmf)
	: d_ (fastdelegate::MakeDelegate (obj, pmf).GetMemento ())
	{
	}

	bool isNull () const
	{
	    return d_.empty();
	}

	void clear ()
	{
	    d_.clear();
	}

	bool operator< (AbstractDelegate const & r) const
	{
	    return d_.IsLess(r.d_);
	}

	bool operator> (AbstractDelegate const & r) const
	{
	    return r.d_.IsLess(d_);
	}

	bool operator== (AbstractDelegate const & r) const
	{
	    return d_.IsEqual(r.d_);
	}

	bool operator!= (AbstractDelegate const & r) const
	{
	    return !d_.IsEqual(r.d_);
	}

	bool operator>= (AbstractDelegate const & r) const
	{
	    return !d_.IsLess(r.d_);
	}

	bool operator<= (AbstractDelegate const & r) const
	{
	    return !r.d_.IsLess(d_);
	}

	template<class D> D castToDelegate () const
	{
	    D retVal;
	    retVal.SetMemento(d_);
	    return retVal;
	}
    private:
	fastdelegate::DelegateMemento d_;
    };
    //------------------------------------------------------------------------------
} //namespace Cpp

#endif //__CPP__EVENTS__ABSTRACT_DELEGATE__HPP