//////////////////////////////////////
///ディファード用モデルシェーダー。
//////////////////////////////////////

//サンプラステート。
sampler g_sampler : register(s0);

Texture2D<float4> g_texture : register(t0);	    //テクスチャ。
Texture2D<float4> g_normalMap : register(t1);   //法線。
Texture2D<float>  g_specularMap : register(t2);	//スペキュラ。
Texture2D<float4> ShadowMap0 : register(t15);	//1枚目
Texture2D<float4> ShadowMap1 : register(t16);	//2枚目
Texture2D<float4> ShadowMap2 : register(t17);	//3枚目
//Texture2D<float4> MINIMAP : register(t18);		//ミニマップ。

//ボーン行列 
StructuredBuffer<float4x4> boneMatrix : register(t20); 

static const int NUM_SHADOW_MAP = 3;            //シャドウマップの数。

//モデル用の定数バッファ
//変えたらMeshPartsの送ってる処理も変えてね！
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	float4 mulColor;
	int mShadowReciever;
};

//スキンあり頂点シェーダー入力。
struct SVSIn{
    float4	pos 		: POSITION;		//モデルの頂点座標。
    float4	Weights		: BLENDWEIGHT0;	//重み。
	float3  normal		: NORMAL;		//法線。
	float3	Tangent		: TANGENT;		//正接線。
	float3	BiNormal	: BINORMAL;		//従法線。
    float2	uv 			: TEXCOORD0;	//UV座標。
	uint4	Indices		: BLENDINDICES0;//インデックスのサイズ。
};

//スキンなし頂点シェーダー入力。
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
	float4 normal		: NORMAL;		//法線。
	float4 Tangent		: TANGENT;		//正接線。
	float4 BiNormal		: BINORMAL;		//従法線。
	float2 uv 			: TEXCOORD0;	//uv座標。
	float4 worldPos		: TEXCOORD1;	//ワールド座標。
	float4 posInView	: TEXCOORD2;	//ビュー座標。
};
//ピクセルシェーダー出力。
struct SPSOut{
	float4 albedo		: SV_Target0;	//アルベド。
	float4 worldPos		: SV_Target3;	//ワールド座標。
	float4 normal		: SV_Target1;	//法線。
	float spec			: SV_Target2;	//スペキュラ反射。
	float Shadow		: SV_Target4;	//Z値。
};

struct SPSInShadow {
	float4 pos	: SV_POSITION;	//座標。
};

//マテリアル定数バッファ。
cbuffer MaterialParamCB :register (b3) {
	int hasNormalMap;		//法線マップある？
	int hasSpecularMap;		//スペキュラマップある？
}

//シャドウ用の定数バッファ。
cbuffer ShadowCB : register(b2) {
	float4x4 mLVP[NUM_SHADOW_MAP];						//ライトビュープロジェクション。
	float3 shadowAreaDepthInViewSpace;					//カメラ空間での影を落とすエリア。
}

//頂点シェーダーエントリーポイント。
SPSIn VSMain(SVSIn vsIn)
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
    //スクリーン行列に変換。
    psIn.pos = mul(skinning, vsIn.pos);						//モデルの頂点をワールド座標系に変換。
	psIn.worldPos = psIn.pos;								//ワールド座標保存。
	psIn.pos = mul(mView, psIn.pos);						//ワールド座標系からカメラ座標系に変換。
	psIn.posInView = psIn.pos;									//ビュー座標保存。
	psIn.pos = mul(mProj, psIn.pos);						//カメラ座標系からスクリーン座標系に変換。
	psIn.uv = vsIn.uv;
	psIn.normal = normalize(mul(skinning, vsIn.normal));			//法線をワールド座標系に変換。
	psIn.Tangent = normalize(mul(skinning, vsIn.Tangent));			//接線を計算。
	psIn.BiNormal = normalize(mul(skinning, vsIn.BiNormal));		//従法線を計算。

	return psIn;
}

SPSIn VSMainNonSkin(SVSInNonSkin vsIn)
{
	SPSIn psIn;

	//スクリーン行列に変換。
	psIn.pos = mul(mWorld, vsIn.pos);						//モデルの頂点をワールド座標系に変換。
	psIn.worldPos = psIn.pos;								//ワールド座標保存。
	psIn.pos = mul(mView, psIn.pos);						//ワールド座標系からカメラ座標系に変換。
	psIn.posInView = psIn.pos;								//ビュー座標保存。
	psIn.pos = mul(mProj, psIn.pos);						//カメラ座標系からスクリーン座標系に変換。
	psIn.normal = normalize(mul(mWorld, vsIn.normal));		//法線をワールド座標系に変換。
	psIn.Tangent = normalize(mul(mWorld, vsIn.Tangent));	//接線を計算。
	psIn.BiNormal = normalize(mul(mWorld, vsIn.BiNormal));	//従法線を計算。
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
	if (depth > shadow_val.r + 0.001f) {
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

float4 GetNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
	float4 binSpaceNormal = g_normalMap.SampleLevel(g_sampler, uv, 0.0f);
    binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;
    
	float4 newNormal = float4(tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z, 0.0f); 

    return newNormal;
}


SPSOut PSMain(SPSIn psIn)
{
	//GBufferに出力。
	SPSOut psOut;
	//アルベド出力。
	psOut.albedo = g_texture.Sample(g_sampler, psIn.uv);
	psOut.albedo *= mulColor;
	//法線出力。色々あってSpriteするときに計算させるのというほど
	//速度変わらない気もするからPBRの法線はここで計算。
	if (hasNormalMap) {
		//法線を計算。
		//psOut.normal = GetNormal( psIn.normal, psIn.Tangent, psIn.BiNormal, psIn.uv);
		psOut.normal = psIn.normal;
	}else{
		//ない。
		psOut.normal = psIn.normal;
	}
	//ワールド座標出力。
	psOut.worldPos = psIn.worldPos;
	//スペキュラ出力。
	if (hasSpecularMap) {
		psOut.spec = 0;//g_specularMap.Sample(g_sampler, psIn.uv);	
	}
	else {
		psOut.spec = 0;
	}
	if (mShadowReciever) {
		//シャドウ出力。
		psOut.Shadow = 0;//CalcShadow(psIn.worldPos, psIn.posInView.z);
	}
	else {
		psOut.Shadow = 0;
	}

	//GBufferを転送。
	return psOut;	
}
