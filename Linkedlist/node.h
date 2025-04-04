/***********************************************************************
 * Header:
 *    NODE
 * Summary:
 *    One node in a linked list (and the functions to support them).
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        Node         : A class representing a Node
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    Jacob Mower 
 *    Elroe Woldemichael
 *    Thatcher Albiston 
 ************************************************************************/

#pragma once

#include <cassert>     // for ASSERT
#include <iostream>    // for NULL

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <class T>
class Node
{
public:
   //
   // Construct
   //
   //
   // Construct
   //

   Node()               : pNext(nullptr), pPrev(nullptr), data(T())  { }
   Node(const T&  data) : pNext(nullptr), pPrev(nullptr), data(data) { }
   Node(      T&& data) : pNext(nullptr), pPrev(nullptr), data(std::move(data)) { }

   //
   // Member variables
   //

   T data;                 // user data
   Node <T> * pNext;       // pointer to next node
   Node <T> * pPrev;       // pointer to previous node
};

/***********************************************
 * COPY
 * Copy the list from the pSource and return
 * the new list
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline Node <T> * copy(const Node <T> * pSource) 
{
   // If the source node is null, return null
   if (!pSource)
      return nullptr;

   // Create a new node with the data from the source node
   Node<T>* pDestination = new Node<T>(pSource->data);

   // Initialize pointers for traversing the source and destination lists
   Node<T>* pSrc = pSource->pNext;
   Node<T>* pDes = pDestination;

   // Traverse the source list and copy each node to the destination list
   for (Node<T>* p = pSrc; p; p = p->pNext)
      pDes = insert(pDes, p->data, true);

   // Return the head of the copied list
   return pDestination;
}

/***********************************************
 * Assign
 * Copy the values from pSource into pDestination
 * reusing the nodes already created in pDestination if possible.
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline void assign(Node <T> * & pDestination, const Node <T> * pSource)
{
   const Node<T>* pSrc = pSource;
   Node<T>* pDes = pDestination;
   Node<T>* pDesPrevious = pDestination;

   //Both loops have nodes
   while (pSrc && pDes)
   {
      pDesPrevious = pDes;
      pDes->data = pSrc->data;
      pDes = pDes->pNext;
      pSrc = pSrc->pNext;
   }
   
   //Destination has run out of nodes
   if (pSrc)
   {
      pDes = pDesPrevious;
      while (pSrc)
      {
         pDes = insert(pDes, pSrc->data, true);
         if (!pDestination)
            pDestination = pDes;
         
         pSrc = pSrc->pNext;
      }
   }
   //Source has run out of nodes, get rid of extra destination ones
   else if (pDes)
   {
      bool setToNull = false;
      
      if (pDes->pPrev)
         pDes->pPrev->pNext = nullptr;
      else
         setToNull = true;
      
      clear(pDes);
      
      //Source was empty so now destination needs to be.
      if (setToNull)
         pDestination = nullptr;
      
   }
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(Node <T>* &pLHS, Node <T>* &pRHS)
{
   std::swap(pLHS, pRHS);
}

/***********************************************
 * REMOVE
 * Remove the node pSource in the linked list
 *   INPUT  : the node to be removed
 *   OUTPUT : the pointer to the parent node
 *   COST   : O(1)
 **********************************************/
template <class T>
inline Node <T> * remove(const Node <T> * pRemove) 
{
   // If the node to be removed is null, return null
   if (!pRemove)
      return nullptr;

   // Update the previous node's next pointer, if it exists
   if (pRemove->pPrev)
      pRemove->pPrev->pNext = pRemove->pNext;

   // Update the next node's previous pointer, if it exists
   if (pRemove->pNext)
      pRemove->pNext->pPrev = pRemove->pPrev;

   // Determine the node to return after removal
   Node<T>* pReturn;
   if (pRemove->pPrev)
      pReturn = pRemove->pPrev;
   else
      pReturn = pRemove->pNext;

   // Delete the node to be removed
   delete pRemove;

   // Return the node that follows the removed node
   return pReturn;
}


/**********************************************
 * INSERT 
 * Insert a new node the the value in "t" into a linked
 * list immediately before the current position.
 *   INPUT   : t - the value to be used for the new node
 *             pCurrent - a pointer to the node before which
 *                we will be inserting the new node
 *             after - whether we will be inserting after
 *   OUTPUT  : return the newly inserted item
 *   COST    : O(1)
 **********************************************/
template <class T>
inline Node <T> * insert(Node <T> * pCurrent,
                  const T & t,
                  bool after = false)
{
   Node <T>* pNew = new Node <T>(t);
   // Before pCurrent 
   if (pCurrent != nullptr && !after)
   {
      pNew->pNext = pCurrent;
      pNew->pPrev = pCurrent->pPrev;
      pCurrent->pPrev = pNew;
      if (pNew->pPrev)
         pNew->pPrev->pNext = pNew;
   }
   // After pCurrent 
   if (pCurrent != nullptr && after)
   {
      pNew->pPrev = pCurrent;
      pNew->pNext = pCurrent->pNext;
      pCurrent->pNext = pNew;
      if (pNew->pNext)
         pNew->pNext->pPrev = pNew;
   }
   return pNew;
}

/******************************************************
 * SIZE
 * Find the size an unsorted linked list.  
 *  INPUT   : a pointer to the head of the linked list
 *            the value to be found
 *  OUTPUT  : number of nodes
 *  COST    : O(n)
 ********************************************************/
template <class T>
inline size_t size(const Node <T> * pHead)
{
   size_t s = 0;
   if (pHead)
   {
      for (const Node<T>* p = pHead; p; p = p->pNext)
         s += 1;
   }
   return s;
}

/***********************************************
 * DISPLAY
 * Display all the items in the linked list from here on back
 *    INPUT  : the output stream
 *             pointer to the linked list
 *    OUTPUT : the data from the linked list on the screen
 *    COST   : O(n)
 **********************************************/
template <class T>
inline std::ostream & operator << (std::ostream & out, const Node <T> * pHead)
{
   for (const Node<T>* p = pHead; p; p = p->pNext)
      std::cout << p->data << std::endl;
}

/*****************************************************
 * FREE DATA
 * Free all the data currently in the linked list
 *   INPUT   : pointer to the head of the linked list
 *   OUTPUT  : pHead set to NULL
 *   COST    : O(n)
 ****************************************************/
template <class T>
inline void clear(Node <T> * & pHead)
{
   while (pHead)
   {
      Node<T>* pDelete = pHead;
      pHead = pHead->pNext;
      delete pDelete;
   }
}


