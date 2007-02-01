/*!  \file kstringlist.h
 */

#ifndef _FBXSDK_KSTRING_LIST_H_
#define _FBXSDK_KSTRING_LIST_H_

/**************************************************************************************

 Copyright © 1995 - 2006 Autodesk, Inc. and/or its licensors.
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

#include <stdlib.h> 

#if defined(KARCH_ENV_MACOSX_CFM)
#include <extras.h>
#endif


#include <klib/karrayul.h>

#include <isocpp_conformant.h> // RR: for new deprecated functions introduced by VC++ 2005
// We include the file here because we want to make sure that the symbols we have re-defined
// in isocpp_conformat.h correctly overwrite the respective functions.

#include <klib/t-str.h>
#include <klib/kstring.h>
#include <kbaselib_nsbegin.h>

    class KBASELIB_DLL KStringListItem {
    public:
	    KString		mString; 
	    kReference	mReference;
	    KStringListItem( ) { mReference = 0;} 
	    KStringListItem( char* pString, kReference pRef=0 ) { mString = pString; mReference = pRef;} 
    };

    inline int CompareKStringList(const void *E1,const void *E2)
    {
		return t_stricmp((*(KStringListItem *)E1).mString.Buffer(),(*(KStringListItem *)E2).mString.Buffer());
    }

	inline int CompareCaseSensitiveKStringList(const void *E1,const void *E2)
    {
		return strcmp((*(KStringListItem *)E1).mString.Buffer() ,(*(KStringListItem *)E2).mString.Buffer());
    }

    typedef class KArrayTemplate<KStringListItem *> KArrayStringListItem;

    template <class Type> class KStringListTemplate {
    protected:
	    KArrayTemplate<Type *> mList;

    public:
	    int		AddItem( Type *pItem )		{ return mList.Add( pItem ); }
	    int		InsertItemAt( int pIndex, Type *pItem )	{ return mList.InsertAt( pIndex, pItem ); }
	    Type   *GetItemAt( int pIndex )		{ return mList[pIndex]; }
	    int		FindItem( Type *pItem )		{ return mList.Find( pItem ); }

    public :    
	    KStringListTemplate()
	    {
	    }

	    virtual ~KStringListTemplate() { Clear(); }

        void RemoveLast() { RemoveAt( mList.GetCount()-1 ); }
	    inline int		GetCount() { return mList.GetCount(); }
        KString   &operator[](int pIndex) { return mList[pIndex]->mString; }
        kReference		GetReferenceAt(int pIndex) { return mList[pIndex]->mReference; }
        void			SetReferenceAt(int pIndex, kReference pRef) { mList[pIndex]->mReference = pRef; }
        char*		GetStringAt(int pIndex) { if (pIndex<mList.GetCount()) return mList[pIndex]->mString.Buffer(); else return NULL; }
        virtual bool	SetStringAt(int pIndex,char* pString) 
	    { 
		    if (pIndex<mList.GetCount()) 
		    {
			    mList[pIndex]->mString = pString; 
			    return true;
		    } else return false; 
	    }

	    int Find( Type &pItem ) 
	    { 
		    for (int Count=0; Count<mList.GetCount(); Count++) {
			    if (mList[Count]==&pItem) {
				    return Count;
			    }
		    }
		    return -1;
	    }

	    int FindIndex( kReference pReference ) 
	    { 
		    for (int Count=0; Count<mList.GetCount(); Count++) {
			    if (mList[Count]->mReference==pReference) {
				    return Count;
			    }
		    }
		    return -1;
	    }

	    int FindIndex( char* pString ) 
	    { 
		    for (int Count=0; Count<mList.GetCount(); Count++) {
			    if (mList[Count]->mString==pString) {
				    return Count;
			    }
		    }
		    return -1;
	    }

	    kReference FindReference( char* pString )
	    {
	    int Index = FindIndex( pString );
		    if (Index!=-1) {
			    return mList[Index]->mReference;
		    }
		    return NULL;
	    }

        bool Remove ( Type &pItem )
	    {
	    int Index = Find( pItem );
	        if (Index>=0) {
			    RemoveAt( Index );
			    return true;
		    }
		    return false;
	    }

        bool Remove ( char* pString )
	    {
	    int Index = FindIndex( pString );
	        if (Index>=0) {
			    RemoveAt( Index );
			    return true;
		    }
		    return false;
	    }

        bool RemoveIt ( Type pItem )
	    {
	    int Index = Find( pItem );
	        if (Index>=0) {
			    RemoveAt( Index );
			    return true;
		    }
		    return false;
	    }

	    void Sort( )
	    {
		    qsort( &(mList.GetArray()[0]),mList.GetCount(),sizeof(KStringListItem *),CompareKStringList );
	    }

	    void* FindEqual( char* pString)
	    {
	    KStringListItem Key(pString);  
        
		    if (mList.GetCount() != 0)
		    {
			    return bsearch ( &Key, &(mList.GetArray()[0]),mList.GetCount(),sizeof(KStringListItem *),CompareKStringList );
		    }
		    else
		    {
			    return NULL ;
		    }
	    }

		void* FindCaseSensitive(char * pString)
		{
	    KStringListItem Key(pString);  
        
		    if (mList.GetCount() != 0)
		    {
			    return bsearch ( &Key, &(mList.GetArray()[0]),mList.GetCount(),sizeof(KStringListItem *), CompareCaseSensitiveKStringList);
		    }
		    else
		    {
			    return NULL ;
		    }
		
		}


	    int Add( char* pString, kReference pItem=0 ) 
	    { 
		    return InsertAt( mList.GetCount(),pString,pItem ); 
	    }

	    virtual int InsertAt( int pIndex, char* pString, kReference pItem=0 ) 
	    { 
		    return mList.InsertAt( pIndex,new Type( pString,(kReference)pItem )); 
	    }

	    virtual void RemoveAt( int pIndex ) 
	    { 
		    delete mList[pIndex]; mList.RemoveAt( pIndex ); 
	    }

        virtual void Clear()
	    {
		    for (int Count=0; Count<mList.GetCount(); Count++ ) {
			    delete mList[Count];
		    }
		    mList.Clear();
	    }

        virtual void GetText(KString& pText)
	    {
		    int		Count;
		    for (Count=0; Count<mList.GetCount(); Count++) 
            {
                pText += mList[Count]->mString;
                if (Count<mList.GetCount()-1) 
                {
                    pText += _T("~");
                }
		    }
	    }


	    virtual int SetText(char* pList)
	    {
  	    int		Pos=0, OldPos = 0;
	    int		LastIndex=0;
  	    KString	Name=pList;

  		    Clear();
    	    for (Pos=0; Name.Buffer()[Pos]!=0; Pos++) {
        	    if (Name.Buffer()[Pos]==_T('~')) {
            	    Name.Buffer()[Pos]=0;
            	    LastIndex = Add(Name.Buffer()+OldPos);
            	    OldPos=Pos+1;
        	    }
		    }

		    if(OldPos != Pos)
		    {
		        LastIndex = Add(Name.Buffer()+OldPos);
		    }
		    return LastIndex;
	    } 


    };

    typedef class KBASELIB_DLL KStringListTemplate< KStringListItem > KStringListTemplateItem;

    class KBASELIB_DLL KStringList : public KStringListTemplate< KStringListItem >
    {
    public :    
	    KStringList(); 
	    KStringList( const KStringList &pOriginal );
	    void CopyFrom( const KStringList *pOriginal  );
    	
	    KStringList& operator=(const KStringList& pOriginal);
    };

    typedef KStringList *HKStringList;

#include <kbaselib_nsend.h>

#endif
