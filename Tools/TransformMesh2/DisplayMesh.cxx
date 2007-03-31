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
#include "DisplayMaterial.h"
#include "DisplayTexture.h"
#include "DisplayLink.h"
#include "DisplayShape.h"

#include <stdio.h>

#include <fbxfilesdk_nsuse.h>

void DisplayControlsPoints(KFbxMesh* pMesh);
void DisplayPolygons(KFbxMesh* pMesh);
void DisplayMaterialMapping(KFbxMesh* pMesh);
void DisplayTextureMapping(KFbxMesh* pMesh);


void DisplayMesh(KFbxNode* pNode)
{
	KFbxMesh* lMesh = (KFbxMesh*) pNode->GetNodeAttribute ();

	DisplayString("Mesh Name: ", (char *) pNode->GetName());
	DisplayControlsPoints(lMesh);
	DisplayPolygons(lMesh);
	DisplayMaterialMapping(lMesh);
	DisplayMaterial(lMesh);
	DisplayTextureMapping(lMesh);
    DisplayTexture(lMesh);
	DisplayLink(lMesh);
	DisplayShape(lMesh);
}


void DisplayControlsPoints(KFbxMesh* pMesh)
{
	int i, lControlPointsCount = pMesh->GetControlPointsCount();
	KFbxVector4* lControlPoints = pMesh->GetControlPoints();

	DisplayString("    Control Points");

	for (i = 0; i < lControlPointsCount; i++)
	{
		DisplayInt("        Control Point ", i);
		Display3DVector("            Coordinates: ", lControlPoints[i]);

		for (int j = 0; j < pMesh->GetLayerCount(); j++)
		{
			KFbxLayerElementNormal* leNormals = pMesh->GetLayer(j)->GetNormals();
			if (leNormals)
			{
				if (leNormals->GetMappingMode() == KFbxLayerElement::eBY_CONTROL_POINT)
				{
					char header[100];
					sprintf(header, "            Normal Vector (on layer %d): ", j); 
					if (leNormals->GetReferenceMode() == KFbxLayerElement::eDIRECT)
						Display3DVector(header, leNormals->GetDirectArray().GetAt(i));
				}
			}
		}
	}

	DisplayString("");
}


void DisplayPolygons(KFbxMesh* pMesh)
{
	int i, j, lPolygonCount = pMesh->GetPolygonCount();
	KFbxVector4* lControlPoints = pMesh->GetControlPoints(); 
	char header[100];

	DisplayString("    Polygons");

	int vertexId = 0;
	for (i = 0; i < lPolygonCount; i++)
	{
		DisplayInt("        Polygon ", i);
		int l;

		for (l = 0; l < pMesh->GetLayerCount(); l++)
		{
			KFbxLayerElementPolygonGroup* lePolgrp = pMesh->GetLayer(l)->GetPolygonGroups();
			if (lePolgrp)
			{				
				switch (lePolgrp->GetMappingMode())
				{
				case KFbxLayerElement::eBY_POLYGON:
					if (lePolgrp->GetReferenceMode() == KFbxLayerElement::eINDEX)
					{					
						sprintf(header, "        Assigned to group (on layer %d): ", l); 
						int polyGroupId = lePolgrp->GetIndexArray().GetAt(i);
						DisplayInt(header, polyGroupId);
						break;
					}
				default:
					// any other mapping modes don't make sense
					DisplayString("        \"unsupported group assignment\"");
					break;
				}
			}
		}
		
		int lPolygonSize = pMesh->GetPolygonSize(i);

		for (j = 0; j < lPolygonSize; j++)
		{
			int lControlPointIndex = pMesh->GetPolygonVertex(i, j);

			Display3DVector("            Coordinates: ", lControlPoints[lControlPointIndex]);

			for (l = 0; l < pMesh->GetLayerCount(); l++)
			{
				KFbxLayerElementVertexColor* leVtxc = pMesh->GetLayer(l)->GetVertexColors();
				if (leVtxc)
				{
					sprintf(header, "            Color vertex (on layer %d): ", l); 

					switch (leVtxc->GetMappingMode())
					{
					case KFbxLayerElement::eBY_CONTROL_POINT:
						switch (leVtxc->GetReferenceMode())
						{
						case KFbxLayerElement::eDIRECT:
							DisplayColor(header, leVtxc->GetDirectArray().GetAt(lControlPointIndex));
							break;
						case KFbxLayerElement::eINDEX_TO_DIRECT:
							{
								int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
								DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
						break;

					case KFbxLayerElement::eBY_POLYGON_VERTEX:
						{
						switch (leVtxc->GetReferenceMode())
						{
						case KFbxLayerElement::eDIRECT:
							DisplayColor(header, leVtxc->GetDirectArray().GetAt(vertexId));
							break;
						case KFbxLayerElement::eINDEX_TO_DIRECT:
							{
								int id = leVtxc->GetIndexArray().GetAt(vertexId);
								DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
						}
						break;

					case KFbxLayerElement::eBY_POLYGON: // doesn't make much sense for UVs
					case KFbxLayerElement::eALL_SAME:   // doesn't make much sense for UVs
					case KFbxLayerElement::eNONE:       // doesn't make much sense for UVs
						break;
					}
				}

				KFbxLayerElementUV* leUV = pMesh->GetLayer(l)->GetUVs();
				if (leUV)
				{
					sprintf(header, "            Texture UV (on layer %d): ", l); 

					switch (leUV->GetMappingMode())
					{
					case KFbxLayerElement::eBY_CONTROL_POINT:
						switch (leUV->GetReferenceMode())
						{
						case KFbxLayerElement::eDIRECT:
							Display2DVector(header, leUV->GetDirectArray().GetAt(lControlPointIndex));
							break;
						case KFbxLayerElement::eINDEX_TO_DIRECT:
							{
								int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
								Display2DVector(header, leUV->GetDirectArray().GetAt(id));
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
						break;

					case KFbxLayerElement::eBY_POLYGON_VERTEX:
						{
						int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
						switch (leUV->GetReferenceMode())
						{
						case KFbxLayerElement::eDIRECT:
							Display2DVector(header, leUV->GetDirectArray().GetAt(lTextureUVIndex));
							break;
						case KFbxLayerElement::eINDEX_TO_DIRECT:
							{
								int id = leUV->GetIndexArray().GetAt(lTextureUVIndex);
								Display2DVector(header, leUV->GetDirectArray().GetAt(id));
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
						}
						break;

					case KFbxLayerElement::eBY_POLYGON: // doesn't make much sense for UVs
					case KFbxLayerElement::eALL_SAME:   // doesn't make much sense for UVs
					case KFbxLayerElement::eNONE:       // doesn't make much sense for UVs
						break;
					}
				}
			} // for layer
			vertexId++;
		} // for polygonSize




	} // for polygonCount


	//check visibility for the edges of the mesh
	for(int l=0; l<pMesh->GetLayerCount(); ++l)
	{
		KFbxLayerElementVisibility* leVisibility=pMesh->GetLayer(0)->GetVisibility();
		if(leVisibility)
		{
			sprintf(header, "    Edge Visibilty (on layer %d): ", l);
			DisplayString(header);
			switch(leVisibility->GetMappingMode())
			{
			//should be eBY_EDGE
			case KFbxLayerElement::eBY_EDGE:
				//should be eDIRECT
				for(int j=0; j!=pMesh->GetMeshEdgeCount();++j)
				{
					DisplayInt("        Edge ", j);
					DisplayBool("              Edge visibilty: ", leVisibility->GetDirectArray().GetAt(j));
				}
				
				break;
			}

		}
	}
	DisplayString("");
}


void DisplayMaterialMapping(KFbxMesh* pMesh)
{
	char* lMappingTypes[] = { "None", "By Control Point", "By Polygon Vertex", "By Polygon", "By Edge", "All Same" };

	for (int l = 0; l < pMesh->GetLayerCount(); l++)
	{
		KFbxLayerElementMaterial* leMat = pMesh->GetLayer(l)->GetMaterials();
		if (leMat)
		{
			char header[100];
			sprintf(header, "    Material layer %d: ", l); 
			DisplayString(header);

			
			DisplayString("           Mapping: ", lMappingTypes[leMat->GetMappingMode()]);
                          
			int lMaterialCount = 0;
			KString lString;


			// @FIX: ReJ
			// incorect material count
			// 
			// OLD: 
			// if (leMat->GetReferenceMode() == KFbxLayerElement::eDIRECT ||
			// 	leMat->GetReferenceMode() == KFbxLayerElement::eINDEX_TO_DIRECT)
			// {
			// 	lMaterialCount = leMat->GetDirectArray().GetCount();
			// }

			switch (leMat->GetMappingMode())
			{
			case KFbxLayerElement::eBY_POLYGON:
				lMaterialCount = pMesh->GetPolygonCount();
				break;
			case KFbxLayerElement::eALL_SAME:
				lMaterialCount = 1;
				break;
			default:
				break; // other type should not occur.
			}

			if (leMat->GetReferenceMode() == KFbxLayerElement::eINDEX ||
				leMat->GetReferenceMode() == KFbxLayerElement::eINDEX_TO_DIRECT)
			{
				int i;
				int* lMaterialIndices = leMat->GetIndexArray().GetArray();

				lString = "           Indices: ";

				for (i = 0; i < lMaterialCount; i++)
				{
					lString += lMaterialIndices[i];

					if (i < lMaterialCount - 1)
					{
						lString += ", ";
					}
				}
			}

			lString += "\n";
	
			printf(lString);
		}
	}

	DisplayString("");
}
void DisplayTextureMappingInfo(KFbxMesh *pMesh, KFbxLayerElementTexture* leTex, int l)
{
	char* lMappingTypes[] = { "None", "By Control Point", "By Polygon Vertex", "By Polygon", "All Same" };
	char* lBlendModes[]   = { "Translucent", "Add", "Modulate", "Modulate2" };
	char header[100];
	sprintf(header, "    Texture layer %d: ", l); 
	DisplayString(header);

	DisplayString("           Mapping: ", lMappingTypes[leTex->GetMappingMode()]);

	int lTextureCount = 0;

	switch (leTex->GetMappingMode())
	{
	case KFbxLayerElement::eBY_POLYGON:
		lTextureCount = pMesh->GetPolygonCount();
		break;
	case KFbxLayerElement::eALL_SAME:
		lTextureCount = 1;
		break;
	default:
		break; // other type should not occur.
	}

	KString lString;

	if (leTex->GetReferenceMode() == KFbxLayerElement::eINDEX ||
		leTex->GetReferenceMode() == KFbxLayerElement::eINDEX_TO_DIRECT)
	{
		int i;
		int* lTextureIndices = leTex->GetIndexArray().GetArray();
		
		lString = "           Indices: ";

		for( i = 0; i < lTextureCount; i++)
		{
			lString += lTextureIndices[i];

			if (i < lTextureCount - 1)
			{
				lString += ", ";
			}
		}
	}

	lString += "\n";

	printf(lString);
	DisplayString("        Blend Mode: ", lBlendModes[ leTex->GetBlendMode()]);
	DisplayDouble("             Alpha: ", leTex->GetAlpha());
	DisplayString("");
}

void DisplayTextureMapping(KFbxMesh* pMesh)
{
	

	for (int l = 0; l < pMesh->GetLayerCount(); l++)
	{
		KFbxLayerElementTexture* leTex= pMesh->GetLayer(l)->GetTextures(KFbxLayerElementTexture::eDIFFUSE_TEXTURES);
		if(leTex)
			DisplayTextureMappingInfo(pMesh, leTex, l);
		leTex=pMesh->GetLayer(l)->GetTextures(KFbxLayerElementTexture::eDIFFUSE_FACTOR_TEXTURES);
		if(leTex)
			DisplayTextureMappingInfo(pMesh, leTex, l);
		
		leTex=pMesh->GetLayer(l)->GetTextures(KFbxLayerElementTexture::eEMISSIVE_TEXTURES);
		if(leTex)
			DisplayTextureMappingInfo(pMesh, leTex, l);

		leTex=pMesh->GetLayer(l)->GetTextures(KFbxLayerElementTexture::eEMISSIVE_FACTOR_TEXTURES);
		if(leTex)
			DisplayTextureMappingInfo(pMesh, leTex, l);
		
		leTex=pMesh->GetLayer(l)->GetTextures(KFbxLayerElementTexture::eAMBIENT_TEXTURES);
		if(leTex)
			DisplayTextureMappingInfo(pMesh, leTex, l);

		leTex=pMesh->GetLayer(l)->GetTextures(KFbxLayerElementTexture::eAMBIENT_FACTOR_TEXTURES);
		if(leTex)
			DisplayTextureMappingInfo(pMesh, leTex, l);

		leTex=pMesh->GetLayer(l)->GetTextures(KFbxLayerElementTexture::eSPECULAR_TEXTURES);
		if(leTex)
			DisplayTextureMappingInfo(pMesh, leTex, l);

		leTex=pMesh->GetLayer(l)->GetTextures(KFbxLayerElementTexture::eSPECULAR_FACTOR_TEXTURES);
		if(leTex)
			DisplayTextureMappingInfo(pMesh, leTex, l);

		leTex=pMesh->GetLayer(l)->GetTextures(KFbxLayerElementTexture::eSHININESS_TEXTURES);
		if(leTex)
			DisplayTextureMappingInfo(pMesh, leTex, l);

		leTex=pMesh->GetLayer(l)->GetTextures(KFbxLayerElementTexture::eBUMP_TEXTURES);
		if(leTex)
			DisplayTextureMappingInfo(pMesh, leTex, l);

		leTex=pMesh->GetLayer(l)->GetTextures(KFbxLayerElementTexture::eTRANSPARENT_TEXTURES);
		if(leTex)
			DisplayTextureMappingInfo(pMesh, leTex, l);		

		leTex=pMesh->GetLayer(l)->GetTextures(KFbxLayerElementTexture:: eTRANSPARENCY_FACTOR_TEXTURES);
		if(leTex)
			DisplayTextureMappingInfo(pMesh, leTex, l);		

		leTex=pMesh->GetLayer(l)->GetTextures(KFbxLayerElementTexture:: eREFLECTION_TEXTURES);
		if(leTex)
			DisplayTextureMappingInfo(pMesh, leTex, l);

		leTex=pMesh->GetLayer(l)->GetTextures(KFbxLayerElementTexture:: eREFLECTION_FACTOR_TEXTURES);
		if(leTex)
			DisplayTextureMappingInfo(pMesh, leTex, l);


		
		
	}
	DisplayString("");
}


