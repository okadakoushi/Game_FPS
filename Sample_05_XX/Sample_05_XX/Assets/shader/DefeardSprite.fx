//////////////////////////////////////
///ディファード用スプライトシェーダー。
//////////////////////////////////////

#include "calcLightFunc.h"

static const int NUM_DIRECTIONAL_LIGHT = 4;       //ディレクションライト。
static const int NUM_SHADOW_MAP = 3;            //シャドウマップの数。

//転送されたGBuffer。
Texture2D<float4> g_texture : register(t0);	    //テクスチャ。
Texture2D<float4> g_normalMap : register(t1);   //法線。
Texture2D<float> g_specularMap : register(t2); //スペキュラ。
Texture2D<float4> g_worldPos : register(t3);    //ワールド座標。
Texture2D<float4> g_shadow : register(t4);		//シャドウ。
//todo:shadowMap

sampler g_sampler : register(s0);               //サンプラステート。

//スプライト用定数バッファ。
cbuffer cb : register(b0){
	float4x4 mvp;		//ビュープロジェ。
	float4 mulColor;	//乗算カラー。
};

//ディレクションライト。
struct DirectionalLight{
    float3 direction;   //ライト方向。
	float4 color;       //ライトの色。
};
//ライト用定数バッファ。    
cbuffer LightCb : register(b1){
	DirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];
	float3 eyePos;					//カメラの視点。
	float specPow;					//スペキュラの絞り。
	float3 ambinentLight;			//環境光。
};

//シャドウ用の定数バッファ。
cbuffer ShadowCB : register(b2) {
	float4x4 mLVP[NUM_SHADOW_MAP];						//ライトビュープロジェクション。
	float3 shadowAreaDepthInViewSpace;					//カメラ空間での影を落とすエリア。
}

//頂点入力構造体。
struct VSInput{
	float4 pos      : POSITION; //モデルの頂点座標。
    float3 normal   : NORMAL;   //法線。
	float2 uv       : TEXCOORD0;//UV。
};
//ピクセル入力構造体。
struct PSInput{
	float4 pos      : SV_POSITION;
    float3 normal   : NORMAL;       //法線。
	float2 uv       : TEXCOORD0;
};

//頂点シェーダーのエントリーポイント。
PSInput VSMain(VSInput In) 
{
	PSInput psIn;
	psIn.pos = mul( mvp, In.pos );
    psIn.normal = In.normal;
	psIn.uv = In.uv;
	return psIn;
}

//ピクセルシェーダーエントリーポイント。
float4 PSMain( PSInput In ) : SV_Target0
{
    //ワールド座標。
    float4 posInWorld = g_worldPos.Sample(g_sampler, In.uv);
    //法線。
	float3 normal = g_normalMap.Sample(g_sampler, In.uv);
    //ランバート拡散反射。
	float3 lig = 0;
	//ワールド座標から視点に向かうベクトル。
	float3 toEye = normalize(eyePos - posInWorld);
	//反射の度合。
	float metaric = 0.0f;
	//テクスチャをサンプリング。
	float4 albedColor = g_texture.Sample(g_sampler, In.uv);

	//スペキュラの値を持ってくる。
	metaric = g_specularMap.Sample( g_sampler, In.uv);
	//ランバート拡散反射と鏡面反射の計算をしていく。
	for(int ligNo = 0; ligNo < NUM_DIRECTIONAL_LIGHT; ligNo++){
		//ライトの本数分計算。
		//法線とライトで内積の性質を利用して、ライトのベースカラーを計算する。
		//内積の性質...2つのベクトルが同じ向きなら1、反対になるほど-1に近づいていく。
		float3 baseColor = max(dot(normal, -directionalLight[ligNo].direction), 0.0f)
						*directionalLight[ligNo].color;
		//正規化済み、Disney拡散反射を加算。
		lig += NormalizedDisneyDiffuse(baseColor, normal, -directionalLight[ligNo].direction, toEye, 0.5f);
		//スペキュラ反射。
		lig += BRDF( -directionalLight[ligNo].direction, toEye, normal) * directionalLight[ligNo].color.xyz * metaric * directionalLight[ligNo].color.w;
	}
	//環境光。
	lig += ambinentLight; //足し算するだけ

	//シャドウ計算。
	float shadow = g_shadow.Sample(g_sampler, In.uv);
	lig *= lerp(1.0f, 0.5f, shadow);

    //テクスチャカラーをサンプリング。
	float4 texColor = g_texture.Sample(g_sampler, In.uv);
	//影を適用させる。
	texColor.xyz *= lig; 
	return float4(texColor.xyz, 1.0f);	
}
