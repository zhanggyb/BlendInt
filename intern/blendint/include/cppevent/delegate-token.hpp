/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Freeman Zhang <zhanggyb@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <cppevent/delegate.hpp>
#include <cppevent/invokable-token.hpp>

namespace CppEvent {

template<typename ... ParamTypes>
class DelegateToken : public InvokableToken < ParamTypes... >
{
 public:

  DelegateToken() = delete;

  inline DelegateToken(const Delegate<void, ParamTypes...>& d);

  virtual ~DelegateToken();

  virtual void Invoke(ParamTypes... Args) override;

  const Delegate<void, ParamTypes...>& delegate () const
  {
    return delegate_;
  }

 private:

  Delegate<void, ParamTypes...> delegate_;

};

template<typename ... ParamTypes>
inline DelegateToken<ParamTypes...>::DelegateToken(const Delegate<void, ParamTypes...>& d)
    : InvokableToken<ParamTypes...>(), delegate_(d)
{
}

template<typename ... ParamTypes>
DelegateToken<ParamTypes...>::~DelegateToken()
{
}

template<typename ... ParamTypes>
void DelegateToken<ParamTypes...>::Invoke(ParamTypes... Args)
{
  delegate_(Args...);
}

} // namespace CppEvent
