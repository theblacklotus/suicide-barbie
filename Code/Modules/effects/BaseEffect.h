#ifndef MUTALISK_EFFECTS__BASE_EFFECT_H_
#define MUTALISK_EFFECTS__BASE_EFFECT_H_

#include "cfg.h"
#include "platform.h"
#include <mutalisk/arena.h>
#include <mutalisk/scene.h>

namespace mutalisk { namespace effects {

struct IVertexTransformer;
struct BaseEffect
{
	enum nMatrix {
		WorldMatrix,
		ViewMatrix,
		ProjMatrix,
		ViewProjMatrix,
		WorldViewProjMatrix,
		InvWorldMatrix,
		MaxCount_nMatrix
	};
	//enum nTexture {
	//	DiffuseTexture,
	//	EnvmapTexture,
	//	MaxCount_nTexture
	//};
	//enum nVec {
	//	AmbientColor,
	//	DiffuseColor,
	//	SpecularColor,
	//	MaxCount_nVec
	//};

public:
	virtual ~BaseEffect() {}

	struct Input;
	//Input& allocInput();
	static void clearInput(Input& input);

	struct PassInfo;
	virtual void begin() = 0;
	//virtual unsigned begin() = 0;
	//virtual PassInfo const& passInfo(unsigned passIndex) = 0;
	//virtual void pass(unsigned passIndex) = 0;
	virtual unsigned passCount(Input const& i) = 0;
	virtual PassInfo const& passInfo(Input const& i, unsigned passIndex) = 0;
	virtual void pass(Input const& i, unsigned passIndex) = 0;
	virtual void end() = 0;

	virtual void captureState() = 0;

public:
	struct PassInfo
	{
		enum nPassType {
			GenericPass,
			LightPass,
			SurfacePass,
			AssemblyPass
		};

		IVertexTransformer const*	rigidTransformer;
//		IVertexTransformer const*	skinnedTransformer;
		nPassType					passType;

		PassInfo(IVertexTransformer const* v = 0, nPassType	p = GenericPass) 
			: rigidTransformer(v), passType(p) {}
	};

	struct Input
	{
		//typedef std::pair<mutalisk::data::scene::Light const*, MatrixT>	LightInfoT;
		//struct Surface
		//{
		//	mutalisk::array<TextureT const*>	textures;
		//	mutalisk::array<MatrixT const*>		matrices;
		//	mutalisk::array<VecT>				vecs;
		//	mutalisk::array<float>				floats;
		//	mutalisk::array<int>				ints;
		//};

		struct Lights
		{
			LightT const*	data;
			MatrixT const*	matrices;
			size_t			count;
		};
		struct Surface
		{
			ColorT			ambient;
			ColorT			diffuse;
			ColorT			specular;
			ColorT			emissive;

			TextureT*		diffuseTexture;
			TextureT*		envmapTexture;

			float			uOffset;
			float			vOffset;
			float			transparency;
			float			dummy;

			VecT			aux0;
		};

		Lights lights;
		Surface const* surface;
		MatrixT const* matrices;
		//mutalisk::array<MatrixT> const* matrices;
	};

	//struct Request
	//{
	//	typedef std::vector<unsigned>	InputPortsT;
	//	struct OptionalInputs
	//	{
	//		InputPortsT	textures;
	//		InputPortsT	matrices;
	//	};
	//	struct RequiredInputs : OptionalInputs
	//	{
	//		InputPortsT	vecs;
	//		InputPortsT	floats;
	//		InputPortsT	ints;
	//	};
	//
	//	OptionalInputs			optional;
	//	RequiredInputs			required;
	//	std::pair<int, int>		lightCountRange;
	//	bool					vertexStreamOuput;
	//
	//	Request() : lightCountRange(std::make_pair(0, -1)), vertexStreamOuput(false) {}
	//};

	//Request const& getRequiredInputs() const { return mRequest; }
	//static bool validateInput(Input const& input, Request const& request);
	//static void allocInput(Input& input, Request const& request);

protected:
	//Request	mRequest;
	//arena	mInputArena;	// $TBD: shared
	//Input	mInputTemplate;
};

} // namespace effects 
} // namespace mutalisk

#endif // MUTALISK_EFFECTS__BASE_EFFECT_H_
