/*!
 * @brief ブルーム
 */

cbuffer cb : register(b0)
{
    float4x4 mvp;       // MVP行列
    float4 mulColor;    // 乗算カラー
};

struct VSInput
{
    float4 pos : POSITION;
    float2 uv  : TEXCOORD0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};

/*!
 * @brief 頂点シェーダー
 */
PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}

Texture2D<float4> mainRenderTargetTexture : register(t0); // メインレンダリングターゲットのテクスチャ
sampler Sampler : register(s0);

/////////////////////////////////////////////////////////
// 輝度抽出用
/////////////////////////////////////////////////////////
// RGBからHSVのV(輝度)を求める
float Rgb2V( float3 rgb)
{
    return max(rgb.r, max(rgb.g, rgb.b));
}
/*!
 * @brief 輝度抽出用のピクセルシェーダー
 */
float4 PSSamplingLuminance(PSInput In) : SV_Target0
{
    // メインレンダリングターゲットからカラーをサンプリング
    float4 color = mainRenderTargetTexture.Sample(Sampler, In.uv);

    // サンプリングしたカラーの明るさを計算
    float t = Rgb2V(color.xyz);//dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f));

    // clip()関数は引数の値がマイナスになると、以降の処理をスキップする
    // なので、マイナスになるとピクセルカラーは出力されない
    // 今回の実装はカラーの明るさが1以下ならピクセルキルする
    clip(t - 0.9f);

    return color;
}

//ボケ画像。
Texture2D<float4> g_bokeTexture_0 : register(t0);
Texture2D<float4> g_bokeTexture_1 : register(t1);
Texture2D<float4> g_bokeTexture_2 : register(t2);
Texture2D<float4> g_bokeTexture_3 : register(t3);

float4 PSBloomFinal(PSInput In) : SV_Target0
{
    //サンプリング、平均を取って出力。
    float4 combineColor = g_bokeTexture_0.Sample(Sampler, In.uv);
    combineColor += g_bokeTexture_1.Sample(Sampler, In.uv);
    combineColor += g_bokeTexture_2.Sample(Sampler, In.uv);
    combineColor += g_bokeTexture_3.Sample(Sampler, In.uv);
    combineColor /= 4.0f;
    combineColor.a = 1.0f;
    return combineColor;
}
