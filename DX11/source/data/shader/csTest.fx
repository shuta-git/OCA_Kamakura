
Texture2D<float4>	Texture   : register(t0);	// SRV
RWTexture2D<float4>	RWTexture : register(u0);	// UAV

// 共有メモリ (最大1024)
// groupshared float4	sharedMemory[1024];

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
[numthreads(32, 32, 1)]
void main(uint3 threadID : SV_DispatchThreadID)	// 左上からの計算済index
{
	int2	position = threadID.xy;		// ピクセル位置

	// ComputeShaderは早期にreturnすると次のスレッドが、より早く起動する
	if( position.x >= 128 ) return;
	if( position.y >= 128 ) return;

	// 半透明合成
	float4	targetColor = RWTexture[position];
	float4	color = Texture.Load(int3(position, 0));

	RWTexture[position] = lerp(color, targetColor, 0.5);
}
