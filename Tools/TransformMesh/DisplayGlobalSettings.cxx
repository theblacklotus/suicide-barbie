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
#include "DisplayCamera.h"

using namespace FBXSDK_NAMESPACE;

void DisplayGlobalLightSettings(KFbxGlobalLightSettings* pGlobalLightSettings)
{
	int i, lCount;

    DisplayColor("Ambient Color: ", pGlobalLightSettings->GetAmbientColor());
	DisplayString("Fog Options");
	DisplayBool("    Fog Enable: ", pGlobalLightSettings->GetFogEnable());

	kCharPtr lFogModes[] = { "Linear", "Exponential", "Squareroot Exponential" }; 
    
	DisplayString("    Fog Mode: ", lFogModes[pGlobalLightSettings->GetFogMode()]);

	if (pGlobalLightSettings->GetFogMode() == KFbxGlobalLightSettings::eLINEAR)
	{
		DisplayDouble("    Fog Start: ", pGlobalLightSettings->GetFogStart());
		DisplayDouble("    Fog End: ", pGlobalLightSettings->GetFogEnd());
	}
	else
	{
		DisplayDouble("    Fog Density: ", pGlobalLightSettings->GetFogDensity());
	}

	DisplayColor("    Fog Color: ", pGlobalLightSettings->GetFogColor());

    lCount = pGlobalLightSettings->GetShadowPlaneCount();

	if (lCount)
	{
		DisplayString("    Shadow Planes");
		DisplayBool("        Enable: ", pGlobalLightSettings->GetShadowEnable());
		DisplayDouble("        Intensity: ", pGlobalLightSettings->GetShadowIntensity());

		for (i = 0; i < lCount; i++)
		{
	        DisplayInt("        Shadow Plane ", i);
			DisplayBool("            Enable: ", pGlobalLightSettings->GetShadowPlane(i)->mEnable);
			Display3DVector("            Origin: ", pGlobalLightSettings->GetShadowPlane(i)->mOrigin);
			Display3DVector("            Normal: ", pGlobalLightSettings->GetShadowPlane(i)->mNormal);
        }
	}

	DisplayString("");
}


void DisplayGlobalCameraSettings(KFbxGlobalCameraSettings* pGlobalCameraSettings)
{
    DisplayString("Default Camera: ", pGlobalCameraSettings->GetDefaultCamera());
  
	kCharPtr lDefaultViewingModes[] = { "Standard", "X-Ray", "Models Only" };
	
	DisplayString("Default Viewing Mode: ", lDefaultViewingModes[pGlobalCameraSettings->GetDefaultViewingMode()]);

	DisplayCamera(&pGlobalCameraSettings->GetCameraProducerPerspective(), PRODUCER_PERSPECTIVE);
	DisplayCamera(&pGlobalCameraSettings->GetCameraProducerTop(), PRODUCER_TOP);
	DisplayCamera(&pGlobalCameraSettings->GetCameraProducerFront(), PRODUCER_FRONT);
	DisplayCamera(&pGlobalCameraSettings->GetCameraProducerRight(), PRODUCER_RIGHT);

	DisplayString("");
}


void DisplayGlobalTimeSettings(KFbxGlobalTimeSettings* pGlobalTimeSettings)
{
	char lTimeString[256];
	int i, lCount;

	kCharPtr lTimeModes[] = { "Default Mode", "Cinema", "PAL", "Frames 30", 
			                  "NTSC Drop Frame", "Frames 50", "Frames 60",
							  "Frames 100", "Frames 120", "NTSC Full Frame", 
							  "Frames 30 Drop", "Frames 1000" }; 

    DisplayString("Time Mode: ", lTimeModes[pGlobalTimeSettings->GetTimeMode()]);

	kCharPtr lTimeProtocols[] = { "SMPTE", "Frame", "Default Protocol" };

	DisplayString("Time Protocol: ", lTimeProtocols[pGlobalTimeSettings->GetTimeProtocol()]);
	DisplayBool("Snap On Frame: ", pGlobalTimeSettings->GetSnapOnFrame());

    lCount = pGlobalTimeSettings->GetTimeMarkerCount();

	if (lCount)
	{
		DisplayString("Time Markers");
		DisplayInt("    Current Time Marker: ", pGlobalTimeSettings->GetCurrentTimeMarker());

		for (i = 0; i < lCount; i++)
		{
	        DisplayInt("    Time Marker ", i);
			DisplayString("        Name: ", pGlobalTimeSettings->GetTimeMarker(i)->mName.Buffer());
			DisplayString("        Time: ", pGlobalTimeSettings->GetTimeMarker(i)->mTime.GetTimeString(lTimeString));
			DisplayBool("        Loop: ", pGlobalTimeSettings->GetTimeMarker(i)->mLoop);
        }
	}

	DisplayString("");
}


