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

#include <stdio.h>

using namespace FBXSDK_NAMESPACE;

void DisplayLink(KFbxGeometry* pGeometry)
{
    kInt i, lLinkCount;
	KFbxLink* lLink;
    
	lLinkCount = pGeometry->GetLinkCount();

	for (i = 0; i < lLinkCount; i++)
	{
		DisplayInt("    Link ", i);

	    lLink = pGeometry->GetLink(i);    
		
		kCharPtr lLinkModes[] = { "Normalize", "Additive", "Total1" };

		DisplayString("    Mode: ", lLinkModes[lLink->GetLinkMode()]);

		if(lLink->GetLink() != NULL)
		{
			DisplayString("        Name: ", lLink->GetLink()->GetName());
		}

		KString lString1 = "        Link Indices: ";
		KString lString2 = "        Weight Values: ";
	
		kInt j, lIndexCount = lLink->GetControlPointIndicesCount();
		kInt* lIndices = lLink->GetControlPointIndices();
		kDouble* lWeights = lLink->GetControlPointWeights();
	
		for(j = 0; j < lIndexCount; j++)
		{
	        lString1 += lIndices[j];	
			lString2 += (kFloat) lWeights[j];

			if (j < lIndexCount - 1)
			{
				lString1 += ", ";
				lString2 += ", ";
			}
		}
	
		lString1 += "\n";
		lString2 += "\n";
	
		printf(lString1);
		printf(lString2);
		
		DisplayString("");

		KFbxVector4 lVector;

		Display3DVector("        Transform Translation: ", lLink->GetTransformT(lVector));
		Display3DVector("        Transform Rotation: ", lLink->GetTransformR(lVector));
		Display3DVector("        Transform Scaling: ", lLink->GetTransformS(lVector));
		Display3DVector("        Transform Link Translation: ", lLink->GetTransformLinkT(lVector));
		Display3DVector("        Transform Link Rotation: ", lLink->GetTransformLinkR(lVector));
		Display3DVector("        Transform Link Scaling: ", lLink->GetTransformLinkS(lVector));

		if (lLink->GetAssociateModel() != NULL)
		{
			DisplayString("        Associate Model: ", lLink->GetAssociateModel()->GetName());
			Display3DVector("        Associate Model Translation: ", lLink->GetTransformAssociateModelT(lVector));
			Display3DVector("        Associate Model Rotation: ", lLink->GetTransformAssociateModelR(lVector));
			Display3DVector("        Associate Model Scaling: ", lLink->GetTransformAssociateModelS(lVector));
		}

		DisplayString("");
	}
}


