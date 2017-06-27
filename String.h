// Copyright (c) 2017 Trevor Sundberg
// This code is licensed under the MIT license (see LICENSE.txt for details)

#pragma once

#include "Singleton.h"

namespace Skugo
{
  // Our string class is automatically pooled with other strings globally, ensuring that comparisons
  // are as simple as comparing a pointer. Due to this behaivor we're able to use the pointer as the hash,
  // which also makes string lookups incredibly fast in unordered_maps.
  class String : public string
  {
  public:
  };
}
