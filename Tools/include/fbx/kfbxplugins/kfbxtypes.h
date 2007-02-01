/*!  \file kfbxtypes.h
 */

#ifndef _KFbxTypes_h
#define _KFbxTypes_h

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

#ifndef MB_FBXSDK
#include <kbaselib_nsuse.h>
#endif
#include <kbaselib_forward.h>
#include <klib/kdebug.h>
#include <klib/kstring.h>

#include <kfbxplugins/kfbxsymbol.h>
#include <kfbxplugins/kfbxdatastream.h>
#include <kfbxplugins/kfbxcolor.h>

#include <fbxfilesdk_nsbegin.h>

	class 	KFbxSdkManager;
	class	KFbxTypeInfo_internal;

	// Type definitions
	enum EFbxType {
		eUNIDENTIFIED,
		eBOOL1,
		eINTEGER1,
		eFLOAT1,
		eDOUBLE1,
		eDOUBLE3,
		eDOUBLE4,
		eDOUBLE44,
		eENUM,
		eSTRING,
		eTIME,
		eREFERENCE,  // used as a port entry to reference object or properties
		eDATASTREAM,  // used as a port entry to reference object or properties
		eMAX_TYPES
	};

	// Base Type for vectors
	template<class T> class fbxVectorTemplate3 {
		public:
			inline fbxVectorTemplate3()							 { *this = 0;  }
			inline fbxVectorTemplate3(T pData0,T pData1,T pData2) { mData[0] = pData0; mData[1] = pData1; mData[2] = pData2;  }
			inline ~fbxVectorTemplate3() {}
		public:
			inline T &operator[](int pIndex) { return mData[pIndex]; }
			inline T const&operator[](int pIndex) const { return mData[pIndex]; }
			inline fbxVectorTemplate3<T> &operator=(int pValue) { mData[0] = pValue; mData[1] = pValue; mData[2] = pValue; return *this; }
	inline bool operator==(fbxVectorTemplate3<T> const & pRHS) const { return ((mData[0] == pRHS.mData[0]) && (mData[1] == pRHS.mData[1]) && (mData[2] == pRHS.mData[2])); }

		private:
			T mData[3];
	};

	template<class T> class fbxVectorTemplate4 {
		public:
			inline fbxVectorTemplate4()										{ *this = 0; }
			inline fbxVectorTemplate4(T pData0,T pData1,T pData2,T pData3)	{ mData[0] = pData0; mData[1] = pData1; mData[2] = pData2; mData[3] = pData3;   }
			inline ~fbxVectorTemplate4() {}
		public:
			inline T &operator[](int pIndex) { return mData[pIndex]; }
			inline T const&operator[](int pIndex) const { return mData[pIndex]; }
			inline fbxVectorTemplate4<T> &operator=(int pValue) { mData[0] = pValue; mData[1] = pValue; mData[2] = pValue; mData[3] = pValue; return *this; }
	inline bool operator==(fbxVectorTemplate4<T> const & pRHS) const { return ((mData[0] == pRHS.mData[0]) && (mData[1] == pRHS.mData[1]) && (mData[2] == pRHS.mData[2]) && (mData[3] == pRHS.mData[3])); }

		private:
			T mData[4];
	};

	// Correcponding C Types
	typedef bool								fbxBool1;
	typedef int									fbxInteger1;
	typedef float								fbxFloat1;
	typedef double								fbxDouble1;
	typedef fbxVectorTemplate3<double>			fbxDouble3;
	typedef fbxVectorTemplate4<double>			fbxDouble4;
	typedef fbxVectorTemplate4<fbxDouble4>		fbxDouble44;
	typedef int									fbxEnum;
	typedef KString								fbxString;
	typedef KTime								fbxTime;
	class										fbxReference {}; // void type for references
	typedef KFbxDataStream						fbxDataStream;

	// Type memory footprint
	const size_t FbxTypeSizeOf[eMAX_TYPES] = {
		0,
		sizeof(fbxBool1),
		sizeof(fbxInteger1),
		sizeof(fbxFloat1),
		sizeof(fbxDouble1),
		sizeof(fbxDouble3),
		sizeof(fbxDouble4),
		sizeof(fbxDouble44),
		sizeof(fbxEnum),
		sizeof(fbxString),
		sizeof(fbxTime),
		sizeof(fbxReference),
		sizeof(fbxDataStream)
	};

	// Type management for properties
	template <class T> inline EFbxType FbxTypeOf( T const &pItem ) { K_ASSERT_MSG_NOW("Unknown type" );  return eUNIDENTIFIED; }

	inline EFbxType FbxTypeOf( fbxBool1 const &pItem )		{ return eBOOL1;	}
	inline EFbxType FbxTypeOf( fbxInteger1 const &pItem )	{ return eINTEGER1; }
	inline EFbxType FbxTypeOf( fbxFloat1 const &pItem )		{ return eFLOAT1;	}
	inline EFbxType FbxTypeOf( fbxDouble1 const &pItem )		{ return eDOUBLE1;	}
	inline EFbxType FbxTypeOf( fbxDouble3 const &pItem )		{ return eDOUBLE3;	}
	inline EFbxType FbxTypeOf( fbxDouble4 const &pItem )		{ return eDOUBLE4;	}
	inline EFbxType FbxTypeOf( fbxDouble44 const &pItem )	{ return eDOUBLE44; }
//	inline EFbxType FbxTypeOf( fbxEnum const &pItem )		{ return eENUM; }
	inline EFbxType FbxTypeOf( fbxString const &pItem )		{ return eSTRING;   }
	inline EFbxType FbxTypeOf( fbxTime	const &pItem )		{ return eTIME;		 }
	inline EFbxType FbxTypeOf( fbxReference const &pItem )	{ return eREFERENCE; }
	inline EFbxType FbxTypeOf( fbxDataStream const &pItem )	{ return eDATASTREAM; }
	// Additional types
	inline EFbxType FbxTypeOf( KFbxColor	const &pItem )	{ return eDOUBLE4; }

	// internal type management
	template< class T1,class T2 > inline bool internalfbxCopy2( T1 &pDst,T2 const &pSrc ) { pDst[0] = (T1)pSrc[0]; pDst[1] = (T1)pSrc[1]; return true; }
	template< class T1,class T2 > inline bool internalfbxCopy3( T1 &pDst,T2 const &pSrc ) { pDst[0] = (T1)pSrc[0]; pDst[1] = (T1)pSrc[1]; pDst[2] = (T1)pSrc[2]; return true; }
	template< class T1,class T2 > inline bool internalfbxCopy4( T1 &pDst,T2 const &pSrc ) { pDst[0] = (T1)pSrc[0]; pDst[1] = (T1)pSrc[1]; pDst[2] = (T1)pSrc[2]; pDst[3] = (T1)pSrc[3]; return true; }

	// Copy types and conversions
	template< class T1,class T2 > inline bool fbxCopy ( T1 &pDst,T2 const &pSrc ) { K_ASSERT_MSG_NOW("Incompatible type assigment" ); return false; }

	inline bool fbxCopy( fbxBool1 &pDst,fbxBool1 const &pSrc )			{ pDst=pSrc; return true; }
	inline bool fbxCopy( fbxInteger1 &pDst,fbxInteger1 const &pSrc )	{ pDst=pSrc; return true; }
	inline bool fbxCopy( fbxFloat1 &pDst,fbxFloat1 const &pSrc )		{ pDst=pSrc; return true; }
	inline bool fbxCopy( fbxDouble1 &pDst,fbxDouble1 const &pSrc )		{ pDst=pSrc; return true; }
	inline bool fbxCopy( fbxDouble3 &pDst,fbxDouble3 const &pSrc )		{ pDst=pSrc; return true; }
	inline bool fbxCopy( fbxDouble4 &pDst,fbxDouble4 const &pSrc )		{ pDst=pSrc; return true; }
	inline bool fbxCopy( fbxDouble44 &pDst,fbxDouble44 const &pSrc )	{ pDst=pSrc; return true; }
//	inline bool fbxCopy( fbxEnum &pDst,fbxEnum const &pSrc )			{ pDst=pSrc; return true; }
	inline bool fbxCopy( fbxString &pDst,fbxString const &pSrc )		{ pDst=pSrc; return true; }
	inline bool fbxCopy( fbxTime &pDst,fbxTime const &pSrc )			{ pDst=pSrc; return true; }
	inline bool fbxCopy( fbxReference &pDst,fbxReference const &pSrc )	{ pDst=pSrc; return true; }
	inline bool fbxCopy( fbxDataStream &pDst,fbxDataStream const &pSrc ){ pDst=pSrc; return true; }

	template< class T > inline bool fbxCopy( T &pDst,void const *pSrc,EFbxType pSrcType)
	{
		switch (pSrcType) {
			case eBOOL1:	return fbxCopy( pDst,*(fbxBool1 *)pSrc );
			case eINTEGER1:	return fbxCopy( pDst,*(fbxInteger1 *)pSrc );
			case eFLOAT1:	return fbxCopy( pDst,*(fbxFloat1 *)pSrc );
			case eDOUBLE1:	return fbxCopy( pDst,*(fbxDouble1 *)pSrc );
			case eDOUBLE3:	return fbxCopy( pDst,*(fbxDouble3 *)pSrc );
			case eDOUBLE4:	return fbxCopy( pDst,*(fbxDouble4 *)pSrc );
			case eDOUBLE44:	return fbxCopy( pDst,*(fbxDouble44 *)pSrc );
			case eENUM:		return fbxCopy( pDst,*(fbxEnum *)pSrc );
			case eSTRING:	return fbxCopy( pDst,*(fbxString *)pSrc );
			case eTIME:		return fbxCopy( pDst,*(fbxTime *)pSrc );
			case eREFERENCE:		
				K_ASSERT_MSG_NOW("Trying to set value on a void Reference type" );
			break;
			case eDATASTREAM:	return fbxCopy( pDst,*(fbxDataStream *)pSrc );
			default:
				K_ASSERT_MSG_NOW("Trying to assign an unknown type" );
		}
		return false;
	}

	template< class T > inline bool fbxCopy( void *pDst,EFbxType pDstType,T const &pSrc)
	{
		switch (pDstType) {
			case eBOOL1:	return fbxCopy( *(fbxBool1 *)pDst,pSrc    );
			case eINTEGER1:	return fbxCopy( *(fbxInteger1 *)pDst,pSrc );
			case eFLOAT1:	return fbxCopy( *(fbxFloat1 *)pDst,pSrc   );
			case eDOUBLE1:	return fbxCopy( *(fbxDouble1 *)pDst,pSrc  );
			case eDOUBLE3:	return fbxCopy( *(fbxDouble3 *)pDst,pSrc  );
			case eDOUBLE4:	return fbxCopy( *(fbxDouble4 *)pDst,pSrc  );
			case eDOUBLE44:	return fbxCopy( *(fbxDouble44 *)pDst,pSrc );
			case eENUM:		return fbxCopy( *(fbxEnum *)pDst,pSrc	  );
			case eSTRING:	return fbxCopy( *(fbxString *)pDst,pSrc   );
			case eTIME:		return fbxCopy( *(fbxTime *)pDst,pSrc   );
			case eREFERENCE:		
				K_ASSERT_MSG_NOW("Trying to set value on a void Reference type" );
			break;
			case eDATASTREAM:	return fbxCopy( *(fbxDataStream *)pDst,pSrc   );
			default:
				K_ASSERT_MSG_NOW("Trying to assign an unknown type" );
		}
		return false;
	}

	inline bool fbxCopy( void *pDst,EFbxType pDstType,void const *pSrc,EFbxType pSrcType)
	{
		switch (pDstType) {
			case eBOOL1:	return fbxCopy( pDst,pDstType,*(fbxBool1 *)pSrc );
			case eINTEGER1:	return fbxCopy( pDst,pDstType,*(fbxInteger1 *)pSrc );
			case eFLOAT1:	return fbxCopy( pDst,pDstType,*(fbxFloat1 *)pSrc );
			case eDOUBLE1:	return fbxCopy( pDst,pDstType,*(fbxDouble1 *)pSrc );
			case eDOUBLE3:	return fbxCopy( pDst,pDstType,*(fbxDouble3 *)pSrc );
			case eDOUBLE4:	return fbxCopy( pDst,pDstType,*(fbxDouble4 *)pSrc );
			case eDOUBLE44:	return fbxCopy( pDst,pDstType,*(fbxDouble44 *)pSrc );
			case eENUM:		return fbxCopy( pDst,pDstType,*(fbxEnum *)pSrc );
			case eSTRING:	return fbxCopy( pDst,pDstType,*(fbxString *)pSrc);
			case eTIME:		return fbxCopy( pDst,pDstType,*(fbxTime*)pSrc);
			case eREFERENCE:		
				K_ASSERT_MSG_NOW("Trying to set value on a void Reference type" );
			break;
			case eDATASTREAM:	return fbxCopy( pDst,pDstType,*(fbxDataStream*)pSrc);
			default:
				K_ASSERT_MSG_NOW("Trying to assign an unknown type" );
		}
		return false;
	}

	class KFBX_DLL KFbxDataType {
		public:
			KFbxDataType();
			KFbxDataType( KFbxDataType const &pDataType );
			~KFbxDataType();
		private:
			KFbxDataType(char *pName,EFbxType pType);
		public:
			inline KFbxDataType& operator = (const KFbxDataType	&pDataType)	{ mData=pDataType.mData; return *this; }
			inline bool operator == (const KFbxDataType& pDataType) const	{ return mData==pDataType.mData;	}
			inline bool operator != (const KFbxDataType& pDataType) const	{ return mData!=pDataType.mData;	}

		public:
			inline bool		Valid() const { return mData ? true : false; } 
			EFbxType		GetType() const;
			char*			GetName() const;
			inline bool		Is(const KFbxDataType& pDataType) const { return mData==pDataType.mData; }
			
		private:
			KFbxTypeInfo_internal	*mData;

		friend class KFbxSdkManager;
	};

	// Default Basic Types
	extern KFbxDataType	DTNone;			
	extern KFbxDataType	DTBool;			
	extern KFbxDataType	DTInteger;		
	extern KFbxDataType	DTFloat;
	extern KFbxDataType	DTDouble;
	extern KFbxDataType	DTDouble3;
	extern KFbxDataType	DTDouble4;
	extern KFbxDataType	DTDouble44;
	extern KFbxDataType	DTEnum;
	extern KFbxDataType	DTStringList;	// ?
	extern KFbxDataType	DTString;
	extern KFbxDataType	DTCharPtr;		// ?
	extern KFbxDataType	DTTime;			
	extern KFbxDataType	DTReference;
	extern KFbxDataType	DTDataStream;

	// MB Specific datatypes
	extern KFbxDataType	DTAction;
	extern KFbxDataType	DTEvent;

	// Specialised reference Properties
	extern KFbxDataType	DTReferenceObject;
	extern KFbxDataType	DTReferenceProperty;

	// Extended sub types 
	extern KFbxDataType	DTColor3;
	extern KFbxDataType	DTColor4;

	// Support for older datatypes
    extern KFbxDataType	DTReal; 
    extern KFbxDataType	DTVector3D; 
    extern KFbxDataType	DTVector4D; 

	// Tranforms Types
	extern KFbxDataType	DTTranslation;		
	extern KFbxDataType	DTRotation;			
	extern KFbxDataType	DTScaling;			
	extern KFbxDataType	DTQuaternion;		

	extern KFbxDataType	DTLocalTranslation;	
	extern KFbxDataType	DTLocalRotation;	
	extern KFbxDataType	DTLocalScaling;		
	extern KFbxDataType	DTLocalQuaternion;	

	extern KFbxDataType	DTTransformMatrix;	
	extern KFbxDataType	DTTranslationMatrix;	
	extern KFbxDataType	DTRotationMatrix;	
	extern KFbxDataType	DTScalingMatrix;		

// Material-related types
extern KFbxDataType DTMaterialEmissive;
extern KFbxDataType DTMaterialEmissiveFactor;
extern KFbxDataType DTMaterialAmbient;
extern KFbxDataType DTMaterialAmbientFactor;
extern KFbxDataType DTMaterialDiffuse;
extern KFbxDataType DTMaterialDiffuseFactor;
extern KFbxDataType DTMaterialBump;
extern KFbxDataType DTMaterialNormalMap;
extern KFbxDataType DTMaterialTransparentColor;
extern KFbxDataType DTMaterialTransparencyFactor;
extern KFbxDataType DTMaterialSpecular;
extern KFbxDataType DTMaterialSpecularFactor;
extern KFbxDataType DTMaterialShininess;
extern KFbxDataType DTMaterialReflection;
extern KFbxDataType DTMaterialReflectionFactor;

	// Motipn Builder Specialised Types
	extern KFbxDataType	DTIntensity;			
	extern KFbxDataType	DTConeAngle;			
	extern KFbxDataType	DTFog;				
	extern KFbxDataType	DTShape;		
	extern KFbxDataType	DTFieldOfView;		
	extern KFbxDataType	DTFieldOfViewX;		
	extern KFbxDataType	DTFieldOfViewY;		
	extern KFbxDataType	DTOpticalCenterX;	
	extern KFbxDataType	DTOpticalCenterY;	

	extern KFbxDataType	DTRoll;				
	extern KFbxDataType	DTCameraIndex;		
	extern KFbxDataType	DTTimeWarp;			
	extern KFbxDataType	DTVisibility;		

	extern KFbxDataType	DTTranslationUV;		
	extern KFbxDataType	DTScalingUV;			
	extern KFbxDataType	DTHSB;				

	extern KFbxDataType	DTOrientation;		
	extern KFbxDataType	DTLookAt;			

	extern KFbxDataType	DTOcclusion;
	extern KFbxDataType	DTWeight;

	extern KFbxDataType	DTIKReachTranslation;
	extern KFbxDataType	DTIKReachRotation;	

	KFbxDataType const & GetFbxDataType(EFbxType pType);

	// Internal use
	bool				KFbxTypesInit	(KFbxSdkManager *pManager);
	void				KFbxTypesRelease(KFbxSdkManager *pManager);

	char*				FbxDataTypeToFormatPropertyType( KFbxDataType const &pDataType );
	KFbxDataType const &FormatPropertyTypeToFbxDataType( char *pDataTypeName );



#include <fbxfilesdk_nsend.h>

#endif // _KFbxTypes_h


