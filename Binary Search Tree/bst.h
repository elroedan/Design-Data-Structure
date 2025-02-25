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
 * BALANCE FUNCTION IS RECURSIVE
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
   size_t size()  const noexcept { return numElements;   }
   
private:

   class BNode;
   BNode * root;              // root node of the binary search tree
   size_t numElements;        // number of elements currently in the tree

   // assign 
   void assign(BNode*& pDest, const BNode* pSrc);
   void deleteBinaryTree(BNode*& pThis);
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
   BNode() : data(T()), pParent(nullptr), pLeft(nullptr),
      pRight(nullptr), isRed(true) { }

   BNode(const T& t) : data(t), pParent(nullptr), pLeft(nullptr),
      pRight(nullptr), isRed(true) { }

   BNode(T&& t) : data(std::move(t)), pParent(nullptr), pLeft(nullptr),
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
   bool isRightChild(BNode * pNode) const { return true; }
   bool isLeftChild( BNode * pNode) const { return true; }

   // balance the tree
   void balance();

   

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
   }
   iterator(const iterator & rhs)         
   { 
   }
   iterator & operator = (const iterator & rhs)
   {
      return *this;
   }

   // compare
   bool operator == (const iterator & rhs) const
   {
      return true;
   }
   bool operator != (const iterator & rhs) const
   {
      return true;
   }

   // de-reference. Cannot change because it will invalidate the BST
   const T & operator * () const 
   {
      return *(new T);
   }

   // increment and decrement
   iterator & operator ++ ();
   iterator   operator ++ (int postfix)
   {
      return *this;
   }
   iterator & operator -- ();
   iterator   operator -- (int postfix)
   {
      return *this;;
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
BST <T> ::BST()
{
   numElements = 0;
   root = nullptr;
}

/*********************************************
 * BST :: COPY CONSTRUCTOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST ( const BST<T>& rhs) : root(nullptr), numElements(0)
{
   (*this) = rhs;
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
BST <T> :: BST(const std::initializer_list<T>& il) : root(nullptr), numElements(0)
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
BST <T> & BST <T> :: operator = (const std::initializer_list<T>& il)
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
void BST <T> :: swap (BST <T>& rhs)
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
   std::pair<iterator, bool> pairReturn(end(), false);
   return pairReturn;
}

template <typename T>
std::pair<typename BST <T> ::iterator, bool> BST <T> :: insert(T && t, bool keepUnique)
{
   std::pair<iterator, bool> pairReturn(end(), false);
   return pairReturn;
}

/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST <T> ::iterator BST <T> :: erase(iterator & it)
{  
   return end();
}

/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST <T> ::clear() noexcept
{
   deleteBinaryTree(root);
}

template <typename T> 
void BST <T> ::deleteBinaryTree(BNode*& pThis)
{
   if (!pThis)
      return;

   //Recurse down
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
   return end();
}


/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST <T> :: iterator BST<T> :: find(const T & t)
{
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
      pNode->pParent = *this;

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
      pNode->pParent = *this;

   this->pRight = pNode;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{
   BNode* pAdd = new BNode(t);
   pAdd->pParent = *this;
   this->pLeft = pAdd;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> ::BNode::addLeft(T && t)
{
   BNode* pAdd = new BNode(std::move(t));
   pAdd->pParent = *this;
   this->pLeft = pAdd;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{
   BNode* pAdd = new BNode(t);
   pAdd->pParent = *this;
   this->pRight = pAdd;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> ::BNode::addRight(T && t)
{
   BNode* pAdd = new BNode(std::move(t));
   pAdd->pParent = *this;
   this->pRight = pAdd;
}

/******************************************************
 * BINARY NODE :: ASSIGN
 * A
 ******************************************************/
template <typename T>
void BST <T> :: assign(BNode *& pDest, const BNode * pSrc)
{
   //Source is empty, clear dest
   if (!pSrc)
   {
      deleteBinaryTree(pDest);
      return;
   }

   //Destination is empty
   if (!pDest && pSrc)
      pDest = new BNode(pSrc->data);

   //Neither is empty
   else if (pSrc && pDest)
      pDest->data = pSrc->data;

   //Recurse down
   assign(pDest->pRight, pSrc->pRight);
   assign(pDest->pLeft, pSrc->pLeft);

   //Connect parents and children
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
   if (!pParent->isRed())
      return; 
   // Case 3: if the aunt is red, then just recolor
   else
   {
      if (pParent->isLeftChild())
      {
         if (pParent->pParent->pRight && pParent->pParent->pRight->isRed())
         {
            pParent->pParent->isRed = true; 
            pParent->pParent->pRight = false; 
            pParent = false;
         }
      }
      else
      {
         if (pParent->pParent->pLeft && pParent->pParent->pLeft->isRed())
         {
            pParent->pParent->isRed = true;
            pParent->pParent->pRight = false;
            pParent = false;
         }
      }
   }
   
   // Case 4: if the aunt is black or non-existant, then we need to rotate
   if (pParent->isRed() && !pParent->pParent->isRed())
   {
      if (pParent->isLeftChild())
      {
         // Case 4a: We are mom's left and mom is granny's left
         if (isLeftChild())
         {
            pParent->addRight(pParent->pParent);
            pParent->pParent->addLeft(pParent->pRight);

            // pHead <- pParent 
            if (pParent->pParent->pParent == nullptr)
            {
               pParent->pParent = nullptr; 
               root = pParent; 
            }
            else
            {
               // is our grandparent a left child? 
               if (pParent->pParent->isLeftChild())
                  // set our parent to be our great grandparent's new left child
                  pParent->pParent->pParent->addLeft(pParent);
               // its a right child 
               else
                  // set our parent to be our great grandparent's new right child
                  pParent->pParent->pParent->addRight(pParent);
            }
            pParent->pParent->isRed = true; 
            pParent->isRed = false; 
         }
         // Case 4c: We are mom's right and mom is granny's left
         else
         {
            pParent->pParent->addLeft(pRight);
            pParent->addRight(pLeft);

            if (!pParent->pParent)
               pParent = nullptr;
            else if (pParent->pParent->isRightChild())
               pParent->pParent->pParent->addRight(*this);
            else
               pParent->pParent->pParent->addLeft(*this);

            this->addRight(pParent->pParent);
            this->addLeft(pParent);

            pParent->pParent->isRed = true;
            this->isRed = false;
         }
      }
      else
      {
         // case 4d: we are mom's left and mom is granny's right
         if (isLeftChild())
         {
            pParent->pParent->addRight(pLeft);
            pParent->addLeft(pRight);

            if (!pParent->pParent)
               pParent = nullptr;
            else if (pParent->pParent->isRightChild())
               pParent->pParent->pParent->addRight(*this);
            else 
               pParent->pParent->pParent->addLeft(*this);

            this->addLeft(pParent->pParent);
            this->addRight(pParent);

            pParent->pParent->isRed = true;
            this->isRed = false; 
         }
         // case 4b: We are mom's right and mom is granny's right
         else
         {
            pParent->addLeft(pParent->pParent);
            pParent->pParent->addRight(pParent->pLeft);
            // pHead <- pParent 
            if (pParent->pParent->pParent == nullptr)
            {
               pParent->pParent = nullptr;
               root = pParent;
            }
            else
            {
               // is our grandparent a left child? 
               if (pParent->pParent->isLeftChild())
                  // set our parent to be our great grandparent's new left child
                  pParent->pParent->pParent->addLeft(pParent);
               // its a right child 
               else
                  // set our parent to be our great grandparent's new right child
                  pParent->pParent->pParent->addRight(pParent);
            }
            pParent->pParent->isRed = true;
            pParent->isRed = false;
         }
      }
   }
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
   return *this;  
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator -- ()
{
   return *this;

}


} // namespace custom


