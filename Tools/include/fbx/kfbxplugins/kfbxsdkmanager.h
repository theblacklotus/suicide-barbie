/*!  \file kfbxsdkmanager.h
 */

#ifndef _FBXSDK_SDK_MANAGER_H_
#define _FBXSDK_SDK_MANAGER_H_

/**************************************************************************************

 Copyright © 2001 - 2006 Autodesk, Inc. and/or its licensors.
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

#include <klib/karrayul.h>
#include <klib/kerror.h>

#ifndef MB_FBXSDK
#include <kbaselib_nsuse.h>
#endif
#include <kbaselib_forward.h>

#include <kfbxplugins/kfbxmemoryallocator.h>

#include <kfbxplugins/kfbxplug.h>

#include <kfbxplugins/kfbxthumbnail.h>
#include <kfbxplugins/kfbxsceneinfo.h>
#include <kfbxplugins/kfbxgeometryconverter.h>
#include <kfbxplugins/kfbximageconverter.h>
#include <kfbxplugins/kfbxproperty.h>
#include <kfbxplugins/kfbxtypes.h>
#include <kfbxplugins/kfbxobject.h>
#include <kfbxio/kfbxexporter.h>
#include <kfbxio/kfbximporter.h>
#include <kfbxplugins/kfbxlayer.h>
#include <fbxfilesdk_nsbegin.h>

	class KFbxUserNotification;
	class KFbxObject;
	class KFbxGeometryConverter;
	class KFbxImageConverter;
	class KFbxScene;
	class KFbxExporter;
	class KFbxImporter;
	class KFbxNode;
	class KFbxSurfaceMaterial;
	class KFbxTexture;
	class KFbxVideo;
	class KFbxNodeAttribute;
	class KFbxNull;
	class KFbxMarker;
	class KFbxCamera;
	class KFbxCameraSwitcher;
	class KFbxLight;
	class KFbxMesh;
	class KFbxPatch;
	class KFbxNurb;
	class KFbxGeometryWeightedMap;
	class KFbxSkeleton;
	class KFbxOpticalReference;
	class KFbxPose;
	class KFbxThumbnail;
	class KFbxShape;
	class KFbxProperty;
	class KFbxSceneInfo;
	class KFbxGenericNode;
	class KFbxDeformer;
	class KFbxSkin;
	class KFbxSubDeformer;
	class KFbxCluster;
	class KFbxConstraint;
	class KFbxConstraintPosition;
	class KFbxConstraintScale;
	class KFbxConstraintRotation;
	class KFbxConstraintParent;
	class KFbxConstraintAim;
	class KFbxConstraintSingleChainIK;
	class KFbxLayer;
	class KFbxTakeNode;
	class KFbxCharacter;
	class KFbxCharacterPose;
	class KFbxControlSetPlug;
	class KFbxNurbsCurve;
	class KFbxTrimNurbsSurface;
	class KFbxBoundary;
	class KFbxNurbsSurface;
	class KFbxDataType;

	typedef class KFbxCluster KFbxLink;


	class KFbxSdkManager_internal;


	/** SDK object manager
	  * The SDK manager is in charge of:
	  *		- scene element allocation
	  *		- scene element deallocation
	  *		- scene element search and access
	  * 
	  * Upon destruction all objects allocated by the SDK manager and 
	  * not explicitly destroyed are destroyed as well. A derived 
	  * class can be defined to allocate and deallocate a set of 
	  * specialized scene elements.
	  * \nosubgrouping
	  */
	class KFBX_DLL KFbxSdkManager
	{

	public:
		/** 
		  * \name Memory Management.
		  */
		//@{
		
			/** SDK Memory management 
			  * Use this method to specify custom memory management routines.
			  * The FBX SDK will use the provided routines to allocate and
			  * deallocate memory.
			  * \remarks Important: If you plan to specify custom memory management
			  * routines, you must do so BEFORE creating the first SDK manager. Those
			  * routines are global and thus shared between different instances of SDK managers.
			  */
			static bool SetMemoryAllocator(KFbxMemoryAllocator* pMemoryAllocator);
		//@}


		/**
		  * \name FBX SDK Manager creation/destruction.
		  */
		//@{
			/** SDK manager allocation method.
			  *	\return A pointer to the SDK manager or \c NULL if this is an
			  * evaluation copy of the FBX SDK and it is expired.
			  */
			static KFbxSdkManager* CreateKFbxSdkManager();	

			/** Destructor.
			  *	Deallocates all object previously created by the SDK manager.
			  */
			virtual void DestroyKFbxSdkManager();
		//@}


		/**
		  * \name plug and object definition and management 
		  */
		//@{
		public:
			/** Class registration. 
			  *	\param pFBX_TYPE_Class
			  *	\param pFBX_TYPE_ParentClass
			  *	\param pName
			  *	\param pFbxFileTypeName
			  *	\param pFbxFileSubTypeName
			  *	\return The class Id of the newly register class
			  * 
			  */
			template <typename T1,typename T2> inline kFbxClassId RegisterClass(char const *pName,T1 const *pFBX_TYPE_Class,T2 const *pFBX_TYPE_ParentClass,const char *pFbxFileTypeName=0,const char *pFbxFileSubTypeName=0) {
				if (!Internal_RegisterClass(T1::ClassId)) {
					T1::ClassId  = Internal_RegisterClass(pName,T2::ClassId,(kFbxPlugConstructor)T1::SdkManagerCreate,pFbxFileTypeName,pFbxFileSubTypeName );
				}
				return T1::ClassId;
			}

			template <typename T> inline kFbxClassId	RegisterRuntimeClass(char const *pName,T const *pFBX_TYPE_ParentClass,const char *pFbxFileTypeName=0,const char *pFbxFileSubTypeName=0) {
				return Internal_RegisterClass(pName,T::ClassId,(kFbxPlugConstructor)T::SdkManagerCreate,pFbxFileTypeName,pFbxFileSubTypeName );
			}

			template <typename T1,typename T2> inline kFbxClassId OverrideClass(T1 const *pFBX_TYPE_Class,T2 const *pFBX_TYPE_OverridenClass) {
				T1::ClassId  = Internal_OverrideClass(T2::ClassId,(kFbxPlugConstructor)T1::SdkManagerCreate );
				return T1::ClassId;
			}

			KFbxPlug*		CreateClass		(kFbxClassId,char const *pName);
			kFbxClassId		FindClass		(char const *pClassName);
			kFbxClassId		FindFbxFileClass(const char *pFbxFileTypeName,const char *pFbxFileSubTypeName);

		//@}

		/**
		  * \name General Object Connection and Relationship Management
		  */
		//@{
		public:
			/** Get the number of objects of class pClassId.
			  *\return Return the number of pClassId objects owned by the manager.
			  *	\remarks the class id can be optained by calling <Class>::Classid
			  */
			int									GetSrcObjectCount(kFbxClassId pClassId);
			inline int							GetSrcObjectCount()						{ return GetSrcObjectCount(kFbxClassId()); }
			template < class T > inline	int		GetSrcObjectCount(T const *pFBX_TYPE)	{ return GetSrcObjectCount(T::ClassId); }
			/** Get the [i]th object of class pClassId.
			  *\return Return the pointer to the object owned by the manager.
			  *	\remarks the class id can be optained by calling <Class>::Classid
			  */
			KFbxObject*							GetSrcObject(kFbxClassId pClassId,int pIndex=0);
			inline KFbxObject*					GetSrcObject(int pIndex=0)							{ return GetSrcObject(kFbxClassId(),pIndex); }
			template < class T > inline T*		GetSrcObject(T const *pFBX_TYPE,int pIndex=0)		{ return (T *)GetSrcObject(T::ClassId,pIndex); }
			/** Find the object of class pClassId that has the name pName.
			  *	\param pName Name of the node.
			  *	\param pStartIndex Zero based index specifiyng where the search begins 
			  * \return Return the pointer to the object owned by the manager.
			  *	\remarks the class id can be optained by calling <Class>::Classid
			  */
			KFbxObject*							FindSrcObject(kFbxClassId pClassId,const char *pName,int pStartIndex=0);	
			inline KFbxObject*					FindSrcObject(const char *pName,int pStartIndex=0)	{ return FindSrcObject(kFbxClassId(),pName,pStartIndex); }
			template < class T > inline T*		FindSrcObject(T const *pFBX_TYPE,const char *pName,int pStartIndex=0)	{ return (T *)FindSrcObject(pName,T::ClassId,pStartIndex); }
		//@}

		/**
		  * \name Error Management
		  */
		//@{
		public:
			/** Retrieve error object.
			 *	\return Reference to the Manager's error object.
			 */
			KError& GetError();

			/** \enum EError Error codes
			  */
			typedef enum 
			{
				eOBJECT_NOT_FOUND,    /**< The requested object was not found in the Manager's database. */
				eNAME_ALREADY_IN_USE, /**< A name clash occurred.                                        */
				eERROR_COUNT          /**< Mark the end of the error enum.                               */
			} EError;
			
			/** Get last error code.
			 *	\return Last error code.
			 */
			EError GetLastErrorID();

			/** Get last error string.
			 *	\return Textual description of the last error.
			 */
			char* GetLastErrorString();

		//@}

		/** Add a prefix to a name 
		  * \param pPrefix The prefix to be added to the \c pName. This
		  * string must contain the "::" characters in order to be considered
		  * as a prefix.
		  * \param pName The name to be prefix.
		  * \return The prefixed string
		  * \remarks If a prefix already exists, it is removed before
		  * adding \c pPrefix.
		  */
		static KString PrefixName(char const* pPrefix, char const* pName);

		/**
		  * \name datatype management
		  */
		//@{
			/** Register a new datatype to the manager
			 *	\return true if it success
			 */
			KFbxDataType CreateFbxDataType(char *pName,EFbxType pType);

			/** Finds a datatype 
			 *	\return the found datatype. return null if not found
			 */
			KFbxDataType const &GetFbxDataTypeFromName(char *pDataType);
			
		//@}


		/**
		  * \name Depricated Object Creation
	      *	Create specific object of class X (X must derive from KFbxPlug),
		  *	These functions are deprecated please use Object::Create instead
		  * This section contains creation methods for the different objects
		  * of the FBX SDK. Objects of the FBX SDK can be derived. The derived
		  * object can be properly created by overwritting the corresponding
		  * virtual allocator routine. See the protected section below.
		  */
		//@{
			K_DEPRECATED KFbxScene* CreateKFbxScene();
			K_DEPRECATED KFbxNode* CreateKFbxNode(char const* pName);
			K_DEPRECATED KFbxNull* CreateKFbxNull();
			K_DEPRECATED KFbxMarker* CreateKFbxMarker();
			K_DEPRECATED KFbxCamera* CreateKFbxCamera();
			K_DEPRECATED KFbxLight* CreateKFbxLight();
			K_DEPRECATED KFbxSkeleton* CreateKFbxSkeleton();
			K_DEPRECATED KFbxMesh* CreateKFbxMesh(char const* pName = "");
			K_DEPRECATED KFbxPatch* CreateKFbxPatch(char const* pName = "");
			K_DEPRECATED KFbxNurb* CreateKFbxNurb(char const* pName = "");
			K_DEPRECATED KFbxNurbsSurface* CreateKFbxNurbsSurface(char const* pName = "");
			K_DEPRECATED KFbxNurbsCurve* CreateKFbxNurbsCurve(char const* pName = "");
			K_DEPRECATED KFbxTrimNurbsSurface* CreateKFbxTrimNurbsSurface(char const* pName = "");
			K_DEPRECATED KFbxBoundary* CreateKFbxBoundary(char const* pName = "");
			K_DEPRECATED KFbxGeometryWeightedMap* CreateKFbxGeometryWeightedMap(char const* pName = "");
			K_DEPRECATED KFbxOpticalReference* CreateKFbxOpticalReference();
			K_DEPRECATED KFbxLink* CreateKFbxLink();
			K_DEPRECATED KFbxSurfaceMaterial* CreateKFbxSurfaceMaterial(char const* pName);	
			K_DEPRECATED KFbxShape* CreateKFbxShape();
			K_DEPRECATED KFbxTexture* CreateKFbxTexture(char const* pName);
			K_DEPRECATED KFbxVideo* CreateKFbxVideo(char const* pName);
			K_DEPRECATED KFbxPose* CreateKFbxPose(char const* pName, bool pIsBindPose);
			K_DEPRECATED KFbxGenericNode* CreateKFbxGenericNode(char const* pName);
			K_DEPRECATED KFbxSkin* CreateKFbxSkin(char const* pName);
			K_DEPRECATED KFbxCluster* CreateKFbxCluster(char const* pName);
			K_DEPRECATED KFbxThumbnail* CreateKFbxThumbnail();
			K_DEPRECATED KFbxSceneInfo* CreateKFbxSceneInfo();
			K_DEPRECATED KFbxGeometryConverter* CreateKFbxGeometryConverter();
			K_DEPRECATED KFbxImageConverter* CreateKFbxImageConverter();
			K_DEPRECATED KFbxExporter* CreateKFbxExporter();
			K_DEPRECATED KFbxImporter* CreateKFbxImporter();
			K_DEPRECATED KFbxConstraint* CreateKFbxConstraint(char const* pName, int pType );
			K_DEPRECATED KFbxCharacter* CreateKFbxCharacter(char const* pName );
			K_DEPRECATED KFbxCharacterPose* CreateKFbxCharacterPose(char const* pName );
			K_DEPRECATED KFbxControlSetPlug* CreateKFbxControlSetPlug(char const* pName );


			virtual KFbxUserNotification* AllocateKFbxUserNotification(const KString& pLogFileName, const KString& pSessionDescription);
			KFbxUserNotification*		  CreateKFbxUserNotification(const KString& pLogFileName, const KString& pSessionDescription);
		//@}

		/**
		  * \name Deprecated Object Destruction
	      *	Destroy and deallocate specific object of class X (X must derive from KFbxPlug),
		  *	These functions are deprecated please use Object->Destroy() instead
		  */
		//@{
			K_DEPRECATED virtual bool DestroyKFbxScene(KFbxScene* pScene, bool pDestroyContent = true);
			K_DEPRECATED virtual bool DestroyKFbxNode(KFbxNode* pNode, bool pDestroyContent = true);
			K_DEPRECATED virtual bool DestroyKFbxNodeAttribute(KFbxNodeAttribute* pNodeAttribute);
			K_DEPRECATED virtual bool DestroyKFbxGeometryWeightedMap(KFbxGeometryWeightedMap* pGeometryWeightedMap);
			K_DEPRECATED virtual bool DestroyKFbxLink(KFbxLink* pLink);
			K_DEPRECATED virtual bool DestroyKFbxSurfaceMaterial(KFbxSurfaceMaterial* pMaterial);	
			K_DEPRECATED virtual bool DestroyKFbxShape(KFbxShape* pShape);
			K_DEPRECATED virtual bool DestroyKFbxTexture(KFbxTexture* pTexture);
			K_DEPRECATED virtual bool DestroyKFbxVideo(KFbxVideo* pVideo);
			K_DEPRECATED virtual bool DestroyKFbxPose(KFbxPose* pPose);
			K_DEPRECATED virtual bool DestroyKFbxGenericNode(KFbxGenericNode* pGenericNode);
			K_DEPRECATED virtual bool DestroyKFbxDeformer(KFbxDeformer* pDeformer);
			K_DEPRECATED virtual bool DestroyKFbxSubDeformer(KFbxSubDeformer* pSubDeformer);
			K_DEPRECATED virtual bool DestroyKFbxThumbnail(KFbxThumbnail* pThumbnail);
			K_DEPRECATED virtual bool DestroyKFbxSceneInfo(KFbxSceneInfo* pSceneInfo);
			K_DEPRECATED bool DestroyKFbxGeometryConverter(KFbxGeometryConverter* pConverter);
			K_DEPRECATED bool DestroyKFbxImageConverter(KFbxImageConverter* pConverter);
			K_DEPRECATED bool DestroyKFbxExporter(KFbxExporter* pExporter);
			K_DEPRECATED bool DestroyKFbxImporter(KFbxImporter* pImporter);
			K_DEPRECATED bool DestroyKFbxConstraint(KFbxConstraint* pConstraint);
			K_DEPRECATED bool DestroyKFbxCharacter(KFbxCharacter* pCharacter);
			K_DEPRECATED bool DestroyKFbxCharacterPose(KFbxCharacterPose* pCharacterPose);
			K_DEPRECATED bool DestroyKFbxControlSetPlug(KFbxControlSetPlug* pControlSetPlug);

			virtual bool DestroyKFbxUserNotification();

		//@}
			
		/**
		  * \name Deprecated Object Search
          * Replaced by FindObjectSrc
		  */
		//@{
		public:	
			K_DEPRECATED virtual KFbxNode* FindKFbxNode(char const* pName, int pIndex = 0);
			K_DEPRECATED virtual KFbxSurfaceMaterial* FindKFbxSurfaceMaterial(char const* pName, int pIndex = 0);
			K_DEPRECATED virtual KFbxTexture* FindKFbxTexture(char const* pName, int pIndex = 0);
			K_DEPRECATED virtual KFbxVideo* FindKFbxVideo(char const* pName, int pIndex = 0);
			K_DEPRECATED virtual KFbxPose* FindKFbxPose(char const* pName, int pIndex = 0);
			K_DEPRECATED virtual KFbxGenericNode* FindKFbxGenericNode(char const* pName, int pIndex = 0);
		//@}

		/**
		  * \name Deprecated Access to object Object Count
          * Replaced by GetObjectSrcCount and GetObjectSrc
		  */
		//@{
			K_DEPRECATED int GetKFbxNodeCount() ;
			K_DEPRECATED KFbxNode* GetKFbxNode(int pIndex) ;
			K_DEPRECATED int GetKFbxGenericNodeCount() ;
			K_DEPRECATED KFbxGenericNode* GetKFbxGenericNode(int pIndex) ;
			K_DEPRECATED int GetKFbxSurfaceMaterialCount() ;
			K_DEPRECATED KFbxSurfaceMaterial* GetKFbxSurfaceMaterial(int pIndex) ;
			K_DEPRECATED int GetKFbxTextureCount() ;
			K_DEPRECATED KFbxTexture* GetKFbxTexture(int pIndex) ;
			K_DEPRECATED int GetKFbxVideoCount() ;
			K_DEPRECATED KFbxVideo* GetKFbxVideo(int pIndex) ;
			K_DEPRECATED int GetKFbxPoseCount() ;
			K_DEPRECATED KFbxPose* GetKFbxPose(int pIndex) ;
			K_DEPRECATED int GetKFbxLinkCount() ;
			K_DEPRECATED KFbxLink* GetKFbxLink(int pIndex) ;
			K_DEPRECATED int GetKFbxConstraintCount() ;
			K_DEPRECATED int GetKFbxConstraintCount(int pType) ;
			K_DEPRECATED KFbxConstraint* GetKFbxConstraint(int pIndex, int pType) ;
			K_DEPRECATED int GetKFbxCharacterCount() ;
			K_DEPRECATED KFbxCharacter* GetKFbxCharacter(int pIndex) ;
			K_DEPRECATED int GetKFbxCharacterPoseCount() ;
			K_DEPRECATED KFbxCharacterPose* GetKFbxCharacterPose(int pIndex) ;
			K_DEPRECATED int GetKFbxControlSetPlugCount() ;
			K_DEPRECATED KFbxControlSetPlug* GetKFbxControlSetPlug(int pIndex) ;
			K_DEPRECATED int GetKFbxGeometryWeightedMapCount() ;
			K_DEPRECATED KFbxGeometryWeightedMap* GetKFbxGeometryWeightedMap(int pIndex) ;
			K_DEPRECATED int GetKFbxSceneCount() ;
			K_DEPRECATED KFbxScene* GetKFbxScene(int pIndex) ;

			/** Access to the unique UserNotification object.
			  * \return The pointer to the user notification or \c NULL \c if the object
			  * has not been allocated.
			*/
			KFbxUserNotification* GetUserNotification();
		//@}
			
			//ADDED MATERIAL
			/*	KFbxLayerElement
				KFbxLayerElementNormal
				KFbxLayerElementMaterial
				KFbxLayerElementPolygonGroup
				KFbxLayerElementUV
				KFbxLayerElementVertexColor
				KFbxLayerElementSmoothing
				KFbxLayerElementUserData
				KFbxLayerElementVisibility
				KFbxLayerElementTexture
			*/
			bool DestroyKFbxLayerElement(KFbxLayerElement* pLayerElement);
			KFbxLayerElement* CreateKFbxLayerElement(char const* pName );

			bool DestroyKFbxLayerElementNormal(KFbxLayerElementNormal* pLayerElementNormal);
			KFbxLayerElementNormal* CreateKFbxLayerElementNormal(char const* pName );

			bool DestroyKFbxLayerElementMaterial(KFbxLayerElementMaterial* pLayerElementMaterial);
			KFbxLayerElementMaterial* CreateKFbxLayerElementMaterial(char const* pName );

			bool DestroyKFbxLayerElementPolygonGroup(KFbxLayerElementPolygonGroup* pLayerElementPolygonGroup);
			KFbxLayerElementPolygonGroup* CreateKFbxLayerElementPolygonGroup(char const* pName );

			bool DestroyKFbxLayerElementUV(KFbxLayerElementUV* pLayerElementUV);
			KFbxLayerElementUV* CreateKFbxLayerElementUV(char const* pName );

			bool DestroyKFbxLayerElementVertexColor(KFbxLayerElementVertexColor* pLayerElementVertexColor);
			KFbxLayerElementVertexColor* CreateKFbxLayerElementVertexColor(char const* pName );

			bool DestroyKFbxLayerElementSmoothing(KFbxLayerElementSmoothing* pLayerElementSmoothing);
			KFbxLayerElementSmoothing* CreateKFbxLayerElementSmoothing(char const* pName );

			bool DestroyKFbxLayerElementUserData(KFbxLayerElementUserData* pLayerElementUserData);
			KFbxLayerElementUserData* CreateKFbxLayerElementUserData(char const* pName, KFbxLayerElementUserData const& pOther );
			KFbxLayerElementUserData* CreateKFbxLayerElementUserData(char const* pName, int pId, KArrayTemplate<KFbxDataType>& pDataTypes, KArrayTemplate<const char*>& pDataNames);

			bool DestroyKFbxLayerElementVisibility(KFbxLayerElementVisibility* pLayerElementVisibility);
			KFbxLayerElementVisibility* CreateKFbxLayerElementVisibility(char const* pName );

			bool DestroyKFbxLayerElementTexture(KFbxLayerElementTexture* pLayerElementTexture);
			KFbxLayerElementTexture* CreateKFbxLayerElementTexture(char const* pName );


	///////////////////////////////////////////////////////////////////////////////
	//
	//  WARNING!
	//
	//	Anything beyond these lines may not be documented accurately and is 
	// 	subject to change without notice.
	//
	///////////////////////////////////////////////////////////////////////////////

	#ifndef DOXYGEN_SHOULD_SKIP_THIS

	public:




		KFbxProperty*    CreateKFbxProperty(KProperty* pProperty);
		KFbxCameraSwitcher*  CreateKFbxCameraSwitcher();

		bool CanDestroyFbxSrcObject(KFbxObject* pObject, KFbxObject* pSrcObject, bool pRecursive, bool pDependents);

		void CreateMissingBindPoses(KFbxScene *pScene);
		int  GetBindPoseCount(KFbxScene *pScene);

		bool RegisterPlug(KFbxPlug const *pPlug);
		bool UnregisterPlug(KFbxPlug const *pPlug);

	private:
		kFbxClassId		Internal_RegisterClass	(char const *pClassName,kFbxClassId pParentClassId,kFbxPlugConstructor=0,char const* pFbxFileTypeName=0,char const* pFbxFileSubTypeName=0);
		bool			Internal_RegisterClass	(kFbxClassId	pClassId);
		kFbxClassId		Internal_OverrideClass	(kFbxClassId pClassId,kFbxPlugConstructor=0);


	#ifdef K_FBXNEWRENAMING
		
		enum ERenamingMode
		{ 
			eMAYA_TO_FBX,
			eFBX_TO_MAYA,
			eLW_TO_FBX,
			eFBX_TO_LW,
			eXSI_TO_FBX,
			eFBX_TO_XSI,
			eMAX_TO_FBX,
			eFBX_TO_MAX,
			eMB_TO_FBX,
			eFBX_TO_MB
		};

		void RenameFor(ERenamingMode pMode);

		/** Rename all object to remove name clashing.
		 * \param pFromFbx Index of the requested KFbxConstraint.
		 * \param pIgnoreNS Index of the requested KFbxConstraint.
		 * \param pIsCaseSensitive .
		 * \param pComvertToWideChar Needs to ba converted to widechar.
		 * \param pReplaceNonAlphaNum replace non alpha character.
		 * \param pNameSpaceSymbol identifier of a namespace.
		 *	\return void.
		 */
		void ResolveNameClashing(bool pFromFbx, bool pIgnoreNS, bool pIsCaseSensitive, bool pComvertToWideChar, bool pReplaceNonAlphaNum, KString pNameSpaceSymbol);

	#endif


	protected:

		KFbxSdkManager();	
		virtual ~KFbxSdkManager();


		/*ADDED template array for
		KFbxLayerElement
		KFbxLayerElementNormal
		KFbxLayerElementMaterial
		KFbxLayerElementPolygonGroup
		KFbxLayerElementUV
		KFbxLayerElementVertexColor
		KFbxLayerElementSmoothing
		KFbxLayerElementUserData
		KFbxLayerElementVisibility
		KFbxLayerElementTexture
		*/

		KArrayTemplate <KFbxLayerElement *> mLayerElement;
		KArrayTemplate <KFbxLayerElementNormal *> mLayerElementNormal;
		KArrayTemplate <KFbxLayerElementMaterial *> mLayerElementMaterial;
		KArrayTemplate <KFbxLayerElementPolygonGroup *> mLayerElementPolygonGroup;
		KArrayTemplate <KFbxLayerElementUV *> mLayerElementUV;
		KArrayTemplate <KFbxLayerElementVertexColor *> mLayerElementVertexColor;
		KArrayTemplate <KFbxLayerElementSmoothing *> mLayerElementSmoothing ;
		KArrayTemplate <KFbxLayerElementUserData *> mLayerElementUserData;
		KArrayTemplate <KFbxLayerElementVisibility *> mLayerElementVisibility;
		KArrayTemplate <KFbxLayerElementTexture *> mLayerElementTexture;



		


		//! Clear the manager of all objects it has created.
		virtual void Clear();

		// this object is destroyed when the manager is destroyed.
		KFbxUserNotification*				   mUserNotification;

		KObjectList*						   mObjectArray;

		// The KRenamingStrategy pointers may be set by derived class
		// By default, they are null and not used.
		KRenamingStrategy* mNodeRenamingStrategy;
		KRenamingStrategy* mMaterialRenamingStrategy;
		KRenamingStrategy* mTextureRenamingStrategy;
		KRenamingStrategy* mVideoRenamingStrategy;

		mutable KError mError;

		friend class KFbxReaderFbx;
		friend class KFbxReaderFbx6;
		friend class KFbxWriterFbx;
		friend class KFbxWriterFbx6;
		friend class KFbxPose;

		// Class Management
		bool ClassInit();
		bool ClassRelease();

		private:
			KFbxSdkManager_internal*	mInternal;

	#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS
		
	};

	typedef KFbxSdkManager* HKFbxSdkManager;

	// Connection management
	template < class T > inline int KFbxGetSrcCount(KFbxSdkManager *pObject, T const* VC6Dummy = 0)								{ return pObject ? pObject->GetSrcObjectCount(T::ClassId) : 0; }
	template < class T > inline int KFbxGetSrcCount(KFbxSdkManager *pObject,kFbxClassId pClassId, T const* VC6Dummy = 0)		{ return pObject ? pObject->GetSrcObjectCount(pClassId) : 0;	 }
	template < class T > inline T*	KFbxGetSrc(KFbxSdkManager *pObject,int pIndex=0)												{ return pObject ? (T *) pObject->GetSrcObject(T::ClassId,pIndex) : 0; }
	template < class T > inline T*	KFbxGetSrc(KFbxSdkManager *pObject,int pIndex,kFbxClassId pClassId)						{ return pObject ? (T *) pObject->GetSrcObject(pClassId,pIndex) : 0;	}
	template < class T > inline T*	KFbxFindSrc(KFbxSdkManager *pObject,char const *pName,int pIndex=0)							{ return pObject ? (T *) pObject->FindSrcObject(T::ClassId,pName,pIndex) : 0;	}
	template < class T > inline T*	KFbxFindSrc(KFbxSdkManager *pObject,char const *pName,kFbxClassId pClassId,int pIndex=0)	{ return pObject ? (T *) pObject->FindSrcObject(pClassId,pName,pIndex) : 0;	}
	
	// Class Registration management

#include <fbxfilesdk_nsend.h>

#endif // #ifndef _FBXSDK_SDK_MANAGER_H_


