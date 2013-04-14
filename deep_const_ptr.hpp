/*
 * Copyright (c) 2013 Torbjörn Klatt <opensource@torbjoern-klatt.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __DEEP_CONST_PTR_HPP
#define __DEEP_CONST_PTR_HPP

#include <memory>

#include <iostream>

/**
 * \author Torbjörn Klatt <opensource@torbjoern-klatt.de>
 * \date 2013-04-14
 * \copyright MIT License
 * \brief An extended smart pointer for enabling deep (true) constness with pimpl idiom
 * \details This smart pointer is intended to be used with classes implementing
 *   the pimpl idiom with their private members in a private class.
 *   With the classic naive approach there is no way of ensuring const access
 *   of the private members in const member function through the private pointer.
 * 
 *   Managing the private pointer via deep_const_ptr allows for usage of C++11
 *   std::unique_ptr and std::shared_ptr and provides true const access of the
 *   private members via the private pointer.
 * 
 *   Example usage:
 * \code
class MyClass {
  private:
    class Private;
    deep_const_ptr< std::shared_ptr< MyClass::Private > > d;

  public:
    MyClass() : 
      d( deep_const_ptr< std::shared_ptr< MyClass::Private > >( new MyClass::Private )
    {}
    
    void setInt( int i ) {
      d->privateInt = i;             // uses non-const operator->()
    }

    void print() const {
      std::cout << "My int is "
                << d->privateInt     // uses const operator->()
                << std::endl;
    }
};

class MyClass::Private {
  public:
    explicit Private() : privateInt(0) {}

    int privateInt;
};
 * \endcode
 * \tparam pointerTypeT The actual smart pointer to derive from (usually
 *   `std::shared_ptr<T>` or `std::unique_ptr<T>`.
 * \note That smart pointer needs to implement `T* operator->()` and provide 
 *   the public member `element_type` for accessing the pointer type of the 
 *   managed object.
 */
template<typename pointerTypeT>
class deep_const_ptr : public pointerTypeT
{
  public:
    /**
     * \brief Standard Constructor
     * \details This calls the standard ctor of the derived smart pointer
     * \param ptr Actual pointer, which will be accessed by the owner(s) of this
     * \tparam pointerTypeT The underlying smart pointer
     */
    explicit deep_const_ptr(typename pointerTypeT::element_type* ptr) :
        pointerTypeT(ptr)
    {}

    /**
     * \brief Provides a non-const pointer to the managed object
     * \return Non-const pointer to the managed object
     * \note Calls the underlying smart pointer's operator `T* operator->()`
     */
    typename pointerTypeT::element_type* operator->()
    {
      std::cout << "deep_const_ptr::operator->()" << std::endl;
      return pointerTypeT::operator->();
    }

    /**
     * \brief Provides a const pointer to the managed object
     * \return Const pointer to the managed object
     * \note Calls the underlying smart pointer's operator `T* operator->()`
     */
    const typename pointerTypeT::element_type* operator->() const
    {
      std::cout << "deep_const_ptr::operator->() const" << std::endl;
      return pointerTypeT::operator->();
    }
};

#endif // __DEEP_CONST_PTR_HPP
