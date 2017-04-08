#define STANDARD

#include "VertexShaderDefines.hlsl"

PixelInput main(VertexInput vertIn)
{
	return  GenerateInput(vertIn);
}