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
#include "DisplayUserProperties.h"

#include <fbxfilesdk_nsuse.h>

void DisplayMaterial(KFbxGeometry* pGeometry)
{
	for (int l = 0; l < pGeometry->GetLayerCount(); l++)
	{
		KFbxLayerElementMaterial* leMat = pGeometry->GetLayer(l)->GetMaterials();
		if (leMat)
		{
			if (leMat->GetReferenceMode() == KFbxLayerElement::eINDEX)
				// Materials are in an undefined external table
				continue;

			if (leMat->GetDirectArray().GetCount())
			{
				char header[100];
				KFbxPropertyDouble3 lKFbxDouble3;
				KFbxPropertyDouble1 lKFbxDouble1;
				KFbxColor theColor;

				sprintf(header, "    Materials on layer %d: ", l); 
				DisplayString(header);

				for (int lCount = 0; lCount < leMat->GetDirectArray().GetCount(); lCount ++)
				{
					DisplayInt("        Material ", lCount);

					KFbxSurfaceMaterial *lMaterial = leMat->GetDirectArray().GetAt(lCount);

					DisplayString("            Name: \"", (char *) lMaterial->GetName(), "\""); 
					if(lMaterial->GetNewFbxClassId().Is(KFbxSurfaceLambert::ClassId) )
					{
						// We found a Lambert material. Display its properties.
						// Display the Ambient Color
						lKFbxDouble3=((KFbxSurfaceLambert *)lMaterial)->GetAmbientColor();
						theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
						DisplayColor("            Ambient: ", theColor);

						// Display the Diffuse Color
						lKFbxDouble3 =((KFbxSurfaceLambert *)lMaterial)->GetDiffuseColor();
						theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
						DisplayColor("            Diffuse: ", theColor);

						// Display the Emissive
						lKFbxDouble3 =((KFbxSurfaceLambert *)lMaterial)->GetDiffuseColor();
						theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
						DisplayColor("            Diffuse: ", theColor);

						// Display the Opacity
						lKFbxDouble1 =((KFbxSurfaceLambert *)lMaterial)->GetTransparencyFactor();
						DisplayDouble("            Opacity: ", 1.0-lKFbxDouble1.Get());
					}
					else if (lMaterial->GetNewFbxClassId().Is(KFbxSurfacePhong::ClassId))
					{
						// We found a Phong material.  Display its properties.

						// Display the Ambient Color
						lKFbxDouble3 =((KFbxSurfacePhong *) lMaterial)->GetAmbientColor();
						theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
						DisplayColor("            Ambient: ", theColor);

						// Display the Diffuse Color
						lKFbxDouble3 =((KFbxSurfacePhong *) lMaterial)->GetDiffuseColor();
						theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
						DisplayColor("            Diffuse: ", theColor);

						// Display the Specular Color (unique to Phong materials)
						lKFbxDouble3 =((KFbxSurfacePhong *) lMaterial)->GetSpecularColor();
						theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
						DisplayColor("            Specular: ", theColor);

						// Display the Emissive Color
						lKFbxDouble3 =((KFbxSurfacePhong *) lMaterial)->GetEmissiveColor();
						theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
						DisplayColor("            Emissive: ", theColor);

						//Opacity is Transparency factor now
						lKFbxDouble1 =((KFbxSurfacePhong *) lMaterial)->GetTransparencyFactor();
						DisplayDouble("            Opacity: ", 1.0-lKFbxDouble1.Get());

						// Display the Shininess
						lKFbxDouble1 =((KFbxSurfacePhong *) lMaterial)->GetShininess();
						DisplayDouble("            Shininess: ", lKFbxDouble1.Get());

						// Display the Reflectivity
						lKFbxDouble1 =((KFbxSurfacePhong *) lMaterial)->GetReflectionColor();
						DisplayDouble("            Reflectivity: ", lKFbxDouble1.Get());	
					}
					else
						DisplayString("Unkown type of Material");

					KFbxPropertyString lString;
					lString = lMaterial->GetShadingModel();
					DisplayString("            Shading Model: ", lString.Get().Buffer());
					DisplayString("");
				}
			}
		}
	}
}