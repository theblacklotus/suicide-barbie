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

#include <fbxsdk.h>

#include <stdio.h>

#include <fbxfilesdk_nsuse.h>

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

