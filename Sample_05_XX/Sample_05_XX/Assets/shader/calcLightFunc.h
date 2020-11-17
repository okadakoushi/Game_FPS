/// <summary>
/// ���C�g�v�Z�֐��B
/// </summary>

static const float PI = 3.14159265358979323846;	//�~�����B

//�t���l�����˗����v�Z�B
//param1 u = 
//param2 f0 = 
//param3 f90 = 
//return 
float SchlickFresnel(float u, float f0, float f90)
{
	return f0 + (f90 + f0) * pow(1.0f - u, 5.0f);
}

//���K���ς݊g�U���ˁB
//param1 baseColor = ���C�g�̃x�[�X�J���[
//param2 N = �@���B
//param3 L = ���C�g�̕����B
//param4 V = ���_�ւ̃x�N�g���B
//param5 roughness = �\�ʂ̑e��
//return 
float NormalizedDisneyDiffuse(float baseColor, float3 N, float3 L, float3 V, float roughness)
{
	//���C�g�����ƁA���_�ւ̃x�N�g���̃n�[�t�x�N�g���B
	float3 H = normalize(L + V);

	//�U���U���Ƃ�������̌v�Z�H
	//���̍�����I�Ȋ����H�u���[���݂����ȓz���ł�B
	float energyBias = lerp(0.0f, 0.5f, roughness);

	//Disney�g�U����BRDF�̓�����A1.0�ȏ�̒l��Ԃ����Ƃ�����̂ŁA
	//saturate(x) x�̒l��0�`1�ŃN�����v�i����j����B
	//�\�ʖ@���ƃ��C�g�x�N�g���̊Ԃ̊p�x���v�Z�B
	float dotLH = saturate(dot(L, H));
	float dotNL = saturate(dot(N, L));
	//�\�ʖ@���Ǝ����x�N�g���̊Ԃ̊p�x���v�Z�B
	float dotNV = saturate(dot(N, V));

	//�t���l�����˗��̌v�Z�B
	float Fd90 = energyBias + 2.0f * dotLH * dotLH * roughness;

	//�t���l�����˂������o�[�g���˂ɉ�����B
	float FL = SchlickFresnel(1.0f, Fd90, dotNL);
	float FV = SchlickFresnel(1.0f, Fd90, dotNV);
	return (baseColor * FL * FV) / PI;
}

float Beckmann(float m, float t)
{
	float M = m * m;
	float T = t * t;
	return exp((T - 1) / (M * T)) / (M * T * T);
}

float spcFresnel(float f0, float u)
{
	// from Schlick
	return f0 + (1 - f0) * pow(1 - u, 5);
}

//BRDF(�o�������˗����z�֐�)
//param1 L = ���C�g�̕����B
//param2 V = ���_�ւ̃x�N�g���B
//param3 N = �@���B
//return 
float3 BRDF(float3 L, float3 V, float3 N)
{
	float microfacet = 0.3;
	float f0 = 0.5;
	bool include_F = 0;
	bool include_G = 0;
	// compute the half float3
	float3 H = normalize(L + V);

	float NdotH = dot(N, H);
	float VdotH = dot(V, H);
	float NdotL = dot(N, L);
	float NdotV = dot(N, V);


	float D = Beckmann(microfacet, NdotH);
	float F = spcFresnel(f0, VdotH);

	float t = 2.0 * NdotH / VdotH;
	float G = max(0.0f, min(1.0, min(t * NdotV, t * NdotL)));
	float m = PI * NdotV * NdotL;

	return max(F * D * G / m, 0.0);
}