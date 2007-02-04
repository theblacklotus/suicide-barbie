#ifndef MUTALISK_EXPORT_H_
#define MUTALISK_EXPORT_H_

#include <fbxsdk.h>

#include <d3d9.h>
#include "../dx9/Dx9NullDevice.h"


#ifdef __cplusplus
extern "C" {
#endif
	#include <lwo2.h>
#ifdef __cplusplus
}
#endif 

// mutalisk stuff ======================================================================================
#pragma warning(disable:4512)
#pragma warning(disable:4996)
//#pragma warning( disable:4239 )

float const WEIGHT_EPSILON = 1e-4f;
double const ANIM_SAMPLING_FREQ = 1.0f / 30.0;

#include <set>
#include <map>
#include <list>
#include <vector>
#include <limits>
#include <memory>
#include <string>

#include <mutant/mutant.h>
#include <mutant/writer.h>
#include <mutant/io_factory.h>

#include <mutalisk/mutalisk.h>
#include <mutalisk/types.h>
#include <mutalisk/dx9/dx9.h>
#include <mutalisk/psp/psp.h>
#include <mutalisk/psp/pspXcompile.h>

#include <d3d9.h>
#include <d3dx9.h>

namespace mutalisk
{
void error(std::string msg) { throw std::runtime_error(msg); }
void warning(std::string msg) { printf("WARNING: %s\n", msg.c_str()); }// assert(!!!"WARNING"); }
//void assertWarning(bool condition, std::string msg) { if(!condition) warning(msg); }
void assertWarning(int condition, std::string msg) { if(!condition) warning(msg); }
#define warning(msg, action) { warning(msg); action; }
#define assertWarning(condition, msg, action) { if(!(condition)) {warning(msg); action;} }

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
		Vec3 uvw;

		std::vector<
			std::pair<BoneIndexT, float> > weights;
	};
	typedef unsigned short IndexT;
	struct Subset
	{
		std::vector<IndexT> indices;
	};
	typedef std::vector<Subset> SubsetsT;

	// data
	std::vector<Vertex> vertices;
	std::vector<IndexT> indices;
	std::vector<Subset> subsets;
	std::vector<Bone> bones;
};

struct OutputSkinnedMesh : public BaseSkinnedMesh
{
	std::string	source;
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

struct OutputTexture
{
	std::string					source;
	std::string					name;
	KFbxTexture*				parameters;
	com_ptr<IDirect3DTexture9>	data;
};

struct OutputScene
{
	// definition
	struct Material
	{
		OutputTexture*			texture;
		KFbxSurfaceMaterial*	parameters;
		std::string				shader;
		//Shader*				shader;
	};
	typedef std::vector<Material>						MaterialsT;

	struct Properties
	{
		typedef std::map<std::string, std::string>			StringPropertyMapT;
		typedef std::map<std::string, array<double> >		VectorPropertyMapT;

		bool hasString(std::string const& byName) const { return (strings.find(byName) != strings.end()); }
		bool hasVector(std::string const& byName) const { return (vectors.find(byName) != vectors.end()); }

		StringPropertyMapT	strings;
		VectorPropertyMapT	vectors;
	};

	struct Actor
	{
		OutputSkinnedMesh*	mesh;
		MaterialsT			materials;
		Properties			properties;

		KFbxNode*			node;
	};

	typedef std::map<std::string, OutputSkinnedMesh>	OutputMeshesT;
	typedef std::map<std::string, OutputTexture>		OutputTexturesT;
	typedef std::set<std::string>						ShadersT;
	typedef std::vector<KFbxCamera*>					CamerasT;
	typedef std::vector<KFbxLight*>						LightsT;
	typedef std::vector<Actor>							ActorsT;

	// data
	std::string				source;
	std::string				name;

	mutant::anim_character_set
							animResource;
	OutputMeshesT			meshResources;
	OutputTexturesT			textureResources;
	ShadersT				shaders;
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
void processMaterials(KFbxMesh* pMesh, OutputScene::MaterialsT& materials, OutputSkinnedMesh::SubsetsT* subsets = 0);
void processSubsets(KFbxMesh* pMesh, OutputSkinnedMesh::SubsetsT& subsets);
void processProperties(KFbxObject const* pObject, OutputScene::Properties& properties);
OutputTexture& processTextureResource(KFbxTexture* pTexture);
std::string processShaderResource(KFbxSurfaceMaterial* pMaterial);
OutputSkinnedMesh& processMeshResource(KFbxNode* pNode);
OutputSkinnedMesh& processMeshResource(lwLayer* subset, std::string resourceName);
void processCamera(KFbxNode* pNode);
void processLight(KFbxNode* pLight);

int processHierarchy(mutant::anim_hierarchy& hierarchy, KFbxNode* pNode, int pDepth);
//std::auto_ptr<mutant::anim_hierarchy> processHierarchy(KFbxScene* pScene);
void processHierarchy(mutant::anim_character& anim_char, KFbxScene* pScene);
void processAnimation(mutant::anim_clip& clip, KFbxNode* pNode);
void processAnimation(mutant::anim_character& anim_char, KFbxScene* pScene);
std::auto_ptr<mutant::anim_bundle> processChannels(KFbxNode* pNode, KFbxTakeNode* pTakeNode, KFbxTakeNode* pDefaultTakeNode);
CurveT processCurve(KFCurve *pCurve);
float processDefaultCurve(KFCurve *pCurve);

void applyProperties(OutputScene::Actor& actor, OutputScene::Properties const& properties);

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
com_ptr<IDirect3DDevice9> gDxNullRefDevice = 0;


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

std::string nodeBaseName(std::string nodeName)
{
	return cutFront(cutEnd(nodeName));
}

std::string nodeBaseName(char const* nodeName)
{
	return nodeBaseName(std::string(nodeName));
}

std::string shaderBaseName(std::string shaderName)
{
	shaderName = nodeBaseName(shaderName);
	return shaderName.substr(0, shaderName.find_last_of("_"));
}

std::string shaderBaseName(char const* shaderName)
{
	return shaderBaseName(std::string(shaderName));
}

std::string withoutPath(std::string fileName, char delimiter)
{
	std::string::const_reverse_iterator tailIt = fileName.rbegin();
	while(tailIt != fileName.rend() && *tailIt == delimiter) ++tailIt;

	std::string::const_reverse_iterator headIt = tailIt;
	while(headIt != fileName.rend() && *headIt != delimiter) ++headIt;
	return std::string(headIt.base(), tailIt.base());
}

std::string withoutPath(std::string fileName)
{
	fileName = withoutPath(fileName, '/');
	fileName = withoutPath(fileName, '\\');
	return fileName;
}

std::string withoutExtension(std::string fileName)
{
	return fileName.substr(0, fileName.find_last_of("."));
}

std::string onlyExtension(std::string fileName)
{
	size_t pos = fileName.find_last_of(".");
	if(pos == std::string::npos)
		return "";
	return fileName.substr(pos + 1);
}

std::string fileName2MeshName(std::string fileName)
{
	return withoutExtension(withoutPath(fileName));
}

std::string fileName2TextureName(std::string fileName)
{
	return withoutExtension(withoutPath(fileName));
}

std::string fileName2SceneName(std::string fileName)
{
	return withoutExtension(withoutPath(fileName));
}

std::string meshName2FileName(std::string nodeName)
{
	std::replace(nodeName.begin(), nodeName.end(), ':', '_');
	std::replace(nodeName.begin(), nodeName.end(), '.', '_');
	std::replace(nodeName.begin(), nodeName.end(), ' ', '_');
	return nodeName + ".msh";
}

std::string textureName2FileName(std::string textureName)
{
	return withoutExtension(textureName) + ".png";
}

std::string sceneName2FileName(std::string sceneName)
{
	return sceneName + ".msk";
}

std::string sceneName2AnimFileName(std::string sceneName)
{
	return sceneName + ".man";
}

std::string outputFileName(std::string fileName)
{
	return gPlatform.targetDir + fileName;
}

template <typename Data>
void save(std::string dstName, Data& data)
{
	mutant::mutant_writer mutWriter(mutant::writer_factory::createOutput(outputFileName(dstName)));
	mutWriter << data;
}

template <>
void save(std::string dstName, mutant::anim_character_set& data)
{
	mutant::mutant_writer mutWriter(mutant::writer_factory::createOutput(outputFileName(dstName)));
	mutWriter.write(data);
}

template <>
void save(std::string dstName, IDirect3DTexture9& texture)
{
	std::string ext = onlyExtension(dstName);
	D3DXIMAGE_FILEFORMAT format = D3DXIFF_DDS;
	if(ext == "bmp")
		format = D3DXIFF_BMP;
	else if(ext == "jpg")
		format = D3DXIFF_JPG;
	else if(ext == "tga")
		format = D3DXIFF_TGA;
	else if(ext == "png")
		format = D3DXIFF_PNG;
	else if(ext == "dds")
		format = D3DXIFF_DDS;
	else if(ext == "ppm")
		format = D3DXIFF_PPM;
	else if(ext == "dib")
		format = D3DXIFF_DIB;
	else if(ext == "hdr")
		format = D3DXIFF_HDR;
	else if(ext == "pfm")
		format = D3DXIFF_PFM;
	else
		warning("Texture format not recognized, defaulting to DDS");

	HRESULT hr = D3DXSaveTextureToFile(outputFileName(dstName).c_str(), format, &texture, 0);
	assertWarning(SUCCEEDED(hr), std::string("Failed to save texture: ") + dstName);
}

void blitBaseMesh(OutputSkinnedMesh const& mesh, mutalisk::data::base_mesh& data)
{
	data.vertexCount = mesh.vertices.size();
	data.vertexDataSize = data.vertexCount * data.vertexStride;
	data.vertexData = new byte[data.vertexDataSize];
	for(size_t q = 0; q < data.vertexCount; ++q)
	{
		float* asFloat = reinterpret_cast<float*>(data.vertexData + q * data.vertexStride);

		int i = 0;
		if(gLWMode)
		{
			asFloat[i++] = mesh.vertices[q].pos[2]* 100.0f;
			asFloat[i++] = mesh.vertices[q].pos[0]* 100.0f;
			asFloat[i++] = mesh.vertices[q].pos[1]* 100.0f;
			asFloat[i++] = mesh.vertices[q].normal[0]* 1.0f;
			asFloat[i++] = mesh.vertices[q].normal[1]* 1.0f;
			asFloat[i++] = mesh.vertices[q].normal[2]* 1.0f;
			asFloat[i++] = mesh.vertices[q].uvw[0];
			asFloat[i++] = mesh.vertices[q].uvw[1];
		}
		else
		{
			asFloat[i++] = mesh.vertices[q].pos[0];
			asFloat[i++] = mesh.vertices[q].pos[1];
			asFloat[i++] = mesh.vertices[q].pos[2];
			asFloat[i++] = mesh.vertices[q].normal[0];
			asFloat[i++] = mesh.vertices[q].normal[1];
			asFloat[i++] = mesh.vertices[q].normal[2];
			asFloat[i++] = mesh.vertices[q].uvw[0];
			asFloat[i++] = mesh.vertices[q].uvw[1];
		}
	}
}

void blitMesh(OutputSkinnedMesh const& mesh, mutalisk::data::dx9_mesh& data)
{
	data.fvfVertexDecl = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
	data.vertexStride = D3DXGetFVFVertexSize(data.fvfVertexDecl);
	data.skinInfo = 0;

	blitBaseMesh(mesh, data);
}

void blitMesh(OutputSkinnedMesh const& mesh, mutalisk::data::psp_mesh& data)
{
	data.vertexDecl = GU_VERTEX_32BITF | GU_NORMAL_32BITF | GU_TEXTURE_32BITF;
	data.vertexStride = sizeof(float) * (3+3+2);
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
			int i = 0;
			asFloat[i++] = mesh.vertices[q].uvw[0];
			asFloat[i++] = mesh.vertices[q].uvw[1];
			asFloat[i++] = mesh.vertices[q].normal[0]* 1.0f;
			asFloat[i++] = mesh.vertices[q].normal[1]* 1.0f;
			asFloat[i++] = mesh.vertices[q].normal[2]* 1.0f;
			asFloat[i++] = mesh.vertices[q].pos[2]* 100.0f;
			asFloat[i++] = mesh.vertices[q].pos[0]* 100.0f;
			asFloat[i++] = mesh.vertices[q].pos[1]* 100.0f;
		}
		else
		{
			int i = 0;
			asFloat[i++] = mesh.vertices[q].uvw[0];
			asFloat[i++] = mesh.vertices[q].uvw[1];
			asFloat[i++] = mesh.vertices[q].normal[0];
			asFloat[i++] = mesh.vertices[q].normal[1];
			asFloat[i++] = mesh.vertices[q].normal[2];
			asFloat[i++] = mesh.vertices[q].pos[0];
			asFloat[i++] = mesh.vertices[q].pos[1];
			asFloat[i++] = mesh.vertices[q].pos[2];
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
	data.fvfVertexDecl = D3DFVF_XYZB5 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
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
	data.skinInfo->bones.resize(mesh.bones.size());
	for(size_t q = 0; q < data.skinInfo->bones.size(); ++q)
	{
		assert(sizeof(data.skinInfo->bones[q].matrix) == sizeof(mesh.bones[q].matrix));
		memcpy(&data.skinInfo->bones[q].matrix, &mesh.bones[q].matrix, sizeof(mesh.bones[q].matrix));
		data.skinInfo->bones[q].name = mesh.bones[q].name;
	}

	for(size_t q = 0; q < data.vertexCount; ++q)
	{
		float* asFloat = reinterpret_cast<float*>(data.vertexData + q * data.vertexStride);
		DWORD* asDword = reinterpret_cast<DWORD*>(data.vertexData + q * data.vertexStride);

		int i = 0;
		if(gLWMode)
		{
			asFloat[i++] = mesh.vertices[q].pos[2]* 100.0f;
			asFloat[i++] = mesh.vertices[q].pos[0]* 100.0f;
			asFloat[i++] = mesh.vertices[q].pos[1]* 100.0f;
			asFloat[i++] = mesh.vertices[q].normal[0]* 1.0f;
			asFloat[i++] = mesh.vertices[q].normal[1]* 1.0f;
			asFloat[i++] = mesh.vertices[q].normal[2]* 1.0f;
			asFloat[i++] = mesh.vertices[q].uvw[0];
			asFloat[i++] = mesh.vertices[q].uvw[1];
		}
		else
		{
			asFloat[i++] = mesh.vertices[q].pos[0];
			asFloat[i++] = mesh.vertices[q].pos[1];
			asFloat[i++] = mesh.vertices[q].pos[2];
			asFloat[i++] = mesh.vertices[q].normal[0];
			asFloat[i++] = mesh.vertices[q].normal[1];
			asFloat[i++] = mesh.vertices[q].normal[2];
			asFloat[i++] = mesh.vertices[q].uvw[0];
			asFloat[i++] = mesh.vertices[q].uvw[1];
		}

		size_t weightsPerVertex = data.skinInfo->weightsPerVertex;

		size_t w = 0;
		for(; w < weightsPerVertex; ++w)
		{
			float weight = getBoneWeight(mesh, q, w).second;				// weight
			asFloat[i++] = weight;
		}
		for(; w < 4; ++w)
			asFloat[i++] = 0.0f;

		asDword[i] = 0;
		for(size_t w = 0; w < weightsPerVertex; ++w)
		{
			unsigned short boneIndex = getBoneWeight(mesh, q, w).first;		// boneIndex
			assert(boneIndex < 256);
			asDword[i] |= ((0xff & boneIndex) << (w * 8));
		}
		++i;
	}
}

void blitSkinned(OutputSkinnedMesh const& mesh, mutalisk::data::psp_mesh& data)
{
	size_t weightsPerVertex = getWeightsPerVertex(mesh);

	data.vertexCount = mesh.vertices.size();
	data.vertexDecl = GU_VERTEX_32BITF | GU_NORMAL_32BITF | GU_TEXTURE_32BITF | GU_WEIGHT_32BITF|GU_WEIGHTS(weightsPerVertex);
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
	data.skinInfo->bones.resize(mesh.bones.size());
	for(size_t q = 0; q < data.skinInfo->bones.size(); ++q)
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

		w = weightsPerVertex;
		if(gLWMode)
		{
			asFloat[w++] = mesh.vertices[q].uvw[0];
			asFloat[w++] = mesh.vertices[q].uvw[1];
			asFloat[w++] = mesh.vertices[q].normal[0]* 1.0f;
			asFloat[w++] = mesh.vertices[q].normal[1]* 1.0f;
			asFloat[w++] = mesh.vertices[q].normal[2]* 1.0f;
			asFloat[w++] = mesh.vertices[q].pos[0]* 100.0f;
			asFloat[w++] = mesh.vertices[q].pos[1]* 100.0f;
			asFloat[w++] = mesh.vertices[q].pos[2]* 100.0f;
		}
		else
		{
			asFloat[w++] = mesh.vertices[q].uvw[0];
			asFloat[w++] = mesh.vertices[q].uvw[1];
			asFloat[w++] = mesh.vertices[q].normal[0];
			asFloat[w++] = mesh.vertices[q].normal[1];
			asFloat[w++] = mesh.vertices[q].normal[2];
			asFloat[w++] = mesh.vertices[q].pos[0];
			asFloat[w++] = mesh.vertices[q].pos[1];
			asFloat[w++] = mesh.vertices[q].pos[2];
		}
	}
}

void blitIndices(OutputSkinnedMesh const& mesh, mutalisk::data::base_mesh& data)
{
	data.indexCount = mesh.indices.size();
	data.indexSize = sizeof(OutputSkinnedMesh::IndexT);
	data.indexData = new byte[data.indexCount * data.indexSize];

	unsigned int indexOffset = 0;
	data.subsets.resize(mesh.subsets.size());
	if(mesh.subsets.empty())
	{
		for(size_t q = 0; q < data.indexCount; ++q)
			reinterpret_cast<OutputSkinnedMesh::IndexT*>(data.indexData)[q] = mesh.indices[q];
	}
	else
	for(size_t q = 0; q < mesh.subsets.size(); ++q)
	{
		unsigned int indexCount = 0;

		typedef std::vector<OutputSkinnedMesh::IndexT> IndicesT;
		IndicesT const& indices = mesh.subsets[q].indices;
		for(IndicesT::const_iterator indexIt = indices.begin(); indexIt != indices.end(); ++indexIt, ++indexCount) 
			reinterpret_cast<OutputSkinnedMesh::IndexT*>(data.indexData)[indexOffset + indexCount] = *indexIt;
		assert(indexCount == indices.size());

		data.subsets[q].offset = indexOffset;
		data.subsets[q].count = indexCount;
		indexOffset += indexCount;
		assert(indexOffset <= data.indexCount);
	}
}

void blit(OutputSkinnedMesh const& mesh, mutalisk::data::dx9_mesh& data)
{
	if(getWeightsPerVertex(mesh) > 0)
		blitSkinned(mesh, data);
	else
		blitMesh(mesh, data);

	// $TBD: 32 bit indices
	blitIndices(mesh, data);

	data.primitiveType = D3DPT_TRIANGLELIST;
}

void blit(OutputSkinnedMesh const& mesh, mutalisk::data::psp_mesh& data)
{
	if(getWeightsPerVertex(mesh) > 0)
		blitSkinned(mesh, data);
	else
		blitMesh(mesh, data);

	blitIndices(mesh, data);

	data.primitiveType = GU_TRIANGLES;
}

void blit(OutputScene const& scene, mutalisk::data::scene& data)
{
	// shared resources
	size_t q = 0;
	data.meshIds.resize(scene.meshResources.size());
	for(OutputScene::OutputMeshesT::const_iterator i = scene.meshResources.begin(); i != scene.meshResources.end(); ++i, ++q)
	{
		data.meshIds[q] = meshName2FileName(i->second.name);
	}

	q = 0;
	data.textureIds.resize(scene.textureResources.size());
	for(OutputScene::OutputTexturesT::const_iterator i = scene.textureResources.begin(); i != scene.textureResources.end(); ++i, ++q)
	{
		data.textureIds[q] = textureName2FileName(i->second.name);
	}

	q = 0;
	data.shaderIds.resize(scene.shaders.size());
	for(OutputScene::ShadersT::const_iterator i = scene.shaders.begin(); i != scene.shaders.end(); ++i, ++q)
	{
		data.shaderIds[q] = *i;
	}

	// lights
	q = 0;
	data.lights.resize(scene.lights.size());
	for(OutputScene::LightsT::const_iterator i = scene.lights.begin(); i != scene.lights.end(); ++i, ++q)
	{
		assert(*i);
		assert((*i)->GetNode());
		assert((*i)->GetAttributeType() == KFbxNodeAttribute::eLIGHT);
		char const* name = (*i)->GetNode()->GetName();
		data.lights[q].nodeName = name;
		processMatrix(data.lights[q].worldMatrix.data, (*i)->GetNode()->GetGlobalFromDefaultTake());

		OutputScene::Properties properties;
		processProperties((*i)->GetNode(), properties);

		switch((*i)->GetLightType())
		{
		case KFbxLight::eDIRECTIONAL:
			data.lights[q].type = mutalisk::data::scene::Light::Directional;
			break;
		case KFbxLight::ePOINT:
			data.lights[q].type = mutalisk::data::scene::Light::Point;
			break;
		case KFbxLight::eSPOT:
			data.lights[q].type = mutalisk::data::scene::Light::Spot;
			break;
		}
/*		if(properties.hasString("typeEx"))
		{
			if(properties.strings["typeEx"] == "DirectionalExt")
			{
				assertWarning(properties.hasVector("backColor"), "DirectionalExt light type misses 'Back Color' property");
				assertWarning(properties.hasVector("equatorColor"), "DirectionalExt light type misses 'Equator Color' property");
			}
		}*/
		data.lights[q].ambient.r = data.lights[q].ambient.g = data.lights[q].ambient.b = 0.0f;
		data.lights[q].ambient.a = 1.0f;

		KFbxColor lColor; (*i)->GetDefaultColor(lColor);
		float intensity = static_cast<float>((*i)->GetDefaultIntensity()) * 0.01f;
		data.lights[q].diffuse.r = static_cast<float>(lColor.mRed) * intensity;		
		data.lights[q].diffuse.g = static_cast<float>(lColor.mGreen) * intensity;
		data.lights[q].diffuse.b = static_cast<float>(lColor.mBlue) * intensity;		
		data.lights[q].diffuse.a = static_cast<float>(lColor.mAlpha) * intensity;

		data.lights[q].specular = data.lights[q].diffuse;

		// $TBD: light attenuation (decay)
		//(*i)->GetDecayStart();
		//(*i)->GetDecayEnd();
		//(*i)->GetDecayType();

		data.lights[q].phi = static_cast<float>((*i)->GetDefaultConeAngle()) * (D3DX_PI / 180.0f);
		data.lights[q].theta = 0.0f;
	}

	// cameras
	data.cameras.resize(scene.cameras.size());
	if(!scene.cameras.empty())
		data.defaultCameraIndex = 0;
	else
		data.defaultCameraIndex = ~0U;

	q = 0;
	for(OutputScene::CamerasT::const_iterator i = scene.cameras.begin(); i != scene.cameras.end(); ++i, ++q)
	{
		assert(*i);
		assert((*i)->GetNode());
		assert((*i)->GetAttributeType() == KFbxNodeAttribute::eCAMERA ||
			(*i)->GetAttributeType() == KFbxNodeAttribute::eCAMERA_SWITCHER);
		char const* name = (*i)->GetNode()->GetName();
		data.cameras[q].nodeName = name;
		processMatrix(data.cameras[q].worldMatrix.data, (*i)->GetNode()->GetGlobalFromDefaultTake());
	}

	// actors
	data.actors.resize(scene.actors.size());
	q = 0;
	for(OutputScene::ActorsT::const_iterator i = scene.actors.begin(); i != scene.actors.end(); ++i, ++q)
	{
		assert(i->node);
		assert(i->mesh);

		// node
		data.actors[q].nodeName = scene.actors[q].node->GetName();

		// mesh
		processMatrix(data.actors[q].worldMatrix.data, i->node->GetGlobalFromDefaultTake());
		size_t meshIndex = std::distance(scene.meshResources.begin(), scene.meshResources.find(i->mesh->source));
		assert(meshIndex < scene.meshResources.size());
		data.actors[q].meshIndex = meshIndex;

		// materials
		data.actors[q].materials.resize(i->materials.size());
		for(size_t w = 0; w < i->materials.size(); ++w)
		{
			{ // texture
				std::string textureName = "";
				if(i->materials[w].texture)
					textureName = i->materials[w].texture->source;

				size_t textureIndex = std::distance(scene.textureResources.begin(), scene.textureResources.find(textureName));
				if(textureIndex == scene.textureResources.size())
					textureIndex = ~0U;

				data.actors[q].materials[w].textureIndex = textureIndex;
			}

			{ // shader
				std::string shaderName = i->materials[w].shader;
				size_t shaderIndex = std::distance(scene.shaders.begin(), scene.shaders.find(shaderName));
				if(shaderIndex == scene.shaders.size())
					shaderIndex = ~0U;

				data.actors[q].materials[w].shaderIndex = shaderIndex;
			}

			{ // surface properties
				mutalisk::data::Color& ambient = data.actors[q].materials[w].ambient;
				mutalisk::data::Color& diffuse = data.actors[q].materials[w].diffuse;
				mutalisk::data::Color& specular = data.actors[q].materials[w].specular;

				KFbxPropertyDouble3 lKFbxDouble3;
				if(i->materials[w].parameters->GetNewFbxClassId().Is(KFbxSurfaceLambert::ClassId))
				{
					KFbxSurfaceLambert* surface = static_cast<KFbxSurfaceLambert*>(i->materials[w].parameters);
					lKFbxDouble3 = surface->GetAmbientColor();
					ambient.r = static_cast<float>(lKFbxDouble3.Get()[0]);
					ambient.g = static_cast<float>(lKFbxDouble3.Get()[1]);
					ambient.b = static_cast<float>(lKFbxDouble3.Get()[2]);
					ambient.a = 1.0f;

					lKFbxDouble3 = surface->GetDiffuseColor();
					diffuse.r = static_cast<float>(lKFbxDouble3.Get()[0]);
					diffuse.g = static_cast<float>(lKFbxDouble3.Get()[1]);
					diffuse.b = static_cast<float>(lKFbxDouble3.Get()[2]);
					diffuse.a = 1.0f;

					specular.r = 0.0f;
					specular.g = 0.0f;
					specular.b = 0.0f;
					specular.a = 1.0f;
				}
				else if (i->materials[w].parameters->GetNewFbxClassId().Is(KFbxSurfacePhong::ClassId))
				{
					KFbxSurfacePhong* surface = static_cast<KFbxSurfacePhong*>(i->materials[w].parameters);
					lKFbxDouble3 = surface->GetAmbientColor();
					ambient.r = static_cast<float>(lKFbxDouble3.Get()[0]);
					ambient.g = static_cast<float>(lKFbxDouble3.Get()[1]);
					ambient.b = static_cast<float>(lKFbxDouble3.Get()[2]);
					ambient.a = 1.0f;

					lKFbxDouble3 = surface->GetDiffuseColor();
					diffuse.r = static_cast<float>(lKFbxDouble3.Get()[0]);
					diffuse.g = static_cast<float>(lKFbxDouble3.Get()[1]);
					diffuse.b = static_cast<float>(lKFbxDouble3.Get()[2]);
					diffuse.a = 1.0f;

					lKFbxDouble3 = surface->GetSpecularColor();
					specular.r = static_cast<float>(lKFbxDouble3.Get()[0]);
					specular.g = static_cast<float>(lKFbxDouble3.Get()[1]);
					specular.b = static_cast<float>(lKFbxDouble3.Get()[2]);
					specular.a = 1.0f;
				}
			}
		}
	}

	// animations
	data.animCharId = sceneName2AnimFileName(scene.name);
	if(scene.animResource.charPair(0).second && scene.animResource.charPair(0).second->size() > 0)
		data.defaultClipIndex = 0;
	else
		data.defaultClipIndex = ~0U;
}


unsigned short processIndex(kInt srcIndex)
{
	unsigned long dw = static_cast<unsigned long>(srcIndex);
	if( dw & 0xffffff00000000 )
		warning("Couldn't convert 32bit index to 16bit index", dw = 0);
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

void processSubsets(KFbxMesh* pMesh, OutputSkinnedMesh::SubsetsT& subsets)
{
	OutputScene::MaterialsT tmpMaterials;
	processMaterials(pMesh, tmpMaterials, &subsets);
}

void processMaterials(KFbxMesh* pMesh, OutputScene::MaterialsT& materials, OutputSkinnedMesh::SubsetsT* subsets)
{
	// NOTE: use only 1st layer for subset splitting
	kInt const MAX_LAYER_COUNT = 1;
	for (kInt l = 0; l < min(MAX_LAYER_COUNT, pMesh->GetLayerCount()); l++)
	{
		KFbxLayerElementTexture* leTex = pMesh->GetLayer(l)->GetTextures(KFbxLayerElementTexture::eDIFFUSE_TEXTURES);
		KFbxLayerElementMaterial* leMat = pMesh->GetLayer(l)->GetMaterials();

		assertWarning(leMat, "No materials found", continue);

		int textureCount = 0;
		int textureIndexCount = 0;
		kInt* textureIndices = 0;
		if (leTex)
		{
			if (leTex->GetReferenceMode() != KFbxLayerElement::eINDEX &&
				leTex->GetReferenceMode() != KFbxLayerElement::eINDEX_TO_DIRECT)
				warning("Not supported reference mode for KFbxLayerElement<Texture>", continue);

			if (leTex->GetMappingMode() == KFbxLayerElement::eBY_POLYGON)
				textureIndexCount = pMesh->GetPolygonCount();
			else if (leTex->GetMappingMode() == KFbxLayerElement::eALL_SAME)
				textureIndexCount = 1;
			else
				warning("Not supported mapping mode for KFbxLayerElement<Texture>", continue);

			textureCount = leTex->GetDirectArray().GetCount();
			textureIndices = leTex->GetIndexArray().GetArray();
		}

		int materialCount = 0;
		int materialIndexCount = 0;
		kInt* materialIndices = 0;
		assert(leMat);
		{
			if (leMat->GetReferenceMode() != KFbxLayerElement::eINDEX &&
				leMat->GetReferenceMode() != KFbxLayerElement::eINDEX_TO_DIRECT)
				warning("Not supported reference mode for KFbxLayerElement<Material>", continue);

			if (leMat->GetMappingMode() == KFbxLayerElement::eBY_POLYGON)
				materialIndexCount = pMesh->GetPolygonCount();
			else if(leMat->GetMappingMode() == KFbxLayerElement::eALL_SAME)
				materialIndexCount = 1;
			else
				warning("Not supported mapping mode for KFbxLayerElement<Material>", continue);

			materialCount = leMat->GetDirectArray().GetCount();
			materialIndices = leMat->GetIndexArray().GetArray();
		}

		// find all unique material/texture combinations and store them as subsets
		typedef int MaterialIndexT;
		typedef int TextureIndexT;
		typedef size_t CombinationIndexT;
		typedef std::pair<MaterialIndexT, TextureIndexT> CombinationKeyT;
		std::map<CombinationKeyT, CombinationIndexT> combinationIndices;

		// preallocate enough space for all possible material/texture permutations
		materials.resize(materialCount * textureCount);
		if (subsets)
			subsets->resize(materialCount * textureCount);

		if (materials.empty())
			continue;

		CombinationIndexT nextCombinationIndex = 0;
		for (int i = 0, materialIt = 0, textureIt = 0; i < pMesh->GetPolygonCount(); ++i)
		{
			assert(leMat);
			assert(materialIndices);
			kInt materialIndex = materialIndices[materialIt];
			KFbxSurfaceMaterial* lMaterial = leMat->GetDirectArray().GetAt(materialIndex);

			KFbxTexture* lTexture = 0;
			kInt textureIndex = -1;
			if (leTex)
			{
				assert(textureIndices);
				textureIndex = textureIndices[textureIt];
				if (textureIndex >= 0)
					lTexture = leTex->GetDirectArray().GetAt(textureIndex);
			}

			if (materialIt < materialIndexCount - 1)
				++materialIt;
			if (textureIt < textureIndexCount - 1)
				++textureIt;

			CombinationKeyT combKey = std::make_pair(materialIndex, textureIndex);
			if (combinationIndices.find(combKey) == combinationIndices.end())
				combinationIndices[combKey] = nextCombinationIndex++;
			int combIndex = combinationIndices[combKey];

			{
				materials[combIndex].texture = (lTexture)? &processTextureResource(lTexture): 0;
				materials[combIndex].parameters = lMaterial;
				materials[combIndex].shader = (lMaterial)? processShaderResource(lMaterial): "";

				if (subsets)
				{
					kInt const POLY_SIZE = 3;
					assert(pMesh->GetPolygonSize(i) <= POLY_SIZE);
					for (kInt j = 0; j < min(POLY_SIZE, pMesh->GetPolygonSize(i)); j++)
					{
						kInt lControlPointIndex = pMesh->GetPolygonVertex(i, j);
						(*subsets)[combIndex].indices.push_back(processIndex(lControlPointIndex));
					}
				}
			}
		}

		assert(nextCombinationIndex <= combinationIndices.size());
		materials.resize(combinationIndices.size());
		if (subsets)
			subsets->resize(combinationIndices.size());
	}
}

void processMesh(KFbxNode* pNode)
{
	std::string nodeName = pNode->GetName();
	KFbxMesh* pMesh = (KFbxMesh*)pNode->GetNodeAttribute();

	if(gLWMode)
	{
		std::string resourceName = nodeBaseName(nodeName) + ".lwo";

		unsigned int failId = 0;
		int failpos = 0;
		lwObject* lwo = lwGetObject(const_cast<char*>(resourceName.c_str()), &failId, &failpos);
		lwLayer* layer = lwo->layer;
		for(; layer; layer = layer->next)
		{
			std::string layerName = (layer->name)? std::string("_") + std::string(layer->name): "";

			OutputScene::Actor newActor;
			newActor.mesh = &processMeshResource(layer, nodeBaseName(nodeName) + layerName);
			processMaterials(pMesh, newActor.materials);	// $TBD: support multiple layers
			assertWarning(newActor.mesh->subsets.size() == newActor.materials.size(), "Mesh subset count and actor material count doesn't match.");
			processProperties(pNode, newActor.properties);
			newActor.node = pNode;
	
			gOutputScene.actors.push_back(newActor);
		}
	}
	else
	{
		OutputScene::Actor newActor;
		newActor.mesh = &processMeshResource(pNode);
		processMaterials(pMesh, newActor.materials);
		assertWarning(newActor.mesh->subsets.size() == newActor.materials.size(), "Mesh subset count and actor material count doesn't match.");
		processProperties(pNode, newActor.properties);
		newActor.node = pNode;

		gOutputScene.actors.push_back(newActor);
	}
}

void processProperties(KFbxObject const* pObject, OutputScene::Properties& properties)
{
	OutputScene::Properties::VectorPropertyMapT vectorProps = properties.vectors;
	OutputScene::Properties::StringPropertyMapT stringProps = properties.strings;

	int count = pObject->GetPropertyCount();
	for (int i=0; i<count; i++)
	{
		KFbxUserProperty lProperty = pObject->GetProperty(i);
		if (!lProperty.GetFlag(KFbxUserProperty::eUSER))
			continue; // process only user properties

		std::string propName(lProperty.GetLabel().Buffer());
		EFbxType propType = lProperty.GetPropertyDataType().GetType();

		switch(propType)
		{
		// scalars/vectors
		case eBOOL1:
			vectorProps[propName].resize(1);
			vectorProps[propName][0] = static_cast<double>(KFbxGet<bool>(lProperty));
			break;

		case eINTEGER1:
		case eFLOAT1:
		case eDOUBLE1:
			vectorProps[propName].resize(1);
			vectorProps[propName][0] = KFbxGet<double>(lProperty);
			break;

		case eDOUBLE3:
			vectorProps[propName].resize(3);
			for(int q = 0; q < 3; ++q)
				vectorProps[propName][q] = KFbxGet<fbxDouble3>(lProperty)[q];
			break;
		case eDOUBLE4:
			vectorProps[propName].resize(4);
			for(int q = 0; q < 4; ++q)
				vectorProps[propName][q] = KFbxGet<fbxDouble4>(lProperty)[q];
			break;
		case eDOUBLE44:
			vectorProps[propName].resize(16);
			for(int i = 0, q = 0; q < 4; ++q)
				for(int w = 0; w < 4; ++w)
					vectorProps[propName][i++] = KFbxGet<fbxDouble44>(lProperty)[q][w];
			break;

		// strings
		case eENUM:
			stringProps[propName] = std::string(lProperty.GetEnumValue(KFbxGet<int>(lProperty)));
			break;
		case eSTRING:
			stringProps[propName] = std::string(KFbxGet<KString>(lProperty).GetBuffer());
			break;

		// not supported
		case eTIME:
		case eREFERENCE:  // used as a port entry to reference object or properties
		case eDATASTREAM:  // used as a port entry to reference object or properties
			warning("Property type not supported");
			break;
		case eUNIDENTIFIED:
			warning("Property type not unidentified");
			break;
		}
	}
}

void applyProperties(OutputScene::Actor& actor, OutputScene::Properties const& properties)
{
	// $TBD: props
	// "shaderOverride"
}

OutputTexture& processTextureResource(KFbxTexture* pTexture)
{
	assert(pTexture);
	std::string resourceName = pTexture->GetRelativeFileName();

	if(gOutputScene.textureResources.find(resourceName) != gOutputScene.textureResources.end())
		return gOutputScene.textureResources[resourceName];

	OutputTexture& result = gOutputScene.textureResources[resourceName];

	result.source = resourceName;
	result.name = fileName2TextureName(resourceName);
	result.parameters = pTexture;

	assert(gDxNullRefDevice);
	HRESULT hr = D3DXCreateTextureFromFile(gDxNullRefDevice, resourceName.c_str(), &result.data);
	assertWarning(SUCCEEDED(hr), std::string("Failed to load texture: ") + resourceName);

	return result;
}

std::string processShaderResource(KFbxSurfaceMaterial* pMaterial)
{
	assert(pMaterial);
	std::string resourceName = shaderBaseName(pMaterial->GetName());
	gOutputScene.shaders.insert(resourceName);
	return resourceName;
}

OutputSkinnedMesh& processMeshResource(KFbxNode* pNode)
{
	assert(pNode);
	std::string resourceName = pNode->GetName();

	if(gOutputScene.meshResources.find(resourceName) != gOutputScene.meshResources.end())
		return gOutputScene.meshResources[resourceName];

	OutputSkinnedMesh& result = gOutputScene.meshResources[resourceName];

	// fill result
	KFbxMesh* pMesh = (KFbxMesh*) pNode->GetNodeAttribute();
	assert(gFbxGeomConverter);
	// mesh triangulized => can safely assume that all polys == triangles
	KFbxMesh* pMeshTriangulated = gFbxGeomConverter->TriangulateMesh(pMesh);

	assert(pMesh->GetLayerCount() == pMeshTriangulated->GetLayerCount());

	kInt lControlPointsCount = pMesh->GetControlPointsCount();
	kInt lPolygonCount = pMeshTriangulated->GetPolygonCount();
	kInt lLinkCount = pMesh->GetLinkCount();

	result.clear();
	result.source = resourceName;
	result.name = nodeBaseName(resourceName);
	result.vertices.resize(lControlPointsCount);
	result.indices.resize(lPolygonCount*3);
	result.bones.resize(lLinkCount);

	// [FBX gather] vertices & normals
	{
		// $TBD: extract vertex colors from layer
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
	} // \[FBX gather] vertices & normals

	// [FBX gather] uv (single layer)
//	if(0)
	{
		KFbxMesh* pMeshWithUv = pMeshTriangulated;

		// $TBD: support multiple layers
		kInt const MAX_LAYER_COUNT = 1;
		for (kInt l = 0; l < min(MAX_LAYER_COUNT, pMeshWithUv->GetLayerCount()); l++)
		{
			KFbxLayerElementUV* leUV = pMeshWithUv->GetLayer(l)->GetUVs();
			if (!leUV)
				continue;

			for (kInt i = 0; i < pMeshWithUv->GetPolygonCount(); i++)
			{
				for (kInt j = 0; j < pMeshWithUv->GetPolygonSize(i); j++)
				{
					kInt lControlPointIndex = pMeshWithUv->GetPolygonVertex(i, j);

					int tmpId;
					KFbxVector2 uv;
					switch (leUV->GetMappingMode())
					{
					case KFbxLayerElement::eBY_CONTROL_POINT:
						switch (leUV->GetReferenceMode())
						{
						case KFbxLayerElement::eDIRECT:
							uv = leUV->GetDirectArray().GetAt(lControlPointIndex);
							break;
						case KFbxLayerElement::eINDEX_TO_DIRECT:
							tmpId = leUV->GetIndexArray().GetAt(lControlPointIndex);
							uv = leUV->GetDirectArray().GetAt(tmpId);
							break;
						default:
							break; // other reference modes not shown here!
						}
						break;

					case KFbxLayerElement::eBY_POLYGON_VERTEX:
						{
//						kInt tmpTextureUVIndex = pMeshWithUv->GetTextureUVIndex(i, j);
						switch (leUV->GetReferenceMode())
						{
						case KFbxLayerElement::eDIRECT:
							tmpId = leUV->GetIndexArray().GetAt(lControlPointIndex);
							uv = leUV->GetDirectArray().GetAt(tmpId);
							break;
						case KFbxLayerElement::eINDEX_TO_DIRECT:
//							tmpId = tmpTextureUVIndex;//leUV->GetIndexArray().GetAt(tmpTextureUVIndex);
							tmpId = leUV->GetIndexArray().GetAt(lControlPointIndex);
							uv = leUV->GetDirectArray().GetAt(tmpId);
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

					KFbxVector4 uvw(uv[0], 1.0f - uv[1], 0.0f, 0.0f);
					for(int w = 0; w < 3; ++w)
					{
						assert(lControlPointIndex < (kInt)result.vertices.size());
						result.vertices[lControlPointIndex].uvw[w] = static_cast<float>(uvw[w]);
					}
				}
			}
		}
	} // \[FBX gather] uv

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
	} // \[FBX gather] bone weights

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
	} // \[FBX gather] indices

	// [FBX gather] subsets
	{
		processSubsets(pMeshTriangulated, result.subsets);
	} // \[FBX gather] subsets

	// [FBX gather] bones
	{
		for (kInt i = 0; i < lLinkCount; i++)
		{
			KFbxXMatrix m;
			KFbxLink* lLink = pMesh->GetLink(i);
			assert(lLink);
			assert(lLink->GetLink());

			char const* boneName = lLink->GetLink()->GetName();
			assert(i < (kInt)result.bones.size());
			result.bones[i].name = boneName;

			processMatrix(
				result.bones[i].matrix.data,
				lLink->GetTransformLinkMatrix(m));

//			D3DXMATRIX inverseBoneMatrix(result.bones[i].matrix.data);
//			D3DXMatrixInverse(&inverseBoneMatrix, 0, &inverseBoneMatrix);
//			memcpy(result.bones[i].matrix.data, &inverseBoneMatrix, sizeof(float)*16);
		}
	} // \[FBX gather] bones

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
	result.source = resourceName;
	result.name = fileName2MeshName(resourceName);
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

void processCamera(KFbxNode* pNode)
{
	KFbxCamera* pCamera = (KFbxCamera*)pNode->GetNodeAttribute();
	gOutputScene.cameras.push_back(pCamera);
}

void processLight(KFbxNode* pNode)
{
	KFbxLight* pLight = (KFbxLight*)pNode->GetNodeAttribute();
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

//std::auto_ptr<mutant::anim_hierarchy> processHierarchy(KFbxScene* pScene)
void processHierarchy(mutant::anim_character& anim_char, KFbxScene* pScene)
{
	int i;
	KFbxNode* lRootNode = pScene->GetRootNode();

	std::auto_ptr<mutant::anim_hierarchy> animHierarchy(new mutant::anim_hierarchy());
	animHierarchy->setName(mutant::sTypeNames::HIERARCHY_DEFAULT);

	animHierarchy->push_back(mutant::anim_hierarchy::node_t("root"));
	int parentNodeIndex = animHierarchy->size()-1;

	for( i = 0; i < lRootNode->GetChildCount(); i++)
	{
		(*animHierarchy)[parentNodeIndex].add_child(
			processHierarchy(*animHierarchy, lRootNode->GetChild(i), 0));
	}
	mutant::anim_hierarchy::node_t::update_parent_all(*animHierarchy);

	anim_char.insert_hierarchy(animHierarchy);
//	return animHierarchy;
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
	KTime totalTime; if(lastKey > 0) totalTime = pCurve->KeyGetTimeAll(lastKey); else totalTime.SetSecondDouble(1.0/ANIM_SAMPLING_FREQ);
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


void test()
{
	std::string s;
	assert((s=withoutPath("dummy.txt")) == "dummy.txt");
	assert((s=withoutPath("/dummy.txt")) == "dummy.txt");
	assert((s=withoutPath("/dummy.txt/")) == "dummy.txt");
	assert((s=withoutPath("dummy.txt/")) == "dummy.txt");
	assert((s=withoutPath("\\dummy.txt")) == "dummy.txt");
	assert((s=withoutPath("dummy.txt\\")) == "dummy.txt");
	assert((s=withoutPath("\\dummy.txt\\")) == "dummy.txt");
	assert((s=withoutPath("a:/directory1/dummy.txt")) == "dummy.txt");
	assert((s=withoutPath("a:\\directory1\\dummy.txt")) == "dummy.txt");
	assert((s=withoutPath("a:\\directory1\\directory2\\dummy.txt")) == "dummy.txt");
	assert((s=withoutPath("a:\\directory1/directory2\\dummy.txt")) == "dummy.txt");
	assert((s=withoutPath("..\\directory2\\dummy.txt")) == "dummy.txt");
	assert((s=withoutPath("..\\directory2\\dummy.txt\\")) == "dummy.txt");
	assert((s=withoutPath("../directory2/dummy.txt/")) == "dummy.txt");
	assert((s=withoutPath("..\\directory2\\dummy.txt/////////////////")) == "dummy.txt");


	assert((s=nodeBaseName("hello")) == "hello");
	assert((s=nodeBaseName("hello_ () world")) == "hello_");
	assert((s=nodeBaseName("hello_ _(1)")) == "hello_ _");
	assert((s=nodeBaseName("hello__     (1)")) == "hello__");
	assert((s=nodeBaseName("(1) moo")) == "");
	assert((s=nodeBaseName("(0)")) == "");
	assert((s=nodeBaseName("    (0)")) == "");
	assert((s=nodeBaseName("hello(")) == "hello(");
	assert((s=nodeBaseName("x::hello(")) == "hello(");
	assert((s=nodeBaseName("x::y::hello(")) == "hello(");
	assert((s=nodeBaseName("x::y::hello(::)")) == "hello");
	assert((s=nodeBaseName("x::y::hello::(z)")) == "");


	assert((s=shaderBaseName("shader")) == "shader");
	assert((s=shaderBaseName("shader_")) == "shader");
	assert((s=shaderBaseName("shader_01")) == "shader");
	assert((s=shaderBaseName("shader_name_01")) == "shader_name");
}

void init(KFbxSdkManager* fbxSdkManager)
{
	assert(fbxSdkManager);

	gDxNullRefDevice = createDx9NullDevice();
	gFbxGeomConverter = fbxSdkManager->CreateKFbxGeometryConverter();
}

void setPlatform(Platform platform)
{
	gPlatform = platform;
}

void beginScene(char const* sceneFileName)
{
	gOutputScene.source = sceneFileName;
	gOutputScene.name = mutalisk::fileName2SceneName(std::string(sceneFileName));
	gOutputScene.animResource.insertCharacter("scene",
		std::auto_ptr<mutant::anim_character>(new mutant::anim_character()));
}

void processHierarchy(KFbxScene* pScene)
{
	processHierarchy(gOutputScene.animResource["scene"], pScene);
}

void processAnimation(KFbxScene* pScene)
{
	processAnimation(gOutputScene.animResource["scene"], pScene);
}

void endScene()
{
	mutalisk::data::scene sceneData;
	blit(gOutputScene, sceneData);
	save(sceneName2FileName(gOutputScene.name), sceneData);

	for(OutputScene::OutputMeshesT::const_iterator i = gOutputScene.meshResources.begin(); i != gOutputScene.meshResources.end(); ++i)
	{
		assert(i->first == i->second.name);
		if(gPlatform == Platform::DX9)
		{
			mutalisk::data::dx9_mesh meshData;
			blit(i->second, meshData);
			save(meshName2FileName(i->second.name), meshData);
		}
		else if(gPlatform == Platform::PSP)
		{
			mutalisk::data::psp_mesh meshData;
			blit(i->second, meshData);
			save(meshName2FileName(i->second.name), meshData);
		}
	}

	for(OutputScene::OutputTexturesT::const_iterator i = gOutputScene.textureResources.begin(); i != gOutputScene.textureResources.end(); ++i)
	{
		assert(fileName2TextureName(i->first) == i->second.name);
		if(!i->second.data)
			continue;

		if(gPlatform == Platform::DX9)
		{
			// i->second.parameters
			save(textureName2FileName(i->second.name), *i->second.data);
		}
		else if(gPlatform == Platform::PSP)
		{
			warning("Not implemented");
		}
	}

	save(sceneName2AnimFileName(gOutputScene.name), gOutputScene.animResource);
	gCurves.clear();
}

} // \namespace mutalisk
// \mutalisk stuff =====================================================================================

#endif