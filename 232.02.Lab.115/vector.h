/***********************************************************************
 * Header:
 *    VECTOR
 * Summary:
 *    Our custom implementation of std::vector
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        vector                 : A class that represents a Vector
 *        vector::iterator       : An iterator through Vector
 * Author
 *    Jacob Mower
 *    Elroe Woldemichael
 *    Thatcher Albiston
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
#include <new>      // std::bad_alloc
#include <memory>   // for std::allocator

class TestVector; // forward declaration for unit tests
class TestStack;
class TestPQueue;
class TestHash;

namespace custom
{

/*****************************************
 * VECTOR
 * Just like the std :: vector <T> class
 ****************************************/
template <typename T, typename A = std::allocator<T>>
class vector
{
   friend class ::TestVector; // give unit tests access to the privates
   friend class ::TestStack;
   friend class ::TestPQueue;
   friend class ::TestHash;
public:
   
   //
   // Construct
   //
   vector(const A& a = A());
   vector(size_t numElements,                const A & a = A());
   vector(size_t numElements, const T & t,   const A & a = A());
   vector(const std::initializer_list<T>& l, const A & a = A());
   vector(const vector &  rhs);
   vector(      vector && rhs);
  ~vector();

   //
   // Assign
   //
   void swap(vector& rhs)
   {
      std::swap(data, rhs.data);
      std::swap(numElements, rhs.numElements);
      std::swap(numCapacity, rhs.numCapacity);
   }
   vector & operator = (const vector & rhs);
   vector & operator = (vector&& rhs);

   //
   // Iterator
   //
   class iterator;
   iterator begin() 
   { 
      return iterator(data);
   }
   iterator end() 
   { 
      return iterator(data + numElements);
   }

   //
   // Access
   //
         T& operator [] (size_t index);
   const T& operator [] (size_t index) const;
         T& front();
   const T& front() const;
         T& back();
   const T& back() const;

   //
   // Insert
   //
   void push_back(const T& t);
   void push_back(T&& t);
   void reserve(size_t newCapacity);
   void resize(size_t newElements);
   void resize(size_t newElements, const T& t);

   //
   // Remove
   //
   void clear()
   {
      //Remove all the elements and reset the whole thing
      for (size_t i = 0; i < numElements; ++i)
         alloc.destroy(data + i);
      numElements = 0;
   }
   
   void pop_back()
   {
      //If it's not empty then remove the last element
      if (!empty())
      {
         alloc.destroy(data + (numElements - 1));
         numElements -= 1;
      }
   }
   void shrink_to_fit();

   //
   // Status
   //
   size_t  size()          const { return numElements;}
   size_t  capacity()      const { return numCapacity;}
   bool empty()            const { return numElements == 0;}
  
private:
   
   A    alloc;                // use allocator for memory allocation
   T *  data;                 // user data, a dynamically-allocated array
   size_t  numCapacity;       // the capacity of the array
   size_t  numElements;       // the number of items currently used
};

/**************************************************
 * VECTOR ITERATOR
 * An iterator through vector.  You only need to
 * support the following:
 *   1. Constructors (default and copy)
 *   2. Not equals operator
 *   3. Increment (prefix and postfix)
 *   4. Dereference
 * This particular iterator is a bi-directional meaning
 * that ++ and -- both work.  Not all iterators are that way.
 *************************************************/
template <typename T, typename A>
class vector <T, A> ::iterator
{
   friend class ::TestVector; // give unit tests access to the privates
   friend class ::TestStack;
   friend class ::TestPQueue;
   friend class ::TestHash;
public:
   // constructors, destructors, and assignment operator
   iterator()                           : p(nullptr)        {  }
   iterator(T* p)                       : p(p)              {  }
   iterator(const iterator& rhs)        : p(rhs.p)          {  }
   iterator(size_t index, vector<T>& v) : p(v.data + index) {  }
   iterator& operator = (const iterator& rhs)
   {
      this->p = rhs.p;
      return *this;
   }

   // equals, not equals operator
   bool operator != (const iterator& rhs) const { return this->p != rhs.p; }
   bool operator == (const iterator& rhs) const { return this->p == rhs.p; }

   // dereference operator
   T& operator * ()
   {
      return *(p);
   }

   // prefix increment
   iterator& operator ++ ()
   {
      this->p += 1;
      return *this;
   }

   // postfix increment
   iterator operator ++ (int postfix)
   {
      iterator it = *this;
      this->p += 1;
      return it;
   }

   // prefix decrement
   iterator& operator -- ()
   {
      this->p -= 1;
      return *this;
   }

   // postfix decrement
   iterator operator -- (int postfix)
   {
      iterator it = *this;
      this->p -= 1;
      return it;
   }

private:
   T* p;
};


/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector(const A & a) : alloc(a), numCapacity(0), numElements(0)
{
   data = nullptr;
}


/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector(size_t num, const T & t, const A & a): alloc(a), numCapacity(num), numElements(num)
{
   // Allocate memory using the allocator
   data = alloc.allocate(num);

   // Use the allocator to construct each element with the value `t`
   for (size_t i = 0; i < num; ++i)
   {
       alloc.construct(data + i, t);
   }
}

/*****************************************
 * VECTOR :: INITIALIZATION LIST constructors
 * Create a vector with an initialization list.
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector(const std::initializer_list<T> & l, const A & a) : alloc(a), numCapacity(l.size()), numElements(l.size())
{
   data = alloc.allocate(l.size());
   auto it = l.begin();
   for (size_t i = 0; i < l.size(); ++i, ++it)
   {
      new (data + i) T(*it); 
   }
}

/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector(size_t num, const A & a) : alloc(a), numCapacity(num), numElements(num)
{
   if (num > 0)
      data = new T[num];
   else
      data = nullptr;
   //data = alloc.allocate(num);
}

/*****************************************
 * VECTOR :: COPY CONSTRUCTOR
 * Allocate the space for numElements and
 * call the copy constructor on each element
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector (const vector & rhs) : alloc(rhs.alloc), numCapacity(rhs.numElements), numElements(rhs.numElements)
{
   if (!rhs.empty())
   {
      data = alloc.allocate(rhs.numElements);
      for (int i = 0; i < numElements; i++)
      {
         alloc.construct(data + i, rhs.data[i]);
      }
   }
   else 
   {
      data = nullptr;
      numElements = 0;
      numCapacity = 0;
   }
}
   
/*****************************************
 * VECTOR :: MOVE CONSTRUCTOR
 * Steal the values from the RHS and set it to zero.
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector (vector && rhs) : numCapacity(rhs.numCapacity), numElements(rhs.numElements), data(rhs.data)
{
   rhs.data = nullptr;
   rhs.numCapacity = 0;
   rhs.numElements = 0;
}

/*****************************************
 * VECTOR :: DESTRUCTOR
 * Call the destructor for each element from 0..numElements
 * and then free the memory
 ****************************************/
template <typename T, typename A>
vector <T, A> :: ~vector()
{
   //Loop through elements and destroy them each
   clear();
   if (numCapacity)
   {
      //If the vector has memory deallocate the memory that the vector had.
      alloc.deallocate(data, numCapacity);
   }
}

/***************************************
 * VECTOR :: RESIZE
 * This method will adjust the size to newElements.
 * This will either grow or shrink newElements.
 *     INPUT  : newCapacity the size of the new buffer
 *     OUTPUT :
 **************************************/
template <typename T, typename A>
void vector <T, A> :: resize(size_t newElements)//HELPPPP US IT"S NOT EFFICIENT
{
   //Sesired size is smaller than current so destroy thos old slots
   if (newElements < numElements)
   {
      for (size_t i = newElements; i < numElements; i++)
         alloc.destroy(data + i);
   }
   
   //New size/position is made
   else if (newElements > numElements)
   {
      if (newElements > numCapacity)
         reserve(newElements);
      for (size_t i = numElements; i < newElements; i++)
         new (&data[i]) T;
   }
   
   //Only change the number of elements if needed
   if (numElements != newElements)
   {
      numElements = newElements;
   }
}

template <typename T, typename A>
void vector <T, A> :: resize(size_t newElements, const T & t)
{
   //Sesired size is smaller than current so destroy thos old slots
   if (newElements < numElements)
      for (size_t i = newElements; i < numElements; i++)
         alloc.destroy(data + i);
   
   //New size is made and any extra spots are filled with t
   else if (newElements > numElements)
   {
      if (newElements > numCapacity)
         reserve(newElements);
      for (size_t i = numElements; i < newElements; i++)
         new (&data[i]) T(t);
   }
   numElements = newElements;
}

/***************************************
 * VECTOR :: RESERVE
 * This method will grow the current buffer
 * to newCapacity.  It will also copy all
 * the data from the old buffer into the new
 *     INPUT  : newCapacity the size of the new buffer
 *     OUTPUT :
 **************************************/
template <typename T, typename A>
void vector <T, A> :: reserve(size_t newCapacity)
{
   //Already have enough space
   if (newCapacity <= numCapacity)
      return;
   
   //Make new space as needed and move elements
   T * dataNew = alloc.allocate(newCapacity);
   for (auto i = 0; i < numElements; i++) 
      new ((void*)(dataNew + i)) T(std::move(data[i]));
   
   //Destroy old elements after move is complete
   for (size_t i = 0; i < numElements; ++i)
      alloc.destroy(data + i);
   alloc.deallocate(data, numCapacity);
   data = dataNew;
   numCapacity = newCapacity;
}

/***************************************
 * VECTOR :: SHRINK TO FIT
 * Get rid of any extra capacity
 *     INPUT  :
 *     OUTPUT :
 **************************************/
template <typename T, typename A>
void vector <T, A> :: shrink_to_fit()
{
   if (numElements != 0)
   {
      //Vector is already correct size
      if (numElements == numCapacity)
         return;

      //Allocate new spot for smaller vector
      T * dataNew = alloc.allocate(numElements);

      //Create our vector in the new spot
      for (size_t i = 0; i < numElements; i++)
         alloc.construct(&dataNew[i], data[i]);

      //Destroy the vector in the old spot
      for (size_t i = 0; i < numElements; ++i)
         alloc.destroy(&data[i]);
      alloc.deallocate(data, numCapacity);

      //Update members
      data = dataNew;
      numCapacity = numElements;
   }
   else
   {
      //There are no elements, but we need to free the memory
      alloc.deallocate(data, numCapacity);
      data = nullptr;
      numCapacity = 0;
   }

}



/*****************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 ****************************************/
template <typename T, typename A>
T & vector <T, A> :: operator [] (size_t index)
{
   return data[index];
    
}

/******************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 *****************************************/
template <typename T, typename A>
const T & vector <T, A> :: operator [] (size_t index) const
{
   return data[index];
}

/*****************************************
 * VECTOR :: FRONT
 * Read-Write access
 ****************************************/
template <typename T, typename A>
T & vector <T, A> :: front ()
{
   return data[0];
}

/******************************************
 * VECTOR :: FRONT
 * Read-Write access
 *****************************************/
template <typename T, typename A>
const T & vector <T, A> :: front () const
{
   return data[0];
}

/*****************************************
 * VECTOR :: FRONT
 * Read-Write access
 ****************************************/
template <typename T, typename A>
T & vector <T, A> :: back()
{
   return data[numElements - 1];
}

/******************************************
 * VECTOR :: FRONT
 * Read-Write access
 *****************************************/
template <typename T, typename A>
const T & vector <T, A> :: back() const
{
   return data[numElements - 1];
}

/***************************************
 * VECTOR :: PUSH BACK
 * This method will add the element 't' to the
 * end of the current buffer.  It will also grow
 * the buffer as needed to accomodate the new element
 *     INPUT  : 't' the new element to be added
 *     OUTPUT : *this
 **************************************/
template <typename T, typename A>
void vector <T, A> :: push_back (const T & t)
{
   //Our vector was empty so make a single spot
   if (capacity() == 0)
      reserve(1);
   //Double the size if needed and put our element in
   if (size() == capacity())
      reserve(capacity() * 2);
   new ((void*)(&data[numElements++])) T(t);
}

template <typename T, typename A>
void vector <T, A> ::push_back(T && t)
{
   //Our vector was empty so make a single spot
   if (capacity() == 0)
      reserve(1);
   //Double the size if needed and put our element in
   if (size() == capacity())
      reserve(capacity() * 2);
   new ((void*)(&data[numElements++])) T(std::move(t));
}

/***************************************
 * VECTOR :: ASSIGNMENT
 * This operator will copy the contents of the
 * rhs onto *this, growing the buffer as needed
 *     INPUT  : rhs the vector to copy from
 *     OUTPUT : *this
 **************************************/
template <typename T, typename A>
vector <T, A> & vector <T, A> :: operator = (const vector & rhs)
{
   //Vectors are the same size, just move data
   if (rhs.size() == size())
      for (size_t i = 0; i < size(); i++)
         data[i] = rhs.data[i];
   
   //Our Source is bigger than the destination but our dest has room
   else if (rhs.size() > size())
   {
      if (rhs.size() <= capacity())
      {
         for (size_t i = 0; i < size(); i++)
            data[i] = rhs.data[i];
         for (size_t i = size(); i < rhs.size(); i++)
            alloc.construct(data + i, rhs.data[i]);
      }
      
      //Our source is bigger than dest so adjust the dest
      else
      {
         T * dataNew = alloc.allocate(rhs.size());
         for (size_t i = 0; i < rhs.size(); i++)
            alloc.construct(dataNew + i, rhs.data[i]);
         clear();
         alloc.deallocate(data, numCapacity);
         data = dataNew;
         numCapacity = rhs.size();
      }
   }
   
   //The dest is bigger so move the data and take down the
   else
   {
      for (size_t i = 0; i < rhs.size(); i++)
         data[i] = rhs.data[i];
      //Destroy the extra elements left over in dest
      for (size_t i = rhs.size(); i < size(); i++)
         alloc.destroy(data + i);
   }

   numElements = rhs.size();
   return *this;
}
template <typename T, typename A>
vector <T, A>& vector <T, A> :: operator = (vector&& rhs)//change the 
{
   //If they are the same vector, do nothing
   if (this == &rhs)
      return *this;
   
   //Set our current/lhs vector to be default
   this->clear();
   this->data = nullptr;
   this->numElements = 0;
   this->numCapacity = 0;
   
   //Swap the vectors
   this->swap(rhs);
   return *this;
}




} // namespace custom

