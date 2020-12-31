/*!  \file kfbxtakenode.h
 */

#ifndef _FBXSDK_TAKE_NODE_H_
#define _FBXSDK_TAKE_NODE_H_

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

#include <klib/kerror.h>

#ifndef MB_FBXSDK
#include <kbaselib_nsuse.h>
#endif
#include <kbaselib_forward.h>

#include <kfcurve/kfcurve_forward.h>
#ifndef MB_FBXSDK
#include <kfcurve/kfcurve_nsuse.h>
#endif

#include <fbxfilesdk_nsbegin.h>

class KFbxGeometry;
class KFbxVector4;
class KFbxProperty;
class KFbxObject;

#define KFBXTAKENODE_DEFAULT_NAME			"Default"
#define KFBXTAKENODE_ROOT_CURVE_NODE_NAME	"Defaults"


/** \brief A take node contains channels of animation.
  *	An object derived from class KFbxTakeNodeContainer contains multiple take 
  * nodes to hold animation data. 
  * 
  * In the case of class KFbxNode, a take node contains all animation curves 
  * necessary to define the animation for a node and its attribute. By having 
  * multiple take nodes, a node might be animated differently. A scene 
  * description held in a node hierarchy can be animated differently using 
  * different animation take nodes.
  *	 
  * This object can be used to access the different animation curves
  *	that define a take node. Some channels are only accessible once the
  * proper node attribute has been associated with an object of type KFbxNode.
  * \nosubgrouping
  */
class KFBX_DLL KFbxTakeNode
{

public:
	/**
	  * \name Take Node creation/destruction.
	  */
	//@{

	/** Constructor.
	  * \param pName The name of the take this node is part of.
	  */
	KFbxTakeNode(char* pName = KFBXTAKENODE_DEFAULT_NAME);

	/** Copy constructor.
	  * \param pTakeNode A node from which data is cloned.
	  */
	KFbxTakeNode(KFbxTakeNode& pTakeNode);

	//! Destructor.
	~KFbxTakeNode();

	//@}

	/**
	  * \name Take Node name management.
	  */
	//@{

    /** Set take node name.
	  * \param pName The name of the take this node is part of.
	  */
    void SetName(char* pName);

	/** Get take node name.
	  * \return Return the name of the take this node is part of.
	  */
	char* GetName();

	//@}

	/**
	  * \name Transform Channels Access
	  * These channels are only accessible when a take node is part of a node.
	  */
	//@{

	/** Get pointer to root KFCurveNode object.
	  * \remarks All the channels of a take node are accessible by browsing
	  * the KFCurveNode tree under this object.
	  */
	KFCurveNode* GetKFCurveNode();

	/** Get X translation channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetTranslationX();

	/** Get Y translation channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetTranslationY();

	/** Get Z translation channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetTranslationZ();

	/** Get X rotation channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetEulerRotationX();

	/** Get Y rotation channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */ 
	KFCurve* GetEulerRotationY();

	/** Get Z rotation channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetEulerRotationZ();

	/** Get X scale channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetScaleX();

	/** Get Y scale channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetScaleY();

	/** Get Z scale channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetScaleZ();

	/** Get visibility channel.
	  * A node is visible if this parameter is higher than 0. It is invisible otherwise.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetVisibility();

	//@}

	/**
	  * \name Light Channels Access
	  * These channels are only accessible when a take node is part of a node
	  * with an associated light node attribute.
	  */
	//@{	

	/** Get red color channel.
	  * This parameter has a scale from 0 to 1, 1 meaning full intensity.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetColorR();

	/** Get green color channel.
	  * This parameter has a scale from 0 to 1, 1 meaning full intensity.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetColorG();

	/** Get blue color channel.
	  * This parameter has a scale from 0 to 1, 1 meaning full intensity.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetColorB();

	/** Get light intensity channel.
	  * This parameter has a scale from 0 to 200, 200 meaning full intensity. 
	  * This parameter's default value is 100.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetLightIntensity();

	/** Get cone angle channel for a spot light.
	  * This parameter has a scale from 0 to 160 degrees.
	  * This parameter has no effect if the light type is not set to \e eSPOT.
	  * Its default value is 45 degrees.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetLightConeAngle();

	/** Get fog intensity channel.
	  * This parameter has a scale from 0 to 200, 200 meaning full fog opacity.
	  * This parameter has no effect if the light type is not set to \e eSPOT.
	  * Its default value is 50.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetLightFog();

	//@}

	/**
	  * \name Camera Channels Access 
	  * These channels are only accessible when a take node is part of a node
	  * with an associated camera node attribute.
	  */
	//@{
	
	/** Get field of view channel.
	  * When the camera aperture mode is set to \e eHORIZONTAL, this parameter 
	  * sets the horizontal field of view in degrees and the vertical field of
	  * view is adjusted accordingly.
	  * When the camera aperture mode is set to \e eVERTICAL, this parameter 
	  * sets the vertical field of view in degrees and the horizontal field of
	  * view is adjusted accordingly.
	  * This parameter has no effect if the camera aperture mode is set to
	  * \e eHORIZONTAL_AND_VERTICAL. Its default value is 25.115.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetCameraFieldOfView();

	/** Get field of view in X channel.
	  * When the camera aperture mode is set to \e eHORIZONTAL_AND_VERTICAL, 
	  * this parameter gets the horizontal field of view in degrees.
	  * This parameter has no effect otherwise. Its default value is 40.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetCameraFieldOfViewX();

	/** Get field of view in Y channel.
	  * When the camera aperture mode is set to \e eHORIZONTAL_AND_VERTICAL, 
	  * this parameter gets the vertical field of view in degrees.
	  * This parameter has no effect otherwise. Its default value is 40.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetCameraFieldOfViewY();

	/** Get focal length channel.
	  * This channel is only valid if the field of view is not animated.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetCameraFocalLength();

	/** Get the horizontal optical center channel.
	  * This parameter gets the optical center horizontal offset when the 
	  * camera aperture mode is set to \e eHORIZONTAL_AND_VERTICAL. Its
	  * default value is 0.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetCameraOpticalCenterX();

	/** Get the vertical optical center channel.
	  * This parameter gets the optical center vertical offset when the 
	  * camera aperture mode is set to \e eHORIZONTAL_AND_VERTICAL. Its
	  * default value is 0.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetCameraOpticalCenterY();

	/** Get camera roll channel in degrees.
	  * This parameter's default value is 0 degrees.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetCameraRoll();

	/** Get camera turn table channel in degrees.
	  * This parameter's default value is 0 degrees.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetCameraTurnTable();

	/** Get camera background red color channel.
	  * This parameter has a scale from 0 to 1, 1 meaning full intensity.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetBackgroundColorR();

	/** Get camera background green color channel.
	  * This parameter has a scale from 0 to 1, 1 meaning full intensity.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetBackgroundColorG();

	/** Get camera background blue color channel.
	  * This parameter has a scale from 0 to 1, 1 meaning full intensity.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetBackgroundColorB();

	//@}

	/**
	  * \name Camera Switcher Channels Access 
	  * These channels are only accessible when a take node is part of a node
	  * with an associated camera switcher node attribute.
	  */
	//@{
	
	/** Get camera index channel.
	  * This parameter has an integer scale from 1 to the number of cameras in the scene.
	  * This parameter's default value is 1.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetCameraIndex();

	//@}


	/**
	  * \name Geometry Channels Access 
	  * These channels are only accessible when a take node is part of a node
	  * with an associated geometry node attribute.
	  */
	//@{

	/** Get a shape channel.
	  * This parameter has a scale from 0 to 100, 100 meaning full shape deformation.
	  * This parameter's default value is 0.
	  * \param pGeometry Pointer to geometry, required because meshes, nurbs and
	  * patches may have different naming schemes for shape channels.
	  * \param pShapeIndex Shape index.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetShapeChannel(KFbxGeometry* pGeometry, int pShapeIndex);

	//@}

	/**
	  * \name Marker Channels Access 
	  * These channels are only accessible when a take node is part of a node
	  * with an associated marker node attribute.
	  */
	//@{

	/** Get marker occlusion channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  * \remarks This channel is only accessible if the associated marker node attribute 
	  * is of type \e KFbxMarker::eOPTICAL.
	  */
	KFCurve* GetOcclusion();

	/** Get marker IK reach translation channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  * \remarks This channel is only accessible if the associated marker node attribute 
	  * is of type \e KFbxMarker::eIK_EFFECTOR.
	  */
	KFCurve* GetIKReachTranslation();

	/** Get marker IK reach rotation channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  * \remarks This channel is only accessible if the associated marker node attribute 
	  * is of type \e KFbxMarker::eIK_EFFECTOR.
	  */
	KFCurve* GetIKReachRotation();

	//@}

	/**
	  * \name Texture Channels Access
	  * These channels are only accessible when a take node is part of a texture.
	  */
	//@{
	
	/** Get texture X translation channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetTextureTranslationX();
	/** Get texture X translation channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve const* GetTextureTranslationX() const;

	/** Get texture Y translation channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetTextureTranslationY();
	/** Get texture Y translation channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve const* GetTextureTranslationY() const;

	/** Get texture Z translation channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetTextureTranslationZ();
	/** Get texture Z translation channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve const* GetTextureTranslationZ() const;

	/** Get texture X rotation channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetTextureEulerRotationX();
	/** Get texture X rotation channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve const* GetTextureEulerRotationX() const;

	/** Get texture Y rotation channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */ 
	KFCurve* GetTextureEulerRotationY();
	/** Get texture Y rotation channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */ 
	KFCurve const* GetTextureEulerRotationY() const;

	/** Get texture Z rotation channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetTextureEulerRotationZ();
	/** Get texture Z rotation channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve const* GetTextureEulerRotationZ() const;

	/** Get texture X scale channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetTextureScaleX();
	/** Get texture X scale channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve const* GetTextureScaleX() const;

	/** Get texture Y scale channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetTextureScaleY();
	/** Get texture Y scale channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve const* GetTextureScaleY() const;

	/** Get texture Z scale channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetTextureScaleZ();
	/** Get texture Z scale channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve const* GetTextureScaleZ() const;

	/** Get texture alpha channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetTextureAlpha();

	//@}

	/**
	  * \name Material Channels Access 
	  * These channels are only accessible when a take node is part of a material.
	  */
	//@{	

	/** Get material emissive red color channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetMaterialEmissiveColorR();

	/** Get material emissive green color channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetMaterialEmissiveColorG();

	/** Get material emissive blue color channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetMaterialEmissiveColorB();

	/** Get material ambient red color channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetMaterialAmbientColorR();

	/** Get material ambient green color channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetMaterialAmbientColorG();

	/** Get material ambient blue color channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetMaterialAmbientColorB();

	/** Get material diffuse red color channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetMaterialDiffuseColorR();

	/** Get material diffuse green color channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetMaterialDiffuseColorG();

	/** Get material diffuse blue color channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c  KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetMaterialDiffuseColorB();

	/** Get material specular red color channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetMaterialSpecularColorR();

	/** Get material specular green color channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetMaterialSpecularColorG();

	/** Get material specular blue color channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetMaterialSpecularColorB();

	/** Get material opacity channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetMaterialOpacity();

	/** Get material reflectivity channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetMaterialReflectivity();

	/** Get material shininess channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetMaterialShininess();

	//@}

	/**
	  * \name Constraint Channels Access 
	  * These channels are only accessible when a take node is part of a constraint.
	  */
	//@{	

	/** Get Weight channel.
	  *	\param Object that we want the KFCurve.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetConstraintObjectWeight(KFbxObject* pObject);

	/** Get X position offset channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetPositionConstraintOffsetX();

	/** Get Y position offset channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetPositionConstraintOffsetY();

	/** Get Z position offset channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetPositionConstraintOffsetZ();

	/** Get X rotation offset channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetRotationConstraintOffsetX();

	/** Get Y rotation offset channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetRotationConstraintOffsetY();

	/** Get Z rotation offset channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetRotationConstraintOffsetZ();

	/** Get X scale offset channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetScaleConstraintOffsetX();

	/** Get Y scale offset channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetScaleConstraintOffsetY();

	/** Get Z scale offset channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetScaleConstraintOffsetZ();

	/** Get X rotation offset channel.
	  *	\param Object that we want the KFCurve.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetParentConstraintRotationOffsetX(KFbxObject* pObject);

	/** Get Y rotation offset channel.
	  *	\param Object that we want the KFCurve.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetParentConstraintRotationOffsetY(KFbxObject* pObject);

	/** Get Z rotation offset channel.
	  *	\param Object that we want the KFCurve.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetParentConstraintRotationOffsetZ(KFbxObject* pObject);

	/** Get X translation offset channel.
	  *	\param Object that we want the KFCurve.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetParentConstraintTranslationOffsetX(KFbxObject* pObject);

	/** Get Y translation offset channel.
	  *	\param Object that we want the KFCurve.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetParentConstraintTranslationOffsetY(KFbxObject* pObject);

	/** Get Z translation offset channel.
	  *	\param Object that we want the KFCurve.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetParentConstraintTranslationOffsetZ(KFbxObject* pObject);

	/** Get X aim offset channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetAimConstraintOffsetX();

	/** Get Y aim offset channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetAimConstraintOffsetY();

	/** Get Z aim offset channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetAimConstraintOffsetZ();

	/** Get X aim world up vector channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetAimConstraintWorldUpVectorX();

	/** Get Y aim world up vector channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetAimConstraintWorldUpVectorY();

	/** Get Z aim world up vector channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetAimConstraintWorldUpVectorZ();

	/** Get X aim vector channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetAimConstraintAimVectorX();

	/** Get Y aim vector channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetAimConstraintAimVectorY();

	/** Get Z aim vector channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetAimConstraintAimVectorZ();

	/** Get X aim up vector channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetAimConstraintUpVectorX();

	/** Get Y aim up vector channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetAimConstraintUpVectorY();

	/** Get Z aim up vector channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetAimConstraintUpVectorZ();

	/** Get single chain ik weight channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetSCIKConstraintWeight();

	/** Get single chain ik twist channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetSCIKConstraintTwist();

	/** Get single chain ik X pole vector channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetSCIKConstraintPoleVectorX();

	/** Get single chain ik Y pole vector channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetSCIKConstraintPoleVectorY();

	/** Get single chain ik Z pole vector channel.
	  *	\return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetSCIKConstraintPoleVectorZ();

	//@}

	/**
	  * \name Property Access 
	  */
	//@{	

	/** Get a user property's component channel.
	  * \param pProperty Specify the property we are interested in.
      * \param pComponentIndex The index of the channel.
	  * \return Animation curve or \c NULL if an error occurred.
	  * \remarks In the last case \c KFbxTakeNode::GetLastErrorID() returns \e eNO_CURVE_FOUND.
	  */
	KFCurve* GetPropertyAnimation(KFbxProperty* pProperty, kUInt pComponentIndex);

	//@}

	/**
	  * \name Error Management
	  */
	//@{

	/** Retrieve error object.
	 *	\return Reference to the take node's error object.
	 */
	KError& GetError();

	/** \enum EError Error identifiers.
	  */
	typedef enum 
	{
		eNO_CURVE_FOUND, /**< The requested FCurve was not found. */
		eERROR_COUNT     /**< Mark the end of the EError enum.    */
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

	/**
	  * \name Utility functions used by some plugins etc.
	  * 
	  */
	//@{

	/** Find out start and end time of the animation.
	  * This function retrieves the including time span for all
	  * the FCurve's time span of this take node.
	  *	\param pStart Reference to receive start time.
	  *	\param pStop Reference to receive end time.
	  *	\return \c true on success, \c false otherwise.
	  */
	bool GetAnimationInterval(KTime& pStart, KTime& pStop);	

	/** Rotates the translation animation at a given angle
	 *	\param pRotation vector containing the rotation.
	 *  \returns \c True if everything went ok.
	 */
	bool AddRotationToTranslation(KFbxVector4 pRotation);


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

	bool IsChannelAnimated(char* pGroup, char* pSubGroup, char* pName);
	bool IsChannelAnimated(char* pGroup, char* pSubGroup, KDataType* pDataType);

private:

	void DeleteRecursive(KFCurveNode* pNode);
	KFCurve* GetKFCurve(char* pGroup, char* pSubGroup, char* pName);
	KFCurve const* GetKFCurve(char* pGroup, char* pSubGroup, char* pName) const;
	KFCurve* GetKFCurve(char* pGroup, char* pSubGroup, KDataType* pDataType);
	KFCurve const* GetKFCurve(char* pGroup, char* pSubGroup, KDataType* pDataType) const;

	KString mName;
	KFCurveNode* mNode;

	KError mError;

	friend class KFbxTakeNodeContainer;

#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS

};

typedef KFbxTakeNode* HKFbxTakeNode;

#include <fbxfilesdk_nsend.h>

#endif // #ifndef _FBXSDK_TAKE_NODE_H_


