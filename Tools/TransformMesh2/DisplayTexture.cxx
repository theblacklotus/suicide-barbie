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

void DisplayTextureInfo(KFbxLayerElementTexture* leTex, int l)
{
	char header[100];
	sprintf(header, "    Textures on layer %d: ", l); 
	DisplayString(header);

	for (int lCount = 0; lCount < leTex->GetDirectArray().GetCount(); lCount++)
	{
		DisplayInt("        Texture ", lCount);

		KFbxTexture* lTexture = leTex->GetDirectArray().GetAt(lCount);

		DisplayString("            Name: \"", (char *) lTexture->GetName(), "\"");
		DisplayString("            File Name: \"", (char *) lTexture->GetFileName(), "\"");
		DisplayDouble("            Scale U: ", lTexture->GetScaleU());
		DisplayDouble("            Scale V: ", lTexture->GetScaleV());
		DisplayDouble("            Translation U: ", lTexture->GetTranslationU());
		DisplayDouble("            Translation V: ", lTexture->GetTranslationV());
		DisplayBool("            Swap UV: ", lTexture->GetSwapUV());
		DisplayDouble("            Rotation U: ", lTexture->GetRotationU());
		DisplayDouble("            Rotation V: ", lTexture->GetRotationV());
		DisplayDouble("            Rotation W: ", lTexture->GetRotationW());

		char* lAlphaSources[] = { "None", "RGB Intensity", "Black" };

		DisplayString("            Alpha Source: ", lAlphaSources[lTexture->GetAlphaSource()]);
		DisplayDouble("            Cropping Left: ", lTexture->GetCroppingLeft());
		DisplayDouble("            Cropping Top: ", lTexture->GetCroppingTop());
		DisplayDouble("            Cropping Right: ", lTexture->GetCroppingRight());
		DisplayDouble("            Cropping Bottom: ", lTexture->GetCroppingBottom());

		char* lMappingTypes[] = { "Null", "Planar", "Spherical", "Cylindrical", 
									 "Box", "Face", "UV", "Environment" };

		DisplayString("            Mapping Type: ", lMappingTypes[lTexture->GetMappingType()]);

		if (lTexture->GetMappingType() == KFbxTexture::ePLANAR)
		{
			char* lPlanarMappingNormals[] = { "X", "Y", "Z" };

			DisplayString("            Planar Mapping Normal: ", lPlanarMappingNormals[lTexture->GetPlanarMappingNormal()]);
		}

		char* lMaterialUses[] = { "Model Material", "Default Material" };

		DisplayString("            Material Use: ", lMaterialUses[lTexture->GetMaterialUse()]);

		char* lTextureUses[] = { "Standard", "Shadow Map", "Light Map", 
									"Spherical Reflexion Map", "Sphere Reflexion Map" };

		DisplayString("            Texture Use: ", lTextureUses[lTexture->GetTextureUse()]);
		DisplayString("");
	}

}

void DisplayTexture(KFbxGeometry* pGeometry)
{
	//go through all the possible textures too
	for (int l = 0; l < pGeometry->GetLayerCount(); l++)
	{
		//get diffuse texture
		KFbxLayerElementTexture* leTex = pGeometry->GetLayer(l)->GetDiffuseTextures();
		if (leTex)
		{
			if (leTex->GetReferenceMode() == KFbxLayerElement::eINDEX)
				// textures are in an undefined external table
				continue;
			if (leTex->GetDirectArray().GetCount())
				DisplayTextureInfo(leTex, l);
		}

		leTex = pGeometry->GetLayer(l)->GetDiffuseFactorTextures();
		if (leTex)
		{
			if (leTex->GetReferenceMode() == KFbxLayerElement::eINDEX)
				// textures are in an undefined external table
				continue;
			if (leTex->GetDirectArray().GetCount())
				DisplayTextureInfo(leTex, l);
		}

		leTex=pGeometry->GetLayer(l)->GetEmissiveTextures();
		if (leTex)
		{
			if (leTex->GetReferenceMode() == KFbxLayerElement::eINDEX)
				// textures are in an undefined external table
				continue;
			if (leTex->GetDirectArray().GetCount())
				DisplayTextureInfo(leTex, l);
		}
		leTex=pGeometry->GetLayer(l)->GetEmissiveFactorTextures();
		if (leTex)
		{
			if (leTex->GetReferenceMode() == KFbxLayerElement::eINDEX)
				// textures are in an undefined external table
				continue;
			if (leTex->GetDirectArray().GetCount())
				DisplayTextureInfo(leTex, l);
		}

		leTex=pGeometry->GetLayer(l)->GetAmbientTextures();
		if (leTex)
		{
			if (leTex->GetReferenceMode() == KFbxLayerElement::eINDEX)
				// textures are in an undefined external table
				continue;
			if (leTex->GetDirectArray().GetCount())
				DisplayTextureInfo(leTex, l);
		}
		leTex=pGeometry->GetLayer(l)->GetAmbientFactorTextures();
		if (leTex)
		{
			if (leTex->GetReferenceMode() == KFbxLayerElement::eINDEX)
				// textures are in an undefined external table
				continue;
			if (leTex->GetDirectArray().GetCount())
				DisplayTextureInfo(leTex, l);
		}

		leTex=pGeometry->GetLayer(l)->GetBumpTextures();
		if (leTex)
		{
			if (leTex->GetReferenceMode() == KFbxLayerElement::eINDEX)
				// textures are in an undefined external table
				continue;
			if (leTex->GetDirectArray().GetCount())
				DisplayTextureInfo(leTex, l);
		}

		leTex=(pGeometry->GetLayer(l))->GetSpecularTextures();
		if (leTex)
		{
			if (leTex->GetReferenceMode() == KFbxLayerElement::eINDEX)
				// textures are in an undefined external table
				continue;
			if (leTex->GetDirectArray().GetCount())
				DisplayTextureInfo(leTex, l);
		}

		leTex=(pGeometry->GetLayer(l))->GetSpecularFactorTextures();
		if (leTex)
		{
			if (leTex->GetReferenceMode() == KFbxLayerElement::eINDEX)
				// textures are in an undefined external table
				continue;
			if (leTex->GetDirectArray().GetCount())
				DisplayTextureInfo(leTex, l);
		}

		leTex=(pGeometry->GetLayer(l))->GetShininessTextures();
		if (leTex)
		{
			if (leTex->GetReferenceMode() == KFbxLayerElement::eINDEX)
				// textures are in an undefined external table
				continue;
			if (leTex->GetDirectArray().GetCount())
				DisplayTextureInfo(leTex, l);
		}

		leTex=(pGeometry->GetLayer(l))->GetBumpTextures();
		if (leTex)
		{
			if (leTex->GetReferenceMode() == KFbxLayerElement::eINDEX)
				// textures are in an undefined external table
				continue;
			if (leTex->GetDirectArray().GetCount())
				DisplayTextureInfo(leTex, l);
		}

		leTex=(pGeometry->GetLayer(l))->GetTransparentTextures();
		if (leTex)
		{
			if (leTex->GetReferenceMode() == KFbxLayerElement::eINDEX)
				// textures are in an undefined external table
				continue;
			if (leTex->GetDirectArray().GetCount())
				DisplayTextureInfo(leTex, l);
		}

		leTex=(pGeometry->GetLayer(l))->GetTransparencyFactorTextures();
		if (leTex)
		{
			if (leTex->GetReferenceMode() == KFbxLayerElement::eINDEX)
				// textures are in an undefined external table
				continue;
			if (leTex->GetDirectArray().GetCount())
				DisplayTextureInfo(leTex, l);
		}

		leTex=(pGeometry->GetLayer(l))->GetReflectionTextures();
		if (leTex)
		{
			if (leTex->GetReferenceMode() == KFbxLayerElement::eINDEX)
				// textures are in an undefined external table
				continue;
			if (leTex->GetDirectArray().GetCount())
				DisplayTextureInfo(leTex, l);
		}
		leTex=(pGeometry->GetLayer(l))->GetReflectionFactorTextures();
		if (leTex)
		{
			if (leTex->GetReferenceMode() == KFbxLayerElement::eINDEX)
				// textures are in an undefined external table
				continue;
			if (leTex->GetDirectArray().GetCount())
				DisplayTextureInfo(leTex, l);
		}


	}
}

	


