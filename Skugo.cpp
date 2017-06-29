// Copyright (c) 2017 Trevor Sundberg
// This code is licensed under the MIT license (see LICENSE.txt for details)

#include "Precompiled.h"
//#include "SafeObject.h"
//#include "UnitTests.h"
#include "intrusive_list.h"
#include <stdio.h>

using namespace Skugo;

class Animal : public intrusive_link
{
public:
  Animal(const char* name) :
    mName(name)
  {
  }

  const char* mName;
};

int main(void)
{
  //SafeObjectSingleton::Initialize();
  //
  //SafeObject a;
  //
  //Handle aHandle(&a);
  //
  //SafeObject* aDeref = aHandle.Dereference();
  //
  //SafeObject* b = SkugoNew(SafeObject);
  //Handle bHandle(b);
  //
  //SafeObject* bDeref1 = bHandle.Dereference();
  //
  //delete b;
  //
  //
  //SafeObject* bDeref2 = bHandle.Dereference();

  intrusive_list<Animal> farm;

  Animal bob("bob");
  Animal ted("ted");
  Animal ned("ned");
  farm.push_back(bob);
  farm.push_back(ted);
  farm.push_back(ned);
  farm.push_front(ned);

  farm.erase(bob, ted);

  size_t s1 = farm.size();

  intrusive_list<Animal> stateFarm;
  stateFarm.insert_before(stateFarm.end(), farm.begin(), farm.end());

  size_t s2 = farm.size();
  size_t s3 = stateFarm.size();

  printf("------------------\n");
  for (const Animal& animal : stateFarm)
  {
    printf("%s\n", animal.mName);
  }
  printf("------------------\n");
  for (const Animal& animal : const_cast<const intrusive_list<Animal>&>(stateFarm))
  {
    printf("%s\n", animal.mName);
  }
  printf("------------------\n");

  //SafeObjectSingleton::Uninitialize();
}
