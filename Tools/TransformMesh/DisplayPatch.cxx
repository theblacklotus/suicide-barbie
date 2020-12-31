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

#include "DisplayCommon.h"
#include "DisplayTexture.h"
#include "DisplayMaterial.h"
#include "DisplayLink.h"
#include "DisplayShape.h"

using namespace FBXSDK_NAMESPACE;

void DisplayPatch(KFbxNode* pNode)
{
    KFbxPatch* lPatch = (KFbxPatch*) pNode->GetNodeAttribute();

	DisplayString("Patch Name: ", pNode->GetName());

	kCharPtr lSurfaceModes[] = { "Raw", "Low No Normals", "Low", "High No Normals", "High" };

	DisplayString("    Surface Mode: ", lSurfaceModes[lPatch->GetSurfaceMode()]);

	kInt i, lControlPointsCount = lPatch->GetControlPointsCount();
	KFbxVector4* lControlPoints = lPatch->GetControlPoints();

	for (i = 0; i < lControlPointsCount; i++)
	{
		DisplayInt("    Control Point ", i);
		Display3DVector("        Coordinates: ", lControlPoints[i]);
		DisplayDouble("        Weight: ", lControlPoints[i][3]);
	}	

	kCharPtr lPatchTypes[] = { "Bezier", "Bezier Quadric", "Cardinal", "B-Spline", "Linear" };

	DisplayString("    Patch U Type: ", lPatchTypes[lPatch->GetPatchUType()]);
	DisplayInt("    U Count: ", lPatch->GetUCount());
	DisplayString("    Patch V Type: ", lPatchTypes[lPatch->GetPatchVType()]);
	DisplayInt("    V Count: ", lPatch->GetVCount());
	DisplayInt("    U Step: ", lPatch->GetUStep());
	DisplayInt("    V Step: ", lPatch->GetVStep());
	DisplayBool("    U Closed: ", lPatch->GetUClosed());
	DisplayBool("    V Closed: ", lPatch->GetVClosed());
	DisplayBool("    U Capped Top: ", lPatch->GetUCappedTop());
	DisplayBool("    U Capped Bottom: ", lPatch->GetUCappedBottom());
	DisplayBool("    V Capped Top: ", lPatch->GetVCappedTop());
	DisplayBool("    V Capped Bottom: ", lPatch->GetVCappedBottom());

	DisplayString("");

    DisplayTexture(lPatch);
    DisplayMaterial(lPatch);
	DisplayLink(lPatch);
	DisplayShape(lPatch);
}


