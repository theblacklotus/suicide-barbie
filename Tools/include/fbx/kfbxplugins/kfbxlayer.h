/*!  \file kfbxlayer.h
 */
 
#ifndef _FBXSDK_LAYER_H_
#define _FBXSDK_LAYER_H_

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

#include <klib/kstring.h>
#include <klib/karrayul.h>
#include <klib/kdebug.h>

#ifndef MB_FBXSDK
#include <kbaselib_nsuse.h>
#endif

#include <kfbxmath/kfbxvector2.h>
#include <kfbxmath/kfbxvector4.h>
#include <kfbxplugins/kfbxcolor.h>
#include <kfbxplugins/kfbxtypes.h>

#include <fbxfilesdk_nsbegin.h>

class KFbxTexture;
class KFbxSurfaceMaterial;
class KFbxSdkManager;

class KFbxGeometryBase;
class KFbxLayer;
class KFbxLayerContainer;
class KFbxMesh;
class KFbxReader3DS;
class KFbxGeometryConverter;
class KFbxReaderFbx;
class KFbxReaderFbx6;
class KFbxReaderObj;
//class KFbxReaderCollada;

/** \brief KFbxLayerElement is the base class for Layer Elements. 
  * It describes how a Layer Element is mapped on a geometry surface and how the 
  * mapping information is arranged in memory.
  * \nosubgrouping
  */
class KFBX_DLL KFbxLayerElement
{

public:

	/** \enum ELayerElementType     Type identifier for Layer Elements.
	  * - \e eUNDEFINED                        Undefined Layer Element class.
	  * - \e eNORMAL                           Layer Element of type KFbxLayerElementNormal.
	  * - \e eMATERIAL                         Layer Element of type KFbxLayerElementMaterial.
	  * - \e eDIFFUSE_TEXTURES                 Layer Element of type KFbxLayerElementTexture.
	  * - \e ePOLYGON_GROUP                    Layer Element of type KFbxLayerElementPolygonGroup.
	  * - \e eUV                               Layer Element of type KFbxLayerElementUV.
	  * - \e eVERTEX_COLOR                     Layer Element of type KFbxLayerElementVertexColor.
	  * - \e eSMOOTHING                        Layer Element of type KFbxLayerElementSmoothing.
	  * - \e eUSER_DATA                        Layer Element of type KFbxLayerElementUserData.
	  * - \e eVISIBILITY                       Layer Element of type KFbxLayerElementVisibility.
	  * - \e eEMISSIVE_TEXTURES                Layer Element of type KFbxLayerElementTexture.
	  * - \e eEMISSIVE_FACTOR_TEXTURES         Layer Element of type KFbxLayerElementTexture.
	  * - \e eAMBIENT_TEXTURES                 Layer Element of type KFbxLayerElementTexture.
	  * - \e eAMBIENT_FACTOR_TEXTURES          Layer Element of type KFbxLayerElementTexture.
	  * - \e eDIFFUSE_FACTOR_TEXTURES          Layer Element of type KFbxLayerElementTexture.
	  * - \e eSPECULAR_TEXTURES                Layer Element of type KFbxLayerElementTexture.
	  * - \e eNORMALMAP_TEXTURES               Layer Element of type KFbxLayerElementTexture.
	  * - \e eSPECULAR_FACTOR_TEXTURES         Layer Element of type KFbxLayerElementTexture.
	  * - \e eSHININESS_TEXTURES               Layer Element of type KFbxLayerElementTexture.
	  * - \e eBUMP_TEXTURES                    Layer Element of type KFbxLayerElementTexture.
	  * - \e eTRANSPARENT_TEXTURES             Layer Element of type KFbxLayerElementTexture.
	  * - \e eTRANSPARENCY_FACTOR_TEXTURES     Layer Element of type KFbxLayerElementTexture.
	  * - \e eREFLECTION_TEXTURES              Layer Element of type KFbxLayerElementTexture.
	  * - \e eREFLECTION_FACTOR_TEXTURES       Layer Element of type KFbxLayerElementTexture.
	  * - \e eLAST_ELEMENT_TYPE
	  */
	typedef enum 
	{
		eUNDEFINED,
		eNORMAL,
		eMATERIAL,
		eDIFFUSE_TEXTURES,
		ePOLYGON_GROUP,
		eUV,
		eVERTEX_COLOR,
		eSMOOTHING,
		eUSER_DATA,
		eVISIBILITY,
		eEMISSIVE_TEXTURES,
		eEMISSIVE_FACTOR_TEXTURES,
		eAMBIENT_TEXTURES,
		eAMBIENT_FACTOR_TEXTURES,
		eDIFFUSE_FACTOR_TEXTURES,
		eSPECULAR_TEXTURES,
		eNORMALMAP_TEXTURES,
		eSPECULAR_FACTOR_TEXTURES,
		eSHININESS_TEXTURES,
		eBUMP_TEXTURES,
		eTRANSPARENT_TEXTURES,
		eTRANSPARENCY_FACTOR_TEXTURES,
		eREFLECTION_TEXTURES,
		eREFLECTION_FACTOR_TEXTURES,
		eLAST_ELEMENT_TYPE
	} ELayerElementType;

	/**	\enum EMappingMode     Determine how the element is mapped on a surface.
	  * - \e eNONE                  The mapping is undetermined.
	  * - \e eBY_CONTROL_POINT      There will be one mapping coordinate for each surface control point/vertex.
	  * - \e eBY_POLYGON_VERTEX     There will be one mapping coordinate for each vertex, for each polygon it is part of.
	                                This means that a vertex will have as many mapping coordinates as polygons it is part of.
	  * - \e eBY_POLYGON            There can be only one mapping coordinate for the whole polygon.
	  * - \e eBY_EDGE               There will be one mapping coordinate for each unique edge in the mesh.
	                                This is meant to be used with smoothing layer elements.
	  * - \e eALL_SAME              There can be only one mapping coordinate for the whole surface.
	  */
	typedef enum 
	{
		eNONE,
		eBY_CONTROL_POINT,
		eBY_POLYGON_VERTEX,
		eBY_POLYGON,
		eBY_EDGE,
		eALL_SAME
	} EMappingMode;

	/** \enum EReferenceMode     Determine how the mapping information is stored in the array of coordinate.
	  * - \e eDIRECT              This indicates that the mapping information for the n'th element is found in the n'th place of 
	                              KFbxLayerElementTemplate::mDirectArray.
	  * - \e eINDEX,              This symbol is kept for backward compatibility with FBX v5.0 files. In FBX v6.0 and higher, 
	                              this symbol is replaced with eINDEX_TO_DIRECT.
	  * - \e eINDEX_TO_DIRECT     This indicates that the KFbxLayerElementTemplate::mIndexArray
	                              contains, for the n'th element, an index in the KFbxLayerElementTemplate::mDirectArray
	                              array of mapping elements. eINDEX_TO_DIRECT is usually useful to store coordinates
	                              for eBY_POLYGON_VERTEX mapping mode elements. Since the same coordinates are usually
	                              repeated a large number of times, it saves spaces to store the coordinate only one time
	                              and refer to them with an index. Materials and Textures are also referenced with this
	                              mode and the actual Material/Texture can be accessed via the KFbxLayerElementTemplate::mDirectArray
	  */
	typedef enum 
	{
		eDIRECT,
		eINDEX,
		eINDEX_TO_DIRECT
	} EReferenceMode;

	
	/** Set the Mapping Mode
	  * \param pMappingMode     Specify the way the layer element is mapped on a surface.
	  */
	void SetMappingMode(EMappingMode pMappingMode) { mMappingMode = pMappingMode; }

	/** Set the Reference Mode
	  * \param pMappingMode     Specify how the mapping information is arraged in memory.
	  */
	void SetReferenceMode(EReferenceMode pReferenceMode) { mReferenceMode = pReferenceMode; }

	/** Get the Mapping Mode
	  * \return     The current Mapping Mode.
	  */
	EMappingMode GetMappingMode() { return mMappingMode; }

	/** Get the Reference Mode
	  * \return     The current Reference Mode.
	  */
	EReferenceMode GetReferenceMode() { return mReferenceMode; }

	/** Set the name of this object
	  * \param pName     Specify the name of this LayerElement object.
	  */
	void SetName(const char* pName) { mName = KString(pName); }

	/** Get the name of this object
	  * \return     The current name of this LayerElement object.
	  */
	const char* GetName() const { return ((KFbxLayerElement*)this)->mName.GetBuffer(); }


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
	KFbxLayerElement() : mMappingMode(eNONE), mReferenceMode(eDIRECT), mName("") {}
	EMappingMode mMappingMode;
	EReferenceMode mReferenceMode;
	KString mName;
	friend class KFbxSdkManager;
#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS
};


/** This class complements the KFbxLayerElement class.
  * \nosubgrouping
  */
template <class Type> class KFbxLayerElementTemplate : public KFbxLayerElement
{

public:
	/** Access the array of Layer Elements.
	  * \return      A reference to the Layer Elements array.
	  * \remarks     You cannot put elements in the direct array when the mapping mode is set to eINDEX.
	  */
	KArrayTemplate<Type>& GetDirectArray() 
	{ 
		K_ASSERT(mReferenceMode == KFbxLayerElement::eDIRECT || mReferenceMode == KFbxLayerElement::eINDEX_TO_DIRECT);
		return mDirectArray; 
	}

	/** Access the array of index.
	  * \return      A reference to the index array.
	  * \remarks     You cannot put elements in the index array when the mapping mode is set to eDIRECT.
	  */
	KArrayTemplate<int>& GetIndexArray() 
	{ 
		K_ASSERT(mReferenceMode == KFbxLayerElement::eINDEX || mReferenceMode == KFbxLayerElement::eINDEX_TO_DIRECT);
		return mIndexArray; 
	}

	/** Remove all elements from the direct and the index arrays.
	  */
	void Clear()
	{
		mDirectArray.Clear();
		mIndexArray.Clear();
	}

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
	KFbxLayerElementTemplate(){}
	KArrayTemplate<Type> mDirectArray;
	KArrayTemplate<int> mIndexArray;
	//friend class KFbxReaderCollada;
	//friend class KFbxGeometryBase;
	//friend class KFbxLayer;
	//friend class KFbxMesh;
	//friend class KFbxReader3DS;
	//friend class KFbxGeometryConverter;
	//friend class KFbxReaderFbx;
	//friend class KFbxReaderFbx6;
	//friend class KFbxReaderObj;
	friend class KFbxSdkManager;
#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS
};

/** \brief Layer to map Normals on a geometry.
  * \nosubgrouping
  */
typedef class KFBX_DLL KFbxLayerElementTemplate<KFbxVector4>   KFbxLayerElementNormal;

/** \brief Layer to map Materials on a geometry.
  * \nosubgrouping
  */
typedef class KFBX_DLL KFbxLayerElementTemplate<KFbxSurfaceMaterial*> KFbxLayerElementMaterial;

/** \brief Layer to group related polygons together.
  * \nosubgrouping
  */
typedef class KFBX_DLL KFbxLayerElementTemplate<int>          KFbxLayerElementPolygonGroup;

/** \brief Layer to map UVs on a geometry.
  * \nosubgrouping
  */
typedef class KFBX_DLL KFbxLayerElementTemplate<KFbxVector2>   KFbxLayerElementUV;

/** \brief Layer to map Vertex Colors on a geometry.
  * \nosubgrouping
  */
typedef class KFBX_DLL KFbxLayerElementTemplate<KFbxColor>	   KFbxLayerElementVertexColor;

	/** Flag to indicate no direct array mapping for an index array element
	 */
const int kNoMapping = -1;

/** \brief Layer to map custom user data on a geometry. 
  * This layer differs from the other types of layers in that there are multiple direct arrays. There is one array for each user data attribute.
  * Each array is indexed by the index array.
  * \nosubgrouping
  */
class KFbxLayerElementUserData;

template <class T> inline KArrayTemplate<T>& KFbxGetDirectArray( KFbxLayerElementUserData *pLayerElement,int pIndex, bool* pStatus = NULL);
template <class T> inline KArrayTemplate<T>& KFbxGetDirectArray( KFbxLayerElementUserData *pLayerElement,const char* pName, bool* pStatus = NULL );

class KFBX_DLL KFbxLayerElementUserData : public KFbxLayerElementTemplate<void*>
{
public:

	/* Flag to indicate no direct array mapping for an index array element
	 */




	~KFbxLayerElementUserData()
	{
		Clear();
	}

	
	KArrayTemplate<void*>* GetDirectArrayVoid( int pIndex, bool* pStatus = NULL)
	{
		if( pIndex >= 0 || pIndex < mDirectArray.GetCount() )
			return (KArrayTemplate<void*> *)mDirectArray[pIndex];
		else
		{
			if( pStatus )
				*pStatus = false;

			K_ASSERT_MSG_NOW("Index out of bounds");
			return (KArrayTemplate<void*>*)NULL;
		}
	}

	/** Gets the direct array with the given attribute name
	 */ 
	KArrayTemplate<void *>* GetDirectArrayVoid ( const char* pName, bool* pStatus = NULL )
	{
		KString lName( pName );
		for( int i = 0; i < mDataNames.GetCount(); ++i )
		{
			if( *mDataNames[i] == lName )
				return GetDirectArrayVoid(i, pStatus);
		}

		return (KArrayTemplate<void *>*)NULL;
	}

	/** Gets the data type for the given index
	 * \param pIndex     The index of the attribute being queried
	 * \return           The data type, or DTNone if pIndex is out of range
	 */
	KFbxDataType GetDataType( int pIndex ) const
	{
		if( pIndex < 0 || pIndex >= mDataTypes.GetCount() )
			return DTNone;

		return mDataTypes[pIndex];
	}

	/** Gets the data type for the given attribute
	 * \param pName     The name of the attribute being queried
	 * \return          The data type, or DTNone if no attribute has the given name
	 */
	KFbxDataType GetDataType( const char* pName ) const
	{
		KString lName( pName );

		for( int i = 0; i < mDataNames.GetCount(); ++i )
		{
			if( *mDataNames[i] == lName )
				return mDataTypes[i];
		}

		return DTNone;
	}

	/** Gets the attribute name at the given index
	 * \param pIndex     Attribute index
	 * \return           The name, or \c NULL if pIndex is out of range.
	 */
	const char* GetDataName( int pIndex ) const
	{
		if( pIndex >= 0 && pIndex < mDataNames.GetCount() )
			return mDataNames[pIndex]->GetBuffer();

		return NULL;
	}

	/** Resize all direct arrays to the given size. 
	 * \param pSize     The new size of the direct arrays.
	 */
	void ResizeAllDirectArrays( int pSize )
	{
		for( int i = 0; i < mDirectArray.GetCount(); ++i )
		{
			switch( mDataTypes[i].GetType() )
			{
				case eBOOL1:	KFbxGetDirectArray<bool>(this,i).Resize( pSize );			break;
				case eINTEGER1:	KFbxGetDirectArray<int>(this,i).Resize( pSize );				break;
				case eFLOAT1:	KFbxGetDirectArray<float>(this,i).Resize( pSize );			break;
				case eDOUBLE1:	KFbxGetDirectArray<double>(this,i).Resize( pSize );			break;
				//case eDOUBLE3:	GetDirectArray< fbxDouble3 >(i).Resize( pSize );	break;
				//case eDOUBLE4:	GetDirectArray< fbxDouble4 >(i).Resize( pSize );	break;
				//case eDOUBLE44:	GetDirectArray< fbxDouble44>(i).Resize( pSize );	break;  
				default:
					K_ASSERT_MSG_NOW("unknown type" ); break;
			}
		}
	}

	/** Removes a single element at pIndex from all direct arrays.
	 * \param pIndex     The index to remove.
	 */
	void RemoveFromAllDirectArrays( int pIndex )
	{
		for( int i = 0; i < mDirectArray.GetCount(); ++i )
		{
			switch( mDataTypes[i].GetType() )
			{
				case eBOOL1:	KFbxGetDirectArray<bool>(this,i).RemoveAt( pIndex );				break;
				case eINTEGER1:	KFbxGetDirectArray<int>(this,i).RemoveAt( pIndex );				break;
				case eFLOAT1:	KFbxGetDirectArray<float>(this,i).RemoveAt( pIndex );			break;
				case eDOUBLE1:	KFbxGetDirectArray<double>(this,i).RemoveAt( pIndex );			break;
				//case eDOUBLE3:	GetDirectArray< fbxDouble3 >(i).RemoveAt( pIndex );	break;
				//case eDOUBLE4:	GetDirectArray< fbxDouble4 >(i).RemoveAt( pIndex );	break;
				//case eDOUBLE44:	GetDirectArray< fbxDouble44>(i).RemoveAt( pIndex );	break;  
				default:
					K_ASSERT_MSG_NOW("unknown type" ); break;
			}
		}
	}

	/** Get the direct array count for the attribute at pIndex
	 * \param pIndex     The index to query
	 * \return           The direct array count
	 */
	int GetArrayCount( int pIndex ) const 
	{
		if( pIndex >= 0 && pIndex < mDirectArray.GetCount() )
		{
			switch( mDataTypes[pIndex].GetType() )
			{
				case eBOOL1:	return ((const KArrayTemplate<bool>*)mDirectArray[pIndex])->GetCount();
				case eINTEGER1:	return ((const KArrayTemplate<int>*)mDirectArray[pIndex])->GetCount();
				case eFLOAT1:	return ((const KArrayTemplate<float>*)mDirectArray[pIndex])->GetCount();
				case eDOUBLE1:	return ((const KArrayTemplate<double>*)mDirectArray[pIndex])->GetCount();
				//case eDOUBLE3:	return GetDirectArray< fbxDouble3 >(pIndex).GetCount();
				//case eDOUBLE4:	return GetDirectArray< fbxDouble4 >(pIndex).GetCount();
				//case eDOUBLE44:	return GetDirectArray< fbxDouble44>(pIndex).GetCount();
				default:
					K_ASSERT_MSG_NOW("Unknown type" ); break;
			}
		}

		return -1;
	}

	/** Query the id
	 * \return     The Id expressed as an int
	 */
	int GetId() const { return mId; }

	/** Get the direct array count
	 * \return     The direct array count expressed as an int
	 */
	int GetDirectArrayCount() const { return mDirectArray.GetCount(); }

	/** Assignment does a deep copy */
	KFbxLayerElementUserData& operator=( KFbxLayerElementUserData const& pOther )
	{
      int i;
		Clear();

		// Build descriptives.
		//
		for( i = 0; i < pOther.mDataNames.GetCount(); ++i )
			mDataNames.Add( new KString( *pOther.mDataNames[i] ) );

		for( i = 0; i < pOther.mDataTypes.GetCount(); ++i )
			mDataTypes.Add( pOther.mDataTypes[i] );


		Init();

		for( i = 0; i < pOther.GetDirectArrayCount(); ++i )
		{
			switch( mDataTypes[i].GetType() )
			{
				case eBOOL1:	((KArrayTemplate<bool>*)mDirectArray[i])->operator=( (*(const KArrayTemplate<bool>*)pOther.mDirectArray[i]) ); break;
				case eINTEGER1:	((KArrayTemplate<int>*)mDirectArray[i])->operator=( (*(const KArrayTemplate<int>*)pOther.mDirectArray[i]) ); break;
				case eFLOAT1:	((KArrayTemplate<float>*)mDirectArray[i])->operator=( (*(const KArrayTemplate<float>*)pOther.mDirectArray[i]) ); break;
				case eDOUBLE1:	((KArrayTemplate<double>*)mDirectArray[i])->operator=( (*(const KArrayTemplate<double>*)pOther.mDirectArray[i]) ); break;
				//case eDOUBLE3:	GetDirectArray< fbxDouble3 >(pIndex).SetAt(j, pOther.GetDirectArray<bool>(i)[j] );
				//case eDOUBLE4:	GetDirectArray< fbxDouble4 >(pIndex).SetAt(j, pOther.GetDirectArray<bool>(i)[j] );
				//case eDOUBLE44:	GetDirectArray< fbxDouble44>(pIndex).SetAt(j, pOther.GetDirectArray<bool>(i)[j] );
				default:
					K_ASSERT_MSG_NOW("Unknown type" ); break;
			}
		}

		mId = pOther.mId;

		mIndexArray = pOther.mIndexArray;

		return *this;
	}

	/** Clears all of the data from this layer element.
	 */
	void Clear()
	{
	  int i;

		for( i = 0; i < mDirectArray.GetCount(); ++i )
		{
			if( mDirectArray[i] )
			{
				delete mDirectArray[i];
				mDirectArray[i] = NULL;
			}
		}
		// cleared in template::clear

		for( i = 0; i < mDataNames.GetCount(); ++i )
		{
			if( mDataNames[i] )
			{
				delete mDataNames[i];
				mDataNames[i] = NULL;
			}
		}
		mDataNames.Clear();

		mDataTypes.Clear();

		KFbxLayerElementTemplate<void*>::Clear();
	}

protected:
	
	/** Constructs a user data layer element. 
	 * \param pId            An indentifier for this UserData layer element
	 * \param pDataTypes     Attribute data types for this layer element
	 * \param pDataNames     Attribute names for this layer element
	 */
	KFbxLayerElementUserData( int pId, KArrayTemplate<KFbxDataType>& pDataTypes, KArrayTemplate<const char*>& pDataNames )
		:
		mId( pId ),
		mDataTypes( pDataTypes )
	{
		K_ASSERT( pDataTypes.GetCount() == pDataNames.GetCount() );
		Init();

		for( int i = 0; i < pDataNames.GetCount(); ++i )
		{
			mDataNames.Add( new KString( pDataNames[i] ) );
		}
	}
	/* Copy constructor. A deep copy is made.
	*/
	KFbxLayerElementUserData( KFbxLayerElementUserData const& pOther )
	{
		*this = pOther;
	}
	//friend class KFbxReaderFbx6;
	friend class KFbxSdkManager;
private:

	void Init()
	{
	  int i;
		mDirectArray.Resize( mDataTypes.GetCount() );

		// initialize arrays
		for( i = 0; i < mDataTypes.GetCount(); ++i )
		{
			switch( mDataTypes[i].GetType() )
			{
				case eBOOL1:	mDirectArray[i] = new KArrayTemplate<bool>();			break;
				case eINTEGER1:	mDirectArray[i] = new KArrayTemplate<int>();				break;
				case eFLOAT1:	mDirectArray[i] = new KArrayTemplate<float>();			break;
				case eDOUBLE1:	mDirectArray[i] = new KArrayTemplate<double>();			break;
				//case eDOUBLE3:	mDirectArray[i] = new KArrayTemplate< fbxDouble3 >();	break;  
				//case eDOUBLE4:	mDirectArray[i] = new KArrayTemplate< fbxDouble4 >();	break;
				//case eDOUBLE44:	mDirectArray[i] = new KArrayTemplate< fbxDouble44 >();	break;  
				default:
					K_ASSERT_MSG_NOW("Trying to assign an unknown type" ); break;
			}
		}
	}

	int mId;
	KArrayTemplate<KFbxDataType> mDataTypes;
	KArrayTemplate<KString*> mDataNames;
};

/** Access the direct array at pIndex. The template type must match the attribute type at pIndex. 
 * \param pIndex      The direct array index
 * \param pStatus     Will be set to false if accessing the direct array encounters an error.
 * \return            If pStatus receives \c true, the direct array at the given index is
 *                    returned. Otherwise the return value is \c undefined.
 */
template <class T>
inline KArrayTemplate<T>& KFbxGetDirectArray( KFbxLayerElementUserData *pLayerElement,int pIndex, bool* pStatus)
{
	return *(KArrayTemplate<T>*)pLayerElement->GetDirectArrayVoid(pIndex,pStatus);
}

/** Gets the direct array with the given attribute name
 */ 
template <class T>
inline KArrayTemplate<T>& KFbxGetDirectArray( KFbxLayerElementUserData *pLayerElement,const char* pName, bool* pStatus )
{
	return *(KArrayTemplate<T>*)pLayerElement->GetDirectArrayVoid(pName,pStatus);
}



/** Layer to indicate smoothness of components of a geometry.
  * \nosubgrouping
  */
class KFBX_DLL KFbxLayerElementSmoothing : public KFbxLayerElementTemplate<bool>
{
public:

	void SetReferenceMode( KFbxLayerElement::EReferenceMode pMode )
	{
		if( pMode != KFbxLayerElement::eDIRECT )
		{
			K_ASSERT_MSG_NOW( "Smoothing layer elements must be direct mapped" );
			return;
		}
	}
protected:
	KFbxLayerElementSmoothing()
	{
		mReferenceMode = KFbxLayerElement::eDIRECT;
	}
	friend class KFbxSdkManager;

};

/** To indicate if specified components are shown/hidden 
 */
typedef class KFBX_DLL KFbxLayerElementTemplate<bool> KFbxLayerElementVisibility;

typedef class KFBX_DLL KFbxLayerElementTemplate<KFbxTexture*>  KFbxLayerElementTextureBase;

/** Layer to map Textures on a geometry.
  * \nosubgrouping
  */
class KFBX_DLL KFbxLayerElementTexture : public KFbxLayerElementTextureBase
{

public:

	/** \enum EBlendMode     Lets you control how textures are combined when multiple layers of texture are applied to an surface.
	  * - \e eTRANSLUCENT     The new texture layer is transparent depending on the Alpha value.
	  * - \e eADD             The color of the new texture is added to the previous texture.
	  * - \e eMODULATE        The color value of the new texture is multiplied by the color values of all previous layers of texture.
	  * - \e eMODULATE2       The color value of the new texure is multiplied by two and then multiplied by the color values of all previous layers of texture.
	  * - \e eMAXBLEND        Marks the end of the blend mode enum.
	  */
	typedef enum 
	{
		eTRANSLUCENT,
		eADD,
		eMODULATE,
		eMODULATE2,
		eMAXBLEND
	} EBlendMode;

	/** Set the way Textures are blended between layers.
	  * \param pBlendMode     A valid blend mode.
	  */
	void       SetBlendMode(EBlendMode pBlendMode) { mBlendMode = pBlendMode; }

	/** Set the level of transparency between multiple texture levels.
	  * \param pAlpha     Set to a value between 0.0 and 1.0, where 0.0 is totally transparent and 1.0 is totally opaque.
	  * \remarks          Values smaller than 0.0 are clipped to 0.0, while values larger than 1.0 are clipped to 1.0.
	  */
	void       SetAlpha(double pAlpha)             
	{ 
		mAlpha = pAlpha > 1.0 ? 1.0 : pAlpha;
		mAlpha = pAlpha < 0.0 ? 0.0 : pAlpha;
	}

	/** Get the way Textures are blended between layers.
	  * \return     The current Blend Mode.
	  */
	EBlendMode GetBlendMode()                      { return mBlendMode; } 

	/** Get the level of transparency between multiple levels of textures.
	  * \return     An alpha value between 0.0 and 1.0, where 0.0 is totally transparent and 1.0 is totally opaque.
	  */
	double     GetAlpha()                          { return mAlpha; }

///////////////////////////////////////////////////////////////////////////////
//
//  WARNING!
//
//	Anything beyond these lines may not be documented accurately and is 
// 	subject to change without notice.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    void SetType(KFbxDataType* pType) { mType = pType; }
protected:
	/** Constructor
	  * By default, textures have a Blend Mode of eTRANSLUCENT,
	  * a Reference Mode of eINDEX_TO_DIRECT, and an Alpha value of 1.0.
	  */
	KFbxLayerElementTexture() : mBlendMode(eTRANSLUCENT)
	{
		mReferenceMode = eINDEX_TO_DIRECT;
		mAlpha         = 1.0;
	}
	friend class KFbxSdkManager;

private:
    KFbxDataType* mType;
	EBlendMode mBlendMode;
	double     mAlpha;
#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS
};


/** \brief KFbxLayer class provides the base for the layering mechanism.
  * 
  * A layer can contain one or more of the following layer elements:
  *      - Normals
  *      - UVs
  *      - Textures (diffuse, ambient, specular, etc.)
  *      - Materials
  *      - Polygon Groups
  *      - Vertex Colors and
  *      - Smoothing information
  *      - Custom User Data
  * 
  * A typical layer for a Mesh will contain Normals, UVs and Materials. A typical layer for Nurbs will contain only Materials. If a texture
  * is applied to a Mesh, a typical layer will contain the Textures along with the UVs. In the case of the Nurbs, the Nurbs' parameterization is
  * used for the UVs; there should be no UVs specified.  The mapping of a texture is completely defined within the layer; there is no 
  * cross-layer management.
  * 
  * In most cases, a single layer is sufficient to describe a geometry. Many applications will only support what is defined on the first layer. 
  * This should be taken into account when filling the layer. For example, it is totaly legal to define the Layer 0 with the Textures and UVs and
  * define the model's Normals on layer 1. However a file constructed this way may not be imported correctly in other applications. The user
  * should put the Normals in Layer 0.
  * 
  * Texture layering is achieved by defining more than one layer containing Textures and UVs elements. For example, a Mesh may have Textures and
  * the corresponding UVs elements on Layer 0 for the primary effect, and another set of Textures and UVs on Layer 1. The way the texture blending
  * is done is defined in the Texture layer element.
  * \nosubgrouping
  */
class KFBX_DLL KFbxLayer
{

public:

	/**
	  * \name Layer Element Management
	  */
	//@{

	/** Get the Normals description for this layer.
	  * \return      Pointer to the Normals layer element, or \c NULL if no Normals are defined for this layer.
	  * \remarks     A geometry of type KFbxNurb or KFbxPatch should not have Normals defined.
	  */
	KFbxLayerElementNormal* GetNormals();	

	/** Get the Normals description for this layer.
	  * \return      Pointer to the Normals layer element, or \c NULL if no Normals are defined for this layer.
	  * \remarks     A geometry of type KFbxNurb or KFbxPatch should not have Normals defined.
	  */
	KFbxLayerElementNormal const* GetNormals() const;	

	/** Get the Materials description for this layer.
	  * \return     Pointer to the Materials layer element, or \c NULL if no Materials are defined for this layer.
	  */
	KFbxLayerElementMaterial* GetMaterials();

	/** Get the Materials description for this layer.
	  * \return     Pointer to the Materials layer element, or \c NULL if no Materials are defined for this layer.
	  */
	KFbxLayerElementMaterial const* GetMaterials() const;

	/** Get the Polygon Groups description for this layer.
	  * \return     Pointer to the Polygon Groups layer element, or \c NULL if no Polygon Groups are defined for this layer.
	  */
	KFbxLayerElementPolygonGroup* GetPolygonGroups();

	/** Get the Polygon Groups description for this layer.
	  * \return     Pointer to the Polygon Groups layer element, or \c NULL if no Polygon Groups are defined for this layer.
	  */
	KFbxLayerElementPolygonGroup const* GetPolygonGroups() const;


	   /** Get the EmissiveUV description for this layer.
	  * \return     Pointer to the EmissiveUV layer element, or \c NULL if no EmissiveUV are defined for this layer.
	  */
	KFbxLayerElementUV* GetEmissiveUV();

	/** Get the EmissiveUV description for this layer.
	  * \return     Pointer to the EmissiveUV layer element, or \c NULL if no EmissiveUV are defined for this layer.
	  */
	KFbxLayerElementUV const* GetEmissiveUV() const;

    /** Get the EmissiveFactorUV description for this layer.
	  * \return     Pointer to the EmissiveFactorUV layer element, or \c NULL if no EmissiveFactorUV are defined for this layer.
	  */
	KFbxLayerElementUV* GetEmissiveFactorUV();

	/** Get the EmissiveFactorUV description for this layer.
	  * \return     Pointer to the EmissiveFactorUV layer element, or \c NULL if no EmissiveFactorUV are defined for this layer.
	  */
	KFbxLayerElementUV const* GetEmissiveFactorUV() const;

    /** Get the EmissiveFactorUV description for this layer.
	  * \return     Pointer to the AmbientUV layer element, or \c NULL if no AmbientUV are defined for this layer.
	  */
	KFbxLayerElementUV* GetAmbientUV();

	/** Get the AmbientUV description for this layer.
	  * \return     Pointer to the AmbientUV layer element, or \c NULL if no AmbientUV are defined for this layer.
	  */
	KFbxLayerElementUV const* GetAmbientUV() const;

    /** Get the AmbientUV description for this layer.
	  * \return     Pointer to the AmbientFactorUV layer element, or \c NULL if no AmbientFactorUV are  defined for this layer.
	  */
	KFbxLayerElementUV* GetAmbientFactorUV();

	/** Get the AmbientFactorUV description for this layer.
	  * \return     Pointer to the AmbientFactorUV layer element, or \c NULL if no AmbientFactorUV are defined for this layer.
	  */
	KFbxLayerElementUV const* GetAmbientFactorUV() const;
    
    /** Get the AmbientFactorUV description for this layer.
     * \return     Pointer to the DiffuseUV layer element, or \c NULL if no DiffuseUV are defined for this layer.
	  */
	KFbxLayerElementUV* GetDiffuseUV();

	/** Get the DiffuseUV description for this layer.
	  * \return     Pointer to the DiffuseUV layer element, or \c NULL if no DiffuseUV are defined for this layer.
	  */
	KFbxLayerElementUV const* GetDiffuseUV() const;

    /** Get the DiffuseUV description for this layer.
	  * \return     Pointer to the DiffuseFactorUV layer element, or \c NULL if no DiffuseFactorUV are defined for this layer.
	  */
	KFbxLayerElementUV* GetDiffuseFactorUV();

	/** Get the DiffuseFactorUV description for this layer.
	  * \return     Pointer to the DiffuseFactorUV layer element, or \c NULL if no DiffuseFactorUV are defined for this layer.
	  */
	KFbxLayerElementUV const* GetDiffuseFactorUV() const;

    /** Get the DiffuseFactorUV description for this layer.
	  * \return     Pointer to the SpecularUV layer element, or \c NULL if no SpecularUV are defined for this layer.
	  */
	KFbxLayerElementUV* GetSpecularUV();

	/** Get the SpecularUV description for this layer.
	  * \return     Pointer to the SpecularUV layer element, or \c NULL if no SpecularUV are defined for this layer.
	  */
	KFbxLayerElementUV const* GetSpecularUV() const;

    /** Get the SpecularUV description for this layer.
	  * \return     Pointer to the SpecularFactorUV layer element, or \c NULL if no SpecularFactorUV are defined for this layer.
	  */
	KFbxLayerElementUV* GetSpecularFactorUV();

	/** Get the SpecularFactorUV description for this layer.
	  * \return     Pointer to the SpecularFactorUV layer element, or \c NULL if no SpecularFactorUV are defined for this layer.
	  */
	KFbxLayerElementUV const* GetSpecularFactorUV() const;

    /** Get the SpecularFactorUV description for this layer.
	  * \return     Pointer to the ShininessUV layer element, or \c NULL if no ShininessUV are defined for this layer.
	  */
	KFbxLayerElementUV* GetShininessUV();

	/** Get the ShininessUV description for this layer.
	  * \return     Pointer to the ShininessUV layer element, or \c NULL if no ShininessUV are defined for this layer.
	  */
	KFbxLayerElementUV const* GetShininessUV() const;

	/** Get the NormalMapUV description for this layer.
	  * \return     Pointer to the BumpUV layer element, or \c NULL if no BumpUV are defined for this layer.
	  */
	KFbxLayerElementUV* GetNormalMapUV();

	/** Get the NormalMapUV description for this layer.
	  * \return     Pointer to the BumpUV layer element, or \c NULL if no BumpUV are defined for this layer.
	  */
	KFbxLayerElementUV const* GetNormalMapUV() const;

    /** Get the BumpUV description for this layer.
	  * \return     Pointer to the BumpUV layer element, or \c NULL if no BumpUV are defined for this layer.
	  */
	KFbxLayerElementUV* GetBumpUV();

	/** Get the BumpUV description for this layer.
	  * \return     Pointer to the BumpUV layer element, or \c NULL if no BumpUV are defined for this layer.
	  */
	KFbxLayerElementUV const* GetBumpUV() const;

    /** Get the TransparentUV description for this layer.
	  * \return     Pointer to the TransparentUV layer element, or \c NULL if no TransparentUV are defined for this layer.
	  */
	KFbxLayerElementUV* GetTransparentUV();

	/** Get the TransparentUV description for this layer.
	  * \return     Pointer to the TransparentUV layer element, or \c NULL if no TransparentUV are defined for this layer.
	  */
	KFbxLayerElementUV const* GetTransparentUV() const;

    /** Get the TransparencyFactorUV description for this layer.
	  * \return     Pointer to the TransparencyFactorUV layer element, or \c NULL if no TransparencyFactorUV are defined for this layer.
	  */
	KFbxLayerElementUV* GetTransparencyFactorUV();

	/** Get the TransparencyFactorUV description for this layer.
	  * \return     Pointer to the TransparencyFactorUV layer element, or \c NULL if no TransparencyFactorUV are defined for this layer.
	  */
	KFbxLayerElementUV const* GetTransparencyFactorUV() const;

    /** Get the ReflectionUV description for this layer.
	  * \return     Pointer to the ReflectionUV layer element, or \c NULL if no ReflectionUV are defined for this layer.
	  */
	KFbxLayerElementUV* GetReflectionUV();

	/** Get the ReflectionUV description for this layer.
	  * \return     Pointer to the ReflectionUV layer element, or \c NULL if no ReflectionUV are defined for this layer.
	  */
	KFbxLayerElementUV const* GetReflectionUV() const;

    /** Get the ReflectionFactorUV description for this layer.
	  * \return     Pointer to the ReflectionFactorUV layer element, or \c NULL if no ReflectionFactorUV are defined for this layer.
	  */
	KFbxLayerElementUV* GetReflectionFactorUV();

	/** Get the ReflectionFactorUV description for this layer.
	  * \return     Pointer to the ReflectionFactorUV layer element, or \c NULL if no ReflectionFactorUV are defined for this layer.
	  */
	KFbxLayerElementUV const* GetReflectionFactorUV() const;

	/** Get the UVs description for this layer.
	  * \return      Pointer to the UVs layer element, or \c NULL if no UV are defined for this layer.
	  * \remarks     A geometry of type KFbxNurb or KFbxPatch should not have UVs defined. 
	  *              The Nurbs/Patch parameterization is used as UV parameters to map a texture.
	  */
	KFbxLayerElementUV* GetUVs(KFbxLayerElement::ELayerElementType pTypeIdentifier=KFbxLayerElement::eDIFFUSE_TEXTURES);

	/** Get the UVs description for this layer.
	  * \return      Pointer to the UVs layer element, or \c NULL if no UV are defined for this layer.
	  * \remarks     A geometry of type KFbxNurb or KFbxPatch should not have UVs defined.
	  *              The Nurbs/Patch parameterization is used as UV parameters to map a texture.
	  */
	KFbxLayerElementUV const* GetUVs(KFbxLayerElement::ELayerElementType pTypeIdentifier=KFbxLayerElement::eDIFFUSE_TEXTURES) const;


	/** Get the number of different UV set for this layer.
	  */
	int GetUVSetCount() const;
	
	/** Get an array of UV of the different UV set for this layer.
	  */
	KArrayTemplate<KFbxLayerElement::ELayerElementType> GetUVSetChannels() const;

	/** Get the Vertex Colors description for this layer.
	  * \return      Pointer to the Vertex Colors layer element, or \c NULL if no Vertex Color are defined for this layer.
	  * \remarks     A geometry of type KFbxNurb or KFbxPatch should not have Vertex Colors defined, since no vertex exists.
	  */
	KFbxLayerElementVertexColor* GetVertexColors();

	/** Get the Vertex Colors description for this layer.
	  * \return      Pointer to the Vertex Colors layer element, or \c NULL if no Vertex Color are defined for this layer.
	  * \remarks     A geometry of type KFbxNurb or KFbxPatch should not have Vertex Colors defined, since no vertex exists.
	  */
	KFbxLayerElementVertexColor const* GetVertexColors() const;

	/** Get the Smoothing description for this layer.
	  * \return      Pointer to the Smoothing layer element, or \c NULL if no Smoothing is defined for this layer.
	  * \remarks     A geometry of type KFbxNurb or KFbxPatch should not have Smoothing defined.
	  */
	KFbxLayerElementSmoothing* GetSmoothing();

	/** Get the Smoothing description for this layer.
	  * \return      Pointer to the Smoothing layer element, or \c NULL if no Smoothing is defined for this layer.
	  * \remarks     A geometry of type KFbxNurb or KFbxPatch should not have Smoothing defined.
	  */
	KFbxLayerElementSmoothing const* GetSmoothing() const;

	/** Get the User Data for this layer.
	  * \return     Pointer to the User Data layer element, or \c NULL if no User Data is defined for this layer.
	  */
	KFbxLayerElementUserData* GetUserData();

	/** Get the User Data for this layer.
	  * \return     Pointer to the User Data layer element, or \c NULL if no User Data is defined for this layer.
	  */
	KFbxLayerElementUserData const* GetUserData() const;

	/** Get the visibility for this layer.
	  * \return     Pointer to the visibility layer element, or \c NULL if no visibility is defined for this layer.
	  */
	KFbxLayerElementVisibility* GetVisibility();

	/** Get the visibility for this layer.
	  * \return     Pointer to the visibility layer element, or \c NULL if no visibility is defined for this layer.
	  */
	KFbxLayerElementVisibility const* GetVisibility() const;

    /** Get the EmissiveTextures description for this layer.
	  * \return     Pointer to the EmissiveTextures layer element, or \c NULL if no EmissiveTextures are defined for this layer.
	  */
	KFbxLayerElementTexture* GetEmissiveTextures();

	/** Get the EmissiveTextures description for this layer.
	  * \return     Pointer to the EmissiveTextures layer element, or \c NULL if no EmissiveTextures are defined for this layer.
	  */
	KFbxLayerElementTexture const* GetEmissiveTextures() const;

    /** Get the EmissiveFactorTextures description for this layer.
	  * \return     Pointer to the EmissiveFactorTextures layer element, or \c NULL if no EmissiveFactorTextures are defined for this layer.
	  */
	KFbxLayerElementTexture* GetEmissiveFactorTextures();

	/** Get the EmissiveFactorTextures description for this layer.
	  * \return     Pointer to the EmissiveFactorTextures layer element, or \c NULL if no EmissiveFactorTextures are defined for this layer.
	  */
	KFbxLayerElementTexture const* GetEmissiveFactorTextures() const;

    /** Get the AmbientTextures description for this layer.
	  * \return     Pointer to the AmbientTextures layer element, or \c NULL if no AmbientTextures are defined for this layer.
	  */
	KFbxLayerElementTexture* GetAmbientTextures();

	/** Get the AmbientTextures description for this layer.
	  * \return     Pointer to the AmbientTextures layer element, or \c NULL if no AmbientTextures are defined for this layer.
	  */
	KFbxLayerElementTexture const* GetAmbientTextures() const;

    /** Get the AmbientFactorTextures description for this layer.
	  * \return     Pointer to the AmbientFactorTextures layer element, or \c NULL if no AmbientFactorTextures are defined for this layer.
	  */
	KFbxLayerElementTexture* GetAmbientFactorTextures();

	/** Get the AmbientFactorTextures description for this layer.
	  * \return     Pointer to the AmbientFactorTextures layer element, or \c NULL if no AmbientFactorTextures are defined for this layer.
	  */
	KFbxLayerElementTexture const* GetAmbientFactorTextures() const;
    
    /** Get the DiffuseTextures description for this layer.
     * \return     Pointer to the DiffuseTextures layer element, or \c NULL if no DiffuseTextures are defined for this layer.
	  */
	KFbxLayerElementTexture* GetDiffuseTextures();

	/** Get the DiffuseTextures description for this layer.
	  * \return     Pointer to the DiffuseTextures layer element, or \c NULL if no DiffuseTextures are defined for this layer.
	  */
	KFbxLayerElementTexture const* GetDiffuseTextures() const;

    /** Get the DiffuseFactorTextures description for this layer.
	  * \return     Pointer to the DiffuseFactorTextures layer element, or \c NULL if no DiffuseFactorTextures are defined for this layer.
	  */
	KFbxLayerElementTexture* GetDiffuseFactorTextures();

	/** Get the DiffuseFactorTextures description for this layer.
	  * \return     Pointer to the DiffuseFactorTextures layer element, or \c NULL if no DiffuseFactorTextures are defined for this layer.
	  */
	KFbxLayerElementTexture const* GetDiffuseFactorTextures() const;

    /** Get the SpecularTextures description for this layer.
	  * \return     Pointer to the SpecularTextures layer element, or \c NULL if no SpecularTextures are defined for this layer.
	  */
	KFbxLayerElementTexture* GetSpecularTextures();

	/** Get the SpecularTextures description for this layer.
	  * \return     Pointer to the SpecularTextures layer element, or \c NULL if no SpecularTextures are defined for this layer.
	  */
	KFbxLayerElementTexture const* GetSpecularTextures() const;

    /** Get the SpecularFactorTextures description for this layer.
	  * \return     Pointer to the SpecularFactorTextures layer element, or \c NULL if no SpecularFactorTextures are defined for this layer.
	  */
	KFbxLayerElementTexture* GetSpecularFactorTextures();

	/** Get the SpecularFactorTextures description for this layer.
	  * \return     Pointer to the SpecularFactorTextures layer element, or \c NULL if no SpecularFactorTextures are defined for this layer.
	  */
	KFbxLayerElementTexture const* GetSpecularFactorTextures() const;

    /** Get the ShininessTextures description for this layer.
	  * \return     Pointer to the ShininessTextures layer element, or \c NULL if no ShininessTextures are defined for this layer.
	  */
	KFbxLayerElementTexture* GetShininessTextures();

	/** Get the ShininessTextures description for this layer.
	  * \return     Pointer to the ShininessTextures layer element, or \c NULL if no ShininessTextures are defined for this layer.
	  */
	KFbxLayerElementTexture const* GetShininessTextures() const;

	/** Get the NormalMapTextures description for this layer.
	  * \return     Pointer to the BumpTextures layer element, or \c NULL if no BumpTextures are defined for this layer.
	  */
	KFbxLayerElementTexture* GetNormalMapTextures();

	/** Get the NormalMapTextures description for this layer.
	  * \return     Pointer to the BumpTextures layer element, or \c NULL if no BumpTextures are defined for this layer.
	  */
	KFbxLayerElementTexture const* GetNormalMapTextures() const;

    /** Get the BumpTextures description for this layer.
	  * \return     Pointer to the BumpTextures layer element, or \c NULL if no BumpTextures are defined for this layer.
	  */
	KFbxLayerElementTexture* GetBumpTextures();

	/** Get the BumpTextures description for this layer.
	  * \return     Pointer to the BumpTextures layer element, or \c NULL if no BumpTextures are defined for this layer.
	  */
	KFbxLayerElementTexture const* GetBumpTextures() const;

    /** Get the TransparentTextures description for this layer.
	  * \return     Pointer to the TransparentTextures layer element, or \c NULL if no TransparentTextures are defined for this layer.
	  */
	KFbxLayerElementTexture* GetTransparentTextures();

	/** Get the TransparentTextures description for this layer.
	  * \return     Pointer to the TransparentTextures layer element, or \c NULL if no TransparentTextures are defined for this layer.
	  */
	KFbxLayerElementTexture const* GetTransparentTextures() const;

    /** Get the TransparencyFactorTextures description for this layer.
	  * \return     Pointer to the TransparencyFactorTextures layer element, or \c NULL if no TransparencyFactorTextures are defined for this layer.
	  */
	KFbxLayerElementTexture* GetTransparencyFactorTextures();

	/** Get the TransparencyFactorTextures description for this layer.
	  * \return     Pointer to the TransparencyFactorTextures layer element, or \c NULL if no TransparencyFactorTextures are defined for this layer.
	  */
	KFbxLayerElementTexture const* GetTransparencyFactorTextures() const;

    /** Get the ReflectionTextures description for this layer.
	  * \return     Pointer to the ReflectionTextures layer element, or \c NULL if no ReflectionTextures are defined for this layer.
	  */
	KFbxLayerElementTexture* GetReflectionTextures();

	/** Get the ReflectionTextures description for this layer.
	  * \return     Pointer to the ReflectionTextures layer element, or \c NULL if no ReflectionTextures are defined for this layer.
	  */
	KFbxLayerElementTexture const* GetReflectionTextures() const;

    /** Get the ReflectionFactorTextures description for this layer.
	  * \return     Pointer to the ReflectionFactorTextures layer element, or \c NULL if no ReflectionFactorTextures are defined for this layer.
	  */
	KFbxLayerElementTexture* GetReflectionFactorTextures();

	/** Get the ReflectionFactorTextures description for this layer.
	  * \return     Pointer to the ReflectionFactorTextures layer element, or \c NULL if no ReflectionFactorTextures are defined for this layer.
	  */
	KFbxLayerElementTexture const* GetReflectionFactorTextures() const;


	KFbxLayerElementTexture* GetTextures(KFbxLayerElement::ELayerElementType pType);
	KFbxLayerElementTexture const* GetTextures(KFbxLayerElement::ELayerElementType pType) const;
	void SetTextures(KFbxLayerElement::ELayerElementType pType, KFbxLayerElementTexture* pTextures);

	/** Get the layer element description of the specified type for this layer.
	  * \param pType     The Layer element type required. Supported values are KFbxLayerElement::eNORMAL, KFbxLayerElement::eMATERIAL,
	  *                  KFbxLayerElement::eTEXTURE, KFbxLayerElement::ePOLYGON_GROUP, KFbxLayerElement::eUV and KFbxLayerElement::eVERTEX_COLOR. 
	  *                       - Calling with eNORMAL is equivalent to calling GetNormals().
	  *                       - Calling with eMATERIAL is equivalent to calling GetMaterials().
	  *                       - Calling with ePOLYGON_GROUP is equivalent to calling GetPolygonGroups().
	  *                       - Calling with eUV is equivalent to calling GetUVs().
	  *                       - Calling with eVERTEX_COLOR is equivalent to calling GetVertexColors().
	  *                       - Calling with eSMOOTHING is equivalent to calling GetSmoothing().
	  *                       - Calling with eUSER_DATA is equivalent to calling GetUserData().
      *                       - Calling with eEMISSIVE_TEXTURES is equivalent to calling GetEmissiveTextures().
      *                       - Calling with eEMISSIVE_FACTOR_TEXTURES is equivalent to calling GetEmissiveFactorTextures().
      *                       - Calling with eAMBIENT_TEXTURES is equivalent to calling GetAmbientTextures().
      *                       - Calling with eAMBIENT_FACTOR_TEXTURES is equivalent to calling GetAmbientFactorTextures().
      *                       - Calling with eDIFFUSE_TEXTURES is equivalent to calling GetDiffuseTextures().
      *                       - Calling with eDIFFUSE_FACTOR_TEXTURES is equivalent to calling GetDiffuseFactorTextures().
      *                       - Calling with eSPECULAR_TEXTURES is equivalent to calling GetSpecularTextures().
      *                       - Calling with eSPECULAR_FACTOR_TEXTURES is equivalent to calling GetSpecularFactorTextures().
      *                       - Calling with eSHININESS_TEXTURES is equivalent to calling GetShininessTextures().
      *                       - Calling with eBUMP_TEXTURES is equivalent to calling GetBumpTextures().
	  *                       - Calling with eNORMALMAP_TEXTURES is equivalent to calling GetNormalMapTextures().
      *                       - Calling with eTRANSPARENT_TEXTURES is equivalent to calling GetTransparentTextures().
      *                       - Calling with eTRANSPARENCY_FACTOR_TEXTURES is equivalent to calling GetTransparencyFactorTextures().
      *                       - Calling with eREFLECTION_TEXTURES is equivalent to calling GetReflectionTextures().
      *                       - Calling with eREFLECTION_FACTOR_TEXTURES is equivalent to calling GetReflectionFactorTextures().
      * \param pIsUV     When \c true, request the UV LayerElement corresponding to the specified Layer Element type.
	  * \return          Pointer to the requested layer element, or \e NULL if the layer element is not defined for this layer.
	  */
	KFbxLayerElement* GetLayerElementOfType(KFbxLayerElement::ELayerElementType pType, bool pIsUV=false);

	/** Get the layer element description of the specified type for this layer.
	  * \param pType     The Layer element type required. Supported values are KFbxLayerElement::eNORMAL, KFbxLayerElement::eMATERIAL, 
	  *                  KFbxLayerElement::eTEXTURE, KFbxLayerElement::ePOLYGON_GROUP, KFbxLayerElement::eUV and KFbxLayerElement::eVERTEX_COLOR. 
	  *                       - Calling with eNORMAL is equivalent to calling GetNormals().
	  *                       - Calling with eMATERIAL is equivalent to calling GetMaterials().
	  *                       - Calling with ePOLYGON_GROUP is equivalent to calling GetPolygonGroups().
	  *                       - Calling with eUV is equivalent to calling GetUVs().
	  *                       - Calling with eVERTEX_COLOR is equivalent to calling GetVertexColors().
	  *                       - Calling with eSMOOTHING is equivalent to calling GetSmoothing().
	  *                       - Calling with eUSER_DATA is equivalent to calling GetUserData().
      *                       - Calling with eEMISSIVE_TEXTURES is equivalent to calling GetEmissiveTextures().
      *                       - Calling with eEMISSIVE_FACTOR_TEXTURES is equivalent to calling GetEmissiveFactorTextures().
      *                       - Calling with eAMBIENT_TEXTURES is equivalent to calling GetAmbientTextures().
      *                       - Calling with eAMBIENT_FACTOR_TEXTURES is equivalent to calling GetAmbientFactorTextures().
      *                       - Calling with eDIFFUSE_TEXTURES is equivalent to calling GetDiffuseTextures().
      *                       - Calling with eDIFFUSE_FACTOR_TEXTURES is equivalent to calling GetDiffuseFactorTextures().
      *                       - Calling with eSPECULAR_TEXTURES is equivalent to calling GetSpecularTextures().
      *                       - Calling with eSPECULAR_FACTOR_TEXTURES is equivalent to calling GetSpecularFactorTextures().
      *                       - Calling with eSHININESS_TEXTURES is equivalent to calling GetShininessTextures().
      *                       - Calling with eBUMP_TEXTURES is equivalent to calling GetBumpTextures().
	  *                       - Calling with eNORMALMAP_TEXTURES is equivalent to calling GetNormalMapTextures().
      *                       - Calling with eTRANSPARENT_TEXTURES is equivalent to calling GetTransparentTextures().
      *                       - Calling with eTRANSPARENCY_FACTOR_TEXTURES is equivalent to calling GetTransparencyFactorTextures().
      *                       - Calling with eREFLECTION_TEXTURES is equivalent to calling GetReflectionTextures().
      *                       - Calling with eREFLECTION_FACTOR_TEXTURES is equivalent to calling GetReflectionFactorTextures().
      * \param pIsUV     When \c true, request the UV LayerElement corresponding to the specified Layer Element type.
	  * \return          Pointer to the requested layer element, or \e NULL if the layer element is not defined for this layer.
	  */
	KFbxLayerElement const* GetLayerElementOfType(KFbxLayerElement::ELayerElementType pType, bool pIsUV=false) const;

	/** Set the Normals description for this layer.
	  * \param pNormals     Pointer to the Normals layer element, or \c NULL to remove the Normals definition.
	  * \remarks            A geometry of type KFbxNurb or KFbxPatch should not have Normals defined.
	  */
	void SetNormals(KFbxLayerElementNormal* pNormals);

	/** Set the Materials description for this layer.
	  * \param pMaterials     Pointer to the Materials layer element, or \c NULL to remove the Material definition.
	  */
	void SetMaterials(KFbxLayerElementMaterial* pMaterials);

	/** Set the Polygon Groups description for this layer.
	  * \param pPolygonGroups     Pointer to the Polygon Groups layer element, or \c NULL to remove the Polygon Group definition.
	  */
	void SetPolygonGroups(KFbxLayerElementPolygonGroup* pPolygonGroups);

	/** Set the UVs description for this layer.
	  * \param pUVs     Pointer to the UVs layer element, or \c NULL to remove the UV definition.
	  * \remarks        A geometry of type KFbxNurb or KFbxPatch should not have UVs defined.
	  *                 The Nurbs/Patch parameterization is used as UV parameters to map a texture.
	  */
	void SetUVs(KFbxLayerElementUV* pUVs, KFbxLayerElement::ELayerElementType pTypeIdentifier=KFbxLayerElement::eDIFFUSE_TEXTURES);

	/** Set the Vertex Colors description for this layer.
	  * \param pVertexColors     Pointer to the Vertex Colors layer element, or \c NULL to remove the Vertex Color definition.
	  * \remarks                 A geometry of type KFbxNurb or KFbxPatch should not have Vertex Colors defined, since no vertex exists.
	  */
	void SetVertexColors (KFbxLayerElementVertexColor* pVertexColors);

	/** Set the Smoothing description for this layer.
	  * \param pSmoothing     Pointer to the Smoothing layer element, or \c NULL to remove the Smoothing definition.
	  * \remarks              A geometry of type KFbxNurb or KFbxPatch should not have Smoothing defined.
	  */
	void SetSmoothing (KFbxLayerElementSmoothing* pSmoothing);

	/** Set the User Data for this layer.
	  * \param pUserData     Pointer to the User Data layer element, or \c NULL to remove the User Data.
	  */
	void SetUserData (KFbxLayerElementUserData* pUserData);

	/** Set the visibility for this layer.
	  * \param pUserData     Pointer to the visibility layer element, or \c NULL to remove the visibility.
	  */
	void SetVisibility( KFbxLayerElementVisibility* lVisibility );

	/** Set the EmissiveTextures description for this layer.
	  * \param pTextures     Pointer to the Textures layer element, or \c NULL to remove the Texture definition.
	  */
	void SetEmissiveTextures(KFbxLayerElementTexture* pTextures);

	/** Set the EmissiveFactorTextures description for this layer.
	  * \param pTextures     Pointer to the Textures layer element, or \c NULL to remove the Texture definition.
	  */
	void SetEmissiveFactorTextures(KFbxLayerElementTexture* pTextures);

	/** Set the AmbientTextures description for this layer.
	  * \param pTextures     Pointer to the Textures layer element, or \c NULL to remove the Texture definition.
	  */
	void SetAmbientTextures(KFbxLayerElementTexture* pTextures);

	/** Set the AmbientFactorTextures description for this layer.
	  * \param pTextures     Pointer to the Textures layer element, or \c NULL to remove the Texture definition.
	  */
	void SetAmbientFactorTextures(KFbxLayerElementTexture* pTextures);

	/** Set the DiffuseTextures description for this layer.
	  * \param pTextures     Pointer to the Textures layer element, or \c NULL to remove the Texture definition.
	  */
	void SetDiffuseTextures(KFbxLayerElementTexture* pTextures);

	/** Set the DiffuseFactorTextures description for this layer.
	  * \param pTextures     Pointer to the Textures layer element, or \c NULL to remove the Texture definition.
	  */
	void SetDiffuseFactorTextures(KFbxLayerElementTexture* pTextures);

	/** Set the SpecularTextures description for this layer.
	  * \param pTextures     Pointer to the Textures layer element, or \c NULL to remove the Texture definition.
	  */
	void SetSpecularTextures(KFbxLayerElementTexture* pTextures);

	/** Set the SpecularFactorTextures description for this layer.
	  * \param pTextures     Pointer to the Textures layer element, or \c NULL to remove the Texture definition.
	  */
	void SetSpecularFactorTextures(KFbxLayerElementTexture* pTextures);

	/** Set the ShininessTextures description for this layer.
	  * \param pTextures     Pointer to the Textures layer element, or \c NULL to remove the Texture definition.
	  */
	void SetShininessTextures(KFbxLayerElementTexture* pTextures);

	/** Set the BumpTextures description for this layer.
	  * \param pTextures     Pointer to the Textures layer element, or \c NULL to remove the Texture definition.
	  */
	void SetNormalMapTextures(KFbxLayerElementTexture* pTextures);

	/** Set the BumpTextures description for this layer.
	  * \param pTextures     Pointer to the Textures layer element, or \c NULL to remove the Texture definition.
	  */
	void SetBumpTextures(KFbxLayerElementTexture* pTextures);

	/** Set the TransparentTextures description for this layer.
	  * \param pTextures     Pointer to the Textures layer element, or \c NULL to remove the Texture definition.
	  */
	void SetTransparentTextures(KFbxLayerElementTexture* pTextures);

	/** Set the TransparencyFactorTextures description for this layer.
	  * \param pTextures     Pointer to the Textures layer element, or \c NULL to remove the Texture definition.
	  */
	void SetTransparencyFactorTextures(KFbxLayerElementTexture* pTextures);

	/** Set the ReflectionTextures description for this layer.
	  * \param pTextures     Pointer to the Textures layer element, or \c NULL to remove the Texture definition.
	  */
	void SetReflectionTextures(KFbxLayerElementTexture* pTextures);

	/** Set the ReflectionFactorTextures description for this layer.
	  * \param pTextures     Pointer to the Textures layer element, or \c NULL to remove the Texture definition.
	  */
	void SetReflectionFactorTextures(KFbxLayerElementTexture* pTextures);


	void Clone(KFbxLayer const& pSrcLayer, KFbxSdkManager* pSdkManager);	
	
	//! Assignment operator.
	//KFbxLayer& operator=(KFbxLayer const& pSrcLayer);	
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
	//! Assignment operator.
	KFbxLayer& operator=(KFbxLayer const& pSrcLayer);	
	//@}	
private:

	KFbxLayer();
	virtual ~KFbxLayer();

	void Clear();

	KFbxLayerElementNormal*       mNormals;
	KFbxLayerElementMaterial*     mMaterials;
	KFbxLayerElementPolygonGroup* mPolygonGroups;
	KFbxLayerElementUV*           mUVs;	
	KFbxLayerElementVertexColor*  mVertexColors;
	KFbxLayerElementSmoothing*    mSmoothing;
	KFbxLayerElementUserData*	  mUserData;
	KFbxLayerElementVisibility*   mVisibility;

	KFbxLayerElementUV*      mEmissiveUV;
	KFbxLayerElementUV*      mEmissiveFactorUV;
	KFbxLayerElementUV*      mAmbientUV;
	KFbxLayerElementUV*      mAmbientFactorUV;
	KFbxLayerElementUV*      mDiffuseFactorUV;
	KFbxLayerElementUV*      mSpecularUV;
	KFbxLayerElementUV*      mSpecularFactorUV;
	KFbxLayerElementUV*      mShininessUV;
	KFbxLayerElementUV*      mNormalMapUV;
	KFbxLayerElementUV*      mBumpUV;
	KFbxLayerElementUV*      mTransparentUV;
	KFbxLayerElementUV*      mTransparencyFactorUV;
	KFbxLayerElementUV*      mReflectionUV;
	KFbxLayerElementUV*      mReflectionFactorUV;


	KFbxLayerElementTexture*      mEmissiveTextures;
	KFbxLayerElementTexture*      mEmissiveFactorTextures;
	KFbxLayerElementTexture*      mAmbientTextures;
	KFbxLayerElementTexture*      mAmbientFactorTextures;
	KFbxLayerElementTexture*      mDiffuseTextures;
	KFbxLayerElementTexture*      mDiffuseFactorTextures;
	KFbxLayerElementTexture*      mSpecularTextures;
	KFbxLayerElementTexture*      mSpecularFactorTextures;
	KFbxLayerElementTexture*      mShininessTextures;
	KFbxLayerElementTexture*      mNormalMapTextures;
	KFbxLayerElementTexture*      mBumpTextures;
	KFbxLayerElementTexture*      mTransparentTextures;
	KFbxLayerElementTexture*      mTransparencyFactorTextures;
	KFbxLayerElementTexture*      mReflectionTextures;
	KFbxLayerElementTexture*      mReflectionFactorTextures;

	friend class KFbxLayerContainer;

#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS
};

#include <fbxfilesdk_nsend.h>

#endif // #ifndef _FBXSDK_LAYER_H_



