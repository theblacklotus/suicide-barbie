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

#include <stdio.h>

using namespace FBXSDK_NAMESPACE;

void DisplayPivotsAndLimits(KFbxNode* pNode)
{
	KFbxVector4 lTmpVector;

	//
	// Pivots
	//
	printf("    Pivot Information\n");

	KFbxNode::EPivotState lPivotState;
	pNode->GetPivotState(KFbxNode::eSOURCE_SET, lPivotState);
	printf("        Pivot State: %s\n", lPivotState == KFbxNode::ePIVOT_STATE_ACTIVE ? "Active" : "Reference");

	lTmpVector = pNode->GetPreRotation(KFbxNode::eSOURCE_SET);
	printf("        Pre-Rotation: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

	lTmpVector = pNode->GetPostRotation(KFbxNode::eSOURCE_SET);
	printf("        Post-Rotation: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

	lTmpVector = pNode->GetRotationPivot(KFbxNode::eSOURCE_SET);
	printf("        Rotation Pivot: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

	lTmpVector = pNode->GetRotationOffset(KFbxNode::eSOURCE_SET);
	printf("        Rotation Offset: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

	lTmpVector = pNode->GetScalingPivot(KFbxNode::eSOURCE_SET);
	printf("        Scaling Pivot: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

	lTmpVector = pNode->GetScalingOffset(KFbxNode::eSOURCE_SET);
	printf("        Scaling Offset: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);
	
	//
	// Limits
	//
	KFbxNodeLimits lLimits = pNode->GetLimits();
	bool           lIsActive, lMinXActive, lMinYActive, lMinZActive;
	bool           lMaxXActive, lMaxYActive, lMaxZActive;
	KFbxVector4    lMinValues, lMaxValues;

	printf("    Limits Information\n");

	lIsActive = lLimits.GetTranslationLimitActive();
	lLimits.mTranslationLimits.GetLimitMinActive(lMinXActive, lMinYActive, lMinZActive);
	lLimits.mTranslationLimits.GetLimitMaxActive(lMaxXActive, lMaxYActive, lMaxZActive);
	lMinValues = lLimits.mTranslationLimits.GetLimitMin();
	lMaxValues = lLimits.mTranslationLimits.GetLimitMax();

	printf("        Translation limits: %s\n", lIsActive ? "Active" : "Inactive");
	printf("            X\n");
	printf("                Min Limit: %s\n", lMinXActive ? "Active" : "Inactive");
	printf("                Min Limit Value: %f\n", lMinValues[0]);
	printf("                Max Limit: %s\n", lMaxXActive ? "Active" : "Inactive");
	printf("                Max Limit Value: %f\n", lMaxValues[0]);
	printf("            Y\n");
	printf("                Min Limit: %s\n", lMinYActive ? "Active" : "Inactive");
	printf("                Min Limit Value: %f\n", lMinValues[1]);
	printf("                Max Limit: %s\n", lMaxYActive ? "Active" : "Inactive");
	printf("                Max Limit Value: %f\n", lMaxValues[1]);
	printf("            Z\n");
	printf("                Min Limit: %s\n", lMinZActive ? "Active" : "Inactive");
	printf("                Min Limit Value: %f\n", lMinValues[2]);
	printf("                Max Limit: %s\n", lMaxZActive ? "Active" : "Inactive");
	printf("                Max Limit Value: %f\n", lMaxValues[2]);

	lIsActive = lLimits.GetRotationLimitActive();
	lLimits.mRotationLimits.GetLimitMinActive(lMinXActive, lMinYActive, lMinZActive);
	lLimits.mRotationLimits.GetLimitMaxActive(lMaxXActive, lMaxYActive, lMaxZActive);
	lMinValues = lLimits.mRotationLimits.GetLimitMin();
	lMaxValues = lLimits.mRotationLimits.GetLimitMax();

	printf("        Rotation limits: %s\n", lIsActive ? "Active" : "Inactive");
	printf("            X\n");
	printf("                Min Limit: %s\n", lMinXActive ? "Active" : "Inactive");
	printf("                Min Limit Value: %f\n", lMinValues[0]);
	printf("                Max Limit: %s\n", lMaxXActive ? "Active" : "Inactive");
	printf("                Max Limit Value: %f\n", lMaxValues[0]);
	printf("            Y\n");
	printf("                Min Limit: %s\n", lMinYActive ? "Active" : "Inactive");
	printf("                Min Limit Value: %f\n", lMinValues[1]);
	printf("                Max Limit: %s\n", lMaxYActive ? "Active" : "Inactive");
	printf("                Max Limit Value: %f\n", lMaxValues[1]);
	printf("            Z\n");
	printf("                Min Limit: %s\n", lMinZActive ? "Active" : "Inactive");
	printf("                Min Limit Value: %f\n", lMinValues[2]);
	printf("                Max Limit: %s\n", lMaxZActive ? "Active" : "Inactive");
	printf("                Max Limit Value: %f\n", lMaxValues[2]);

	lIsActive = lLimits.GetScalingLimitActive();
	lLimits.mScalingLimits.GetLimitMinActive(lMinXActive, lMinYActive, lMinZActive);
	lLimits.mScalingLimits.GetLimitMaxActive(lMaxXActive, lMaxYActive, lMaxZActive);
	lMinValues = lLimits.mScalingLimits.GetLimitMin();
	lMaxValues = lLimits.mScalingLimits.GetLimitMax();

	printf("        Scaling limits: %s\n", lIsActive ? "Active" : "Inactive");
	printf("            X\n");
	printf("                Min Limit: %s\n", lMinXActive ? "Active" : "Inactive");
	printf("                Min Limit Value: %f\n", lMinValues[0]);
	printf("                Max Limit: %s\n", lMaxXActive ? "Active" : "Inactive");
	printf("                Max Limit Value: %f\n", lMaxValues[0]);
	printf("            Y\n");
	printf("                Min Limit: %s\n", lMinYActive ? "Active" : "Inactive");
	printf("                Min Limit Value: %f\n", lMinValues[1]);
	printf("                Max Limit: %s\n", lMaxYActive ? "Active" : "Inactive");
	printf("                Max Limit Value: %f\n", lMaxValues[1]);
	printf("            Z\n");
	printf("                Min Limit: %s\n", lMinZActive ? "Active" : "Inactive");
	printf("                Min Limit Value: %f\n", lMinValues[2]);
	printf("                Max Limit: %s\n", lMaxZActive ? "Active" : "Inactive");
	printf("                Max Limit Value: %f\n", lMaxValues[2]);
}

