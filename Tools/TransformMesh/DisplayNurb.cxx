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

#include <stdio.h>

using namespace FBXSDK_NAMESPACE;

void DisplayNurb(KFbxNode* pNode)
{
    KFbxNurb* lNurb = (KFbxNurb*) pNode->GetNodeAttribute ();
	kInt i;

	DisplayString("Nurb Name: ", pNode->GetName());

	kCharPtr lSurfaceModes[] = { "Raw", "Low No Normals", "Low", "High No Normals", "High" };

	DisplayString("    Surface Mode: ", lSurfaceModes[lNurb->GetSurfaceMode()]);

	kInt lControlPointsCount = lNurb->GetControlPointsCount();
	KFbxVector4* lControlPoints = lNurb->GetControlPoints();

	for (i = 0; i < lControlPointsCount; i++)
	{
		DisplayInt("    Control Point ", i);
		Display3DVector("        Coordinates: ", lControlPoints[i]);
		DisplayDouble("        Weight: ", lControlPoints[i][3]);
	}	

	kCharPtr lNurbTypes[] = { "Periodic", "Closed", "Open" };

	DisplayString("    Nurb U Type: ", lNurbTypes[lNurb->GetNurbUType()]);
	DisplayInt("    U Count: ", lNurb->GetUCount());
	DisplayString("    Nurb V Type: ", lNurbTypes[lNurb->GetNurbVType()]);
	DisplayInt("    V Count: ", lNurb->GetVCount());
	DisplayInt("    U Order: ", lNurb->GetUOrder());
	DisplayInt("    V Order: ", lNurb->GetVOrder());
	DisplayInt("    U Step: ", lNurb->GetUStep());
	DisplayInt("    V Step: ", lNurb->GetVStep());

	KString lString;
	kInt lUKnotCount = lNurb->GetUKnotCount();
	kInt lVKnotCount = lNurb->GetVKnotCount();
	kInt lUMultiplicityCount = lNurb->GetUCount();
	kInt lVMultiplicityCount = lNurb->GetVCount();
	kDouble* lUKnotVector = lNurb->GetUKnotVector();
	kDouble* lVKnotVector = lNurb->GetVKnotVector();
	kInt* lUMultiplicityVector = lNurb->GetUMultiplicityVector();
	kInt* lVMultiplicityVector = lNurb->GetVMultiplicityVector();
	
	lString = "    U Knot Vector: ";

	for (i = 0; i < lUKnotCount; i++)
	{
		lString += (kFloat) lUKnotVector[i];

		if (i < lUKnotCount - 1)
		{
			lString += ", ";
		}
	}

	lString += "\n";
	printf(lString);

	lString = "    V Knot Vector: ";

	for (i = 0; i < lVKnotCount; i++)
	{
		lString += (kFloat) lVKnotVector[i];

		if (i < lVKnotCount - 1)
		{
			lString += ", ";
		}
	}

	lString += "\n";
	printf(lString);

	lString = "    U Multiplicity Vector: ";

	for (i = 0; i < lUMultiplicityCount; i++)
	{
		lString += lUMultiplicityVector[i];

		if (i < lUMultiplicityCount - 1)
		{
			lString += ", ";
		}
	}

	lString += "\n";
	printf(lString);

	lString = "    V Multiplicity Vector: ";

	for (i = 0; i < lVMultiplicityCount; i++)
	{
		lString += lVMultiplicityVector[i];

		if (i < lVMultiplicityCount - 1)
		{
			lString += ", ";
		}
	}

	lString += "\n";
	printf(lString);

	DisplayString("");

    DisplayTexture(lNurb);
    DisplayMaterial(lNurb);
	DisplayLink(lNurb);
	DisplayShape(lNurb);
}






