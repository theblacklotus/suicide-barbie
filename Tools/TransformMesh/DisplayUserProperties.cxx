/**************************************************************************************

 Copyright © 2004 - 2005 Systèmes Alias Québec Inc. and/or its licensors.  
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

#include <kfbxplugins/kfbxuserproperty.h>

#include "DisplayCommon.h"

#include <stdio.h>

using namespace FBXSDK_NAMESPACE;

void DisplayUserProperties(KFbxObject* pObject)
{
		int i,  lCount    = pObject->GetPropertyCount();
		KString lTitleStr = "    Property Count: ";
		
		if (pObject->GetPropertyCount(KFbxUserProperty::eUSER) == 0)
			return; // there are no user properties to display

		DisplayInt(lTitleStr.Buffer(), pObject->GetPropertyCount(KFbxUserProperty::eUSER));

		for (i=0; i<lCount; i++)
		{
			KFbxUserProperty* lProperty = pObject->GetProperty(i);
			if (!lProperty->GetFlag(KFbxUserProperty::eUSER))
				continue; // only print user properties
			
			DisplayInt("        Property ", i);
			DisplayString("            Display Name: ", lProperty->GetLabel().Buffer());
			DisplayString("            Internal Name: ", lProperty->GetName().Buffer());
			DisplayString("            Type: ", lProperty->GetPropertyTypeName(lProperty->GetPropertyType()));
			DisplayDouble("            Min Limit: ", lProperty->GetMinLimit());
			DisplayDouble("            Max Limit: ", lProperty->GetMaxLimit());
			DisplayBool  ("            Is Animatable: ", lProperty->GetFlag(KFbxUserProperty::eANIMATABLE));
			DisplayBool  ("            Is Temporary: ", lProperty->GetFlag(KFbxUserProperty::eTEMPORARY));

			switch (lProperty->GetPropertyType())
			{
				case KFbxUserProperty::eBOOL:
				{
					bool lDefault;

					lProperty->GetDefaultValue(lDefault);
					DisplayBool("            Default Value: ", lDefault);
				}
				break;

				case KFbxUserProperty::eREAL:
				{
					double lDefault;

					lProperty->GetDefaultValue(lDefault);
					DisplayDouble("            Default Value: ", lDefault);
				}
				break;

				case KFbxUserProperty::eCOLOR:
				{
					KFbxColor lDefault;
					char      lBuf[64];

					lProperty->GetDefaultValue(lDefault);
					sprintf(lBuf, "R=%f, G=%f, B=%f, A=%f", lDefault.mRed, lDefault.mGreen, lDefault.mBlue, lDefault.mAlpha);
					DisplayString("            Default Value: ", lBuf);
				}
				break;

				case KFbxUserProperty::eINTEGER:
				{
					int lDefault;

					lProperty->GetDefaultValue(lDefault);
					DisplayInt("            Default Value: ", lDefault);
				}
				break;

				case KFbxUserProperty::eVECTOR:
				{
					double lDefaultX,lDefaultY,lDefaultZ;
					char   lBuf[64];

					lProperty->GetDefaultValue(lDefaultX, lDefaultY, lDefaultZ);
					sprintf(lBuf, "X=%f, Y=%f, Z=%f", lDefaultX, lDefaultY, lDefaultZ);
					DisplayString("            Default Value: ", lBuf);
				}
				break;

				case KFbxUserProperty::eUNIDENTIFIED:
				{
					DisplayString("            Default Value: UNIDENTIFIED");
				}
				break;
			}
		}
}
