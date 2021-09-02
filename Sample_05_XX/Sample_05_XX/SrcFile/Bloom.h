#pragma once

#include "SrcFile/GaussianBlur.h"

class Bloom : Noncopyable
{
public:
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="mainRT"></param>
	void Init(RenderTarget& mainRT);
	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="rc"></param>
	/// <param name="mainRT"></param>
	void Render(RenderContext& rc, RenderTarget& mainRT);
	/// <summary>
	/// ポストエフェクトを実行した結果となるテクスチャ。
	/// </summary>
	/// <returns></returns>
	Texture& GetResultTexture()
	{
		return m_luminanceRenderTarget.GetRenderTargetTexture();
	}

private:
	RenderTarget m_luminanceRenderTarget;	//輝度抽出用RT。
	Sprite m_luminanceSprite;				//輝度抽出用Sprite。
	GaussianBlur m_gaussianBlur[4];			//ガウシアンブラー。
	Sprite m_finalSprite;					//最終合成スプライト。
	float m_clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
};

