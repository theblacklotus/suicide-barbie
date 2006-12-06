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

#include <stdio.h>

#include "DisplayCommon.h"

using namespace FBXSDK_NAMESPACE;

void DisplayAnimation(KFbxNode* pNode, bool isSwitcher = false);
void DisplayChannels(KFbxNode* pNode, KFbxTakeNode* pTakeNode, void (*DisplayCurve) (KFCurve *pCurve), bool isSwitcher);
void DisplayCurveKeys(KFCurve *pCurve);
void DisplayCurveDefault(KFCurve *pCurve);

void DisplayAnimation(KFbxScene* pScene)
{
	KArrayTemplate<KString*> lTakeNameArray;
	int i;

	pScene->FillTakeNameArray(lTakeNameArray);

	for(i = 0; i < lTakeNameArray.GetCount(); i++)
	{
		// It's useless to display the default animation because it is always empty.
		if(lTakeNameArray.GetAt(i)->Compare(KFBXTAKENODE_DEFAULT_NAME) == 0)
		{
			continue;
		}
		
		KString lOutputString = "Take Name: ";

		pScene->SetCurrentTake(lTakeNameArray.GetAt(i)->Buffer());
		lOutputString += pScene->GetCurrentTakeName();
		lOutputString += "\n\n";
		printf(lOutputString);

		DisplayAnimation(pScene->GetCameraSwitcher(), true);
		DisplayAnimation(pScene->GetRootNode());	
	}

	DeleteAndClear(lTakeNameArray);
}

void DisplayAnimation(KFbxNode* pNode, bool isSwitcher)
{
    KFbxTakeNode* lCurrentTakeNode = pNode->GetCurrentTakeNode();
	kInt lModelCount;

	// Display nothing if the current take node points to default values.
    if(lCurrentTakeNode && lCurrentTakeNode != pNode->GetDefaultTakeNode())
    {
		KString lOutputString;

		lOutputString = "     Node Name: ";
		lOutputString += pNode->GetName();
		lOutputString += "\n\n";
		printf(lOutputString);
	
		DisplayChannels(pNode, lCurrentTakeNode, DisplayCurveKeys, isSwitcher);
	
		printf ("\n");
	}

	for(lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
    {
        DisplayAnimation(pNode->GetChild(lModelCount), isSwitcher);
    }
}

void DisplayDefaultAnimation(KFbxNode* pNode)
{
    KFbxTakeNode* lDefaultTakeNode = pNode->GetDefaultTakeNode();

	printf("    Default Animation\n");

	if(lDefaultTakeNode)
	{
		DisplayChannels(pNode, lDefaultTakeNode, DisplayCurveDefault, false);
	}

	printf ("\n");
}

void DisplayChannels(KFbxNode* pNode, KFbxTakeNode* pTakeNode, void (*DisplayCurve) (KFCurve *pCurve), bool isSwitcher)
{
	KFCurve* lCurve = NULL;

	// Display general curves.
	if (!isSwitcher)
	{
		lCurve = pTakeNode->GetTranslationX();
		printf("        TX\n");
		DisplayCurve(lCurve);
		
		lCurve = pTakeNode->GetTranslationY();
		printf("        TY\n");
		DisplayCurve(lCurve);

		lCurve = pTakeNode->GetTranslationZ();
		printf("        TZ\n");
		DisplayCurve(lCurve);

		lCurve = pTakeNode->GetEulerRotationX();
		printf("        RX\n");
		DisplayCurve(lCurve);

		lCurve = pTakeNode->GetEulerRotationY();
		printf("        RY\n");
		DisplayCurve(lCurve);

		lCurve = pTakeNode->GetEulerRotationZ();
		printf("        RZ\n");
		DisplayCurve(lCurve);
 
		lCurve = pTakeNode->GetScaleX();
		printf("        SX\n");
		DisplayCurve(lCurve);
            
		lCurve = pTakeNode->GetScaleY();
		printf("        SY\n");
		DisplayCurve(lCurve);

		lCurve = pTakeNode->GetScaleZ();
		printf("        SZ\n");
		DisplayCurve(lCurve);
	}

	// Display curves specific to a light or marker.

	lCurve = pTakeNode->GetColorR();

	if(lCurve)
	{
		printf("        Red\n");
		DisplayCurve(lCurve);
	}
	
	lCurve = pTakeNode->GetColorG();

	if(lCurve)
	{
		printf("        Green\n");
		DisplayCurve(lCurve);
	}

	lCurve = pTakeNode->GetColorB();

	if(lCurve)
	{
		printf("        Blue\n");
		DisplayCurve(lCurve);
	}

	// Display curves specific to a light.

	lCurve = pTakeNode->GetLightIntensity();

	if(lCurve)
	{
		printf("        Intensity\n");
		DisplayCurve(lCurve);
	}

	lCurve = pTakeNode->GetLightConeAngle();

	if(lCurve)
	{
		printf("        Cone Angle\n");
		DisplayCurve(lCurve);
	}

	lCurve = pTakeNode->GetLightFog();

	if(lCurve)
	{
		printf("        Fog\n");
		DisplayCurve(lCurve);
	}

	// Display curves specific to a camera.

	lCurve = pTakeNode->GetCameraFieldOfView();

	if(lCurve)
	{
		printf("        Field of View\n");
		DisplayCurve(lCurve);
	}
	
	lCurve = pTakeNode->GetCameraFieldOfViewX();

	if(lCurve)
	{
		printf("        Field of View X\n");
		DisplayCurve(lCurve);
	}

	lCurve = pTakeNode->GetCameraFieldOfViewY();

	if(lCurve)
	{
		printf("        Field of View Y\n");
		DisplayCurve(lCurve);
	}

	lCurve = pTakeNode->GetCameraOpticalCenterX();

	if(lCurve)
	{
		printf("        Optical Center X\n");
		DisplayCurve(lCurve);
	}
	
	lCurve = pTakeNode->GetCameraOpticalCenterY();

	if(lCurve)
	{
		printf("        Optical Center Y\n");
		DisplayCurve(lCurve);
	}

	lCurve = pTakeNode->GetCameraRoll();

	if(lCurve)
	{
		printf("        Roll\n");
		DisplayCurve(lCurve);
	}

	// Display curves specific to a geometry.

	KFbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

	if(lNodeAttribute)
	{
		if(lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eMESH ||
		   lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eNURB ||
		   lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::ePATCH)
		{
			KFbxGeometry* lGeometry = (KFbxGeometry*) lNodeAttribute;
			
			kInt lCount;

			for(lCount = 0; lCount < lGeometry->GetShapeCount(); lCount++)
			{
				KString lShapeName = lGeometry->GetShapeName(lCount);

				lCurve = lGeometry->GetShapeChannel(pTakeNode, lCount);

				if(lCurve)
				{
					printf("        Shape %s\n", lShapeName.Buffer());
					DisplayCurve(lCurve);
				}
			}
		}
	}

	// Display curves specific to properties
	int i, lCount = pNode->GetPropertyCount();

	for (i=0; i<lCount; i++)
	{
		KFbxUserProperty* lProperty = pNode->GetProperty(i);

		KString lFbxFCurveNodeName  = lProperty->GetName();
		KFCurveNode* lFbxFCurveNode = pTakeNode->GetKFCurveNode()->FindRecursive(lFbxFCurveNodeName.Buffer());

		if (!lFbxFCurveNode) continue;

		switch (lProperty->GetPropertyType())
		{
			case KFbxUserProperty::eBOOL:
			case KFbxUserProperty::eREAL:
			case KFbxUserProperty::eINTEGER:
			{
				KString lMessage;
				lCurve = lFbxFCurveNode->FCurveGet();

				lMessage =  "        Property ";
				lMessage += lProperty->GetName();
				if (lProperty->GetLabel().GetLen() > 0)
				{
					lMessage += " (Label: ";
					lMessage += lProperty->GetLabel();
					lMessage += ")";
				};

				DisplayString(lMessage.Buffer());
				DisplayCurve(lCurve);
			}
			break;

			case KFbxUserProperty::eVECTOR:
			case KFbxUserProperty::eCOLOR:
			{
				kCharPtr lComponentName1 = lProperty->GetPropertyType() == KFbxUserProperty::eCOLOR ? (kCharPtr)KFCURVENODE_COLOR_RED : (kCharPtr)"X";
				kCharPtr lComponentName2 = lProperty->GetPropertyType() == KFbxUserProperty::eCOLOR ? (kCharPtr)KFCURVENODE_COLOR_GREEN : (kCharPtr)"Y";
				kCharPtr lComponentName3 = lProperty->GetPropertyType() == KFbxUserProperty::eCOLOR ? (kCharPtr)KFCURVENODE_COLOR_BLUE  : (kCharPtr)"Z";
				KFCurveNode* lFbxComponentFCurveNode = NULL;
				KString      lMessage;

				lMessage =  "        Property ";
				lMessage += lProperty->GetName();
				if (lProperty->GetLabel().GetLen() > 0)
				{
					lMessage += " (Label: ";
					lMessage += lProperty->GetLabel();
					lMessage += ")";
				}

				DisplayString(lMessage.Buffer());

				lFbxComponentFCurveNode = lFbxFCurveNode->FindRecursive(lComponentName1);
				if (lFbxComponentFCurveNode)
				{
					lCurve = lFbxComponentFCurveNode->FCurveGet();
					DisplayString("        Component ", lComponentName1);
					DisplayCurve(lCurve);
				}

				lFbxComponentFCurveNode = lFbxFCurveNode->FindRecursive(lComponentName2);
				if (lFbxComponentFCurveNode)
				{
					lCurve = lFbxComponentFCurveNode->FCurveGet();
					DisplayString("        Component ", lComponentName2);
					DisplayCurve(lCurve);
				}

				lFbxComponentFCurveNode = lFbxFCurveNode->FindRecursive(lComponentName3);
				if (lFbxComponentFCurveNode)
				{
					lCurve = lFbxComponentFCurveNode->FCurveGet();
					DisplayString("        Component ", lComponentName3);
					DisplayCurve(lCurve);
				}
			}
			break;
		}
	}
	
}


static int InterpolationFlagToIndex(int flags)
{
	if ((flags&KFCURVE_INTERPOLATION_CONSTANT)==KFCURVE_INTERPOLATION_CONSTANT)
		return 1;
	if ((flags&KFCURVE_INTERPOLATION_LINEAR)==KFCURVE_INTERPOLATION_LINEAR)
		return 2;
	if ((flags&KFCURVE_INTERPOLATION_CUBIC)==KFCURVE_INTERPOLATION_CUBIC)
		return 3;
	return 0;
}

static int ConstantmodeFlagToIndex(int flags)
{
	if ((flags&KFCURVE_CONSTANT_STANDARD)==KFCURVE_CONSTANT_STANDARD)
		return 1;
	if ((flags&KFCURVE_CONSTANT_NEXT)==KFCURVE_CONSTANT_NEXT)
		return 2;
	return 0;
}

static int TangeantmodeFlagToIndex(int flags)
{
	if ((flags&KFCURVE_TANGEANT_AUTO) == KFCURVE_TANGEANT_AUTO)
		return 1;
	if ((flags&KFCURVE_TANGEANT_AUTO_BREAK)==KFCURVE_TANGEANT_AUTO_BREAK)
		return 2;
	if ((flags&KFCURVE_TANGEANT_TCB) == KFCURVE_TANGEANT_TCB)
		return 3;
	if ((flags&KFCURVE_TANGEANT_USER) == KFCURVE_TANGEANT_USER)
		return 4;
	if ((flags&KFCURVE_GENERIC_BREAK) == KFCURVE_GENERIC_BREAK)
		return 5;
	if ((flags&KFCURVE_TANGEANT_BREAK) ==KFCURVE_TANGEANT_BREAK)
		return 6;
	return 0;
}

static int TangeantweightFlagToIndex(int flags)
{
	if ((flags&KFCURVE_WEIGHTED_NONE) == KFCURVE_WEIGHTED_NONE)
		return 1;
	if ((flags&KFCURVE_WEIGHTED_RIGHT) == KFCURVE_WEIGHTED_RIGHT)
		return 2;
	if ((flags&KFCURVE_WEIGHTED_NEXT_LEFT) == KFCURVE_WEIGHTED_NEXT_LEFT)
		return 3;
	return 0;
}

static int TangeantVelocityFlagToIndex(int flags)
{
	if ((flags&KFCURVE_VELOCITY_NONE) == KFCURVE_VELOCITY_NONE)
		return 1;
	if ((flags&KFCURVE_VELOCITY_RIGHT) == KFCURVE_VELOCITY_RIGHT)
		return 2;
	if ((flags&KFCURVE_VELOCITY_NEXT_LEFT) == KFCURVE_VELOCITY_NEXT_LEFT)
		return 3;
	return 0;
}

void DisplayCurveKeys(KFCurve *pCurve)
{
	static char* interpolation[] = { "?", "constant", "linear", "cubic"};
	static char* constantMode[] =  { "?", "Standard", "Next" };
	static char* cubicMode[] =     { "?", "Auto", "Auto break", "Tcb", "User", "Break", "User break" };
	static char* tangentWVMode[] = { "?", "None", "Right", "Next left" };


	KFCurveKey* lKey;
	KTime lKeyTime;
	char lTimeString[256];
    KString lOutputString;
	int lCount;

	int lKeyCount = pCurve->KeyGetCount();
	
    for(lCount = 0; lCount < lKeyCount; lCount++)
    {
		lKey = pCurve->KeyGetPtr(lCount);
        lOutputString = "            Key Time: ";
        lKeyTime = lKey->GetTime();
        lOutputString += lKeyTime.GetTimeString(lTimeString);
        lOutputString += ".... Key Value: ";
        lOutputString += static_cast<float> (lKey->GetValue());
		lOutputString += " [ ";
		lOutputString += interpolation[ InterpolationFlagToIndex(lKey->GetInterpolation()) ];
		if ((lKey->GetInterpolation()&KFCURVE_INTERPOLATION_CONSTANT) == KFCURVE_INTERPOLATION_CONSTANT)
		{
			lOutputString += " | ";
			lOutputString += constantMode[ ConstantmodeFlagToIndex(lKey->GetConstantMode()) ];
			lOutputString += " ]";
		}
		else
		if ((lKey->GetInterpolation()&KFCURVE_INTERPOLATION_CUBIC) == KFCURVE_INTERPOLATION_CUBIC)
		{
			lOutputString += " | ";
			lOutputString += cubicMode[ TangeantmodeFlagToIndex(lKey->GetTangeantMode()) ];
			lOutputString += " | ";
			lOutputString += tangentWVMode[ TangeantweightFlagToIndex(lKey->GetTangeantWeightMode()) ];
			lOutputString += " | ";
			lOutputString += tangentWVMode[ TangeantVelocityFlagToIndex(lKey->GetTangeantVelocityMode()) ];
			lOutputString += " ]";
		}
        lOutputString += "\n";
        printf (lOutputString);
    }
}

void DisplayCurveDefault(KFCurve *pCurve)
{
    KString lOutputString;

    lOutputString = "            Default Value: ";
    lOutputString += static_cast<float> (pCurve->GetValue());
    lOutputString += "\n";
    printf (lOutputString);
}


