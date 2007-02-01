/*!  \file kfbxcharacter.h
 */

#ifndef _FBXSDK_CHARACTER_H_
#define _FBXSDK_CHARACTER_H_

/**************************************************************************************

 Copyright © 2002 - 2006 Autodesk, Inc. and/or its licensors.
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

#include <kfbxmath/kfbxvector4.h>

#include <klib/kstring.h>
#include <klib/kerror.h>

#include <kfbxplugins/kfbxconstraint.h>
#include <kfbxplugins/kfbxnodelimits.h>
#include <kfbxplugins/kfbxgroupname.h>

#include <fbxfilesdk_nsbegin.h>

class KFbxNode;
class KFbxControlSet;
class KFbxFilterSet;
class KFbxSdkManager;
class KFbxCharacter_Internal;

/** \enum ECharacterInputType           Character input type.
      * - \e eCharacterInputActor       Not supported.
      * - \e eCharacterInputCharacter   The character's input is another character.
      * - \e eCharacterInputMarkerSet   The character's input is a control rig.
      * - \e eCharacterOutputMarkerSet  Not supported.
      * - \e eCharacterInputStance      The character's input is the stance pose.
      */
typedef enum 
{ 
	eCharacterInputActor = 0, 
	eCharacterInputCharacter = 1, 
	eCharacterInputMarkerSet = 2, 
	eCharacterOutputMarkerSet = 3, 
	eCharacterInputStance = 4 
} ECharacterInputType;


typedef enum  
{	
	eCharacterHips = 0,             // Required
    eCharacterLeftHip,              // Required
    eCharacterLeftKnee,             // Required
	eCharacterLeftAnkle,            // Required
	eCharacterLeftFoot,
	eCharacterRightHip,             // Required
	eCharacterRightKnee,            // Required
	eCharacterRightAnkle,           // Required
	eCharacterRightFoot,
	eCharacterWaist,				// Spine0, Required
	eCharacterChest,		        // Spine1
	eCharacterLeftCollar,
	eCharacterLeftShoulder,         // Required
	eCharacterLeftElbow,            // Required
	eCharacterLeftWrist,            // Required
	eCharacterRightCollar,
	eCharacterRightShoulder,        // Required
	eCharacterRightElbow,           // Required
	eCharacterRightWrist,           // Required
	eCharacterNeck,
	eCharacterHead,                 // Required
	eCharacterLeftHipRoll,
	eCharacterLeftKneeRoll,
	eCharacterRightHipRoll,
	eCharacterRightKneeRoll,
	eCharacterLeftShoulderRoll,
	eCharacterLeftElbowRoll,
	eCharacterRightShoulderRoll,
	eCharacterRightElbowRoll,
	eCharacterSpine2,
	eCharacterSpine3,
	eCharacterSpine4,
	eCharacterSpine5,
	eCharacterSpine6,
	eCharacterSpine7,
	eCharacterSpine8,
	eCharacterSpine9,
	eCharacterLeftThumbA,
	eCharacterLeftThumbB,
	eCharacterLeftThumbC,
	eCharacterLeftIndexA,
	eCharacterLeftIndexB,
	eCharacterLeftIndexC,
	eCharacterLeftMiddleA,
	eCharacterLeftMiddleB,
	eCharacterLeftMiddleC,
	eCharacterLeftRingA,
	eCharacterLeftRingB,
	eCharacterLeftRingC,
	eCharacterLeftPinkyA,
	eCharacterLeftPinkyB,
	eCharacterLeftPinkyC,
	eCharacterRightThumbA,
	eCharacterRightThumbB,
	eCharacterRightThumbC,
	eCharacterRightIndexA,
	eCharacterRightIndexB,
	eCharacterRightIndexC,
	eCharacterRightMiddleA,
	eCharacterRightMiddleB,
	eCharacterRightMiddleC,
	eCharacterRightRingA,
	eCharacterRightRingB,
	eCharacterRightRingC,
	eCharacterRightPinkyA,
	eCharacterRightPinkyB,
	eCharacterRightPinkyC,
	eCharacterReference,
	eCharacterLeftFloor,
	eCharacterRightFloor,
	eCharacterHipsTranslation,
	eCharacterProps0,
	eCharacterProps1,
	eCharacterProps2,
	eCharacterProps3,
	eCharacterProps4,

	// Added in 4.0x as patch.

	eCharacterGameModeParentLeftHipRoll,
	eCharacterGameModeParentLeftKnee,
	eCharacterGameModeParentLeftKneeRoll,
	eCharacterGameModeParentRightHipRoll,
	eCharacterGameModeParentRightKnee,
	eCharacterGameModeParentRightKneeRoll,
	eCharacterGameModeParentLeftShoulderRoll,	
	eCharacterGameModeParentLeftElbow,	
	eCharacterGameModeParentLeftElbowRoll,	
	eCharacterGameModeParentRightShoulderRoll,
	eCharacterGameModeParentRightElbow,		
	eCharacterGameModeParentRightElbowRoll,	


	// Elements added for Dominus.
	// From this point, the enum doesn't have to be synchronized with the HumanIK library.

	eCharacterLeftHandFloor,
	eCharacterRightHandFloor,

	eCharacterLeftHand,
	eCharacterRightHand,
	
	eCharacterNeck1,
	eCharacterNeck2,
	eCharacterNeck3,
	eCharacterNeck4,
	eCharacterNeck5,
	eCharacterNeck6,
	eCharacterNeck7,
	eCharacterNeck8,
	eCharacterNeck9,

	eCharacterLeftInHandThumb,
	eCharacterLeftThumbD,
	
	eCharacterLeftInHandIndex,
	eCharacterLeftIndexD,
	
	eCharacterLeftInHandMiddle,
	eCharacterLeftMiddleD,
	
	eCharacterLeftInHandRing,
	eCharacterLeftRingD,
	
	eCharacterLeftInHandPinky,
	eCharacterLeftPinkyD,

	eCharacterLeftInHandExtraFinger,
	eCharacterLeftExtraFingerA,
	eCharacterLeftExtraFingerB,
	eCharacterLeftExtraFingerC,
	eCharacterLeftExtraFingerD,

	eCharacterRightInHandThumb,
	eCharacterRightThumbD,
	
	eCharacterRightInHandIndex,
	eCharacterRightIndexD,
	
	eCharacterRightInHandMiddle,
	eCharacterRightMiddleD,
	
	eCharacterRightInHandRing,
	eCharacterRightRingD,
	
	eCharacterRightInHandPinky,
	eCharacterRightPinkyD,

	eCharacterRightInHandExtraFinger,
	eCharacterRightExtraFingerA,
	eCharacterRightExtraFingerB,
	eCharacterRightExtraFingerC,
	eCharacterRightExtraFingerD,

	eCharacterLeftInFootThumb,
	eCharacterLeftFootThumbA,
	eCharacterLeftFootThumbB,
	eCharacterLeftFootThumbC,
	eCharacterLeftFootThumbD,
	eCharacterLeftInFootIndex,
	eCharacterLeftFootIndexA,
	eCharacterLeftFootIndexB,
	eCharacterLeftFootIndexC,
	eCharacterLeftFootIndexD,
	eCharacterLeftInFootMiddle,
	eCharacterLeftFootMiddleA,
	eCharacterLeftFootMiddleB,
	eCharacterLeftFootMiddleC,
	eCharacterLeftFootMiddleD,
	eCharacterLeftInFootRing,
	eCharacterLeftFootRingA,
	eCharacterLeftFootRingB,
	eCharacterLeftFootRingC,
	eCharacterLeftFootRingD,
	eCharacterLeftInFootPinky,
	eCharacterLeftFootPinkyA,
	eCharacterLeftFootPinkyB,
	eCharacterLeftFootPinkyC,
	eCharacterLeftFootPinkyD,
	eCharacterLeftInFootExtraFinger,
	eCharacterLeftFootExtraFingerA,
	eCharacterLeftFootExtraFingerB,
	eCharacterLeftFootExtraFingerC,
	eCharacterLeftFootExtraFingerD,

	eCharacterRightInFootThumb,
	eCharacterRightFootThumbA,
	eCharacterRightFootThumbB,
	eCharacterRightFootThumbC,
	eCharacterRightFootThumbD,
	eCharacterRightInFootIndex,
	eCharacterRightFootIndexA,
	eCharacterRightFootIndexB,
	eCharacterRightFootIndexC,
	eCharacterRightFootIndexD,
	eCharacterRightInFootMiddle,
	eCharacterRightFootMiddleA,
	eCharacterRightFootMiddleB,
	eCharacterRightFootMiddleC,
	eCharacterRightFootMiddleD,
	eCharacterRightInFootRing,
	eCharacterRightFootRingA,
	eCharacterRightFootRingB,
	eCharacterRightFootRingC,
	eCharacterRightFootRingD,
	eCharacterRightInFootPinky,
	eCharacterRightFootPinkyA,
	eCharacterRightFootPinkyB,
	eCharacterRightFootPinkyC,
	eCharacterRightFootPinkyD,
	eCharacterRightInFootExtraFinger,
	eCharacterRightFootExtraFingerA,
	eCharacterRightFootExtraFingerB,
	eCharacterRightFootExtraFingerC,
	eCharacterRightFootExtraFingerD,

	eCharacterLastNodeId
} ECharacterNodeId;


typedef enum 
{
	eCharacterGroup_Base,
	eCharacterGroup_Auxiliary,
	eCharacterGroup_Spine,
	eCharacterGroup_Roll,
	eCharacterGroup_Special,
	eCharacterGroup_LeftHand,
	eCharacterGroup_RightHand,
	eCharacterGroup_Props,
	eCharacterGroup_GameModeParent,

	// Added for 4.5 

	eCharacterGroup_Neck,
	eCharacterGroup_LeftFoot,
	eCharacterGroup_RightFoot,

	eLastCharacterGroupId
} ECharacterGroupId;


typedef enum 
{
	eCharacterLink,
	eControlSetLink,
	eControlSetEffector,
	eControlSetEffectorAux
} ECharacterLinkType;

/** \class KFbxCharacterLink
  *
  * \brief This class represents a link between a given character's node and the associated node in the hierarchy. It also contains
  *  the characterization matrix (offset) for the linked character's node.
  */
class KFBX_DLL KFbxCharacterLink
{
public:

	KFbxCharacterLink();
	KFbxCharacterLink(const KFbxCharacterLink& pCharacterLink);
	KFbxCharacterLink& operator=(const KFbxCharacterLink& pCharacterLink);
	void Reset();

    //! The character's node in hierarchy linked to this character link.
	KFbxNode* mNode;

    //! A template name is a naming convention that is used to automatically map
    //! the nodes of other skeletons that use the same naming convention.
	KString mTemplateName;

    //! Get offset position of this character link.
	KFbxVector4 mOffsetT;
    //! Get offset rotation of this character link.
	KFbxVector4 mOffsetR;
    //! Get offset scale of this character link.
	KFbxVector4 mOffsetS;
	//! Get the parent offset rotation of this character link
	KFbxVector4 mParentROffset;

	//! \c true if this character link has a defined rotation space
	bool		mHasRotSpace;
	//! Get the rotation limits of this character link
	KFbxLimits  mRLimits;
	//! Get the PreRotation of this character link
	KFbxVector4 mPreRotation;
	//! Get the PostRotation of this character link
	KFbxVector4 mPostRotation;
	//! Get the rotation order of this character link
	int			mRotOrder;
	//! Get the axis length of this character link
	double		mAxisLen;


///////////////////////////////////////////////////////////////////////////////
//
//  WARNING!
//
//	Anything beyond these lines may not be documented accurately and is 
// 	subject to change without notice.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef DOXYGEN_SHOULD_SKIP_THIS	

	KFbxProperty mPropertyLink;

#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS

};

/** \class KFbxCharacter
  *
  * \brief This class contains all methods to setup an exported character or query information on an imported character.
  *
  * This class also contains some methods for manipulating the KFbxCharacterLink, KFbxControlSet, and KFbxFilterSet.
  * 
  * The most important part of a KFbxCharacter is the KFbxCharacterLink. There is one KFbxCharacterLink for each characterized node.
  * This class contains the associated KFbxNode to the node represented by this link and also the characterization offset of this node. 
  * For more information see KFbxCharacterLink class documentation. 
  * 
  * The character's control set can be accessed by getting a KFbxControlSet reference with the function KFbxCharacter::GetControlSet().
  * All character parameters can be accessed by getting a KFbxFilterSet reference with the function KFbxCharacter::GetFilterSet().
  *
  */
class KFBX_DLL KFbxCharacter : public KFbxConstraint
{
	KFBXOBJECT_DECLARE(KFbxCharacter);

public:
	/** Reset to default values. 
      *     - Input type will be set to eCharacterInputStance.
      *     - Input object will be set to NULL.
      *     - Each Character link will be reset.
      *     - The control set will be reset.
    */
	void Reset();

	/** Set input type and index.
	  * \param pInput           Input type.
	  * \param pInputObject     Pointer to input character if input type equals eCharacterInputCharacter, otherwise \c NULL.
      */
	void SetInput(ECharacterInputType pInputType, KFbxObject* pInputObject = NULL);

	//! Get input type.
	ECharacterInputType GetInputType() const;

	/** Get input actor or character.
	  * \return     Pointer or \c Null, depending on the input type.
      *                  - If the input type is set to eCharacterInputCharacter. The returned pointer can be casted to a pointer of type KFbxCharacter.
	  *                  - \c Null pointer if the input object has not been set, or if the input type is not set to eCharacterInputCharacter.
	  */
	KFbxObject* GetInputObject() const;

	/** Associate a character link to a given character node ID. If a character link already exists for this character node ID,
	  * the character link will be removed.
	  * \param pCharacterNodeId      Character node ID.
	  * \param pCharacterLink        Character link.
	  * \param pUpdateObjectList     Set to \c true to update the object list (default value).
	  * \return                      \c true if successful, \c false otherwise.
	  */
	bool SetCharacterLink(ECharacterNodeId pCharacterNodeId, const KFbxCharacterLink& pCharacterLink, bool pUpdateObjectList = true);

	/** Get a character link associated with a given character node ID.
      * \param pCharacterNodeId     ID of character node requested.
	  * \param pCharacterLink       Optional pointer to receive the character link if function succeeds.
	  * \return                     \c true if successful, \c false otherwise.
	  */
	bool GetCharacterLink(ECharacterNodeId pCharacterNodeId, KFbxCharacterLink* pCharacterLink = NULL) const;

	/** Get control set associated with the character.
      * \return     Return the control set associated with the character.
      */
	KFbxControlSet& GetControlSet() const;

	/** Get filter set associated with the character.
	  * \return     Return the filter set associated with the character.
	  */
	KFbxFilterSet& GetFilterSet() const;

	/** Get number of elements in a given character group.
	  * \param pCharacterGroupId     Character group ID.
	  * \return                      The number of elements in the pCharacterGroupId character group.
	  */
	static int GetCharacterGroupCount(ECharacterGroupId pCharacterGroupId);

	/** Get character node ID of an element in a given character group.
	  * \param pCharacterGroupId     Character group ID.
	  * \param pIndex                Character index ID.
	  * \return                      Character node ID.
	  */
	static ECharacterNodeId GetCharacterGroupElementByIndex(ECharacterGroupId pCharacterGroupId, int pIndex);

    
	/** Get character node name of an element in a given character group.
	  * \param pCharacterGroupId     Character group ID.
	  * \param pIndex                Character index ID.
	  * \return                      Character node name.
	  */
	static char* GetCharacterGroupNameByIndex(ECharacterGroupId pCharacterGroupId, int pIndex);

	/** Get character node version of an element in a given character group.
	  * \param pCharacterGroupId     Character group ID.
	  * \param pIndex                Character index ID.
	  * \return                      Character node version.
	  */
	static int GetCharacterGroupVersionByIndex(ECharacterGroupId pCharacterGroupId, int pIndex);

	/** Find the character group index associated with a given character node name.
	  * \param pName                 Character node name.
	  * \param pForceGroupId         Set to \c true to force the character group ID.
	  * \param pCharacterGroupId     Receives character group ID.
	  * \param pIndex                Receives character index ID.
	  * \return                      \c true if successful, otherwise \c false.
	  */
	static bool FindCharacterGroupIndexByName(char* pName, bool pForceGroupId, ECharacterGroupId& pCharacterGroupId, int& pIndex);

	/** Get character node group and index of a given character node ID.
	  * \param pCharacterNodeId     Character node ID.
	  * \param pVersion             if the Character node ID is found, the method returns the group ID through this parameter
	  * \param pIndex               if the Character node ID is found, the method returns the index through this parameter
	  * \remarks                    Only works for a character node ID that is part of a group.
	  * \return                     \c true if successful, \c false otherwise.
	  */
	static bool GetCharacterGroupIndexByElement(ECharacterNodeId pCharacterNodeId, ECharacterGroupId& pCharacterGroupId, int& pIndex);

	/** Get character node version of a given character node ID.
	  * \param pCharacterNodeId     Character node ID to get version.
	  * \param pVersion             if the node ID is found, the method returns the version through this parameter
	  * \remarks                    Only works for a character node ID is part of a group.
	  * \return                     \c true if successful, \c false otherwise.
	  */
	static bool GetCharacterGroupVersionByElement(ECharacterNodeId pCharacterNodeId, int& pVersion);

	/** Get character node name associated with a given character node ID.
	  * \param pCharacterNodeId     Character node ID to get name.
	  * \param pName                if the node ID is found, the method returns the node name through this parameter
	  *                             Since the Pointer points to internal data, it is not necessary to allocate a string buffer 
	  *                             before calling this function.
	  * \return                     \c true if a name exists for the given node ID.
	  */
	static bool GetCharacterNodeNameFromNodeId(ECharacterNodeId pCharacterNodeId, char*& pName);

	/** Get the character node ID associated with a given character node name.
      * \param pName                Character node name to get node ID.
      * \param pCharacterNodeId     if the node name is found, this method returns the node ID through this parameter
      * \return                     \c true if a node ID exists for the given node name.
	  */
	static bool GetCharacterNodeIdFromNodeName(char* pName, ECharacterNodeId& pCharacterNodeId);

	typedef enum 
	{
		eINTERNAL_ERROR,
		eERROR_COUNT
	} EError; 


///////////////////////////////////////////////////////////////////////////////
//
//  WARNING!
//
//	Anything beyond these lines may not be documented accurately and is 
// 	subject to change without notice.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef DOXYGEN_SHOULD_SKIP_THIS	

	virtual KObject* GetFbxObject_internal();
	virtual KObject const* GetFbxObject_internal() const;

	// Clone
	virtual KFbxObject* Clone(KFbxObject::ECloneType pCloneType) const;

protected:
	KFbxCharacter& operator=(KFbxCharacter const& pCharacter); 

public:
	virtual KFbxObject::ENameSpace GetNameSpace() const; 

	static char const* GetNamePrefix() { return CONSTRAINT_PREFIX; }
	
	KFbxCharacter(KFbxSdkManager& pManager, char const* pName);
	~KFbxCharacter();

	virtual void Destruct(bool pRecursive, bool pDependents);

	virtual EConstraintType GetConstraintType();
	virtual KString			GetTypeName() const;

	virtual KStringList	GetTypeFlags() const;

	// KFbxTakeNodeContainer virtual functions
	virtual void AddChannels(KFbxTakeNode *pTakeNode);

	bool FbxPlugNotify(KFbxPlugEvent const &pEvent);

	KFbxCharacter_Internal* mPH;
	
	bool GetFilterSetNameFromPropertyNameOrModeAndIndex( char const*  &pFilterSetName, char const* pPropertyName, int pIndex )const ;
	bool GetPropertyNameAndModeNameAndIndexFromFilterSetName(char const* &pPropertyName,char const* &pPropertyModeName,int &pIndex, char const* pFilterSetName) const;	
	
	KError mError;

	KString mName;	
	KFbxCharacterLink mCharacterLink[eCharacterLastNodeId];
	KFbxControlSet* mControlSet;
	KFbxFilterSet* mFilterSet;

	friend class KFbxScene;
	friend class KFbxNode;
	friend class KFbxReaderFbx;
	friend class KFbxCharacter_Internal;

#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS

protected:
	
};

typedef KFbxCharacter* HKFbxCharacter;

#include <fbxfilesdk_nsend.h>

#endif // #ifndef _FBXSDK_CHARACTER_H_


