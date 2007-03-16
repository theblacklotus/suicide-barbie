#include "shader.h"

namespace mutalisk { namespace data
{

// shader_fixed
//
shader_fixed::shader_fixed()
:	frameBufferOp(fboReplace),
	zBufferOp(zboReadWrite)
{
}
// shader
//
shader::shader()
{
}

} // namespace data 
} // namespace mutalisk
