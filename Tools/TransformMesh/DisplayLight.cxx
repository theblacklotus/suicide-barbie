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

void DisplayDefaultAnimationValues(KFbxLight* pLight);


void DisplayLight(KFbxNode* pNode)
{
    KFbxLight* lLight = (KFbxLight*) pNode->GetNodeAttribute();

    DisplayString("Light Name: ", pNode->GetName());

	kCharPtr lLightTypes[] = { "Point", "Directional", "Spot" };

	DisplayString("    Type: ", lLightTypes[lLight->GetLightType()]);
	DisplayBool("    Cast Light: ", lLight->GetCastLight());

	if (!KString(lLight->GetFileName()).IsEmpty())
	{
		DisplayString("    Gobo");

		DisplayString("        File Name: \"", lLight->GetFileName(), "\"");
		DisplayBool("        Ground Projection: ", lLight->GetGroundProjection());
		DisplayBool("        Volumetric Projection: ", lLight->GetVolumetricProjection());
		DisplayBool("        Front Volumetric Projection: ", lLight->GetFrontVolumetricProjection());
	}

	DisplayDefaultAnimationValues(lLight);
}


void DisplayDefaultAnimationValues(KFbxLight* pLight)
{
	KFbxColor lColor;

	DisplayString("    Default Animation Values");

	DisplayColor("        Default Color: ", pLight->GetDefaultColor(lColor));
	DisplayDouble("        Default Intensity: ", pLight->GetDefaultIntensity());
	DisplayDouble("        Default Cone Angle: ", pLight->GetDefaultConeAngle());
	DisplayDouble("        Default Fog: ", pLight->GetDefaultFog());
}


