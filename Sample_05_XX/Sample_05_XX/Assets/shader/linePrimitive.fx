/*!
 * @brief	ラインプリミティブ用のシェーダー。
 */

//頂点シェーダーへの入力頂点構造体。
struct VSInput {
	float4 pos : POSITION;
};
//頂点シェーダーの出力。
struct VSOutput {
	float4 pos : SV_POSITION;
};

cbuffer CB : register(b0) {
	float4x4 mView;
	float4x4 mProj;
};

//頂点。
VSOutput VSMain(VSInput In) : SV_POSITION
{
	VSOutput Out;
	float4 outPos = mul(mView, In.pos);
	outPos = mul(mProj, outPos);
	Out.pos = outPos;
	return Out;
}
//ピクセル。
float4 PSMain(VSOutput vsOut) : SV_Target0
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}