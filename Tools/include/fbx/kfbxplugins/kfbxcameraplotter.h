/*!  \file kfbxcameraplotter.h
 */

#ifndef _FBXSDK_CAMERA_PLOTTER_H_
#define _FBXSDK_CAMERA_PLOTTER_H_

/**************************************************************************************

 Copyright © 2004 - 2006 Autodesk, Inc. and/or its licensors.
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

#include <kaydaradef.h>
#ifndef KFBX_DLL 
	#define KFBX_DLL K_DLLIMPORT
#endif

#include <kaydara.h>

// Not part of the public FBX SDK
#ifndef K_FBXSDK

#include <fbxfilesdk_nsbegin.h>


class KFbxNode;
class KFbxVector4;

/* PlotCameraConstraints
   Function used to plot the Target and TargetUp constraints on a camera
   pPlotLookAt : True when we want to plot the lookat target contraint on the Ry and Rz of the camera
   pPlotUpNode : True when we want to plot the target Up contraint on the Rx or Roll of the camera
   pPlotUpToRoll : True if we want to plot the target Up contraint on the Roll of the camera
   pResamplingRate : Rate at which the resampling will occur
   pCameraDefaultUp : The Up vector that the destination camera has by default (ex: in 3dsmax = 0,0,1)
   pDefaultUp : The Up vector defining the 0 roll
   */

void PlotCameraConstraints(KFbxNode *pNode, bool pPlotLookAt, bool pPlotUpNode,bool pPlotUpToRoll, 
						   float pResamplingRate , 
						   KFbxVector4 *pCameraDefaultUp = NULL,
						   KFbxVector4 *pDefaultUp = NULL);
#include <fbxfilesdk_nsend.h>

#endif // #ifndef K_FBXSDK
#endif // #define _FBXSDK_CAMERA_PLOTTER_H_

