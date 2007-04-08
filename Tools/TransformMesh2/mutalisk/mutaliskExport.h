#ifndef MUTALISK_EXPORT_H_
#define MUTALISK_EXPORT_H_

#include <fbxsdk.h>
#include <shlwapi.h>

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
#include <functional>

#include <mutalisk/mutalisk.h>
#include <mutalisk/types.h>
#include <mutalisk/utility.h>
#include <mutalisk/dx9/dx9.h>
#include <mutalisk/psp/psp.h>
#include <mutalisk/psp/pspXcompile.h>

#include <mutant/mutant.h>
#include <mutant/writer.h>
#include <mutant/io_factory.h>

#include <effects/library.h>

#include <lua/lua_include.h>
#include <lua/luaPlayer.h>
#include <lua/Properties.h>

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
		Vec3() { std::fill_n(data, 3, 0.0f); }
		float const& operator[](size_t i) const { return data[i]; }
		float& operator[](size_t i) { return data[i]; }
		float data[3];
	};
	struct Mat16 {
		Mat16() { std::fill_n(data, 16, 0.0f); }
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
		typedef std::vector<
			std::pair<BoneIndexT, float> > WeightsT;
		typedef DWORD ColorT;

		Vec3 pos;
		Vec3 normal;
		Vec3 uvw;
		ColorT color;

		WeightsT weights;
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
	bool hasVertexColor;
};

struct OutputSkinnedMesh : public BaseSkinnedMesh
{
	std::string	source;
	std::string name;

	Vec3 pivotOffset;

	// memory management
	~OutputSkinnedMesh() { clear(); }
	void clear()
	{
		vertices.resize(0);
		indices.resize(0);
		bones.resize(0);
		hasVertexColor = false;
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
		OutputTexture*			colorTexture;
		OutputTexture*			envmapTexture;
		unsigned				systemTextureIndex;
		KFbxSurfaceMaterial*	parameters;
		std::string				shader;
		//Shader*				shader;
		std::string				frameBufferOp;
		std::string				zBufferOp;
		std::string				xTexWrapOp;
		std::string				yTexWrapOp;
	};
	typedef std::vector<Material>						MaterialsT;

	struct Properties
	{
		template <typename T>
		struct CaseInsensitiveCompare : public std::binary_function<T, T, bool>
		{
			void tolowerArray(T& t) const
			{
				for(size_t q = 0; q < t.size(); ++q)
					t[q] = static_cast<T::value_type>(tolower(t[q]));
			}
			
			bool operator()(T const& l, T const& r) const
			{
				T tmpL = l; tolowerArray(tmpL);
				T tmpR = r; tolowerArray(tmpR);
				return (tmpL.compare(tmpR) < 0);
			}
		};

		typedef std::map<std::string, std::string,
			CaseInsensitiveCompare<std::string> >		StringPropertyMapT;
		typedef std::map<std::string, std::vector<double>,
			CaseInsensitiveCompare<std::string> >		VectorPropertyMapT;
		typedef std::map<std::string, std::vector<KFCurve*>,
			CaseInsensitiveCompare<std::string> >		CurvePropertyMapT;

		bool hasString(std::string const& byName) const { return (strings.find(byName) != strings.end()); }
		bool hasVector(std::string const& byName) const { return (vectors.find(byName) != vectors.end()); }
		bool hasCurve(std::string const& byName) const { return (curves.find(byName) != curves.end()); }

		StringPropertyMapT	strings;
		VectorPropertyMapT	vectors;
		CurvePropertyMapT	curves;
	};

	struct Actor
	{
		Actor() : mesh(0), transparency(0), node(0), active(true), slice(0) {}
		OutputSkinnedMesh*	mesh;
		MaterialsT			materials;
		float				transparency;
		Properties			properties;

		KFbxNode*			node;

		unsigned			active;
		unsigned			slice;
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
mutalisk::lua::PropertiesByNameT gProperties;
float gGlobalScale = 0.01f;
bool gEnableScalingPivots = true;

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
void processMaterials(KFbxMesh* pMesh, std::vector<OutputSkinnedMesh::IndexT> const& indices, 
					  OutputScene::MaterialsT& materials, OutputSkinnedMesh::SubsetsT* subsets = 0);
void processSubsets(KFbxMesh* pMesh, std::vector<OutputSkinnedMesh::IndexT> const& indices,
					OutputSkinnedMesh::SubsetsT& subsets);
void processProperties(KFbxNode const* pObject, OutputScene::Properties& properties);
void processLuaProperties(mutalisk::lua::Properties const& src, OutputScene::Properties& dst);
void processAnimatedProperties(KFbxNode* pNode, OutputScene::Properties& properties);
OutputTexture* processTextureResource(std::string resourceName);
OutputTexture* processTextureResource(KFbxTexture* pTexture);
std::string processShaderResource(KFbxSurfaceMaterial* pMaterial);
OutputSkinnedMesh& processMeshResource(KFbxNode* pNode);
OutputSkinnedMesh& processMeshResource(lwLayer* subset, std::string resourceName);
void processCamera(KFbxNode* pNode);
void processLight(KFbxNode* pLight);

int processHierarchy(mutant::anim_hierarchy& hierarchy, KFbxNode* pNode, int pDepth);
//std::auto_ptr<mutant::anim_hierarchy> processHierarchy(KFbxScene* pScene);
void processHierarchy(mutant::anim_character& anim_char, KFbxScene* pScene);
void processAnimation(mutant::anim_clip& clip, KFbxNode* pNode, KTimeSpan timeSpan);
void processAnimation(mutant::anim_character& anim_char, KFbxScene* pScene);
//std::auto_ptr<mutant::anim_bundle> processChannels(KFbxNode* pNode, KFbxTakeNode* pTakeNode, KFbxTakeNode* pDefaultTakeNode);
std::auto_ptr<mutant::anim_bundle> processChannels(KFbxNode* pNode, KFbxTakeNode* pTakeNode, KFbxTakeNode* pDefaultTakeNode, KTime from, KTime last);
CurveT processCurve(KFCurve *pCurve);
CurveT::DataT sampleCurve(KFCurve *pCurve, float time, float defaultValue);
float processDefaultCurve(KFCurve *pCurve);

void applyProperties(OutputScene::Actor& actor, OutputScene::Properties& properties);

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
	if (gPlatform == Platform::DX9)
		return withoutExtension(textureName) + ".png";
	else if (gPlatform == Platform::PSP)
		return withoutExtension(textureName) + ".mtx";
	else
		return textureName;
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
	mutant::mutant_writer mutWriter(mutant::writer_factory::createOutput(outputFileName(dstName)));//, writer_factory::PLAIN));
	mutWriter << data;
}

template <>
void save(std::string dstName, mutant::anim_character_set& data)
{
	mutant::mutant_writer mutWriter(mutant::writer_factory::createOutput(outputFileName(dstName)));
	mutWriter.write(data);
}


inline void convertTexture(const std::string& dstName, const std::string& srcName)
{
	char exeDir[MAX_PATH];
	GetModuleFileNameA(NULL, exeDir, MAX_PATH);
	PathRemoveFileSpec(exeDir);
	strcat_s(exeDir, sizeof(exeDir), "\\ImageConverter.exe");

	PROCESS_INFORMATION procInfo;
	STARTUPINFO startupInfo = { sizeof(startupInfo) };
	DWORD result = 0;
	std::string params = "\"" + std::string(exeDir) + "\" -8 -blend -timestamp " + srcName + " " + outputFileName(dstName);
	if (CreateProcessA(NULL, const_cast<char*>(params.c_str()), NULL, NULL, FALSE, CREATE_UNICODE_ENVIRONMENT, NULL, NULL, &startupInfo, &procInfo))
	{
		WaitForSingleObject(procInfo.hProcess, INFINITE);
		CloseHandle(procInfo.hThread);
		CloseHandle(procInfo.hProcess);
		GetExitCodeProcess(procInfo.hProcess, &result);
	}
	else
	{
		warning("Failed to launch:\n\t" + params);
	}
	if (result != 0)
	{
		warning("ImageConverter failed with " + result);
	}
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
			if(mesh.hasVertexColor)
				asDword[i++] = mesh.vertices[q].color;		
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
			if(mesh.hasVertexColor)
				asDword[i++] = mesh.vertices[q].color;		
			asFloat[i++] = mesh.vertices[q].uvw[0];
			asFloat[i++] = mesh.vertices[q].uvw[1];
		}
	}
}

void blitMesh(OutputSkinnedMesh const& mesh, mutalisk::data::dx9_mesh& data)
{
	data.fvfVertexDecl = 
		D3DFVF_XYZ |
		D3DFVF_NORMAL |
		((mesh.hasVertexColor)? D3DFVF_DIFFUSE: 0) |
		(D3DFVF_TEX1|D3DFVF_TEXCOORDSIZE2(0))
		;
	data.vertexStride = D3DXGetFVFVertexSize(data.fvfVertexDecl);
	data.skinInfo = 0;

	blitBaseMesh(mesh, data);
}

namespace {
	size_t guVertexSize(unsigned int vertexDecl, unsigned weights = 0)
	{
		unsigned elements = 0;
		if(vertexDecl & GU_VERTEX_32BITF) elements += 3;
		if(vertexDecl & GU_NORMAL_32BITF) elements += 3;
		if(vertexDecl & GU_TEXTURE_32BITF) elements += 2;
		if(vertexDecl & GU_COLOR_8888) elements += 1;
		if(vertexDecl & GU_WEIGHT_32BITF) elements += weights;
		return elements * sizeof(float);
	}
}

void blitMesh(OutputSkinnedMesh const& mesh, mutalisk::data::psp_mesh& data)
{
	data.vertexDecl =
		GU_TEXTURE_32BITF |
		((mesh.hasVertexColor)? GU_COLOR_8888: 0) |
		GU_NORMAL_32BITF |
		GU_VERTEX_32BITF
		;
	data.vertexStride = guVertexSize(data.vertexDecl);
	data.skinInfo = 0;

//	blitBaseMesh(mesh, data);
	data.vertexCount = mesh.vertices.size();
	data.vertexDataSize = data.vertexCount * data.vertexStride;
	data.vertexData = new byte[data.vertexDataSize];
	for(size_t q = 0; q < data.vertexCount; ++q)
	{
		float* asFloat = reinterpret_cast<float*>(data.vertexData + q * data.vertexStride);
		DWORD* asDword = reinterpret_cast<DWORD*>(data.vertexData + q * data.vertexStride);

		if(gLWMode)
		{
			int i = 0;
			asFloat[i++] = mesh.vertices[q].uvw[0];
			asFloat[i++] = mesh.vertices[q].uvw[1];
			if(mesh.hasVertexColor)
				asDword[i++] = mesh.vertices[q].color;
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
			if(mesh.hasVertexColor)
				asDword[i++] = mesh.vertices[q].color;
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
	size_t weightsPerVertex = getWeightsPerVertex(mesh);
	assert(weightsPerVertex <= 4);

	data.vertexCount = mesh.vertices.size();
	data.fvfVertexDecl = 
		D3DFVF_XYZB5 |
		D3DFVF_LASTBETA_UBYTE4 |
		D3DFVF_NORMAL |
		((mesh.hasVertexColor)? D3DFVF_DIFFUSE: 0) |
		(D3DFVF_TEX1|D3DFVF_TEXCOORDSIZE2(0))
		;
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
		DWORD* asDword = reinterpret_cast<DWORD*>(data.vertexData + q * data.vertexStride);

		int i = 0;
		if(gLWMode)
		{
			asFloat[i++] = mesh.vertices[q].pos[2]* 100.0f;
			asFloat[i++] = mesh.vertices[q].pos[0]* 100.0f;
			asFloat[i++] = mesh.vertices[q].pos[1]* 100.0f;
		}
		else
		{
			asFloat[i++] = mesh.vertices[q].pos[0];
			asFloat[i++] = mesh.vertices[q].pos[1];
			asFloat[i++] = mesh.vertices[q].pos[2];
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

		asFloat[i++] = mesh.vertices[q].normal[0];
		asFloat[i++] = mesh.vertices[q].normal[1];
		asFloat[i++] = mesh.vertices[q].normal[2];
		if(mesh.hasVertexColor)
			asDword[i++] = mesh.vertices[q].color;
		asFloat[i++] = mesh.vertices[q].uvw[0];
		asFloat[i++] = mesh.vertices[q].uvw[1];
	}
}

void blitSkinned(OutputSkinnedMesh const& mesh, mutalisk::data::psp_mesh& data)
{
	size_t weightsPerVertex = getWeightsPerVertex(mesh);
	assert(weightsPerVertex <= 4);

	data.vertexCount = mesh.vertices.size();
	data.vertexDecl = 
		GU_TEXTURE_32BITF |
		((mesh.hasVertexColor)? GU_COLOR_8888: 0) |
		GU_NORMAL_32BITF | 
		GU_VERTEX_32BITF
		;
	
	data.vertexStride = guVertexSize(data.vertexDecl);
	data.vertexDataSize = data.vertexCount * data.vertexStride;
	data.vertexData = new byte[data.vertexDataSize];

	data.weightStride = sizeof(float) * weightsPerVertex;
	data.weightDataSize = data.vertexCount * data.weightStride;
	data.weightData = new byte[data.weightDataSize];

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
		DWORD* asDword = reinterpret_cast<DWORD*>(data.vertexData + q * data.vertexStride);

		float* asFloatW = reinterpret_cast<float*>(data.weightData + q * data.weightStride);
		byte* asByte = reinterpret_cast<byte*>(data.boneIndexData + q * data.boneIndexStride);

		for(size_t w = 0; w < weightsPerVertex; ++w)
		{
			float weight = getBoneWeight(mesh, q, w).second; // weight
			unsigned short boneIndex = getBoneWeight(mesh, q, w).first;	// boneIndex	

			asFloatW[w] = weight;
			assert(boneIndex < 256);
			asByte[w] = static_cast<byte>(boneIndex);
		}

		w = 0;
		if(gLWMode)
		{
			asFloat[w++] = mesh.vertices[q].uvw[0];
			asFloat[w++] = mesh.vertices[q].uvw[1];
			if(mesh.hasVertexColor)
				asDword[w++] = mesh.vertices[q].color;
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
			if(mesh.hasVertexColor)
				asDword[w++] = mesh.vertices[q].color;
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

namespace
{
	template <typename ValueType, typename Comparator = OutputScene::Properties::CaseInsensitiveCompare<std::string> >
	struct ValueByName
	{
		typedef ValueType									ValueT;
		typedef std::map<std::string, ValueT, Comparator>	MapT;
		MapT	m;
		ValueT	defaultValue;

		ValueType operator() (std::string const& str)
		{
			MapT::const_iterator it = m.find(str);
			if(it == m.end())
				return defaultValue;
			return it->second;
		}
	};

	struct FrameBufferOpByName : ValueByName<mutalisk::data::shader_fixed::FrameBufferOp>
	{
		FrameBufferOpByName()
		{
			defaultValue	= mutalisk::data::shader_fixed::fboReplace;
			m["Replace"]	= mutalisk::data::shader_fixed::fboReplace;
			m["Add"]		= mutalisk::data::shader_fixed::fboAdd;
			m["Sub"]		= mutalisk::data::shader_fixed::fboSub;
			m["Lerp"]		= mutalisk::data::shader_fixed::fboLerp;
			m["Mul"]		= mutalisk::data::shader_fixed::fboMul;
			m["Madd"]		= mutalisk::data::shader_fixed::fboMadd;
			m["Reject"]		= mutalisk::data::shader_fixed::fboReject;
		}
	};
	struct ZBufferOpByName : ValueByName<mutalisk::data::shader_fixed::ZBufferOp>
	{
		ZBufferOpByName()
		{
			defaultValue			= mutalisk::data::shader_fixed::zboReadWrite;
			m["ReadWrite"]			= mutalisk::data::shader_fixed::zboReadWrite;
			m["ReadOnly"]			= mutalisk::data::shader_fixed::zboRead;
			m["WriteOnly"]			= mutalisk::data::shader_fixed::zboWrite;
			m["None"]				= mutalisk::data::shader_fixed::zboNone;
			m["TwoPassReadWrite"]	= mutalisk::data::shader_fixed::zboTwoPassReadWrite;
		}
	};
	struct TexWrapOpByName : ValueByName<mutalisk::data::shader_fixed::TexWrapOp>
	{
		TexWrapOpByName()
		{
			defaultValue		= mutalisk::data::shader_fixed::twoClamp;
			m["Clamp"]			= mutalisk::data::shader_fixed::twoClamp;
			m["Repeat"]			= mutalisk::data::shader_fixed::twoRepeat;
		}
	};
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
	data.textureIds.resize(scene.textureResources.size() + mutalisk::effects::nSystemTexture_Count);
	for( ;q < mutalisk::effects::nSystemTexture_Count; ++q)
		data.textureIds[q] = "sys_texture";
	for(OutputScene::OutputTexturesT::const_iterator i = scene.textureResources.begin(); i != scene.textureResources.end(); ++i, ++q)
	{
		data.textureIds[q] = textureName2FileName(i->second.name);
	}

	data.shaderLibraryVersion =	mutalisk::effects::version();

/*	q = 0;
	data.shaderIds.resize(scene.shaders.size());
	for(OutputScene::ShadersT::const_iterator i = scene.shaders.begin(); i != scene.shaders.end(); ++i, ++q)
	{
		data.shaderIds[q] = *i;
	}*/

	// lights
	q = 0;
	data.lights.resize(scene.lights.size());
	for(OutputScene::LightsT::const_iterator i = scene.lights.begin(); i != scene.lights.end(); ++i, ++q)
	{
		assert(*i);
		assert((*i)->GetNode());
		assert((*i)->GetAttributeType() == KFbxNodeAttribute::eLIGHT);
		char const* name = (*i)->GetNode()->GetName();

		// node
		data.lights[q].id = q;
		data.lights[q].nodeName = name;
		data.lights[q].active = true;
		processMatrix(data.lights[q].worldMatrix.data, (*i)->GetNode()->GetGlobalFromDefaultTake());

		// properties
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
		if(properties.hasString("typeExt"))
		{
			if(properties.strings["typeExt"] == "DirectionalExt")
			{
				std::string const BackColor = "backColor";
				std::string const EquatorColor = "equatorColor";
				assertWarning(properties.hasVector(BackColor), "DirectionalExt light type misses 'Back Color' property");
				assertWarning(properties.hasVector(EquatorColor), "DirectionalExt light type misses 'Equator Color' property");

				assertWarning(properties.vectors[BackColor].size() >= 3, "'Back Color' property must be Vector3 or Vector4");
				assertWarning(properties.vectors[EquatorColor].size() >= 3, "'Equator Color' property must be Vector3 or Vector4");

				data.lights[q].type = mutalisk::data::scene::Light::DirectionalExt;

				if(properties.vectors[EquatorColor].size() >= 3)
				{
					data.lights[q].ambient.r = static_cast<float>(properties.vectors[EquatorColor][0]);
					data.lights[q].ambient.g = static_cast<float>(properties.vectors[EquatorColor][1]);
					data.lights[q].ambient.b = static_cast<float>(properties.vectors[EquatorColor][2]);
				}
				if(properties.vectors[EquatorColor].size() >= 4)
					data.lights[q].ambient.a = static_cast<float>(properties.vectors[EquatorColor][3]);

				if(properties.vectors[BackColor].size() >= 3)
				{
					data.lights[q].diffuseAux0.r = static_cast<float>(properties.vectors[BackColor][0]);
					data.lights[q].diffuseAux0.g = static_cast<float>(properties.vectors[BackColor][1]);
					data.lights[q].diffuseAux0.b = static_cast<float>(properties.vectors[BackColor][2]);
				}
				if(properties.vectors[BackColor].size() >= 4)
					data.lights[q].diffuseAux0.a = static_cast<float>(properties.vectors[BackColor][3]);
			}
		}
		else
		{
			data.lights[q].ambient.r = data.lights[q].ambient.g = data.lights[q].ambient.b = 0.0f;
			data.lights[q].ambient.a = 1.0f;
		}

		KFbxColor lColor; (*i)->GetDefaultColor(lColor);
		float intensity = static_cast<float>((*i)->GetDefaultIntensity()) * 0.01f;
		data.lights[q].diffuse.r = static_cast<float>(lColor.mRed) * intensity;		
		data.lights[q].diffuse.g = static_cast<float>(lColor.mGreen) * intensity;
		data.lights[q].diffuse.b = static_cast<float>(lColor.mBlue) * intensity;		
		data.lights[q].diffuse.a = static_cast<float>(lColor.mAlpha) * intensity;

		std::string const DiffuseColor = "diffuseColor";
		if(properties.hasVector(DiffuseColor))
		{
			assertWarning(properties.vectors[DiffuseColor].size() >= 3, "'Diffuse Color' property must be Vector3 or Vector4");
			if(properties.vectors[DiffuseColor].size() >= 3)
			{
				data.lights[q].diffuse.r = static_cast<float>(properties.vectors[DiffuseColor][0]);
				data.lights[q].diffuse.g = static_cast<float>(properties.vectors[DiffuseColor][1]);
				data.lights[q].diffuse.b = static_cast<float>(properties.vectors[DiffuseColor][2]);
			}
			if(properties.vectors[DiffuseColor].size() >= 4)
				data.lights[q].diffuse.a = static_cast<float>(properties.vectors[DiffuseColor][3]);
		}

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

		// node
		char const* name = (*i)->GetNode()->GetName();
		data.cameras[q].id = q;
		data.cameras[q].nodeName = name;
		data.cameras[q].active = true;
		processMatrix(data.cameras[q].worldMatrix.data, (*i)->GetNode()->GetGlobalFromDefaultTake());

		// camera
		assert((*i)->GetAttributeType() == KFbxNodeAttribute::eCAMERA ||
			(*i)->GetAttributeType() == KFbxNodeAttribute::eCAMERA_SWITCHER);

		double const constAspect = 480.0 / 272.0;
		double fovy = 30.0f;
		switch((*i)->GetApertureFormat())
		{
		case KFbxCamera::eHORIZONTAL:
			fovy = static_cast<float>((*i)->GetDefaultFieldOfView() / constAspect);
			break;
		case KFbxCamera::eVERTICAL:
			fovy = static_cast<float>((*i)->GetDefaultFieldOfView());
			break;
		case KFbxCamera::eHORIZONTAL_AND_VERTICAL:
		case KFbxCamera::eFOCAL_LENGTH:
			fovy = (*i)->ComputeFieldOfView((*i)->GetDefaultFocalLength()) / constAspect;
			break;
		}
		data.cameras[q].fov = static_cast<float>(fovy);
		data.cameras[q].aspect = static_cast<float>(constAspect);
		data.cameras[q].background.r = static_cast<float>((*i)->GetDefaultBackgroundColor().mRed);
		data.cameras[q].background.g = static_cast<float>((*i)->GetDefaultBackgroundColor().mGreen);
		data.cameras[q].background.b = static_cast<float>((*i)->GetDefaultBackgroundColor().mBlue);
		data.cameras[q].background.a = static_cast<float>((*i)->GetDefaultBackgroundColor().mAlpha);
	}

	// actors
	data.actors.resize(scene.actors.size());
	q = 0;
	for(OutputScene::ActorsT::const_iterator i = scene.actors.begin(); i != scene.actors.end(); ++i, ++q)
	{
		assert(i->node);
		assert(i->mesh);

		// node
		data.actors[q].id = q;
		data.actors[q].nodeName = scene.actors[q].node->GetName();
		data.actors[q].active = scene.actors[q].active;

		// mesh
		processMatrix(data.actors[q].worldMatrix.data, i->node->GetGlobalFromDefaultTake());
		size_t meshIndex = std::distance(scene.meshResources.begin(), scene.meshResources.find(i->mesh->source));
		assert(meshIndex < scene.meshResources.size());
		data.actors[q].meshIndex = meshIndex;

		// slice
		data.actors[q].slice = scene.actors[q].slice;

		// materials
		assert(i->materials.size() > 0);
		data.actors[q].materials.resize(i->materials.size());
		for(size_t w = 0; w < i->materials.size(); ++w)
		{
			{ // color texture
				size_t textureIndex = i->materials[w].systemTextureIndex;
				if(textureIndex == ~0U)
				{
					std::string textureName = "";
					if(i->materials[w].colorTexture)
						textureName = i->materials[w].colorTexture->source;

					textureIndex = std::distance(scene.textureResources.begin(), scene.textureResources.find(textureName));
					if(textureIndex == scene.textureResources.size())
						textureIndex = ~0U;
					else
						textureIndex += mutalisk::effects::nSystemTexture_Count;
				}

				data.actors[q].materials[w].shaderInput.diffuseTexture = textureIndex;
			}

			{ // envmap texture
				std::string textureName = "";
				if(i->materials[w].envmapTexture)
					textureName = i->materials[w].envmapTexture->source;

				size_t textureIndex = std::distance(scene.textureResources.begin(), scene.textureResources.find(textureName));
				if(textureIndex == scene.textureResources.size())
					textureIndex = ~0U;
				else
					textureIndex += mutalisk::effects::nSystemTexture_Count;

				data.actors[q].materials[w].shaderInput.envmapTexture = textureIndex;
			}

			{ // shader
				std::string shaderName = i->materials[w].shader;
				/*size_t shaderIndex = std::distance(scene.shaders.begin(), scene.shaders.find(shaderName));
				if(shaderIndex == scene.shaders.size())
					shaderIndex = ~0U;*/

				mutalisk::effects::IndexT shaderIndex = mutalisk::effects::getIndexByName(shaderName);
				if(shaderIndex == mutalisk::effects::NotFound)
					shaderIndex = mutalisk::effects::Default;
				data.actors[q].materials[w].shaderIndex = shaderIndex;

				if(i->materials[w].frameBufferOp != "")
				{
					static FrameBufferOpByName opByName;
					data.actors[q].materials[w].shaderInput.frameBufferOp = opByName(i->materials[w].frameBufferOp);
				}
				if(i->materials[w].zBufferOp != "")
				{
					static ZBufferOpByName opByName;
					data.actors[q].materials[w].shaderInput.zBufferOp = opByName(i->materials[w].zBufferOp);
				}
				if(i->materials[w].xTexWrapOp != "")
				{
					static TexWrapOpByName opByName;
					data.actors[q].materials[w].shaderInput.xTexWrapOp = opByName(i->materials[w].xTexWrapOp);
				}
				if(i->materials[w].yTexWrapOp != "")
				{
					static TexWrapOpByName opByName;
					data.actors[q].materials[w].shaderInput.yTexWrapOp = opByName(i->materials[w].yTexWrapOp);
				}

				data.actors[q].materials[w].shaderInput.transparency = i->transparency;
			}

			{ // surface properties
				mutalisk::data::Color& ambient = data.actors[q].materials[w].shaderInput.ambient;
				mutalisk::data::Color& diffuse = data.actors[q].materials[w].shaderInput.diffuse;
				mutalisk::data::Color& specular = data.actors[q].materials[w].shaderInput.specular;
				mutalisk::data::Color& emissive = data.actors[q].materials[w].shaderInput.emissive;

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

					lKFbxDouble3 = surface->GetEmissiveColor();
					emissive.r = static_cast<float>(lKFbxDouble3.Get()[0]);
					emissive.g = static_cast<float>(lKFbxDouble3.Get()[1]);
					emissive.b = static_cast<float>(lKFbxDouble3.Get()[2]);
					emissive.a = 1.0f;

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

					lKFbxDouble3 = surface->GetEmissiveColor();
					emissive.r = static_cast<float>(lKFbxDouble3.Get()[0]);
					emissive.g = static_cast<float>(lKFbxDouble3.Get()[1]);
					emissive.b = static_cast<float>(lKFbxDouble3.Get()[2]);
					emissive.a = 1.0f;
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

	dstMatrix[12] *= gGlobalScale;
	dstMatrix[13] *= gGlobalScale;
	dstMatrix[14] *= gGlobalScale;
}

void processSubsets(KFbxMesh* pMesh, std::vector<OutputSkinnedMesh::IndexT> const& indices, OutputSkinnedMesh::SubsetsT& subsets)
{
	OutputScene::MaterialsT tmpMaterials;
	processMaterials(pMesh, indices, tmpMaterials, &subsets);
}

void processMaterials(KFbxMesh* pMesh, std::vector<OutputSkinnedMesh::IndexT> const& indices, OutputScene::MaterialsT& materials, OutputSkinnedMesh::SubsetsT* subsets)
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
		materials.resize(materialCount * max(1, textureCount));
		if (subsets)
			subsets->resize(materialCount * max(1, textureCount));

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
				materials[combIndex].colorTexture = (lTexture)? processTextureResource(lTexture): 0;
				materials[combIndex].envmapTexture = 0;
				materials[combIndex].systemTextureIndex = ~0U;
				materials[combIndex].parameters = lMaterial;
				materials[combIndex].shader = (lMaterial)? processShaderResource(lMaterial): "";

				if (subsets)
				{
					kInt const POLY_SIZE = 3;
					assert(pMesh->GetPolygonSize(i) <= POLY_SIZE);
					for (kInt j = 0; j < min(POLY_SIZE, pMesh->GetPolygonSize(i)); j++)
					{
						OutputSkinnedMesh::IndexT index = indices[i*POLY_SIZE + j];
						(*subsets)[combIndex].indices.push_back(index);
						//kInt lControlPointIndex = pMesh->GetPolygonVertex(i, j);
						//(*subsets)[combIndex].indices.push_back(processIndex(lControlPointIndex));
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


KFbxVector4 getPivot(KFbxNode* pNode)
{
	if(!gEnableScalingPivots)
		return KFbxVector4(0, 0, 0, 0);

	assert(pNode);
	KFbxVector4 rotationPivot, scalingPivot;
	rotationPivot = pNode->GetRotationPivot(KFbxNode::eSOURCE_SET);
	scalingPivot = pNode->GetScalingPivot(KFbxNode::eSOURCE_SET);
	assertWarning(rotationPivot[0] == scalingPivot[0], "Scaling and rotation pivot must be the same");
	assertWarning(rotationPivot[1] == scalingPivot[1], "Scaling and rotation pivot must be the same");
	assertWarning(rotationPivot[2] == scalingPivot[2], "Scaling and rotation pivot must be the same");
	return scalingPivot;
}

void processMesh(KFbxNode* pNode)
{
	assert(pNode);
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
			processMaterials(pMesh, newActor.mesh->indices, newActor.materials);	// $TBD: support multiple layers
			assertWarning(newActor.mesh->subsets.size() == newActor.materials.size(), "Mesh subset count and actor material count doesn't match.");
			processProperties(pNode, newActor.properties);
			newActor.node = pNode;
	
			applyProperties(newActor, newActor.properties);
			gOutputScene.actors.push_back(newActor);
		}
	}
	else
	{
		OutputScene::Actor newActor;
		newActor.mesh = &processMeshResource(pNode);
		processMaterials(pMesh, newActor.mesh->indices, newActor.materials);
		assertWarning(newActor.mesh->subsets.size() == newActor.materials.size(), "Mesh subset count and actor material count doesn't match.");
		processProperties(pNode, newActor.properties);
		newActor.node = pNode;

		applyProperties(newActor, newActor.properties);
		gOutputScene.actors.push_back(newActor);
	}
}

void processProperties(KFbxNode const* pObject, OutputScene::Properties& properties)
{
	OutputScene::Properties::VectorPropertyMapT& vectorProps = properties.vectors;
	OutputScene::Properties::StringPropertyMapT& stringProps = properties.strings;

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
			vectorProps[propName].resize(4);
			for(int q = 0; q < 3; ++q)
				vectorProps[propName][q] = KFbxGet<fbxDouble3>(lProperty)[q];
			vectorProps[propName][3] = 0.0f;
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

	// process LUA properties for all parents and leaf node
	// properties stored in nodes closer to leaf will override if names collide
	static std::vector<KFbxNode*> nodes; nodes.resize(0);
	for(KFbxNode* node = const_cast<KFbxNode*>(pObject); node != 0; node = node->GetParent())
		nodes.push_back(node);

	for(std::vector<KFbxNode*>::const_reverse_iterator nodeIt = nodes.rbegin(); nodeIt != nodes.rend(); ++nodeIt)
	{
		assert(*nodeIt);
		std::string objectName = (*nodeIt)->GetName();
		mutalisk::lua::PropertiesByNameT::const_iterator propsIt = gProperties.find(objectName);
		if(propsIt != gProperties.end())
			processLuaProperties(propsIt->second, properties);
	}
}

void processLuaProperties(mutalisk::lua::Properties const& src, OutputScene::Properties& dst)
{
	for(mutalisk::lua::Properties::StringsT::const_iterator i = src.strings.begin(); i != src.strings.end(); ++i)
	{
		dst.strings[i->first] = i->second;
	}

	for(mutalisk::lua::Properties::VectorsT::const_iterator i = src.vectors.begin(); i != src.vectors.end(); ++i)
	{
/*		size_t size = 12;//i->second.size();
		std::string name = i->first;

		printf("Moo::::: %s %i\n", name.c_str(), size);
		mutalisk::array<double>& v = dst.vectors[name];
		printf("Moo::::: %i %x\n", v.size(), &v.front());
		v.resize(size);
		printf("Moo::::: %i %x\n", v.size(), &v.front());
		v.resize(0);
		printf("Moo::::: %i %x\n", v.size(), &v.front());
		v.resize(size);
		printf("Moo::::: %i %x\n", v.size(), &v.front());
		mutalisk::array<double> a(8);
		v = a;
		printf("Moo::::: %i %x\n", v.size(), &v.front());*/
		dst.vectors[i->first].resize(i->second.size());
		std::copy(i->second.begin(), i->second.end(), dst.vectors[i->first].begin());
	}

	for(mutalisk::lua::Properties::NumbersT::const_iterator i = src.numbers.begin(); i != src.numbers.end(); ++i)
	{
		dst.vectors[i->first].resize(1);
		dst.vectors[i->first][0] = i->second;
	}
}

void processAnimatedProperties(KFbxNode* pNode, OutputScene::Properties& properties)
{
	OutputScene::Properties::CurvePropertyMapT& curveProps = properties.curves;
    KFbxTakeNode* lCurrentTakeNode = pNode->GetCurrentTakeNode();
	
	// Do nothing if the current take node points to default values.
    if(!lCurrentTakeNode) return;
	if(lCurrentTakeNode == pNode->GetDefaultTakeNode()) return;

	int count = pNode->GetPropertyCount();
	for (int i=0; i<count; i++)
	{
		KFbxUserProperty lProperty = pNode->GetProperty(i);
		if (!lProperty.GetFlag(KFbxUserProperty::eUSER))
			continue; // process only user properties

		std::string propName(lProperty.GetLabel().Buffer());
		EFbxType propType = lProperty.GetPropertyDataType().GetType();

		if (!lCurrentTakeNode->GetPropertyAnimation(&lProperty, 0))
			continue;

		switch(propType)
		{
		// scalars/vectors
		case eBOOL1:
		case eINTEGER1:
		case eFLOAT1:
		case eDOUBLE1:
			curveProps[propName].resize(1);
			curveProps[propName][0] = lCurrentTakeNode->GetPropertyAnimation(&lProperty, 0);
			break;

		case eDOUBLE3:
			curveProps[propName].resize(3);
			for(int q = 0; q < 3; ++q)
				curveProps[propName][q] = lCurrentTakeNode->GetPropertyAnimation(&lProperty, q);
			break;
		case eDOUBLE4:
			curveProps[propName].resize(4);
			for(int q = 0; q < 4; ++q)
				curveProps[propName][q] = lCurrentTakeNode->GetPropertyAnimation(&lProperty, q);
			break;
		case eDOUBLE44:
			curveProps[propName].resize(16);
			for(int i = 0, q = 0; q < 4; ++q)
				for(int w = 0; w < 4; ++w, ++i)
					curveProps[propName][i] = lCurrentTakeNode->GetPropertyAnimation(&lProperty, i);
			break;

		// not supported
		case eENUM:
		case eSTRING:
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

namespace
{
fbxDouble3 readColorFromProperties(OutputScene::Properties& properties, std::string const& propName)
{
	fbxDouble3 color;
	if(properties.hasVector(propName) && !properties.vectors[propName].empty())
	{
		assert(properties.vectors[propName].size() >= 3);
		color[0] = properties.vectors[propName][0];
		color[1] = properties.vectors[propName][1];
		color[2] = properties.vectors[propName][2];
	}
	return color;
}
}

void applyProperties(OutputScene::Actor& actor, OutputScene::Properties& properties)
{
	if(properties.hasString("xTexWrap"))
	{
		std::string const& value = properties.strings["texWrapX"];
		for(size_t w = 0; w < actor.materials.size(); ++w)
			actor.materials[w].xTexWrapOp = value;
	}
	if(properties.hasString("yTexWrap"))
	{
		std::string const& value = properties.strings["texWrapY"];
		for(size_t w = 0; w < actor.materials.size(); ++w)
			actor.materials[w].xTexWrapOp = value;
	}
	if(properties.hasString("texWrap"))
	{
		std::string const& value = properties.strings["texWrap"];
		for(size_t w = 0; w < actor.materials.size(); ++w)
		{
			actor.materials[w].xTexWrapOp = value;
			actor.materials[w].yTexWrapOp = value;
		}
	}

	if(properties.hasString("systemMap"))
	{
		for(size_t w = 0; w < actor.materials.size(); ++w)
		{
			std::string const& textureName = properties.strings["systemMap"];
			actor.materials[w].systemTextureIndex = mutalisk::effects::getSystemTextureIndexByName(textureName);
		}
	}	
	if(properties.hasString("diffuseMap"))
	{
		for(size_t w = 0; w < actor.materials.size(); ++w)
		{
			std::string const& textureName = properties.strings["diffuseMap"];
			actor.materials[w].colorTexture = processTextureResource(textureName);
		}
	}
	if(properties.hasString("envMap"))
	{
		for(size_t w = 0; w < actor.materials.size(); ++w)
		{
			std::string const& textureName = properties.strings["envMap"];
			actor.materials[w].envmapTexture = processTextureResource(textureName);
		}
	}

	if(properties.hasString("frameOp") || properties.hasString("blendOp"))
	{
		std::string value = 
			properties.hasString("frameOp")? properties.strings["frameOp"]: properties.strings["blendOp"];

		for(size_t w = 0; w < actor.materials.size(); ++w)
			actor.materials[w].frameBufferOp = value;
	}

	if(properties.hasString("zOp") || properties.hasString("depthOp"))
	{
		std::string value =
			properties.hasString("zOp")? properties.strings["zOp"]: properties.strings["depthOp"];

		for(size_t w = 0; w < actor.materials.size(); ++w)
			actor.materials[w].zBufferOp = value;
	}

	if(properties.hasString("shader"))
	{
		for(size_t w = 0; w < actor.materials.size(); ++w)
		{
			std::string const& shaderName = properties.strings["shader"];
			gOutputScene.shaders.insert(shaderName);
			actor.materials[w].shader = shaderName;
		}
	}

	if(properties.hasString("active"))
	{
		if(properties.strings["active"] == "false")
			actor.active = false;
	}

	if(properties.hasVector("slice") && !properties.vectors["slice"].empty())
	{
		actor.slice = static_cast<unsigned>(properties.vectors["slice"][0]);
	}


	if(properties.hasVector("opacity"))
	{
		actor.transparency = 1.0f - static_cast<float>(properties.vectors["opacity"][0]);
	}
	if(properties.hasVector("transparency"))
	{
		actor.transparency = static_cast<float>(properties.vectors["transparency"][0]);
	}
	if(properties.hasVector("ambient") || properties.hasVector("diffuse") || properties.hasVector("specular") || properties.hasVector("emissive"))
	{
		for(size_t w = 0; w < actor.materials.size(); ++w)
		{
			KFbxPropertyDouble3 lKFbxDouble3;

			if(actor.materials[w].parameters->GetNewFbxClassId().Is(KFbxSurfaceLambert::ClassId))
			{
				KFbxSurfaceLambert* surface = static_cast<KFbxSurfaceLambert*>(actor.materials[w].parameters);
				if(properties.hasVector("ambient"))
					surface->GetAmbientColor().Set(readColorFromProperties(properties, "ambient"));
				if(properties.hasVector("diffuse"))
					surface->GetDiffuseColor().Set(readColorFromProperties(properties, "diffuse"));
				if(properties.hasVector("emissive"))
					surface->GetEmissiveColor().Set(readColorFromProperties(properties, "emissive"));
			}
			else if(actor.materials[w].parameters->GetNewFbxClassId().Is(KFbxSurfacePhong::ClassId))
			{
				KFbxSurfacePhong* surface = static_cast<KFbxSurfacePhong*>(actor.materials[w].parameters);
				if(properties.hasVector("ambient"))
					surface->GetAmbientColor().Set(readColorFromProperties(properties, "ambient"));
				if(properties.hasVector("diffuse"))
					surface->GetDiffuseColor().Set(readColorFromProperties(properties, "diffuse"));
				if(properties.hasVector("specular"))
					surface->GetSpecularColor().Set(readColorFromProperties(properties, "specular"));
				if(properties.hasVector("emissive"))
					surface->GetEmissiveColor().Set(readColorFromProperties(properties, "emissive"));
			}
		}
	}
}

OutputTexture* processTextureResource(std::string resourceName)
{
	if(gOutputScene.textureResources.find(resourceName) != gOutputScene.textureResources.end())
		return &gOutputScene.textureResources[resourceName];

	com_ptr<IDirect3DTexture9> textureData;
	assert(gDxNullRefDevice);
	HRESULT hr = D3DXCreateTextureFromFile(gDxNullRefDevice, resourceName.c_str(), &textureData);
	assertWarning(SUCCEEDED(hr), std::string("Failed to load texture: ") + resourceName);

	if(FAILED(hr))
		return 0;

	OutputTexture& result = gOutputScene.textureResources[resourceName];

	result.source = resourceName;
	result.name = fileName2TextureName(resourceName);
	result.parameters = 0;
	result.data = textureData;

	return &result;
}

OutputTexture* processTextureResource(KFbxTexture* pTexture)
{
	assert(pTexture);
	std::string resourceName = pTexture->GetRelativeFileName();
	OutputTexture* result = processTextureResource(resourceName);
	result->parameters = pTexture;
	return result;
}

std::string processShaderResource(KFbxSurfaceMaterial* pMaterial)
{
	assert(pMaterial);
	std::string resourceName = shaderBaseName(pMaterial->GetName());
	gOutputScene.shaders.insert(resourceName);
	return resourceName;
}

namespace
{
	void samplePosition(OutputSkinnedMesh::Vec3& outPos, KFbxMesh& mesh, int i, int j, int vertexId, KFbxVector4 pivot)
	{
		kInt controlPointIndex = mesh.GetPolygonVertex(i, j);
		KFbxVector4* lControlPoints = mesh.GetControlPoints();

		for(int w = 0; w < 3; ++w)
			outPos[w] = static_cast<float>(lControlPoints[controlPointIndex][w] - pivot[w]) * gGlobalScale;
	}
	void sampleNormal(OutputSkinnedMesh::Vec3& outNormal, KFbxMesh& mesh, KFbxLayerElementNormal* leVtxn, int i, int j, int vertexId)
	{
		int tmpId = -1;
		kInt controlPointIndex = mesh.GetPolygonVertex(i, j);

		KFbxVector4 normal(0, 1, 0);
		if(leVtxn && leVtxn->GetMappingMode() == KFbxLayerElement::eBY_CONTROL_POINT)
		{
			switch (leVtxn->GetReferenceMode())
			{
			case KFbxLayerElement::eDIRECT:
				normal = leVtxn->GetDirectArray().GetAt(controlPointIndex);
				break;
			case KFbxLayerElement::eINDEX_TO_DIRECT:
				tmpId = leVtxn->GetIndexArray().GetAt(controlPointIndex);
				assert(tmpId >= 0);
				normal = leVtxn->GetDirectArray().GetAt(tmpId);
				break;
			default:
				break; // other reference modes not shown here!
			}
		}
		else

			mesh.GetPolygonVertexNormal(i, j, normal);

		for(int w = 0; w < 3; ++w)
			outNormal[w] = static_cast<float>(normal[w]);
	}
	void sampleUv(OutputSkinnedMesh::Vec3& outUv, KFbxMesh& mesh, KFbxLayerElementUV* leUV, int i, int j, int vertexId)
	{
		int tmpId = -1;
		kInt controlPointIndex = mesh.GetPolygonVertex(i, j);
		kInt textureUVIndex = mesh.GetTextureUVIndex(i, j);

		KFbxVector2 uv; uv[0] = 0.0; uv[1] = 0.0;
		if(leUV)
		{
			switch (leUV->GetMappingMode())
			{
			case KFbxLayerElement::eBY_CONTROL_POINT:
				switch (leUV->GetReferenceMode())
				{
				case KFbxLayerElement::eDIRECT:
					uv = leUV->GetDirectArray().GetAt(controlPointIndex);
					break;
				case KFbxLayerElement::eINDEX_TO_DIRECT:
					tmpId = leUV->GetIndexArray().GetAt(controlPointIndex);
					assert(tmpId >= 0);
					uv = leUV->GetDirectArray().GetAt(tmpId);
					break;
				default:
					break; // other reference modes not shown here!
				}
				break;

			case KFbxLayerElement::eBY_POLYGON_VERTEX:
//				assert(textureUVIndex >= 0);
				switch (leUV->GetReferenceMode())
				{
				case KFbxLayerElement::eDIRECT:
					uv = leUV->GetDirectArray().GetAt(textureUVIndex);
					break;
				case KFbxLayerElement::eINDEX_TO_DIRECT:
					//tmpId = leUV->GetIndexArray().GetAt(textureUVIndex);
					//uv = leUV->GetDirectArray().GetAt(tmpId);
					if(textureUVIndex < 0)
						textureUVIndex = 0;
					uv = leUV->GetDirectArray().GetAt(textureUVIndex);
					break;
				default:
					break; // other reference modes not shown here!
				}
				break;

			case KFbxLayerElement::eBY_POLYGON: // doesn't make much sense for UVs
			case KFbxLayerElement::eALL_SAME:   // doesn't make much sense for UVs
			case KFbxLayerElement::eNONE:       // doesn't make much sense for UVs
				break;

			}
		}

		outUv[0] = static_cast<float>(uv[0]);
		outUv[1] = static_cast<float>(1.0 - uv[1]);
		outUv[2] = 0.0f;
	}
	void sampleColor(OutputSkinnedMesh::Vertex::ColorT& outColor, KFbxMesh& mesh, KFbxLayerElementVertexColor* leVtxc, int i, int j, int vertexId)
	{
		int tmpId = -1;
		kInt controlPointIndex = mesh.GetPolygonVertex(i, j);
		
		KFbxColor color(1.0, 1.0, 1.0);
		if(leVtxc)
		{
			switch (leVtxc->GetMappingMode())
			{
			case KFbxLayerElement::eBY_CONTROL_POINT:
				switch (leVtxc->GetReferenceMode())
				{
				case KFbxLayerElement::eDIRECT:
					color = leVtxc->GetDirectArray().GetAt(controlPointIndex);
					break;
				case KFbxLayerElement::eINDEX_TO_DIRECT:
					tmpId = leVtxc->GetIndexArray().GetAt(controlPointIndex);
					assert(tmpId >= 0);
					color = leVtxc->GetDirectArray().GetAt(tmpId);
					break;
				default:
					break; // other reference modes not shown here!
				}
				break;

			case KFbxLayerElement::eBY_POLYGON_VERTEX:
				switch (leVtxc->GetReferenceMode())
				{
				case KFbxLayerElement::eDIRECT:
					color = leVtxc->GetDirectArray().GetAt(i * 3 + j);
					break;
				case KFbxLayerElement::eINDEX_TO_DIRECT:
					tmpId = leVtxc->GetIndexArray().GetAt(i * 3 + j);
					color = leVtxc->GetDirectArray().GetAt(tmpId);
					break;
				default:
					break; // other reference modes not shown here!
				}
				break;

			case KFbxLayerElement::eBY_POLYGON: // doesn't make much sense for Colors
			case KFbxLayerElement::eALL_SAME:   // doesn't make much sense for Colors
			case KFbxLayerElement::eNONE:       // doesn't make much sense for Colors
				break;

			}
		}

		if(gPlatform == Platform::DX9)
		{
			 outColor = mutalisk::data::colorRGBAtoDWORD(
				static_cast<float>(color.mBlue),
				static_cast<float>(color.mGreen),
				static_cast<float>(color.mRed),
				static_cast<float>(color.mAlpha));
		}
		else
		{
			outColor = mutalisk::data::colorRGBAtoDWORD(
				static_cast<float>(color.mRed),
				static_cast<float>(color.mGreen),
				static_cast<float>(color.mBlue),
				static_cast<float>(color.mAlpha));
		}
	}
	void sampleWeights(OutputSkinnedMesh::Vertex::WeightsT& outWeights, KFbxMesh& mesh, int i, int j, int vertexId)
	{
		kInt lControlPointIndex = mesh.GetPolygonVertex(i, j);

		kInt lLinkCount = mesh.GetLinkCount();
		for(unsigned short q = 0; q < lLinkCount; ++q)
		{
			KFbxLink* lLink = mesh.GetLink(q);
			assert(lLink);
			assert(lLink->GetLink());

			kInt lIndexCount = lLink->GetControlPointIndicesCount();
			kInt* lIndices = lLink->GetControlPointIndices();
			kDouble* lWeights = lLink->GetControlPointWeights();

			for(int w = 0; w < lIndexCount; w++)
			{
				if(lControlPointIndex != lIndices[w])
					continue;

				unsigned short boneId = q;
				float boneWeight = static_cast<float>(lWeights[w]);
				if(boneWeight <= WEIGHT_EPSILON )
					continue;

				outWeights.push_back(std::make_pair(boneId, boneWeight));
			}
		}
	}

struct compareVertex
	: public std::binary_function<OutputSkinnedMesh::Vertex, OutputSkinnedMesh::Vertex, bool>
{
	bool operator()(const OutputSkinnedMesh::Vertex& a, const OutputSkinnedMesh::Vertex& b) const
	{
		// compare everything except bone weights
		return (memcmp(&a, &b, 
			sizeof(OutputSkinnedMesh::Vertex) - sizeof(OutputSkinnedMesh::Vertex::WeightsT)) < 0);
	}
};

typedef std::map<OutputSkinnedMesh::Vertex, kInt, compareVertex>	VertexMapT;
typedef std::vector<
	std::pair<OutputSkinnedMesh::Vertex, kInt> >					VertexMapT2;

	kInt insertVertexInMap(VertexMapT& vertexMap, kInt& uniqueId, OutputSkinnedMesh::Vertex const& v)
	{
		kInt index = 0;
		VertexMapT::const_iterator vIt = vertexMap.find(v);
		if(vIt == vertexMap.end())
		{
			index = uniqueId;
			vertexMap.insert(std::make_pair(v, uniqueId++));
		}
		else
			index = vIt->second;

		return index;
	}
	kInt insertVertexInMap(VertexMapT2& vertexMap, kInt& uniqueId, OutputSkinnedMesh::Vertex const& v)
	{
		kInt index = uniqueId;
		vertexMap.push_back(std::make_pair(v, uniqueId++));
		return index;
	}
}

OutputSkinnedMesh& processMeshResource(KFbxNode* pNode)
{
	assert(pNode);
	std::string resourceName = pNode->GetName();
	KFbxVector4 pivotOffset = getPivot(pNode);

	if(gOutputScene.meshResources.find(resourceName) != gOutputScene.meshResources.end())
	{
		OutputSkinnedMesh& result = gOutputScene.meshResources[resourceName];
		assert(result.pivotOffset[0] == pivotOffset[0]);
		assert(result.pivotOffset[1] == pivotOffset[1]);
		assert(result.pivotOffset[2] == pivotOffset[2]);
		return gOutputScene.meshResources[resourceName];
	}

	OutputSkinnedMesh& result = gOutputScene.meshResources[resourceName];
	result.pivotOffset[0] = static_cast<float>(pivotOffset[0]);
	result.pivotOffset[1] = static_cast<float>(pivotOffset[1]);
	result.pivotOffset[2] = static_cast<float>(pivotOffset[2]);

	// fill result
	KFbxMesh* pMesh = (KFbxMesh*) pNode->GetNodeAttribute();

	static bool forceComputeVertexNormals = false;
	if(forceComputeVertexNormals)
		pMesh->ComputeVertexNormals();

	assert(gFbxGeomConverter);
	// mesh triangulized => can safely assume that all polys == triangles
	KFbxMesh* pMeshTriangulated = gFbxGeomConverter->TriangulateMesh(pMesh);
//	KFbxMesh* pMeshTriangulated = pMesh;

	assert(pMesh->GetLayerCount() == pMeshTriangulated->GetLayerCount());

	kInt lPolygonCount = pMeshTriangulated->GetPolygonCount();
	kInt lLinkCount = pMesh->GetLinkCount();

	result.clear();
	result.source = resourceName;
	result.name = nodeBaseName(resourceName);
	result.indices.resize(lPolygonCount*3);
	result.bones.resize(lLinkCount);

	typedef VertexMapT	VertexMap;
	VertexMap vertexMap;
	// [FBX gather] mesh
	{
		// $TBD: support multiple layers
		KFbxLayerElementNormal* leVtxn = 0;
		KFbxLayerElementVertexColor* leVtxc = 0;
		KFbxLayerElementUV* leUV = 0;
		if(pMeshTriangulated->GetLayerCount() > 0)
		{
			leVtxn = pMeshTriangulated->GetLayer(0)->GetNormals();
			leVtxc = pMeshTriangulated->GetLayer(0)->GetVertexColors();
			leUV = pMeshTriangulated->GetLayer(0)->GetUVs();
		}

		if(leVtxc)
			result.hasVertexColor = true;

		kInt vertexId = 0;
		kInt uniqueId = 0;

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
			for (kInt j = 0; j < min(POLY_SIZE, lPolygonSize); j++, vertexId++)
			{
				OutputSkinnedMesh::Vertex v;
				samplePosition(v.pos,	*pMeshTriangulated, i, j			, vertexId, pivotOffset);
				sampleNormal(v.normal,	*pMeshTriangulated, leVtxn, i, j	, vertexId);
				sampleUv(v.uvw,			*pMeshTriangulated, leUV, i, j		, vertexId);
				sampleColor(v.color,	*pMeshTriangulated, leVtxc, i, j	, vertexId);
				sampleWeights(v.weights,*pMeshTriangulated, i, j			, vertexId);

				kInt index = insertVertexInMap(vertexMap, uniqueId, v);

				assert(i*POLY_SIZE + j < (int)result.indices.size());
				result.indices[i*POLY_SIZE + j] = processIndex(index);
			}
		}

		result.vertices.resize(vertexMap.size());
		for(VertexMap::const_iterator vIt = vertexMap.begin(); vIt != vertexMap.end(); ++vIt)
			result.vertices[processIndex(vIt->second)] = vIt->first;

	} // \[FBX gather] mesh


	// [FBX gather] subsets
	{
		processSubsets(pMeshTriangulated, result.indices, result.subsets);
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
				result.vertices[q].pos[w] = points[q].pos[w] * gGlobalScale;
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

void processAnimation(mutant::anim_clip& clip, KFbxNode* pNode, KTimeSpan timeSpan)
{
    KFbxTakeNode* lCurrentTakeNode = pNode->GetCurrentTakeNode();
	kInt lModelCount;

	KTime animStart, animStop;
//	lCurrentTakeNode->GetAnimationInterval(animStart, animStop);
	animStart = timeSpan.GetStart();
	animStop = timeSpan.GetStop();
	float clipLength = static_cast<float>(animStop.GetSecondDouble() - animStart.GetSecondDouble());
	clip.set_length(clipLength);

	// Do nothing if the current take node points to default values.
    if(lCurrentTakeNode)// && lCurrentTakeNode != pNode->GetDefaultTakeNode())
    {
		clip.insertBundle(pNode->GetName(), 
			processChannels(pNode, lCurrentTakeNode, pNode->GetDefaultTakeNode(), animStart, animStop));
	}

	for(lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
    {
        processAnimation(clip, pNode->GetChild(lModelCount), timeSpan);
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

		assert(lTakeNameArray.GetAt(i));
		KFbxTakeInfo const* takeInfo = pScene->GetTakeInfo(*lTakeNameArray.GetAt(i));
		assert(takeInfo);

		std::auto_ptr<mutant::anim_clip> clip(new mutant::anim_clip());
		processAnimation(*clip, pScene->GetRootNode(), takeInfo->mLocalTimeSpan);
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

float processDefaultCurve(KFCurve *pCurve)
{
	return static_cast<float> (pCurve->GetValue());
}

struct CurveSequentialSampler
{
	KFCurve *curve;
	float defaultValue;
	kFCurveIndex keyIndex;

	CurveSequentialSampler(KFCurve *c, float v) : curve(c), defaultValue(v), keyIndex(0) {}

	float operator() (float time)
	{
		if(this->curve->KeyGetCountAll() == 0)
			return this->defaultValue;

		KTime sampleTime; sampleTime.SetSecondDouble(time);
		return this->curve->Evaluate(sampleTime, &this->keyIndex);
	}
	float operator() (KTime time)
	{
		if(this->curve->KeyGetCountAll() == 0)
			return this->defaultValue;
		return this->curve->Evaluate(time, &this->keyIndex);
	}
};

std::auto_ptr<mutant::anim_bundle> processChannels(KFbxNode* pNode, KFbxTakeNode* pTakeNode, KFbxTakeNode* pDefaultTakeNode, KTime from, KTime last)
{
	// process transform curves.
	CurveSequentialSampler tx(pTakeNode->GetTranslationX(), processDefaultCurve(pDefaultTakeNode->GetTranslationX()));
	CurveSequentialSampler ty(pTakeNode->GetTranslationY(), processDefaultCurve(pDefaultTakeNode->GetTranslationY()));
	CurveSequentialSampler tz(pTakeNode->GetTranslationZ(), processDefaultCurve(pDefaultTakeNode->GetTranslationZ()));

	CurveSequentialSampler rx(pTakeNode->GetEulerRotationX(), processDefaultCurve(pDefaultTakeNode->GetEulerRotationX()));
	CurveSequentialSampler ry(pTakeNode->GetEulerRotationY(), processDefaultCurve(pDefaultTakeNode->GetEulerRotationY()));
	CurveSequentialSampler rz(pTakeNode->GetEulerRotationZ(), processDefaultCurve(pDefaultTakeNode->GetEulerRotationZ()));

	CurveSequentialSampler sx(pTakeNode->GetScaleX(), processDefaultCurve(pDefaultTakeNode->GetScaleX()));
	CurveSequentialSampler sy(pTakeNode->GetScaleY(), processDefaultCurve(pDefaultTakeNode->GetScaleY()));
	CurveSequentialSampler sz(pTakeNode->GetScaleZ(), processDefaultCurve(pDefaultTakeNode->GetScaleZ()));

	KFbxVector4 pivotOffset = getPivot(pNode);

	size_t const VQV_CURVE_ANIM_COMPONENTS = 3 + 4 + 3;
	typedef mutant::comp_quaternion_from_euler<Quat,float,float,float> t_comp_3_floats_to_quaternion;


	std::vector<float> keys;
	std::vector<float> values;

	{
		KTime deltaTime; deltaTime.SetSecondDouble(ANIM_SAMPLING_FREQ);
		KTime curTime = from;
		KTime totalTime = last;
		if(curTime == totalTime) totalTime += deltaTime;

		size_t keyCount = static_cast<size_t>(totalTime.GetSecondDouble()/ANIM_SAMPLING_FREQ+1);
		keys.reserve(keyCount);
		values.reserve(keyCount * VQV_CURVE_ANIM_COMPONENTS);

		for(; curTime < totalTime; curTime += deltaTime)
		{
			keys.push_back(static_cast<float>(curTime.GetSecondDouble()));

/*/
			KFbxVector4 localT = pNode->GetLocalTFromCurrentTake(curTime);
			KFbxVector4 localR = pNode->GetLocalRFromCurrentTake(curTime);
			KFbxVector4 localS = pNode->GetLocalSFromCurrentTake(curTime);

			KFbxVector4 rotateAroundX(localR[0], 0, 0);
			KFbxVector4 rotateAroundY(0, localR[1], 0);
			KFbxVector4 rotateAroundZ(0, 0, localR[2]);

			values.push_back(static_cast<float>(localT[0] + pivotOffset[0])* gGlobalScale);
			values.push_back(static_cast<float>(localT[1] + pivotOffset[1])* gGlobalScale);
			values.push_back(static_cast<float>(localT[2] + pivotOffset[2])* gGlobalScale);

			KFbxXMatrix r[3];
			r[0].SetR(rotateAroundX);
			r[1].SetR(rotateAroundY);
			r[2].SetR(rotateAroundZ);

			KFbxXMatrix matRotation;
			matRotation = r[2] * r[1] * r[0];

			Quat quat;
			KFbxQuaternion quatRotation = matRotation.GetQ();
			quat.x = static_cast<float>(quatRotation[0]);
			quat.y = static_cast<float>(quatRotation[1]);
			quat.z = static_cast<float>(quatRotation[2]);
			quat.w = static_cast<float>(quatRotation[3]);

			values.push_back(quat.x);
			values.push_back(quat.y);
			values.push_back(quat.z);
			values.push_back(quat.w);

			values.push_back(static_cast<float>(localS[0]));
			values.push_back(static_cast<float>(localS[1]));
			values.push_back(static_cast<float>(localS[2]));	
/*/
			// position
			values.push_back((tx(curTime) + static_cast<float>(pivotOffset[0]))* gGlobalScale);
			values.push_back((ty(curTime) + static_cast<float>(pivotOffset[1]))* gGlobalScale);
			values.push_back((tz(curTime) + static_cast<float>(pivotOffset[2]))* gGlobalScale);

			Quat quat;		
			if(gLWMode)
			{
				quat = t_comp_3_floats_to_quaternion()(
					deg2rad(ry(curTime)),
					deg2rad(rz(curTime)),
					deg2rad(rx(curTime)));
			}
			else
			{
				quat = t_comp_3_floats_to_quaternion()(
					deg2rad(rx(curTime)),
					deg2rad(ry(curTime)),
					deg2rad(rz(curTime)));
			}

			KFbxXMatrix preRotation;
			{
				KFbxVector4 lTmpVector;		
				lTmpVector = pNode->GetPreRotation(KFbxNode::eSOURCE_SET);

				KFbxVector4 rotateAroundX(lTmpVector[0], 0, 0);
				KFbxVector4 rotateAroundY(0, lTmpVector[1], 0);
				KFbxVector4 rotateAroundZ(0, 0, lTmpVector[2]);

				KFbxXMatrix r[3];
				r[0].SetR(rotateAroundX);
				r[1].SetR(rotateAroundY);
				r[2].SetR(rotateAroundZ);
				preRotation = r[2] * r[1] * r[0];
			}

			KFbxVector4 rotateAroundX(rx(curTime), 0, 0);
			KFbxVector4 rotateAroundY(0, ry(curTime), 0);
			KFbxVector4 rotateAroundZ(0, 0, rz(curTime));

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
			fbxRotationMatrix = preRotation * fbxRotationMatrix;

			KFbxQuaternion quatRotation = fbxRotationMatrix.GetQ();
			quat.x = static_cast<float>(quatRotation[0]);
			quat.y = static_cast<float>(quatRotation[1]);
			quat.z = static_cast<float>(quatRotation[2]);
			quat.w = static_cast<float>(quatRotation[3]);

			values.push_back(quat.x);
			values.push_back(quat.y);
			values.push_back(quat.z);
			values.push_back(quat.w);

			values.push_back(sx(curTime));
			values.push_back(sy(curTime));
			values.push_back(sz(curTime));
//*/
		}
	}

	gCurves.push_back(CurveT(keys, values, VQV_CURVE_ANIM_COMPONENTS));
	mutant::knot_data<float,float>& vqv = gCurves.back().data;

	std::auto_ptr<mutant::anim_bundle> animBundle(new mutant::anim_bundle());
	animBundle->insertData(mutant::sTypeNames::VEC_QUAT_VEC, vqv);

	// animated properties
	OutputScene::Properties properties;
	processAnimatedProperties(pNode, properties);
	for(OutputScene::Properties::CurvePropertyMapT::iterator propAnimIt = properties.curves.begin();
		propAnimIt != properties.curves.end(); ++propAnimIt)
	{
		// $TBD: support non-scalar properties
		assert(propAnimIt->second[0]);
		CurveSequentialSampler s(propAnimIt->second[0], 0.0f);

		std::vector<float> keys;
		std::vector<float> values;

		{
			KTime deltaTime; deltaTime.SetSecondDouble(ANIM_SAMPLING_FREQ);
			KTime curTime = from;
			KTime totalTime = last;
			if(curTime == totalTime) totalTime += deltaTime;

			size_t keyCount = static_cast<size_t>(totalTime.GetSecondDouble()/ANIM_SAMPLING_FREQ+1);
			keys.reserve(keyCount);
			values.reserve(keyCount);

			for(; curTime < totalTime; curTime += deltaTime)
			{
				keys.push_back(static_cast<float>(curTime.GetSecondDouble()));		
				values.push_back(s(curTime));
			}
		}

		gCurves.push_back(CurveT(keys, values, 1));
		mutant::knot_data<float,float>& vqv = gCurves.back().data;
		animBundle->insertData(propAnimIt->first, vqv);
	}

	return animBundle;
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
	// LUA
	std::string sceneProperties = mutalisk::fileName2SceneName(std::string(sceneFileName)) + ".props";
	mutalisk::lua::LuaPlayer::getInstance().exec(sceneProperties);
	mutalisk::lua::readFromResult(gProperties);
	mutalisk::lua::LuaPlayer::getInstance().garbageCollect();

	{
		OutputScene::Properties properties;
		mutalisk::lua::PropertiesByNameT::const_iterator propsIt = gProperties.find("RootNode");
		if(propsIt != gProperties.end())
			processLuaProperties(propsIt->second, properties);

		const std::string GlobalScale = "globalScale";
		if(properties.hasVector(GlobalScale))
		{
			if(properties.vectors[GlobalScale].size() > 0)
				gGlobalScale = static_cast<float>(properties.vectors[GlobalScale][0]);
		}
		
		const std::string EnableScalingPivots = "enableScalingPivots";
		if(properties.hasVector(EnableScalingPivots))
		{
			if(properties.vectors[EnableScalingPivots].size() > 0)
				gEnableScalingPivots = (properties.vectors[EnableScalingPivots][0] > 0);
		}
	}
	// \LUA

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

void saveScene(Platform platform)
{
	setPlatform(platform);

	mutalisk::data::scene sceneData;
	blit(gOutputScene, sceneData);
	save(sceneName2FileName(gOutputScene.name), sceneData);

	for(OutputScene::OutputMeshesT::const_iterator i = gOutputScene.meshResources.begin(); i != gOutputScene.meshResources.end(); ++i)
	{
		assert(i->first == i->second.source);
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
			convertTexture(textureName2FileName(i->second.name), i->first);
		}
	}

	save(sceneName2AnimFileName(gOutputScene.name), gOutputScene.animResource);
}

void endScene()
{
	gCurves.clear();
}

} // \namespace mutalisk
// \mutalisk stuff =====================================================================================

#endif
