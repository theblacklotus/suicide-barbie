/*!  \file kfbxsurfacematerial.h
 */

#ifndef _FBXSDK_SURFACEMATERIAL_H_
#define _FBXSDK_SURFACEMATERIAL_H_

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

#include <kaydaradef.h>
#ifndef KFBX_DLL 
	#define KFBX_DLL K_DLLIMPORT
#endif

#include <kaydara.h>

#include <kfbxplugins/kfbxtakenodecontainer.h>
#include <kfbxplugins/kfbxcolor.h>
#include <kfbxplugins/kfbxgroupname.h>
#include <kfbxplugins/kfbxnodeattribute.h>

#include <klib/kerror.h>

#include <fbxfilesdk_nsbegin.h>

class KFbxSurfaceMaterial_internal;

/** Material settings.
  * \nosubgrouping
  * A material is attached to an instance of class KFbxGeometry
  * by calling KFbxGeometry::AddMaterial(). Materials can be shared among 
  * many instances of class KFbxGeometry.
  */
class KFBX_DLL KFbxSurfaceMaterial : public KFbxTakeNodeContainer
{
	KFBXOBJECT_DECLARE(KFbxSurfaceMaterial);

public:
	/**
	  * \name Standard Material Property Names
	  */
	//@{	

	static char const* sShadingModel;
	static char const* sMultiLayer;
	
	static char const* sEmissive;
	static char const* sEmissiveFactor;
	
	static char const* sAmbient;
	static char const* sAmbientFactor;
	
	static char const* sDiffuse;
	static char const* sDiffuseFactor;
	
	static char const* sSpecular;
	static char const* sSpecularFactor;
	static char const* sShininess;
	
	static char const* sBump;
	
	static char const* sTransparentColor;
	static char const* sTransparencyFactor;
	
	static char const* sReflection;
	static char const* sReflectionFactor;
	
	//@}	

	/**
	  * \name Material Properties
	  */
	//@{	

	//! Reset the material to default values.
	void Reset();
	
	KFbxProperty GetShadingModel() const;
	KFbxProperty GetMultiLayer() const;
	
	void AddChannels(KFbxTakeNode *pTakeNode);
	
	//@}	

	/** 
	 * \name Deprecated KFbxMaterial interface.
	 * \remarks These functions are provided to ease the transition
	 * from KFbxMaterial to the new surface materials.
	 */
	//@{

	/** Set material shading model.
	  * \param pShadingModel Shading model for the material.
	  * \remarks "phong" is the default shading model.
	  */
	K_DEPRECATED bool SetShadingModel(KString const& pShadingModel);

	/** Get material shading model.
	  * \return The shading model type string.
	  */
	K_DEPRECATED bool GetShadingModel(KString& pShadingModel) const;

	/**	Set multi layer state flag.
	  * \param pMultiLayer If \c true, multi-layering is enabled.
	  */
	K_DEPRECATED bool SetMultiLayer(bool pMultiLayer);

	/**	Get multilayer state.
	  * \return The state of the multy-layer flag.
	  */
	K_DEPRECATED bool GetMultiLayer(bool& pMultiLayer) const;

	//@}

	/**
	  * \name Error Management
	  */
	//@{

	/** Retrieve error object.
	  *	\return Reference to error object.
	  */
	KError const& GetError() const;

	/** \enum EError Error identifiers.
	  * - \e eTAKE_NODE_ERROR
	  * - \e eERROR_COUNT
	  */
	typedef enum 
	{
		eTAKE_NODE_ERROR,
		eERROR_COUNT
	} EError;	

	/** Get last error code.
	  *	\return Last error code.
	  */
	EError GetLastErrorID() const;

	/** Get last error string.
	  *	\return Textual description of the last error.
	  */
	char const* GetLastErrorString() const;

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

	virtual KObject*  GetFbxObject_internal();
	virtual KObject const*  GetFbxObject_internal() const;
	
	// Clone
	virtual KFbxObject* Clone(KFbxObject::ECloneType pCloneType) const;

protected:
	bool SetColorParameter(KFbxProperty pProperty, KFbxColor const& pColor);
	bool GetColorParameter(KFbxProperty pProperty, KFbxColor& pColor) const;
	bool SetDoubleParameter(KFbxProperty pProperty, double pDouble);
	bool GetDoubleParameter(KFbxProperty pProperty, double pDouble) const;
	
	static char const* GetNamePrefix() { return MATERIAL_PREFIX; }
	
	KFbxSurfaceMaterial(KFbxSdkManager& pManager, char const* pName);
	virtual ~KFbxSurfaceMaterial();

	virtual void Destruct(bool pRecursive, bool pDependents);

	//! Assignment operator.
    KFbxSurfaceMaterial& operator=(KFbxSurfaceMaterial const& pMaterial);

	// Comparison operator
	bool operator==(KFbxSurfaceMaterial const& pMaterial) const;

	// From KFbxObject
	KFbxObject::ENameSpace GetNameSpace() const; 
	virtual KString		GetTypeName() const;
	virtual KStringList	GetTypeFlags() const;

	// Local
	void Init();

	KError  mError;

	// Internal Property Handler
	KFbxSurfaceMaterial_internal* mPH;
	
	KFbxPropertyString ShadingModel;
	KFbxPropertyBool1 MultiLayer;

	friend class KFbxLayerContainer;
	friend class KFbxReaderFbx;
	friend class KFbxReaderFbx6;
	friend class KFbxWriterFbx6;

#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS 

};

typedef KFbxSurfaceMaterial* HKFbxSurfaceMaterial;

#include <fbxfilesdk_nsend.h>

#endif


