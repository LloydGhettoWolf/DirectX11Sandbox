#define STANDARD_VERTEX_INPUT

#include "VertexShaderDefines.hlsl"

StandardPixelInput main(StandardVertexInput vertIn)
{
	return  GenerateInput(vertIn);
}