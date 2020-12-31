#ifndef NEWAGE__EFFECT_H_
#define NEWAGE__EFFECT_H_

#include "cfg.h"


struct IVertexTransformer;
struct BaseEffect
{
	enum nMatrices {
		WorldMatrix,
		ViewMatrix,
		ViewProjMatrix,
		WorldViewProjMatrix,
		InvWorldMatrix,
		MaxCount
	};
	enum nTextures {
		DiffuseTexture,
		MaxCount
	};
	enum nVec {
		AmbientColor,
		DiffuseColor,
		SpecularColor,
		MaxCount
	};

public:
	virtual ~BaseEffect() {}

	struct Input;
	Input& allocInput();

	struct PassInfo;
	virtual unsigned begin() = 0;
	virtual PassInfo const& beginPass(unsigned passIndex) = 0;
	virtual void endPass() = 0;

	virtual void captureState() = 0;
	virtual void restoreState() = 0;

	virtual void clearInput(Input& input) = 0;

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

		PassInfo() : vertexTransformer(0), passType(GenericPass) {}
	};

	struct Input
	{
		typedef std::pair<data::scene::Light*, MatrixT>	LightInfoT;
		mutalisk::array<LightInfoT>	lights;

		mutalisk::array<TextureT*>	textures;
		mutalisk::array<MatrixT*>	matrices;
		mutalisk::array<Vec4>		vecs;
		mutalisk::array<float>		floats;
		mutalisk::array<int>		ints;
	};

	struct Request
	{
		struct OptionalInputs
		{
			std::vector<unsigned>	textures;
			std::vector<unsigned>	matrices;
		};
		struct RequiredInputs : OptionalInputs
		{
			std::vector<unsigned>	vecs;
			std::vector<float>		floats;
			std::vector<int>		ints;
		};

		OptionalInputs			optional;
		RequiredInputs			required;
		std::pair<int, int>		lightCountRange;
		bool					vertexStreamOuput;

		Request() : lightCountRange(make_pair(0, -1)), vertexStreamOuput(false) {}
	};

	Request const& getRequiredInputs() const { return mRequest; }
	static bool validateInput(Input const& input, Request const& request);
	static bool allocInput(Input& input, Request const& request);

protected:
	EffectRequest	mRequest;
	EffectInput		mInput;
};

/*class ActorRenderer
{
	setScene();

	setScene(data::scene const& scene);
	setLights(data::scene::Light const* lights, unsigned count);
	drawActor(data::scene::Actor const& actor, unsigned materialIt);
};*/

struct IVertexTransformer
{
	enum nVertexElement {
		Position,
		BoneWeights,
		BoneIndices,
		Normal,
		BiNormal,
		Tangent,
		Color0,
		Color1,
		TexCoord0,
		TexCoord1,
		TexCoord2,
		TexCoord3,
		TexCoord4,
		TexCoord5,
		TexCoord6,
		TexCoord7
	};

	struct VertexStream
	{
		nVertexElement type;
		unsigned elements;
	};
	struct VertexStreamIn : VertexStream
	{
		void const* data;
		unsigned stride;
	};
	struct VertexStreamOut : VertexStream
	{
		void* data;
		unsigned stride;
	};

	struct Request
	{
		std::vector<VertexStream>	inputs;
		std::vector<VertexStream>	outputs;
	};

	virtual ~IVertexTransformer() {}
	virtual void transform(
		VertexStreamIn const* input, unsigned inputStreamCount,
		VertexStreamOut const* ouput, unsigned outputStreamCount) const = 0;
};

#endif // NEWAGE__EFFECT_H_
