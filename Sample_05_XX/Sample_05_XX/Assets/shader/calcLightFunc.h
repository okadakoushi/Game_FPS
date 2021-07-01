

static const float PI = 3.14159265358979323846;	

float SchlickFresnel(float u, float f0, float f90)
{
	//フリネルベース用のコード。
	//視点の位置によって明るさが変わるようになる。
	return f0 + (f90 + f0) * pow(1.0f - u, 5.0f);
}

/*
	正規化ディズニー拡散反射を求めていく。
	param1 法線。
	param2 ライトの位置。
	param3 視点の位置。
	param4 粗さ。
	return 正規化済みディズニー拡散反射。
*/
float NormalizedDisneyDiffuse(float3 N, float3 L, float3 V, float roughness)
{
	float3 H = normalize(L + V);

	float energyBias = lerp(0.0f, 0.5f, roughness);

	float dotLH = saturate(dot(L, H));
	//法線とライトの内積。
		//法線とライトが近ければ近いほど強く屈折光が乱反射するため、拡散反射光が大きくなる。
	float dotNL = saturate(dot(N, L));
	//法線と視点の内積。
	float dotNV = saturate(dot(N, V));

	float Fd90 = energyBias + 2.0f * dotLH * dotLH * roughness;

	float FL = SchlickFresnel(dotNL,1.0f, Fd90 );
	float FV = SchlickFresnel(dotNV,1.0f, Fd90 );
	return max( 0.2f, (FL * FV) );
}

/*
	サーフェイスの凹凸に応じて、
	鏡面反射の分散具合を求める。
*/
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

float3 BRDF(float3 L, float3 V, float3 N, float metaric)
{
	float microfacet = 0.3f;
	float f0 = metaric;
	// compute the half float3
	float3 H = normalize(L + V);

	float NdotH = max( 0.01f, dot(N, H) );
    float VdotH = max( 0.01f, dot(V, H) );
    float NdotL = max( 0.01f, dot(N, L) );
    float NdotV = max( 0.01f, dot(N, V) );

	float D = Beckmann(microfacet, NdotH);
	float F = spcFresnel(f0, VdotH);

	float G = min(1.0f, min(2*NdotH*NdotV/VdotH, 2*NdotH*NdotL/VdotH));
	float m = PI * NdotV * NdotH;

	return F * D * G / m; //max(F * D * G / m, 0.0);
}