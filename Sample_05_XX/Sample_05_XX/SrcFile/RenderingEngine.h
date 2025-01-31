#pragma once

#include "Bloom.h"
#include "ToneMap.h"

/// <summary>
/// レンダリングエンジン。
/// </summary>
/// <remarks>
/// レンダリング関係の処理をまとめたクラス。
/// 作成中。
/// 現在はRTを保持、初期化のみ。todoとして描画順番もこのクラスで管理する。
/// </remarks>
class RenderingEngine : public Noncopyable
{
public:
	/// <summary>
	/// 初期化。
	/// </summary>
	void Init();
	/// <summary>
	/// メインRTをフレームバッファにコピーする。
	/// </summary>
	/// <param name="rc"></param>
	void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);
public:
	void ZPrePass(RenderContext& rc);
	void ToneMapRender();
	void BloomRender();
public:
	/// <summary>
	/// メインレンダーターゲットを取得。
	/// </summary>
	/// <returns></returns>
	RenderTarget& GetMainRenderTarget()
	{
		return m_mainRenderTarget;
	}
	/// <summary>
	/// ZPrePass用のレンダーターゲットを取得。
	/// </summary>
	/// <returns></returns>
	RenderTarget& GetZPrePassRenderTarget()
	{
		return m_zPrePassRT;
	}
	/// <summary>
	/// ブルームを取得。
	/// </summary>
	/// <returns></returns>
	Bloom& GetBloom()
	{
		return m_bloom;
	}
	/// <summary>
	/// トーンマップを取得。
	/// </summary>
	/// <returns></returns>
	ToneMap& GetToneMap()
	{
		return m_toneMap;
	}
public:
	/// <summary>
	/// レンダーを登録。
	/// </summary>
	/// <param name="render"></param>
	void RegistRender(SkinModelRender* render)
	{
		m_renders.push_back(render);
	}
	/// <summary>
	/// リストから除去。
	/// </summary>
	/// <param name="model"></param>
	void RemoveRender(SkinModelRender* model)
	{
		auto it = std::find(m_renders.begin(), m_renders.end(), model);
		if (it != m_renders.end()) {
			//見つかった。
			m_renders.erase(it);
		}
	}
	/// <summary>
	/// レンダーのリストを取得。
	/// </summary>
	/// <returns></returns>
	const std::vector<SkinModelRender*>& GetRenderList() const
	{
		return m_renders;
	}
private:
	RenderTarget m_zPrePassRT;						//zPrePass用RT。
	RenderTarget m_mainRenderTarget;				//メイン用RT。
	Sprite m_copyMainRtToFrameBufferSprite;			//メインRTからフレームバッファにコピーするスプライト。

	//postEffects//
	ToneMap		m_toneMap;							//トーンマップ。
	Bloom		m_bloom;							//ブルーム。
	
	std::vector<SkinModelRender*> m_renders;		//レンダーリスト。
};

