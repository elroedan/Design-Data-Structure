/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    Thatcher Albiston
 *    Elroe Woldemichael
 *    Jacob Mower
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator

class TestList; // forward declaration for unit tests
class TestHash; // forward declaration for hash used later

namespace custom
{

/**************************************************
 * LIST
 * Just like std::list
 **************************************************/
template <typename T, typename A = std::allocator<T>>
class list
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   friend void swap(list& lhs, list& rhs);
public:
   
   //
   // Construct
   //
   
   list(const A& a = A())  
   {
      numElements = 0;
      pHead = nullptr;
      pTail = nullptr;
   }
   list(list <T, A>& rhs, const A& a = A())
   {
      pHead = pTail = nullptr;
      numElements = 0;

      for (auto it = rhs.begin(); it != rhs.end(); it++)
         this->push_back(*it);
   }
   list(list <T, A>&& rhs, const A& a = A());
   list(size_t num, const T & t, const A& a = A());
   list(size_t num, const A& a = A());
   list(const std::initializer_list<T>& il, const A& a = A()) 
   {
      pHead = pTail = nullptr;
      numElements = 0;

      for (auto it = il.begin(); it != il.end(); it++)
         this->push_back(*it);
   }
   template <class Iterator>
   list(Iterator first, Iterator last, const A& a = A()): pHead(nullptr),
   pTail(nullptr), numElements(0), alloc(a)
   {
      for (auto it = first; it < last; it++)
      {
         Node* pNew = new Node(*it);
//
         pNew->pPrev = pTail;
         if (pTail)
            pTail->pNext = pNew;
         else
            pHead = pNew;

         pTail = pNew;
         numElements++;
      }
   }
   ~list()
   { 
      clear();
   }
   
   //
   // Assign
   //
   
   list <T, A> & operator = (list <T, A> &  rhs);
   list <T, A> & operator = (list <T, A> && rhs);
   list <T, A> & operator = (const std::initializer_list<T>& il);
   void swap(list <T, A>& rhs)
   {
      std::swap(this->pHead, rhs.pHead);
      std::swap(this->pTail, rhs.pTail);
      std::swap(this->numElements, rhs.numElements);

   }

   //
   // Iterator
   //
   
   class iterator;
   iterator begin()  { return iterator (pHead);   }
   iterator rbegin() { return iterator (pTail);   }
   iterator end()    { return iterator (nullptr); }
   
   //
   // Access
   //
   
   T & front();
   T & back();

   //
   // Insert
   //
   
   void push_front(const T &  data);
   void push_front(      T && data);
   void push_back (const T &  data);
   void push_back (      T && data);
   iterator insert(iterator it, const T &  data);
   iterator insert(iterator it,       T && data);

   //
   // Remove
   //
   
   void pop_back();
   void pop_front();
   void clear();
   iterator erase(const iterator & it);

   //
   // Status
   //
   
   bool empty()  const { return numElements == 0; }
   size_t size() const { return numElements;   }

private:
   // nested linked list class
   class Node;

   // member variables
   A    alloc;         // use alloacator for memory allocation
   size_t numElements; // though we could count, it is faster to keep a variable
   Node * pHead;       // pointer to the beginning of the list
   Node * pTail;       // pointer to the ending of the list
};

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <typename T, typename A>
class list <T, A> :: Node
{
public:
   //
   // Construct
   //

   Node() : pNext(nullptr), pPrev(nullptr), data(T()) { }
   Node(const T& data) : pNext(nullptr), pPrev(nullptr), data(data) { }
   Node(T&& data) : pNext(nullptr), pPrev(nullptr), data(std::move(data)) { }

   //
   // Member Variables
   //

   T data;             // user data
   Node * pNext;       // pointer to next node
   Node * pPrev;       // pointer to previous node
};

/*************************************************
 * LIST ITERATOR
 * Iterate through a List, non-constant version
 ************************************************/
template <typename T, typename A>
class list <T, A> :: iterator
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   template <typename TT, typename AA>
   friend class custom::list;
   
public:
   // constructors, destructors, and assignment operator
   iterator()  
   {
      p = nullptr;
   }
   iterator(Node * pRHS)
   {
      p = pRHS;
   }
   iterator(const iterator  & rhs) 
   {
      p = rhs.p;
   }
   iterator & operator = (const iterator & rhs)
   {
      this->p = rhs.p;
      return *this ;
   }
   
   // equals, not equals operator
   bool operator == (const iterator & rhs) const { return this->p == rhs.p ; }
   bool operator != (const iterator & rhs) const { return this->p != rhs.p; }

   // dereference operator, fetch a node
   T & operator * ()
   {
      return p->data;
   }

   // postfix increment
   iterator operator ++ (int postfix)
   {
      iterator it = *this;
      this->p = p->pNext;
      return it;
   }

   // prefix increment
   iterator & operator ++ ()
   {
      this->p = p->pNext;
      return *this;
   }
   
   // postfix decrement
   iterator operator -- (int postfix)
   {
      iterator it = *this;
      this->p = p->pPrev;
      return it;
   }

   // prefix decrement
   iterator & operator -- ()
   {
      this->p = p->pPrev;
      return *this;
   } 

   // two friends who need to access p directly
   friend iterator list <T, A> :: insert(iterator it, const T &  data);
   friend iterator list <T, A> :: insert(iterator it,       T && data);
   friend iterator list <T, A> :: erase(const iterator & it);

private:

   typename list <T, A> :: Node * p;
};

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T, typename A>
list <T, A> ::list(size_t num, const T & t, const A& a) : pHead(nullptr),
   pTail(nullptr), numElements(0), alloc(a)
{
   if (num > 0)
   {
      for (size_t i = 0; i < num; i++)
         push_back(t);
   }
}

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T, typename A>
list <T, A> ::list(size_t num, const A& a) : pHead(nullptr),
pTail(nullptr), numElements(0), alloc(a)
{
   if (num > 0)
   {
      for (size_t i = 0; i < num; i++)
      {
         Node* pNew = new Node();
         
         pNew->pPrev = pTail;
         if (pTail)
            pTail->pNext = pNew;
         else
            pHead = pNew;
         
         pTail = pNew;
         numElements++;
      }
   }
}

/*****************************************
 * LIST :: MOVE constructors
 * Steal the values from the RHS
 ****************************************/
template <typename T, typename A>
list <T, A> ::list(list <T, A>&& rhs, const A& a) :
   numElements(rhs.numElements), pHead(rhs.pHead), pTail(rhs.pTail), alloc(a)
{
   rhs.pHead = rhs.pTail = nullptr;
   rhs.numElements = 0;
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS 
 *********************************************/
template <typename T, typename A>
list <T, A>& list <T, A> :: operator = (list <T, A> && rhs)
{
   clear();
   swap(rhs);
   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T, typename A>
list <T, A> & list <T, A> :: operator = (list <T, A> & rhs)
{
   iterator itRHS = rhs.begin();
   iterator itLHS = this->begin();
   
   while (itRHS != rhs.end() && itLHS != this->end())
   {
      *itLHS = *itRHS;
      itLHS++;
      itRHS++;
   }
   
   if (itRHS != rhs.end())
   {
      while (itRHS != rhs.end())
      {
         push_back(*itRHS);
         itRHS++;
      }
   }
   
   else if (rhs.empty())
      clear();
   
   else if (itLHS != this->end())
   {
      Node* p = itLHS.p;
      pTail = p->pPrev;
      Node* pNext = p->pNext;
      
      while (p)
      {
         pNext = p->pNext;
         delete p;
         p = pNext;
         numElements--;
      }
      
      pTail->pNext = nullptr;
   }
      
   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T, typename A>
list <T, A>& list <T, A> :: operator = (const std::initializer_list<T>& rhs)
{
   if (rhs.size() == size())
   {
      Node* pDest = pHead;
      for (auto it = rhs.begin(); it != rhs.end(); it++)
      {
         pDest->data = *it;
         pDest = pDest->pNext;
      }
         
   }
   
   else if (rhs.size() > size())
   {
      //Our source is bigger than dest so adjust the dest
      Node* pDest = pHead;
      auto itRHS = rhs.begin();
      for (size_t i = 0; i < numElements; i++)
      {
         pDest->data = *itRHS;
         itRHS++;
         pDest = pDest->pNext;
      }
      for (size_t i = numElements; i < rhs.size(); i++)
      {
         push_back(*itRHS);
         itRHS++;
      }
   }
   
//   The dest is bigger so move the data and take down the
   else
   {
      Node* pDest = pHead;
      for (auto it = rhs.begin(); it != rhs.end(); it++)
      {
         pDest->data = *it;
         pDest = pDest->pNext;
      }
      pTail = pDest->pPrev;
      while (pDest)
      {
         Node* pNext = pDest->pNext;
         delete pDest;
         pDest = pNext;
         numElements--;
      }
      pTail->pNext = nullptr;
   }

   numElements = rhs.size();
   return *this;
}

/**********************************************
 * LIST :: CLEAR
 * Remove all the items currently in the linked list
 *     INPUT  :
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T, typename A>
void list <T, A> :: clear()
{
   Node* p = pHead;
   
   while (p)
   {
      Node* pNext = p->pNext;
      delete p;
      p = pNext;
   }
   pHead = nullptr;
   pTail = nullptr;
   numElements = 0;
}

/*********************************************
 * LIST :: PUSH BACK
 * add an item to the end of the list
 *    INPUT  : data to be added to the list
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> ::push_back(const T& data)
{
   Node* pNew = new Node(data);

   pNew->pPrev = pTail;
   if (pTail)
      pTail->pNext = pNew;
   else
      pHead = pNew;

   pTail = pNew;
   numElements++;
}

template <typename T, typename A>
void list <T, A> ::push_back(T && data)
{
   Node* pNew = new Node(std::move(data));
   
   pNew->pPrev = pTail;
   if (pTail)
      pTail->pNext = pNew;
   else
      pHead = pNew;

   pTail = pNew;
   numElements++;
}

/*********************************************
 * LIST :: PUSH FRONT
 * add an item to the head of the list
 *     INPUT  : data to be added to the list
 *     OUTPUT :
 *     COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> :: push_front(const T & data)
{
   Node* pNew = new Node(data);

   pNew->pNext = pHead;
   if (pHead)
      pHead->pPrev = pNew;
   else
      pTail = pNew;

   pHead = pNew;
   numElements++;
}

template <typename T, typename A>
void list <T, A> ::push_front(T && data)
{
   Node* pNew = new Node(std::move(data));

   pNew->pNext = pHead;
   if (pHead)
      pHead->pPrev = pNew;
   else
      pTail = pNew;

   pHead = pNew;
   numElements++;
}


/*********************************************
 * LIST :: POP BACK
 * remove an item from the end of the list
 *    INPUT  : 
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> ::pop_back()
{
   if (pTail) 
   {
      //Only one item in list
      if (pTail == pHead)
      {
         delete pTail;
         pTail = nullptr;
         pHead = nullptr;
      }
      //More than one item in the list
      else
      {
         Node* pRemove = pTail;
         pTail = pTail->pPrev;
         pTail->pNext = nullptr;
         delete pRemove;
      }
      numElements--;
   }
}

/*********************************************
 * LIST :: POP FRONT
 * remove an item from the front of the list
 *    INPUT  :
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> ::pop_front()
{
   if (pHead)
   {
      //Only one item in list
      if (pTail == pHead)
      {
         delete pHead;
         pTail = nullptr;
         pHead = nullptr;
      }
      //More than one item in the list
      else
      {
         Node* pRemove = pHead;
         pHead = pHead->pNext;
         pHead->pPrev = nullptr;
         delete pRemove;
      }
      numElements--;
   }
}

/*********************************************
 * LIST :: FRONT
 * retrieves the first element in the list
 *     INPUT  : 
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T, typename A>
T & list <T, A> :: front()
{
   if (pHead)
      return pHead->data;
   throw "ERROR: unable to access data from an empty list";
}

/*********************************************
 * LIST :: BACK
 * retrieves the last element in the list
 *     INPUT  : 
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T, typename A>
T & list <T, A> :: back()
{
   if (pTail)
      return pTail->data;
   throw "ERROR: unable to access data from an empty list";
}


/******************************************
 * LIST :: REMOVE
 * remove an item from the middle of the list
 *     INPUT  : an iterator to the item being removed
 *     OUTPUT : iterator to the new location 
 *     COST   : O(1)
 ******************************************/
template <typename T, typename A>
typename list <T, A> :: iterator  list <T, A> :: erase(const list <T, A> :: iterator & it)
{
   iterator itNext = end();
   
   if (it.p)
   {
      if (it.p->pNext)
      {
         it.p->pNext->pPrev = it.p->pPrev;
         itNext = it.p->pNext;
      }
      else {
         pTail = pTail->pPrev;
      }
      
      if (it.p->pPrev)
         it.p->pPrev->pNext = it.p->pNext;
      else
         pHead = pHead->pNext;
      
      delete it.p;
      numElements --;
   }
   
   return itNext;
}

/******************************************
 * LIST :: INSERT
 * add an item to the middle of the list
 *     INPUT  : data to be added to the list
 *              an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T, typename A>
typename list <T, A> :: iterator list <T, A> :: insert(list <T, A> :: iterator it,
                                                 const T & data) 
{
   if (empty())
   {
      pHead = new Node(data);
      pTail = pHead;
      numElements = 1;
      return begin();
   }
   else if (it == end())
   {
      Node* pNew = new Node(data);
      pTail->pNext = pNew;
      pNew->pPrev = pTail;
      pTail = pNew;
      numElements++;
      return iterator(pNew);
   }
   else
   {
      Node* pNew = new Node(data);
      pNew->pPrev = it.p->pPrev;
      pNew->pNext = it.p;
      
      if (pNew->pPrev)
         pNew->pPrev->pNext = pNew;
      else
         pHead = pNew;
      
      if (pNew->pNext)
         pNew->pNext->pPrev = pNew;
      else
         pTail = pNew;
      numElements++;
      return iterator(pNew);
   }
}


/******************************************
 * LIST :: INSERT
 * add several items into the middle of the list
 *     INPUT  : data to be added to the list
 *              an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T, typename A>
typename list <T, A> ::iterator list <T, A> ::insert(list <T, A> ::iterator it,
   T && data)
{
   if (empty())
   {
      pHead = new Node(std::move(data));
      pTail = pHead;
      numElements = 1;
      return begin();
   }
   else if (it == end())
   {
      Node* pNew = new Node(std::move(data));
      pTail->pNext = pNew;
      pNew->pPrev = pTail;
      pTail = pNew;
      numElements++;
      return iterator(pNew);
   }
   else
   {
      Node* pNew = new Node(std::move(data));
      pNew->pPrev = it.p->pPrev;
      pNew->pNext = it.p;
      
      if (pNew->pPrev)
         pNew->pPrev->pNext = pNew;
      else
         pHead = pNew;
      
      if (pNew->pNext)
         pNew->pNext->pPrev = pNew;
      else
         pTail = pNew;
      numElements++;
      return iterator(pNew);
   }
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS
 *********************************************/
template <typename T, typename A>
void swap(list <T, A> & lhs, list <T, A> & rhs)
{
   lhs.swap(rhs);
}

}; // namespace custom


