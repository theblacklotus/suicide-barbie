/*!  \file kfbxobject.h
 */

#ifndef _FBXSDK_OBJECT_H_
#define _FBXSDK_OBJECT_H_

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

#include <klib/kstring.h>
#include <klib/kstringlist.h>
#include <klib/kname.h>
#include <klib/karrayul.h>

#include <kfbxplugins/kfbxplug.h>
#include <kfbxplugins/kfbxproperty.h>

#ifndef MB_FBXSDK
#include <kbaselib_nsuse.h>
#endif
#include <kbaselib_forward.h>

#include <fbxfilesdk_nsbegin.h>

	class KFbxSdkManager;
	class KFbxScene;
	class KFbxObject_internal;
	class KFbxProperty_internal;
	class KFbxTakeNodeContainer;
	class KFbxTakeNode;

	#define KFBXOBJECT_DECLARE(Class)	KFBXPLUG_DECLARE(Class) 
	#define KFBXOBJECT_IMPLEMENT(Class) KFBXPLUG_IMPLEMENT(Class) 

	/** \brief Basic class for object type identification and instance naming.
	  * \nosubgrouping
	  */
	class KFBX_DLL KFbxObject : public KFbxPlug
	{
		KFBXOBJECT_DECLARE(KFbxObject);


		public:
		/**
		  * \name Object Name Management
		  */
		//@{

			/** Set the name of the object. 
			  * \param pName A \c NULL terminated string.
			  */
	void SetName(char const* pName, bool wantPrefix=false);

			/** Return the full name of the object.
			  * \return Return a \c NULL terminated string.
			  */
	char const* GetName() const;

			/** Return the name of the object without the namespace qualifier.
			  * \return Return the name in a temporary string.
			  */
			KString GetNameWithoutNameSpacePrefix() const;

			/** Return the name of the object with the namespace qualifier.
			  * \return Return the name in a temporary string.
			  */
			KString GetNameWithNameSpacePrefix() const;

			/** Set the initial name of the object. 
			  * \param pName A \c NULL terminated string.
			  */
			void SetInitialName(char const* pName);

			/** Return the initial name of the object.
			  * \return Return a \c NULL terminated string.
			  */
	char const* GetInitialName() const;


			/** Return the namespace of the object.
			  * \return Return a \c NULL terminated string.
			  */
			KString GetNameSpaceOnly( ) { return mName.GetNameSpace();}
			
			/** Set the namespace of the object. 
			  * \return Return a \c NULL terminated string.
			  */
			void SetNameSpace(KString pNameSpace);
		//@}

		/**
		  * \name Properties access
		  */
		//@{
			/** Return the number of property defining this object.
			*/
	int GetPropertyCount() const;

			/** Get the property at the given index.
			  * \param pIndex The index of the requested property. This value
			  * must be between 0 and the number of properties returned by
			  * GetPropertyCount().
			  * \return Return the property at the given index, or \c NULL if \param pIndex
			  * is out of bound.
			  */
	KFbxProperty	GetProperty(int pIndex) const;

	/** Find a property using its name and its data type.
			  * \param pName The name of the property as a \c NULL terminated string.
			  * \return Return a pointer to the property found or \c NULL if the property was not found.
			  */
	KFbxProperty FindProperty(const char* pName, bool pCaseSensitive = true) const;
	KFbxProperty FindProperty(const char* pName, KFbxDataType const &pDataType, bool pCaseSensitive = true) const;

		//@}

		/**
		  * \name General Property Connection and Relationship Management
		  */
		//@{
			// Properties
			inline int				GetSrcPropertyCount		()										{ return GetFbxSrcCount(KFbxProperty::ClassId); }
			inline KFbxProperty		GetSrcProperty			(int pIndex=0)							{ return *((KFbxProperty *)GetFbxSrc(pIndex,KFbxProperty::ClassId)); }
			inline KFbxProperty		FindSrcProperty			(const char *pName,int pStartIndex=0)	{ return *((KFbxProperty *)FindFbxSrc(pName,KFbxProperty::ClassId,pStartIndex)); }
			inline bool				ConnectSrcProperty		(KFbxProperty const & pProperty)		{ return ConnectFbxSrc((KFbxPlug *)&pProperty);		}
			inline bool				IsConnectedSrcProperty	(KFbxProperty const & pProperty)		{ return IsConnectedFbxSrc((KFbxPlug *)&pProperty); }
			inline bool				DisconnectSrcProperty	(KFbxProperty const & pProperty)		{ return DisconnectFbxSrc((KFbxPlug *)&pProperty);	}

			inline int				GetDstPropertyCount		()										{ return GetFbxDstCount(KFbxProperty::ClassId); }
			inline KFbxProperty		GetDstProperty			(int pIndex=0)							{ return *((KFbxProperty *)GetFbxDst(pIndex,KFbxProperty::ClassId)); }
			inline KFbxProperty		FindDstProperty			(const char *pName,int pStartIndex=0)	{ return *((KFbxProperty *)FindFbxDst(pName,KFbxProperty::ClassId,pStartIndex)); }
			inline bool				ConnectDstProperty		(KFbxProperty const & pProperty)		{ return ConnectFbxDst((KFbxPlug *)&pProperty);		}
			inline bool				IsConnectedDstProperty	(KFbxProperty const & pProperty)		{ return IsConnectedFbxDst((KFbxPlug *)&pProperty); }
			inline bool				DisconnectDstProperty	(KFbxProperty const & pProperty)		{ return DisconnectFbxDst((KFbxPlug *)&pProperty);	}
		//@}

		void SetUserDataPtr(void* pUserData);
		void* GetUserDataPtr();
	   
		/** Get a pointer to the scene containing this object.
		  * \return Return a pointer to the scene containing this object or \c NULL if the
		  * object does not belong to any scene.
		  */
		KFbxScene* GetScene();

		/** Get a const pointer to the scene containing this object.
		  * \return Return a const pointer to the scene containing this object or \c NULL if the
		  * object does not belong to any scene.
		  */
		const KFbxScene* GetScene() const;

	///////////////////////////////////////////////////////////////////////////////
	//
	//  WARNING!
	//
	//	Anything beyond these lines may not be documented accurately and is 
	// 	subject to change without notice.
	//
	///////////////////////////////////////////////////////////////////////////////

	#ifndef DOXYGEN_SHOULD_SKIP_THIS

		/** Return an array of all the namespace of the object 
		  * \return Return a \c NULL terminated string.
		  */
		KArrayTemplate<KString*> GetNameSpaceArray( char identifier ) { return mName.GetNameSpaceArray(identifier);}
		
		/** Get the name only (no namespace or prefix) of the object. 
		  * \return Return a \c NULL terminated string.
		  */
		KString GetNameOnly() const;

		bool IsModel() { return mIsModel; }
		void SetIsModel(bool state) { mIsModel = state; }

	public:

		typedef enum 
		{
			eNODE					= 0x00000001,
			eTEXTURE				= 0x00000002,
			eMATERIAL				= 0x00000003,
			eGOBO					= 0x00000004,
			eMEDIA					= 0x00000005,
			eACTOR					= 0x00000006,
			eMARKER_SET				= 0x00000007,
			eCHARACTER				= 0x00000008,
			eCHARACTER_POSE			= 0x00000009,
			eGENERIC_NODE			= 0x0000000a,
			ePOSE					= 0x0000000b,
			eCONTROLSET_PLUG		= 0x0000000c,
			eCONSTRAINT				= 0x0000000d,
			eNODEATTRIBUTE			= 0x0000000e,
			eGEOMETRY_WEIGHTED_MAP	= 0x00000010,
			eSCENE					= 0x00000011,
			eDEFORMER               = 0x00000012,
			eSUBDEFORMER            = 0x00000013,
			eGLOBAL_SETTING         = 0x00000014,
			eCACHE                  = 0x00000015,
		} ENameSpace;

		virtual ENameSpace GetNameSpace() const {return ENameSpace(0); }
		KString GetNameSpacePrefix() const;
		static char const* GetNamePrefix() { return 0; }

		static KString GetDefaultNameSpacePrefix(ENameSpace pNameSpace);
		static KString RemovePrefix(char* pName);
		static KString StripPrefix(KString lName);
		static KString StripPrefix(char* pName);

		// internal property management
		protected:
			void			RegisterProperty(KFbxProperty* pProperty);
			void			UnregisterProperty(KFbxProperty* pProperty);
			KFbxProperty*	GetProperty(KProperty* pKProperty);
			virtual void	PropertyDefaultValueChanged(KFbxProperty* pProperty);
		public:
			void			PublishProperties(); 

		// Connection management
		typedef enum 
		{
			eSURFACE_CLONE,
			eREFERENCE_CLONE,
			eDEEP_CLONE
		} ECloneType;

		// Clone
		virtual KFbxObject* Clone(KFbxObject::ECloneType pCloneType = eDEEP_CLONE) const { return 0; }



	protected:
		KFbxObject(KFbxSdkManager& pManager, char const* pName);
		virtual ~KFbxObject();

		virtual void Construct();
		virtual void Destruct(bool pRecursive, bool pDependents);

	public:
		virtual KFbxSdkManager*	GetFbxSdkManager();
		virtual KObject*		GetFbxObject_internal();
		virtual KObject const*  GetFbxObject_internal() const;
		virtual KPlug*			GetInternalPlug(bool pCreateOnDemand=false);
		virtual KPlug const*	GetInternalPlug(bool pCreateOnDemand=false) const;
		virtual kFbxClassId  GetRuntimeClassId() const;

	protected:
		virtual KFbxTakeNodeContainer* GetTakeNodeContainer();

	protected:
		KFbxObject& operator=(KFbxObject const& pObject);
		
		virtual bool			SetRuntimeClassId(kFbxClassId pClassId);
		virtual					bool FbxPlugNotify(KFbxPlugEvent const &pEvent);

		virtual KString			GetTypeName() const;
		virtual KStringList		GetTypeFlags() const;

		virtual void			PropertyAdded(KFbxProperty* pProperty);
		virtual void			PropertyRemoved(KFbxProperty* pProperty);	

		// Animation Management
		virtual void			AddChannels(KFbxTakeNode *pTakeNode);
		virtual void			UpdateChannelFromProperties(KFbxTakeNode *pTakeNode);
		
		virtual void SetScene(KFbxScene* pScene);

		KFbxObject_internal*				mInternal;
		KName								mName;
		KArrayTemplate<KFbxProperty*>		mProperties;
		void*								mUserData;
		KFbxSdkManager*						mManager;
		kFbxClassId						mRuntimeClassId;

		bool				mIsModel; 
		// normally stays at false unless while we
		// are renaming the objects (on import) we detect that this node is actually
		// a Model node.

		// friend classes for sdk access
		friend class KFbxReaderFbx;
		friend class KFbxWriterFbx6;
		friend class KFbxScene;
		friend class KFbxObject_internal;
		friend class KFbxProperty;

	#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS

	};

	typedef KFbxObject* HKFbxObject;

	// template access functions SRC
	template < class T > inline int KFbxGetSrcCount(KFbxObject const *pObject, T const* VC6Dummy = 0)								{ return pObject ? pObject->GetSrcObjectCount(T::ClassId) : 0; }
	template < class T > inline int KFbxGetSrcCount(KFbxObject const *pObject,kFbxClassId pClassId, T const* VC6Dummy = 0)		{ return pObject ? pObject->GetSrcObjectCount(pClassId) : 0;	 }
	template < class T > inline T* KFbxGetSrc(KFbxObject const *pObject,int pIndex=0)												{ return pObject ? (T *) pObject->GetSrcObject(T::ClassId,pIndex) : 0; }
	template < class T > inline T* KFbxGetSrc(KFbxObject const *pObject,int pIndex,kFbxClassId pClassId)						{ return pObject ? (T *) pObject->GetSrcObject(pClassId,pIndex) : 0;	}
	template < class T > inline T* KFbxFindSrc(KFbxObject const *pObject,char const *pName,int pIndex=0)							{ return pObject ? (T *) pObject->FindSrcObject(T::ClassId,pName,pIndex) : 0;	}
	template < class T > inline T* KFbxFindSrc(KFbxObject const *pObject,char const *pName,kFbxClassId pClassId,int pIndex=0)	{ return pObject ? (T *) pObject->FindSrcObject(pClassId,pName,pIndex) : 0;	}
	inline bool KFbxConnectSrc(KFbxObject *pDstObject,KFbxObject *pSrcObject)												{ return (pSrcObject && pDstObject) ? pDstObject->ConnectSrcObject(pSrcObject) : 0;	}
	template < class T > inline bool KFbxDisconnectAllSrc(KFbxObject *pObject,T *VC6Dummy=0)								{ return pObject->DisconnectAllSrcObject(T::ClassId);	}

	// template access functions DST
	template < class T > inline int KFbxGetDstCount(KFbxObject const *pObject, T const* VC6Dummy = 0)								{ return pObject ? pObject->GetDstObjectCount(T::ClassId) : 0; }
	template < class T > inline int KFbxGetDstCount(KFbxObject const *pObject,kFbxClassId pClassId, T const* VC6Dummy = 0)		{ return pObject ? pObject->GetDstObjectCount(pClassId) : 0;	 }
	template < class T > inline T* KFbxGetDst(KFbxObject const *pObject,int pIndex=0)												{ return pObject ? (T *) pObject->GetDstObject(T::ClassId,pIndex) : 0; }
	template < class T > inline T* KFbxGetDst(KFbxObject const *pObject,int pIndex,kFbxClassId pClassId)						{ return pObject ? (T *) pObject->GetDstObject(pClassId,pIndex) : 0;	}
	template < class T > inline T* KFbxFindDst(KFbxObject const *pObject,char const *pName,int pIndex=0)							{ return pObject ? (T *) pObject->FindDstObject(T::ClassId,pName,pIndex) : 0;	}
	template < class T > inline T* KFbxFindDst(KFbxObject const *pObject,char const *pName,kFbxClassId pClassId,int pIndex=0)	{ return pObject ? (T *) pObject->FindDstObject(pClassId,pName,pIndex) : 0;	}
	inline bool KFbxConnectDst(KFbxObject *pSrcObject,KFbxObject *pDstObject)												{ return (pSrcObject && pDstObject) ? pDstObject->ConnectSrcObject(pSrcObject) : 0;	}
	template < class T > inline bool KFbxDisconnectAllDst(KFbxObject *pObject,T *VC6Dummy=0)								{ return pObject->DisconnectAllDstObject(T::ClassId);	}

#include <fbxfilesdk_nsend.h>

#endif // #ifndef _FBXSDK_OBJECT_H_


