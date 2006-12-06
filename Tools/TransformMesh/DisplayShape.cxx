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

using namespace FBXSDK_NAMESPACE;

void DisplayShape(KFbxGeometry* pGeometry)
{
    kInt i, lShapeCount;
	KFbxShape* lShape;

	lShapeCount = pGeometry->GetShapeCount();

	for (i = 0; i < lShapeCount; i++)
	{
		DisplayString("    Shape ", pGeometry->GetShapeName(i));
		DisplayDouble("        Default Animation Value: ", pGeometry->GetDefaultShape(i));

	    lShape = pGeometry->GetShape(i);    
	
		kInt j, lControlPointsCount = lShape->GetControlPointsCount();
		KFbxVector4* lControlPoints = lShape->GetControlPoints();
	    KFbxVector4* lNormals = lShape->GetNormals();

		for(j = 0; j < lControlPointsCount; j++)
		{
			DisplayInt("        Control Point ", j);
			Display3DVector("            Coordinates: ", lControlPoints[j]);

			if (lNormals)
			{
				Display3DVector("            Normal Vector: ", lNormals[j]);
			}
		}

		DisplayString("");
	}
}


