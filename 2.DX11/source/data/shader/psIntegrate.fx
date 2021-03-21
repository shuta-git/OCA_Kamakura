#include "shader.h"

float4 main(PS_INPUT input) : SV_Target
{
	float4	diffuse  = Texture0.Load(int3(input.position_.xy,0));
	float4	specular = Texture1.Load(int3(input.position_.xy,0));

	return float4(diffuse.rgb + specular.rgb, 1.0);
}
