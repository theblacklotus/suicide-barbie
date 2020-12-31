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

void DisplayMarker(KFbxNode* pNode)
{
	KFbxMarker* lMarker = (KFbxMarker*)pNode->GetNodeAttribute();
	KString     lString;

    DisplayString("Marker Name: ", (char *) pNode->GetName());

	// Type
	lString = "    Marker Type: ";
	switch (lMarker->GetType())
	{
		case KFbxMarker::eSTANDARD:    lString += "Standard";    break;
		case KFbxMarker::eOPTICAL:     lString += "Optical";     break;
		case KFbxMarker::eIK_EFFECTOR: lString += "IK Effector"; break;
		case KFbxMarker::eFK_EFFECTOR: lString += "FK Effector"; break;
	}
	DisplayString(lString.Buffer());

	// Look
	lString = "    Marker Look: ";
	switch (lMarker->GetLook())
	{
		case KFbxMarker::eCUBE:			lString += "Cube";        break;
		case KFbxMarker::eHARD_CROSS:	lString += "Hard Cross";  break;
		case KFbxMarker::eLIGHT_CROSS:	lString += "Light Cross"; break;
		case KFbxMarker::eSPHERE:		lString += "Sphere";      break;
	}
	DisplayString(lString.Buffer());

	// Size
	lString = KString("    Size: ") + KString(lMarker->GetSize());
	DisplayString(lString.Buffer());

	// Color
	DisplayColor("    Color: ", lMarker->GetColor());	

	// IKPivot
	Display3DVector("    IKPivot: ", lMarker->GetIKPivot());	
}

