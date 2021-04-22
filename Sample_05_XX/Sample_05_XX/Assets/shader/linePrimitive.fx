/*!
 * @brief	���C���v���~�e�B�u�p�̃V�F�[�_�[�B
 */

//���_�V�F�[�_�[�ւ̓��͒��_�\���́B
struct VSInput {
	float4 pos : POSITION;
};
//���_�V�F�[�_�[�̏o�́B
struct VSOutput {
	float4 pos : SV_POSITION;
};

cbuffer CB : register(b0) {
	float4x4 mView;
	float4x4 mProj;
};

//���_�B
VSOutput VSMain(VSInput In) : SV_POSITION
{
	VSOutput Out;
	float4 outPos = mul(mView, In.pos);
	outPos = mul(mProj, outPos);
	Out.pos = outPos;
	return Out;
}
//�s�N�Z���B
float4 PSMain(VSOutput vsOut) : SV_Target0
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}