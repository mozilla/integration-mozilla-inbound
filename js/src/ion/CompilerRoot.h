/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * vim: set ts=4 sw=4 et tw=99:
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if !defined(jsion_ion_gc_h__) && defined(JS_ION)
#define jsion_ion_gc_h__

#include "jscntxt.h"
#include "gc/Root.h"

namespace js {
namespace ion {

// Roots a read-only GCThing for the lifetime of a single compilation.
// Each root is maintained in a linked list that is walked over during tracing.
// The CompilerRoot must be heap-allocated and may not go out of scope.
template <typename T>
class CompilerRoot : public CompilerRootNode
{
  public:
    CompilerRoot()
      : CompilerRootNode(NULL)
    { }

    CompilerRoot(T ptr)
      : CompilerRootNode(NULL)
    {
        if (ptr)
            setRoot(ptr);
    }

  public:
    // Sets the pointer and inserts into root list. The pointer becomes read-only.
    void setRoot(T root) {
        JS::CompilerRootNode *&rootList = GetIonContext()->temp->rootList();

        JS_ASSERT(!ptr);
        ptr = root;
        next = rootList;
        rootList = this;
    }

  public:
    operator T () const { return static_cast<T>(ptr); }
    T operator ->() const { return static_cast<T>(ptr); }
};

typedef CompilerRoot<JSObject*>   CompilerRootObject;
typedef CompilerRoot<JSFunction*> CompilerRootFunction;
typedef CompilerRoot<PropertyName*> CompilerRootPropertyName;
typedef CompilerRoot<Value> CompilerRootValue;

} // namespace ion
} // namespace js

#endif // jsion_ion_gc_h__

