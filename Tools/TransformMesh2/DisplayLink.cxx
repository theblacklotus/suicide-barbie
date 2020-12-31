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

#include <stdio.h>

#include <fbxfilesdk_nsuse.h>

void DisplayLink(KFbxGeometry* pGeometry)
{
	//Display cluster now

    //int i, lLinkCount;
	//KFbxLink* lLink;
	
	int i, j;
	int lSkinCount=0;
	int lClusterCount=0;
	KFbxCluster* lCluster;

	lSkinCount=pGeometry->GetDeformerCount(KFbxDeformer::eSKIN);
	
		
	
	//lLinkCount = pGeometry->GetLinkCount();
	for(i=0; i!=lSkinCount; ++i)
	{
		lClusterCount = ((KFbxSkin *) pGeometry->GetDeformer(i, KFbxDeformer::eSKIN))->GetClusterCount();
		for (j = 0; j != lClusterCount; ++j)
		{
			DisplayInt("    Cluster ", i);

			lCluster=((KFbxSkin *) pGeometry->GetDeformer(i, KFbxDeformer::eSKIN))->GetCluster(j);
			//lLink = pGeometry->GetLink(i);    
			
			char* lClusterModes[] = { "Normalize", "Additive", "Total1" };

			DisplayString("    Mode: ", lClusterModes[lCluster->GetLinkMode()]);

			if(lCluster->GetLink() != NULL)
			{
				DisplayString("        Name: ", (char *) lCluster->GetLink()->GetName());
			}

			KString lString1 = "        Link Indices: ";
			KString lString2 = "        Weight Values: ";
		
			int k, lIndexCount = lCluster->GetControlPointIndicesCount();
			int* lIndices = lCluster->GetControlPointIndices();
			double* lWeights = lCluster->GetControlPointWeights();
		
			for(k = 0; k < lIndexCount; k++)
			{
				lString1 += lIndices[k];	
				lString2 += (float) lWeights[k];

				if (k < lIndexCount - 1)
				{
					lString1 += ", ";
					lString2 += ", ";
				}
			}
		
			lString1 += "\n";
			lString2 += "\n";
		
			printf(lString1);
			printf(lString2);
			
			DisplayString("");

			KFbxXMatrix lMatrix;

			lMatrix = lCluster->GetTransformMatrix(lMatrix);
			Display3DVector("        Transform Translation: ", lMatrix.GetT());
			Display3DVector("        Transform Rotation: ", lMatrix.GetR());
			Display3DVector("        Transform Scaling: ", lMatrix.GetS());

			lMatrix = lCluster->GetTransformLinkMatrix(lMatrix);
			Display3DVector("        Transform Link Translation: ", lMatrix.GetT());
			Display3DVector("        Transform Link Rotation: ", lMatrix.GetR());
			Display3DVector("        Transform Link Scaling: ", lMatrix.GetS());

			if (lCluster->GetAssociateModel() != NULL)
			{
				lMatrix = lCluster->GetTransformAssociateModelMatrix(lMatrix);
				DisplayString("        Associate Model: ", (char *) lCluster->GetAssociateModel()->GetName());
				Display3DVector("        Associate Model Translation: ", lMatrix.GetT());
				Display3DVector("        Associate Model Rotation: ", lMatrix.GetR());
				Display3DVector("        Associate Model Scaling: ", lMatrix.GetS());
			}

			DisplayString("");
		}
	}
}


