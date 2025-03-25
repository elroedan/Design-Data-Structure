/***********************************************************************
 * Header:
 *    DEQUE
 * Summary:
 *    Our custom implementation of a deque 
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        deque                 : A class that represents a deque
 *        deque::iterator       : An iterator through a deque
 * Author
 *    Jacob Mower
 *    Thatcher Albinston
 *    Elroe Woldemichael
 ************************************************************************/

#pragma once

// Debug stuff
#include <cassert>
#include <memory>   // for std::allocator

class TestDeque;    // forward declaration for TestDeque unit test class

namespace custom
{

/******************************************************
 * DEQUE
 *****************************************************/
template <typename T, typename A = std::allocator<T>>
class deque
{
   friend class ::TestDeque; // give unit tests access to the privates
public:

   // 
   // Construct
   //
   deque(const A& a = A())
   { 
      data = nullptr;
      numElements = 0;
      numBlocks = 0; 
      numCells = 16; 
   }
   deque(deque & rhs);
   ~deque()
   {
   }

   //
   // Assign
   //
   deque & operator = (deque& rhs);

   // 
   // Iterator
   //
   class iterator;
   iterator begin() 
   { 
      return iterator(); 
   }
   iterator end()   
   { 
      return iterator(); 
   }

   // 
   // Access
   //
   T & front()       
   { 
      return *(new T);
   }
   const T & front() const 
   {
      return *(new T);
   }
   T & back()
   {
      return *(new T);
   }
   const T & back() const
   {
      return *(new T);
   }
   T & operator[](int id)
   {
      assert(0 <= id < numElements);
      assert(nullptr != data[ibFromID(id)]);
      return data[ibFromID(id)][icFromID(id)];
   }
   const T & operator[](int id) const
   {
      assert(0 <= id < numElements);
      assert(nullptr != data[ibFromID(id)]);
      return data[ibFromID(id)][icFromID(id)];
   }

   //
   // Insert
   //
   void push_back(const T & t);
   void push_back(T && t);
   void push_front(const T& t);
   void push_front(T&& t);

   //
   // Remove
   //
   void pop_front();
   void pop_back();
   void clear();

   //
   // Status
   //
   size_t size()  const { return numElements; }
   bool   empty() const { return numElements == 0; }
   
private:
   // array index from deque index
   int iaFromID(int id) const
   {
      assert(0 <= id < numElements);
      assert(0 <= iaFront < numCells * numBlocks);
      int ia = (id + iaFront) % numCells * numBlocks; 
      assert(0 <= ia < numCells * numBlocks);
      return ia;
      //return 0;
   }

   // block index from deque index
   int ibFromID(int id) const
   {
      int ib = iaFromID(id) / numCells;
      assert(0 <= ib < numBlocks);
      return ib;
   }

   // cell index from deque index
   int icFromID(int id) const
   {
      int ic = iaFromID(id) % numCells;
      assert(0 <= ic < numCells);
      return ic;
   }

   // reallocate
   void reallocate(int numBlocksNew);

   A    alloc;                // use alloacator for memory allocation
   size_t numCells;           // number of cells in a block
   size_t numBlocks;          // number of blocks in the data array
   size_t numElements;        // number of elements in the deque
   int iaFront;               // array-centered index of the front of the deque
   T ** data;                 // array of arrays
};

/**************************************************
 * DEQUE ITERATOR
 * An iterator through deque.  You only need to
 * support the following:
 *   1. Constructors (default and copy)
 *   2. Not equals operator
 *   3. Increment (prefix and postfix)
 *   4. Dereference
 * This particular iterator is a bi-directional meaning
 * that ++ and -- both work.  Not all iterators are that way.
 *************************************************/
template <typename T, typename A>
class deque <T, A> ::iterator
{
   friend class ::TestDeque; // give unit tests access to the privates
public:
   // 
   // Construct
   //
   iterator() : d(nullptr), id(0)
   {
   }
   iterator(int id, deque* d) : d(d), id(id)
   {
   }
   iterator(const iterator& rhs) : d(rhs.d), id(rhs.id)
   { 
   }

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
      this = rhs.d;
      this = rhs.id;
      return *this;
   }

   // 
   // Compare
   //
   bool operator != (const iterator& rhs) const { return this->d != rhs.d && this->id != rhs.id; }
   bool operator == (const iterator& rhs) const { return this->d == rhs.d && this->id == rhs.id; }

   // 
   // Access
   //
   T& operator * ()
   {
      return d[id];
   }

   // 
   // Arithmetic
   //
   int operator - (iterator it) const
   {
      return 99;
   }
   iterator& operator += (int offset)
   {
      return *this;
   }
   iterator& operator ++ ()
   {
      return *this;
   }
   iterator operator ++ (int postfix)
   {
      return *this;
   }
   iterator& operator -- ()
   {
      return *this;
   }
   iterator operator -- (int postfix)
   {
      return *this;
   }

private:
   int id;
   deque* d;
};

/*****************************************
 * DEQUE :: COPY CONSTRUCTOR
 * Allocate the space for the elements and
 * call the copy constructor on each element
 ****************************************/
template <typename T, typename A>
deque <T, A> ::deque(deque& rhs) 
{
   this = rhs;
}

/*****************************************
 * DEQUE :: COPY-ASSIGN
 * Allocate the space for the elements and
 * call the copy constructor on each element
 ****************************************/
template <typename T, typename A>
deque <T, A> & deque <T, A> :: operator = (deque & rhs)
{
   return *this;
}

/*****************************************
 * DEQUE :: PUSH_BACK
 * add an element to the back of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_back(const T& t)
{
}

/*****************************************
 * DEQUE :: PUSH_BACK - move
 * add an element to the back of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_back(T && t)
{
}

/*****************************************
 * DEQUE :: PUSH_FRONT
 * add an element to the front of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_front(const T& t)
{
}

/*****************************************
 * DEQUE :: PUSH_FRONT - move
 * add an element to the front of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_front(T&& t)
{
}

/*****************************************
 * DEQUE :: CLEAR
 * Remove all the elements from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::clear()
{
}

/*****************************************
 * DEQUE :: POP FRONT
 * Remove the front element from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> :: pop_front()
{
}

/*****************************************
 * DEQUE :: POP BACK
 * Remove the back element from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::pop_back()
{
   
}

/*****************************************
 * DEQUE :: REALLOCATE
 * Remove all the elements from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> :: reallocate(int numBlocksNew)
{
   assert(numBlocksNew > 0 && numBlocksNew > numElements);

   T** dataNew = new T * [numBlocksNew];
   for (int id = 0; id < numElements; id++)
   {
      dataNew[id] = std::move(this->data[id]);
   }
   //numCapacity = numBlocksNew;
   iaFront = 0;
   delete data;
   data = dataNew;
}




} // namespace custom
