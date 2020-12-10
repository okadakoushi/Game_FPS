#include "calcLightFunc.h"

//ランバート拡散反射サンプル00。
//拡散反射光のみを確認するためのサンプルです。

//変えたら変える。
static const int NUM_DIRECTIONAL_LIGHT = 4;	//ディレクションライトの本数。
static const int NUM_SHADOW_MAP = 3;		//シャドウマップの数。

//モデル用の定数バッファ
//変えたらMeshPartsの送ってる処理も変えてね！
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	int mShadowReciever;
};

//ディレクションライト。
struct DirectionalLight{
	float3 direction;	//ライトの方向。
	float4 color;		//ライトの色。
};
//ライト用の定数バッファ。
cbuffer LightCb : register(b1){
	DirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];
	float3 eyePos;					//カメラの視点。
	float specPow;					//スペキュラの絞り。
	float3 ambinentLight;			//環境光。
};
//マテリアル定数バッファ。
cbuffer MaterialParamCB :register (b3){
	int hasNormalMap;		//法線マップある？
	int hasSpecularMap;		//スペキュラマップある？
}
//頂点シェーダーへの入力。
struct SVSIn{
	float4	pos 		: POSITION;		//モデルの頂点座標。
	float3	normal		: NORMAL;		//法線。
	float3	Tangent		: TANGENT;		//正接線。
	float3	BiNormal	: BINORMAL;		//従法線。
	float2	uv 			: TEXCOORD0;	//UV座標。
	uint4	Indices		: BLENDINDICES0;//インデックスのサイズ。
	float4	Weights		: BLENDWEIGHT0;	//重み。
};
//スキンなし頂点シェーダー。
struct SVSInNonSkin {
	float4 pos		: POSITION;		//頂点座標。
	float3 normal	: NORMAL;		//法線。
	float3 Tangent	: TANGENT;		//正接線。
	float3 BiNormal	: BINORMAL;		//従法線。
	float2 uv 		: TEXCOORD0;	//uv座標。
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal		: NORMAL;		//法線。
	float3 Tangent		: TANGENT;		//正接線。
	float3 BiNormal		: BINORMAL;		//従法線。
	float2 uv 			: TEXCOORD0;	//uv座標。
	float3 worldPos		: TEXCOORD1;	//ワールド空間でのピクセルの座標。
	float4 posInWorld	: TEXCOORD2;	//ワールド座標。
	float4 posInview	: TEXCOORD3;	//ビュー座標
};

//シャドウ用の定数バッファ。
cbuffer ShadowCB : register(b2) {
	float4x4 mLVP[NUM_SHADOW_MAP];						//ライトビュープロジェクション。
	float3 shadowAreaDepthInViewSpace;					//カメラ空間での影を落とすエリア。
}
//シャドウ用ピクセルシェーダー入力。
struct SPSInShadow {
	float4 pos	: SV_POSITION;	//座標。
};

//変更したらMeshPartsのディスクリプタヒープのレジスタも変更すること。
//モデルテクスチャ。
Texture2D<float4> g_texture : register(t0);	
Texture2D<float4> g_normalMap : register(t1);
Texture2D<float4> g_specularMap : register(t2);
Texture2D<float4> ShadowMap0 : register(t15);	//1枚目
Texture2D<float4> ShadowMap1 : register(t16);	//2枚目
Texture2D<float4> ShadowMap2 : register(t17);	//3枚目
StructuredBuffer<float4x4> boneMatrix : register(t20); //ボーン行列 

//サンプラステート。
sampler g_sampler : register(s0);

/// <summary>
/// モデル用の頂点シェーダーのエントリーポイント。
/// アニメーションサンプル用にスキニングしてる。
/// </summary>
SPSIn VSMain(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;
	//スキン行列の計算。
	float4x4 skinning = 0;
	float w = 0.0f;
	for (int i = 0; i < 3; i++) {
		skinning += boneMatrix[vsIn.Indices[i]] * vsIn.Weights[i];
		w += vsIn.Weights[i];
	}
	skinning += boneMatrix[vsIn.Indices[3]] * (1.0f - w);

	psIn.pos = mul(skinning, vsIn.pos);						//モデルの頂点をワールド座標系に変換。
	psIn.posInWorld = psIn.pos;								//ワールド座標を保存しておく。
	psIn.pos = mul(mView, psIn.pos);						//ワールド座標系からカメラ座標系に変換。
	psIn.posInview = psIn.pos;								//ビュー座標を保存しておく。
	psIn.pos = mul(mProj, psIn.pos);						//カメラ座標系からスクリーン座標系に変換。
	psIn.normal = normalize(mul(skinning , vsIn.normal));			//法線をワールド座標系に変換。
	psIn.Tangent = normalize(mul(skinning, vsIn.Tangent));			//接線を計算。
	psIn.BiNormal = normalize(mul(skinning , vsIn.BiNormal));		//従法線を計算。
	psIn.uv = vsIn.uv;

	return psIn;
}

/// <summary>
/// モデル用スキンなしシェーダー。
/// </summary>
SPSIn VSMainNonSkin(SVSInNonSkin vsIn)
{
	SPSIn psIn;
	psIn.worldPos = 0;					//なんやこれは！todo: delete
	psIn.pos = mul(mWorld, vsIn.pos);	//１：モデルの頂点をワールド座標系に変換。	
	psIn.posInWorld = psIn.pos;			//ワールド座標。
	psIn.pos = mul(mView, psIn.pos);	//２：ワールド座標系からカメラ座標系に変換。
	psIn.posInview = psIn.pos;			//ビュー座標
	psIn.pos = mul(mProj, psIn.pos);	//３：カメラ座標系からスクリーン座標系に変換。
	psIn.normal = normalize(mul(mWorld , vsIn.normal));			//法線をワールド座標系に変換。
	psIn.Tangent = normalize(mul(mWorld , vsIn.Tangent));			//接線を計算。
	psIn.BiNormal = normalize(mul(mWorld , vsIn.BiNormal));		//従法線を計算。
	psIn.uv = vsIn.uv;
	return psIn;
}

//カスケードのインデックスを計算して取得。
int GetCascadeIndex(float zInView)
{
	for (int i = 0; i < NUM_SHADOW_MAP; i++) {
		if (zInView < shadowAreaDepthInViewSpace[i]) {
			return i;
		}
	}
	return 0;
}

//シャドウが落ちるかどうか計算。
float CalcShadowPercent(Texture2D<float4> tex, float2 uv, float depth)
{
	//シャドウマップの深度情報
	float shadow_val = tex.Sample(g_sampler, uv).r;
	//return shadow_val;
	//深度テスト
	if (depth > shadow_val.r + 0.01f) {
		//手前にあるのでシャドウを落とす。
		return 1.0f;
	}
	return 0.0f;	
}

//シャドウ計算
float CalcShadow(float3 wp, float zInView)
{
	//1.0fだった場合シャドウが落ちる。
	float Shadow = 0;
	//シャドウマップの番号。
	int MapNum = 0;

	//まず使用するシャドウマップの番号を取得する。
	MapNum = GetCascadeIndex(zInView);

	//モデルの座標をライトのLVPでライトカメラ軸に変換する。
	float4 posInLVP = mul(mLVP[MapNum], float4(wp, 1.0f));
	//ライト座標系での深度値を計算。
	posInLVP.xyz /= posInLVP.w;
	//深度値取得。
	float depth = posInLVP.z;
	//UV座標に変換。
	float2 shadowMapUV = float2(0.5f, -0.5f) * posInLVP.xy + float2(0.5f, 0.5f);

	{
		//どのシャドウマップの深度情報をとるのか識別。
		if (MapNum == 0) {
			//0番目の深度情報で計算。
			Shadow = CalcShadowPercent(ShadowMap0, shadowMapUV, depth);
		}
		else if (MapNum == 1) {
			//1番目の深度情報で計算。
			Shadow = CalcShadowPercent(ShadowMap1, shadowMapUV, depth);
		}
		else if (MapNum == 2) {
			//2番目の深度情報で計算。
			Shadow = CalcShadowPercent(ShadowMap2, shadowMapUV, depth);
		}
	}
	return Shadow;
}
/// <summary>
/// モデル用のピクセルシェーダーのエントリーポイント
/// </summary>
float4 PSMain( SPSIn psIn ) : SV_Target0
{
	//法線計算。
	float3 normal = psIn.normal;
	if(hasNormalMap){
		//法線マップがある。
		//法線マップから法線を持ってくる。
		float3 binSpaceNormal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;
		//UV座標に変換。
		binSpaceNormal = (binSpaceNormal * 2.0f) -1.0f;
		//X方向は接線。
		//Y方向は従法線。
		//Z方向は法線。
		normal = psIn.Tangent * binSpaceNormal.x
			+ psIn.BiNormal * binSpaceNormal.y
			+ psIn.normal * binSpaceNormal.z;
	}else{
		//法線マップがない。
		normal = psIn.normal;
	}
	//ランバート拡散反射。
	float3 lig = 0;
	//ワールド座標から視点に向かうベクトル。
	float3 toEye = normalize(eyePos - psIn.posInWorld);
	//反射の度合。
	float metaric = 0.0f;
	//テクスチャをサンプリング。
	float4 albedColor = g_texture.Sample(g_sampler, psIn.uv);
	
	//ランバート拡散反射と鏡面反射の計算をしていく。
	if(hasSpecularMap){
		//スペキュラマップがある。
		//絞りの値をマップからとってくる。
		metaric = g_specularMap.Sample( g_sampler, psIn.uv).a;
	}
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

	//シャドウの計算。
	if (mShadowReciever == 1) {
		//シャドウレシーバーなのでシャドウが落ちるかの計算。
		//影を落とすかどうかの計算をしていく。
		float Shadow = CalcShadow(psIn.posInWorld, psIn.posInview.z);
		//Shadowの値が0.0fなら0.5f, 1.0fなら1.0。
		//GPU処理でのif文削減。
		lig *= lerp(1.0f, 0.5f, Shadow);
	}
	//テクスチャカラーをサンプリング。
	float4 texColor = g_texture.Sample(g_sampler, psIn.uv);
	//影を適用させる。
	texColor.xyz *= lig; 
	return float4(texColor.xyz, 1.0f);	
}

/*
	スキンありシャドウマップ生成用の頂点シェーダー。
*/
SPSInShadow VSMain_ShadowMapSkin(SVSIn vsIn)
{
	//どのピクセルシェーダに返すか。
	SPSInShadow psInput = (SPSInShadow)0;

	//スキン行列の計算。
	float4x4 skinning = 0;
	float w = 0.0f;
	float4 pos = 0;
	for (int i = 0; i < 3; i++) {
		skinning += boneMatrix[vsIn.Indices[i]] * vsIn.Weights[i];
		w += vsIn.Weights[i];
	}
	skinning += boneMatrix[vsIn.Indices[3]] * (1.0f - w);

	pos = mul(skinning, vsIn.pos);						//モデルの頂点をワールド座標系に変換。
	pos = mul(mView, pos);						//ワールド座標系からカメラ座標系に変換。
	pos = mul(mProj, pos);						//カメラ座標系からスクリーン座標系に変換。77
	psInput.pos = pos;
	return psInput;
}

/*
	スキンなしシャドウ描画。
*/
SPSInShadow VSMain_ShadowMapNonSkin(SVSIn vsIn)
{
	//どのピクセルシェーダに返すか。
	SPSInShadow psInput = (SPSInShadow)0;

	float4 pos = 0;
	pos = mul(mWorld, vsIn.pos);				//モデルの頂点をワールド座標系に変換。
	pos = mul(mView, pos);						//ワールド座標系からカメラ座標系に変換。
	pos = mul(mProj, pos);						//カメラ座標系からスクリーン座標系に変換。77
	psInput.pos = pos;
	return psInput;
}

/*
	ピクセルシェーダーのエントリ関数。
*/
float4 PSMain_ShadowMap(SPSInShadow In) : SV_Target0
{
	//射影空間でのZ値を返す。
	float z = In.pos.z / In.pos.w;
	return z;
}
