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

/////////////////////////////////////////////////////////////////////////
//
// This example illustrates how to detect if a scene is password 
// protected, import and browse the scene to access node and animation 
// information. It displays the content of the FBX file which name is 
// passed as program argument. You can try it with the various FBX files 
// output by the export examples.
//
/////////////////////////////////////////////////////////////////////////

#include <fbxsdk.h>

#include "Common/Common.h"
#include "DisplayCommon.h"
#include "DisplayHierarchy.h"
#include "DisplayAnimation.h"
#include "DisplayMarker.h"
#include "DisplaySkeleton.h"
#include "DisplayMesh.h"
#include "DisplayNurb.h"
#include "DisplayPatch.h"
#include "DisplayCamera.h"
#include "DisplayLight.h"
#include "DisplayGlobalSettings.h"
#include "DisplayPose.h"
#include "DisplayPivotsAndLimits.h"
#include "DisplayUserProperties.h"


#ifdef __cplusplus
extern "C" {
#endif
	#include <lwo2.h>
#ifdef __cplusplus
}
#endif 

// mutant stuff ======================================================================================
#pragma warning( disable:4512 )
//#pragma warning( disable:4239 )


float const WEIGHT_EPSILON = 1e-4f;
double const ANIM_SAMPLING_FREQ = 1.0f / 30.0;

#include <list>
#include <vector>
#include <limits>
#include <memory>

/*
#include <mutant/clip.h>
#include <mutant/hierarchy.h>
#include <mutant/character.h>

#include <mutant/io_factory.h>
#include <mutant/writer.h>
#include <mutant/reader.h>
//#include <mutant/simple_scene.h>
//#include <mutant/simple_skinned.h>
#include <mutant/type_names.h>
#include <mutant/compose_algo.h>*/

#include <mutant/mutant.h>
#include <mutant/writer.h>
#include <mutant/io_factory.h>

#include <mutalisk/mutalisk.h>
#include <mutalisk/dx9/dx9.h>
#include <mutalisk/psp/psp.h>
#include <mutalisk/psp/pspXcompile.h>

#include <d3d9.h>
#include <d3dx9.h>


#define THROW_ERROR(msg) throw std::runtime_error(msg)

/*
struct OutputSkinnedMesh : public BaseSkinnedMesh
{
	std::string name;

	// memory management
	OutputSkinnedMesh() { positions = 0; normals = 0; weights = 0; boneIndices = 0; indices = 0; bones = 0; }
	~OutputSkinnedMesh() { freeDataArrays(); }

	void allocDataArrays()
	{
		freeDataArrays();
		this->positions = new Vec3[this->vertexCount];
		this->normals = new Vec3[this->vertexCount];
		this->weights = new float[this->vertexCount * this->weightsPerVertex];
		this->boneIndices = new unsigned short[this->vertexCount * this->weightsPerVertex];
		this->indices = new unsigned short[this->indexCount];
		this->bones = new Bone[this->boneCount];
	}

	void freeDataArrays()
	{
		delete[] this->positions;
		delete[] this->normals;
		delete[] this->weights;
		delete[] this->boneIndices;
		delete[] this->indices;
		delete[] this->bones;
	}
};*/
//mutant::anim_character_set gAnimCharacterSet;
//typedef std::list<OutputSkinnedMesh>	OutputMeshesT;
//OutputMeshesT gOutputMeshes;

struct BaseSkinnedMesh
{
	// definition
	struct Vec3 {
		float const& operator[](size_t i) const { return data[i]; }
		float& operator[](size_t i) { return data[i]; }
		float data[3];
	};
	struct Mat16 {
		float const& operator[](size_t i) const { return data[i]; }
		float& operator[](size_t i) { return data[i]; }
		float data[16];
	};
	struct Bone
	{
		Mat16 matrix;
		std::string	name;
	};
	struct Vertex
	{
		typedef unsigned short BoneIndexT;

		Vec3 pos;
		Vec3 normal;

		std::vector<
			std::pair<BoneIndexT, float> > weights;
	};
	typedef unsigned short IndexT;

	// data
	std::vector<Vertex> vertices;
	std::vector<IndexT> indices;
	std::vector<Bone> bones;
};

struct OutputSkinnedMesh : public BaseSkinnedMesh
{
	std::string name;

	// memory management
	~OutputSkinnedMesh() { clear(); }
	void clear()
	{
		vertices.resize(0);
		indices.resize(0);
		bones.resize(0);
	}
};

struct OutputScene
{
	// definition
	struct Actor
	{
		OutputSkinnedMesh*	mesh;
		KFbxNode*			node;
	};

	typedef std::map<std::string, OutputSkinnedMesh>	OutputMeshesT;
	typedef std::vector<KFbxCamera*>					CamerasT;
	typedef std::vector<KFbxLight*>						LightsT;
	typedef std::vector<Actor>							ActorsT;

	// data
	std::string				name;

	mutant::anim_character_set
							animResource;
	OutputMeshesT			meshResources;
	CamerasT				cameras;
	LightsT					lights;
	ActorsT					actors;

};
OutputScene gOutputScene;

struct Curve
{
	typedef mutant::knot_data<float,float> DataT;

	Curve(DataT::key_vector_t const& keys_, DataT::value_vector_t values_, size_t component)
		: keys(keys_), values(values_), data(keys, values, component) {}
	Curve(Curve const& c) : keys(c.keys), values(c.values), data(keys, values, c.data.componentSize()) {}
	Curve() : data(keys, values, 0) {}

	size_t size() const { return data.size(); }

	DataT::key_vector_t keys;
	DataT::value_vector_t values;
	DataT data;
private:
	Curve& operator= (Curve const& rhs);
};
typedef Curve CurveT;
typedef std::list<CurveT>	CurvesT;
CurvesT gCurves;

unsigned short processIndex(kInt srcIndex);
void processMatrix(float* dstMatrix, KFbxXMatrix const& srcMatrix);
void processMesh(KFbxNode* pNode);
OutputSkinnedMesh& processMeshResource(KFbxNode* pNode);
OutputSkinnedMesh& processMeshResource(lwLayer* subset, std::string resourceName);
void processCamera(KFbxCamera* pCamera);
void processLight(KFbxLight* pLight);

int processHierarchy(mutant::anim_hierarchy& hierarchy, KFbxNode* pNode, int pDepth);
std::auto_ptr<mutant::anim_hierarchy> processHierarchy(KFbxScene* pScene);
void processAnimation(mutant::anim_clip& clip, KFbxNode* pNode);
void processAnimation(mutant::anim_character& anim_char, KFbxScene* pScene);
std::auto_ptr<mutant::anim_bundle> processChannels(KFbxNode* pNode, KFbxTakeNode* pTakeNode, KFbxTakeNode* pDefaultTakeNode);
CurveT processCurve(KFCurve *pCurve);
float processDefaultCurve(KFCurve *pCurve);


struct Platform
{
	static const Platform DX9;
	static const Platform PSP;

	Platform(std::string name_, std::string targetDir_) : name(name_), targetDir(targetDir_) {}
	std::string name;
	std::string targetDir;

	bool operator==(Platform const& p) const { return name == p.name; }
};
Platform const Platform::DX9 = Platform("dx9", "dx9\\");
Platform const Platform::PSP = Platform("psp", "psp\\");


bool gLWMode = false;
Platform gPlatform = Platform::DX9;

KFbxSdkManager* gFbxSdkManager = 0;
KFbxGeometryConverter* gFbxGeomConverter = 0;



std::string cutEnd(std::string const& nodeName)
{
	std::string::const_reverse_iterator i = std::find(nodeName.rbegin(), nodeName.rend(), ')');
	if(i == nodeName.rend())
		return nodeName;

	i = std::find(i, nodeName.rend(), '(');
	if(i == nodeName.rend())
		return nodeName;

	do ++i; while(i != nodeName.rend() && *i == ' ');

	return std::string(nodeName.begin(), i.base());
}

std::string cutFront(std::string const& nodeName)
{
	std::string::const_reverse_iterator i = nodeName.rbegin();
	while(i != nodeName.rend() && *i != ':') ++i;
//	--j;
	return std::string(i.base(), nodeName.end());
}

std::string getBaseName(std::string const& nodeName)
{
	return cutFront(cutEnd(nodeName));
}

std::string getBaseName(char const* nodeName)
{
	return getBaseName(std::string(nodeName));
}

std::string fileName2SceneName(std::string sceneName)
{
	return sceneName.substr(0, sceneName.find_last_of("."));
}

std::string nodeName2FileName(std::string nodeName)
{
	std::replace(nodeName.begin(), nodeName.end(), ':', '_');
	std::replace(nodeName.begin(), nodeName.end(), '.', '_');
	std::replace(nodeName.begin(), nodeName.end(), ' ', '_');
	return nodeName + ".msh";
}

std::string sceneName2FileName(std::string sceneName)
{
	return sceneName + ".msk";
}

std::string sceneName2AnimFileName(std::string sceneName)
{
	return sceneName + ".man";
}

template <typename Data>
void save(std::string dstName, Data& data)
{
	mutant::mutant_writer mutWriter(mutant::writer_factory::createOutput(gPlatform.targetDir + dstName));
	mutWriter << data;
}

template <>
void save(std::string dstName, mutant::anim_character_set& data)
{
	mutant::mutant_writer mutWriter(mutant::writer_factory::createOutput(gPlatform.targetDir + dstName));
	mutWriter.write(data);
}


void blitBaseMesh(OutputSkinnedMesh const& mesh, mutalisk::data::base_mesh& data)
{
	data.vertexCount = mesh.vertices.size();
	data.vertexDataSize = data.vertexCount * data.vertexStride;
	data.vertexData = new byte[data.vertexDataSize];
	for(size_t q = 0; q < data.vertexCount; ++q)
	{
		float* asFloat = reinterpret_cast<float*>(data.vertexData + q * data.vertexStride);

		if(gLWMode)
		{
			asFloat[0] = mesh.vertices[q].pos[2]* 100.0f;
			asFloat[1] = mesh.vertices[q].pos[0]* 100.0f;
			asFloat[2] = mesh.vertices[q].pos[1]* 100.0f;
			asFloat[3] = mesh.vertices[q].normal[0]* 1.0f;
			asFloat[4] = mesh.vertices[q].normal[1]* 1.0f;
			asFloat[5] = mesh.vertices[q].normal[2]* 1.0f;
		}
		else
		{
			asFloat[0] = mesh.vertices[q].pos[0];
			asFloat[1] = mesh.vertices[q].pos[1];
			asFloat[2] = mesh.vertices[q].pos[2];
			asFloat[3] = mesh.vertices[q].normal[0];
			asFloat[4] = mesh.vertices[q].normal[1];
			asFloat[5] = mesh.vertices[q].normal[2];
		}
	}
}

void blitMesh(OutputSkinnedMesh const& mesh, mutalisk::data::dx9_mesh& data)
{
	data.fvfVertexDecl = D3DFVF_XYZ | D3DFVF_NORMAL;
	data.vertexStride = D3DXGetFVFVertexSize(data.fvfVertexDecl);
	data.skinInfo = 0;

	blitBaseMesh(mesh, data);
}

void blitMesh(OutputSkinnedMesh const& mesh, mutalisk::data::psp_mesh& data)
{
	data.vertexDecl = GU_VERTEX_32BITF | GU_NORMAL_32BITF;
	data.vertexStride = sizeof(float) * 6;
	data.skinInfo = 0;

//	blitBaseMesh(mesh, data);
	data.vertexCount = mesh.vertices.size();
	data.vertexDataSize = data.vertexCount * data.vertexStride;
	data.vertexData = new byte[data.vertexDataSize];
	for(size_t q = 0; q < data.vertexCount; ++q)
	{
		float* asFloat = reinterpret_cast<float*>(data.vertexData + q * data.vertexStride);

		if(gLWMode)
		{
			asFloat[0] = mesh.vertices[q].normal[0]* 1.0f;
			asFloat[1] = mesh.vertices[q].normal[1]* 1.0f;
			asFloat[2] = mesh.vertices[q].normal[2]* 1.0f;
			asFloat[3] = mesh.vertices[q].pos[2]* 100.0f;
			asFloat[4] = mesh.vertices[q].pos[0]* 100.0f;
			asFloat[5] = mesh.vertices[q].pos[1]* 100.0f;
		}
		else
		{
			asFloat[0] = mesh.vertices[q].normal[0];
			asFloat[1] = mesh.vertices[q].normal[1];
			asFloat[2] = mesh.vertices[q].normal[2];
			asFloat[3] = mesh.vertices[q].pos[0];
			asFloat[4] = mesh.vertices[q].pos[1];
			asFloat[5] = mesh.vertices[q].pos[2];
		}
	}
}

size_t getWeightsPerVertex(OutputSkinnedMesh const& mesh)
{
	size_t weightsPerVertex = 0;
	for(size_t q = 0; q < mesh.vertices.size(); ++q)
	{
		weightsPerVertex = max(
			mesh.vertices[q].weights.size(),
			weightsPerVertex);
	}
	return weightsPerVertex;
}

std::pair<BaseSkinnedMesh::Vertex::BoneIndexT, float> getBoneWeight(OutputSkinnedMesh const& mesh, size_t vertexId, size_t weightId)
{
	assert(vertexId < mesh.vertices.size());
	if(weightId < mesh.vertices[vertexId].weights.size())
		return mesh.vertices[vertexId].weights[weightId];

	return std::make_pair(0, 0.0f);
}

void blitSkinned(OutputSkinnedMesh const& mesh, mutalisk::data::dx9_mesh& data)
{
	data.vertexCount = mesh.vertices.size();
	data.fvfVertexDecl = D3DFVF_XYZB5 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL;
	// struct VERTEXPOSITION
	// {
	//    float pos[3];
	//    union 
	//    {
	// 		float beta[5];
	// 		struct
	// 		{
	// 		  float weights[4];
	// 		  DWORD MatrixIndices;  // Used as UBYTEs
	// 		}
	//    }
	// };
	
	data.vertexStride = D3DXGetFVFVertexSize(data.fvfVertexDecl);
	data.vertexDataSize = data.vertexCount * data.vertexStride;
	data.vertexData = new byte[data.vertexDataSize];

	data.skinInfo = new mutalisk::data::skin_info;
	data.skinInfo->weightsPerVertex = getWeightsPerVertex(mesh);
	data.skinInfo->boneCount = mesh.bones.size();
	data.skinInfo->bones = new mutalisk::data::skin_info::Bone[data.skinInfo->boneCount];
	for(size_t q = 0; q < data.skinInfo->boneCount; ++q)
	{
		assert(sizeof(data.skinInfo->bones[q].matrix) == sizeof(mesh.bones[q].matrix));
		memcpy(&data.skinInfo->bones[q].matrix, &mesh.bones[q].matrix, sizeof(mesh.bones[q].matrix));
		data.skinInfo->bones[q].name = mesh.bones[q].name;
	}

	for(size_t q = 0; q < data.vertexCount; ++q)
	{
		float* asFloat = reinterpret_cast<float*>(data.vertexData + q * data.vertexStride);
		DWORD* asDword = reinterpret_cast<DWORD*>(data.vertexData + q * data.vertexStride);

		if(gLWMode)
		{
			asFloat[0] = mesh.vertices[q].pos[2]* 100.0f;
			asFloat[1] = mesh.vertices[q].pos[0]* 100.0f;
			asFloat[2] = mesh.vertices[q].pos[1]* 100.0f;
			asFloat[3] = mesh.vertices[q].normal[0]* 1.0f;
			asFloat[4] = mesh.vertices[q].normal[1]* 1.0f;
			asFloat[5] = mesh.vertices[q].normal[2]* 1.0f;
		}
		else
		{
			asFloat[0] = mesh.vertices[q].pos[0];
			asFloat[1] = mesh.vertices[q].pos[1];
			asFloat[2] = mesh.vertices[q].pos[2];
			asFloat[3] = mesh.vertices[q].normal[0];
			asFloat[4] = mesh.vertices[q].normal[1];
			asFloat[5] = mesh.vertices[q].normal[2];
		}

		size_t weightsPerVertex = data.skinInfo->weightsPerVertex;

		size_t w = 0;
		for(; w < weightsPerVertex; ++w)
		{
			float weight = getBoneWeight(mesh, q, w).second;				// weight
			asFloat[3+w] = weight;
		}
		for(; w < 4; ++w)
			asFloat[3+w] = 0.0f;

		asDword[7] = 0;
		for(size_t w = 0; w < weightsPerVertex; ++w)
		{
			unsigned short boneIndex = getBoneWeight(mesh, q, w).first;		// boneIndex
			assert(boneIndex < 256);
			asDword[7] |= ((0xff & boneIndex) << (w * 8));
		}
	}
}

void blitSkinned(OutputSkinnedMesh const& mesh, mutalisk::data::psp_mesh& data)
{
	size_t weightsPerVertex = getWeightsPerVertex(mesh);

	data.vertexCount = mesh.vertices.size();
	data.vertexDecl = GU_VERTEX_32BITF|GU_NORMAL_32BITF|GU_WEIGHT_32BITF|GU_WEIGHTS(weightsPerVertex);
	// struct VERTEXPOSITION
	// {
	//	float skinWeight[WEIGHTS_PER_VERTEX];
	//  ... other attributes goes here
	//	float x,y,z;
	// };
	
	data.vertexStride = sizeof(float) * 6 + sizeof(float) * weightsPerVertex;
	data.vertexDataSize = data.vertexCount * data.vertexStride;
	data.vertexData = new byte[data.vertexDataSize];

	data.boneIndexStride = sizeof(byte) * weightsPerVertex;
	data.boneIndexDataSize = data.vertexCount * data.boneIndexStride;
	data.boneIndexData = new byte[data.boneIndexDataSize];

	data.skinInfo = new mutalisk::data::skin_info;
	data.skinInfo->weightsPerVertex = weightsPerVertex;
	data.skinInfo->boneCount = mesh.bones.size();
	data.skinInfo->bones = new mutalisk::data::skin_info::Bone[data.skinInfo->boneCount];
	for(size_t q = 0; q < data.skinInfo->boneCount; ++q)
	{
		assert(sizeof(data.skinInfo->bones[q].matrix) == sizeof(mesh.bones[q].matrix));
		memcpy(&data.skinInfo->bones[q].matrix, &mesh.bones[q].matrix, sizeof(mesh.bones[q].matrix));
		data.skinInfo->bones[q].name = mesh.bones[q].name;
	}

	for(size_t q = 0; q < data.vertexCount; ++q)
	{
		float* asFloat = reinterpret_cast<float*>(data.vertexData + q * data.vertexStride);
		byte* asByte = reinterpret_cast<byte*>(data.boneIndexData + q * data.boneIndexStride);

		size_t w = 0;
		for(; w < weightsPerVertex; ++w)
		{
			float weight = getBoneWeight(mesh, q, w).second; // weight
			unsigned short boneIndex = getBoneWeight(mesh, q, w).first;	// boneIndex	

			asFloat[w] = weight;
			assert(boneIndex < 256);
			asByte[w] = static_cast<byte>(boneIndex);				
		}

		if(gLWMode)
		{
			asFloat[w+0] = mesh.vertices[q].normal[0]* 1.0f;
			asFloat[w+1] = mesh.vertices[q].normal[1]* 1.0f;
			asFloat[w+2] = mesh.vertices[q].normal[2]* 1.0f;
			asFloat[w+3] = mesh.vertices[q].pos[0]* 100.0f;
			asFloat[w+4] = mesh.vertices[q].pos[1]* 100.0f;
			asFloat[w+5] = mesh.vertices[q].pos[2]* 100.0f;
		}
		else
		{
			asFloat[w+0] = mesh.vertices[q].normal[0];
			asFloat[w+1] = mesh.vertices[q].normal[1];
			asFloat[w+2] = mesh.vertices[q].normal[2];
			asFloat[w+3] = mesh.vertices[q].pos[0];
			asFloat[w+4] = mesh.vertices[q].pos[1];
			asFloat[w+5] = mesh.vertices[q].pos[2];
		}
	}
}


void blit(OutputSkinnedMesh const& mesh, mutalisk::data::dx9_mesh& data)
{
	if(getWeightsPerVertex(mesh) > 0)
		blitSkinned(mesh, data);
	else
		blitMesh(mesh, data);

	// $TBD: 32 bit indices
	data.indexCount = mesh.indices.size();
	data.indexSize = sizeof(OutputSkinnedMesh::IndexT);
	data.indexData = new byte[data.indexCount * data.indexSize];
	for(size_t q = 0; q < data.indexCount; ++q)
	{
		reinterpret_cast<OutputSkinnedMesh::IndexT*>(data.indexData)[q] = mesh.indices[q];
	}

	data.primitiveType = D3DPT_TRIANGLELIST;
}

void blit(OutputSkinnedMesh const& mesh, mutalisk::data::psp_mesh& data)
{
//	if(getWeightsPerVertex(mesh) > 0)
//		blitSkinned(mesh, data);
//	else
		blitMesh(mesh, data);

	data.indexCount = mesh.indices.size();
	data.indexSize = sizeof(OutputSkinnedMesh::IndexT);
	data.indexData = new byte[data.indexCount * data.indexSize];
	for(size_t q = 0; q < data.indexCount; ++q)
	{
		reinterpret_cast<OutputSkinnedMesh::IndexT*>(data.indexData)[q] = mesh.indices[q];
	}

	data.primitiveType = GU_TRIANGLES;
}

void blit(OutputScene const& scene, mutalisk::data::scene& data)
{
/*	data.meshIds = "_";
	data.animIds = "_";
	data.nodeNames = "_";*/

	data.meshCount = scene.meshResources.size();
	data.meshIds = new mutalisk::data::scene::Ref[data.meshCount];
	size_t q = 0;
	for(OutputScene::OutputMeshesT::const_iterator i = scene.meshResources.begin(); i != scene.meshResources.end(); ++i, ++q)
	{
/*		data.meshes[q].data = (void*)&(data.meshIds[data.meshIds.size()-1]+1);
		data.meshIds += '0x00';
		data.meshIds += nodeName2FileName(i->first);*/
		data.meshIds[q] = nodeName2FileName(i->first);
	}

	data.lightCount = 0;//scene.lights.size();

	data.cameraCount = scene.cameras.size();
	data.cameras = new mutalisk::data::scene::Camera[data.cameraCount];
	data.defaultCameraIndex = 0;
	q = 0;
	for(OutputScene::CamerasT::const_iterator i = scene.cameras.begin(); i != scene.cameras.end(); ++i, ++q)
	{
		assert(scene.cameras[q]);
		assert(scene.cameras[q]->GetNode());
		assert(scene.cameras[q]->GetAttributeType() == KFbxNodeAttribute::eCAMERA ||
			scene.cameras[q]->GetAttributeType() == KFbxNodeAttribute::eCAMERA_SWITCHER);
		char const* name = scene.cameras[q]->GetNode()->GetName();
		data.cameras[q].nodeName = name;
		processMatrix(data.cameras[q].worldMatrix.data, scene.cameras[q]->GetNode()->GetGlobalFromDefaultTake());
	}

	data.actorCount = scene.actors.size();
	data.actors = new mutalisk::data::scene::Actor[data.actorCount];
	for(size_t q = 0; q < data.actorCount; ++q)
	{
		assert(scene.actors[q].node);
		assert(scene.actors[q].mesh);

/*		data.actors[q].nodeName.data = (void*)&(data.nodeNames[data.nodeNames.size()-1]+1);
		data.nodeNames += '0x00';
		data.nodeNames += scene.actors[q].node->GetName();
*/
		data.actors[q].nodeName = scene.actors[q].node->GetName();

		processMatrix(data.actors[q].worldMatrix.data, scene.actors[q].node->GetGlobalFromDefaultTake());
//			GetGlobalMatrix(KFbxNode::eSOURCE_SET, true));
		size_t meshIndex = std::distance(scene.meshResources.begin(), scene.meshResources.find(scene.actors[q].mesh->name));
		assert(meshIndex < scene.meshResources.size());
		data.actors[q].meshIndex = meshIndex;
	}

/*	scene.animChar.data = (void*)&(data.animIds[data.animIds.size()-1]+1);
	data.animIds += '0x00';
	data.animIds += sceneName2AnimFileName(scene.name);
*/

/*	mutant::anim_character const& animCharacter = scene.animResource["scene"];
	data.clipCount = animCharacter.size();
	data.clips = new mutant::simple_scene::Ref[data.clipCount];
	for(size_t q = 0; q < data.clipCount; ++q)
	{
		data.clips[q].data = (void*)&(data.animIds[data.animIds.size()-1]+1);
		data.animIds += '0x00';
		data.animIds += animCharacter.clipPair(q).first;
	}*/

	data.animCharId = sceneName2AnimFileName(scene.name);
	data.defaultClipIndex = 0;
}


unsigned short processIndex(kInt srcIndex)
{
	unsigned long dw = static_cast<unsigned long>(srcIndex);
	if( dw & 0xffffff00000000 )
		THROW_ERROR( "couldn't convert 32bit index to 16bit index" );
	return static_cast<unsigned short>(dw);
}

void processMatrix(float* dstMatrix, KFbxXMatrix const& srcMatrix_)
{
//	KFbxXMatrix srcMatrix = const_cast<KFbxXMatrix&>(srcMatrix_).Inverse();
	KFbxXMatrix& srcMatrix = const_cast<KFbxXMatrix&>(srcMatrix_);
	for(int q = 0; q < 4; ++q)
		for(int w = 0; w < 4; ++w)
			dstMatrix[q*4+w] = static_cast<float>(srcMatrix.Get(q, w));
}

void processMesh(KFbxNode* pNode)
{
	std::string nodeName = pNode->GetName();

	if(gLWMode)
	{
		std::string resourceName = getBaseName(nodeName) + ".lwo";

		unsigned int failID = 0;
		int failpos = 0;
		lwObject* lwo = lwGetObject(const_cast<char*>(resourceName.c_str()), &failID, &failpos);
		lwLayer* layer = lwo->layer;
		for(; layer; layer = layer->next)
		{
			std::string layerName = (layer->name)?std::string("_") + std::string(layer->name): "";

			OutputScene::Actor newActor;
			newActor.mesh = &processMeshResource(layer, getBaseName(nodeName) + layerName);
			newActor.node = pNode;
	
			gOutputScene.actors.push_back(newActor);
		}
	}
	else
	{
		OutputScene::Actor newActor;
		newActor.mesh = &processMeshResource(pNode);
		newActor.node = pNode;

		gOutputScene.actors.push_back(newActor);
	}
}
	
OutputSkinnedMesh& processMeshResource(KFbxNode* pNode)
{
	std::string nodeName = pNode->GetName();
	std::string resourceName = getBaseName(nodeName);

	if(gOutputScene.meshResources.find(resourceName) != gOutputScene.meshResources.end())
		return gOutputScene.meshResources[resourceName];

	OutputSkinnedMesh& result = gOutputScene.meshResources[resourceName];

	// fill result
	KFbxMesh* pMesh = (KFbxMesh*) pNode->GetNodeAttribute();
	assert(gFbxGeomConverter);
	// mesh triangulized => can safely assume that all polys == triangles
	KFbxMesh* pMeshTriangulated = gFbxGeomConverter->TriangulateMesh(pMesh);

	kInt lControlPointsCount = pMesh->GetControlPointsCount();
	kInt lPolygonCount = pMeshTriangulated->GetPolygonCount();
	kInt lLinkCount = pMesh->GetLinkCount();

	result.clear();
	result.name = resourceName;
	result.vertices.resize(lControlPointsCount);
	result.indices.resize(lPolygonCount*3);
	result.bones.resize(lLinkCount);

	// [FBX gather] vertices & normals
	{
		// $TBD: extract vertex colors from layer
		// $TBD: extract uv from layer
		KFbxVector4* lControlPoints = pMesh->GetControlPoints();
		KFbxVector4* lNormals = pMesh->GetNormals();
		for (kInt i = 0; i < lControlPointsCount; i++)
		{
			for(int w = 0; w < 3; ++w)
			{
				assert(i < (kInt)result.vertices.size());
				result.vertices[i].pos[w] = static_cast<float>(lControlPoints[i][w]);
				result.vertices[i].normal[w] = static_cast<float>(lNormals[i][w]);
			}
		}
	}

	// [FBX gather] bone weights
	{
		for(unsigned short q = 0; q < lLinkCount; ++q)
		{
			KFbxLink* lLink = pMesh->GetLink(q);
			assert(lLink);
			assert(lLink->GetLink());

			kInt lIndexCount = lLink->GetControlPointIndicesCount();
			kInt* lIndices = lLink->GetControlPointIndices();
			kDouble* lWeights = lLink->GetControlPointWeights();

			for(int w = 0; w < lIndexCount; w++)
			{
				int vertexId = lIndices[w];
				unsigned short boneId = q;
				float boneWeight = static_cast<float>(lWeights[w]);
				if(boneWeight <= WEIGHT_EPSILON )
					continue;

				assert(vertexId < (int)result.vertices.size());
				result.vertices[vertexId].weights.push_back(std::make_pair(boneId, boneWeight));
			}
		}
	}

	// [FBX gather] indices
	{
		kInt const POLY_SIZE = 3;
		// $NOTE: assume only 1 subset
		// $TBD: support multiple subsets
		for (kInt i = 0; i < lPolygonCount; i++)
		{
			for (kInt j = 0; j < POLY_SIZE; ++j)
			{
				assert(i*POLY_SIZE + j < (kInt)result.indices.size());
				result.indices[i*POLY_SIZE + j] = 0;
			}

			kInt lPolygonSize = pMeshTriangulated->GetPolygonSize(i);
			assert(lPolygonSize <= POLY_SIZE);
			for (kInt j = 0; j < min(POLY_SIZE, lPolygonSize); j++)
			{
				kInt lControlPointIndex = pMeshTriangulated->GetPolygonVertex(i, j);
				assert(i*POLY_SIZE + j < (int)result.indices.size());
				result.indices[i*POLY_SIZE + j] = processIndex(lControlPointIndex);
			}
		}
	}

	// [FBX gather] bones
	{
		for (kInt i = 0; i < lLinkCount; i++)
		{
			KFbxXMatrix m;
			KFbxLink* lLink = pMesh->GetLink(i);
			assert(lLink);
			assert(lLink->GetLink());

			char* boneName = lLink->GetLink()->GetName();
			assert(i < (kInt)result.bones.size());
			result.bones[i].name = boneName;

			processMatrix(
				result.bones[i].matrix.data,
				lLink->GetTransformLinkMatrix(m));

//			D3DXMATRIX inverseBoneMatrix(result.bones[i].matrix.data);
//			D3DXMatrixInverse(&inverseBoneMatrix, 0, &inverseBoneMatrix);
//			memcpy(result.bones[i].matrix.data, &inverseBoneMatrix, sizeof(float)*16);
		}
	}

	return result;
}

OutputSkinnedMesh& processMeshResource(lwLayer* subset, std::string resourceName)
{
	if(gOutputScene.meshResources.find(resourceName) != gOutputScene.meshResources.end())
		return gOutputScene.meshResources[resourceName];

	OutputSkinnedMesh& result = gOutputScene.meshResources[resourceName];

	// fill result
	size_t vertexCount = subset->point.count;
	size_t polyCount = subset->polygon.count;

	result.clear();
	result.name = resourceName;
	result.vertices.resize(vertexCount);
	result.indices.resize(polyCount*3);

	// [LWO gather] positions & normals
	{
		// $TBD: extract vertex colors from layer
		// $TBD: extract uv from layer
		lwPoint* points = subset->point.pt;
		lwPolygon* polys = subset->polygon.pol;
		for(size_t q = 0; q < vertexCount; q++)
		{
			for(size_t w = 0; w < 3; ++w)
			{
				assert(q < result.vertices.size());
				result.vertices[q].pos[w] = points[q].pos[w];
				result.vertices[q].normal[w] = 0.0f;

				for(int e = 0; e < points[q].npols; ++e)
				{
					int polyId = points[q].pol[e];
					result.vertices[q].normal[w] += polys[polyId].norm[w];
				}
				result.vertices[q].normal[w] /= points[q].npols;
			}
		}
	}

	// [LWO gather] bones & bone weights
	{
		lwVMap* vmap = subset->vmap;
		for(; vmap; vmap = vmap->next)
		{
			if(vmap->type != LWID_('W','G','H','T'))
				continue;

			result.bones.push_back(BaseSkinnedMesh::Bone());
			result.bones.back().name = vmap->name;

			unsigned short boneId = static_cast<unsigned short>(result.bones.size() - 1);
			for(int q = 0; q < vmap->nverts; ++q)
			{
				assert(!vmap->perpoly);
				assert(vmap->dim == 1);

				int vertexId = vmap->vindex[q];
				float boneWeight = vmap->val[q][0];
				if(boneWeight <= WEIGHT_EPSILON )
					continue;

				assert(vertexId < (int)result.vertices.size());
				result.vertices[vertexId].weights.push_back(
					std::make_pair(boneId, boneWeight));
			}
		}
	}

	// [LWO gather] indices
	{
		size_t const POLY_SIZE = 3;
		// $NOTE: assume only 1 subset
		// $TBD: support multiple subsets
		lwPolygon* polys = subset->polygon.pol;
		for(size_t q = 0; q < polyCount; q++)
		{
			for(size_t w = 0; w < POLY_SIZE; ++w)
			{
				assert(POLY_SIZE*3 + w < result.indices.size());
				result.indices[q*POLY_SIZE + w] = 0;
			}

			size_t polySize = polys[q].nverts;
			assert(polySize == POLY_SIZE);
			for(size_t w = 0; w < min(POLY_SIZE, polySize); w++)
			{
				int vertexId = polys[q].v[w].index;
				assert(q*POLY_SIZE + w < result.indices.size());
				result.indices[q*POLY_SIZE + w] = processIndex(vertexId);
			}
		}
	}

	return result;
}

/*
void processMesh(lwObject* lwo, char const* name)
{
	lwLayer* layer = lwo->layer;
	for(; layer; layer = layer->next)
		processMeshSubset(layer, std::string(name) + "_" + std::string(layer->name));

	// $NOTE: assume all(polys) == tris
	// $TBD: non tri polys
}
*/

void processCamera(KFbxCamera* pCamera)
{
	gOutputScene.cameras.push_back(pCamera);
}

void processLight(KFbxLight* pLight)
{
	gOutputScene.lights.push_back(pLight);
}

//mutant::anim_hierarchy gHierarchy;
int processHierarchy(mutant::anim_hierarchy& animHierarchy, KFbxNode* pNode, int pDepth)
{
	animHierarchy.push_back(mutant::anim_hierarchy::node_t(pNode->GetName()));
	int parentNodeIndex = animHierarchy.size()-1;
	
	for(int i = 0; i < pNode->GetChildCount(); i++)
	{
//		processHierarchy(animHierarchy, pNode->GetChild(i), pDepth + 1);
		animHierarchy[parentNodeIndex].add_child(
			processHierarchy(animHierarchy, pNode->GetChild(i), pDepth + 1));
	}
	return parentNodeIndex;
}

std::auto_ptr<mutant::anim_hierarchy> processHierarchy(KFbxScene* pScene)
{
	int i;
	KFbxNode* lRootNode = pScene->GetRootNode();

	std::auto_ptr<mutant::anim_hierarchy> animHierarchy(new mutant::anim_hierarchy());
	animHierarchy->setName(mutant::sTypeNames::HIERARCHY_DEFAULT);

	animHierarchy->push_back(mutant::anim_hierarchy::node_t("root"));
	int parentNodeIndex = animHierarchy->size()-1;

	for( i = 0; i < lRootNode->GetChildCount(); i++)
	{
//		processHierarchy(*animHierarchy, lRootNode->GetChild(i), 0);
		(*animHierarchy)[parentNodeIndex].add_child(
			processHierarchy(*animHierarchy, lRootNode->GetChild(i), 0));
	}
	mutant::anim_hierarchy::node_t::update_parent_all(*animHierarchy);
	return animHierarchy;
}

void processAnimation(mutant::anim_clip& clip, KFbxNode* pNode)
{
    KFbxTakeNode* lCurrentTakeNode = pNode->GetCurrentTakeNode();
	kInt lModelCount;

	KTime animStart, animStop;
	lCurrentTakeNode->GetAnimationInterval(animStart, animStop);
	clip.set_length(static_cast<float>(animStop.GetSecondDouble() - animStart.GetSecondDouble()));

	// Do nothing if the current take node points to default values.
    if(lCurrentTakeNode && lCurrentTakeNode != pNode->GetDefaultTakeNode())
    {
		clip.insertBundle(pNode->GetName(), 
			processChannels(pNode, lCurrentTakeNode, pNode->GetDefaultTakeNode()));
	}

	for(lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
    {
        processAnimation(clip, pNode->GetChild(lModelCount));
    }
}

void processAnimation(mutant::anim_character& animChar, KFbxScene* pScene)
{
	KArrayTemplate<KString*> lTakeNameArray;
	int i;

	pScene->FillTakeNameArray(lTakeNameArray);

	for(i = 0; i < lTakeNameArray.GetCount(); i++)
	{
		// It's useless to display the default animation because it is always empty.
		if(lTakeNameArray.GetAt(i)->Compare(KFBXTAKENODE_DEFAULT_NAME) == 0)
			continue;
		
		pScene->SetCurrentTake(lTakeNameArray.GetAt(i)->Buffer());

		std::auto_ptr<mutant::anim_clip> clip(new mutant::anim_clip());
		processAnimation(*clip, pScene->GetRootNode());
		animChar.insertClip(pScene->GetCurrentTakeName(), clip);
	}

	DeleteAndClear(lTakeNameArray);
}

template <typename C>
float valueAt(C& c, size_t i, float defaultValue = 0.0f)
{
	if(c.empty())
		return defaultValue;
	if(i >= c.size())
		return c.back();
	return c[i];
}

float deg2rad(float deg)
{
	float const deg2rad_ = 3.14f / 180.0f;
	return deg * deg2rad_;
}

std::auto_ptr<mutant::anim_bundle> processChannels(KFbxNode* pNode, KFbxTakeNode* pTakeNode, KFbxTakeNode* pDefaultTakeNode)
{
	KFCurve* lCurve = NULL;

	// process transform curves.
	lCurve = pTakeNode->GetTranslationX();
	CurveT tx = processCurve(lCurve);
	lCurve = pDefaultTakeNode->GetTranslationX();
	float dtx = processDefaultCurve(lCurve);
	
	lCurve = pTakeNode->GetTranslationY();
	CurveT ty = processCurve(lCurve);
	lCurve = pDefaultTakeNode->GetTranslationY();
	float dty = processDefaultCurve(lCurve);

	lCurve = pTakeNode->GetTranslationZ();
	CurveT tz = processCurve(lCurve);
	lCurve = pDefaultTakeNode->GetTranslationZ();
	float dtz = processDefaultCurve(lCurve);

	lCurve = pTakeNode->GetEulerRotationX();
	CurveT rx = processCurve(lCurve);
	lCurve = pDefaultTakeNode->GetEulerRotationX();
	float drx = processDefaultCurve(lCurve);

	lCurve = pTakeNode->GetEulerRotationY();
	CurveT ry = processCurve(lCurve);
	lCurve = pDefaultTakeNode->GetEulerRotationY();
	float dry = processDefaultCurve(lCurve);

	lCurve = pTakeNode->GetEulerRotationZ();
	CurveT rz = processCurve(lCurve);
	lCurve = pDefaultTakeNode->GetEulerRotationZ();
	float drz = processDefaultCurve(lCurve);

	lCurve = pTakeNode->GetScaleX();
	CurveT sx = processCurve(lCurve);
        
	lCurve = pTakeNode->GetScaleY();
	CurveT sy = processCurve(lCurve);

	lCurve = pTakeNode->GetScaleZ();
	CurveT sz = processCurve(lCurve);

	size_t const VQV_CURVE_ANIM_COMPONENTS = 3 + 4 + 3;

/*	assert(tx.size() == ty.size());
	assert(tx.size() == tz.size());
	assert(tx.size() == rx.size());
	assert(tx.size() == ry.size());
	assert(tx.size() == rz.size());
	assert(tx.size() == sx.size());
	assert(tx.size() == sy.size());
	assert(tx.size() == sz.size());*/
	std::vector<float> keys;
	std::vector<float> values;
	keys.reserve(tx.size());
	values.reserve(tx.size() * VQV_CURVE_ANIM_COMPONENTS);
	if(rx.size() == 0)
	{
		keys.push_back(0.0f);
		values.push_back(0.0f);
		values.push_back(0.0f);
		values.push_back(0.0f);
		values.push_back(0.0f);
		values.push_back(0.0f);
		values.push_back(0.0f);
		values.push_back(0.0f);
		values.push_back(1.0f);
		values.push_back(1.0f);
		values.push_back(1.0f);
	}
	for(size_t q = 0; q < rx.size(); ++q)
	{
		keys.push_back(rx.keys[q]);
//		keys.push_back(q * 0.1f);
		values.push_back(valueAt(tx.values, q, dtx));
		values.push_back(valueAt(ty.values, q, dty));
		values.push_back(valueAt(tz.values, q, dtz));
	
		typedef mutant::comp_quaternion_from_euler<Quat,float,float,float> t_comp_3_floats_to_quaternion;
/*
		Quat quat = t_comp_3_floats_to_quaternion()(
			deg2rad(valueAt(ry.values, q) - dry),
			deg2rad(valueAt(rx.values, q) - drx),
			deg2rad(valueAt(rz.values, q) - drz));
/*/
		// xyz	*	.
		// yzx	*	+
		// zxy	*

		// yxz	*	+
		// xzy	*
		// zyx	*	.
/*		Quat quat = t_comp_3_floats_to_quaternion()(
			deg2rad(-valueAt(ry.values, q, dry)),
			deg2rad( valueAt(rz.values, q, drz)),
			deg2rad(-valueAt(rx.values, q, drx)));
*/
		Quat quat;
		
		
		if(gLWMode)
		{
			quat = t_comp_3_floats_to_quaternion()(
				deg2rad( valueAt(ry.values, q, dry)),
				deg2rad( valueAt(rz.values, q, drz)),
				deg2rad( valueAt(rx.values, q, drx)));
		}
		else
		{
			quat = t_comp_3_floats_to_quaternion()(
				deg2rad( valueAt(rx.values, q, drx)),
				deg2rad( valueAt(ry.values, q, dry)),
				deg2rad( valueAt(rz.values, q, drz)));
		}

		KFbxVector4 rotateAroundX(valueAt(rx.values, q, drx), 0, 0);
		KFbxVector4 rotateAroundY(0, valueAt(ry.values, q, dry), 0);
		KFbxVector4 rotateAroundZ(0, 0, valueAt(rz.values, q, drz));

		KFbxXMatrix fbxRotationAroundAxis[3];
		fbxRotationAroundAxis[0].SetR(rotateAroundX);
		fbxRotationAroundAxis[1].SetR(rotateAroundY);
		fbxRotationAroundAxis[2].SetR(rotateAroundZ);

		KFbxXMatrix fbxRotationMatrix;
		
		// rotation order
		ERotationOrder rotationOrder;
		pNode->GetRotationOrder(KFbxNode::eSOURCE_SET, rotationOrder);
		switch(rotationOrder)
		{
			case eSPHERIC_XYZ:
			case eEULER_XYZ:
				fbxRotationMatrix = fbxRotationAroundAxis[2] * fbxRotationAroundAxis[1] * fbxRotationAroundAxis[0];
			break;
			case eEULER_XZY:
				fbxRotationMatrix = fbxRotationAroundAxis[1] * fbxRotationAroundAxis[2] * fbxRotationAroundAxis[0];
			break;
			case eEULER_YZX:
				fbxRotationMatrix = fbxRotationAroundAxis[0] * fbxRotationAroundAxis[2] * fbxRotationAroundAxis[1];
			break;
			case eEULER_YXZ:
				fbxRotationMatrix = fbxRotationAroundAxis[2] * fbxRotationAroundAxis[0] * fbxRotationAroundAxis[1];
			break;
			case eEULER_ZXY:
				fbxRotationMatrix = fbxRotationAroundAxis[1] * fbxRotationAroundAxis[0] * fbxRotationAroundAxis[2];
			break;
			case eEULER_ZYX:
				fbxRotationMatrix = fbxRotationAroundAxis[0] * fbxRotationAroundAxis[1] * fbxRotationAroundAxis[2];
			break;
		}

		KFbxQuaternion quatRotation = fbxRotationMatrix.GetQ();
		quat.x = static_cast<float>(quatRotation[0]);
		quat.y = static_cast<float>(quatRotation[1]);
		quat.z = static_cast<float>(quatRotation[2]);
		quat.w = static_cast<float>(quatRotation[3]);

//*/
		values.push_back(quat.x);
		values.push_back(quat.y);
		values.push_back(quat.z);
		values.push_back(quat.w);

		values.push_back(valueAt(sx.values, q, 1.0f));
		values.push_back(valueAt(sy.values, q, 1.0f));
		values.push_back(valueAt(sz.values, q, 1.0f));
	}

	gCurves.push_back(CurveT(keys, values, VQV_CURVE_ANIM_COMPONENTS));
	mutant::knot_data<float,float>& vqv = gCurves.back().data;

	std::auto_ptr<mutant::anim_bundle> animBundle(
		new mutant::anim_bundle() );
	animBundle->insertData(mutant::sTypeNames::VEC_QUAT_VEC, vqv);
	return animBundle;
}

float processDefaultCurve(KFCurve *pCurve)
{
	return static_cast<float> (pCurve->GetValue());
}

CurveT processCurve(KFCurve *pCurve)
{
	std::vector<float> keys;
	std::vector<float> values;

	if(pCurve->KeyGetCountAll() == 0)
		return CurveT(keys, values, 1);

	kInt lastKey = pCurve->KeyGetCountAll() - 1;
	KTime totalTime = pCurve->KeyGetTimeAll(lastKey);
	KTime curTime; curTime.SetSecondDouble(0.0);
	KTime deltaTime; deltaTime.SetSecondDouble(ANIM_SAMPLING_FREQ);

	kFCurveIndex keyIndex = 0;
	keys.reserve(static_cast<size_t>(totalTime.GetSecondDouble()/ANIM_SAMPLING_FREQ+1));
	values.reserve(static_cast<size_t>(totalTime.GetSecondDouble()/ANIM_SAMPLING_FREQ+1));
	for(; curTime < totalTime; curTime += deltaTime)
	{
		keys.push_back(static_cast<float>(curTime.GetSecondDouble()));
		values.push_back(pCurve->Evaluate(curTime, &keyIndex));
	}

	return CurveT(keys, values, 1);
}



// \mutant stuff =====================================================================================







using namespace FBXSDK_NAMESPACE;

// Local function prototypes.
void DisplayContent(KFbxScene* pScene);
void DisplayContent(KFbxNode* pNode);
void DisplayTarget(KFbxNode* pNode);
void DisplayTransformPropagation(KFbxNode* pNode);
void DisplayGeometricTransform(KFbxNode* pNode);
void DisplayMetaData(KFbxScene* pScene);

/*void processLWOMesh(char const* fname)
{
	unsigned int failID = 0;
	int failpos = 0;
	lwObject* lwo = lwGetObject((char*)fname, &failID, &failpos);
	processMesh(lwo, fname);
}*/


int main(int argc, char** argv)
{

	std::string s;
	assert((s=getBaseName("hello")) == "hello");
	assert((s=getBaseName("hello_ () world")) == "hello_");
	assert((s=getBaseName("hello_ _(1)")) == "hello_ _");
	assert((s=getBaseName("hello__     (1)")) == "hello__");
	assert((s=getBaseName("(1) moo")) == "");
	assert((s=getBaseName("(0)")) == "");
	assert((s=getBaseName("    (0)")) == "");
	assert((s=getBaseName("hello(")) == "hello(");
	assert((s=getBaseName("x::hello(")) == "hello(");
	assert((s=getBaseName("x::y::hello(")) == "hello(");
	assert((s=getBaseName("x::y::hello(::)")) == "hello");
	assert((s=getBaseName("x::y::hello::(z)")) == "");


	KFbxScene* lScene = NULL;
	bool lResult;

	// Prepare the FBX SDK.
	InitializeSdkObjects(gFbxSdkManager, lScene);
	gFbxGeomConverter = gFbxSdkManager->CreateKFbxGeometryConverter();

	// Load the scene.
	// The example can take a FBX file as an argument.
	if(argc > 2)
	{
		std::string target = argv[1];
		if(target == "dx9")
			gPlatform = Platform::DX9;
		else if(target == "psp")
			gPlatform = Platform::PSP;
	}

	if(argc > 1)
	{
		printf("\n\nPlatform: %s\n", gPlatform.name);
		printf("Target dir: %s\n", gPlatform.targetDir);
		printf("File: %s\n\n", argv[argc-1]);
		lResult = LoadScene(gFbxSdkManager, lScene, argv[argc-1]);
		gOutputScene.name = fileName2SceneName(std::string(argv[argc-1]));
	}
	else
	{
		// Destroy all objects created by the FBX SDK.
		DestroySdkObjects(gFbxSdkManager);
		lResult = false;

		printf("\n\nUsage: ImportScene <FBX file name>\n\n");
	}

	if(lResult == false)
	{
		printf("\n\nAn error occured while loading the scene...");
		return 0;
	}
	else 
	{
		mutant::anim_character* animChar = new mutant::anim_character();
		gOutputScene.animResource.insertCharacter("scene", std::auto_ptr<mutant::anim_character>(animChar));

		// Display the scene.
		DisplayMetaData(lScene);

		printf("\n\n---------------------\nGlobal Light Settings\n---------------------\n\n");

//		DisplayGlobalLightSettings(&lScene->GetGlobalLightSettings());

		printf("\n\n----------------------\nGlobal Camera Settings\n----------------------\n\n");

//		DisplayGlobalCameraSettings(&lScene->GetGlobalCameraSettings());

		printf("\n\n--------------------\nGlobal Time Settings\n--------------------\n\n");

//		DisplayGlobalTimeSettings(&lScene->GetGlobalTimeSettings());

		printf("\n\n---------\nHierarchy\n---------\n\n");

		DisplayHierarchy(lScene);
		animChar->insert_hierarchy(processHierarchy(lScene));

		printf("\n\n------------\nNode Content\n------------\n\n");

		DisplayContent(lScene);

		printf("\n\n----\nPose\n----\n\n");

		DisplayPose(lScene);

		printf("\n\n---------\nAnimation\n---------\n\n");

		DisplayAnimation(lScene);
		processAnimation(*animChar, lScene);
	}

	mutalisk::data::scene sceneData;
	blit( gOutputScene, sceneData );
	save( sceneName2FileName(gOutputScene.name), sceneData );

	for(OutputScene::OutputMeshesT::const_iterator i = gOutputScene.meshResources.begin(); i != gOutputScene.meshResources.end(); ++i)
	{
		assert(i->first == i->second.name);
		if(gPlatform == Platform::DX9)
		{
			mutalisk::data::dx9_mesh meshData;
			blit(i->second, meshData);
			save(nodeName2FileName(i->first), meshData);
		}
		else if(gPlatform == Platform::PSP)
		{
			mutalisk::data::psp_mesh meshData;
			blit(i->second, meshData);
			save(nodeName2FileName(i->first), meshData);
		}
	}

	save(sceneName2AnimFileName(gOutputScene.name), gOutputScene.animResource);
	gCurves.clear();


	// Destroy all objects created by the FBX SDK.
	DestroySdkObjects(gFbxSdkManager);

	return 0;
}

void DisplayContent(KFbxScene* pScene)
{
	kInt i;
	KFbxNode* lNode = pScene->GetRootNode();

	if(lNode)
	{
		for(i = 0; i < lNode->GetChildCount(); i++)
		{
			DisplayContent(lNode->GetChild(i));
		}
	}
}

void DisplayContent(KFbxNode* pNode)
{
	KFbxNodeAttribute::EAttributeType lAttributeType;
	kInt i;

    if(pNode->GetNodeAttribute() == NULL)
    {
		printf("NULL Node Attribute\n\n");
    }
	else
    {
		lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());

		switch (lAttributeType)
		{
	        case KFbxNodeAttribute::eMARKER:  
				DisplayMarker(pNode);
				break;
	
			case KFbxNodeAttribute::eSKELETON:  
				DisplaySkeleton(pNode);
				break;
	
			case KFbxNodeAttribute::eMESH:      
				DisplayMesh(pNode);
				processMesh(pNode);
				break;
	
			case KFbxNodeAttribute::eNURB:      
				DisplayNurb(pNode);
				break;
	
			case KFbxNodeAttribute::ePATCH:     
				DisplayPatch(pNode);
				break;
	
			case KFbxNodeAttribute::eCAMERA:    
				DisplayCamera(pNode);
				processCamera((KFbxCamera*) pNode->GetNodeAttribute());
				break;
	
			case KFbxNodeAttribute::eLIGHT:     
				DisplayLight(pNode);
				break;
	    }   
	}

	DisplayUserProperties(pNode);
	DisplayTarget(pNode);
	DisplayPivotsAndLimits(pNode);
	DisplayTransformPropagation(pNode);
	DisplayGeometricTransform(pNode);
	DisplayDefaultAnimation(pNode);	

    for(i = 0; i < pNode->GetChildCount(); i++)
	{
		DisplayContent(pNode->GetChild(i));
	}
}


void DisplayTarget(KFbxNode* pNode)
{
	if(pNode->GetTarget() != NULL)
	{
		DisplayString("    Target Name: ", pNode->GetTarget()->GetName());
	}
}

void DisplayTransformPropagation(KFbxNode* pNode)
{
	printf("    Transformation Propagation\n");

	// 
	// Rotation Space
	//
	ERotationOrder lRotationOrder;
	pNode->GetRotationOrder(KFbxNode::eSOURCE_SET, lRotationOrder);
	
	printf("        Rotation Space: ");

	switch (lRotationOrder)
	{
		case eEULER_XYZ: 
			printf("Euler XYZ\n");
		break;
		case eEULER_XZY:
			printf("Euler XZY\n");
		break;
		case eEULER_YZX:
			printf("Euler YZX\n");
		break;
		case eEULER_YXZ:
			printf("Euler YXZ\n");
		break;
		case eEULER_ZXY:
			printf("Euler ZXY\n");
		break;
		case eEULER_ZYX:
			printf("Euler ZYX\n");
		break;
		case eSPHERIC_XYZ:
			printf("Spheric XYZ\n");
		break;
	}

	//
	// Use the Rotation space only for the limits
	// (keep using eEULER_XYZ for the rest)
	//
	printf("        Use the Rotation Space for Limit specification only: %s\n",
		pNode->GetUseRotationSpaceForLimitOnly(KFbxNode::eSOURCE_SET) ? "Yes" : "No");
	

	//
	// Inherit Type
	//
	ETransformInheritType lInheritType;
	pNode->GetTransformationInheritType(lInheritType);

	printf("        Transformation Inheritance: ");

	switch (lInheritType)
	{
		case eINHERIT_RrSs:
			printf("RrSs\n");
		break;
		case eINHERIT_RSrs:
			printf("RSrs\n");
		break;
		case eINHERIT_Rrs:
			printf("Rrs\n");
		break;
	}
}

void DisplayGeometricTransform(KFbxNode* pNode)
{
	KFbxVector4 lTmpVector;

	printf("    Geometric Transformations\n");

	//
	// Translation
	//
	lTmpVector = pNode->GetGeometricTranslation(KFbxNode::eSOURCE_SET);
	printf("        Translation: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

	//
	// Rotation
	//
	lTmpVector = pNode->GetGeometricRotation(KFbxNode::eSOURCE_SET);
	printf("        Rotation:    %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

	//
	// Scaling
	//
	lTmpVector = pNode->GetGeometricScaling(KFbxNode::eSOURCE_SET);
	printf("        Scaling:     %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);
}


void DisplayMetaData(KFbxScene* pScene)
{
	KFbxSceneInfo* sceneInfo = pScene->GetSceneInfo();
	if (sceneInfo)
	{
		printf("\n\n--------------------\nMeta-Data\n--------------------\n\n");
		printf("    Title: %s\n", sceneInfo->mTitle.GetBuffer());
		printf("    Subject: %s\n", sceneInfo->mSubject.GetBuffer());
		printf("    Author: %s\n", sceneInfo->mAuthor.GetBuffer());
		printf("    Keywords: %s\n", sceneInfo->mKeywords.GetBuffer());
		printf("    Revision: %s\n", sceneInfo->mRevision.GetBuffer());
		printf("    Comment: %s\n", sceneInfo->mComment.GetBuffer());

		KFbxThumbnail* thumbnail = sceneInfo->GetSceneThumbnail();
		if (thumbnail)
		{
			printf("    Thumbnail:\n");

			switch (thumbnail->GetDataFormat())
			{
			case KFbxThumbnail::eRGB_24:
				printf("        Format: RGB\n");
				break;
			case KFbxThumbnail::eRGBA_32:
				printf("        Format: RGBA\n");
				break;
			}

			switch (thumbnail->GetSize())
			{
			case KFbxThumbnail::eNOT_SET:
				printf("        Size: no dimensions specified (%ld bytes)\n", thumbnail->GetSizeInBytes());
				break;
			case KFbxThumbnail::e64x64:
				printf("        Size: 64 x 64 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
				break;
			case KFbxThumbnail::e128x128:
				printf("        Size: 128 x 128 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
			}
		}
	}
}