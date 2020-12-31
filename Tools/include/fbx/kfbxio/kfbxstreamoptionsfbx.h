/*!  \file kfbxstreamoptionsfbx.h
 */

#ifndef _FBXSDK_KFbxStreamOptionsFbxWriter_H_
#define _FBXSDK_KFbxStreamOptionsFbxWriter_H_
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


#define KFBXSTREAMOPT_FBX_CURRENT_TAKE_NAME "CURRENT TAKE NAME"
#define KFBXSTREAMOPT_FBX_PASSWORD "PASSWORD"
#define KFBXSTREAMOPT_FBX_PASSWORD_ENABLE "PASSWORD ENABLE"
#define KFBXSTREAMOPT_FBX_MODEL "MODEL"
#define KFBXSTREAMOPT_FBX_TEXTURE "TEXTURE"
#define KFBXSTREAMOPT_FBX_MATERIAL "MATERIAL"
#define KFBXSTREAMOPT_FBX_MEDIA "MEDIA"
#define KFBXSTREAMOPT_FBX_LINK "LINK"
#define KFBXSTREAMOPT_FBX_SHAPE "SHAPE"
#define KFBXSTREAMOPT_FBX_GOBO "GOBO"
#define KFBXSTREAMOPT_FBX_ANIMATION "ANIMATION"
#define KFBXSTREAMOPT_FBX_CHARACTER "CHARACTER"
#define KFBXSTREAMOPT_FBX_GLOBAL_SETTINGS "GLOBAL SETTINGS"
#define KFBXSTREAMOPT_FBX_PIVOT "PIVOT"
#define KFBXSTREAMOPT_FBX_MERGE_LAYER_AND_TIMEWARP "MERGE LAYER AND TIMEWARP"
#define KFBXSTREAMOPT_FBX_CONSTRAINT "CONSTRAINT"
#define KFBXSTREAMOPT_FBX_EMBEDDED "EMBEDDED"
#define KFBXSTREAMOPT_FBX_MODEL_COUNT "MODEL COUNT"
#define KFBXSTREAMOPT_FBX_DEVICE_COUNT "DEVICE COUNT"
#define KFBXSTREAMOPT_FBX_CHARACTER_COUNT "CHARACTER COUNT"
#define KFBXSTREAMOPT_FBX_ACTOR_COUNT "ACTOR COUNT"
#define KFBXSTREAMOPT_FBX_CONSTRAINT_COUNT "CONSTRAINT_COUNT"
#define KFBXSTREAMOPT_FBX_MEDIA_COUNT "MEDIA COUNT"
#define KFBXSTREAMOPT_FBX_TEMPLATE "TEMPLATE"


#include <kaydaradef.h>
#ifndef KFBX_DLL 
	#define KFBX_DLL K_DLLIMPORT
#endif

#include <kfbxplugins/kfbxsdkmanager.h>
#include <kfbxio/kfbxstreamoptions.h>
#include <kfbxplugins/kfbxobject.h>
#include <klib/karrayul.h>
#include <kfbxplugins/kfbxtakeinfo.h>
#include <kfbxplugins/kfbxsceneinfo.h>



#include <fbxfilesdk_nsbegin.h>
/**	\brief This class is used for accessing the Import options of Fbx files.
  * The content of KfbxStreamOptionsFbx is stored in the inherited Property of its parent (KFbxStreamOptions).
  */
class KFBX_DLL KFbxStreamOptionsFbxReader : public KFbxStreamOptions
{

	KFBXOBJECT_DECLARE(KFbxStreamOptionsFbxReader);
public:

	/** Reset all options to default values
	*/
	void Reset();

#ifndef DOXYGEN_SHOULD_SKIP_THIS
public:
	virtual KFbxObject* Clone(KFbxObject::ECloneType pCloneType) const;
	
	KArrayTemplate<HKFbxTakeInfo> mTakeInfo;
	HKFbxSceneInfo mSceneInfo;
	
protected:
	KFbxStreamOptionsFbxReader(KFbxSdkManager& pManager, char const* pName);
	virtual ~KFbxStreamOptionsFbxReader();
	virtual void Destruct(bool pRecursive, bool pDependents);
#endif
};



/**	\brief This class is used for accessing the Export options of Fbx files.
  * The content of KfbxStreamOptionsFbx is stored in the inherited Property of its parent (KFbxStreamOptions).
  */
class KFBX_DLL KFbxStreamOptionsFbxWriter : public KFbxStreamOptions
{
	KFBXOBJECT_DECLARE(KFbxStreamOptionsFbxWriter);
public:
	/** Reset all options to default values
	*/
	virtual void Reset();
#ifndef DOXYGEN_SHOULD_SKIP_THIS
public:
	
	virtual KFbxObject* Clone(KFbxObject::ECloneType pCloneType) const;
	KArrayTemplate<HKFbxTakeInfo> mTakeInfo;
	HKFbxSceneInfo mSceneInfo;
protected:
	KFbxStreamOptionsFbxWriter(KFbxSdkManager& pManager, char const* pName);
	virtual ~KFbxStreamOptionsFbxWriter();
	virtual void Destruct(bool pRecursive, bool pDependents);
#endif
};




#include <fbxfilesdk_nsend.h>
#endif