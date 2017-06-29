// Copyright (c) 2017 Trevor Sundberg
// This code is licensed under the MIT license (see LICENSE.txt for details)

#pragma once

#include <cassert>
#include <initializer_list>

#ifndef __stl_assert
#define __stl_assert(condition, message) assert(condition && message)
#endif

namespace std
{
  // To use an intrusive_list you must place this intrusive link inside your class
  // In order to be in more than one intrusive_list you must create unique types of intrusive_link
  // by inheriting from intrusive_link, and then setting LinkType on the intrusive_list.
  class intrusive_link
  {
  public:
    template <typename T, typename LinkType>
    friend class intrusive_list;
    
    intrusive_link();
    ~intrusive_link();
    
    bool unlink() const;
    bool is_linked() const;
  
  private:
    // We have to mark the links as mutable otherwise you cannot have an intrusive list
    // of a const object (the links are within the const object, and must be modified).
    // We believe this is acceptable 'const' behavior.
    mutable const intrusive_link* mNext;
    mutable const intrusive_link* mPrevious;
  };

  // An intrusive list is much like an std::list except it does not
  // allocate and relies on the element to inherit from intrusive_link.
  template <typename T, typename LinkType = intrusive_link>
  class intrusive_list
  {
  public:
    // Note: Using allocator_type here is a bit deceptive because we do not allocate
    // However, we want to follow the same patterns as other containers.
    typedef allocator<T> allocator_type;
    typedef typename allocator_type::value_type value_type;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::difference_type difference_type;
    typedef typename allocator_type::size_type size_type;

    class iterator
    {
    public:
      friend class intrusive_list;

      typedef typename allocator_type::difference_type difference_type;
      typedef typename allocator_type::value_type value_type;
      typedef typename allocator_type::reference reference;
      typedef typename allocator_type::pointer pointer;
      typedef std::bidirectional_iterator_tag iterator_category; //or another tag

      iterator();
      iterator(const iterator&);
      iterator(const T&);
      ~iterator();

      iterator& operator=(const iterator&);
      bool operator==(const iterator&) const;
      bool operator!=(const iterator&) const;

      iterator& operator++();
      iterator operator++(int);
      iterator& operator--();
      iterator operator--(int);

      reference operator*() const;
      pointer operator->() const;

    private:
      iterator(const intrusive_link* link);

      const intrusive_link* mLink;
    };

    class const_iterator
    {
    public:
      friend class intrusive_list;

      typedef typename allocator_type::difference_type difference_type;
      typedef typename allocator_type::value_type value_type;
      typedef typename allocator_type::const_reference const_reference;
      typedef typename allocator_type::const_pointer const_pointer;
      typedef std::bidirectional_iterator_tag iterator_category; //or another tag

      const_iterator();
      const_iterator(const const_iterator&);
      const_iterator(const iterator&);
      const_iterator(const T&);
      ~const_iterator();

      const_iterator& operator=(const const_iterator&);
      bool operator==(const const_iterator&) const;
      bool operator!=(const const_iterator&) const;

      const_iterator& operator++();
      const_iterator operator++(int);
      const_iterator& operator--();
      const_iterator operator--(int);

      const_reference operator*() const;
      const_pointer operator->() const;

    private:
      const_iterator(const intrusive_link* link);

      const intrusive_link* mLink;
    };

    typedef std::reverse_iterator<iterator> reverse_iterator; //optional
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator; //optional

    intrusive_list();
    intrusive_list(intrusive_list&&);
    ~intrusive_list();

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator crbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;
    const_reverse_iterator crend() const;

    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    const T& push_front(const T&);
    const T& push_back(const T&);
    T& push_front(T&);
    T& push_back(T&);
    const T& pop_front() const;
    const T& pop_back() const;
    T& pop_front();
    T& pop_back();

    template <typename IteratorType>
    iterator insert_before(const_iterator beforeThis, IteratorType begin, IteratorType end);
    iterator insert_before(const_iterator beforeThis, const T& toBeInserted);
    iterator insert_before(const_iterator beforeThis, const_iterator begin, const_iterator end);
    iterator insert_before(const_iterator beforeThis, const_iterator begin, iterator end);
    iterator insert_before(const_iterator beforeThis, iterator begin, const_iterator end);
    iterator insert_before(const_iterator beforeThis, iterator begin, iterator end);
    iterator insert_before(const_iterator beforeThis, std::initializer_list<T> list);

    template <typename IteratorType>
    iterator insert_after(const_iterator afterThis, IteratorType begin, IteratorType end);
    iterator insert_after(const_iterator afterThis, const T& toBeInserted);
    iterator insert_after(const_iterator beforeThis, const_iterator begin, const_iterator end);
    iterator insert_after(const_iterator beforeThis, const_iterator begin, iterator end);
    iterator insert_after(const_iterator beforeThis, iterator begin, const_iterator end);
    iterator insert_after(const_iterator beforeThis, iterator begin, iterator end);
    iterator insert_after(const_iterator afterThis, std::initializer_list<T> list);

    iterator erase(const_iterator);
    iterator erase(const_iterator, const_iterator);
    void clear();
    template <typename IteratorType>
    void assign(IteratorType, IteratorType);
    void assign(std::initializer_list<T>);

    void swap(intrusive_list&);
    size_type size() const;
    size_type max_size() const;
    bool empty() const;

  private:
    iterator insert_after_helper(const intrusive_link* afterThisLink, const T& toBeInserted);
    iterator insert_before_helper(const intrusive_link* beforeThisLink, const T& toBeInserted);
    iterator insert_after_helper(const intrusive_link* afterThisLink, const_iterator begin, const_iterator end);
    iterator insert_before_helper(const intrusive_link* beforeThisLink, const_iterator begin, const_iterator end);
    static T& to_t(const intrusive_link* link);
    static const intrusive_link* to_link(const T& value);

    // Intrusive lists cannot be copied or assigned to because they own the members inside of them
    intrusive_list(const intrusive_list&) = delete;
    intrusive_list& operator=(const intrusive_list&) = delete;

    // Our head and tail are the mNext and mPrevious of the sentinel node
    intrusive_link mSentinel;
  };
}

namespace std
{
  /***********************************************************************************************/
  inline intrusive_link::intrusive_link() :
    mNext(nullptr),
    mPrevious(nullptr)
  {
  }

  /***********************************************************************************************/
  inline intrusive_link::~intrusive_link()
  {
    unlink();
  }

  /***********************************************************************************************/
  inline bool intrusive_link::unlink() const
  {
    if (mNext == nullptr)
    {
      __stl_assert(mPrevious == nullptr, "The mNext was null but the mPrevious was not");
      return false;
    }

    // Link our next and previous nodes together (removing ourselves)
    mNext->mPrevious = mPrevious;
    mPrevious->mNext = mNext;

    // Clear out our links
    mNext = nullptr;
    mPrevious = nullptr;
    return true;
  }

  /***********************************************************************************************/
  inline bool intrusive_link::is_linked() const
  {
    __stl_assert((mNext != nullptr) == (mPrevious != nullptr), "The mNext and mPrevious must both be linked or null");
    return mNext != nullptr;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  intrusive_list<T, LinkType>::iterator::iterator() :
    mLink(nullptr)
  {
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  intrusive_list<T, LinkType>::iterator::iterator(const intrusive_link* link) :
    mLink(link)
  {
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  intrusive_list<T, LinkType>::iterator::iterator(const iterator& rhs) :
    mLink(rhs.mLink)
  {
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  intrusive_list<T, LinkType>::iterator::iterator(const T& rhs) :
    mLink(to_link(rhs))
  {
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  intrusive_list<T, LinkType>::iterator::~iterator()
  {
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator& intrusive_list<T, LinkType>::iterator::operator=(const iterator& rhs)
  {
    mLink = rhs.mLink;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  bool intrusive_list<T, LinkType>::iterator::operator==(const iterator& rhs) const
  {
    return mLink == rhs.mLink;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  bool intrusive_list<T, LinkType>::iterator::operator!=(const iterator& rhs) const
  {
    return mLink != rhs.mLink;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator& intrusive_list<T, LinkType>::iterator::operator++()
  {
    __stl_assert(mLink != nullptr, "Attempting to increment a null iterator");
    mLink = mLink->mNext;
    return *this;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::iterator::operator++(int)
  {
    __stl_assert(mLink != nullptr, "Attempting to increment a null iterator");
    intrusive_link* temp = mLink;
    mLink = mLink->mNext;
    return temp;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator& intrusive_list<T, LinkType>::iterator::operator--()
  {
    __stl_assert(mLink != nullptr, "Attempting to decrement a null iterator");
    mLink = mLink->mPrevious;
    return *this;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::iterator::operator--(int)
  {
    __stl_assert(mLink != nullptr, "Attempting to decrement a null iterator");
    intrusive_link* temp = mLink;
    mLink = mLink->mPrevious;
    return temp;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator::reference intrusive_list<T, LinkType>::iterator::operator*() const
  {
    // We must first cast it into the link type (could be inherited from intrusive_link, for example maybe a Space_link)
    // Then we cast it to the T type since T should also inherit from the link. The double cast is important
    // because our T could multiply inherit from different types of intrusive_link (non-virtually).
    // One cast would be ambiguous.
    __stl_assert(mLink != nullptr, "Attempting to dereference a null iterator");
    return to_t(mLink);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator::pointer intrusive_list<T, LinkType>::iterator::operator->() const
  {
    __stl_assert(mLink != nullptr, "Attempting to dereference a null iterator");
    return &to_t(mLink);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  intrusive_list<T, LinkType>::const_iterator::const_iterator() :
    mLink(nullptr)
  {
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  intrusive_list<T, LinkType>::const_iterator::const_iterator(const intrusive_link* link) :
    mLink(link)
  {
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  intrusive_list<T, LinkType>::const_iterator::const_iterator(const const_iterator& rhs) :
    mLink(rhs.mLink)
  {
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  intrusive_list<T, LinkType>::const_iterator::const_iterator(const T& rhs) :
    mLink(to_link(rhs))
  {
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  intrusive_list<T, LinkType>::const_iterator::const_iterator(const iterator& rhs) :
    mLink(rhs.mLink)
  {
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  intrusive_list<T, LinkType>::const_iterator::~const_iterator()
  {
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::const_iterator& intrusive_list<T, LinkType>::const_iterator::operator=(const const_iterator& rhs)
  {
    mLink = rhs.mLink;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  bool intrusive_list<T, LinkType>::const_iterator::operator==(const const_iterator& rhs) const
  {
    return mLink == rhs.mLink;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  bool intrusive_list<T, LinkType>::const_iterator::operator!=(const const_iterator& rhs) const
  {
    return mLink != rhs.mLink;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::const_iterator& intrusive_list<T, LinkType>::const_iterator::operator++()
  {
    __stl_assert(mLink != nullptr, "Attempting to increment a null iterator");
    mLink = mLink->mNext;
    return *this;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::const_iterator intrusive_list<T, LinkType>::const_iterator::operator++(int)
  {
    __stl_assert(mLink != nullptr, "Attempting to increment a null iterator");
    intrusive_link* temp = mLink;
    mLink = mLink->mNext;
    return temp;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::const_iterator& intrusive_list<T, LinkType>::const_iterator::operator--()
  {
    __stl_assert(mLink != nullptr, "Attempting to decrement a null iterator");
    mLink = mLink->mPrevious;
    return *this;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::const_iterator intrusive_list<T, LinkType>::const_iterator::operator--(int)
  {
    __stl_assert(mLink != nullptr, "Attempting to decrement a null iterator");
    intrusive_link* temp = mLink;
    mLink = mLink->mPrevious;
    return temp;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::const_iterator::const_reference intrusive_list<T, LinkType>::const_iterator::operator*() const
  {
    __stl_assert(mLink != nullptr, "Attempting to dereference a null iterator");
    return to_t(mLink);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::const_iterator::const_pointer intrusive_list<T, LinkType>::const_iterator::operator->() const
  {
    __stl_assert(mLink != nullptr, "Attempting to dereference a null iterator");
    return &to_t(mLink);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  intrusive_list<T, LinkType>::intrusive_list()
  {
    // Ensure that T inherits from LinkType (only once)
    static_cast<LinkType*>(static_cast<T*>(nullptr));
    // Ensure that LinkType inherits from intrusive_link
    static_cast<intrusive_link*>(static_cast<LinkType*>(nullptr));

    // Setup the sentinel node to point at itself (empty list)
    mSentinel.mNext = &mSentinel;
    mSentinel.mPrevious = &mSentinel;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  intrusive_list<T, LinkType>::intrusive_list(intrusive_list&& rhs) :
    intrusive_list()
  {
    // Since we called the default constructor our mSentinel is now setup
    // Use our insertion helpers to directly steal the rhs list
    insert_after_helper(&mSentinel, rhs.begin(), rhs.end());
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  intrusive_list<T, LinkType>::~intrusive_list()
  {
    clear();
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::begin()
  {
    return iterator(mSentinel.mNext);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::const_iterator intrusive_list<T, LinkType>::begin() const
  {
    return const_iterator(mSentinel.mNext);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::const_iterator intrusive_list<T, LinkType>::cbegin() const
  {
    return const_iterator(mSentinel.mNext);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::end()
  {
    return iterator(&mSentinel);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::const_iterator intrusive_list<T, LinkType>::end() const
  {
    return const_iterator(&mSentinel);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::const_iterator intrusive_list<T, LinkType>::cend() const
  {
    return const_iterator(&mSentinel);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::reverse_iterator intrusive_list<T, LinkType>::rbegin()
  {
    return reverse_iterator(iterator(mSentinel->mPrev));
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::const_reverse_iterator intrusive_list<T, LinkType>::rbegin() const
  {
    return const_reverse_iterator(const_iterator(mSentinel->mPrev));
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::const_reverse_iterator intrusive_list<T, LinkType>::crbegin() const
  {
    return const_reverse_iterator(const_iterator(mSentinel->mPrev));
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::reverse_iterator intrusive_list<T, LinkType>::rend()
  {
    return reverse_iterator(iterator(&mSentinel));
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::const_reverse_iterator intrusive_list<T, LinkType>::rend() const
  {
    return const_reverse_iterator(const_iterator(&mSentinel));
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::const_reverse_iterator intrusive_list<T, LinkType>::crend() const
  {
    return const_reverse_iterator(const_iterator(&mSentinel));
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::reference intrusive_list<T, LinkType>::front()
  {
    __stl_assert(!empty(), "Cannot grab the front element from an empty list");
    return *iterator(mSentinel.mNext);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::const_reference intrusive_list<T, LinkType>::front() const
  {
    __stl_assert(!empty(), "Cannot grab the front element from an empty list");
    return *const_iterator(mSentinel.mNext);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::reference intrusive_list<T, LinkType>::back()
  {
    __stl_assert(!empty(), "Cannot grab the back element from an empty list");
    return *iterator(mSentinel.mPrevious);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::const_reference intrusive_list<T, LinkType>::back() const
  {
    __stl_assert(!empty(), "Cannot grab the back element from an empty list");
    return *const_iterator(mSentinel.mPrevious);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  const T& intrusive_list<T, LinkType>::push_front(const T& value)
  {
    // We guarantee that pushing an item that is within our own list is valid, so we must unlink it first
    // This is only a problem in the case that the item is at the front (or the back for push_back)
    to_link(value)->unlink();
    insert_after_helper(&mSentinel, value);
    return value;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  const T& intrusive_list<T, LinkType>::push_back(const T& value)
  {
    to_link(value)->unlink();
    insert_before_helper(&mSentinel, value);
    return value;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  T& intrusive_list<T, LinkType>::push_front(T& value)
  {
    to_link(value)->unlink();
    insert_after_helper(&mSentinel, value);
    return value;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  T& intrusive_list<T, LinkType>::push_back(T& value)
  {
    to_link(value)->unlink();
    insert_before_helper(&mSentinel, value);
    return value;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  const T& intrusive_list<T, LinkType>::pop_front() const
  {
    __stl_assert(!empty(), "Cannot pop_front on an empty list");
    const_iterator it(mSentinel.mNext);
    erase(it);
    return *it;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  const T& intrusive_list<T, LinkType>::pop_back() const
  {
    __stl_assert(!empty(), "Cannot pop_back on an empty list");
    const_iterator it(mSentinel.mPrevious);
    erase(it);
    return *it;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  T& intrusive_list<T, LinkType>::pop_front()
  {
    __stl_assert(!empty(), "Cannot pop_front on an empty list");
    iterator it(mSentinel.mNext);
    erase(it);
    return *it;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  T& intrusive_list<T, LinkType>::pop_back()
  {
    __stl_assert(!empty(), "Cannot pop_back on an empty list");
    iterator it(mSentinel.mPrevious);
    erase(it);
    return *it;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  template <typename IteratorType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::insert_before(const_iterator beforeThis, IteratorType begin, IteratorType end)
  {
    while (begin != end)
    {
      insert_before_helper(beforeThis.mLink, *begin);
      ++begin;
    }
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::insert_before(const_iterator beforeThis, const T& toBeInserted)
  {
    return insert_before_helper(beforeThis.mLink, toBeInserted);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::insert_before(const_iterator beforeThis, const_iterator begin, const_iterator end)
  {
    return insert_before_helper(beforeThis.mLink, begin, end);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::insert_before(const_iterator beforeThis, const_iterator begin, iterator end)
  {
    return insert_before_helper(beforeThis.mLink, begin, end);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::insert_before(const_iterator beforeThis, iterator begin, const_iterator end)
  {
    return insert_before_helper(beforeThis.mLink, begin, end);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::insert_before(const_iterator beforeThis, iterator begin, iterator end)
  {
    return insert_before_helper(beforeThis.mLink, begin, end);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::insert_before(const_iterator beforeThis, std::initializer_list<T> list)
  {
    for (const T* value : list)
    {
      insert_before_helper(beforeThis.mLink, *value);
    }
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  template <typename IteratorType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::insert_after(const_iterator afterThis, IteratorType begin, IteratorType end)
  {
    while (begin != end)
    {
      insert_after_helper(afterThis.mLink, *begin);
      ++begin;
    }
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::insert_after(const_iterator afterThis, const T& toBeInserted)
  {
    return insert_after_helper(afterThis.mLink, toBeInserted);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::insert_after(const_iterator afterThis, const_iterator begin, const_iterator end)
  {
    return insert_after_helper(afterThis.mLink, begin, end);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::insert_after(const_iterator afterThis, const_iterator begin, iterator end)
  {
    return insert_after_helper(afterThis.mLink, begin, end);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::insert_after(const_iterator afterThis, iterator begin, const_iterator end)
  {
    return insert_after_helper(afterThis.mLink, begin, end);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::insert_after(const_iterator afterThis, iterator begin, iterator end)
  {
    return insert_after_helper(afterThis.mLink, begin, end);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::insert_after(const_iterator afterThis, std::initializer_list<T> list)
  {
    for (const T* value : list)
    {
      insert_after_helper(afterThis.mLink, *value);
    }
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::erase(const_iterator it)
  {
    // Unfortunately, there is no way to check if this link is from our list without adding a lot of overhead
    iterator next = it.mLink->mNext;
    it.mLink->unlink();
    return next;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::erase(const_iterator begin, const_iterator end)
  {
    while (begin != end)
    {
      // If we don't move our iterator forward first then we'll
      // lose the mNext link when we erase the iterator.
      iterator toBeErased(begin.mLink);
      ++begin;

      erase(toBeErased);
    }
    return iterator(end.mLink);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  void intrusive_list<T, LinkType>::clear()
  {
    while (!empty())
    {
      bool unlinked = mSentinel.mNext->unlink();
      (void)unlinked;
      __stl_assert(unlinked, "A link in our list failed to unlink upon clearing");
    }

    __stl_assert(mSentinel.mNext == &mSentinel, "The mSentinel's mNext should point at itself after clearing");
    __stl_assert(mSentinel.mPrevious == &mSentinel, "The mSentinel's mPrevious should point at itself after clearing");
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  template <typename IteratorType>
  void intrusive_list<T, LinkType>::assign(IteratorType beginIt, IteratorType endIt)
  {
    clear();
    insert(begin(), beginIt, endIt);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  void intrusive_list<T, LinkType>::assign(std::initializer_list<T> list)
  {
    clear();
    insert(begin(), list);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  void intrusive_list<T, LinkType>::swap(intrusive_list& rhs)
  {
    intrusive_list& lhs = *this;

    // We can simply swap the sentinel nodes (very fast!)

    std::swap(lhs.mSentinel, rhs.mSentinel);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::size_type intrusive_list<T, LinkType>::size() const
  {
    size_t size = 0;
    for (const T& value : *this)
    {
      ++size;
    }
    return size;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::size_type intrusive_list<T, LinkType>::max_size() const
  {
    return static_cast<size_type>(-1);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  bool intrusive_list<T, LinkType>::empty() const
  {
    __stl_assert(
      (mSentinel.mNext == &mSentinel && mSentinel.mPrevious == &mSentinel) ||
      (mSentinel.mNext != &mSentinel && mSentinel.mPrevious != &mSentinel),
      "The mSentinel should point at itself only if the list is empty");
    return mSentinel.mNext == &mSentinel;
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::insert_after_helper(const intrusive_link* afterThisLink, const T& toBeInserted)
  {
    return insert_before_helper(afterThisLink->mNext, toBeInserted);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::insert_before_helper(const intrusive_link* beforeThisLink, const T& toBeInserted)
  {
    __stl_assert(beforeThisLink != nullptr && beforeThisLink->is_linked(), "We cannot insert into a link that isn't linked to anything (null iterator?)");

    const intrusive_link* toBeInsertedLink = to_link(toBeInserted);
    __stl_assert(!toBeInsertedLink->is_linked(), "When inserting a single value it must already be unlinked (prevents bugs with push_back/front)");

    const intrusive_link* afterThisLink = beforeThisLink->mPrevious;

    // Place the item we're splicing in between (haven't updated our own links yet)
    afterThisLink->mNext = toBeInsertedLink;
    beforeThisLink->mPrevious = toBeInsertedLink;

    toBeInsertedLink->mPrevious = afterThisLink;
    toBeInsertedLink->mNext = beforeThisLink;
    return iterator(toBeInsertedLink);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::insert_after_helper(const intrusive_link* afterThisLink, const_iterator begin, const_iterator end)
  {
    return insert_before_helper(afterThisLink->mNext, begin, end);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  typename intrusive_list<T, LinkType>::iterator intrusive_list<T, LinkType>::insert_before_helper(const intrusive_link* beforeThisLink, const_iterator begin, const_iterator end)
  {
    __stl_assert(beforeThisLink != nullptr && beforeThisLink->is_linked(), "We cannot insert into a link that isn't linked to anything (null iterator?)");

    // If the list were trying to splice in is empty, then do nothing (we actually need this case)
    if (begin == end)
    {
      return iterator(beforeThisLink);
    }

    const intrusive_link* afterThisLink = beforeThisLink->mPrevious;

    __stl_assert(begin.mLink->is_linked(),
      "The beginning iterator should be linked into a list");
    __stl_assert(end.mLink->is_linked(),
      "The ending iterator should be linked into a list");

    // For simplicity, instead of pointing at the end (one past) we point at the last element
    const_iterator last = end;
    --last;

    // Place the list we're splicing in between (haven't updated our own links yet)
    afterThisLink->mNext = begin.mLink;
    beforeThisLink->mPrevious = last.mLink;

    // We need to unlink the sub-list from whatever list it is within
    // Remember that 'end' is already one past, so we don't need to grab it's mNext
    const intrusive_link* previousLink = begin.mLink->mPrevious;
    const intrusive_link* nextLink = last.mLink->mNext;
    previousLink->mNext = nextLink;
    nextLink->mPrevious = previousLink;

    // Now update the list we're splicing in
    begin.mLink->mPrevious = afterThisLink;
    last.mLink->mNext = beforeThisLink;
    return iterator(begin.mLink);
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  T& intrusive_list<T, LinkType>::to_t(const intrusive_link* link)
  {
    __stl_assert(link != nullptr, "The link was null (often an indicator that we tried to use a default constructed iterator in an operation)");
    return *static_cast<T*>(static_cast<LinkType*>(const_cast<intrusive_link*>(link)));
  }

  /***********************************************************************************************/
  template <typename T, typename LinkType>
  const intrusive_link* intrusive_list<T, LinkType>::to_link(const T& value)
  {
    __stl_assert(&value != nullptr, "The value was null");
    return static_cast<const intrusive_link*>(static_cast<const LinkType*>(&value));
  }
}
