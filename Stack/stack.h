/***********************************************************************
 * Module:
 *    Stack
 * Summary:
 *    Our custom implementation of std::stack
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *       stack             : similar to std::stack
 * Author
 *    Jacob Mower
 *    Elroe
 *    Thatcher
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
#include "vector.h"

class TestStack; // forward declaration for unit tests

namespace custom
{

/**************************************************
 * STACK
 * First-in-Last-out data structure
 *************************************************/
template<class T, class Container = custom::vector<T>>
class stack
{
   friend class ::TestStack; // give unit tests access to the privates
public:
  
   //
   // Construct
   //
   
   stack() : container()         {  }
   stack(const stack <T> &  rhs) : container(rhs.container) { }
   stack(stack <T>&& rhs)        : container(std::move(rhs.container)) { }
   stack(const Container& rhs) :container(rhs){  }
   stack(Container&& rhs) :container(std::move(rhs)){  }
   ~stack()                      {                      }
   
   //
   // Assign
   //
   stack <T> & operator = (const stack <T> & rhs)
   {
      return *this;
   }
   stack <T>& operator = (stack <T> && rhs)
   {
      return *this;
   }
   void swap(stack <T>& rhs)
   {

   }

   //
   // Access
   //
   
   T & top()       
   { 
      return *(new T); 
   }
   const T & top() const 
   { 
      return *(new T); 
   }

   //
   // Insert
   //
   
   void push(const T &  t) 
   {  
      this->container.push_back(t);
   }
   void push(      T && t) 
   {  
   
   }

   //
   // Remove
   //
   
   void pop() 
   {  
   
   }

   //
   // Status
   //
   
   size_t size () const { return container.size();   }
   bool   empty() const { return container.empty(); } // 
   
private:
   
   Container container;  // underlying container (probably a vector)
};



} // custom namespace


