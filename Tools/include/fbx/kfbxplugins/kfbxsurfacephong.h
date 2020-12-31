/*!  \file kfbxsurfacephong.h
 */

#ifndef _FBXSDK_SURFACEPHONG_H_
#define _FBXSDK_SURFACEPHONG_H_

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

#include <kfbxplugins/kfbxsurfacelambert.h>
#include <kfbxplugins/kfbxcolor.h>
#include <kfbxplugins/kfbxgroupname.h>

#include <klib/kerror.h>

#include <fbxfilesdk_nsbegin.h>

/** Material settings.
  * \nosubgrouping
  * A material is attached to an instance of class KFbxGeometry
  * by calling KFbxGeometry::AddMaterial(). Materials can be shared among 
  * many instances of class KFbxGeometry.
  */
class KFBX_DLL KFbxSurfacePhong : public KFbxSurfaceLambert
{
	KFBXOBJECT_DECLARE(KFbxSurfacePhong);

public:
	/**
	 * \name Material properties
	 */
	//@{
	
	/** Get the specular color property.
	 */
	KFbxProperty GetSpecularColor() const;
	
	/** Get the specular factor property. This factor is used to
	 * attenuate the specular color.
	 */
	KFbxProperty GetSpecularFactor() const;
	
	/** Get the shininess property. This property controls the aspect
	 * of the shiny spot. It is the specular exponent in the Phong
	 * illumination model.
	 */
	KFbxProperty GetShininess() const;
	
	/** Get the reflection color property. This property is used to
	 * implement reflection mapping.
	 */
	KFbxProperty GetReflectionColor() const;
	
	/** Get the reflection factor property. This property is used to
	 * attenuate the reflection color.
	 */
	KFbxProperty GetReflectionFactor() const;
	
	//@}

	/** 
	 * \name Deprecated KFbxMaterial interface.
	 * \remarks These functions are provided to ease the transition
	 * to the new surface materials.
	 */
	
	//@{
	/** Set default material specular color.
	  * \param pColor Material specular color.
	  */
	K_DEPRECATED bool SetDefaultSpecularColor(KFbxColor const& pColor);
	
	/** Get default material specular color.
	  * \param pColor Material specular color.
	  * \return Input parameter filled with appropriate data.
	  */
	K_DEPRECATED bool GetDefaultSpecularColor(KFbxColor& pColor) const;

	/** Set default material shininess.
	  * \param pShininess Shininess value from 0 to 1, 0 meaning 100% matte.
	  */
	K_DEPRECATED bool SetDefaultShininess(double pShininess);

	/** Get default material shininess.
	  * \return The material shininess. The value is in the interval [0, 1],
	  0 meaning 100% matte.
	  */
	K_DEPRECATED bool	GetDefaultShininess(double& pShininess) const;

	/** Set default material reflectivity.
	  * \param pReflectivity Reflectivity value from 0 to 1, 0 meaning no reflectivity.
	  */
	K_DEPRECATED bool SetDefaultReflectivity(double pReflectivity);

	/** Get default material reflectivity.
	  * \return The material reflectivity. The value is in the interval [0,1],
	  0 meaning no reflectivity.
	  */
	K_DEPRECATED bool	GetDefaultReflectivity(double& pReflectivity) const;

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

	// Clone
	virtual KFbxObject* Clone(KFbxObject::ECloneType pCloneType) const;

	bool operator==(KFbxSurfacePhong const& pMaterial) const;

protected:
	static char const* GetNamePrefix() { return MATERIAL_PREFIX; }
	
	KFbxSurfacePhong(KFbxSdkManager& pManager, char const* pName);
	virtual ~KFbxSurfacePhong();

	virtual void Destruct(bool pRecursive, bool pDependents);

	//! Assignment operator.
    KFbxSurfacePhong& operator=(KFbxSurfacePhong const& pMaterial);

	// From KFbxObject
	KFbxObject::ENameSpace GetNameSpace() const; 
	virtual KString		GetTypeName() const;
	virtual KStringList	GetTypeFlags() const;

	// Local
	void Init();

	KFbxPropertyDouble3 Specular;
	KFbxPropertyDouble1 SpecularFactor;
	KFbxPropertyDouble1 Shininess;

	KFbxPropertyDouble3 Reflection;
	KFbxPropertyDouble1 ReflectionFactor;

#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS 

};

typedef KFbxSurfaceMaterial* HKFbxSurfaceMaterial;

#include <fbxfilesdk_nsend.h>

#endif


