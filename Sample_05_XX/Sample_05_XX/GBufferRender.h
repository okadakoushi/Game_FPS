#pragma once

//GBuffer定義。
enum EnGBuffer {
	GBuffer_albed,		//アルベド。
	GBuffer_normal,		//法線。
	GBuffer_spec,		//スペキュラ。
	GBuffer_worldPos,	//ワールド座標。
	GBuffer_Shadow,		//シャドウ。
	Gbuffer_Num			//GBufferの数。	
};

/// <summary>
/// GBufferRender。
/// GBuffer書き込み用クラス。
/// </summary>
/// <code>
/// ModelRender Reff.
/// </code>
class GBufferRender : Noncopyable
{
public:
	GBufferRender() {};
	~GBufferRender();

	/// <summary>
	/// 初期化。
	/// </summary>
	void Init();
	/// <summary>
	/// 描画。
	/// </summary>
	void Render(RenderContext& rc);
	/// <summary>
	/// モデルを登録。
	/// <para>Dont call from User.</para>
	/// </summary>
	/// <param name="model">モデル。</param>
	void AddModel(SkinModelRender* model)
	{
		m_models.push_back(model);
	}
	/// <summary>
	/// モデルをリストから削除。
	/// <para>Dont call from User.</para>
	/// </summary>
	/// <param name="model">モデル。</param>
	void RemoveModel(SkinModelRender* model)
	{
		auto it = std::find(m_models.begin(), m_models.end(), model);
		if (it != m_models.end()) {
			//見つかった。
			m_models.erase(it);
		}
	}
	/// <summary>
	///	GBufferのテクスチャを取得する。
	/// </summary>
	/// <param name="gbufferNum">Gbufferの番号。</param>
	/// <returns>指定したGbuffer。</returns>
	Texture& GetTexture(const EnGBuffer& gbufferNum)
	{
		return m_GBuffers[gbufferNum].GetRenderTargetTexture();
	}

private:
	std::vector<SkinModelRender*> m_models;			//モデルのリスト。
	RenderTarget m_GBuffers[Gbuffer_Num];			//GBuffer。
};

