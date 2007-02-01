/*!  \file kfbxtakenodecontainer.h
 */

#ifndef _FBXSDK_TAKE_NODE_CONTAINER_H_
#define _FBXSDK_TAKE_NODE_CONTAINER_H_

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

#include <kfbxplugins/kfbxobject.h>

#include <klib/karrayul.h>

#ifndef MB_FBXSDK
#include <kbaselib_nsuse.h>
#endif
#include <kbaselib_forward.h>

#include <kfcurve/kfcurve_forward.h>
#ifndef MB_FBXSDK
#include <kfcurve/kfcurve_nsuse.h>
#endif

#include <fbxfilesdk_nsbegin.h>

class KFbxSdkManager;
class KFbxTakeNode;
class KFbxDefaultTakeCallback;
class KFbxTakeNodeContainer_internal;

/**	\brief This class is a container for take nodes which contain animation data.
  * \nosubgrouping
  * A take node contains the animation keys of the container for a given take.
  *  
  * A default take node is always created. This take is used to store default animation 
  * values. This take is at index 0, and should not contain animation keys or 
  * be removed in any situation.
  * 
  * Be careful when processing take animation. If the current 
  * take does not exist for a container, KFbxTakeNodeContainer::GetCurrentTakeNode() and 
  * KFbxTakeNodeContainer::GetDefaultTakeNode() will return the same KFbxTakeNode object. 
  *	If both the default and the current take node are processed without
  *	appropriate testing, the same data will be processed twice.
  */
class KFBX_DLL KFbxTakeNodeContainer : public KFbxObject
{
	KFBXOBJECT_DECLARE(KFbxTakeNodeContainer);
public:

	/**
	  * \name Take Node Management
	  */
	//@{

	/**	Get default take node.
	  *	\return Pointer to default take node.
  	  *	\remarks See remarks in group header.
	  */
	KFbxTakeNode* GetDefaultTakeNode();

	/**	Get default take node.
	  *	\return Pointer to default take node.
  	  *	\remarks See remarks in group header.
	  */
	KFbxTakeNode const* GetDefaultTakeNode() const;

	/**	Create a take node.
	  *	\param pName New take node name.
	  *	\return Pointer to the created take node or,  if a take node with the same name
	  * already exists in the node, \c NULL.
	  * In this case, KFbxNode::GetLastErrorID() returns eTAKE_NODE_ERROR and
	  * KFbxNode::GetLastErrorString() returns "Take node already exists".
	  */
	KFbxTakeNode* CreateTakeNode(char* pName);

	/**	Remove take node by index.
	  *	\param pIndex Index of take node to remove.
	  *	\return \c true on success, \c false otherwise.
	  *	In the last case, KFbxNode::GetLastErrorID() can return one of the following:
	  *		- eCANNOT_REMOVE_DEFAULT_TAKE_NODE: The default take node can't be removed.
	  *		- eINDEX_OUT_OF_RANGE: The index is out of range.
	  */
	bool RemoveTakeNode(int pIndex);

	/**	Remove take node by name.
	  *	\param pName Take node name.
	  *	\return \c true on success, \c false otherwise.
	  *	In the last case, KFbxNode::GetLastErrorID() can return one of the following:
	  *		- eCANNOT_REMOVE_DEFAULT_TAKE_NODE: The default take node can't be removed.
	  *		- eUNKNOWN_TAKE_NODE_NAME: No take node with this name exists in the current node.
	  */
	bool RemoveTakeNode(char* pName);

	/**	Get take node count.
	  *	\return The number of take nodes including the default take node.
	  */
	int GetTakeNodeCount() const;

    /** Get take node by index.
      * \param pIndex Take node index.
      * \return Pointer to the indexed take node or \c NULL if the index is out of range.
	  * In the last case, KFbxNode::GetLastErrorID() returns eINDEX_OUT_OF_RANGE.
	  *	\remarks Index 0 returns the default take.
      */
	KFbxTakeNode* GetTakeNode(int pIndex) const;

	/**	Get name of take node at a given index.
	  *	\param pIndex Take node index.
	  *	\return Name of take node  or \c NULL if the index is out of range.
	  * In the last case, KFbxNode::GetLastErrorID() returns eINDEX_OUT_OF_RANGE.
	  *	\remarks Index 0 returns the name of the default take.
	  */
	char* GetTakeNodeName(int pIndex);

	/**	Set current take node by index.
	  *	\param pIndex Index of the current take node.
	  *	\return \c true on success, \c false otherwise.
	  *	In the last case, KFbxNode::GetLastErrorID() returns eINDEX_OUT_OF_RANGE
	  * and the current take node is set to index 0, the default take node.
	  */
	virtual bool SetCurrentTakeNode(int pIndex);

	/**	Set current take node by name.
	  *	\param pName Name of the current take node.
	  *	\return \c true on success, \c false otherwise.
	  *	In the last case, KFbxNode::GetLastErrorID() returns eUNKNOWN_TAKE_NODE_NAME
	  * and the current take node is set to index 0, the default take node.
	  */
	virtual bool SetCurrentTakeNode(char* pName);

	/**	GetCurrentTakeNode.
	  *	\return Pointer to the current take node.
      *	\remarks If the current take node is set to index 0, it refers to the default take node.
	  * Is this case, KFbxNode::GetCurrentTakeNode() and KFbxNode::GetDefaultTakeNode return
	  * the same pointer.
	  */
	KFbxTakeNode* GetCurrentTakeNode();	

	/**	Get the name of the current take node.
	  *	\return Name of the current take node.
	  */
	char* GetCurrentTakeNodeName();


	/**	Get index of the current take node.
	  *	\return Index of the current take node.
	  */
	int GetCurrentTakeNodeIndex();

	/** Find the start and end time of the current take.
	  *	\param pStart Reference to store start time.
	  *	\param pStop Reference to store end time.
	  *	\return \c true on success, \c false otherwise.
	  */
	virtual bool GetAnimationInterval(KTime& pStart, KTime& pStop);

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

	void PropertyDefaultValueChanged(KFbxProperty* pProperty);

	bool IsChannelAnimated(char* pGroup, char* pSubGroup, char* pName);
	bool IsChannelAnimated(char* pGroup, char* pSubGroup, KDataType* pDataType);

	void RegisterDefaultTakeCallback(KProperty* pProperty, int pComponentIndex, HKFCurve pFCurve);
protected:
		
	/** Create an KFCurveNode for a property.
	  *	\param pProperty the property that needs an FCURVE to be added.
	  * \remark only create if the property is eANIMATABLE
	  */
	void CreateKFCurveNodeProperty(KFbxProperty* pProperty, const char* pTakeName=NULL);
	friend class KFbxProperty;
protected:
	virtual ~KFbxTakeNodeContainer();
	KFbxTakeNodeContainer(KFbxSdkManager& pManager, char const* pName, KError* pError=0);

	virtual void Construct();
	virtual void Destruct(bool pRecursive, bool pDependents);

	virtual KFbxTakeNodeContainer* GetTakeNodeContainer();
	KFbxTakeNodeContainer& operator=(KFbxTakeNodeContainer const& pTakeNodeContainer);

public:
	virtual KObject*		GetFbxObject_internal();
	virtual KObject const*  GetFbxObject_internal() const;

	void Init();
	void Reset();
	virtual void AddChannels(KFbxTakeNode *pTakeNode) {};

	virtual void              PropertyAdded(KFbxProperty* pProperty);
	virtual void              PropertyRemoved(KFbxProperty* pProperty);		        

	void UpdateFCurveFromProperty(KFbxProperty &pProperty,KFbxTakeNode *pTakeNode=NULL);
//	void UpdateFCurveFromProperty(KFbxProperty &pProperty,char *pTakeName=NULL);
//	void UpdatePropertyFromFCurve(KFbxProperty &pProperty,char *pTakeName=NULL);
//	void UpdatePropertyFromFCurve(KFbxProperty &pProperty,KFbxTakeNode *pTakeNode);


	void CreateChannelsForProperty(KProperty* pProperty, KFbxTakeNode *pTakeNode);
	void UnregisterDefaultTakeCallback(KFbxDefaultTakeCallback*& pTC);
	void UnregisterAllDefaultTakeCallback();
	friend void DefaultTakeValueChangedCallback(KFCurve *pFCurve, KFCurveEvent *pFCurveEvent, void* pObject);
	
private:
	KFbxTakeNodeContainer_internal*			 mTNInternal;
	KArrayTemplate<KFbxTakeNode*>            mTakeNodeArray;
	KArrayTemplate<KFbxDefaultTakeCallback*> mCallbackInfo;
	int mCurrentTakeNodeIndex;
	mutable KError* mError;

#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS

};

typedef KFbxTakeNodeContainer* HKFbxTakeNodeContainer;

#include <fbxfilesdk_nsend.h>

#endif // #ifndef _FBXSDK_TAKE_NODE_CONTAINER_H_


