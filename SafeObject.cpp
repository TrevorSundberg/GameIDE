// Copyright (c) 2017 by Trevor Sundberg
// This code is licensed under the MIT license (see LICENSE.txt for details)

#include "Precompiled.h"
#include "SafeObject.h"
#include "Logging.h"

namespace Skugo
{
  /***********************************************************************************************/
  SafeObjectSingleton::SafeObjectSingleton() :
    mIdCounter(1),
    mIsNextObjectReferenceCounted(false)
  {
  }

  /***********************************************************************************************/
  SafeObject::SafeObject()
  {
    SafeObjectSingleton* singleton = SafeObjectSingleton::Instance();
    mId = singleton->mIdCounter;
    ++singleton->mIdCounter;
    singleton->mIdToSafeObject[mId] = this;

    if (singleton->mIsNextObjectReferenceCounted)
    {
      mReferenceCount = 0;
      singleton->mIsNextObjectReferenceCounted = false;
    }
    else
    {
      mReferenceCount = cNoReferenceCounting;
    }
  }

  /***********************************************************************************************/
  SafeObject::~SafeObject()
  {
    SafeObjectSingleton* singleton = SafeObjectSingleton::Instance();
    size_t itemsRemoved = singleton->mIdToSafeObject.erase(mId);
    SkugoErrorIf(itemsRemoved == 0, "The SafeObject did not exist within the SafeObjectSingleton");
  }

  /***********************************************************************************************/
  Handle::Handle() :
    mId(0)
  {
  }

  /***********************************************************************************************/
  Handle::Handle(SafeObject* safeObject)
  {
    if (safeObject)
    {
      mId = safeObject->mId;
      
      // A reference count of 0 or positive means we are using reference counting
      // A negative reference count means we are not reference counting (not allocated via SkugoNew)
      if (safeObject->mReferenceCount != SafeObject::cNoReferenceCounting)
      {
        ++safeObject->mReferenceCount;
      }
    }
    else
    {
      mId = 0;
    }
  }

  /***********************************************************************************************/
  Handle::~Handle()
  {
    SafeObject* safeObject = Dereference();

    if (safeObject)
    {
      if (safeObject->mReferenceCount != SafeObject::cNoReferenceCounting)
      {
        SkugoErrorIf(safeObject->mReferenceCount == 0,
          "The reference count was already zero but we tried to decrement it");
        --safeObject->mReferenceCount;

        if (safeObject->mReferenceCount == 0)
        {
          delete safeObject;
        }
      }
    }
  }

  /***********************************************************************************************/
  SafeObject* Handle::Dereference()
  {
    SafeObjectSingleton * singleton = SafeObjectSingleton::Instance();
    auto it = singleton->mIdToSafeObject.find(mId);
    if (it != singleton->mIdToSafeObject.end())
    {
      return it->second;
    }

    return nullptr;
  }
}
