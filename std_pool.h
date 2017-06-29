// Copyright (c) 2017 Trevor Sundberg
// This code is licensed under the MIT license (see LICENSE.txt for details)

#pragma once

#include <cassert>
#include <utility>
#include <unordered_map>
#include <mutex>

#ifndef __stl_assert
#define __stl_assert(condition, message) assert(condition && message)
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4521)
#endif

namespace std
{
  // A pooled object is allocated and shared with all other objects that
  // are equal and hash to the same value. Since a pooled object is shared
  // it is considered immutable (hence we only return a const interface).
  // Note that the default constructor for a pooled object will always
  // construct a default T. A pooled object is never null and will always
  // at least point at a default constructed instance of T (e.g. when moved).
  template <
    typename T,
    typename Hash = hash<T>,
    typename KeyEqual = equal_to<T>,
    typename Allocator = allocator<pair<T, int>>>
  class pooled
  {
  public:
    friend struct hash<pooled<T, Hash, KeyEqual, Allocator>>;

    pooled(pooled& rhs) :
      pooled(const_cast<const pooled&>(rhs))
    {
    }

    pooled(const pooled& rhs)
    {
      m_pair = rhs.m_pair;

      shared_pool& pool = get_pool();
      lock_guard<mutex> guard(pool.m_mutex);

      ++m_pair->second;
    }

    pooled(pooled&& rhs)
    {
      // PERFORMANCE (trevor): We could cache a direct pointer to a default
      // constructed instance on the pool to make this even faster.

      m_pair = rhs.m_pair;
      
      rhs.m_pair = nullptr;
      rhs = pooled();
    }

    pooled& operator=(const pooled& rhs)
    {
      // Yolo!
      this->~pooled();
      new (this) pooled(rhs);
      return *this;
    }

    template <typename... Args>
    pooled(Args&&... args)
    {
      T value(std::forward<Args>(args)...);
      shared_pool& pool = get_pool();
      lock_guard<mutex> guard(pool.m_mutex);
      
      auto it = pool.m_map.find(value);
      if (it != pool.m_map.end())
      {
        // We're adding another reference to this pooled argument
        ++it->second;
      }
      else
      {
        // Move the value into the pool and start our reference count at 1
        it = pool.m_map.insert(it, make_pair(move(value), 1));
      }
      
      // Our class is very optimal because it stores a direct pointer
      // and uses that pointer for all comparisons, hashing, etc!
      m_pair = &*it;
    }

    ~pooled()
    {
      // This only happens during the move constructor when
      // operator= explicitly calls the destructor ~pooled()
      if (!m_pair)
      {
        return;
      }

      shared_pool& pool = get_pool();
      lock_guard<mutex> guard(pool.m_mutex);

      auto it = pool.m_map.find(m_pair->first);
      __stl_assert(it != pool.m_map.end(),
        "The pooled object was not within the pool's map");
      
      // Since we're being destructed, we can release a reference
      --it->second;
      if (it->second == 0)
      {
        // Remove the object from the pool entirely
        pool.m_map.erase(it);
      }
    }

    const T& operator*() const
    {
      return m_pair->first;
    }

    const T* operator->() const
    {
      return &m_pair->first;
    }

    bool operator==(const pooled& rhs) const
    {
      return m_pair == rhs.m_pair;
    }

    bool operator!=(const pooled& rhs) const
    {
      return m_pair != rhs.m_pair;
    }

    bool operator<(const pooled& rhs) const
    {
      return m_pair < rhs.m_pair;
    }

    bool operator<=(const pooled& rhs) const
    {
      return m_pair <= rhs.m_pair;
    }

    bool operator>(const pooled& rhs) const
    {
      return m_pair > rhs.m_pair;
    }

    bool operator>=(const pooled& rhs) const
    {
      return m_pair >= rhs.m_pair;
    }

  private:
    class shared_pool
    {
    public:
      unordered_map<T, int, Hash, KeyEqual, Allocator> m_map;
      mutex m_mutex;
    };
    
    static shared_pool& get_pool()
    {
      static shared_pool instance;
      return instance;
    }

    pair<const T, int>* m_pair;
  };

  template <
    typename T,
    typename Hash,
    typename KeyEqual,
    typename Allocator>
  struct hash<pooled<T, Hash, KeyEqual, Allocator>>
  {
    typedef pooled<T, Hash, KeyEqual, Allocator> argument_type;
    typedef size_t result_type;
    result_type operator()(const argument_type& value) const
    {
      // Hash the pointer since we know all pooled objects are shared!
      static const size_t shift = (size_t)log2(1 + sizeof(argument_type));
      return (size_t)(value.m_pair) >> shift;
    }
  };
}
