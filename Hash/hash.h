/***********************************************************************
 * Header:
 *    HASH
 * Summary:
 *    Our custom implementation of std::unordered_Set
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        unordered_set           : A class that represents a hash
 *        unordered_set::iterator : An interator through hash
 * Author
 *    Elroe Woldemichael
 *    Jacob Mower
 *    Thatcher Albiston
 ************************************************************************/

#pragma once

#include "list.h"     // because this->buckets[0] is a list
#include "vector.h"   // because this->buckets is a vector
#include <memory>     // for std::allocator
#include <functional> // for std::hash
#include <cmath>      // for std::ceil
   

class TestHash;             // forward declaration for Hash unit tests

namespace custom
{
/************************************************
 * UNORDERED SET
 * A set implemented as a hash
 ************************************************/
template <typename T,
          typename Hash = std::hash<T>,
          typename EqPred = std::equal_to<T>,
          typename A = std::allocator<T> >
class unordered_set
{
   friend class ::TestHash;   // give unit tests access to the privates
public:
   //
   // Construct
   //
   unordered_set() : maxLoadFactor(1.0), numElements(0), buckets(8)
   {
   }
   unordered_set(size_t numBuckets) : maxLoadFactor(1.0), numElements(0), buckets(numBuckets)
   {
   }
   unordered_set(const unordered_set&  rhs) : maxLoadFactor(rhs.maxLoadFactor), numElements(rhs.numElements), buckets(rhs.buckets)
   {
   }
   unordered_set(unordered_set&& rhs) : maxLoadFactor(std::move(rhs.maxLoadFactor)), numElements(std::move(rhs.numElements)), buckets(std::move(rhs.buckets))
   {
   }
   template <class Iterator>                                                          
   unordered_set(Iterator first, Iterator last) : maxLoadFactor(1.0), numElements(0), buckets(std::distance(first, last))
   {                                                                                  // get the required amount of buckets to store the elements
      // insert the elements from the other container 
      for (auto it = first; it != last; it++)
         insert(*it);
   }
   ~unordered_set() { clear(); }

   //
   // Assign
   //
   unordered_set& operator=(const unordered_set& rhs)
   {
      maxLoadFactor = rhs.maxLoadFactor;
      numElements = rhs.numElements;
      // call vector's copy-assign operator 
      buckets = rhs.buckets;
      return *this;
   }
   unordered_set& operator=(unordered_set&& rhs)
   {
      // do nothing if we are RHS afterall, we are already ourself!!!
      if (this != &rhs) 
      {
         maxLoadFactor = std::move(rhs.maxLoadFactor);
         numElements = std::move(rhs.numElements);
         // call vector's move-assign operator 
         buckets = std::move(rhs.buckets);

         rhs.maxLoadFactor = 1.0;
         rhs.numElements = 0;
         rhs.buckets.resize(8);
      }
      return *this;
   }
   unordered_set& operator=(const std::initializer_list<T>& il)
   {
      // clear the set because we can't keep the old buckets 
      clear(); 

      // loop over the initializer list, inserting each element 
      for (const auto& elem : il) {
         insert(elem);
      }

      return *this;
   }
   void swap(unordered_set& rhs)
   {
      std::swap(this->numElements, rhs.numElements);
      std::swap(this->maxLoadFactor, rhs.maxLoadFactor);
      buckets.swap(rhs.buckets);
   }

   // 
   // Iterator
   //
   class iterator;
   class local_iterator;
   iterator begin()
   {
      // iterate through buckets 
      for (auto it = buckets.begin(); it != buckets.end(); it++)
      {
         // if the bucket isn't empty, return iterator to that element 
         if (!(*it).empty())
            return iterator(buckets.end(), it, (*it).begin());
      }
      // return end() if the set is empty 
      return end();
   }
   iterator end()
   {
      return iterator(buckets.end(), buckets.end(), buckets[0].end());
   }
   local_iterator begin(size_t iBucket)
   {
      return local_iterator(buckets[iBucket].begin());
   }
   local_iterator end(size_t iBucket)
   {
      return local_iterator(buckets[iBucket].end());
   }

   //
   // Access
   //
   size_t bucket(const T& t)
   {
      // mod the hash value of element by number of buckets to find the bucket that t belongs in 
      return Hash{}(t) % bucket_count();
   }
   iterator find(const T& t);

   //   
   // Insert
   //
   custom::pair<iterator, bool> insert(const T& t);
   void insert(const std::initializer_list<T> & il);
   void rehash(size_t numBuckets);
   void reserve(size_t num)
   {
      // rehash takes numBuckets 
      // maxLoadFactor = numElements / numBuckets 
      // Let numBuckets = x 
      // solve for x: 
      // maxLoadFactor = num / x 
      // maxLoadFactor * x = num 
      // x = num / maxLoadFactor 
      // cast to size_t because rehash takes a size_t parameter...
      rehash(size_t(num / max_load_factor()));
   }

   // 
   // Remove
   //
   void clear() noexcept
   {
      // iterate over buckets 
      for (auto bucket = buckets.begin() ; bucket != buckets.end() ;  bucket++)
      {
         // clear each bucket 
         (*bucket).clear();
      }
      numElements = 0;
   }
   iterator erase(const T& t);

   //
   // Status
   //
   size_t size() const 
   { 
      return numElements;
   }
   bool empty() const 
   { 
      return size() == 0; 
   }
   size_t bucket_count() const 
   { 
      return buckets.size();
   }
   size_t bucket_size(size_t i) const
   {
      return (size_t)buckets[i].size();
   }
   float load_factor() const noexcept 
   { 
      return (float)size() / bucket_count(); 
   }
   float max_load_factor() const noexcept 
   { 
      return (float)maxLoadFactor; 
   }
   void  max_load_factor(float m)
   {
      maxLoadFactor = m;
   }

private:

   // return the minimum number of buckets required to maintain correct load factor, given "num" elements
   size_t min_buckets_required(size_t num) const
   {
      // maxLoadFactor = numElements / bucket_count 
      // let bucket_count = x 
      // solve for x: 
      // maxLoadFactor = num / x 
      // maxLoadFactor * x = num 
      // x = num / maxLoadFactor 
      // round up because we can't round down and maintain correct load factor! 
      return std::ceil(float(num) / max_load_factor());
   }

   custom::vector<custom::list<T,A>> buckets;  // each bucket in the hash
   int numElements;                            // number of elements in the Hash
   float maxLoadFactor;                        // the ratio of elements to buckets signifying a rehash
};


/************************************************
 * UNORDERED SET ITERATOR
 * Iterator for an unordered set
 ************************************************/
template <typename T, typename H, typename E, typename A>
class unordered_set <T, H, E, A> ::iterator
{
   friend class ::TestHash;   // give unit tests access to the privates
   template <typename TT, typename HH, typename EE, typename AA>
   friend class custom::unordered_set;
public:
   // 
   // Construct
   iterator() : itVector(), itList(), itVectorEnd()
   {
   }
   iterator(const typename custom::vector<custom::list<T> >::iterator& itVectorEnd,
            const typename custom::vector<custom::list<T> >::iterator& itVector,
            const typename custom::list<T>::iterator &itList) : itVector(itVector), itList(itList), itVectorEnd(itVectorEnd)
   {
   }
   iterator(const iterator& rhs) : itVector(rhs.itVector), itList(rhs.itList), itVectorEnd(rhs.itVectorEnd)
   { 
   }

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
      itVector = rhs.itVector;
      itList = rhs.itList;
      itVectorEnd = rhs.itVectorEnd;
      return *this;
   }

   //
   // Compare
   //
   bool operator != (const iterator& rhs) const 
   { 
      return itList != rhs.itList || itVector != rhs.itVector; 
   }
   bool operator == (const iterator& rhs) const 
   { 
      return itList == rhs.itList && itVector == rhs.itVector;
   }

   // 
   // Access
   //
   T& operator * ()
   {
      return *(itList);
   }

   //
   // Arithmetic
   //
   iterator& operator ++ ();
   iterator operator ++ (int postfix)
   {
      iterator it(*this);
      ++(*this);
      return it;
   }

private:
   // friend that needs access to itVector and itList 
   friend unordered_set <T, H, E, A> ::iterator unordered_set<T, H, E, A>::erase(const T& t); 

   typename vector<list<T>>::iterator itVectorEnd;
   typename list<T>::iterator itList;
   typename vector<list<T>>::iterator itVector;
};


/************************************************
 * UNORDERED SET LOCAL ITERATOR
 * Iterator for a single bucket in an unordered set
 ************************************************/
template <typename T, typename H, typename E, typename A>
class unordered_set <T, H, E, A> ::local_iterator
{
   friend class ::TestHash;   // give unit tests access to the privates

   template <typename TT, typename HH, typename EE, typename AA>
   friend class custom::unordered_set;
public:
   // 
   // Construct
   //
   local_iterator() : itList()
   {
   }
   local_iterator(const typename custom::list<T>::iterator& itList) : itList(itList)
   {
   }
   local_iterator(const local_iterator& rhs) : itList(rhs.itList)
   { 
   }

   //
   // Assign
   //
   local_iterator& operator = (const local_iterator& rhs)
   {
      itList = rhs.itList;
      return *this;
   }

   // 
   // Compare
   //
   bool operator != (const local_iterator& rhs) const
   {
      return itList != rhs.itList;
   }
   bool operator == (const local_iterator& rhs) const
   {
      return itList == rhs.itList;
   }

   // 
   // Access
   //
   T& operator * ()
   {
      return *(itList);
   }

   // 
   // Arithmetic
   //
   local_iterator& operator ++ ()
   {
      ++itList;
      return *this;
   }
   local_iterator operator ++ (int postfix)
   {
      local_iterator it(*this);
      ++this->itList;
      return it;
   }

private:
   typename list<T>::iterator itList;
};


/*****************************************
 * UNORDERED SET :: ERASE
 * Remove one element from the unordered set
 ****************************************/
template <typename T, typename Hash, typename E, typename A>
typename unordered_set <T, Hash, E, A> ::iterator unordered_set<T,Hash,E,A>::erase(const T& t)
{
   // find element to be erased
   auto itErase = find(t);
   if (itErase == end())
      // return end() if element is not present 
      return itErase; 

   // determine the return value 
   auto itReturn = itErase; 
   ++itReturn; 

   // erase the element from the bucket 
   (*itErase.itVector).erase(itErase.itList);
   numElements--; 

   // return iterator to the next element 
   return itReturn;
}

/*****************************************
 * UNORDERED SET :: INSERT
 * Insert one element into the hash
 ****************************************/
template <typename T, typename H, typename E, typename A>
custom::pair<typename custom::unordered_set<T, H, E, A>::iterator, bool> unordered_set<T, H, E, A>::insert(const T& t)
{
   // 1. Find the bucket where the new element is to reside.
   size_t iBucket = bucket(t);
   // 2. See if the element is already there.If so, then return out.
   for (auto it = buckets[iBucket].begin(); it != buckets[iBucket].end(); it++)
      if (*it == t)
      {
         auto itVector = buckets.begin() + (int)iBucket; 
         return custom::pair<custom::unordered_set<T, H, E, A>::iterator, bool>(iterator(buckets.end(), itVector, it), false);
      }
   // 3. Reserve more space if we are already at the limit.
   if (min_buckets_required(numElements + 1) > bucket_count())
   {
      reserve(numElements * 2);
      iBucket = bucket(t);
   }
   // 4. Actually insert the new element on the back of the bucket.
   buckets[iBucket].push_back(t);
   numElements++;
   
   // 5. Return the results. 
   auto itVector = buckets.begin() + (int)iBucket; 
   return custom::pair<custom::unordered_set<T, H, E, A>::iterator, bool>(iterator(buckets.end(), itVector, buckets[iBucket].find(t)), true);
}
template <typename T, typename H, typename E, typename A>
void unordered_set<T, H, E, A>::insert(const std::initializer_list<T> & il)
{
   // loop through the initializer list 
   for (auto it = il.begin(); it != il.end(); ++it)
      // call our insert(const T& t) 
      insert(*it);
}

/*****************************************
 * UNORDERED SET :: REHASH
 * Re-Hash the unordered set by numBuckets
 ****************************************/
template <typename T, typename Hash, typename E, typename A>
void unordered_set<T, Hash, E, A>::rehash(size_t numBuckets)
{
   // do nothing if there is nothing to do 
   if (numBuckets <= bucket_count())
      return;

   // allocate a new vector of size numBuckets 
   custom::vector<list<T>> bucketsNew(numBuckets);
   
   // iterate over old buckets, moving elements over based on newly hashed index 
   for (auto it = this->begin(); it != this->end(); it++ )
   {
      // create our new index 
      size_t newBucketIndex = Hash{}(*it) % numBuckets;
      // move element over to its new index 
      bucketsNew[newBucketIndex].push_back(std::move(*it));
   }
   // which of these is better?
   //buckets = std::move(bucketsNew);
   buckets.swap(bucketsNew);
}


/*****************************************
 * UNORDERED SET :: FIND
 * Find an element in an unordered set
 ****************************************/
template <typename T, typename H, typename E, typename A>
typename unordered_set <T, H, E, A> ::iterator unordered_set<T, H, E, A>::find(const T& t)
{
   // get the index of element t 
   size_t ibucket = bucket(t);
   // use List's find() to find the element in the bucket 
   auto itList = buckets[ibucket].find(t);

   // if the element was found, return an iterator to it 
   if (itList != buckets[ibucket].end())
      return iterator(buckets.end(), custom::vector<list<T>>::iterator(ibucket, buckets), itList);
   // element wasn't found, return end() 
   return end();
}

/*****************************************
 * UNORDERED SET :: ITERATOR :: INCREMENT
 * Advance by one element in an unordered set
 ****************************************/
template <typename T, typename H, typename E, typename A>
typename unordered_set <T, H, E, A> ::iterator & unordered_set<T, H, E, A>::iterator::operator ++ ()
{
   // 1. Only advance if we are not already at the end 
   if (itVector == itVectorEnd)
      return *this;

   // 2. Advance the list iterator. If we are not at the end, then we are done. 
   ++itList;
   if (itList != (*itVector).end())
      return *this;

   // 3. We are at the end of the list. Find the next bucket 
   ++itVector;
   while (itVector != itVectorEnd && (*itVector).empty())
      ++itVector;
   if (itVector != itVectorEnd)
      itList = (*itVector).begin();
   return *this;
}

/*****************************************
 * SWAP
 * Stand-alone unordered set swap
 ****************************************/
template <typename T, typename H, typename E, typename A>
void swap(unordered_set<T,H,E,A>& lhs, unordered_set<T,H,E,A>& rhs)
{
   std::swap(lhs, rhs);
}

}
