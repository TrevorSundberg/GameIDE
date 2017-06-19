// Copyright (c) 2017 Trevor Sundberg
// This code is licensed under the MIT license (see LICENSE.txt for details)

#pragma once

namespace Skugo
{
  /***********************************************************************************************/
  template <typename T, typename... Args>
  T* SafeObjectSingleton::NewReferenceCountedSafeObject(Args&&... args)
  {
    mIsNextObjectReferenceCounted = true;

    // Storing it as a SafeObject* just ensures that the object being allocated is indeed a SafeObject
    SafeObject* safeObject = new T(std::forward<Args>(args)...);
    return safeObject;
  }
}
