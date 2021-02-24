// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float2 TexCoords0      : TEXCOORD0 ;
	float  Fog             : TEXCOORD1 ;		// �t�H�O�p�����[�^
} ;

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0           : SV_TARGET0 ;	// �F
} ;


// �}�e���A���p�����[�^
struct DX_D3D11_CONST_MATERIAL
{
	float4		Diffuse ;				// �f�B�t���[�Y�J���[
	float4		Specular ;				// �X�y�L�����J���[
	float4		Ambient_Emissive ;		// �}�e���A���G�~�b�V�u�J���[ + �}�e���A���A���r�G���g�J���[ * �O���[�o���A���r�G���g�J���[

	float		Power ;					// �X�y�L�����̋���
	float		TypeParam0 ;			// �}�e���A���^�C�v�p�����[�^0
	float		TypeParam1 ;			// �}�e���A���^�C�v�p�����[�^1
	float		TypeParam2 ;			// �}�e���A���^�C�v�p�����[�^2
} ;

// �t�H�O�p�����[�^
struct DX_D3D11_VS_CONST_FOG
{
	float		LinearAdd ;				// �t�H�O�p�p�����[�^ end / ( end - start )
	float		LinearDiv ;				// �t�H�O�p�p�����[�^ -1  / ( end - start )
	float		Density ;				// �t�H�O�p�p�����[�^ density
	float		E ;						// �t�H�O�p�p�����[�^ ���R�ΐ��̒�

	float4		Color ;					// �J���[
} ;

// ���C�g�p�����[�^
struct DX_D3D11_CONST_LIGHT
{
	int			Type ;					// ���C�g�^�C�v( DX_LIGHTTYPE_POINT �Ȃ� )
	int3		Padding1 ;				// �p�f�B���O�P

	float3		Position ;				// ���W( �r���[��� )
	float		RangePow2 ;				// �L�������̂Q��

	float3		Direction ;				// ����( �r���[��� )
	float		FallOff ;				// �X�|�b�g���C�g�pFallOff

	float3		Diffuse ;				// �f�B�t���[�Y�J���[
	float		SpotParam0 ;			// �X�|�b�g���C�g�p�p�����[�^�O( cos( Phi / 2.0f ) )

	float3		Specular ;				// �X�y�L�����J���[
	float		SpotParam1 ;			// �X�|�b�g���C�g�p�p�����[�^�P( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )

	float4		Ambient ;				// �A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������

	float		Attenuation0 ;			// �����ɂ�錸�������p�p�����[�^�O
	float		Attenuation1 ;			// �����ɂ�錸�������p�p�����[�^�P
	float		Attenuation2 ;			// �����ɂ�錸�������p�p�����[�^�Q
	float		Padding2 ;				// �p�f�B���O�Q
} ;

// �s�N�Z���V�F�[�_�[�E���_�V�F�[�_�[���ʃp�����[�^
struct DX_D3D11_CONST_BUFFER_COMMON
{
	DX_D3D11_CONST_LIGHT		Light[ 6 ] ;			// ���C�g�p�����[�^
	DX_D3D11_CONST_MATERIAL		Material ;				// �}�e���A���p�����[�^
	DX_D3D11_VS_CONST_FOG		Fog ;					// �t�H�O�p�����[�^
} ;

// �萔�o�b�t�@�s�N�Z���V�F�[�_�[��{�p�����[�^
struct DX_D3D11_PS_CONST_BUFFER_BASE
{
	float4		FactorColor ;			// �A���t�@�l��

	float		MulAlphaColor ;			// �J���[�ɃA���t�@�l����Z���邩�ǂ���( 0.0f:��Z���Ȃ�  1.0f:��Z���� )
	float		AlphaTestRef ;			// �A���t�@�e�X�g�Ŏg�p�����r�l
	float2		Padding1 ;

	int			AlphaTestCmpMode ;		// �A���t�@�e�X�g��r���[�h( DX_CMP_NEVER �Ȃ� )
	int3		Padding2 ;

	float4		IgnoreTextureColor ;	// �e�N�X�`���J���[���������p�J���[
} ;

// ���_�V�F�[�_�[�E�s�N�Z���V�F�[�_�[���ʃp�����[�^
cbuffer cbD3D11_CONST_BUFFER_COMMON					: register( b0 )
{
	DX_D3D11_CONST_BUFFER_COMMON		g_Common ;
} ;

// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_PS_BASE				: register( b1 )
{
	DX_D3D11_PS_CONST_BUFFER_BASE		g_Base ;
} ;


SamplerState g_DiffuseMapSampler            : register( s0 ) ;		// �f�B�t���[�Y�}�b�v�e�N�X�`��
Texture2D    g_DiffuseMapTexture            : register( t0 ) ;		// �f�B�t���[�Y�}�b�v�e�N�X�`��


// main�֐�
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float4 TextureDiffuseColor ;

	// �e�N�X�`���J���[�̓ǂݍ���
	TextureDiffuseColor = g_DiffuseMapTexture.Sample( g_DiffuseMapSampler, PSInput.TexCoords0 ) ;

	// �o�̓J���[ = �e�N�X�`���J���[
	PSOutput.Color0 = TextureDiffuseColor ;

	// �t�H�O����
	PSOutput.Color0.rgb = lerp( g_Common.Fog.Color.rgb, PSOutput.Color0.rgb, saturate( PSInput.Fog ) );

	// �o�̓A���t�@ = �e�N�X�`���A���t�@ * �s�����x
	PSOutput.Color0.a = TextureDiffuseColor.a * g_Base.FactorColor.a ;

	// �o�̓p�����[�^��Ԃ�
	return PSOutput ;
}

