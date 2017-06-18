// Copyright (c) 2017 by Trevor Sundberg
// This code is licensed under the MIT license (see LICENSE.txt for details)

#pragma once

namespace Skugo
{
  // All singletons follow the same pattern and require explicit initialization and shutdown
  // This is because often order issues are complex and reference counting may keep things
  // alive, requiring an explicit shutdown anyways to remove the reference counts
  template <typename T>
  class Singleton
  {
  public:
    template <typename... Args>
    static void Initialize(Args&&... args);
    static void Uninitialize();
    static T* Instance();

  private:
    static T* mInstance;
  };
}

#include "Singleton.inl"
