// Copyright (c) 2017 Trevor Sundberg
// This code is licensed under the MIT license (see LICENSE.txt for details)

#pragma once

#include "Singleton.h"

namespace Skugo
{
  // All objects in Skugo are reference counted with safe handles.
  // Objects may also be explicitly deleted.

  // This class manages which objects are alive via a map (and assigns ids to new objects)
  class SafeObjectSingleton : public Singleton<SafeObjectSingleton>
  {
  public:
    friend class SafeObject;
    friend class Handle;

    SafeObjectSingleton();

    template <typename T, typename... Args>
    T* NewReferenceCountedSafeObject(Args&&... args);

  private:
    // Counts up for every object (generally never wraps around because it is 64bit).
    // Note that 0 is reserved for null, so the count starts at 1.
    uint64_t mIdCounter;
    unordered_map<uint64_t, SafeObject*> mIdToSafeObject;

    // We only want to perform reference counting and automatic deletion on objects that were
    // allocated via the heap (or more importantly, objects whose lifetimes aren't controlled)
    // For example, a stack object's lifetime is controlled by the compiler and scope, and an
    // object inside an array is controlled by the lifetime of the array (or clear/remove calls).
    // We only want objects that are individually allocated, therefore it must be via SkugoNew.
    // We could try to abuse operator new and assume placement new is used in all cases where memory
    // is controlled, however there is no guarantee std::vector uses placement new vs normal new.
    // The constructor of SafeObject utilizes this bool.
    bool mIsNextObjectReferenceCounted;
  };

  // Allocates a SafeObject that is also reference counted
  #define SkugoNew(T, ...) (::Skugo::SafeObjectSingleton::Instance().NewReferenceCountedSafeObject<T>(__VA_ARGS__))

  // All of our classes should inherit from safe object in order to be refernece counted and looked up via handles
  class SafeObject
  {
  public:
    friend class SafeObjectSingleton;
    friend class Handle;

    SafeObject();
    virtual ~SafeObject();

  private:
    uint64_t mReferenceCount;
    uint64_t mId;

    // A constant we use as the mReferenceCount to signify we aren't doing reference counting for this object
    static const uint64_t cNoReferenceCounting = static_cast<uint64_t >(-1);
  };

  // A handle generically points at any SafeObject
  // The handle will keep the object alive via a reference count, but the object may be explicitly deleted
  class Handle
  {
  public:
    Handle();
    Handle(SafeObject* safeObject);
    ~Handle();

    // Returns a valid SafeObject unless the object has been deleted (then it returns null)
    SafeObject* Dereference();

  private:
    // The id of the object we're pointing at (0 means null)
    uint64_t mId;
  };

  template <typename T>
  class HandleOf : public Handle
  {
  public:
    HandleOf();
    HandleOf(T* instance);

    // Returns a valid T unless the object has been deleted (then it returns null)
    T* Dereference();
  };
}

#include "SafeObject.inl"
