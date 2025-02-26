/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    Jacob Mower 
 *    Elroe Woldemichael
 *    Thacther Albintson
 *
 * BALANCE FUNCTION IS RECURSIVE ONLY RECURSE ON CASE 3!!!!!!!!!!!!!!!!!!!!!!!!
 *   CASE 3 ALWAYS RECURSES OFF OF GRANDPARENT
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>
#include <utility>
#include <memory>     // for std::allocator
#include <functional> // for std::less
#include <utility>    // for std::pair

class TestBST; // forward declaration for unit tests
class TestSet;
class TestMap;

namespace custom
{

   template <typename TT>
   class set;
   template <typename KK, typename VV>
   class map;

/*****************************************************************
 * BINARY SEARCH TREE
 * Create a Binary Search Tree
 *****************************************************************/
template <typename T>
class BST
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestSet;
   friend class ::TestMap;

   template <class TT>
   friend class custom::set;

   template <class KK, class VV>
   friend class custom::map;
public:
   //
   // Construct
   //

   BST();
   BST(const BST &  rhs);
   BST(      BST && rhs);
   BST(const std::initializer_list<T>& il);
   ~BST();

   //
   // Assign
   //

   BST & operator = (const BST &  rhs);
   BST & operator = (      BST && rhs);
   BST & operator = (const std::initializer_list<T>& il);
   void swap(BST & rhs);

   //
   // Iterator
   //

   class iterator;
   iterator   begin() const noexcept;
   iterator   end()   const noexcept { return iterator(nullptr); }

   //
   // Access
   //

   iterator find(const T& t);

   // 
   // Insert
   //

   std::pair<iterator, bool> insert(const T&  t, bool keepUnique = false);
   std::pair<iterator, bool> insert(      T&& t, bool keepUnique = false);

   //
   // Remove
   // 

   iterator erase(iterator& it);
   void   clear() noexcept;

   // 
   // Status
   //

   bool   empty() const noexcept { return size() == 0; }
   size_t size()  const noexcept { return numElements; }
   
private:

   class BNode;
   BNode * root;              // root node of the binary search tree
   size_t numElements;        // number of elements currently in the tree

   // assign 
   void assign(BNode * & pDest, const BNode * pSrc);
   void deleteBinaryTree(BNode * & pThis);
};


/*****************************************************************
 * BINARY NODE
 * A single node in a binary tree. Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <typename T>
class BST <T> :: BNode
{
public:
   // 
   // Construct
   //  
   BNode() : data(), pParent(nullptr), pLeft(nullptr),
      pRight(nullptr), isRed(true) { }

   BNode(const T &  t) : data(t), pParent(nullptr), pLeft(nullptr),
      pRight(nullptr), isRed(true) { }

   BNode(T && t) : data(std::move(t)), pParent(nullptr), pLeft(nullptr),
      pRight(nullptr), isRed(true) { }

   //
   // Insert
   //
   void addLeft (BNode * pNode);
   void addRight(BNode * pNode);
   void addLeft (const T &  t);
   void addRight(const T &  t);
   void addLeft(       T && t);
   void addRight(      T && t);

   // 
   // Status
   //
   bool isRightChild(BNode * pNode) const { return pNode->pRight == this; }
   bool isLeftChild( BNode * pNode) const { return pNode->pLeft  == this; }

   // balance the tree
   void balance();
   void rotateRight(BNode * pGrandparent);
   void rotateLeft( BNode * pGrandparent);
   

#ifdef DEBUG
   //
   // Verify
   //
   std::pair <T,T> verifyBTree() const;
   int findDepth() const;
   bool verifyRedBlack(int depth) const;
   int computeSize() const;
#endif // DEBUG

   //
   // Data
   //
   T data;                  // Actual data stored in the BNode
   BNode* pLeft;          // Left child - smaller
   BNode* pRight;         // Right child - larger
   BNode* pParent;        // Parent
   bool isRed;              // Red-black balancing stuff
};

/**********************************************************
 * BINARY SEARCH TREE ITERATOR
 * Forward and reverse iterator through a BST
 *********************************************************/
template <typename T>
class BST <T> :: iterator
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestSet;
   friend class ::TestMap;

   template <class KK, class VV>
   friend class custom::map;
public:
   // constructors and assignment
   iterator(BNode * p = nullptr)          
   { 
      pNode = p; 
   }
   iterator(const iterator & rhs)         
   { 
      pNode = rhs.pNode;
   }
   iterator & operator = (const iterator & rhs)
   {
      pNode = rhs.pNode; 
      return *this;
   }

   // compare
   bool operator == (const iterator & rhs) const
   {
      return pNode == rhs.pNode;
   }
   bool operator != (const iterator & rhs) const
   {
      return pNode != rhs.pNode;
   }

   // de-reference. Cannot change because it will invalidate the BST
   const T & operator * () const 
   {
      return pNode->data;
   }

   // increment and decrement
   iterator& operator ++ ();
   iterator   operator ++ (int postfix)
   {
      iterator old(this);
      ++this;
      return old;
   }
   iterator& operator -- ();
   iterator   operator -- (int postfix)
   {
      iterator old(this);
      ++this;
      return old;
   }

   // must give friend status to remove so it can call getNode() from it
   friend BST <T> :: iterator BST <T> :: erase(iterator & it);

private:
   
    // the node
    BNode * pNode;
};


/*********************************************
 *********************************************
 *********************************************
 ******************** BST ********************
 *********************************************
 *********************************************
 *********************************************/


 /*********************************************
  * BST :: DEFAULT CONSTRUCTOR
  ********************************************/
template <typename T>
BST <T> :: BST()
{
   root = nullptr;
   numElements = 0;
}

/*********************************************
 * BST :: COPY CONSTRUCTOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST ( const BST<T> & rhs) : root(nullptr), numElements(0)
{
   *this = rhs;
}

/*********************************************
 * BST :: MOVE CONSTRUCTOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST(BST <T> && rhs) : root(nullptr), numElements(0)
{
   root = rhs.root;
   numElements = rhs.numElements;
   rhs.root = nullptr;
   rhs.numElements = 0;
}

/*********************************************
 * BST :: INITIALIZER LIST CONSTRUCTOR
 * Create a BST from an initializer list
 ********************************************/
template <typename T>
BST <T> :: BST(const std::initializer_list<T> & il) : root(nullptr), numElements(0)
{
   if (il.size())
      *this = il; 
}

/*********************************************
 * BST :: DESTRUCTOR
 ********************************************/
template <typename T>
BST <T> :: ~BST()
{
   clear(); 
}


/*********************************************
 * BST :: ASSIGNMENT OPERATOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const BST <T> & rhs)
{
   assign(root, rhs.root);
   numElements = rhs.numElements; 
   return *this; 
}

/*********************************************
 * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
 * Copy nodes onto a BTree
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const std::initializer_list<T> & il)
{
   clear(); 
   for (auto it = il.begin(); it != il.end(); ++it)
      insert(*it);
   return *this;
}

/*********************************************
 * BST :: ASSIGN-MOVE OPERATOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (BST <T> && rhs)
{
   clear(); 
   swap(rhs);
   return *this;
}

/*********************************************
 * BST :: SWAP
 * Swap two trees
 ********************************************/
template <typename T>
void BST <T> :: swap (BST <T> & rhs)
{
   std::swap(root, rhs.root);
   std::swap(numElements, rhs.numElements);
}

/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/
template <typename T>
std::pair<typename BST <T> :: iterator, bool> BST <T> :: insert(const T & t, bool keepUnique)
{
   if (keepUnique)
   {
      // We found the element in the tree...
      auto it = find(t);
      if (it != end())
      {
         std::pair<iterator, bool> pairReturn(it, false);
         return pairReturn;
      }
   }

   // Create a new node
   BNode* pNew = new BNode(t);

   if (root == nullptr)
      root = pNew; 
   else
   {
      BNode * pParent = nullptr;
      BNode * pCurrent = root; 

      // find the new nodes destination... 
      while (pCurrent)
      {
         pParent = pCurrent; 
         if (t < pCurrent->data)
         {
            pCurrent = pCurrent->pLeft;
            if (pCurrent == nullptr)
               pParent->addLeft(pNew);
         }
         else
         {
            pCurrent = pCurrent->pRight; 
            if (pCurrent == nullptr)
               pParent->addRight(pNew);
         }
      }
   }

   // red-black balancing...
   pNew->balance(); 

   // Update root if need be... 
   if (root->pParent)
   {
      BNode * pNode = root; 
      BNode * pNewRoot = nullptr; 

      // climb up the tree until we find the new root...
      while (pNode)
      {
         pNewRoot = pNode;
         pNode = pNode->pParent; 
      }
      root = pNewRoot;
   }
   numElements++; 
   return std::pair<iterator, bool>(iterator(pNew), true);
}

template <typename T>
std::pair<typename BST <T> :: iterator, bool> BST <T> :: insert(T && t, bool keepUnique)
{
   if (keepUnique)
   {
      // We found the element in the tree...
      auto it = find(t);
      if (it != end())
      {
         std::pair<iterator, bool> pairReturn(it, false);
         return pairReturn;
      }
   }

   // Create a new node
   BNode * pNew = new BNode(std::move(t));

   if (root == nullptr)
      root = pNew;
   else
   {
      BNode * pParent = nullptr;
      BNode * pCurrent = root;

      // find the new nodes destination...
      while (pCurrent)
      {
         pParent = pCurrent;
         if (pNew->data < pCurrent->data)
         {
            pCurrent = pCurrent->pLeft;
            if (pCurrent == nullptr)
               pParent->addLeft(pNew);
         }
         else
         {
            pCurrent = pCurrent->pRight;
            if (pCurrent == nullptr)
               pParent->addRight(pNew);
         }
      }
   }

   // red-black balancing...
   pNew->balance();

   // Update root if need be... 
   if (root->pParent)
   {
      BNode* pNode = root;
      BNode* pNewRoot = nullptr;

      // climb up the tree until we find the new root...
      while (pNode)
      {
         pNewRoot = pNode;
         pNode = pNode->pParent;
      }
      root = pNewRoot;
   }
   numElements++; 
   return std::pair<iterator, bool>(iterator(pNew), true);
}

/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST<T> :: iterator BST<T> :: erase(iterator & it)
{
   if (!it.pNode) return end(); // If iterator is invalid, return end()

   iterator itNext = it;
   ++itNext; // Move to the next iterator before deleting the node

   // Case 1: Node has no children (leaf node)
   if (!it.pNode->pLeft && !it.pNode->pRight)
   {
      if (it.pNode->pParent)
      {
         if (it.pNode->isRightChild(it.pNode->pParent))
            it.pNode->pParent->pRight = nullptr;
         else
            it.pNode->pParent->pLeft = nullptr;
      }
      else
      {
         // The node is the root of the BST
         root = nullptr;
      }
   }
   // Case 2: Node has only one child
   else if (!it.pNode->pLeft || !it.pNode->pRight)
   {
      BNode * child = (it.pNode->pLeft) ? it.pNode->pLeft : it.pNode->pRight;
      child->pParent = it.pNode->pParent;

      if (it.pNode->pParent)
      {
         if (it.pNode->isRightChild(it.pNode->pParent))
            it.pNode->pParent->pRight = child;
         else
            it.pNode->pParent->pLeft = child; 
      }
      else
      {
         // The node is the root of the BST
         root = child;
      }
   }
   // Case 3: Node has two children
   else
   {

      // Swap Pointers
      // Update parent's child
      if (it.pNode->isLeftChild(it.pNode->pParent))
         it.pNode->pParent->pLeft = itNext.pNode;
      else
         it.pNode->pParent->pRight = itNext.pNode;

      // Update children's parent but only if
      // they are not parent child
      if (itNext.pNode->isLeftChild(it.pNode))
      {
         // Swap childs parent
         it.pNode->pRight->pParent = itNext.pNode;
         // Swap right nodes
         std::swap(it.pNode->pRight, itNext.pNode->pRight);
         // Swap remaining pointers
         it.pNode->pLeft = itNext.pNode->pRight;
         itNext.pNode->pLeft = it.pNode;
         itNext.pNode->pParent = it.pNode->pParent;
         it.pNode->pParent = itNext.pNode;
      }
      else if (itNext.pNode->isRightChild(it.pNode))
      {
         // Swap childs parent
         it.pNode->pLeft->pParent = itNext.pNode;
         // Swap left nodes
         std::swap(it.pNode->pLeft, itNext.pNode->pLeft);
         // Swap remaining pointers
         it.pNode->pRight = itNext.pNode->pRight;
         itNext.pNode->pRight = it.pNode;
         itNext.pNode->pParent = it.pNode->pParent;
         it.pNode->pParent = itNext.pNode;
      }
      else
      {
         // Swap Parents
         std::swap(it.pNode->pParent, itNext.pNode->pParent);
         // Swap lefts
         std::swap(it.pNode->pLeft, itNext.pNode->pLeft);
         // Swap rights
         std::swap(it.pNode->pRight, itNext.pNode->pRight);
         // Swap childrens parents(us)
         // Do we need an IF statement? 
         itNext.pNode->pRight->pParent = itNext.pNode;
         itNext.pNode->pLeft->pParent = itNext.pNode;

         // let parent know who its new (temporary) child is...
         if (itNext.pNode->isRightChild(it.pNode->pParent))
            it.pNode->pParent->pRight = it.pNode; 
         else 
            it.pNode->pParent->pLeft = it.pNode;
      }



      // Recursively delete the successor node
      erase(it);
      return itNext; // Return next valid iterator
   }

   delete it.pNode; // Free memory
   numElements--;   // Decrement element count
   return itNext;   // Return next valid iterator
}


/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST <T> :: clear() noexcept
{
   deleteBinaryTree(root);
   numElements = 0; 
}

template <typename T> 
void BST <T> :: deleteBinaryTree(BNode * & pThis)
{
   if (!pThis)
      return;

   // Recurse down
   deleteBinaryTree(pThis->pLeft);
   deleteBinaryTree(pThis->pRight);
   delete pThis;

   // After deleting have the node be nullptr
   pThis = nullptr;
}

/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <typename T>
typename BST <T> :: iterator custom :: BST <T> :: begin() const noexcept
{
   if (empty())
      return end();
   BNode * pNode = root; 
   // starting from root, go as far left as we can...
   while (pNode->pLeft)
      pNode = pNode->pLeft; 
   return iterator(pNode);
}


/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST <T> :: iterator BST<T> :: find(const T & t)
{
   BNode * pNode = root; 
   // this is just binary search... 
   while (pNode)
   {
      if (pNode->data == t)
         return iterator(pNode);
      else if (t < pNode->data)
         pNode = pNode->pLeft;
      else
         pNode = pNode->pRight;
   }
   return end();
}

/******************************************************
 ******************************************************
 ******************************************************
 *********************** B NODE ***********************
 ******************************************************
 ******************************************************
 ******************************************************/

 
/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addLeft (BNode * pNode)
{
   if (pNode)
      pNode->pParent = this;

   this->pLeft = pNode;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (BNode * pNode)
{
   if (pNode)
      pNode->pParent = this;

   this->pRight = pNode;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{
   BNode * pAdd = new BNode(t);
   pAdd->pParent = this;
   this->pLeft = pAdd;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft(T && t)
{
   BNode * pAdd = new BNode(std::move(t));
   pAdd->pParent = this;
   this->pLeft = pAdd;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{
   BNode * pAdd = new BNode(t);
   pAdd->pParent = this;
   this->pRight = pAdd;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight(T && t)
{
   BNode * pAdd = new BNode(std::move(t));
   pAdd->pParent = this;
   this->pRight = pAdd;
}

/******************************************************
 * BINARY NODE :: ASSIGN
 * recursive, called from copy-assignment operator
 * copies a binary tree
 ******************************************************/
template <typename T>
void BST <T> :: assign(BNode * & pDest, const BNode * pSrc)
{
   // Source is empty, clear dest
   if (!pSrc)
   {
      deleteBinaryTree(pDest);
      return;
   }

   // Destination is empty
   if (!pDest && pSrc)
   {
      pDest = new BNode(pSrc->data);
      pDest->isRed = pSrc->isRed; // dont forget to copy the color!
   }

   // Neither is empty
   else if (pSrc && pDest)
   {
      pDest->data = pSrc->data;
      pDest->isRed = pSrc->isRed; // dont forget to copy the color!
   }

   // Recurse down
   assign(pDest->pRight, pSrc->pRight);
   assign(pDest->pLeft, pSrc->pLeft);

   // Connect parents and children
   if (pDest->pLeft)
      pDest->pLeft->pParent = pDest;
   if (pDest->pRight)
      pDest->pRight->pParent = pDest;
}

#ifdef DEBUG
/****************************************************
 * BINARY NODE :: FIND DEPTH
 * Find the depth of the black nodes. This is useful for
 * verifying that a given red-black tree is valid
 ****************************************************/
template <typename T>
int BST <T> :: BNode :: findDepth() const
{
   // if there are no children, the depth is ourselves
   if (pRight == nullptr && pLeft == nullptr)
      return (isRed ? 0 : 1);

   // if there is a right child, go that way
   if (pRight != nullptr)
      return (isRed ? 0 : 1) + pRight->findDepth();
   else
      return (isRed ? 0 : 1) + pLeft->findDepth();
}

/****************************************************
 * BINARY NODE :: VERIFY RED BLACK
 * Do all four red-black rules work here?
 ***************************************************/
template <typename T>
bool BST <T> :: BNode :: verifyRedBlack(int depth) const
{
   bool fReturn = true;
   depth -= (isRed == false) ? 1 : 0;

   // Rule a) Every node is either red or black
   assert(isRed == true || isRed == false); // this feels silly

   // Rule b) The root is black
   if (pParent == nullptr)
      if (isRed == true)
         fReturn = false;

   // Rule c) Red nodes have black children
   if (isRed == true)
   {
      if (pLeft != nullptr)
         if (pLeft->isRed == true)
            fReturn = false;

      if (pRight != nullptr)
         if (pRight->isRed == true)
            fReturn = false;
   }

   // Rule d) Every path from a leaf to the root has the same # of black nodes
   if (pLeft == nullptr && pRight && nullptr)
      if (depth != 0)
         fReturn = false;
   if (pLeft != nullptr)
      if (!pLeft->verifyRedBlack(depth))
         fReturn = false;
   if (pRight != nullptr)
      if (!pRight->verifyRedBlack(depth))
         fReturn = false;

   return fReturn;
}


/******************************************************
 * VERIFY B TREE
 * Verify that the tree is correctly formed
 ******************************************************/
template <typename T>
std::pair <T, T> BST <T> :: BNode :: verifyBTree() const
{
   // largest and smallest values
   std::pair <T, T> extremes;
   extremes.first = data;
   extremes.second = data;

   // check parent
   if (pParent)
      assert(pParent->pLeft == this || pParent->pRight == this);

   // check left, the smaller sub-tree
   if (pLeft)
   {
      assert(!(data < pLeft->data));
      assert(pLeft->pParent == this);
      pLeft->verifyBTree();
      std::pair <T, T> p = pLeft->verifyBTree();
      assert(!(data < p.second));
      extremes.first = p.first;

   }

   // check right
   if (pRight)
   {
      assert(!(pRight->data < data));
      assert(pRight->pParent == this);
      pRight->verifyBTree();

      std::pair <T, T> p = pRight->verifyBTree();
      assert(!(p.first < data));
      extremes.second = p.second;
   }

   // return answer
   return extremes;
}

/*********************************************
 * COMPUTE SIZE
 * Verify that the BST is as large as we think it is
 ********************************************/
template <typename T>
int BST <T> :: BNode :: computeSize() const
{
   return 1 +
      (pLeft  == nullptr ? 0 : pLeft->computeSize()) +
      (pRight == nullptr ? 0 : pRight->computeSize());
}
#endif // DEBUG

/******************************************************
 * BINARY NODE :: BALANCE
 * Balance the tree from a given location
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: balance()
{
   // Case 1: if we are the root, then color ourselves black and call it a day.
   if (pParent == nullptr)
   {
      isRed = false;
      return;
   }

   // Case 2: if the parent is black, then there is nothing left to do
   
   // we should still be red...
   assert(isRed);
   // we are not root...
   assert(pParent != nullptr);

   if (!pParent->isRed)
      return;

   // Case 3: if the aunt is red, then just recolor 
   // our parent must be red...
   assert(pParent->isRed);

   // we can't have an aunt if our parent is root...
   if (pParent->pParent == nullptr)
   {
      pParent->balance(); // color our parent black 
      return;
   }

   // we have a grandparent 
   assert(pParent->pParent);

   // make things easier to read...
   BNode * pGrandparent = pParent->pParent;
   BNode * pAunt = nullptr;
   bool parentIsLeft = pParent->isLeftChild(pGrandparent);
   if (parentIsLeft)
      pAunt = pGrandparent->pRight;
   else
      pAunt = pGrandparent->pLeft;

   // Our Aunt exists and is red...
   if (pAunt && pAunt->isRed)
   {
      pGrandparent->isRed = true;  // color the grandparent red
      pParent->isRed = false;      // color the parent black 
      pAunt->isRed = false;        // color the aunt black    
      pGrandparent->balance();     // balance up the tree if necessary... 
      return;
   }

   // Case 4: if the aunt is black or non-existant, then we need to rotate

   // If we've made it here then we know that either the aunt is black or non-existant and that our parent is red...
   assert(pAunt == nullptr || !pAunt->isRed);
   assert(pParent->isRed);

   if (parentIsLeft) // mom is granny's left 
   {
      if (isLeftChild(pParent)) // Case 4a: We are mom's left and mom is granny's left 
      {
         rotateRight(pGrandparent);
      }
      else // Case 4c: We are mom's right and mom is granny's left 
      {
         // move our children...
         pGrandparent->addLeft(pRight);
         pParent->addRight(pLeft);
         BNode * pOldParent = pParent; // in case we're root, don't lose our parent...

         // we take granny's place...
         if (pGrandparent->pParent == nullptr)
            pParent = nullptr;
         else if (pGrandparent->isRightChild(pGrandparent->pParent))
            pGrandparent->pParent->addRight(this);
         else
            pGrandparent->pParent->addLeft(this);

         // granny and mom are our children now...
         addRight(pGrandparent);
         addLeft(pOldParent);

         // recolor grandparent and ourselves... 
         pGrandparent->isRed = true;
         isRed = false;
      }
   }
   else // mom is granny's right 
   {
      if (isRightChild(pParent)) // Case 4b: We are mom's right and mom is granny's right 
      {
         rotateLeft(pGrandparent);
      }
      else // Case 4d: We are mom's left and mom is granny's right 
      {
         // move our children... 
         pGrandparent->addRight(pLeft);
         pParent->addLeft(pRight);
         BNode * pOldParent = pParent; // in case we're root, don't lose our parent...

         // we take granny's place... 
         if (pGrandparent->pParent == nullptr)
            pParent = nullptr;
         else if (pGrandparent->isRightChild(pGrandparent->pParent))
            pGrandparent->pParent->addRight(this);
         else
            pGrandparent->pParent->addLeft(this);

         // mom and granny are our children now... 
         addLeft(pGrandparent);
         addRight(pOldParent);

         // recolor granny and ourselves... 
         pGrandparent->isRed = true; 
         isRed = false; 
      }
   }
}

// can we modify the right rotate and left rotate to cover all four scenarios with case 4??? 

/******************************************************
 * BINARY NODE :: RIGHT ROTATE
 * Case 4 a) red node is left child of red parent and parent is
 * left child of black grandparent
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: rotateRight(BNode * pGrandparent) 
{ 
   // Save the head of this tree...
   BNode * pHead = pGrandparent->pParent;
   // Save our sibling...
   BNode * pSibling = pParent->pRight;

   // Parent adds Grandparent to its right 
   pParent->addRight(pGrandparent); // this updates our parent's left, ie, our sibling, to our grandparent, thus we need to save our sibling first...
   // Grandparent adds our sibling (right) to the left 
   pGrandparent->addLeft(pSibling);
   // Parent becomes the new head of this tree
   if (pHead) // pHead is not nullptr (grandparent was not root)
   {
      if (pGrandparent->isLeftChild(pHead)) // grandparent was a left child 
         pHead->addLeft(pParent);
      else                                      // grandparent was a right child 
         pHead->addRight(pParent);
   }
   else // parent is now the root 
      pParent->pParent = nullptr;

   // Grandparent is colored red 
   pGrandparent->isRed = true;
   // Parent is colored black 
   pParent->isRed = false;
}

/******************************************************
 * BINARY NODE :: LEFT ROTATE
 * Case 4 b) red node is right child of red parent and parent is
 * right child of black grandparent
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: rotateLeft(BNode * pGrandparent)
{
   // Save the head of this tree...
   BNode * pHead = pGrandparent->pParent;
   // Save our sibling...
   BNode * pSibling = pParent->pLeft;

   // Parent adds Grandparent to its left 
   pParent->addLeft(pGrandparent); // this updates our parent's left, ie, our sibling, to our grandparent, thus we need to save our sibling first...
   // Grandparent adds our sibling (left) to the right 
   pGrandparent->addRight(pSibling);
   // Parent becomes the new head of this tree
   if (pHead) // pHead is not nullptr (grandparent was not root)
   {
      if (pGrandparent->isLeftChild(pHead)) // grandparent was a left child 
         pHead->addLeft(pParent);
      else                                      // grandparent was a right child 
         pHead->addRight(pParent);
   }
   else // parent is now the root 
      pParent->pParent = nullptr;

   // Grandparent is colored red 
   pGrandparent->isRed = true;
   // Parent is colored black 
   pParent->isRed = false;
}

/*************************************************
 *************************************************
 *************************************************
 ****************** ITERATOR *********************
 *************************************************
 *************************************************
 *************************************************/     

/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator ++ ()
{
   if (pNode)
   {
      if (pNode->pRight)
      {
         // jig right then go left...
         pNode = pNode->pRight;
         while (pNode->pLeft)
            pNode = pNode->pLeft;
      }
      else if (pNode->pRight == nullptr && pNode->isLeftChild(pNode->pParent))
      {
         // previous node is our parent...
         pNode = pNode->pParent;
      }
      else if (pNode->pRight == nullptr && pNode->isRightChild(pNode->pParent))
      {
         // go up to root or until we're no longer a right child
         while (pNode->pParent && pNode->isRightChild(pNode->pParent))
            pNode = pNode->pParent;
         // previous element is nullptr or left's parent...
         pNode = pNode->pParent;
      }
   }
   return *this;
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator -- ()
{
   if (pNode)
   {
      if (pNode->pLeft)
      {
         // jig left then go right...
         pNode = pNode->pLeft;
         while (pNode->pRight)
            pNode = pNode->pRight;
      }
      else if (pNode->pLeft == nullptr && pNode->isRightChild(pNode->pParent))
      {
         // parent is the previous node...
         pNode = pNode->pParent;
      }
      else if (pNode->pLeft == nullptr && pNode->isLeftChild(pNode->pParent))
      {
         // go up until we're a right child or root...
         while (pNode->pParent && pNode->isLeftChild(pNode->pParent))
            pNode = pNode->pParent;
         // previous is nullptr or right child's parent...
         pNode = pNode->pParent;
      }
   }
   return *this;
}


} // namespace custom


