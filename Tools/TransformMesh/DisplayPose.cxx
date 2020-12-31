/**************************************************************************************

 Copyright © 2003 - 2005 Systèmes Alias Québec Inc. and/or its licensors.  
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

#include "DisplayCommon.h"

#include <stdio.h>

using namespace FBXSDK_NAMESPACE;

void DisplayPose(KFbxScene* pScene)
{
    kInt      i,j,k,lPoseCount;
	KFbxPose* lPose;
	KString	  lName;
    
	lPoseCount = pScene->GetPoseCount();

	for (i = 0; i < lPoseCount; i++)
	{
		lPose = pScene->GetPose(i);

		lName = lPose->GetName();
		DisplayString("Pose Name: ", lName.Buffer());

		DisplayBool("    Is a bind pose: ", lPose->IsBindPose());

		DisplayInt("    Number of items in the pose: ", lPose->GetCount());

		DisplayString("","");

		for (j=0; j<lPose->GetCount(); j++)
		{
			lName = lPose->GetNodeName(j);
			DisplayString("    Item name: ", lName.Buffer());

			if (!lPose->IsBindPose())
			{
				// Rest pose can have local matrix
				DisplayBool("    Is local space matrix: ", lPose->IsLocalMatrix(j));
			}

			DisplayString("    Matrix value: ","");

			KString lMatrixValue;

			for (k=0; k<4; k++)
			{
				KFbxMatrix  lMatrix = lPose->GetMatrix(j);
				KFbxVector4 lRow = lMatrix.GetRow(k);
				char        lRowValue[1024];

				sprintf(lRowValue, "%9.4f %9.4f %9.4f %9.4f\n", lRow[0], lRow[1], lRow[2], lRow[3]);
				lMatrixValue += KString("        ") + KString(lRowValue);
			}

			DisplayString("", lMatrixValue.Buffer());
		}

	}
}


