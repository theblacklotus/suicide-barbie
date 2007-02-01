/*!  \file kfbxplug.h
 */

#ifndef _FBXSDK_PLUG_H_
#define _FBXSDK_PLUG_H_
/**************************************************************************************

 Copyright © 2004 - 2006 Autodesk, Inc. and/or its licensors.
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

#include <kaydaradef.h>
#ifndef KFBX_DLL 
	#define KFBX_DLL K_DLLIMPORT
#endif

#include <kaydara.h>

#include <kbaselib_nsbegin.h> // namespace 

class KPlug;

#include <kbaselib_nsend.h> // namespace 
#ifndef MB_FBXSDK
#include <kbaselib_nsuse.h> // namespace 
#endif


#include <fbxfilesdk_nsbegin.h>

	/** Base To define the ClassId of an object
	  * \remarks This class Id helps the fbxsdk identify the class hierarchy of plugs and objects. Each Plug is identified by a class id and a parent classId
	  * \nosubgrouping
	  */
	class				KFbxPlug;
	class				KFbxSdkManager;
	class				KFbxClassIdInfo;
	class				KClassIdInfoFilter;
	class				KPlugFilter;
	class				KFbxObject;

	typedef KFbxPlug*   (*kFbxPlugConstructor)		(KFbxSdkManager& pManager, char const* pName);

	class KFBX_DLL kFbxClassId {
		public:
			kFbxClassId();
			kFbxClassId(char const *pClassName,kFbxClassId const &pParentClassId,kFbxPlugConstructor pConstructor=0);
		public:
			const char*			GetName	 () const;
			kFbxClassId		GetParent() const;
			KFbxPlug*			Create	 (KFbxSdkManager& pManager,char const *pName);
			bool				Override(kFbxPlugConstructor pConstructor);
			bool				Is(kFbxClassId const pId) const;

			inline KFbxClassIdInfo*	GetClassIdInfo() { return mClassInfo; }

			bool				SetFbxFileTypeName(const char *);
			const char *		GetFbxFileTypeName() const;
			bool				SetFbxFileSubTypeName(const char *);
			const char *		GetFbxFileSubTypeName() const;

			inline bool			IsValid() const { return mClassInfo ? true : false; }

			KPlugFilter*		GetFilter();
			
		private:
			kFbxClassId(KFbxClassIdInfo*	mClassInfo);
			KFbxClassIdInfo*	mClassInfo;

			friend class KClassIdInfoFilter;
	};

#if defined(KARCH_ENV_MACOSX)
	#define KFBXPLUG_DECLARE(Class)\
		public:\
		static kFbxClassId	ClassId;\
		static Class*			Create(KFbxSdkManager *pManager,char const *pName);\
		static Class*			SdkManagerCreate(KFbxSdkManager *pManager,char const *pName);\
		virtual kFbxClassId	GetNewFbxClassId() const { return ClassId; }\
		\
		friend class FBXFILESDK_NAMESPACE::KFbxSdkManager;
#else
	#define KFBXPLUG_DECLARE(Class)\
		public:\
		static kFbxClassId	ClassId;\
		static Class*			Create(KFbxSdkManager *pManager,char const *pName);\
		static Class*			SdkManagerCreate(KFbxSdkManager *pManager,char const *pName);\
		virtual kFbxClassId	GetNewFbxClassId() const { return ClassId; }\
		\
		friend class KFbxSdkManager;
#endif

	#define KFBXPLUG_IMPLEMENT(Class)\
		kFbxClassId	Class::ClassId;\
		Class*	Class::Create(KFbxSdkManager *pManager,char const *pName="")\
		{ \
		  Class* ClassPtr=0;\
			return (Class *)pManager->CreateClass( Class::ClassId,pName );\
		}\
		Class*	Class::SdkManagerCreate(KFbxSdkManager *pManager,char const *pName)\
		{\
		  Class* lClass = new Class(*pManager,pName);\
			lClass->Construct();\
			return lClass;\
		}

	enum EPlugEventType {
		eConnectSrcRequest,
		eConnectSrc,
		eConnectedSrc,
		eConnectDstRequest,
		eConnectDst,
		eConnectedDst,
		eDisconnectSrcRequest,
		eDisconnectSrc,
		eDisconnectedSrc,
		eDisconnectDstRequest,
		eDisconnectDst,
		eDisconnectedDst
	};

	/** Class the handles plug events.
	  * \nosubgrouping
	  */
	class KFBX_DLL KFbxPlugEvent
	{
		/**
		  * \name Constructor and Destructors.
		  */
		//@{
		public:
			inline KFbxPlugEvent(EPlugEventType	pType,KFbxPlug* pSrc,KFbxPlug* pDst)
				: mType(pType)
				, mSrc(pSrc)
				, mDst(pDst)
			{

			}
		//@}

		/**
		  * \name Data Access.
		  */
		//@{
		public:
			inline EPlugEventType	GetType() const	{ return mType; }
			inline KFbxPlug*		GetSrc()  const	{ return mSrc;  }
			inline KFbxPlug*		GetDst()  const	{ return mDst;  }
		//@}

		private:
			EPlugEventType	 mType;
			KFbxPlug*		 mSrc;
			KFbxPlug*		 mDst;
	};

	/** Base class to handle plug connections.
	  * \remarks This class is for the FBX SDK internal use only.
	  * \nosubgrouping
	  */
	class KFBX_DLL KFbxPlug
	{
		KFBXPLUG_DECLARE(KFbxPlug);

		/**
		  * \name Constructor and Destructors.
		  */
		//@{
		public:
			/** Delete the object and Unregister from the FbxSdkManager
			  */
			virtual void Destroy(bool pRecursive = false, bool pDependents = false);
		//@}


		/**
		  * \name Object ownership and type management.
		  */
		//@{
		public:
			/** Get the KFbxSdkManager that created this object 
			  * \return Pointer to the KFbxSdkManager
			  */
			virtual KFbxSdkManager*	GetFbxSdkManager() { return 0; }
			/** Test if the class is a hierarchical children of the specified class type  
			  * \param pClassId ClassId representing the class type 
			  * \return Returns true if the object is of the type specified
			  */
			virtual bool					Is(kFbxClassId pClassId) const			{ return GetNewFbxClassId().Is(pClassId);	}
			template < class T >inline bool	Is(T *pFBX_TYPE) const					{ return Is(T::ClassId); }
			virtual bool					IsRuntime(kFbxClassId pClassId) const	{ return GetRuntimeClassId().Is(pClassId);	}
			virtual bool			SetRuntimeClassId(kFbxClassId pClassId);
			virtual kFbxClassId  GetRuntimeClassId() const;
		//@}

		/**
		  * \name User-specific Data
		  */
		//@{

			/** Store a pointer to an application-specific memory region.
			  * \param pUserData A pointer to be carry over by the KFbxObject.
			  * \remarks The FBX SDK make no use of this pointer, it's the application
			  * responsability to allocate/free the memory referenced by this pointer.
			  */
			virtual void SetUserDataPtr(void* pUserData);

			/** Retrieve the application-specific pointer stored in this KFbxObject.
			  * \return Return the pointer previously stored by SetUserData(), or \c NULL if the
			  * pointer was not set.
			  */
			virtual void* GetUserDataPtr();
		
		//@}

		/**
		  * \name General Object Connection and Relationship Management
		  */
		//@{
			// SrcObjects
			int									GetSrcObjectCount	(kFbxClassId pClassId) const;											
			inline int							GetSrcObjectCount	() const				{ return GetSrcObjectCount(kFbxClassId()); }
			template < class T > inline	int		GetSrcObjectCount	(T const *pFBX_TYPE) const { return GetSrcObjectCount(T::ClassId); }
			KFbxObject*							GetSrcObject		(kFbxClassId pClassId,int pIndex=0) const;							
			inline KFbxObject*					GetSrcObject		(int pIndex=0) const					{ return GetSrcObject(kFbxClassId(),pIndex); }
			template < class T > inline T*		GetSrcObject		(T const *pFBX_TYPE,int pIndex=0) const	{ return (T *)GetSrcObject(T::ClassId,pIndex); }
			KFbxObject*							FindSrcObject		(kFbxClassId pClassId,const char *pName,int pStartIndex=0) const;	
			inline KFbxObject*					FindSrcObject		(const char *pName,int pStartIndex=0) const	{ return FindSrcObject(kFbxClassId(),pName,pStartIndex); }
			template < class T > inline T*		FindSrcObject		(T const *pFBX_TYPE,const char *pName,int pStartIndex=0) const	{ return (T *)FindSrcObject(pName,T::ClassId,pStartIndex); }
			bool								ConnectSrcObject	(KFbxObject* pObject);																
			bool								IsConnectedSrcObject(KFbxObject* pObject);																
			bool								DisconnectSrcObject	(KFbxObject* pObject);																
			bool								DisconnectAllSrcObject(kFbxClassId pClassId);
			inline bool							DisconnectAllSrcObject()						{ return DisconnectAllSrcObject(kFbxClassId()); }
			template < class T > inline bool	DisconnectAllSrcObject(T const *pFBX_TYPE)		{ return DisconnectAllSrcObject(T::ClassId); }

			// DstObjects
			int									GetDstObjectCount	(kFbxClassId pClassId) const;											
			inline int							GetDstObjectCount	() const				{ return GetDstObjectCount(kFbxClassId()); }
			template < class T > inline	int		GetDstObjectCount	(T const *pFBX_TYPE) const { return GetDstObjectCount(T::ClassId); }
			KFbxObject*							GetDstObject		(kFbxClassId pClassId,int pIndex=0) const;							
			inline KFbxObject*					GetDstObject		(int pIndex=0) const					{ return GetDstObject(kFbxClassId(),pIndex); }
			template < class T > inline T*		GetDstObject		(T const *pFBX_TYPE,int pIndex=0) const { return (T *)GetDstObject(T::ClassId,pIndex); }
			KFbxObject*							FindDstObject		(kFbxClassId pClassId,const char *pName,int pStartIndex=0) const;	
			inline KFbxObject*					FindDstObject		(const char *pName,int pStartIndex=0) const	{ return FindDstObject(kFbxClassId(),pName,pStartIndex); }
			template < class T > inline T*		FindDstObject		(T const *pFBX_TYPE,const char *pName,int pStartIndex=0) const	{ return (T *)FindDstObject(pName,T::ClassId,pStartIndex); }
			bool								ConnectDstObject	(KFbxObject* pObject);																
			bool								IsConnectedDstObject(KFbxObject* pObject);																
			bool								DisconnectDstObject	(KFbxObject* pObject);																
			bool								DisconnectAllDstObject(kFbxClassId pClassId);
			inline bool							DisconnectAllDstObject()						{ return DisconnectAllDstObject(kFbxClassId()); }
			template < class T > inline bool	DisconnectAllDstObject(T const *pFBX_TYPE)		{ return DisconnectAllDstObject(T::ClassId); }
		//@}

		/**
		  * \name Internal Connection management
		  */
		//@{
		public:
			int			GetFbxSrcCount		(kFbxClassId pClassId=kFbxClassId()) const;
			KFbxPlug*	GetFbxSrc			(int pIndex=0,kFbxClassId pClassId=kFbxClassId()) const;
			KFbxPlug*	FindFbxSrc			(const char *pName,kFbxClassId pClassId=kFbxClassId(),int pStartIndex=0) const;
			int			FindFbxSrcIndex		(KFbxPlug* pPlug) const;
			int			GetFbxDstCount		(kFbxClassId pClassId=kFbxClassId()) const;
			KFbxPlug*	GetFbxDst			(int pIndex=0,kFbxClassId pClassId=kFbxClassId()) const;
			KFbxPlug*	FindFbxDst			(const char *pName,kFbxClassId pClassId=kFbxClassId(),int pStartIndex=0) const;
			int			FindFbxDstIndex		(KFbxPlug* pPlug) const;
			bool		ConnectFbxSrc		(KFbxPlug* pPlug);
			bool		ConnectFbxDst		(KFbxPlug* pPlug);
			bool		IsConnectedFbxSrc	(KFbxPlug const* pPlug) const;
			bool		IsConnectedFbxDst	(KFbxPlug const* pPlug) const;
			bool		DisconnectFbxSrc	(KFbxPlug* pPlug);
			bool		DisconnectFbxDst	(KFbxPlug* pPlug);
		//@}

		/**
		  * \name Plug value and connection notifications 
		  */
		//@{
			virtual bool FbxPlugNotify(KFbxPlugEvent const &pEvent);
		//@}



		///////////////////////////////////////////////////////////////////////////////
		//
		//  WARNING!
		//
		//	Anything beyond these lines may not be documented accurately and is 
		// 	subject to change without notice.
		//
		///////////////////////////////////////////////////////////////////////////////
		#ifndef DOXYGEN_SHOULD_SKIP_THIS
			protected:
				inline KFbxPlug() { }
				inline KFbxPlug(KFbxSdkManager& pManager, char const* pName) { }
				virtual ~KFbxPlug() {}

				virtual void		Construct();
				virtual void		Destruct(bool pRecursive, bool pDependents);
				virtual void		SetNewFbxClassId(kFbxClassId pClassId) {}
				static char const*	GetNamePrefix() { return 0; }

			// Internal Connection Management
			// based on plugs for the time being
			public:
				virtual KPlug*		GetInternalPlug(bool pCreateOnDemand=false) { return 0; }
				virtual KPlug const* GetInternalPlug(bool pCreateOnDemand=false) const { return 0; }
				static  KFbxPlug*	GetFbxPlug(KPlug*);

			private:
				
		#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS
	};

	#define FBX_TYPE(class)		((class const*)0)
	#define FBX_CLASSID(class)	(class::ClassId)

	template < class T > inline T* KFbxCast(KFbxPlug *pPlug) {
		return pPlug && pPlug->Is(FBX_CLASSID(T)) ? (T *)pPlug : 0;
	}

	template < class T > inline T const* KFbxCast(KFbxPlug const*pPlug) {
		return pPlug && pPlug->Is(FBX_CLASSID(T)) ? (T const*)pPlug : 0;
	}

    /**********************************************************************
    * Plug Iterator
    **********************************************************************/
    class KFbxIteratorSrcBase
    {
    protected:
	    KFbxPlug*		mPlug;
		kFbxClassId	mClassId;
	    int		mSize;
	    int		mIndex;
    public:
	    inline KFbxIteratorSrcBase(KFbxPlug* pPlug,kFbxClassId pClassId) :
			mClassId(pClassId),
			mPlug(pPlug),
		    mSize(0),
		    mIndex(-1)
	    {
		    ResetToBegin();
	    }
	    inline KFbxPlug* GetFirst()
	    {
		    ResetToBegin();
		    return GetNext();
	    }
	    inline KFbxPlug* GetNext()
	    {
		    mIndex++;
		    return ((mIndex>=0) && (mIndex<mSize)) ? mPlug->GetFbxSrc(mIndex,mClassId) : NULL;
	    }
	    inline KFbxPlug* GetSafeNext()
	    {
		    mSize = mPlug->GetFbxSrcCount(mClassId);
		    return GetNext();
	    }
	    inline KFbxPlug* GetLast()
	    {
		    ResetToEnd();
		    return GetPrevious();
	    }
	    inline KFbxPlug* GetPrevious()
	    {
		    mIndex--;
		    return ((mIndex>=0) && (mIndex<mSize)) ? mPlug->GetFbxSrc(mIndex,mClassId) : NULL;
	    }
	    inline KFbxPlug* GetSafePrevious()
	    {
		    mSize = mPlug->GetFbxSrcCount(mClassId);
		    while (mIndex>mSize) mIndex--;
		    return GetPrevious();
	    }


    // Internal Access Function
    protected:
	    inline void ResetToBegin()
	    {
		    mSize = mPlug ? mPlug->GetFbxSrcCount(mClassId) : 0;
		    mIndex = -1;
	    }
	    inline void ResetToEnd()
	    {
		    mSize = mPlug ? mPlug->GetFbxSrcCount(mClassId) : 0;
		    mIndex = mSize;
	    }
    };
     
    template<class Type> class KFbxIteratorSrc : protected KFbxIteratorSrcBase
    {
    public:
	    inline KFbxIteratorSrc(KFbxPlug* pPlug) : KFbxIteratorSrcBase(pPlug,Type::ClassId) {}
	    inline Type *GetFirst()			{ return (Type *)KFbxIteratorSrcBase::GetFirst(); }
	    inline Type *GetNext()			{ return (Type *)KFbxIteratorSrcBase::GetNext(); }
	    inline Type *GetSafeNext()		{ return (Type *)KFbxIteratorSrcBase::GetSafeNext(); }
	    inline Type *GetLast()			{ return (Type *)KFbxIteratorSrcBase::GetLast(); }
	    inline Type *GetPrevious()		{ return (Type *)KFbxIteratorSrcBase::GetPrevious(); }
	    inline Type *GetSafePrevious()	{ return (Type *)KFbxIteratorSrcBase::GetSafePrevious(); }

    // Internal Access Function
    protected:
    };

    class KFbxIteratorDstBase
    {
    protected:
	    KFbxPlug*		mPlug;
		kFbxClassId	mClassId;
	    int				mSize;	
	    int				mIndex;
    public:
	    inline KFbxIteratorDstBase(KFbxPlug* pPlug,kFbxClassId pClassId) :
			mClassId(pClassId),
			mPlug(pPlug),
		    mSize(0),
		    mIndex(-1)
	    {
		    ResetToBegin();
	    }
	    inline KFbxPlug* GetFirst()
	    {
		    ResetToBegin();
		    return GetNext();
	    }
	    inline KFbxPlug* GetNext()
	    {
		    mIndex++;
		    return ((mIndex>=0) && (mIndex<mSize)) ? mPlug->GetFbxDst(mIndex,mClassId) : NULL;
	    }
	    inline KFbxPlug* GetSafeNext()
	    {
		    mSize = mPlug->GetFbxDstCount(mClassId);
		    return GetNext();
	    }
	    inline KFbxPlug* GetLast()
	    {
		    ResetToEnd();
		    return GetPrevious();
	    }
	    inline KFbxPlug* GetPrevious()
	    {
		    mIndex--;
		    return ((mIndex>=0) && (mIndex<mSize)) ? mPlug->GetFbxDst(mIndex,mClassId) : NULL;
	    }
	    inline KFbxPlug* GetSafePrevious()
	    {
		    mSize = mPlug->GetFbxDstCount(mClassId);
		    while (mIndex>mSize) mIndex--;
		    return GetPrevious();
	    }

    // Internal Access Function
    protected:
	    inline void ResetToBegin()
	    {
		    mSize = mPlug->GetFbxDstCount(mClassId);
		    mIndex = -1;
	    }
	    inline void ResetToEnd()
	    {
		    mSize = mPlug->GetFbxDstCount(mClassId);
		    mIndex = mSize;
	    }
    };
     
    template<class Type> class KFbxIteratorDst : protected KFbxIteratorDstBase
    {
    public:
	    inline KFbxIteratorDst(KFbxPlug* pPlug) : KFbxIteratorDstBase(pPlug,Type::ClassId) {}
	    inline Type *GetFirst()			{ return (Type *)KFbxIteratorDstBase::GetFirst(); }
	    inline Type *GetNext()			{ return (Type *)KFbxIteratorDstBase::GetNext(); }
	    inline Type *GetSafeNext()		{ return (Type *)KFbxIteratorDstBase::GetSafeNext(); }
	    inline Type *GetLast()			{ return (Type *)KFbxIteratorDstBase::GetLast(); }
	    inline Type *GetPrevious()		{ return (Type *)KFbxIteratorDstBase::GetPrevious(); }
	    inline Type *GetSafePrevious()	{ return (Type *)KFbxIteratorDstBase::GetSafePrevious(); }

    // Internal Access Function
    protected:
    };

    #define KFbxForEach(Iterator,Plug) for ( (Plug)=(Iterator).GetFirst(); (Plug)!=NULL; (Plug)=(Iterator).GetNext() ) 
    #define KFbxReverseForEach(Iterator,Plug) for ( Plug=(Iterator).GetLast(); Plug;  Plug=(Iterator).GetPrevious() ) 
    #define KFbxForEach_Safe(Iterator,Plug) for ( Plug=(Iterator).GetFirst(); Plug; Plug=(Iterator).GetSafeNext() ) 
    #define KFbxReverseForEach_Safe(Iterator,Plug) for ( Plug=(Iterator).GetLast(); Plug;  Plug=(Iterator).GetSafePrevious() ) 



#include <fbxfilesdk_nsend.h>

#endif // #ifndef _FBXSDK_PLUG_H_


