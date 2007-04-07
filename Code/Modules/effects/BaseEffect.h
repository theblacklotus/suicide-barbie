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

public:
	virtual ~BaseEffect() {}

	struct Input;
	//Input& allocInput();
	static void clearInput(Input& input);

	struct PassInfo;
	virtual void begin() = 0;
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

			int				srcBlend;
			int				dstBlend;
		#if defined(MUTALISK_PSP)
			unsigned int	srcFix;
			unsigned int	dstFix;
		#endif
			int				xTexWrap;
			int				yTexWrap;

			VecT			aux0;
		};
		struct BufferControl
		{
			bool			colorWriteEnable;
			bool			alphaTestEnable;
			//bool			blendEnable;
			bool			zWriteEnable;
			bool			zReadEnable;
			bool			zEqual;
		};

		Lights lights;
		Surface const* surface;
		MatrixT const* matrices;
		BufferControl const* bufferControl;
		//mutalisk::array<MatrixT> const* matrices;
	};
};

} // namespace effects 
} // namespace mutalisk

#endif // MUTALISK_EFFECTS__BASE_EFFECT_H_
