/**************************************************************************************

 Copyright © 2001 - 2006 Autodesk, Inc. and/or its licensors.
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


#include "DisplayCommon.h"
#include "DisplayCamera.h"

#include <fbxfilesdk_nsuse.h>

void DisplayGlobalLightSettings(KFbxGlobalLightSettings* pGlobalLightSettings)
{
	int i, lCount;

    DisplayColor("Ambient Color: ", pGlobalLightSettings->GetAmbientColor());
	DisplayString("Fog Options");
	DisplayBool("    Fog Enable: ", pGlobalLightSettings->GetFogEnable());

	char* lFogModes[] = { "Linear", "Exponential", "Squareroot Exponential" }; 
    
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
  
	char* lDefaultViewingModes[] = { "Standard", "X-Ray", "Models Only" };
	
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

	char* lTimeModes[] = { "Default Mode", "Cinema", "PAL", "Frames 30", 
			                  "NTSC Drop Frame", "Frames 50", "Frames 60",
							  "Frames 100", "Frames 120", "NTSC Full Frame", 
							  "Frames 30 Drop", "Frames 1000" }; 

    DisplayString("Time Mode: ", lTimeModes[pGlobalTimeSettings->GetTimeMode()]);

	char* lTimeProtocols[] = { "SMPTE", "Frame", "Default Protocol" };

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


