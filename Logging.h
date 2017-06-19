// Copyright (c) 2017 Trevor Sundberg
// This code is licensed under the MIT license (see LICENSE.txt for details)

#pragma once

#include "Singleton.h"

namespace Skugo
{
  class LoggingSingleton : public Singleton<LoggingSingleton>
  {
  public:
    // All asserts, console prints, exceptions, logging of any sort will go through this call
    void SignalEvent(const char messageN[], const char* tagsSpaceSeparatedN);
  };
}
