
Texture2D<float4>	Texture   : register(t0);	// SRV
RWTexture2D<float4>	RWTexture : register(u0);	// UAV

// 共有メモリ (最大1024)
groupshared float4	sharedMemory[512];

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
[numthreads(256, 1, 1)]
void main(uint3 id       : SV_GroupThreadID,	// スレッドグループ内の通し番号
		  uint3 threadID : SV_DispatchThreadID)	// 左上からの計算済index
{
	uint2	position = threadID.xy;		// ピクセル位置

	// テクスチャの情報を取得
	uint	w;
	uint	h;
	uint	mipLevels;
	Texture.GetDimensions(0, w, h, mipLevels);

	// 有効なピクセル範囲か
	bool	valid = ( position.x < w ) && (position.y < h );

	//-------------------------------------------------------------
	// 共有メモリにテクスチャをロード
	//-------------------------------------------------------------
	static const uint MARGIN = 3;

	if( valid ) {
		uint	curr = id.x + MARGIN;	// 共有メモリの位置
	
		sharedMemory[curr] = Texture.Load(int3(position, 0));
		// 左端ピクセルの端数部分は端のスレッドが担当
		if( id.x < MARGIN ) {
			sharedMemory[id.x] = Texture.Load( int3(position-int2(MARGIN,0), 0));
		}
		// 右端ピクセル
		if( 256-MARGIN <= id.x ) {
			sharedMemory[curr+MARGIN] = Texture.Load(int3(position+int2(MARGIN,0), 0));
		}
	}

	//-------------------------------------------------------------
	// スレッドグループ内の実行を待機（これまでの処理が完了するまで全スレッド待機）
	//-------------------------------------------------------------
	GroupMemoryBarrierWithGroupSync();

	//-------------------------------------------------------------
	// ブラー実行
	//-------------------------------------------------------------
	if( valid ) {
		uint curr = id.x + MARGIN;	// 現在の共有メモリの位置
		
		float4	color = 0;
		color += sharedMemory[curr-3] * ( 1.0f/64.0f);
		color += sharedMemory[curr-2] * ( 6.0f/64.0f);
		color += sharedMemory[curr-1] * (15.0f/64.0f);
		color += sharedMemory[curr  ] * (20.0f/64.0f);
		color += sharedMemory[curr+1] * (15.0f/64.0f);
		color += sharedMemory[curr+2] * ( 6.0f/64.0f);
		color += sharedMemory[curr+3] * ( 1.0f/64.0f);
		
		RWTexture[position] = color;
	}
}
