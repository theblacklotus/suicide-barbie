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

#include <fbxfilesdk_nsuse.h>

void DisplayCamera(KFbxCamera* pCamera, char* pName, KFbxNode* pTargetNode = NULL, KFbxNode* pTargetUpNode = NULL);
void DisplayDefaultAnimationValues(KFbxCamera* pCamera);
void DisplayRenderOptions(KFbxCamera* pCamera);
void DisplayCameraViewOptions(KFbxCamera* pCamera);
void DisplayBackgroundProperties(KFbxCamera* pCamera);
void DisplayApertureAndFilmControls(KFbxCamera* pCamera);
void DisplayViewingAreaControls(KFbxCamera* pCamera);
void DisplayCameraPositionAndOrientation(KFbxCamera* pCamera, KFbxNode* pTargetNode, KFbxNode* pUpTargetNode);


void DisplayCamera(KFbxNode* pNode)
{
	DisplayCamera((KFbxCamera*) pNode->GetNodeAttribute(), (char *) pNode->GetName(), pNode->GetTarget(), pNode->GetTargetUp());
}


void DisplayCamera(KFbxCamera* pCamera, char* pName, KFbxNode* pTargetNode, KFbxNode* pTargetUpNode)
{
	DisplayString("Camera Name: ", pName);

	DisplayCameraPositionAndOrientation(pCamera, pTargetNode, pTargetUpNode);

	char* lProjectionTypes[] = { "Perspective", "Orthogonal" };

	DisplayString("    Projection Type: ", lProjectionTypes[pCamera->GetProjectionType()]);

	DisplayViewingAreaControls(pCamera);

	// If camera projection type is set to KFbxCamera::eORTHOGONAL, the 
	// aperture and film controls are not relevant.
	if (pCamera->GetProjectionType() != KFbxCamera::eORTHOGONAL)
	{
		DisplayApertureAndFilmControls(pCamera);
	}

	DisplayBackgroundProperties(pCamera);
	DisplayCameraViewOptions(pCamera);
	DisplayRenderOptions(pCamera);
	DisplayDefaultAnimationValues(pCamera);
}


void DisplayCameraPositionAndOrientation(KFbxCamera* pCamera, KFbxNode* pTargetNode, KFbxNode* pTargetUpNode)
{
	DisplayString("    Camera Position and Orientation");
	Display3DVector("        Position: ", pCamera->GetPosition());

	if (pTargetNode)
	{
		DisplayString("        Camera Interest: ",(char *) pTargetNode->GetName());
	}
	else
	{
		Display3DVector("        Default Camera Interest Position: ", pCamera->GetDefaultCameraInterestPosition());
	}

	if (pTargetUpNode)
	{
		DisplayString("        Camera Up Target: ", (char *) pTargetUpNode->GetName());
	}
	else
	{
		Display3DVector("        Up Vector: ", pCamera->GetUpVector());
	}

	DisplayDouble("        Roll: ", pCamera->GetRoll());
}


void DisplayViewingAreaControls(KFbxCamera* pCamera)
{
	DisplayString("    Viewing Area Controls");

	char* lCameraFormat[] = { "Custom", "D1 NTSC", "NTSC", "PAL", "D1 PAL", 
		                      "HD", "640x480", "320x200", "320x240", "128x128",
                              "Full Screen"  };

	DisplayString("        Format: ", lCameraFormat[pCamera->GetFormat()]);

	char* lAspectRatioModes[] = { "Window Size", "Fixed Ratio", "Fixed Resolution", 
                                  "Fixed Width", "Fixed Height" };

	DisplayString("        Aspect Ratio Mode: ", lAspectRatioModes[pCamera->GetAspectRatioMode()]);

	// If the ratio mode is eWINDOW_SIZE, both width and height values aren't relevant.
	if (pCamera->GetAspectRatioMode() != KFbxCamera::eWINDOW_SIZE)
	{
		DisplayDouble("        Aspect Width: ", pCamera->GetAspectWidth());
		DisplayDouble("        Aspect Height: ", pCamera->GetAspectHeight());
	}

	DisplayDouble("        Pixel Ratio: ", pCamera->GetPixelRatio());	
	DisplayDouble("        Near Plane: ", pCamera->GetNearPlane());	
	DisplayDouble("        Far Plane: ", pCamera->GetFarPlane());	
	DisplayBool("        Mouse Lock: ", pCamera->GetMouseLock());
}


void DisplayApertureAndFilmControls(KFbxCamera* pCamera)
{
	DisplayString("    Aperture and Film Controls");
 
	char* lCameraApertureFormats[] = { "Custom", 
                                       "16mm Theatrical",
                                       "Super 16mm",
                                       "35mm Academy",
                                       "35mm TV Projection",
                                       "35mm Full Aperture",
                                       "35mm 1.85 Projection",
                                       "35mm Anamorphic",
                                       "70mm Projection",
                                       "VistaVision",
                                       "Dynavision",
                                       "Imax" };
							
	DisplayString("        Aperture Format: ", lCameraApertureFormats[pCamera->GetApertureFormat()]);

    char* lCameraApertureModes[] = { "Horizontal and Vertical", "Horizontal", "Vertical" };

//	DisplayString("        Aperture Mode: ", lCameraApertureModes[pCamera->GetApertureMode()]);

	DisplayDouble("        Aperture Width: ", pCamera->GetApertureWidth(), " inches");	
	DisplayDouble("        Aperture Height: ", pCamera->GetApertureHeight(), " inches");	
	DisplayDouble("        Squeeze Ratio: ", pCamera->GetSqueezeRatio());	
	DisplayDouble("        Focal Length: ", pCamera->GetDefaultFocalLength(), "mm");	
	DisplayDouble("        Field of View: ", pCamera->GetDefaultFieldOfView(), " degrees");	
}


void DisplayBackgroundProperties(KFbxCamera* pCamera)
{
	DisplayString("    Background Properties");
	
	DisplayString("        Background File Name: \"", (char *) pCamera->GetBackgroundFileName(), "\"");

	char* lBackgroundDisplayModes[] = { "Disabled", "Always", "When Media" };

	DisplayString("        Background Display Mode: ", lBackgroundDisplayModes[pCamera->GetBackgroundDisplayMode()]);

	char* lCameraBackgroundDrawingModes[] = { "Background", "Foreground", "Background and Foreground" };

	DisplayString("        Background Drawing Mode: ", lCameraBackgroundDrawingModes[pCamera->GetBackgroundDrawingMode()]);
	DisplayBool("        Foreground Matte Threshold Enable: ", pCamera->GetForegroundMatteThresholdEnable());
	
	// This option is only relevant if background drawing mode is set to eFOREGROUND or eBACKGROUND_AND_FOREGROUND.
	if (pCamera->GetBackgroundDrawingMode() != KFbxCamera::eBACKGROUND &&
		pCamera->GetForegroundMatteThresholdEnable())
	{
		DisplayDouble("        Foreground Matte Threshold: ", pCamera->GetForegroundMatteThreshold());
	}

	KString lBackgroundPlacementOptions;

	if (pCamera->GetBackgroundPlacementOptions() & KFbxCamera::eFIT)
	{
		lBackgroundPlacementOptions += " Fit,";
	}
	if (pCamera->GetBackgroundPlacementOptions() & KFbxCamera::eCENTER)
	{
		lBackgroundPlacementOptions += " Center,";
	}
	if (pCamera->GetBackgroundPlacementOptions() & KFbxCamera::eKEEP_RATIO)
	{
		lBackgroundPlacementOptions += " Keep Ratio,";
	}
	if (pCamera->GetBackgroundPlacementOptions() & KFbxCamera::eCROP)
	{
		lBackgroundPlacementOptions += " Crop,";
	}
	if (!lBackgroundPlacementOptions.IsEmpty())
	{
		DisplayString("        Background Placement Options: ", lBackgroundPlacementOptions.Left(lBackgroundPlacementOptions.GetLen() - 1).Buffer());
	}

	DisplayDouble("        Background Distance: ", pCamera->GetBackgroundDistance());

	char* lCameraBackgroundDistanceModes[] = { "Relative to Interest", "Absolute from Camera" };

	DisplayString("        Background Distance Mode: ", lCameraBackgroundDistanceModes[pCamera->GetBackgroundDistanceMode()]);
}


void DisplayCameraViewOptions(KFbxCamera* pCamera)
{
	DisplayString("    Camera View Options");

	DisplayBool("        View Camera Interest: ", pCamera->GetViewCameraInterest());
	DisplayBool("        View Near Far Planes: ", pCamera->GetViewNearFarPlanes());
	DisplayBool("        Show Grid: ", pCamera->GetShowGrid());
	DisplayBool("        Show Axis: ", pCamera->GetShowAxis());
	DisplayBool("        Show Name: ", pCamera->GetShowName());
	DisplayBool("        Show Info on Moving: ", pCamera->GetShowInfoOnMoving());
	DisplayBool("        Show Time Code: ", pCamera->GetShowTimeCode());
	DisplayBool("        Display Safe Area: ", pCamera->GetDisplaySafeArea());

    char* lSafeAreaStyles[] = { "Round", "Square" };

	DisplayString("        Safe Area Style: ", lSafeAreaStyles[pCamera->GetSafeAreaStyle()]);
	DisplayBool("        Show Audio: ", pCamera->GetShowAudio());
	DisplayColor("        Background Color: ", pCamera->GetDefaultBackgroundColor());
	DisplayColor("        Audio Color: ", pCamera->GetAudioColor());
	DisplayBool("        Use Frame Color: ", pCamera->GetUseFrameColor());
	DisplayColor("        Frame Color: ", pCamera->GetFrameColor());
}


void DisplayRenderOptions(KFbxCamera* pCamera)
{
	DisplayString("    Render Options");

    char* lCameraRenderOptionsUsageTimes[] = { "Interactive", "At Render" };

	DisplayString("        Render Options Usage Time: ", lCameraRenderOptionsUsageTimes[pCamera->GetRenderOptionsUsageTime()]);
    DisplayBool("        Use Antialiasing: ", pCamera->GetUseAntialiasing());
	DisplayDouble("        Antialiasing Intensity: ", pCamera->GetAntialiasingIntensity());

	char* lCameraAntialiasingMethods[] = { "Oversampling Antialiasing", "Hardware Antialiasing" };

	DisplayString("        Antialiasing Method: ", lCameraAntialiasingMethods[pCamera->GetAntialiasingMethod()]);

	// This option is only relevant if antialiasing method is set to eOVERSAMPLING_ANTIALIASING.
	if (pCamera->GetAntialiasingMethod() == KFbxCamera::eOVERSAMPLING_ANTIALIASING)
	{
		DisplayInt("        Number of Samples: ", pCamera->GetNumberOfSamples());
	}

    char* lCameraSamplingTypes[] = { "Uniform", "Stochastic" };

	DisplayString("        Sampling Type: ", lCameraSamplingTypes[pCamera->GetSamplingType()]);
	DisplayBool("        Use Accumulation Buffer: ", pCamera->GetUseAccumulationBuffer());
	DisplayBool("        Use Depth of Field: ", pCamera->GetUseDepthOfField());

    char* lCameraFocusDistanceSources[] = { "Camera Interest", "Specific Distance" };

	DisplayString("        Focus Distance Source: ", lCameraFocusDistanceSources[pCamera->GetFocusDistanceSource()]);	

	// This parameter is only relevant if focus distance source is set to eSPECIFIC_DISTANCE.
	if (pCamera->GetFocusDistanceSource() == KFbxCamera::eSPECIFIC_DISTANCE)
	{
		DisplayDouble("        Specific Distance: ", pCamera->GetSpecificDistance());
	}
	
	DisplayDouble("        Focus Angle: ", pCamera->GetFocusAngle(), " degrees");
}


void DisplayDefaultAnimationValues(KFbxCamera* pCamera)
{
	DisplayString("    Default Animation Values");

	DisplayDouble("        Default Field of View: ", pCamera->GetDefaultFieldOfView());
	DisplayDouble("        Default Field of View X: ", pCamera->GetDefaultFieldOfViewX());
	DisplayDouble("        Default Field of View Y: ", pCamera->GetDefaultFieldOfViewY());
	DisplayDouble("        Default Optical Center X: ", pCamera->GetDefaultOpticalCenterX());
	DisplayDouble("        Default Optical Center Y: ", pCamera->GetDefaultOpticalCenterY());
	DisplayDouble("        Default Roll: ", pCamera->GetDefaultRoll());
}


