// Copyright (c) 2017 Trevor Sundberg
// This code is licensed under the MIT license (see LICENSE.txt for details)

#include "Precompiled.h"
#include "SafeObject.h"
#include "UnitTests.h"

using namespace Skugo;

int main(void)
{
  SafeObjectSingleton::Initialize();

  SafeObject a;

  Handle aHandle(&a);

  SafeObject* aDeref = aHandle.Dereference();

  SafeObject* b = SkugoNew(SafeObject);
  Handle bHandle(b);

  SafeObject* bDeref1 = bHandle.Dereference();

  delete b;


  SafeObject* bDeref2 = bHandle.Dereference();



  SafeObjectSingleton::Uninitialize();
}
