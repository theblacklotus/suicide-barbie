/*!  \file kfbxpivotreplacement.h
 */

#if !defined(AFX_KFBXPIVOTREPLACEMENT_H__8A31DB7C_8A21_48C4_AFE8_5F7AE39D3D45__INCLUDED_)
#define AFX_KFBXPIVOTREPLACEMENT_H__8A31DB7C_8A21_48C4_AFE8_5F7AE39D3D45__INCLUDED_

/**************************************************************************************

 Copyright © 2003 - 2006 Autodesk, Inc. and/or its licensors.
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


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <kaydara.h>
#include <kfbxplugins/kfbxnode.h>

// Not part of the public FBX SDK
#ifndef K_FBXSDK

#include <fbxfilesdk_nsbegin.h>


/**
 * Function that replaces the pre and post rotation of a node by two null nodes
 * (or skeleton) that will have the pre and post rotation as their rotation channel
 * the pre rotation node will also have all the translation information of the original
 * node. The hierarchy is then changed so that the pre rotation node is parent of
 * the original wich is in turn parent of the post rotation node.
 * \param pNode typically the root node since the function is recursive
 * \param pSdkManager
 * \return returns true if any change has been made
 */
bool ReplacePivotExport(KFbxNode* pNode,KFbxNode::EPivotSet pPivotMode,	KFbxSdkManager& pSdkManager	);

/**
 * Function that finds the pre and post node that were added with ReplacePivotExport and merge
 * them back into a simgle node, setting the pre and post rotation accordingly
 * \param pNode typically the root node since the function is recursive
 * \param pSdkManager*
 */
void ReplacePivotImport(KFbxNode* pNode,KFbxNode::EPivotSet pPivotMode,	KFbxSdkManager& pSdkManager	);

#include <fbxfilesdk_nsend.h>


#endif // #ifndef K_FBXSDK
#endif // !defined(AFX_KFBXPIVOTREPLACEMENT_H__8A31DB7C_8A21_48C4_AFE8_5F7AE39D3D45__INCLUDED_)


