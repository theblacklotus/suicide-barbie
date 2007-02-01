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

#include <fbxsdk.h>

#include <kfbxplugins/kfbxuserproperty.h>

#include "DisplayCommon.h"

#include <stdio.h>

#include <fbxfilesdk_nsuse.h>

void DisplayUserProperties(KFbxObject* pObject)
{
		int i,  lCount    = pObject->GetPropertyCount();
		KString lTitleStr = "    Property Count: ";
		
		if (pObject->GetPropertyCount() == 0)
			return; // there are no user properties to display

		DisplayInt(lTitleStr.Buffer(), pObject->GetPropertyCount());

		for (i=0; i<lCount; i++)
		{
			KFbxUserProperty lProperty = pObject->GetProperty(i);
			if (!lProperty.GetFlag(KFbxUserProperty::eUSER))
				continue; // only print user properties
			
			DisplayInt("        Property ", i);
			DisplayString("            Display Name: ", lProperty.GetLabel().Buffer());
			DisplayString("            Internal Name: ", lProperty.GetName().Buffer());
			DisplayString("            Type: ", lProperty.GetPropertyDataType().GetName());
			DisplayDouble("            Min Limit: ", lProperty.GetMinLimit());
			DisplayDouble("            Max Limit: ", lProperty.GetMaxLimit());
			DisplayBool  ("            Is Animatable: ", lProperty.GetFlag(KFbxUserProperty::eANIMATABLE));
			DisplayBool  ("            Is Temporary: ", lProperty.GetFlag(KFbxUserProperty::eTEMPORARY));
			
			KFbxDataType lPropertyDataType=lProperty.GetPropertyDataType();
			
			KFbxUserProperty::EUserPropertyType lEUserPropertyType = DataTypeToEUserPropertyType(lPropertyDataType);
			//KFbxProperty aProperty;
			switch (lEUserPropertyType)
			{
				case KFbxUserProperty::eBOOL:
					DisplayBool("            Default Value: ", KFbxGet<bool>(lProperty));
				break;

				case KFbxUserProperty::eREAL:
					DisplayDouble("            Default Value: ",KFbxGet<double>(lProperty));
				break;

				case KFbxUserProperty::eCOLOR:
				{
					KFbxColor lDefault;
					char      lBuf[64];
					
					lDefault=KFbxGet <KFbxColor> (lProperty);
					sprintf(lBuf, "R=%f, G=%f, B=%f, A=%f", lDefault.mRed, lDefault.mGreen, lDefault.mBlue, lDefault.mAlpha);
					DisplayString("            Default Value: ", lBuf);
				}
				break;

				case KFbxUserProperty::eINTEGER:
					DisplayInt("            Default Value: ", KFbxGet <int> (lProperty));
				break;

				case KFbxUserProperty::eVECTOR:
				{
					fbxDouble3 lDefault;
					char   lBuf[64];
					
					lDefault = KFbxGet <fbxDouble3> (lProperty);
					sprintf(lBuf, "X=%f, Y=%f, Z=%f", lDefault[0], lDefault[1], lDefault[2]);
					DisplayString("            Default Value: ", lBuf);
				}
				break;

				case KFbxUserProperty::eLIST:
					DisplayInt("            Default Value: ", KFbxGet <int> (lProperty));
				break;

				case KFbxUserProperty::eUNIDENTIFIED:
					DisplayString("            Default Value: UNIDENTIFIED");
				break;
			}
		}
}

