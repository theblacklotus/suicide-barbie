/*!  \file kfbxmarker.h
 */

#ifndef _FBXSDK_MARKER_H_
#define _FBXSDK_MARKER_H_

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

#include <kfbxplugins/kfbxnodeattribute.h>
#include <kfbxplugins/kfbxcolor.h>
#include <kfbxmath/kfbxvector4.h>

#ifndef MB_FBXSDK
#include <kbaselib_nsuse.h>
#endif
#include <kbaselib_forward.h>

#include <fbxfilesdk_nsbegin.h>

class KFbxTakeNode;
class KFbxSdkManager;
class KFbxMarker_internal;

/**	This node attribute contains the properties of a marker.
  * \nosubgrouping
  */
class KFBX_DLL KFbxMarker : public KFbxNodeAttribute
{
	KFBXOBJECT_DECLARE(KFbxMarker);

public:
	//! Return the type of node attribute which is EAttributeType::eMARKER.
	virtual EAttributeType GetAttributeType() const;

	//! Reset the marker to default values.
	void Reset();

	/** \enum EType Marker types.
	  * - \e eSTANDARD
	  * - \e eOPTICAL
	  * - \e eIK_EFFECTOR
	  * - \e eFK_EFFECTOR
	  */
	typedef enum { 
		eSTANDARD, 
		eOPTICAL, 
		eIK_EFFECTOR, 
		eFK_EFFECTOR
	} EType;

	/** Set marker type.
	  * \param pType The type of marker.
	  */
	void SetType(EType pType);

	/** Get marker type.
	  * \return The type of the marker.
	  */
	EType GetType() const;

	/** \enum ELook Marker look.
	  * - \e eCUBE
	  * - \e eHARD_CROSS
	  * - \e eLIGHT_CROSS
	  * - \e eSHPERE
	  */
	typedef enum { 
		eCUBE, 
		eHARD_CROSS, 
		eLIGHT_CROSS, 
		eSPHERE
	} ELook;
	
	/** Set marker look.
	  * \param pLook The look of the marker.
	  */
	void SetLook(ELook pLook);

	/** Get marker look.
	  * \return The look of the marker.
	  */
	ELook GetLook() const;

	/** Set marker size.
	  * \param pSize The size of the marker.
	  */
	void SetSize(double pSize);

	/** Get marker size.
	  * \return The currently set marker size.
	  */
	double GetSize() const;

	/** Set marker color.
	  * \param pColor The color used to display the marker.
	  */
	void SetColor(KFbxColor pColor);

	/** Get marker color.
	  * \return The marker color.
	  */
	KFbxColor GetColor() const;

	/** Set whether a marker label is shown.
	  * \param pShowLabel If set to \c true the marker label is visible.
	  */
	void SetShowLabel(bool pShowLabel);

	/** Get whether a marker label is shown.
	  * \return \c true if the marker label is visible.
	  */
	bool GetShowLabel() const;

	/** Set the IK pivot position.
	  * \param pIKPivot The translation in local coordinates.
	  */
	void SetIKPivot(KFbxVector4& pIKPivot);

	/**  Get the IK pivot position.
	  * \return The pivot position vector.
	  */
	KFbxVector4 GetIKPivot() const;

	/**
	  * \name Default Animation Values
	  * This set of functions provides direct access to default
	  * animation values specific to a marker. The default animation 
	  * values are found in the default take node of the associated node.
	  * Hence, these functions only work if the marker has been associated
	  * with a node.
	  */
	//@{

	/** Get default occlusion.
	  * \return 0.0 if optical marker animation is valid by default, 1.0 if it is occluded by default.
	  * \remarks This function only works if marker type is set to KFbxMarker::eOPTICAL.
	  */
	double GetDefaultOcclusion() const;

	/** Set default occlusion.
	  * \param pOcclusion 0.0 if optical marker animation is valid by default, 1.0 if it is occluded by default.
	  * \remarks This function only works if marker type is set to KFbxMarker::eOPTICAL.
	  */
	void SetDefaultOcclusion(double pOcclusion);

	/** Get default IK reach translation.
	  * \return A value between 0.0 and 100.0, 100.0 means complete IK reach.
	  * \remarks This function only works if marker type is set to KFbxMarker::eIK_EFFECTOR.
	  */
	double GetDefaultIKReachTranslation() const;

	/** Set default IK reach translation.
	  * \param pIKReachTranslation A value between 0.0 and 100.0, 100.0 means complete IK reach.
	  * \remarks This function only works if marker type is set to KFbxMarker::eIK_EFFECTOR.
	  */
	void SetDefaultIKReachTranslation(double pIKReachTranslation);

	/** Get default IK reach rotation.
	  * \return A value between 0.0 and 100.0, 100.0 means complete IK reach.
	  * \remarks This function only works if marker type is set to KFbxMarker::eIK_EFFECTOR.
	  */
	double GetDefaultIKReachRotation() const;

	/** Set default IK reach rotation.
	  * \param pIKReachRotation A value between 0.0 and 100.0, 100.0 means complete IK reach.
	  * \remarks This function only works if marker type is set to KFbxMarker::eIK_EFFECTOR.
	  */
	void SetDefaultIKReachRotation(double pIKReachRotation);

	//@}

	/**
	  * \name Obsolete functions
	  */
	//@{

	/** Get default color.
	  * \return Input parameter filled with appropriate data.
	  * \remarks Marker color can not be animated anymore.
	  */
	KFbxColor& GetDefaultColor(KFbxColor& pColor) const;

	/** Set default color.
	  * \remarks Marker color can not be animated anymore.
	  */
	void SetDefaultColor(KFbxColor& pColor);

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

public:

	virtual KObject*  GetFbxObject_internal();
	virtual KObject const*  GetFbxObject_internal() const;

	// Clone
	virtual KFbxObject* Clone(KFbxObject::ECloneType pCloneType) const;

protected:
	static char const* GetNamePrefix() { return 0; }

	KFbxMarker(KFbxSdkManager& pManager, char const* pName);
	virtual ~KFbxMarker();

	virtual void Destruct(bool pRecursive, bool pDependents);

	//! Assignment operator.
    KFbxMarker& operator=(KFbxMarker const& pMarker);

	/** Add channels specific to a marker attribute.
      * \param pTakeNode Take node to add specialized channels to.
	  */
	virtual void AddChannels(KFbxTakeNode *pTakeNode);

	/** Remove channels specific to a marker attribute.
      * \param pTakeNode Take node to remove specialized channels from.
	  */
	virtual void RemoveChannels(KFbxTakeNode *pTakeNode);

	/**
	  *	Used to retrieve the KProperty list from an attribute.
	  */

	virtual KString		GetTypeName() const;
	virtual KStringList	GetTypeFlags() const;

	EType mType;

	// Properties Handler
	KFbxMarker_internal* mPH;

	friend class KFbxReaderFbx;

#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS

};

typedef KFbxMarker* HKFbxMarker;

#include <fbxfilesdk_nsend.h>

#endif // #ifndef _FBXSDK_MARKER_H_


