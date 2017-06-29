// Copyright (c) 2017 Trevor Sundberg
// This code is licensed under the MIT license (see LICENSE.txt for details)

#pragma once

#include <string>
#include "std_pool.h"

namespace std
{
  // Extend the string interface with some immutable const functions
  // This is intended to work directly with pstring.
  class istring : public string
  {
  public:
    using string::string;

    istring()
    {
    }

    istring(string& rhs) :
      string(rhs)
    {
    }

    istring(const string& rhs) :
      string(rhs)
    {
    }

    istring(istring& rhs) :
      string(rhs)
    {
    }

    istring(const istring& rhs) :
      string(rhs)
    {
    }

    istring(istring&& rhs) :
      string(rhs)
    {
    }

    pooled<istring> substr(size_t pos = 0, size_t len = npos) const
    {
      return pooled<istring>(move(string::substr(pos, len)));
    }
  };

  template <>
  struct hash<istring>
  {
    typedef istring argument_type;
    typedef size_t result_type;
    result_type operator()(const argument_type& value) const
    {
      // Hash the pointer since we know all pooled objects are shared!
      return hash<string>()(value);
    }
  };

  // Our string class is automatically pooled with other strings globally, ensuring that comparisons
  // are as simple as comparing a pointer. Due to this behaivor we're able to use the pointer as the hash,
  // which also makes string lookups incredibly fast in unordered_maps.
  typedef pooled<istring> pstring;
}
