// Copyright (c) 2017 by Trevor Sundberg
// This code is licensed under the MIT license (see LICENSE.txt for details)

namespace Skugo
{
  /***********************************************************************************************/
  template <typename T>
  T* Singleton<T>::mInstance = nullptr;

  /***********************************************************************************************/
  template <typename T>
  template <typename... Args>
  void Singleton<T>::Initialize(Args&&... args)
  {
    SkugoReturnVoidIf(
      mInstance != nullptr,
      "Attempting to initialize the Singleton twice");

    mInstance = new T(args);
  }

  /***********************************************************************************************/
  template <typename T>
  void Singleton<T>::Uninitialize()
  {
    SkugoErrorIf(
      mInstance == nullptr,
      "Attempting to uninitializing a Singleton that was not initialized");

    delete mInstance;
    mInstance = nullptr;
  }

  /***********************************************************************************************/
  template <typename T>
  T* Singleton<T>::Instance()
  {
    SkugoErrorIf(
      mInstance == nullptr,
      "The Singleton should be initialized before grabbing an instance");

    return mInstance;
  }
}
