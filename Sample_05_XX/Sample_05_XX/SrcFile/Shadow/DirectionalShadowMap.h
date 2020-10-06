#pragma once
#include "SrcFile/util/Noncopyable.h"
#include "Model.h"
#include "RenderTarget.h"

/// <summary>
/// カスケードシャドウマップ。
/// </summary>
/// <code>
/// --func call step--
/// 1.RegisterShadowCaster();
/// 2.Update();	要改善。
/// 3.RenderToShadowMap();
/// </code>
class DirectionalShadowMap : Noncopyable
{
public:
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="w">横の解像度。</param>
	/// <param name="h">縦の解像度。</param>
	/// <param name="lightHeight">太陽の高さ。</param>
	void Init(int w, int h, float lightHeight);
	/// <summary>
	/// 更新。
	/// </summary>
	void Update();
	/// <summary>
	/// ライトの位置を計算。
	/// </summary>
	/// <param name="lightHeight">ライトカメラの高さ。</param>
	/// <param name="viewFrustomCenterPosition">視錐台の中心。</param>
	Vector3 CalcLightPosition(float lightHeight, Vector3 viewFrustomCenterPosition);
	/// <summary>
	/// シャドウマップを描画
	/// </summary>
	/// <remarks>
	/// こいつ呼び出すと自動的にシャドウマップ用のRenderTargetに変更する。
	/// 描画が終わり次第、元の描画していたフレームバッファに戻ります。
	/// 後々、スキンモデルレンダーで勝手に呼ばれる処理にする。
	/// </remarks>
	/// <param name="rc"></param>
	void RenderToShadowMap();
	/// <summary>
	/// シャドウキャスターとして登録。
	/// </summary>
	/// <param name="model">登録するモデル。</param>
	void RegisterShadowCaster(Model* model)
	{
		m_shadowCasters.push_back(model);
		//シャドウキャスタのフラグ。
		model->SetShadwoCaster();
		//シャドウだすで！
		m_isEnable = true;
	}

private:
	static const int NUM_SHADOW_MAP = 3;			//カスケードシャドウマップの数。

	struct SShadowCb {
		Matrix mLVP[NUM_SHADOW_MAP];						//ライトビュープロジェクション。
		float shadowAreaDepthInViewSpace[NUM_SHADOW_MAP];	//カメラ空間での影を落とすエリアの深度テーブル。
	};
	RenderTarget m_shadowMaps[NUM_SHADOW_MAP];	//シャドウマップ。
	std::vector<Model*> m_shadowCasters;		//シャドウキャスター
	Matrix m_LVPMatrix[NUM_SHADOW_MAP];			//ライトビュープロジェクション行列。
	Matrix m_lightViewMatrix[NUM_SHADOW_MAP];	//ライトビュー行列
	Matrix m_lightProjMatirx[NUM_SHADOW_MAP];	//ライトプロジェクション行列
	SShadowCb m_shadowCBEntity;					//シャドウの値。
	ConstantBuffer m_shadowCB;					//シャドウの定数バッファ。
	Vector3 m_lightDirection = Vector3::Down;	//ライトの方向。
	float m_shadowAreas[NUM_SHADOW_MAP];		//影の落ちる範囲。
	float m_lightHeight;						//ライトの高さ。
	bool m_isEnable = false;					//シャドウマップが有効？
};

