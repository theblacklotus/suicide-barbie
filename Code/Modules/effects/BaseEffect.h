#ifndef MUTALISK_EFFECTS__BASE_EFFECT_H_
#define MUTALISK_EFFECTS__BASE_EFFECT_H_

#include "cfg.h"
#include "platform.h"
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
	enum nTexture {
		DiffuseTexture,
		MaxCount_nTexture
	};
	enum nVec {
		AmbientColor,
		DiffuseColor,
		SpecularColor,
		MaxCount_nVec
	};

public:
	virtual ~BaseEffect() {}

	struct Input;
	Input& allocInput();

	struct PassInfo;
	virtual unsigned begin() = 0;
	virtual PassInfo const& passInfo(unsigned passIndex) = 0;
	virtual void pass(unsigned passIndex) = 0;
	virtual void end() = 0;

	virtual void captureState() = 0;

	virtual void clearInput(Input& input);

public:
	struct PassInfo
	{
		enum nPassType {
			GenericPass,
			LightPass,
			SurfacePass,
			AssemblyPass
		};

		IVertexTransformer const*	vertexTransformer;
		nPassType					passType;

		PassInfo(IVertexTransformer const* v = 0, nPassType	p = GenericPass) 
			: vertexTransformer(v), passType(p) {}
	};

	struct Input
	{
		typedef std::pair<mutalisk::data::scene::Light const*, MatrixT>	LightInfoT;
		mutalisk::array<LightInfoT>	lights;

		mutalisk::array<TextureT const*>	textures;
		mutalisk::array<MatrixT const*>		matrices;
		mutalisk::array<VecT>				vecs;
		mutalisk::array<float>				floats;
		mutalisk::array<int>				ints;
	};

	struct Request
	{
		typedef std::vector<unsigned>	InputPortsT;
		struct OptionalInputs
		{
			InputPortsT	textures;
			InputPortsT	matrices;
		};
		struct RequiredInputs : OptionalInputs
		{
			InputPortsT	vecs;
			InputPortsT	floats;
			InputPortsT	ints;
		};

		OptionalInputs			optional;
		RequiredInputs			required;
		std::pair<int, int>		lightCountRange;
		bool					vertexStreamOuput;

		Request() : lightCountRange(std::make_pair(0, -1)), vertexStreamOuput(false) {}
	};

	Request const& getRequiredInputs() const { return mRequest; }
	static bool validateInput(Input const& input, Request const& request);
	static void allocInput(Input& input, Request const& request);

protected:
	Request	mRequest;
	Input	mInput;
};

} // namespace effects 
} // namespace mutalisk

#endif // MUTALISK_EFFECTS__BASE_EFFECT_H_
