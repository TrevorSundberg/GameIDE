// Copyright (c) 2017 Trevor Sundberg
// This code is licensed under the MIT license (see LICENSE.txt for details)

#pragma once

#include "Asserts.h"

namespace Skugo
{
  /***********************************************************************************************/
  template <typename SelfType, typename BaseType>
  SelfType* Singleton<SelfType, BaseType>::mInstance = nullptr;

  /***********************************************************************************************/
  template <typename SelfType, typename BaseType>
  template <typename... Args>
  void Singleton<SelfType, BaseType>::Initialize(Args&&... args)
  {
    SkugoReturnVoidIf(
      mInstance != nullptr,
      "Attempting to initialize the Singleton twice");

    mInstance = new SelfType(std::forward<Args>(args)...);
  }

  /***********************************************************************************************/
  template <typename SelfType, typename BaseType>
  void Singleton<SelfType, BaseType>::Uninitialize()
  {
    SkugoErrorIf(
      mInstance == nullptr,
      "Attempting to uninitializing a Singleton that was not initialized");

    delete mInstance;
    mInstance = nullptr;
  }

  /***********************************************************************************************/
  template <typename SelfType, typename BaseType>
  SelfType& Singleton<SelfType, BaseType>::Instance()
  {
    SkugoErrorIf(
      mInstance == nullptr,
      "The Singleton should be initialized before grabbing an instance");

    return *mInstance;
  }
}
