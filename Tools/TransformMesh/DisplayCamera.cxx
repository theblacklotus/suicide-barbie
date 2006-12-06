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
	DisplayCamera((KFbxCamera*) pNode->GetNodeAttribute(), pNode->GetName(), pNode->GetTarget(), pNode->GetTargetUp());
}


void DisplayCamera(KFbxCamera* pCamera, char* pName, KFbxNode* pTargetNode, KFbxNode* pTargetUpNode)
{
	DisplayString("Camera Name: ", pName);

	DisplayCameraPositionAndOrientation(pCamera, pTargetNode, pTargetUpNode);

	kCharPtr lProjectionTypes[] = { "Perspective", "Orthogonal" };

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
		DisplayString("        Camera Interest: ", pTargetNode->GetName());
	}
	else
	{
		Display3DVector("        Default Camera Interest Position: ", pCamera->GetDefaultCameraInterestPosition());
	}

	if (pTargetUpNode)
	{
		DisplayString("        Camera Up Target: ", pTargetUpNode->GetName());
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

	kCharPtr lCameraFormat[] = { "NTSC", "D1 NTSC", "PAL", "D1 PAL", 
		                         "HD", "640x480", "320x200", "320x240", 
								 "Full Screen", "Custom" };

	DisplayString("        Format: ", lCameraFormat[pCamera->GetFormat()]);

	kCharPtr lAspectRatioModes[] = { "Window Size", 
		                             "Fixed Ratio", "Fixed Resolution", 
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
 
	kCharPtr lCameraApertureFormats[] = { "16mm Theatrical",
										  "Super 16mm",
										  "35mm Academy",
										  "35mm TV Projection",
										  "35mm Full Aperture",
										  "35mm 1.85 Projection",
										  "35mm Anamorphic",
										  "70mm Projection",
										  "VistaVision",
										  "Dynavision",
										  "Imax",
										  "Custom" };
							
	DisplayString("        Aperture Format: ", lCameraApertureFormats[pCamera->GetApertureFormat()]);

    kCharPtr lCameraApertureModes[] = { "Horizontal and Vertical", "Horizontal", "Vertical" };

	DisplayString("        Aperture Mode: ", lCameraApertureModes[pCamera->GetApertureMode()]);

	DisplayDouble("        Aperture Width: ", pCamera->GetApertureWidth(), " inches");	
	DisplayDouble("        Aperture Height: ", pCamera->GetApertureHeight(), " inches");	
	DisplayDouble("        Squeeze Ratio: ", pCamera->GetSqueezeRatio());	
	DisplayDouble("        Focal Length: ", pCamera->GetFocalLength(), "mm");	
	DisplayDouble("        Angle of View: ", pCamera->GetAngleOfView(), " degrees");	
}


void DisplayBackgroundProperties(KFbxCamera* pCamera)
{
	DisplayString("    Background Properties");
	
	DisplayString("        Background File Name: \"", pCamera->GetBackgroundFileName(), "\"");

	kCharPtr lBackgroundDisplayModes[] = { "Disabled", "Always", "When Media" };

	DisplayString("        Background Display Mode: ", lBackgroundDisplayModes[pCamera->GetBackgroundDisplayMode()]);

	kCharPtr lCameraBackgroundDrawingModes[] = { "Background", "Foreground", "Background and Foreground" };

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

	kCharPtr lCameraBackgroundDistanceModes[] = { "Relative to Interest", "Absolute from Camera" };

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

    kCharPtr lSafeAreaStyles[] = { "Round", "Square" };

	DisplayString("        Safe Area Style: ", lSafeAreaStyles[pCamera->GetSafeAreaStyle()]);
	DisplayBool("        Show Audio: ", pCamera->GetShowAudio());
	DisplayColor("        Background Color: ", pCamera->GetBackgroundColor());
	DisplayColor("        Audio Color: ", pCamera->GetAudioColor());
	DisplayBool("        Use Frame Color: ", pCamera->GetUseFrameColor());
	DisplayColor("        Frame Color: ", pCamera->GetFrameColor());
}


void DisplayRenderOptions(KFbxCamera* pCamera)
{
	DisplayString("    Render Options");

    kCharPtr lCameraRenderOptionsUsageTimes[] = { "Interactive", "At Render" };

	DisplayString("        Render Options Usage Time: ", lCameraRenderOptionsUsageTimes[pCamera->GetRenderOptionsUsageTime()]);
    DisplayBool("        Use Antialiasing: ", pCamera->GetUseAntialiasing());
	DisplayDouble("        Antialiasing Intensity: ", pCamera->GetAntialiasingIntensity());

	kCharPtr lCameraAntialiasingMethods[] = { "Oversampling Antialiasing", "Hardware Antialiasing" };

	DisplayString("        Antialiasing Method: ", lCameraAntialiasingMethods[pCamera->GetAntialiasingMethod()]);

	// This option is only relevant if antialiasing method is set to eOVERSAMPLING_ANTIALIASING.
	if (pCamera->GetAntialiasingMethod() == KFbxCamera::eOVERSAMPLING_ANTIALIASING)
	{
		DisplayInt("        Number of Samples: ", pCamera->GetNumberOfSamples());
	}

    kCharPtr lCameraSamplingTypes[] = { "Uniform", "Stochastic" };

	DisplayString("        Sampling Type: ", lCameraSamplingTypes[pCamera->GetSamplingType()]);
	DisplayBool("        Use Accumulation Buffer: ", pCamera->GetUseAccumulationBuffer());
	DisplayBool("        Use Depth of Field: ", pCamera->GetUseDepthOfField());

    kCharPtr lCameraFocusDistanceSources[] = { "Camera Interest", "Specific Distance" };

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


