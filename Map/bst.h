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
      BST(const BST& rhs);
      BST(BST&& rhs);
      BST(const std::initializer_list<T>& il);
      ~BST();

      //
      // Assign
      //

      BST& operator = (const BST& rhs);
      BST& operator = (BST&& rhs);
      BST& operator = (const std::initializer_list<T>& il);
      void swap(BST& rhs);

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

      std::pair<iterator, bool> insert(const T& t, bool keepUnique = false);
      std::pair<iterator, bool> insert(T&& t, bool keepUnique = false);

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
      BNode* root;              // root node of the binary search tree
      size_t numElements;        // number of elements currently in the tree

      // assign
      void assign(const BNode* pSrc, BNode*& pDest);
      void deleteBinaryTree(BNode*& pThis);
      void deleteNode(BNode*& pDelete, bool toRight);
   };


   /*****************************************************************
    * BINARY NODE
    * A single node in a binary tree. Note that the node does not know
    * anything about the properties of the tree so no validation can be done.
    *****************************************************************/
   template <typename T>
   class BST <T> ::BNode
   {
   public:
      //
      // Construct
      //
      BNode() : data(), pParent(nullptr), pLeft(nullptr),
         pRight(nullptr), isRed(true) { }

      BNode(const T& t) : data(t), pParent(nullptr), pLeft(nullptr),
         pRight(nullptr), isRed(true) { }

      BNode(T&& t) : data(std::move(t)), pParent(nullptr), pLeft(nullptr),
         pRight(nullptr), isRed(true) { }

      //
      // Insert
      //
      void addLeft(BNode* pNode);
      void addRight(BNode* pNode);
      void addLeft(const T& t);
      void addRight(const T& t);
      void addLeft(T&& t);
      void addRight(T&& t);

      //
      // Status
      //
      bool isRightChild(BNode* pNode) const { return pRight == pNode; }
      bool isLeftChild(BNode* pNode) const { return pLeft == pNode; }

      // balance the tree
      void balance();
      void rotateRight(BNode* pGrandparent, BNode* pAunt);
      void rotateLeft(BNode* pGrandparent, BNode* pAunt);


#ifdef DEBUG
      //
      // Verify
      //
      std::pair <T, T> verifyBTree() const;
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
   class BST <T> ::iterator
   {
      friend class ::TestBST; // give unit tests access to the privates
      friend class ::TestSet;
      friend class ::TestMap;

      template <class KK, class VV>
      friend class custom::map;
   public:
      // constructors and assignment
      iterator(BNode* p = nullptr)
      {
         pNode = p;
      }
      iterator(const iterator& rhs)
      {
         pNode = rhs.pNode;
      }
      iterator& operator = (const iterator& rhs)
      {
         pNode = rhs.pNode;
         return *this;
      }

      // compare
      bool operator == (const iterator& rhs) const
      {
         return pNode == rhs.pNode;
      }
      bool operator != (const iterator& rhs) const
      {
         return pNode != rhs.pNode;
      }

      // de-reference. Cannot change because it will invalidate the BST
      const T& operator * () const
      {
         return pNode->data;
      }

      // increment and decrement
      iterator& operator ++ ();
      iterator   operator ++ (int postfix)
      {
         iterator itReturn = *this;
         ++(*this);
         return itReturn;
      }
      iterator& operator -- ();
      iterator   operator -- (int postfix)
      {
         iterator itReturn = *this;
         --(*this);
         return itReturn;
      }

      // must give friend status to remove so it can call getNode() from it
      friend BST <T> ::iterator BST <T> ::erase(iterator& it);

   private:

      // the node
      BNode* pNode;
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
      root = nullptr;
      numElements = 0;
   }

   /*********************************************
    * BST :: COPY CONSTRUCTOR
    * Copy one tree to another
    ********************************************/
   template <typename T>
   BST <T> ::BST(const BST<T>& rhs) : root(nullptr), numElements(0)
   {
      *this = rhs;
   }

   /*********************************************
    * BST :: MOVE CONSTRUCTOR
    * Move one tree to another
    ********************************************/
   template <typename T>
   BST <T> ::BST(BST <T>&& rhs) : root(nullptr), numElements(0)
   {
      root = rhs.root;
      rhs.root = nullptr;
      numElements = rhs.numElements;
      rhs.numElements = 0;
   }

   /*********************************************
    * BST :: INITIALIZER LIST CONSTRUCTOR
    * Create a BST from an initializer list
    ********************************************/
   template <typename T>
   BST <T> ::BST(const std::initializer_list<T>& il) : root(nullptr), numElements(0)
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
   BST <T>& BST <T> :: operator = (const BST <T>& rhs)
   {
      assign(rhs.root, root);
      numElements = rhs.numElements;
      return *this;
   }

   /*********************************************
    * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
    * Copy nodes onto a BTree
    ********************************************/
   template <typename T>
   BST <T>& BST <T> :: operator = (const std::initializer_list<T>& il)
   {
      deleteBinaryTree(root);
      numElements = 0;
      for (auto&& element : il)
         insert(element);
      return *this;
   }

   /*********************************************
    * BST :: ASSIGN-MOVE OPERATOR
    * Move one tree to another
    ********************************************/
   template <typename T>
   BST <T>& BST <T> :: operator = (BST <T>&& rhs)
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
   void BST <T> ::swap(BST <T>& rhs)
   {
      std::swap(root, rhs.root);
      std::swap(numElements, rhs.numElements);
   }

   /*****************************************************
    * BST :: INSERT
    * Insert a node at a given location in the tree
    ****************************************************/
   template <typename T>
   std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(const T& t, bool keepUnique)
   {
      std::pair<iterator, bool> pairReturn(end(), false);
      try
      {
         // if we are at a trivial state (empty tree), then create a new root
         if (root == nullptr)
         {
            assert(numElements == 0);
            root = new BNode(t);
            root->isRed = false;
            numElements = 1;
            pairReturn.first = iterator(root);
            pairReturn.second = true;
            return pairReturn;
         }

         // otherwise, go search for the correct spot
         BNode* pNode = root;
         bool done = false;
         while (!done)
         {
            // if the node is a match, then do nothing
            if (keepUnique && t == pNode->data)
            {
               pairReturn.first = iterator(pNode);
               pairReturn.second = false;
               return pairReturn;
            }

            // if the center node is larger, go left
            if (t < pNode->data)
            {
               // if there is a node to the left, follow it
               if (pNode->pLeft)
                  pNode = pNode->pLeft;
               // if we are at the leaf, then create a new node
               else
               {
                  pNode->addLeft(t);
                  done = true;
                  pairReturn.first = iterator(pNode->pLeft);
                  pairReturn.second = true;
               }
            }

            // if the center node is smaller, go right
            else
            {
               // if there is a node to the right, follow it
               if (pNode->pRight)
                  pNode = pNode->pRight;
               // if we are at the left (leaf?), then create a new node
               else
               {
                  pNode->addRight(t);
                  done = true;
                  pairReturn.first = iterator(pNode->pRight);
                  pairReturn.second = true;
               }
            }
         }
         // we just inserted something!
         assert(root != nullptr);
         numElements++;

         // if the root moved out from under us, find it again
         while (root->pParent != nullptr)
            root = root->pParent;
         assert(root->pParent == nullptr);
      }
      catch (...)
      {
         throw "ERROR: Unable to allocate a node";
      }
      return pairReturn;

   }

   template <typename T>
   std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(T&& t, bool keepUnique)
   {
      std::pair<iterator, bool> pairReturn(end(), false);
      try
      {
         // if we are at a trivial state (empty tree), then create a new root
         if (root == nullptr)
         {
            assert(numElements == 0);
            root = new BNode(std::move(t));
            root->isRed = false;
            numElements = 1;
            pairReturn.first = iterator(root);
            pairReturn.second = true;
            return pairReturn;
         }

         // otherwise, go search for the correct spot
         BNode* pNode = root;
         bool done = false;
         while (!done)
         {
            // if the node is a match, then do nothing
            if (keepUnique && t == pNode->data)
            {
               pairReturn.first = iterator(pNode);
               pairReturn.second = false;
               return pairReturn;
            }

            // if the center node is larger, go left
            if (t < pNode->data)
            {
               // if there is a node to the left, follow it
               if (pNode->pLeft)
                  pNode = pNode->pLeft;
               // if we are at the leaf, then create a new node
               else
               {
                  pNode->addLeft(std::move(t));
                  done = true;
                  pairReturn.first = iterator(pNode->pLeft);
                  pairReturn.second = true;
               }
            }

            // if the center node is smaller, go right
            else
            {
               // if there is a node to the right, follow it
               if (pNode->pRight)
                  pNode = pNode->pRight;
               // if we are at the left (leaf?), then create a new node
               else
               {
                  pNode->addRight(std::move(t));
                  done = true;
                  pairReturn.first = iterator(pNode->pRight);
                  pairReturn.second = true;
               }
            }
         }
         // we just inserted something!
         assert(root != nullptr);
         numElements++;

         // if the root moved out from under us, find it again
         while (root->pParent != nullptr)
            root = root->pParent;
         assert(root->pParent == nullptr);
      }
      catch (...)
      {
         throw "ERROR: Unable to allocate a node";
      }
      return pairReturn;
   }

   /*************************************************
    * BST :: ERASE
    * Remove a given node as specified by the iterator
    ************************************************/
   template <typename T>
   typename BST<T>::iterator BST<T>::erase(iterator& it)
   {
      // do nothing if there is nothing to do
      if (it == end())
         return end();

      // remember where we were
      iterator itNext = it;
      BNode* pDelete = it.pNode;

      // if there is only one child (right) or no children
      if (pDelete->pLeft == nullptr)
      {
         ++itNext;
         deleteNode(pDelete, true /* goRight */);
      }

      // if there is only one child (left)
      else if (pDelete->pRight == nullptr)
      {
         ++itNext;
         deleteNode(pDelete, false /* goRight */);
      }

      // otherwise, swap places with the in-order successor
      else
      {
         // find the in-order successor (IOS)
         BNode* pIOS = pDelete->pRight;
         while (pIOS->pLeft != nullptr)
            pIOS = pIOS->pLeft;

         // the IOS must not have a right node. Now it will take pDelete's place
         assert(pIOS->pLeft == nullptr);
         pIOS->pLeft = pDelete->pLeft;
         if (pDelete->pLeft)
            pDelete->pLeft->pParent = pIOS;

         // if the IOS is not direct right sibling, then put it in the place of pDelete
         if (pDelete->pRight != pIOS)
         {
            // if the IOS has a right sibling, then it takes his place
            if (pIOS->pRight)
               pIOS->pRight->pParent = pIOS->pParent;
            pIOS->pParent->pLeft = pIOS->pRight;

            // make IOS's right child pDelete's right child
            assert(pDelete->pRight != nullptr);
            pIOS->pRight = pDelete->pRight;
            pDelete->pRight->pParent = pIOS;
         }

         // hook up pIOS's successor
         pIOS->pParent = pDelete->pParent;
         if (pDelete->pParent && pDelete->pParent->pLeft == pDelete)
            pDelete->pParent->pLeft = pIOS;
         if (pDelete->pParent && pDelete->pParent->pRight == pDelete)
            pDelete->pParent->pRight = pIOS;

         // what if that was the root???
         if (root == pDelete)
            root = pIOS;

         itNext = iterator(pIOS);
      }

      numElements--;
      delete pDelete;
      return itNext;
   }

   /*************************************************
   * BST :: DELETE NODE
   * Delete a specific node from the tree.
   * Called from erase()
   *    pDelete      the node to be deleted
   *    toRight      should the right branch inherit our place?
   ************************************************/
   template <typename T>
   void BST <T> :: deleteNode(BNode*& pDelete, bool toRight)
   {
      // shift everything up
      BNode* pNext = (toRight ? pDelete->pRight : pDelete->pLeft);

      // if we are not the parent, hook ourselves into the existing tree
      if (pDelete != root)
      {
         if (pDelete->pParent->pLeft == pDelete)
         {
            pDelete->pParent->pLeft = nullptr;
            pDelete->pParent->addLeft(pNext);
         }
         else
         {
            pDelete->pParent->pRight = nullptr;
            pDelete->pParent->addRight(pNext);
         }
      }

      // otherwise, the pNext is the new root
      else
      {
         root = pNext;
            pNext->pParent = nullptr;
      }
   }


   /*****************************************************
    * BST :: CLEAR
    * Removes all the BNodes from a tree
    ****************************************************/
   template <typename T>
   void BST <T> ::clear() noexcept
   {
      if (root)
         deleteBinaryTree(root);
      numElements = 0;
   }

   template <typename T>
   void BST <T> ::deleteBinaryTree(BNode*& pThis)
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
   typename BST <T> ::iterator custom::BST <T> ::begin() const noexcept
   {
      // if the BST is empty, return the nullptr iterator.
      if (root == nullptr)
         return end();

      // otherwise, find the left-most node
      BNode* p = root;
      while (p->pLeft)
         p = p->pLeft;
      return iterator(p);
   }


   /****************************************************
    * BST :: FIND
    * Return the node corresponding to a given value
    ****************************************************/
   template <typename T>
   typename BST <T> ::iterator BST<T> ::find(const T& t)
   {
      // perform a binary search using a non-recursive solution
      for (BNode* p = root; p != nullptr; p = (t < p->data ? p->pLeft : p->pRight))
         if (p->data == t)
            return iterator(p);

      // nothing was found so return the nullptr iterator
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
   void BST <T> ::BNode::addLeft(BNode* pNode)
   {
      // add the node to the left
      assert(this != nullptr);
      this->pLeft = pNode;

      // if the added node is not a nullptr, then the added node's parent is this
      if (pNode)
         pNode->pParent = this;
   }

   /******************************************************
    * BINARY NODE :: ADD RIGHT
    * Add a node to the right of the current node
    ******************************************************/
   template <typename T>
   void BST <T> ::BNode::addRight(BNode* pNode)
   {
      // add the node to the right
      assert(this != nullptr);
      this->pRight = pNode;

      // if the added node is not a nullptr, then the added node's parent is this
      if (pNode)
         pNode->pParent = this;
   }

   /******************************************************
    * BINARY NODE :: ADD LEFT
    * Add a node to the left of the current node
    ******************************************************/
   template <typename T>
   void BST<T> ::BNode::addLeft(const T& t)
   {
      assert(pLeft == nullptr);

      try
      {
         BNode* pNode = new BNode(t);
         addLeft(pNode);
         pNode->balance();
      }
      catch (...)
      {
         throw "ERROR: Unable to allocate a node";
      }
   }

   /******************************************************
    * BINARY NODE :: ADD LEFT
    * Add a node to the left of the current node
    ******************************************************/
   template <typename T>
   void BST<T> ::BNode::addLeft(T&& t)
   {
      assert(pLeft == nullptr);

      try
      {
         BNode* pNode = new BNode(std::move(t));
         addLeft(pNode);
         pNode->balance();
      }
      catch (...)
      {
         throw "ERROR: Unable to allocate a node";
      }
   }

   /******************************************************
    * BINARY NODE :: ADD RIGHT
    * Add a node to the right of the current node
    ******************************************************/
   template <typename T>
   void BST <T> ::BNode::addRight(const T& t)
   {
      assert(pRight == nullptr);

      try
      {
         BNode* pNode = new BNode(t);
         addRight(pNode);
         pNode->balance();
      }
      catch (...)
      {
         throw "ERROR: Unable to allocate a node";
      }
   }

   /******************************************************
    * BINARY NODE :: ADD RIGHT
    * Add a node to the right of the current node
    ******************************************************/
   template <typename T>
   void BST <T> ::BNode::addRight(T&& t)
   {
      assert(pRight == nullptr);

      try
      {
         BNode* pNode = new BNode(std::move(t));
         addRight(pNode);
         pNode->balance();
      }
      catch (...)
      {
         throw "ERROR: Unable to allocate a node";
      }
   }

   /******************************************************
    * BINARY NODE :: ASSIGN
    * A
    ******************************************************/
   template <typename T>
   void BST <T> ::assign(const BNode* pSrc, BNode*& pDest)
   {
      // if there is no node in pSrc, then do nothing
      if (pSrc == nullptr)
      {
         deleteBinaryTree(pDest);
         return;
      }

      assert(pSrc);
      // if there is not a node for pDest, then allocate one
      try
      {
         if (pDest == nullptr)
            pDest = new BNode(pSrc->data);  // V
         // otherwise, assign the data over
         else
            pDest->data = pSrc->data;
      }
      catch (...)
      {
         throw "ERROR: Unable to allocate a node";
      }
      assert(pDest != nullptr);

      // copy over the red-black stuff
      pDest->isRed = pSrc->isRed;

      // handle the children to the right and left

      assign(pSrc->pLeft, pDest->pLeft);  // L
      if (pSrc->pLeft)
         pDest->pLeft->pParent = pDest;

      assign(pSrc->pRight, pDest->pRight);  // R
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
   int BST <T> ::BNode::findDepth() const
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
   bool BST <T> ::BNode::verifyRedBlack(int depth) const
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
   std::pair <T, T> BST <T> ::BNode::verifyBTree() const
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
   int BST <T> ::BNode::computeSize() const
   {
      return 1 +
         (pLeft == nullptr ? 0 : pLeft->computeSize()) +
         (pRight == nullptr ? 0 : pRight->computeSize());
   }
#endif // DEBUG

   /******************************************************
    * BINARY NODE :: BALANCE
    * Balance the tree from a given location
    ******************************************************/
   template <typename T>
   void BST <T> ::BNode::balance()
   {
      // Case 1: if we are the root, then color ourselves black and call it a day.
      if (pParent == nullptr)
      {
         isRed = false;
         return;
      }
      // Case 2: if the parent is black, then there is nothing left to do
      if (pParent->isRed == false)
         return;


      // we better have a grandparent. Otherwise there is a red node at the root
      assert(pParent->pParent != nullptr);

      // find my relatives
      BNode* pGranny = pParent->pParent;
      BNode* pGreatG = pGranny->pParent;
      BNode* pSibling = pParent->isRightChild(this) ? pParent->pLeft : pParent->pRight;
      BNode* pAunt = pGranny->isRightChild(pParent) ? pGranny->pLeft : pGranny->pRight;

      // verify things are as they should be
      assert(pGranny != nullptr);      // I should have a grandparent here
      assert(pGranny->isRed == false); // if granny is red, we violate red-red!

      // Case 3: if the aunt is red, then just recolor
      if (pAunt != nullptr && pAunt->isRed == true)
      {
         pGranny->isRed = true;  // grandparent becomes red
         pParent->isRed = false; // parent becomes black
         pAunt->isRed = false;   // aunt becomes black
         pGranny->balance();     // balance granny!
         return;
      }

      // Case 4: if the aunt is black or non-existant, then we need to rotate
      assert(pParent->isRed == true && pGranny->isRed == false && (pAunt == nullptr || pAunt->isRed == false));

      // the new top of the sub-tree
      BNode* pHead = nullptr;

      // Case 4a: We are mom's left and mom is granny's left
      if (pParent->isLeftChild(this) && pGranny->isLeftChild(pParent))
      {
         // verify case 4a is as it should be
         assert(pParent->pLeft == this);
         assert(pGranny->pRight == pAunt);
         assert(pGranny->isRed == false);

         // perform the necessary rotation
         pParent->addRight(pGranny);
         pGranny->addLeft(pSibling);
         pHead = pParent;

         // set the colors
         pParent->isRed = false;
         pGranny->isRed = true;
      }

      // Case 4b: We are mom's right and mom is granny's right
      else if (pParent->isRightChild(this) && pGranny->isRightChild(pParent))
      {
         // verify case 4b is as it should be
         assert(pParent->pRight == this);
         assert(pGranny->pLeft == pAunt);
         assert(pGranny->isRed == false);

         // perform the necessary rotation
         pParent->addLeft(pGranny);
         pGranny->addRight(pSibling);
         pHead = pParent;

         // set the colors
         pParent->isRed = false;
         pGranny->isRed = true;
      }

      // Case 4c: We are mom's right and mom is granny's left
      else if (pParent->isRightChild(this) && pGranny->isLeftChild(pParent))
      {
         // verify case 4c is as it should be
         assert(pGranny->pRight == pAunt);
         assert(pParent->pLeft == pSibling);
         assert(pParent->isRed == true);

         // perform the necessary rotation
         pGranny->addLeft(this->pRight);
         pParent->addRight(this->pLeft);
         addRight(pGranny);
         addLeft(pParent);
         pHead = this;

         // set the colors
         this->isRed = false;
         pGranny->isRed = true;
      }

      // case 4d: we are mom's left and mom is granny's right
      else if (pParent->isLeftChild(this) && pGranny->isRightChild(pParent))
      {
         // verify case 4d is as it should be
         assert(pGranny->pLeft == pAunt);
         assert(pGranny->pRight == pParent);
         assert(pParent->pRight == pSibling);

         // perform the necessary rotation
         pGranny->addRight(this->pLeft);
         pParent->addLeft(this->pRight);
         addLeft(pGranny);
         addRight(pParent);
         pHead = this;

         // set the colors
         this->isRed = false;
         pGranny->isRed = true;
      }

      // else we are really confused!
      else
      {
         assert(false); // !!
      }

      // fix up great granny if she is not nullptr
      if (pGreatG == nullptr)
         pHead->pParent = nullptr;
      else if (pGreatG->pRight == pGranny)
         pGreatG->addRight(pHead);
      else if (pGreatG->pLeft == pGranny)
         pGreatG->addLeft(pHead);
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
   typename BST <T> ::iterator& BST <T> ::iterator :: operator ++ ()
   {
      // do nothing if we have nothing
      if (nullptr == pNode)
         return *this;

      // if there is a right node, take it
      if (nullptr != pNode->pRight)
      {
         // go right
         pNode = pNode->pRight;

         // jig left - there might be more left-most children
         while (pNode->pLeft)
            pNode = pNode->pLeft;
         return *this;
      }

      // there are no right children, the left are done
      assert(pNode->pRight == nullptr);
      const BNode* pSave = pNode;

      // go up...
      pNode = pNode->pParent;

      // if the parent is the nullptr, we are done!
      if (pNode == nullptr)
         return *this;

      // if we are the left-child, go to the parent.
      if (pSave == pNode->pLeft)
         return *this;

      // we are the right-child, go up as long as we are the right child!
      while (pNode != nullptr && pSave == pNode->pRight)
      {
         pSave = pNode;
         pNode = pNode->pParent;
      }

      return *this;
   }

   /**************************************************
    * BST ITERATOR :: DECREMENT PREFIX
    * advance by one
    *************************************************/
   template <typename T>
   typename BST <T> ::iterator& BST <T> ::iterator :: operator -- ()
   {
      // do nothing if we have nothing
      if (nullptr == pNode)
         return *this;

      // if there is a left node, take it
      if (nullptr != pNode->pLeft)
      {
         // go left
         pNode = pNode->pLeft;

         // jig right - there might be more right-most children
         while (pNode->pRight)
            pNode = pNode->pRight;
         return *this;
      }

      // there are no left children, the right are done
      assert(pNode->pLeft == nullptr);
      const BNode* pSave = pNode;

      // go up...
      pNode = pNode->pParent;

      // if the parent is the nullptr, we are done!
      if (pNode == nullptr)
         return *this;

      // if we are the right-child, go to the parent.
      if (pSave == pNode->pRight)
         return *this;

      // we are the left-child, go up as long as we are the left child!
      while (pNode != nullptr && pSave == pNode->pLeft)
      {
         pSave = pNode;
         pNode = pNode->pParent;
      }

      return *this;
   }


} // namespace custom


