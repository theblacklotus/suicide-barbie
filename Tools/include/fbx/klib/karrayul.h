/*!  \file karrayul.h
 */

#ifndef _FBXSDK_KARRAYUL_H_
#define _FBXSDK_KARRAYUL_H_

/**************************************************************************************   
 Copyright © 1999 - 2006 Autodesk, Inc. and/or its licensors.
 All Rights Reserved.

 The coded instructions, statements, computer programs, and/or related material
 (collectively the "Data") in these files contain unpublished information
 proprietary to Autodesk, Inc. and/or its licensors, which is protected by
 Canada and United States of America federal copyright law and by international
 treaties.

 The Data may not be disclosed or distributed to third parties, in whole or in
 part, without the prior written consent of Autodesk, Inc. ("Autodesk").

 THE DATA IS PROVIDED "AS IS" AND WITHOUT WARRANTY.
 ALL WARRANTIES ARE EXPRESSLY EXCLUDED AND DISCLAIMED. AUTODESK MAKES NO
 WARRANTY OF ANY KIND WITH RESPECT TO THE DATA, EXPRESS, IMPLIED OR ARISING
 BY CUSTOM OR TRADE USAGE, AND DISCLAIMS ANY IMPLIED WARRANTIES OF TITLE,
 NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE OR USE.
 WITHOUT LIMITING THE FOREGOING, AUTODESK DOES NOT WARRANT THAT THE OPERATION
 OF THE DATA WILL BE UNINTERRUPTED OR ERROR FREE.

 IN NO EVENT SHALL AUTODESK, ITS AFFILIATES, PARENT COMPANIES, LICENSORS
 OR SUPPLIERS ("AUTODESK GROUP") BE LIABLE FOR ANY LOSSES, DAMAGES OR EXPENSES
 OF ANY KIND (INCLUDING WITHOUT LIMITATION PUNITIVE OR MULTIPLE DAMAGES OR OTHER
 SPECIAL, DIRECT, INDIRECT, EXEMPLARY, INCIDENTAL, LOSS OF PROFITS, REVENUE
 OR DATA, COST OF COVER OR CONSEQUENTIAL LOSSES OR DAMAGES OF ANY KIND),
 HOWEVER CAUSED, AND REGARDLESS OF THE THEORY OF LIABILITY, WHETHER DERIVED
 FROM CONTRACT, TORT (INCLUDING, BUT NOT LIMITED TO, NEGLIGENCE), OR OTHERWISE,
 ARISING OUT OF OR RELATING TO THE DATA OR ITS USE OR ANY OTHER PERFORMANCE,
 WHETHER OR NOT AUTODESK HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH LOSS
 OR DAMAGE.
   
**************************************************************************************/
#include <kbaselib_h.h>

#ifndef K_PLUGIN
	#include <klib/kdebug.h>
#endif

#include <string.h>
#include <stdlib.h>

#define KARRAYUL_BLOCKSIZE 16

#include <object/i/iobject.h>

#include <kbaselib_forward.h>
#include <kbaselib_nsbegin.h> // namespace

    /***********************************************************************
	    CLASS KStaticArray
    ************************************************************************/

    template< class Type > class KBaseStaticArray
    {
        protected:
        int  mCount;    
        Type *mArrayBuf;

        public:
        inline int GetCount() { return mCount; }

        //! Access pointer at given index.
	    inline Type &operator[](int pIndex) 
	    {
        #ifndef K_PLUGIN
		    K_ASSERT_MSG( pIndex >= 0	, "Buffer underflow, appelle ton plombier.");
		    K_ASSERT_MSG( pIndex < mCount,"Buffer overflow, appelle ton plombier.");
        #endif
		    return mArrayBuf[pIndex];
	    }
    };

    // Static Array
    template< class Type, int Count > class KStaticArray : public KBaseStaticArray<Type>
    {
	    public:
	    Type mArray[Count];
            inline KStaticArray(){ this->mArrayBuf = mArray; this->mCount = Count;}    
    };

    template< class Type, int Count1, int Count2 > class KStaticArray2d
    {
	    public:
#if defined(KARCH_DEV_MSC) && (_MSC_VER <= 1200)// VC6
	    KStaticArray<Type,Count2> *mArray;

		KStaticArray2d() { mArray = new KStaticArray<Type,Count2>(Count1); }
		~KStaticArray2d() { delete[] mArray; }
#else
	    KStaticArray<Type,Count2> mArray[Count1];
#endif

	    // Access pointer at given index.
	    inline KStaticArray< Type, Count2 > &operator[](int pIndex) 
	    {
        #ifndef K_PLUGIN
		    K_ASSERT_MSG( pIndex >= 0	, "Buffer underflow, appelle ton plombier.");
		    K_ASSERT_MSG( pIndex < Count1,"Buffer overflow, appelle ton plombier.");
        #endif
		    return mArray[pIndex];
	    }
    };

    /***********************************************************************
	    CLASS KArrayTemplate
    ************************************************************************/

    class KBASELIB_DLL KBaseArray
    {
    protected:
	    int mArrayCount;
	    int mBlockCount;
	    int mItemPerBlock; 
	    int mItemSize; 

	    /** Constructor.
	    * \param pItemPerBlock Number of pointers per allocated block.
	    * \param pItemSize Size of one item of the array.
	    */
	    KBaseArray(int pItemPerBlock, int pItemSize);

	    //! Destructor.
	    ~KBaseArray();

	    /** Insert an item at the given position.
	    * \param pIndex Position where to insert the item.
	    * \param pItem  Pointer to the item to be inserted.
	    * \remark if pIndex is greater than the number of items already in the
	    * array, the item will be appended at the end.
	    * \return The actual position where the item as been inserted.
	    */
	    int		InsertAt(int pIndex, void *pItem);	

	    /** Get the item at the given position.
	    * \param pIndex The position of the item to access.
	    * \return Pointer to the item.
	    * \remarks This method assumes that the passed inded is 
	    * in the valid range of the array. No checks are made.
	    */
	    inline void*	GetAt(int pIndex)							{ return &(mBaseArray[(pIndex)*mItemSize]); }

	    /** Removes the item at the given position.
	    * \param pIndex The position of the item to remove.
	    * \remarks If the index is not valid, nothing is performed. Otherwise,
	    * the item is removed from the array and the items are shifted to fill the
	    * empty slot.
	    */
	    void	RemoveAt(int pIndex);

	    /** Check that the given position is inside the array boundaries.
	    * \param pIndex Index value to validate.
	    * \return \c true if the index value is within the array boundaries. \c false
	    * otherwise.
	    */
	    bool	ValidateIndex( int pIndex ) const;

    public:
	    /** Get number of pointers in the array.
	    * \return The number of items in the array.
	    */
	    inline int GetCount() const { return mArrayCount; }

	    //! Remove all pointers without deleting the associated objects.
	    void	Clear();

	    //! Fast empty, set object count to zero but don't free any memory.
	    void	Empty();

	    /** Set array capacity to contain at least the specified number of elements without reallocating.
	    * \param pCapacity Number of items that can be stored in the array before reallocating the memory.
	    * \return The number of available slots in the array.
	    * \remarks If capacity is lower than arrayCount, arrayCount is lowered to capacity.
	    */
	    int		Reserve(int pCapacity);

	    //! Set arrayCount to specified number of elements. The array capacity is adjusted accordingly.
	    //  Differ from SetCount because array capacity can be lowewed.

	    /** Force the array of elements to a given size.
	    * \remarks If the array is upsized, the memory allocated is set to 0 and
	    * no constructor is called. Thus, this function is not appropriate for
	    * types of elements requiring initialization.
	    */
	    void	SetCount (int pCount);
	    void	Resize(int pItemCount);
	    void	AddMultiple(int pItemCount);

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  WARNING!
    //
    //	Anything beyond these lines may not be documented accurately and is 
    // 	subject to change without notice.
    //
    ///////////////////////////////////////////////////////////////////////////////

    #ifndef DOXYGEN_SHOULD_SKIP_THIS

    //private:

	    char* mBaseArray;
    // Utility functions for the DeleteAndClear function template

	    /** Overload function to determine if delete or Destroy() should be call on an item
	    * \remarks return true if Destroy should be call and false if delete can be call 
	    */
	    inline bool DeleteOrDestroy( void *pPtr )	{ return false; } // return false, Delete can be call on the item
	    bool DeleteOrDestroy( HIObject pObject );
	    bool DeleteOrDestroy( KPlug* pPlug );

    #endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS

    };


    //! Array of pointers.
    template < class Type > class KArrayTemplate : public KBaseArray
    {

    public:
	    Type *&mArray;

	    /** Constructor.
	    * \param pItemPerBlock Number of pointers per allocated block.
	    */
	    inline KArrayTemplate(int pItemPerBlock=KARRAYUL_BLOCKSIZE):KBaseArray(pItemPerBlock,sizeof(Type))
	    ,mArray((Type *&)KBaseArray::mBaseArray)
	    {
	    }

	    //! Copy constructor.
	    inline KArrayTemplate(const KArrayTemplate<Type>& pArrayTemplate):KBaseArray(KARRAYUL_BLOCKSIZE,sizeof(Type))
	    ,mArray((Type *&)KBaseArray::mBaseArray)
	    { 
		    *this = pArrayTemplate; 
	    }

	    inline ~KArrayTemplate() {}
    	
	    /** Insert a pointer.
	    * \param pIndex Position where to insert the pointer.
	    * \param pItem  Item to insert.
	    * \return Position of the inserted pointer in the array.
	    * \remarks If the given index is out of range, the pointer is appended at the end.
	    */
	    inline int InsertAt(int pIndex, Type pItem) 
	    { 
		    return KBaseArray::InsertAt( pIndex,&pItem ); 
	    }
    	
	    /** Remove a pointer in the array without deleting the associated object.
	    * \param pIndex Position of the item to remove.
	    * \return Removed item.
	    */
	    inline Type RemoveAt(int pIndex)
	    {
		    Type tmpItem = GetAt(pIndex);
		    KBaseArray::RemoveAt( pIndex );
		    return tmpItem;
	    }
    	
	    /** Remove last pointer in the array without deleting the associated object.
	    * \return Remove item.
	    */
	    inline Type RemoveLast()
	    {
		    return RemoveAt(mArrayCount-1);
	    }

	    /** Remove first matching pointer in the array without deleting the associated object.
	    * \param pItem Item to remove.
	    * \return \c true if a matching pointer is found and removed, \c false otherwise.
	    */
	    inline bool RemoveIt(Type pItem)
	    {
		    int Index = Find (pItem);
		    if (Index>=0) 
		    {
			    RemoveAt (Index);
			    return true;
		    }
		    return false;
	    }
    	
	    //! Access pointer at given index.
	    inline Type &operator[](int pIndex) const
	    {
	    #ifdef _DEBUG
		    if (!ValidateIndex( pIndex )) 
		    {
			    return (Type &)(KBaseArray::mBaseArray[(0)*sizeof(Type)]);
		    }
	    #endif
		    return (Type &)(KBaseArray::mBaseArray[(pIndex)*sizeof(Type)]);
	    }

	    //! Set pointer at given index, must within valid range.
	    inline void SetAt(int pIndex, Type pItem)
	    {
	    #ifdef _DEBUG
		    if (!ValidateIndex( pIndex )) 
		    {
			    return;
		    }
	    #endif
		    GetArray()[pIndex] = pItem;
	    }
    	
	    //! Set last pointer, the array must contain at least one pointer.
	    inline void SetLast(Type pItem)
	    {
		    SetAt (mArrayCount-1, pItem);
	    }

	    //! Get pointer at given index, must be within valid range.
	    inline Type GetAt(int pIndex) const
	    {
	    #ifdef _DEBUG
		    if (!ValidateIndex( pIndex )) 
		    {
			    return (Type &)(KBaseArray::mBaseArray[(0)*sizeof(Type)]);
		    }
	    #endif
		    return (Type &)(KBaseArray::mBaseArray[(pIndex)*sizeof(Type)]);
	    }

	    /** Get first pointer, the array must contain at least one pointer.
	    * \return First pointer.
	    */
	    inline Type GetFirst() const
	    { 
	    #ifndef K_PLUGIN
		    K_ASSERT( mArrayCount >= 1 );
	    #endif
		    return GetAt(0);
	    }

	    /** Get last pointer, the array must contain at least one pointer.
	    * \return Last pointer.
	    */
	    inline Type GetLast() const
	    { 
	    #ifndef K_PLUGIN
		    K_ASSERT( mArrayCount >= 1 );
	    #endif
		    return GetAt(mArrayCount-1);
	    }

	    /** Find first matching pointer.
	    * \return Index of first matching pointer found or -1 if  there is no
	    * matching element.
	    */
	    inline int Find(Type pItem) const
	    {
		    return FindAfter( -1, pItem );
	    }

	    /** Find first matching pointer after given index.
	    * \return Index of first matching pointer found after given index or 
	    * -1 if there is no matching pointer.
	    * \remarks The index must be within valid range.
	    */
	    inline int FindAfter(int pAfterIndex, Type pItem) const
	    {
	    #ifndef K_PLUGIN
	    #ifdef _DEBUG		
		    if ( pAfterIndex > mArrayCount || pAfterIndex < -1 ) 
		    {
			    K_ASSERT_MSG_NOW (_T("ArrayUL : Search Begin Index out of range")); 
			    return -1;
		    }
	    #endif
	    #endif
		    int Count;
		    for ( Count=pAfterIndex+1; Count<mArrayCount; Count++) 
		    {
			    if (GetAt(Count)==pItem) 
			    {
				    return Count;
			    }
		    }
		    return -1;
	    }
    	
	    /** Find first matching pointer before given index.
	    * \return Index of first matching pointer found after given index or 
	    * -1 if there is no matching pointer.
	    * \remarks The index must be within valid range.
	    */
	    inline int FindBefore(int pBeforeIndex, Type pItem) const
	    {
	    #ifndef K_PLUGIN
	    #ifdef _DEBUG		
		    if ( pBeforeIndex > mArrayCount || pBeforeIndex <= 0 ) 
		    {
			    K_ASSERT_MSG_NOW (_T("ArrayUL : Search Begin Index out of range")); 
			    return -1;
		    }
	    #endif
	    #endif
		    int Count;
		    for ( Count=pBeforeIndex-1; Count>=0; Count--) 
		    {
			    if (GetAt(Count)==pItem) 
			    {
				    return Count;
			    }
		    }
		    return -1;
	    }
    	
	    /** Append a pointer at the end of the array.
	    * \return Index of appended pointer.
	    */
	    inline int Add(Type pItem)
	    {
		    return InsertAt(mArrayCount, pItem);
	    }

	    /** Add Element at the end of array if not present.
	    * \return Index of Element.
	    */
	    inline int AddUnique(Type pItem)
	    {
		    int lReturnIndex = Find(pItem);
		    if (lReturnIndex == -1)
		    {
			    lReturnIndex = Add(pItem);
		    }
		    return lReturnIndex;
	    }

	    /**  Add multiple (init to zero) elements in the array, use SetAt or GetArray to set the value of the new elements
	    *	\param pItemCount. How many new Array elements you want.
	    */
	    inline void AddMultiple( kUInt pItemCount )
	    {
		    KBaseArray::AddMultiple( pItemCount );
	    }

	    inline void AddArray(KArrayTemplate<Type> &pArray)
	    {
		    int lSourceIndex, lCount = pArray.GetCount();
		    if( lCount == 0 ) return;
		    int lDestinationIndex = GetCount();
		    AddMultiple(lCount);
		    for( lSourceIndex = 0; lSourceIndex < lCount; lSourceIndex++)
		    {
			    SetAt(lDestinationIndex++, pArray[lSourceIndex]);
		    }
	    }

	    inline void AddArrayNoDuplicate(KArrayTemplate<Type> &pArray)
	    {
		    int i, lCount = pArray.GetCount();
		    for( i = 0; i < lCount; i++)
		    {
			    Type lItem = pArray[i];
			    if (Find(lItem) == -1)
			    {
				    Add(lItem);
			    }
		    }
	    }
	    inline void RemoveArray(KArrayTemplate<Type> &pArray)
	    {
		    int lRemoveIndex, lRemoveCount = pArray.GetCount();
		    for( lRemoveIndex = 0; lRemoveIndex < lRemoveCount; lRemoveIndex++)
		    {
			    RemoveIt(pArray[lRemoveIndex]);
		    }
	    }

	    //! Get pointer to internal array of pointers.
	    inline Type* GetArray()
	    {
		    return (Type*)KBaseArray::mBaseArray;
	    }

	    //! Copy array of pointers without copying the associated objects.
	    inline KArrayTemplate<Type>& operator=(const KArrayTemplate<Type>& pArrayTemplate)
	    {
		    Clear();

		    mItemPerBlock = pArrayTemplate.mItemPerBlock;

		    int i, lCount = pArrayTemplate.GetCount();

		    for (i = 0; i < lCount; i++)
		    {
			    Add(pArrayTemplate[i]);
		    }

		    return (*this);
	    }

	    #ifdef K_PLUGIN
		    //! Cast operator.
		    inline operator Type* ()
		    {
			    return GetArray();
		    }
	    #endif
    };

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  WARNING!
    //
    //	Anything beyond these lines may not be documented accurately and is 
    // 	subject to change without notice.
    //
    ///////////////////////////////////////////////////////////////////////////////

    #ifndef DOXYGEN_SHOULD_SKIP_THIS

    template <class Type> inline void DeleteAndClear(KArrayTemplate<Type>& Array)
    {
	    kUInt lItemCount = Array.GetCount();
	    while( lItemCount )
	    {
		    lItemCount--;
		    Type& Item = (Array.operator[](lItemCount));
		    if (!Array.DeleteOrDestroy( Item ))
		    {
			    delete Item;
		    }
		    Item = NULL;
	    }
	    Array.Clear();
    }

    typedef class KBASELIB_DLL KArrayTemplate<int *>	KArrayHkInt;
    typedef class KBASELIB_DLL KArrayTemplate<kUInt *>	KArrayHkUInt;
    typedef class KBASELIB_DLL KArrayTemplate<double *>	KArrayHkDouble;
    typedef class KBASELIB_DLL KArrayTemplate<float *>	KArrayHkFloat;
    typedef class KBASELIB_DLL KArrayTemplate<void *>	KArrayVoid;
    typedef class KBASELIB_DLL KArrayTemplate<char *>	KArrayChar;
    typedef class KBASELIB_DLL KArrayTemplate<int>		KArraykInt;
    typedef class KBASELIB_DLL KArrayTemplate<kUInt>	KArraykUInt;
    typedef class KBASELIB_DLL KArrayTemplate<float>	KArraykFloat;
    typedef class KBASELIB_DLL KArrayTemplate<double>	KArraykDouble;

    typedef class KBASELIB_DLL KArrayTemplate<kReference>	KArrayUL;

    #endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <kbaselib_nsend.h>

#endif // #define _FBXSDK_KARRAYUL_H_


