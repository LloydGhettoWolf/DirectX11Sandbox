#define NORM_MAP_VERTEX_INPUT

#include "VertexShaderDefines.hlsl"

NormMapPixelInput main(NormMapVertexInput vertIn)
{
	return  GenerateInput(vertIn);
}