
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

#include <stdio.h>
#include "DisplayCommon.h"
#include <fbxfilesdk_nsuse.h>


void DisplayProperties(KFbxObject* pObject);
void DisplayGenericInfo(KFbxNode* pNode, int pDepth);
void DisplayGenericInfo(KFbxScene* pScene)
{
	int i;
	KFbxNode* lRootNode = pScene->GetRootNode();

	for( i = 0; i < lRootNode->GetChildCount(); i++)
	{
		DisplayGenericInfo(lRootNode->GetChild(i), 0);
	}
}


void DisplayGenericInfo(KFbxNode* pNode, int pDepth)
{
	KString lString;
	int i;

	for(i = 0; i < pDepth; i++)
	{
		lString += "     ";
	}

	lString += pNode->GetName();
	lString += "\n";

	DisplayString(lString.Buffer());

	//Display generic info about that Node
	DisplayProperties(pNode);	
	DisplayString("");
	for(i = 0; i < pNode->GetChildCount(); i++)
	{
		DisplayGenericInfo(pNode->GetChild(i), pDepth + 1);
	}
}



void DisplayProperties(KFbxObject* pObject)
{
	
	DisplayString("Name: ", (char *)pObject->GetName());

	int i,  lCount    = pObject->GetPropertyCount();
	KString lTitleStr = "    Property Count: ";
	
	if (pObject->GetPropertyCount() == 0)
		return;

	DisplayInt(lTitleStr.Buffer(), pObject->GetPropertyCount());

	for (i=0; i<lCount; i++)
	{
		KFbxUserProperty lProperty = pObject->GetProperty(i);

	

		DisplayInt("        Property ", i);
		DisplayString("            Display Name: ", lProperty.GetLabel().Buffer());
		DisplayString("            Internal Name: ", lProperty.GetName().Buffer());
		DisplayString("            Type: ", lProperty.GetPropertyDataType().GetName());
		DisplayDouble("            Min Limit: ", lProperty.GetMinLimit());
		DisplayDouble("            Max Limit: ", lProperty.GetMaxLimit());
		DisplayBool  ("            Is Animatable: ", lProperty.GetFlag(KFbxUserProperty::eANIMATABLE));
		DisplayBool  ("            Is Temporary: ", lProperty.GetFlag(KFbxUserProperty::eTEMPORARY));


		switch (lProperty.GetPropertyDataType().GetType())
		{
			case eBOOL1:
				DisplayBool("            Default Value: ", KFbxGet<bool>(lProperty));
			break;

			case eDOUBLE1:
				DisplayDouble("            Default Value: ",KFbxGet<double>(lProperty));
			break;

			case eDOUBLE4:
			{
				KFbxColor lDefault;
				char      lBuf[64];
				
				lDefault=KFbxGet <KFbxColor> (lProperty);
				sprintf(lBuf, "R=%f, G=%f, B=%f, A=%f", lDefault.mRed, lDefault.mGreen, lDefault.mBlue, lDefault.mAlpha);
				DisplayString("            Default Value: ", lBuf);
			}
			break;

			case eINTEGER1:
				DisplayInt("            Default Value: ", KFbxGet <int> (lProperty));
			break;

			case eDOUBLE3:
			{
				fbxDouble3 lDefault;
				char   lBuf[64];
				
				lDefault = KFbxGet <fbxDouble3> (lProperty);
				sprintf(lBuf, "X=%f, Y=%f, Z=%f", lDefault[0], lDefault[1], lDefault[2]);
				DisplayString("            Default Value: ", lBuf);
			}
			break;

			//case DTEnum:
			//	DisplayInt("            Default Value: ", KFbxGet <int> (lProperty));
			//break;

			case eFLOAT1:
				DisplayDouble("            Default Value: ", KFbxGet <float> (lProperty));
			break;
			case eSTRING:
				DisplayString("            Default Value: ", (KFbxGet <KString> (lProperty)).Buffer());
			break;

			default:
				DisplayString("            Default Value: UNIDENTIFIED");
			break;
		}
	}
}

