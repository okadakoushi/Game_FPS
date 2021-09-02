#pragma once
class GaussianBlur : public Noncopyable
{
public:
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="originalTexure">ガウシアンブラーを掛けるオリジナルテクスチャ。</param>
	/// <param name="isBlurAlpha"></param>
	/// <param name="isDownSample"></param>
	void Init(Texture* originalTexure, bool isBlurAlpha = false, bool isDownSample = true);
	/// <summary>
	/// ガウシアンブラーをGPU上で実行。
	/// <para>本関数呼び出しは、DirectX12を利用した描画コマンド生成中に呼び出し。</para>
	/// </summary>
	/// <param name="rc"></param>
	/// <param name="blurPower"></param>
	void ExecuteOnGPU(RenderContext& rc, float blurPower);
	/// <summary>
	/// ボケテクスチャを取得。
	/// </summary>
	/// <returns></returns>
	Texture& GetBokeTexture()
	{
		return m_yBlurRenderTarget.GetRenderTargetTexture();
	}
private:
	/// <summary>
	/// RT初期化。
	/// </summary>
	void InitRenderTarget(bool isDownSample);
	/// <summary>
	/// スプライト初期化。
	/// </summary>
	void InitSprite(bool isBlurAlpha);
	/// <summary>
	/// 重みを更新。
	/// </summary>
	/// <param name="blurPower"></param>
	void UpdateWeightsTable(float blurPower);
private:
	enum { NUM_WEIGHTS = 8 };				//重みの数。
	float m_weights[NUM_WEIGHTS];			//重みのテーブル。
	Texture* m_originalTexture = nullptr;	//オリジナルテクスチャ。
	RenderTarget m_xBlurRenderTarget;		//横ボケ。
	Sprite m_xBlurSprite;
	RenderTarget m_yBlurRenderTarget;		//縦ボケ。
	Sprite m_yBlurSprite;
};

