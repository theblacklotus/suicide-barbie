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

void DisplayTexture(KFbxGeometry* pGeometry)
{
	for (int l = 0; l < pGeometry->GetLayerCount(); l++)
	{
		KFbxLayerElementTexture* leTex = pGeometry->GetLayer(l)->GetTextures();
		if (leTex)
		{
			if (leTex->GetReferenceMode() == KFbxLayerElement::eINDEX)
				// textures are in an undefined external table
				continue;

			if (leTex->GetDirectArray().GetCount())
			{
				char header[100];
				sprintf(header, "    Textures on layer %d: ", l); 
				DisplayString(header);

			    for (int lCount = 0; lCount < leTex->GetDirectArray().GetCount(); lCount++)
				{
					DisplayInt("        Texture ", lCount);

					KFbxTexture* lTexture = leTex->GetDirectArray().GetAt(lCount);

					DisplayString("            Name: \"", lTexture->GetName(), "\"");
					DisplayString("            File Name: \"", lTexture->GetFileName(), "\"");
					DisplayDouble("            Scale U: ", lTexture->GetScaleU());
					DisplayDouble("            Scale V: ", lTexture->GetScaleV());
					DisplayDouble("            Translation U: ", lTexture->GetTranslationU());
					DisplayDouble("            Translation V: ", lTexture->GetTranslationV());
					DisplayBool("            Swap UV: ", lTexture->GetSwapUV());
					DisplayDouble("            Rotation U: ", lTexture->GetRotationU());
					DisplayDouble("            Rotation V: ", lTexture->GetRotationV());
					DisplayDouble("            Rotation W: ", lTexture->GetRotationW());

					kCharPtr lAlphaSources[] = { "None", "RGB Intensity", "Black" };

					DisplayString("            Alpha Source: ", lAlphaSources[lTexture->GetAlphaSource()]);
					DisplayDouble("            Cropping Left: ", lTexture->GetCroppingLeft());
					DisplayDouble("            Cropping Top: ", lTexture->GetCroppingTop());
					DisplayDouble("            Cropping Right: ", lTexture->GetCroppingRight());
					DisplayDouble("            Cropping Bottom: ", lTexture->GetCroppingBottom());

					kCharPtr lMappingTypes[] = { "Null", "Planar", "Spherical", "Cylindrical", 
								                 "Box", "Face", "UV", "Environment" };

					DisplayString("            Mapping Type: ", lMappingTypes[lTexture->GetMappingType()]);

					if (lTexture->GetMappingType() == KFbxTexture::ePLANAR)
					{
						kCharPtr lPlanarMappingNormals[] = { "X", "Y", "Z" };

						DisplayString("            Planar Mapping Normal: ", lPlanarMappingNormals[lTexture->GetPlanarMappingNormal()]);
					}

					kCharPtr lMaterialUses[] = { "Model Material", "Default Material" };

					DisplayString("            Material Use: ", lMaterialUses[lTexture->GetMaterialUse()]);

					kCharPtr lTextureUses[] = { "Standard", "Shadow Map", "Light Map", 
								                "Spherical Reflexion Map", "Sphere Reflexion Map" };

					DisplayString("            Texture Use: ", lTextureUses[lTexture->GetTextureUse()]);
					DisplayString("");
				}
			}
		}
	}
}

	


