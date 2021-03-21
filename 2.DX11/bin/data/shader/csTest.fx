
Texture2D<float4>	Texture   : register(t0);	// SRV
RWTexture2D<float4>	RWTexture : register(u0);	// UAV

// ���L������ (�ő�1024)
// groupshared float4	sharedMemory[1024];

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
[numthreads(32, 32, 1)]
void main(uint3 threadID : SV_DispatchThreadID)	// ���ォ��̌v�Z��index
{
	int2	position = threadID.xy;		// �s�N�Z���ʒu

	// ComputeShader�͑�����return����Ǝ��̃X���b�h���A��葁���N������
	if( position.x >= 128 ) return;
	if( position.y >= 128 ) return;

	// ����������
	float4	targetColor = RWTexture[position];
	float4	color = Texture.Load(int3(position, 0));

	RWTexture[position] = lerp(color, targetColor, 0.5);
}
