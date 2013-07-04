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

#ifndef __CPP__EVENTS__BORROWABLE_DATA__HPP
#define __CPP__EVENTS__BORROWABLE_DATA__HPP

#include <Cpp/Events/AbstractConnection.hpp>
#include <Std/Assert.hpp>
#include <Std/Vector.hpp>

namespace Cpp {
    namespace Private {
	namespace Events {
	    //------------------------------------------------------------------------------
	    typedef std::vector<AbstractConnection*> ConnectionsVector;
	    //------------------------------------------------------------------------------

	    class BorrowableData
	    {
		CPP_DISABLE_COPY (BorrowableData)
	    public:
		class Borrower;

		BorrowableData ()
		: data_ ()
		, borrowed_ ()
		{
		}

		~BorrowableData ()
		{
		    assert(!borrowed_);
		}

		bool isBorrowed () const
		{
		    return borrowed_ != 0;
		}

		ConnectionsVector const & constRef () const
		{
		    if (borrowed_) {
			assert(data_.empty());
			return *borrowed_;
		    }
		    return data_;
		}

		ConnectionsVector & mutableRef ()
		{
		    detach();
		    return data_;
		}

		void detach ()
		{
		    if (borrowed_) {
			assert(data_.empty());
			data_ = *borrowed_;
			borrowed_ = 0;
		    }
		}
	    private:
		ConnectionsVector data_;
		ConnectionsVector * borrowed_;
	    };
	    //------------------------------------------------------------------------------

	    class BorrowableData::Borrower
	    {

		CPP_DISABLE_COPY (Borrower)
	    public:
		Borrower (BorrowableData const * src)
		: src_ (const_cast<BorrowableData*> (src))
		, data_ ()
		{
		    assert(!src_->borrowed_ && "Data can be borrowed only once");
		    data_.swap(src_->data_);
		    src_->borrowed_ = &data_;
		}

		~Borrower ()
		{
		    if (src_->borrowed_ == &data_) {
			assert(src_->data_.empty());
			data_.swap(src_->data_);
			src_->borrowed_ = 0;
		    }
		}

		ConnectionsVector const & constData () const
		{
		    return data_;
		}
	    private:
		BorrowableData * src_;
		ConnectionsVector data_;
	    };
	    //------------------------------------------------------------------------------
	} //namespace Events
    } //namespace Private
} //namespace Cpp

#endif //__CPP__EVENTS__BORROWABLE_DATA__HPP

