// Copyright (c) 2017 Trevor Sundberg
// This code is licensed under the MIT license (see LICENSE.txt for details)

#pragma once

namespace Skugo
{
  // A dummy base class that we use for the singleton's BaseType template
  class EmptyBase
  {
  };

  // All singletons follow the same pattern and require explicit initialization and shutdown
  // This is because often order issues are complex and reference counting may keep things
  // alive, requiring an explicit shutdown anyways to remove the reference counts
  template <typename SelfType, typename BaseType>
  class Singleton : public BaseType
  {
  public:
    template <typename... Args>
    static void Initialize(Args&&... args);
    static void Uninitialize();
    static SelfType& Instance();

  private:
    static SelfType* mInstance;
  };
}

#include "Singleton.inl"
