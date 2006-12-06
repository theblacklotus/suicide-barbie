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

#include <fbxsdk.h>

#include <stdio.h>

using namespace FBXSDK_NAMESPACE;

// Local functions prototype.
void DisplayHierarchy(KFbxNode* pNode, int pDepth);

void DisplayHierarchy(KFbxScene* pScene)
{
	int i;
	KFbxNode* lRootNode = pScene->GetRootNode();

	for( i = 0; i < lRootNode->GetChildCount(); i++)
	{
		DisplayHierarchy(lRootNode->GetChild(i), 0);
	}
}

void DisplayHierarchy(KFbxNode* pNode, int pDepth)
{
	KString lString;
	int i;

	for(i = 0; i < pDepth; i++)
	{
		lString += "     ";
	}

	lString += pNode->GetName();
	lString += "\n";

	printf(lString.Buffer());

	for(i = 0; i < pNode->GetChildCount(); i++)
	{
		DisplayHierarchy(pNode->GetChild(i), pDepth + 1);
	}
}


