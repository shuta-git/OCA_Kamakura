
Texture2D<float4>	Texture   : register(t0);	// SRV
RWTexture2D<float4>	RWTexture : register(u0);	// UAV

// ���L������ (�ő�1024)
groupshared float4	sharedMemory[512];

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
[numthreads(256, 1, 1)]
void main(uint3 id       : SV_GroupThreadID,	// �X���b�h�O���[�v���̒ʂ��ԍ�
		  uint3 threadID : SV_DispatchThreadID)	// ���ォ��̌v�Z��index
{
	uint2	position = threadID.xy;		// �s�N�Z���ʒu

	// �e�N�X�`���̏����擾
	uint	w;
	uint	h;
	uint	mipLevels;
	Texture.GetDimensions(0, w, h, mipLevels);

	// �L���ȃs�N�Z���͈͂�
	bool	valid = ( position.x < w ) && (position.y < h );

	//-------------------------------------------------------------
	// ���L�������Ƀe�N�X�`�������[�h
	//-------------------------------------------------------------
	static const uint MARGIN = 3;

	if( valid ) {
		uint	curr = id.x + MARGIN;	// ���L�������̈ʒu
	
		sharedMemory[curr] = Texture.Load(int3(position, 0));
		// ���[�s�N�Z���̒[�������͒[�̃X���b�h���S��
		if( id.x < MARGIN ) {
			sharedMemory[id.x] = Texture.Load( int3(position-int2(MARGIN,0), 0));
		}
		// �E�[�s�N�Z��
		if( 256-MARGIN <= id.x ) {
			sharedMemory[curr+MARGIN] = Texture.Load(int3(position+int2(MARGIN,0), 0));
		}
	}

	//-------------------------------------------------------------
	// �X���b�h�O���[�v���̎��s��ҋ@�i����܂ł̏�������������܂őS�X���b�h�ҋ@�j
	//-------------------------------------------------------------
	GroupMemoryBarrierWithGroupSync();

	//-------------------------------------------------------------
	// �u���[���s
	//-------------------------------------------------------------
	if( valid ) {
		uint curr = id.x + MARGIN;	// ���݂̋��L�������̈ʒu
		
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
