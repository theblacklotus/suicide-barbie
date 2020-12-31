// @mutalisk
// Array implementation taken from boost
//


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
#ifndef MUTALISK_FROM_BOOST__STATIC_ARRAY_H
#define MUTALISK_FROM_BOOST__STATIC_ARRAY_H

#include "cfg.h"

#include <cstddef>
#include <stdexcept>
#include <cassert>

#include <algorithm>

namespace mutalisk {

    template<class T, std::size_t N>
    class static_array {
      public:
        T elems[N];    // fixed-size array of elements of type T

      public:
        // type definitions
        typedef T              value_type;
        typedef T*             iterator;
        typedef const T*       const_iterator;
        typedef T&             reference;
        typedef const T&       const_reference;
        typedef std::size_t    size_type;
        typedef std::ptrdiff_t difference_type;
    
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
            assert( i < N && "out of range" ); 
            return elems[i];
        }
        
        const_reference operator[](size_type i) const 
        {     
            assert( i < N && "out of range" ); 
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

        // size is constant
        static size_type size() { return N; }
        static bool empty() { return false; }
        static size_type max_size() { return N; }
        enum { static_size = N };

        // swap (note: linear complexity)
        void swap (array<T,N>& y) {
            std::swap_ranges(begin(),end(),y.begin());
        }

        // direct access to data (read-only)
        const T* data() const { return elems; }

        // use array as C array (direct read/write access to data)
        T* c_array() { return elems; }

        // assignment with type conversion
        template <typename T2>
        array<T,N>& operator= (const array<T2,N>& rhs) {
            std::copy(rhs.begin(),rhs.end(), begin());
            return *this;
        }

        // assign one value to all elements
        void assign (const T& value)
        {
            std::fill_n(begin(),size(),value);
        }

        // check range (may be private because it is static)
        static void rangecheck (size_type i) {
            if (i >= size()) { 
                throw std::range_error("array<>: index out of range");
            }
        }

    };

    // comparisons
    template<class T, std::size_t N>
    bool operator== (const array<T,N>& x, const array<T,N>& y) {
        return std::equal(x.begin(), x.end(), y.begin());
    }
    template<class T, std::size_t N>
    bool operator< (const array<T,N>& x, const array<T,N>& y) {
        return std::lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
    }
    template<class T, std::size_t N>
    bool operator!= (const array<T,N>& x, const array<T,N>& y) {
        return !(x==y);
    }
    template<class T, std::size_t N>
    bool operator> (const array<T,N>& x, const array<T,N>& y) {
        return y<x;
    }
    template<class T, std::size_t N>
    bool operator<= (const array<T,N>& x, const array<T,N>& y) {
        return !(y<x);
    }
    template<class T, std::size_t N>
    bool operator>= (const array<T,N>& x, const array<T,N>& y) {
        return !(x<y);
    }

    // global swap()
    template<class T, std::size_t N>
    inline void swap (array<T,N>& x, array<T,N>& y) {
        x.swap(y);
    }
} // namespace mutalisk

#endif //MUTALISK_FROM_BOOST__ARRAY_H
