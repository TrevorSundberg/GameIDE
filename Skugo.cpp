// Copyright (c) 2017 Trevor Sundberg
// This code is licensed under the MIT license (see LICENSE.txt for details)

#include "Precompiled.h"
//#include "SafeObject.h"
//#include "UnitTests.h"
#include "std_intrusive_list.h"
#include "std_pool.h"
#include "std_pstring.h"
#include <string>
#include <stdio.h>
#include <time.h>

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

  ////intrusive_list<Animal> farm;
  ////
  ////Animal bob("bob");
  ////Animal ted("ted");
  ////Animal ned("ned");
  ////farm.push_back(bob);
  ////farm.push_back(ted);
  ////farm.push_back(ned);
  ////farm.push_front(ned);
  ////
  ////farm.erase(bob, ted);
  ////
  ////size_t s1 = farm.size();
  ////
  ////intrusive_list<Animal> stateFarm;
  ////stateFarm.insert_before(stateFarm.end(), farm.begin(), farm.end());
  ////
  ////size_t s2 = farm.size();
  ////size_t s3 = stateFarm.size();
  ////
  ////printf("------------------\n");
  ////for (const Animal& animal : stateFarm)
  ////{
  ////  printf("%s\n", animal.mName);
  ////}
  ////printf("------------------\n");
  ////for (const Animal& animal : const_cast<const intrusive_list<Animal>&>(stateFarm))
  ////{
  ////  printf("%s\n", animal.mName);
  ////}
  ////printf("------------------\n");

  //pooled<string> a("hello");
  //pooled<string> b("hello");

  //pstring a("hello");
  //pstring b("hello");

  //auto x = a->substr(0, 2);

  //SafeObjectSingleton::Uninitialize();

  unordered_map<string, string> test1;
  unordered_map<pstring, pstring> test2;

  const char* tedActual = "OnCollisionStarted";
  const char* ted = "OnCollisionStartea";
  test1["bob"] = "employee";
  test1[tedActual] = "homeless";
  test1["ned"] = "jockey";

  test2["bob"] = "employee";
  test2[tedActual] = "homeless";
  test2["ned"] = "jockey";

  for (int i = 0; i < 10000; ++i)
  {
    string str("wtf");
    str += to_string(i);
    test1[str] = "hello";
    test2[str] = "hello";
  }

  size_t iterations = 200000000;

  clock_t start1 = clock();

  string ted1(ted);
  for (size_t i = 0; i < iterations; ++i)
  {
    test1.find(ted1);
  }

  clock_t end1 = clock();

  clock_t start2 = clock();

  pstring ted2(ted);
  for (size_t i = 0; i < iterations; ++i)
  {
    test2.find(ted2);
  }

  clock_t end2 = clock();

  printf("Test1: %f seconds\n", (end1 - start1) / static_cast<double>(CLOCKS_PER_SEC));
  printf("Test2: %f seconds\n", (end2 - start2) / static_cast<double>(CLOCKS_PER_SEC));

  getchar();
}
