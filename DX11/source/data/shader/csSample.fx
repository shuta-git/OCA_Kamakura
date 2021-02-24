
Texture2D<float4>	Texture   : register(t0);	// SRV
RWTexture2D<float4>	RWTexture : register(u0);	// UAV

// ���L������ (�ő�1024)
// groupshared float4	sharedMemory[1024];

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
[numthreads(8, 4, 1)]
void main(uint3 threadID : SV_DispatchThreadID)	// ���ォ��̌v�Z��index
{
	int2	position = threadID.xy;		// �s�N�Z���ʒu

	float4	color = Texture.Load(int3(position, 0));

	RWTexture[position] = color;
}
