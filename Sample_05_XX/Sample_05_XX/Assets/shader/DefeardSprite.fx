//////////////////////////////////////
///ディファード用スプライトシェーダー。
//////////////////////////////////////

#include "calcLightFunc.h"

static const int NUM_DIRECTIONAL_LIGHT = 4;       //ディレクションライト。
static const int NUM_SHADOW_MAP = 3;            //シャドウマップの数。

//転送されたGBuffer。
Texture2D<float4> g_texture : register(t0);	    //テクスチャ。
Texture2D<float4> g_normalMap : register(t1);   //法線。
Texture2D<float>  g_specularMap : register(t2); //スペキュラ。
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

cbuffer PostEffectCB : register(b10) {
	float mDamageEffectArea;
}

//頂点入力構造体。
struct VSInput{
	float4 pos      : POSITION; //モデルの頂点座標。
	float2 uv       : TEXCOORD0;//UV。
};
//ピクセル入力構造体。
struct PSInput{
	float4 pos      : SV_POSITION;
	float2 uv       : TEXCOORD0;
};

//頂点シェーダーのエントリーポイント。
PSInput VSMain(VSInput In) 
{
	PSInput psIn;
	psIn.pos = mul( mvp, In.pos );
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
	float metaric = g_specularMap.Sample(g_sampler, In.uv);

	float shadow = g_shadow.Sample(g_sampler, In.uv);

	//ランバート拡散反射と鏡面反射の計算をしていく。
	for(int ligNo = 0; ligNo < NUM_DIRECTIONAL_LIGHT; ligNo++){
		//Disney
		float NdotL = saturate( dot( normal, -directionalLight[ligNo].direction ));
	#if 1
		//ディズニーベースの拡散反射を求める。
		float disneyDiffuse = NormalizedDisneyDiffuse(normal, -directionalLight[ligNo].direction, toEye, 1.0f);
		//ディズニーベースの拡散反射をランバート拡散反射に乗算する。
		//ランバート拡散反射は、エネルギー量１に対してπ倍のエネルギーを放出してしまっているので
		//πで除算して正規化している。
		float3 diffuse = directionalLight[ligNo].color * NdotL * disneyDiffuse / PI ;
		//クックトランスモデルの鏡面反射を計算する。
		float3 spec = BRDF(-directionalLight[ligNo].direction, toEye, normal, metaric) * directionalLight[ligNo].color ;
	#endif
		//拡散反射光と鏡面反射光を線形補完。
		if (ligNo == 0) {
			//一本目のライト
			lig += lerp(diffuse, spec, metaric) * (1.0f - shadow);
		}
		else {
			lig += lerp(diffuse, spec, metaric);
		}
	}
	//環境光。
	lig += ambinentLight; //足し算するだけ
	//環境光による鏡面反射を計算する。
	//光が法線方向から入射していると考えて鏡面反射を計算する。
	lig += BRDF(normal, toEye, normal, metaric) * ambinentLight* metaric ;


    //テクスチャカラーをサンプリング。
	float4 texColor = g_texture.Sample(g_sampler, In.uv);
	//影を適用させる。
	texColor.xyz *= lig; 
	texColor *= mulColor;
	return float4(texColor.xyz, 1.0f);	
}
