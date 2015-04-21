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

#ifdef DEBUG
#include <cassert>
#endif

namespace CppEvent {

// forward declaration
class AbstractTrackable;
struct Token;
struct Binding;

/**
 * @brief The abstract event binding
 */
struct Binding
{
  inline Binding ()
      : trackable_object(0),
        previous(0),
        next(0),
        token(0)
  {
  }

  ~Binding ();

  AbstractTrackable* trackable_object;
  Binding* previous;
  Binding* next;
  Token* token;
};

/**
 * @brief The event source
 */
struct Token
{
  inline Token ()
      : trackable_object(0),
        previous(0),
        next(0),
        binding(0)
  {
  }

  virtual ~Token ();

  AbstractTrackable* trackable_object;
  Token* previous;
  Token* next;
  Binding* binding;

};

/**
 * @brief Abstract class for event
 */
class AbstractTrackable
{
  friend struct Binding;
  friend struct Token;

 public:

  inline AbstractTrackable ()
      : first_binding_(0), last_binding_(0)
  {
  }

  virtual ~AbstractTrackable ();

 protected:

  virtual void AuditDestroyingToken (Token* token) = 0;

  void PushBackBinding (Binding* binding);

  void PushFrontBinding (Binding* binding);

  void InsertBinding (int index, Binding* binding);

  void RemoveAllBindings ();

  static inline void link (Token* token, Binding* binding)
  {
#ifdef DEBUG
    assert((token->binding == 0) && (binding->token == 0));
#endif

    token->binding = binding;
    binding->token = token;
  }
  
  static inline void add_binding (AbstractTrackable* trackable,
                                  Binding* conn)
  {
    trackable->PushBackBinding(conn);
  }

  static inline void insert_binding (AbstractTrackable* trackable,
                                     Binding* conn,
                                     int index = 0)
  {
    trackable->InsertBinding(index, conn);
  }

 private:

  Binding* first_binding_;
  Binding* last_binding_;
};

}  // namespace CppEvent
