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
float NormalizedDisneyDiffuse(float3 N, float3 L, float3 V, float roughness)
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
	return (FL * FV) / PI;
}

float Beckmann(float m, float t)
{
	float t2 = t * t;
	float t4 = t * t * t * t;
	float m2 = m * m;
    float D = 1.0f / (4.0f * m2 * t4);
	D *= exp( (-1.0f / m2) * (1.0f-t2)/ t2 );
	return D;
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
float3 BRDF(float3 L, float3 V, float3 N, float metaric)
{
	float microfacet = 0.3f;
	float f0 = metaric;
	// compute the half float3
	float3 H = normalize(L + V);

	float NdotH = max( 0.0f, dot(N, H) );
    float VdotH = max( 0.0f, dot(V, H) );
    float NdotL = max( 0.0f, dot(N, L) );
    float NdotV = max( 0.0f, dot(N, V) );

	float D = Beckmann(microfacet, NdotH);
	float F = spcFresnel(f0, VdotH);

	float G = min(1.0f, min(2*NdotH*NdotV/VdotH, 2*NdotH*NdotL/VdotH));
	float m = PI * NdotV * NdotH;

	return max(F * D * G / m, 0.0);
}