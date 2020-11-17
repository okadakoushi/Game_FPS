/// <summary>
/// ライト計算関数。
/// </summary>

static const float PI = 3.14159265358979323846;	//円周率。

//フレネル反射率を計算。
//param1 u = 
//param2 f0 = 
//param3 f90 = 
//return 
float SchlickFresnel(float u, float f0, float f90)
{
	return f0 + (f90 + f0) * pow(1.0f - u, 5.0f);
}

//正規化済み拡散反射。
//param1 baseColor = ライトのベースカラー
//param2 N = 法線。
//param3 L = ライトの方向。
//param4 V = 視点へのベクトル。
//param5 roughness = 表面の粗さ
//return 
float NormalizedDisneyDiffuse(float baseColor, float3 N, float3 L, float3 V, float roughness)
{
	//ライト方向と、視点へのベクトルのハーフベクトル。
	float3 H = normalize(L + V);

	//散漫散乱とかいうやつの計算？
	//光の錯乱具合的な感じ？ブルームみたいな奴がでる。
	float energyBias = lerp(0.0f, 0.5f, roughness);

	//Disney拡散反射BRDFの特性上、1.0以上の値を返すことがあるので、
	//saturate(x) xの値を0～1でクランプ（制約）する。
	//表面法線とライトベクトルの間の角度を計算。
	float dotLH = saturate(dot(L, H));
	float dotNL = saturate(dot(N, L));
	//表面法線と視線ベクトルの間の角度を計算。
	float dotNV = saturate(dot(N, V));

	//フレネル反射率の計算。
	float Fd90 = energyBias + 2.0f * dotLH * dotLH * roughness;

	//フレネル反射をランバート反射に加える。
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

//BRDF(双方向反射率分布関数)
//param1 L = ライトの方向。
//param2 V = 視点へのベクトル。
//param3 N = 法線。
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