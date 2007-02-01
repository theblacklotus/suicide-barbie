#ifndef MUTALISK__VERTEX_TRANSFORMER_H_
#define MUTALISK__VERTEX_TRANSFORMER_H_

#include "cfg.h"
#include <vector>

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
		TexCoord7,
		Aux0,
		Aux1,
		Aux2,
		Aux3,
		Aux4,
		Aux5,
		Aux6,
		Aux7,
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

#endif // MUTALISK__VERTEX_TRANSFORMER_H_
