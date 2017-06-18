// Copyright (c) 2017 by Trevor Sundberg
// This code is licensed under the MIT license (see LICENSE.txt for details)

namespace Skugo
{
  /***********************************************************************************************/
  template <typename T, typename... Args>
  T* SafeObjectSingleton::NewReferenceCountedSafeObject(Args&&... args)
  {
    mIsNextObjectReferenceCounted = true;

    // Storing it as a SafeObject* just ensures that the object being allocated is indeed a SafeObject
    SafeObject* safeObject = new T(args);
    return safeObject;
  }
}
