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
#include "DisplayMaterial.h"
#include "DisplayTexture.h"
#include "DisplayLink.h"
#include "DisplayShape.h"

#include <stdio.h>

using namespace FBXSDK_NAMESPACE;

void DisplayControlsPoints(KFbxMesh* pMesh);
void DisplayPolygons(KFbxMesh* pMesh);
void DisplayMaterialMapping(KFbxMesh* pMesh);
void DisplayTextureMapping(KFbxMesh* pMesh);


void DisplayMesh(KFbxNode* pNode)
{
	KFbxMesh* lMesh = (KFbxMesh*) pNode->GetNodeAttribute ();

	DisplayString("Mesh Name: ", pNode->GetName());
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
	kInt i, lControlPointsCount = pMesh->GetControlPointsCount();
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
	kInt i, j, lPolygonCount = pMesh->GetPolygonCount();
	KFbxVector4* lControlPoints = pMesh->GetControlPoints(); 
	KFbxVector4* lNormals = pMesh->GetNormals(); 
	char header[100];

	DisplayString("    Polygons");

	kInt vertexId = 0;
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
						kInt polyGroupId = lePolgrp->GetIndexArray().GetAt(i);
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
		
		kInt lPolygonSize = pMesh->GetPolygonSize(i);

		for (j = 0; j < lPolygonSize; j++)
		{
			kInt lControlPointIndex = pMesh->GetPolygonVertex(i, j);

			Display3DVector("            Coordinates: ", lControlPoints[lControlPointIndex]);
			Display3DVector("            Normals: ", lNormals[lControlPointIndex]);

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
						kInt lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
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

	DisplayString("");
}


void DisplayMaterialMapping(KFbxMesh* pMesh)
{
	kCharPtr lMappingTypes[] = { "None", "By Control Point", "By Polygon Vertex", "By Polygon", "All Same" };

	for (int l = 0; l < pMesh->GetLayerCount(); l++)
	{
		KFbxLayerElementMaterial* leMat = pMesh->GetLayer(l)->GetMaterials();
		if (leMat)
		{
			char header[100];
			sprintf(header, "    Material layer %d: ", l); 
			DisplayString(header);

			DisplayString("           Mapping: ", lMappingTypes[leMat->GetMappingMode()]);
                          
			kInt lMaterialCount = 0;
			KString lString;

			if (leMat->GetReferenceMode() == KFbxLayerElement::eDIRECT ||
				leMat->GetReferenceMode() == KFbxLayerElement::eINDEX_TO_DIRECT)
			{
				lMaterialCount = leMat->GetDirectArray().GetCount();
			}
	
			if (leMat->GetReferenceMode() == KFbxLayerElement::eINDEX ||
				leMat->GetReferenceMode() == KFbxLayerElement::eINDEX_TO_DIRECT)
			{
				kInt i;
				kInt* lMaterialIndices = leMat->GetIndexArray().GetArray();

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


void DisplayTextureMapping(KFbxMesh* pMesh)
{
	kCharPtr lMappingTypes[] = { "None", "By Control Point", "By Polygon Vertex", "By Polygon", "All Same" };
	kCharPtr lBlendModes[]   = { "Translucent", "Add", "Modulate", "Modulate2" };

	for (int l = 0; l < pMesh->GetLayerCount(); l++)
	{
		KFbxLayerElementTexture* leTex = pMesh->GetLayer(l)->GetTextures();
		if (leTex)
		{
			char header[100];
			sprintf(header, "    Texture layer %d: ", l); 
			DisplayString(header);

			DisplayString("           Mapping: ", lMappingTypes[leTex->GetMappingMode()]);
	
			kInt lTextureCount = 0;

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
				kInt i;
				kInt* lTextureIndices = leTex->GetIndexArray().GetArray();
				
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
	}
	DisplayString("");
}


