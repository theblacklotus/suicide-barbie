#ifndef _COMMON_H
#define _COMMON_H

/**************************************************************************************

 Copyright © 2001 - 2005 Systèmes Alias Québec Inc. and/or its licensors.  
 All rights reserved.

 The coded instructions, statements, computer programs, and/or related material 
 (collectively the "Data") in these files is provided by Alias (as defined in the Alias 
 Software License Agreement that accompanies the Alias software (the "Agreement")) 
 for the exclusive use of the Customer (as defined in the Agreement). Such Customer has 
 the right to use, modify, and incorporate the Data into other products and to 
 distribute such products for use by end-users.

 THE DATA IS PROVIDED “AS IS” AND WITHOUT WARRANTY. ALL WARRANTIES ARE EXPRESSLY
 EXCLUDED AND DISCLAIMED. ALIAS MAKES NO WARRANTY OF ANY KIND WITH RESPECT TO THE DATA,
 EXPRESS, IMPLIED OR ARISING BY CUSTOM OR TRADE USAGE, AND DISCLAIMS ANY IMPLIED
 WARRANTIES OF TITLE, NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR 
 PURPOSE. IN NO EVENT SHALL ALIAS, ITS AFFILIATES, PARENT COMPANIES, LICENSORS OR 
 SUPPLIERS (“ALIAS GROUP”) BE LIABLE FOR ANY DAMAGES OR EXPENSES OF ANY KIND INCLUDING
 WITHOUT LIMITATION PUNITIVE OR MULTIPLE DAMAGES OR OTHER SPECIAL, DIRECT, INDIRECT, 
 EXEMPLARY, INCIDENTAL, OR CONSEQUENTIAL DAMAGES OF ANY KIND, WHETHER OR NOT ALIAS HAS
 BEEN ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWSOEVER CAUSED AND ON ANY THEORY OF
 LIABILITY. 

**************************************************************************************/

#include <kfbxio/kfbximporter.h>
#include <kfbxplugins/kfbxsdkmanager.h>
#include <kfbxplugins/kfbxscene.h>

#include <stdio.h>

using namespace FBXSDK_NAMESPACE;

void InitializeSdkObjects(KFbxSdkManager*& pSdkManager, KFbxScene*& pScene);
void DestroySdkObjects(KFbxSdkManager* pSdkManager);
bool SaveScene(KFbxSdkManager* pSdkManager, KFbxScene* pScene, const char* pFilename, bool pSaveAsBinary=true, bool pEmbedMedia=false, bool pSaveAsVersion5=false);
bool LoadScene(KFbxSdkManager* pSdkManager, KFbxScene* pScene, const char* pFilename);
KFbxImporter::EFileFormat GetFileFormat(const char* pFilename);

#endif // #ifndef _COMMON_H


