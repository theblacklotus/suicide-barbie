/*!  \file kfbxnodefinderbadscale.h
 */

#ifndef _FBXSDK_NODE_FINDER_BAD_SCALE_H_
#define _FBXSDK_NODE_FINDER_BAD_SCALE_H_

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

#include <kfbxplugins/kfbxnodefinder.h>

#include <klib/karrayul.h>
#include <klib/ktime.h>

#include <kfcurve/kfcurve_forward.h>
#ifndef MB_FBXSDK
#include <kfcurve/kfcurve_nsuse.h>
#endif

// Not part of the public FBX SDK
#ifndef K_FBXSDK

#include <fbxfilesdk_nsbegin.h>

/** CLASS KFbxNodeFinderBadScale
*
*	Description:
*
*	Finder for unsupported scale by FiLMBOX (or another software...).
*	1) Scale on non-terminal bones is not supported between Maya and FiLMBOX
*	2) Negative Scale is not supported in FiLMBOX
*	3) Non uniform scale is not supported between Maya and FiLMBOX.
*	   Scale on terminal node will pass.
*/
class KFBX_DLL KFbxNodeFinderBadScale : public KFbxNodeFinder
{

public:
	//! Constructor.
	KFbxNodeFinderBadScale();

	//! Destructor.
	virtual ~KFbxNodeFinderBadScale();

	//! Reset the finder object
	virtual void Reset();

	/**	Get if finder check scale on node.
	*	\return	flag.
	*/
	bool GetCheckBoneScale() const;

	/**	Set if finder check scale on node.
	*	\param iFlag
	*/
	void SetCheckBoneScale(bool iFlag);

	/**	Get if finder check negative scale.
	*	\return flag.
	*/
	bool GetCheckNegativeScale() const;

	/**	Set if finder check negative scale.
	*	\param iFlag
	*/
	void SetCheckNegativeScale(bool iFlag);

	/**	Get if finder check non uniform scale.
	*	\return flag.
	*/
	bool GetCheckNonUniformScale() const;
	
	/** Set if finder check non uniform scale.
	*	\param iFlag
	*/
	void SetCheckNonUniformScale(bool iFlag);
	
	/**	Retrieve the non uniform scale node array that was built from default static data.
	*	\return
	*/
	KArrayTemplate<KFbxNode*>& BoneScaleStaticNodeArray();

	/**	Retrieve the non uniform scale node array that was built from animation curve.
	*	\return
	*/
	KArrayTemplate<KFbxNode*>& BoneScaleAnimatedNodeArray();
	
	/** Retrieve the negative scale node array that was built from default static data.
	*	\return
	*/
	KArrayTemplate<KFbxNode*>& NegativeScaleStaticNodeArray();

	/** Retrieve the negative scale node array that was built from animation curve.
	*	\return
	*/
	KArrayTemplate<KFbxNode*>& NegativeScaleAnimatedNodeArray();

	/**	Retrieve the non uniform scale node array that was built from default static data.
	*	\return
	*/
	KArrayTemplate<KFbxNode*>& NonUniformScaleStaticNodeArray();

	/**	Retrieve the non uniform scale node array that was built from animation curve.
	*	\return
	*/
	KArrayTemplate<KFbxNode*>& NonUniformScaleAnimatedNodeArray();

	/** Get the currently set EPSILON value for variables comparison.
	*	\return
	*/
	double GetEpsilon() const;

	/** Set the EPSILON value to use for number comparison.
	*	\param iEpsilon
	*/
	void SetEpsilon(double iEpsilon);

protected:

	// Check if a node answers to research criterium.
	// Will check for:
	//	1) negative scale 
	//	2) non uniform scale on non terminal node
	virtual bool CheckNode(KFbxNode& iNode);

private:

	typedef enum 
	{
		eValidScale,
		eBoneScale,
		eNegativeScale,
		eNonUniformScale
	} EScaleType;

	void GetCurve(KFCurveNode& iNode, KFCurve* lCurve[3]);
	EScaleType CheckCurve(KFCurve* iCurve [3]);
	EScaleType CheckValue(double iValue[3], int lValueCount);
	bool CheckKFCurveNode
	(
		KFbxNode& pNode,
		KFCurveNode& lCurveNode, 
		KArrayTemplate<KFbxNode*>& lBoneScaleArray,
		KArrayTemplate<KFbxNode*>& lNegativeScaleArray,
		KArrayTemplate<KFbxNode*>& lNonUniformScaleArray
	);

	KTime mFramePeriod;

	bool mCheckBoneScale;
	KArrayTemplate<KFbxNode*> mBoneScaleStaticNodeArray;
	KArrayTemplate<KFbxNode*> mBoneScaleAnimatedNodeArray;

	bool mCheckNegativeScale;
	KArrayTemplate<KFbxNode*> mNegativeScaleStaticNodeArray;
	KArrayTemplate<KFbxNode*> mNegativeScaleAnimatedNodeArray;

	bool mCheckNonUniformScale;
	KArrayTemplate<KFbxNode*> mNonUniformScaleStaticNodeArray;
	KArrayTemplate<KFbxNode*> mNonUniformScaleAnimatedNodeArray;

	double mEpsilon; // by default this value is set to K_DOUBLE_EPSILON
	/* The need to define variables instead of using the symbolic constants
	   directly, comes from the fact that other 3D packages may not carry
	   around all the precision that a float or a double can have so 
	   the CheckNode method may consider two values to be different while
	   in fact, for the third party package, they would be considered identical
	   (this happens a lot with 3D Studio Max where their EPSILON is considered
	   to be 1e-6 instead of FLT_EPSILON
	*/
};

#include <fbxfilesdk_nsend.h>

#endif // #ifndef K_FBXSDK
#endif // #define _FBXSDK_NODE_FINDER_BAD_SCALE_H_


