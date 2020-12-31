/*!  \file kfbxsceneinfo.h
 */
#ifndef _FBXSDK_SCENE_INFO_H_
#define _FBXSDK_SCENE_INFO_H_

/**************************************************************************************

 Copyright © 2005 - 2006 Autodesk, Inc. and/or its licensors.
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
#include <kfbxplugins/kfbxobject.h>

#ifndef MB_FBXSDK
#include <kbaselib_nsuse.h>
#endif

class KStreamFbx; 

#include <fbxfilesdk_nsbegin.h>

class KFbxThumbnail;
class KFbxSdkManager;

/** Contains scene thumbnail and user-defined summary data.
  */
class KFBX_DLL KFbxSceneInfo : public KFbxObject
{
	KFBXOBJECT_DECLARE(KFbxSceneInfo);

public:
	/** User-defined summary data.
	  * These fields are filled by the user to identify/classify
	  * the files.
	  */
	//@{
	//! Title.
	KString mTitle;

	//! Subject.
	KString mSubject;

	//! Author.
	KString mAuthor;

	//! Keywords.
	KString mKeywords;

	//! Revision.
	KString mRevision;

	//! Comment.
	KString mComment;
	//@}

	/** Scene Thumbnail.
	  */
	//@{

	/** Get the scene thumbnail.
	  * \return Pointer to the thumbnail.
	  */
	KFbxThumbnail* GetSceneThumbnail();

	/** Set the scene thumbnail.
	  * \param pSceneThumbnail Pointer to a thumbnail object.
	  */
	void SetSceneThumbnail(KFbxThumbnail* pSceneThumbnail);
	//@}

	/** Clear the content.
	  * Reset all the strings to the empty string and clears 
	  * the pointer to the thumbnail.
	  */
	void Clear();

	//! assignment operator.
	KFbxSceneInfo& operator=(const KFbxSceneInfo& pSceneInfo);

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

	//! Constructor.
	KFbxSceneInfo(KFbxSdkManager& pManager,char const *pName);

	//! Destructor.
	~KFbxSceneInfo();

	KFbxSdkManager* mManager;
	KFbxThumbnail* mSceneThumbnail;

	friend class KStreamFbx;
#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS 
};

typedef KFbxSceneInfo* HKFbxSceneInfo;

#include <fbxfilesdk_nsend.h>

#endif // #ifndef _FBXSDK_SCENE_INFO_H_


