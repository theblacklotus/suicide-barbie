#ifndef MUTALISK_DATA_SHADER_H_
#define MUTALISK_DATA_SHADER_H_

#include "common.h"
#include <string>


namespace mutalisk { namespace data
{

	struct shader_fixed
	{
		enum { Version = 0x0103 };

		Color			ambient;
		Color			diffuse;
		Color			specular;
		Color			emissive;

		typedef unsigned TextureIndexT;
		TextureIndexT	diffuseTexture;
		TextureIndexT	envmapTexture;

		float			uOffset;
		float			vOffset;
		float			transparency;

		enum FrameBufferOp {
			fboReplace,
			fboAdd,
			fboSub,
			fboLerp,
			fboMul,
			fboMadd,
			fboReject };
		enum ZBufferOp {
			zboNone = 0,
			zboRead = 1,
			zboWrite = 2,
			zboReadWrite = zboRead|zboWrite,
			zboTwoPassReadWrite = 5 };
		enum TexWrapOp {
			twoRepeat,
			twoClamp };

		FrameBufferOp	frameBufferOp;
		ZBufferOp		zBufferOp;
		TexWrapOp		xTexWrapOp;
		TexWrapOp		yTexWrapOp;

		Vec4			aux0;
		
		// default state
		shader_fixed();
	};

	struct shader
	{
		enum { Version = 0x0100 };

		struct Index
		{
			array<unsigned>			textures;
			array<unsigned>			matrices;
			array<unsigned>			vecs;
			array<unsigned>			floats;
			array<unsigned>			ints;
		};
		struct Data
		{
			typedef unsigned TextureIndexT;
			array<TextureIndexT>	textures;
			array<Mat16>			matrices;
			array<Vec4>				vecs;
			array<float>			floats;
			array<int>				ints;
		};
		Index	indices;
		Data	values;

		// default state
		shader();
	};

	// I/O
	template <typename In> In& operator>> (In& i, shader_fixed& data);
	template <typename Out> Out& operator<< (Out& o, shader_fixed const& data);
	template <typename In> In& operator>> (In& i, shader& data);
	template <typename Out> Out& operator<< (Out& o, shader const& data);

} // namespace data 
} // namespace mutalisk

#include "shader.inl"

#endif // MUTALISK_DATA_SHADER_H_
