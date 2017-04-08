#define NORM_MAPPING

#include "VertexShaderDefines.hlsl"

PixelInput main(VertexInput vertIn)
{
	return  GenerateInput(vertIn);
}