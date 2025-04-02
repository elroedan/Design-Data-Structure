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
      iaFront = 0;
   }
   deque(deque & rhs);
   ~deque()
   {
      clear();
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
      return iterator(0, this);
   }
   iterator end()
   {
      return iterator(numElements, this);
   }

   // 
   // Access
   //
   T & front()       
   { 
      return data[ibFromID(0)][icFromID(0)];
   }
   const T & front() const 
   {
      return data[ibFromID(0)][icFromID(0)];
   }
   T & back()
   {
      return data[ibFromID(numElements - 1)][icFromID(numElements - 1)];
   }
   const T & back() const
   {
      return data[ibFromID(numElements - 1)][icFromID(numElements - 1)];
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
      assert(id >= 0);
      //if (numElements > 0)
         //assert(id < numElements);
      assert(0 <= iaFront && iaFront < (numCells * numBlocks));
      int ia = (id + iaFront) % (numCells * numBlocks);
      assert(0 <= ia && ia < (numCells * numBlocks));
      return ia;
   }

   // block index from deque index
   int ibFromID(int id) const
   {
      int ib = iaFromID(id) / numCells;
      assert(0 <= ib && ib < numBlocks);
      return ib;
   }

   // cell index from deque index
   int icFromID(int id) const
   {
      int ic = iaFromID(id) % numCells;
      assert(0 <= ic && ic < numCells);
      return ic;
   }
   
   //Check if all the blocks are filled
   bool isAllBlocksFilled() const
   {
      // We have no choice but to check each block looking for a NULLPTR
      for (size_t ib = 0; ib < numBlocks; ib++)
         if (nullptr == data[ib])
            return false;
      return true;
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
      this->d = rhs.d;
      this->id = rhs.id;
      return *this;
   }

   // 
   // Compare
   //
   bool operator != (const iterator& rhs) const { return this->d == rhs.d && this->id != rhs.id; }
   bool operator == (const iterator& rhs) const { return this->d == rhs.d && this->id == rhs.id; }

   // 
   // Access
   //
   T& operator * ()
   {
      return (*d)[id];
   }

   // 
   // Arithmetic
   //
   int operator - (iterator it) const
   {
      return id - it.id;
   }
   iterator& operator += (int offset)
   {
      id += offset;
      return *this;
   }
   iterator& operator ++ ()
   {
      ++id;
      return *this;
   }
   iterator operator ++ (int postfix)
   {
      iterator temp = *this;
      ++id;
      return temp;
   }
   iterator& operator -- ()
   {
      --id;
      return *this;
   }
   iterator operator -- (int postfix)
   {
      iterator temp = *this;
      --id;
      return temp;
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
deque <T, A> ::deque(deque& rhs) : data(nullptr), numElements(0),
numCells(16), numBlocks(0), iaFront(0)
{
   *this = rhs;
}

/*****************************************
 * DEQUE :: COPY-ASSIGN
 * Allocate the space for the elements and
 * call the copy constructor on each element
 ****************************************/
template <typename T, typename A>
deque <T, A> & deque <T, A> :: operator = (deque & rhs)
{
   iterator itLHS = begin();
   iterator itRHS = rhs.begin();
   iterator itLHSEnd = end();
   
   while (itRHS != rhs.end() && itLHS != end())
   {
      *itLHS = *itRHS;
      itLHS++;
      itRHS++;
   }
   
   while (itLHS != itLHSEnd)
   {
      pop_back();
      itLHS++;
   }
   
   while (itRHS != rhs.end())
   {
      push_back(*itRHS);
      itRHS++;
   }
   return *this;
}

/*****************************************
 * DEQUE :: PUSH_BACK
 * add an element to the back of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_back(const T& t)
{
   //Reallocate the array of blocks as needed
   int icTail = (numElements == 0) ? numCells - 1 : icFromID(numElements - 1);
   if (isAllBlocksFilled() && icTail == numCells - 1)
      reallocate(numBlocks == 0 ? 1 : numBlocks * 2);
  
   //Allocate a new block as needed
   int ib = ibFromID(numElements);
   if (data[ib] == nullptr)
      data[ib] = alloc.allocate(numCells);

   //Assign the value into the block
   new((void*)(&(data[ib][icFromID(numElements)]))) T(t);
   numElements++;
}

/*****************************************
 * DEQUE :: PUSH_BACK - move
 * add an element to the back of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_back(T && t)
{
   //Reallocate the array of blocks as needed
   int icTail = (numElements == 0) ? numCells - 1 : icFromID(numElements - 1);
   if (isAllBlocksFilled() && icTail == numCells - 1)
      reallocate(numBlocks == 0 ? 1 : numBlocks * 2);
  
   //Allocate a new block as needed
   int ib = ibFromID(numElements);
   if (data[ib] == nullptr)
      data[ib] = alloc.allocate(numCells);

   //Assign the value into the block
   new((void*)(&(data[ib][icFromID(numElements)]))) T(std::move(t));
   numElements++;
}

/*****************************************
 * DEQUE :: PUSH_FRONT
 * add an element to the front of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_front(const T& t)
{
   //Reallocate the array of blocks as needed
   int icHead = (numElements == 0) ? 0 : icFromID(0);
   if (isAllBlocksFilled() && icHead == 0)
      reallocate(numBlocks == 0 ? 1 : numBlocks * 2);
   
   if (iaFront != 0)
      iaFront --;
   else
      iaFront = numBlocks * numCells - 1;
   
   //Allocate a new block as needed
   int ib = ibFromID(0);
   if (data[ib] == nullptr)
      data[ib] = alloc.allocate(numCells);
   
   //Assign the value into the block
   new((void*)(&(data[ib][icFromID(0)]))) T(t);
   numElements++;
}

/*****************************************
 * DEQUE :: PUSH_FRONT - move
 * add an element to the front of the deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::push_front(T&& t)
{
   
   //Reallocate the array of blocks as needed
   int icHead = (numElements == 0) ? 0 : icFromID(0);
   if (isAllBlocksFilled() && icHead == 0)
      reallocate(numBlocks == 0 ? 1 : numBlocks * 2);
   
   if (iaFront != 0)
      iaFront --;
   else
      iaFront = numBlocks * numCells - 1;
   
   //Allocate a new block as needed
   int ib = ibFromID(0);
   if (data[ib] == nullptr)
      data[ib] = alloc.allocate(numCells);
   
   //Assign the value into the block
   new((void*)(&(data[ib][icFromID(0)]))) T(std::move(t));
   numElements++;
}

/*****************************************
 * DEQUE :: CLEAR
 * Remove all the elements from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::clear()
{
   for (int id = 0; id < numElements; id++)
      alloc.destroy(&data[ibFromID(id)][icFromID(id)]);
   
   for (int ib = 0; ib < numBlocks; ib++) {
      if (data[ib] != nullptr)
      {
         alloc.deallocate(data[ib], numCells);
         data[ib] = nullptr;
      }
   }
   numElements = 0;
}

/*****************************************
 * DEQUE :: POP FRONT
 * Remove the front element from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> :: pop_front()
{
   int idRemove = 0;
   int ibRemove = ibFromID(idRemove);
   alloc.destroy(&data[ibRemove][icFromID(idRemove)]);
   iaFront++;
   if (iaFront == (numCells * numBlocks))
      iaFront = 0;
   
   if (numElements == 1 ||
       (icFromID(iaFront) == 0 && ibRemove != ibFromID(iaFront)))
   {
      alloc.deallocate(data[ibRemove],numCells);
      data[ibRemove] = nullptr;
   }
   numElements--;
}

/*****************************************
 * DEQUE :: POP BACK
 * Remove the back element from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> ::pop_back()
{
   int idRemove = numElements - 1;
   alloc.destroy(&data[ibFromID(idRemove)][icFromID(idRemove)]);
   if (numElements == 1 || 
       (icFromID(idRemove) == 0 && ibFromID(idRemove) != ibFromID(0)))
   {
      alloc.deallocate(data[ibFromID(idRemove)],numCells);
      data[ibFromID(idRemove)] = nullptr;
   }
   numElements--;
}

/*****************************************
 * DEQUE :: REALLOCATE
 * Remove all the elements from a deque
 ****************************************/
template <typename T, typename A>
void deque <T, A> :: reallocate(int numBlocksNew)
{
   T** dataNew = new T*[numBlocksNew];
   int ibNew = 0;
   for (int idOld = 0; idOld < numElements; idOld += numCells)
   {
      dataNew[ibNew] = data[ibFromID(idOld)];
      ibNew++;
   }
   
   while (ibNew < numBlocksNew)
   {
      dataNew[ibNew] = nullptr;
      ibNew++;
   }
   
   if (numElements > 0 && ibFromID(0) == ibFromID(numElements - 1) &&
       icFromID(0) > icFromID(numElements - 1))
   {
      int ibFrontOld = ibFromID(0);
      int ibBackOld = ibFromID(numElements - 1);
      int ibBackNew = numElements / numCells;
      dataNew[ibBackNew] = alloc.allocate(numCells);
      
      for (int ic = 0; ic < icFromID(numElements - 1); ic++) 
      {
         new((void*)(&(dataNew[ibBackNew][ic]))
             ) T(std::move(data[ibBackOld][ic]));
      }
   }
   
   if (data)
      delete data;
   
   data = dataNew;
   numBlocks = numBlocksNew;
   iaFront = iaFront % numCells;
   
}




} // namespace custom
