/***********************************************************************
 * Header:
 *    BINARY NODE
 * Summary:
 *    One node in a binary tree (and the functions to support them).
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        BNode         : A class representing a BNode
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    Jacob Mower
 *    Thatcher Albintson
 *    Elroe Woldemichael
 ************************************************************************/

#pragma once

#include <iostream>  // for OFSTREAM
#include <cassert>

/*****************************************************************
 * BNODE
 * A single node in a binary tree.  Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <class T>
class BNode
{
public:
   // 
   // Construct
   //
   BNode() : data(T()), pParent(nullptr), pLeft(nullptr),
      pRight(nullptr) { }

   BNode(const T &  t) : data(t), pParent(nullptr), pLeft(nullptr),
      pRight(nullptr) { }

   BNode(T && t) : data(std::move(t)), pParent(nullptr), pLeft(nullptr),
      pRight(nullptr) { }

   //
   // Data
   //
   BNode <T> * pLeft;
   BNode <T> * pRight;
   BNode <T> * pParent;
   T data;
};

/*******************************************************************
 * SIZE BTREE
 * Return the size of a b-tree under the current node
 *******************************************************************/
template <class T>
inline size_t size(const BNode <T> * p)
{
   if (!p)
      return 0;
   return size(p->pLeft) + 1 + size(p->pRight);
}


/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T>
inline void addLeft(BNode <T> * pNode, BNode <T> * pAdd)
{
   if (pAdd)
      pAdd->pParent = pNode;
   
   pNode->pLeft = pAdd;
}

/******************************************************
 * ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <class T>
inline void addRight (BNode <T> * pNode, BNode <T> * pAdd)
{
   if (pAdd)
      pAdd->pParent = pNode;
   
   pNode->pRight = pAdd;
}

/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T>
inline void addLeft (BNode <T> * pNode, const T & t) 
{
   BNode <T>* pAdd = new BNode <T>(t);
   pAdd->pParent = pNode;
   pNode->pLeft = pAdd;
}

template <class T>
inline void addLeft(BNode <T>* pNode, T && t)
{
   BNode <T>* pAdd = new BNode <T>(std::move(t));
   pAdd->pParent = pNode;
   pNode->pLeft = pAdd;
}

/******************************************************
 * ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <class T>
void addRight (BNode <T> * pNode, const T & t)
{
   BNode <T>* pAdd = new BNode <T>(t);
   pAdd->pParent = pNode;
   pNode->pRight = pAdd;
}

template <class T>
void addRight(BNode <T>* pNode, T && t)
{
   BNode <T>* pAdd = new BNode <T>(std::move(t));
   pAdd->pParent = pNode;
   pNode->pRight = pAdd;
}

/*****************************************************
 * DELETE BINARY TREE
 * Delete all the nodes below pThis including pThis
 * using postfix traverse: LRV
 ****************************************************/
template <class T>
void clear(BNode <T> * & pThis)
{
   if (!pThis)
      return ;
   
   //Recurse down
   clear(pThis->pLeft);
   clear(pThis->pRight);
   delete pThis;

   // After deleting have the node be nullptr
   pThis = nullptr;
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(BNode <T>*& pLHS, BNode <T>*& pRHS)
{
   std::swap(pLHS, pRHS);
}

/**********************************************
 * COPY BINARY TREE
 * Copy pSrc->pRight to pDest->pRight and
 * pSrc->pLeft onto pDest->pLeft
 *********************************************/
template <class T>
BNode <T> * copy(const BNode <T> * pSrc) 
{
   //Source is empty
   if (!pSrc)
      return nullptr;
   BNode <T>* pDest = new BNode <T> (pSrc->data);

   //Left branch
   pDest->pLeft = copy(pSrc->pLeft);
   if (pDest->pLeft)
      pDest->pLeft->pParent = pDest;

   //Right Branch
   pDest->pRight = copy(pSrc->pRight);
   if (pDest->pRight)
      pDest->pRight->pParent = pDest;

   return pDest;
}

/**********************************************
 * assign
 * copy the values from pSrc onto pDest preserving
 * as many of the nodes as possible.
 *********************************************/
template <class T>
void assign(BNode <T> * & pDest, const BNode <T>* pSrc)
{
   //Source is empty, clear dest
   if (!pSrc)
   {
      clear(pDest);
      return;
   }

   //Destination is empty
   if (!pDest && pSrc)
      pDest = new BNode <T>(pSrc->data);
   
   //Neither is empty
   else if (pSrc && pDest)
      pDest->data = pSrc->data;
  
   //Recurse down
   assign(pDest->pRight, pSrc->pRight);
   assign(pDest->pLeft, pSrc->pLeft);

   //Connect parents and children
   if(pDest->pLeft)
      pDest->pLeft->pParent = pDest;
   if (pDest->pRight)
      pDest->pRight->pParent = pDest;

}
