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

#include "Sprite.h"

class SkyBox;

/// <summary>
/// GBufferRender。
/// GBuffer書き込み用クラス。
/// </summary>
/// <code>
/// ModelRender Reff.
/// </code>
class DefferdRender : Noncopyable
{
public:
	DefferdRender() {};
	~DefferdRender();

	/// <summary>
	/// 初期化。
	/// </summary>
	void Init();
	/// <summary>
	/// スプライトの初期化。
	/// </summary>
	void SpriteInit();
	/// <summary>
	/// 描画。
	/// </summary>
	void Render(RenderContext& rc, const Matrix& view, const Matrix& proj, bool Clear = true);
	/// <summary>
	/// 描画。
	/// </summary>
	void DeffardRender(RenderContext& rc, const Matrix& view, const Matrix& proj);
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
	/// <summary>
	/// Gbufferのデプスを返却。
	/// </summary>
	/// <returns></returns>
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetDSV() const 
	{
		return m_GBuffers[GBuffer_albed].GetDSVCpuDescriptorHandle();
	}
	/// <summary>
	/// ディファードスプライトを取得。
	/// </summary>
	/// <returns></returns>
	Sprite& GetDefferdSprite()
	{
		return m_defferdSprite;
	}
	/// <summary>
	/// ダメージエリアを設定。
	/// </summary>
	/// <param name="area"></param>
	void SetDamageArea(float& area)
	{
		m_postEffectEntity.DamageArea = area;
	}
private:
	/// <summary>
	/// ポストエフェクト的パラメーター。
	/// </summary>
	struct PostEffectEntity {
		float DamageArea = 0.0f;
	};
private:
	std::vector<SkinModelRender*> m_models;			//モデルのリスト。
	RenderTarget m_GBuffers[Gbuffer_Num];			//GBuffer。
	Sprite m_defferdSprite;
	SkyBox* m_skyBox = nullptr;
	bool m_isInited = false;						//初期化フラグ。
	PostEffectEntity m_postEffectEntity;			//ポストエフェクトエンティティ。

};

