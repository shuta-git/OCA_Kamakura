#include "shader.h"

float4 main(PS_INPUT input) : SV_Target
{
	return input.color_;
}
