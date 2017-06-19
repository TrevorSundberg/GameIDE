// Copyright (c) 2017 Trevor Sundberg
// This code is licensed under the MIT license (see LICENSE.txt for details)

#pragma once

namespace Skugo
{
  // Class forward declarations (sorted)
  class EmptyBase;
  class Handle;
  class SafeObject;
  class SafeObjectSingleton;

  // Templated forward declarations (sorted)
  template <typename T>
  class HandleOf;
  template <typename SelfType, typename BaseType = EmptyBase>
  class Singleton;
}
