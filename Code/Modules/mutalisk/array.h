// @mutalisk
// Array implementation based on boost (static) array<T, N>
// Changed constant size array to dynamically allocated one


/* The following code declares class array,
 * an STL container (as wrapper) for arrays of constant size.
 *
 * See
 *      http://www.josuttis.com/cppcode
 * for details and the latest version.
 * See
 *      http://www.boost.org/libs/array for Documentation.
 * for documentation.
 *
 * (C) Copyright Nicolai M. Josuttis 2001.
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * 29 Jan 2004 - c_array() added, BOOST_NO_PRIVATE_IN_AGGREGATE removed (Nico Josuttis)
 * 23 Aug 2002 - fix for Non-MSVC compilers combined with MSVC libraries.
 * 05 Aug 2001 - minor update (Nico Josuttis)
 * 20 Jan 2001 - STLport fix (Beman Dawes)
 * 29 Sep 2000 - Initial Revision (Nico Josuttis)
 *
 * Jan 29, 2004
 */
#ifndef MUTALISK_FROM_BOOST__ARRAY_H
#define MUTALISK_FROM_BOOST__ARRAY_H

#include "cfg.h"

#include <cstddef>
#include <stdexcept>
#include <algorithm>

namespace mutalisk {

    template<class T>
    class array {
      public:
          T* elems;
          std::size_t N;
		  static T dummy;

      public:
        // type definitions
        typedef T              value_type;
        typedef T*             iterator;
        typedef const T*       const_iterator;
        typedef T&             reference;
        typedef const T&       const_reference;
        typedef std::size_t    size_type;
        typedef std::ptrdiff_t difference_type;

      public:
          array(size_type n = 0) : elems(0) { resize(n); }
		  array(array<T> const& c) :  elems(0) { assignArray(c); }
          ~array() { resize(0); }
    
        // iterator support
        iterator begin() { return elems; }
        const_iterator begin() const { return elems; }
        iterator end() { return elems+N; }
        const_iterator end() const { return elems+N; }

        // @mutalisk
        // NOTE: removed reverse iterators
        // for compatibility reasons

        // operator[]
        reference operator[](size_type i) 
        {
            ASSERT(i < N && "out of range"); 
			if(empty()) return dummy;
            return elems[i];
        }
        
        const_reference operator[](size_type i) const 
        {     
            ASSERT(i < N && "out of range"); 
			if(empty()) return dummy;
            return elems[i]; 
        }

        // at() with range check
        reference at(size_type i) { rangecheck(i); return elems[i]; }
        const_reference at(size_type i) const { rangecheck(i); return elems[i]; }
    
        // front() and back()
        reference front() 
        { 
            return elems[0]; 
        }
        
        const_reference front() const 
        {
            return elems[0];
        }
        
        reference back() 
        { 
            return elems[N-1]; 
        }
        
        const_reference back() const 
        { 
            return elems[N-1]; 
        }

		// size
        void resize(size_type size)
        {
			if(size == N)
				return;

            delete[] elems;
            if(size > 0)
                elems = new T[size];
            else
                elems = 0;
            N = size;
        }
        size_type size() const { return N; }
        bool empty() const { return (elems == 0); }

        void swap (array<T>& y) {
            std::swap(elems, y.elems);
            std::swap(N, y.N);
            // @boost:
            // std::swap_ranges(begin(),end(),y.begin());
        }

        // direct access to data (read-only)
        const T* data() const { return elems; }

        // use array as C array (direct read/write access to data)
        T* c_array() { return elems; }

        // assign one value to all elements
        void assign(const T& value)
        {
            std::fill_n(begin(),size(),value);
        }

        // assignment with type conversion
        template <typename T2>
        array<T>& assignArray(const array<T2>& rhs) {
            if(this == &rhs)
                return *this;
            resize(rhs.size());
            std::copy(rhs.begin(),rhs.end(), begin());
            return *this;
        }
        template <typename T2>
        array<T>& operator= (const array<T2>& rhs) {
			return assignArray(rhs);
        }

        // check range
        void rangecheck (size_type i) {
            ASSERT(i >= size() && "index out of range");

            // @boost:
            // if (i >= size()) { 
            // throw std::range_error("array<>: index out of range");
            // }
        }

    };

	template <typename T> T array<T>::dummy;


    // comparisons
    template<class T>
    bool operator== (const array<T>& x, const array<T>& y) {
        return std::equal(x.begin(), x.end(), y.begin());
    }
    template<class T>
    bool operator< (const array<T>& x, const array<T>& y) {
        return std::lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
    }
    template<class T>
    bool operator!= (const array<T>& x, const array<T>& y) {
        return !(x==y);
    }
    template<class T>
    bool operator> (const array<T>& x, const array<T>& y) {
        return y<x;
    }
    template<class T>
    bool operator<= (const array<T>& x, const array<T>& y) {
        return !(y<x);
    }
    template<class T>
    bool operator>= (const array<T>& x, const array<T>& y) {
        return !(x<y);
    }

    // global swap()
    template<class T>
    inline void swap (array<T>& x, array<T>& y) {
        x.swap(y);
    }
} // namespace mutalisk

#endif //MUTALISK_FROM_BOOST__ARRAY_H
