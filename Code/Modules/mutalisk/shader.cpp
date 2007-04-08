#include "shader.h"

namespace mutalisk { namespace data
{

// shader_fixed
//
shader_fixed::shader_fixed()
:	diffuseTexture(~0U)
,	envmapTexture(~0U)
,	uOffset(0.0f)
,	vOffset(0.0f)
,	uScale(1.0f)
,	vScale(1.0f)
,	transparency(0.0f)
,	frameBufferOp(fboReplace)
,	zBufferOp(zboReadWrite)
,	xTexWrapOp(twoClamp)
,	yTexWrapOp(twoClamp)
{
	ambient.r = ambient.g = ambient.b = ambient.a = 0.0f;
	diffuse.r = diffuse.g = diffuse.b = diffuse.a = 0.0f;
	specular.r = specular.g = specular.b = specular.a = 0.0f;
	emissive.r = emissive.g = emissive.b = emissive.a = 0.0f;
}
// shader
//
shader::shader()
{
}

} // namespace data 
} // namespace mutalisk
