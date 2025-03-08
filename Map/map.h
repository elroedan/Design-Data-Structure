/***********************************************************************
 * Header:
 *    MAP
 * Summary:
 *    Our custom implementation of a std::map 
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        map                 : A class that represents a map
 *        map::iterator       : An iterator through a map
 * Author
 *    Jacob Mower
 *    Elroe Woldemichael
 *    Thatcher Albiston 
 ************************************************************************/

#pragma once

#include "pair.h"     // for pair
#include "bst.h"      // no nested class necessary for this assignment

#ifndef debug
#ifdef DEBUG
#define debug(x) x
#else
#define debug(x)
#endif // DEBUG
#endif // !debug

class TestMap;

namespace custom
{

/*****************************************************************
 * MAP
 * Create a Map, similar to a Binary Search Tree
 *****************************************************************/
template <class K, class V>
class map
{
   friend class ::TestMap;

   template <class KK, class VV>
   friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs); 
public:
   using Pairs = custom::pair<K, V>;

   // 
   // Construct
   //
   map()
   {
   }
   map(const map &  rhs) : bst(rhs.bst)
   {
   }
   map(map && rhs) : bst(std::move(rhs.bst))
   {
   }
   template <class Iterator>
   map(Iterator first, Iterator last) 
   {
      // use map's range insert 
      insert(first, last);
   }
   map(const std::initializer_list <Pairs>& il)
   {
      insert(il);
   }
  ~map()         
   {
     /* Calls ~BST() */
   }

   //
   // Assign
   //
   map & operator = (const map & rhs) 
   {
      this->bst = rhs.bst;
      return *this;
   }
   map & operator = (map && rhs)
   {
      this->bst = std::move(rhs.bst);
      return *this;
   }
   map & operator = (const std::initializer_list <Pairs> & il)
   {
      clear();
      // use map's init insert 
      insert(il);
      return *this;
   }
   
   // 
   // Iterator
   //
   class iterator;
   iterator begin() 
   { 
      return iterator(bst.begin());
   }
   iterator end() 
   { 
      return iterator(bst.end());
   }

   // 
   // Access
   //
   const V & operator [] (const K & k) const;
         V & operator [] (const K & k);
   const V & at (const K& k) const;
         V & at (const K& k);
   iterator find(const K & k)
   {
      return iterator(bst.find(k));
   }

   //
   // Insert
   //
   custom::pair<typename map::iterator, bool> insert(Pairs && rhs)
   {
      // use bst's insert 
      auto pbst = bst.insert(std::move(rhs));
      // turn std::pair into our custom::pair 
      return pair<iterator, bool>(iterator(pbst.first), pbst.second);
   }
   custom::pair<typename map::iterator, bool> insert(const Pairs & rhs)
   {
      // use bst's insert 
      auto pbst = bst.insert(rhs);
      // turn std::pair into our custom::pair 
      return pair<iterator, bool>(iterator(pbst.first), pbst.second);
   }

   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
      for (auto it = first; it != last; it++)
         bst.insert(*it, true);
   }
   void insert(const std::initializer_list <Pairs>& il)
   {
      for (auto&& element : il)
         bst.insert(element, true);
   }

   //
   // Remove
   //
   void clear() noexcept
   {
      bst.clear();
   }
   size_t erase(const K& k);
   iterator erase(iterator it);
   iterator erase(iterator first, iterator last);

   //
   // Status
   //
   bool empty() const noexcept 
   { 
      return bst.empty();
   }
   size_t size() const noexcept 
   { 
      return bst.size();
   }


private:

   // the students DO NOT need to use a nested class
   BST < pair <K, V >> bst;
};


/**********************************************************
 * MAP ITERATOR
 * Forward and reverse iterator through a Map, just call
 * through to BSTIterator
 *********************************************************/
template <typename K, typename V>
class map <K, V> :: iterator
{
   friend class ::TestMap;
   template <class KK, class VV>
   friend class custom::map; 
public:
   //
   // Construct
   //
   iterator() : it()
   {
   }
   iterator(const typename BST < pair <K, V> > :: iterator & rhs)
   { 
      this->it = rhs;
   }
   iterator(const iterator & rhs)
   { 
      this->it = rhs.it;
   }

   //
   // Assign
   //
   iterator & operator = (const iterator & rhs)
   {
      this->it = rhs.it;
      return *this;
   }

   //
   // Compare
   //
   bool operator == (const iterator & rhs) const 
   {
      return this->it == rhs.it;
   }
   bool operator != (const iterator & rhs) const 
   { 
      return this->it != rhs.it;
   }

   // 
   // Access
   //
   const pair <K, V> & operator * () const
   {
      return *it;
   }

   //
   // Increment
   //
   iterator & operator ++ ()
   {
      ++it;
      return *this;
   }
   iterator operator ++ (int postfix)
   {
      iterator temp(*this);
      ++it;
      return temp;
   }
   iterator & operator -- ()
   {
      --it;
      return *this;
   }
   iterator  operator -- (int postfix)
   {
      iterator temp(*this);
      --it;
      return temp;
   }

private:

   // Member variable
   typename BST < pair <K, V >>  :: iterator it;   
};


/*****************************************************
 * MAP :: SUBSCRIPT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
V& map <K, V> :: operator [] (const K& key)
{
   // create a pair using key 
   pair<K, V> pair(key);
   // look for the key in our bst 
   auto pbst = bst.find(pair);
   // did we find the key? 
   if (pbst != bst.end())
      // return the associated value 
      return pbst.pNode->data.second;
   else
      // otherwise insert the key and return it's value
      return bst.insert(pair, true).first.pNode->data.second;
}

/*****************************************************
 * MAP :: SUBSCRIPT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
const V& map <K, V> :: operator [] (const K& key) const
{
   // create a pair using key 
   pair<K, V> pair(key);
   // look for the key in our bst 
   auto pbst = bst.find(pair);
   // did we find the key? 
   if (pbst != bst.end())
      // return the associated value 
      return pbst.pNode->data.second;
   else
      // otherwise insert the key and return it's value
      return bst.insert(pair, true).first.pNode->data.second;
}

/*****************************************************
 * MAP :: AT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
V& map <K, V> ::at(const K& key)
{
   // look for the key in our bst 
   auto pbst = bst.find(pair<K, V>(key));
   // was it found? 
   if (pbst != bst.end())
      // return the associated value 
      return pbst.pNode->data.second;
   else
      // otherwise throw 
      throw std::out_of_range("invalid map<K, T> key");
}
/*****************************************************
 * MAP :: AT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
const V& map <K, V> ::at(const K& key) const
{
   // look for the key in our bst 
   auto pbst = bst.find(pair<K, V>(key));
   // was it found? 
   if (pbst != bst.end())
      // return the associated value 
      return pbst.pNode->data.second;
   else
      // otherwise throw 
      throw std::out_of_range("invalid map<K, T> key");
}

/*****************************************************
 * SWAP
 * Swap two maps
 ****************************************************/
template <typename K, typename V>
void swap(map <K, V>& lhs, map <K, V>& rhs)
{
   // call bst's swap 
   lhs.bst.swap(rhs.bst);
}

/*****************************************************
 * ERASE
 * Erase one element
 ****************************************************/
template <typename K, typename V>
size_t map<K, V>::erase(const K& k)
{
   // look for the key, k 
   iterator it = find(k);
   // found it? 
   if (it == this->end())
      // no...
      return 0;
   // yes... so, erase it
   erase(it);
   return 1;
}

/*****************************************************
 * ERASE
 * Erase several elements
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::erase(map<K, V>::iterator first, map<K, V>::iterator last)
{
   // loop through elements from first to last 
   while (first != last)
      // update first as next element after deleting previous first 
      first = erase(first);
   return last;
}

/*****************************************************
 * ERASE
 * Erase one element
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::erase(map<K, V>::iterator it)
{
   // use bst's erase 
   return iterator(bst.erase(it.it));
}

}; //  namespace custom

