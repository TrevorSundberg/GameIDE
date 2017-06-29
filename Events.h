// Copyright (c) 2017 Trevor Sundberg
// This code is licensed under the MIT license (see LICENSE.txt for details)

#pragma once

#include "SafeObject.h"
#include "std_pstring.h"

namespace Skugo
{
  class Event
  {
  public:
    pstring mName;
  };

  // An event connection is allocated and exists between the sender and the reciever.
  // When either sender or receiver dies, we attempt to delete the EventConnection.
  // The virtual Invoke allows us to use many types of callbacks (including script).
  class EventConnection
  {
  public:
    virtual void Invoke(Event* event);
    
    // TODO (Trevor): Make this intrusive!
  };

  class EventObject : public SafeObject
  {
  public:
  };

}
